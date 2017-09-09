
#include "../atrshmlog_internal.h"

#include <string.h>


/*******************************************************************/

/**
 * \file atrshmlogimpl_read.c
 */


/** 
 * \n Main code:
 *
 * \brief We read a buffer and write it to a local memory area.
 *
 * We can read half full, full and writeable buffers.
 *
 * The latter is needed for the cleanup after finish of the 
 * buffers so far not cleared and only partly full.
 *
 * This was the  pthread version.
 *
 *  This is done in a safe way. so we can do this in parallel
 * to running writers.
 *
 * And we need to do it. 

 * If all buffers are full the writers have 
 * to loop forever. 
 *
 * So we have to use the mt safe way here.
 *
 * Thats a bit a blockade of the system, but if the number of
 * buffers is sufficient we dont disturb the writers much..
 *
 * They check from outside of the lock.
 * so we can simply lock, switch the flag to dont disturb me mode
 * and clenaup the buffer.
 *
 * Anything else is up to the writer process. 
 * So try to handle the data then as fast as you can.
 * See for a simple reader the
 * atrshmlogreader.c
 *
 * test t_read.c
 */
atrshmlog_ret_t atrshmlog_read(volatile const void* const i_area,
			       const atrshmlog_int32_t i_index_buffer,
			       void * const o_target,
			       atrshmlog_int32_t * const o_target_length,
			       atrshmlog_pid_t* const o_pid,
			       atrshmlog_tid_t* const o_tid,
			       atrshmlog_internal_time_t* const o_inittime,
			       atrshmlog_time_t* const o_inittimetsc_before,
			       atrshmlog_time_t* const o_inittimetsc_after,
			       atrshmlog_internal_time_t* const o_lasttime,
			       atrshmlog_time_t* const o_lasttimetsc_before,
			       atrshmlog_time_t* const o_lasttimetsc_after,
			       atrshmlog_time_t* const o_difftimetransfer,
			       atrshmlog_time_t* const o_starttransfer,
			       atrshmlog_time_t* const o_acquiretime,
			       atrshmlog_int32_t* const o_id,
			       atrshmlog_int32_t* const o_number_dispatched,
			       atrshmlog_int32_t* const o_counter_write0,
			       atrshmlog_int32_t* const o_counter_write0_discard,
			       atrshmlog_int32_t* const o_counter_write0_wait,
			       atrshmlog_int32_t* const o_counter_write0_adaptive,
			       atrshmlog_int32_t* const o_counter_write0_adaptive_fast,
			       atrshmlog_int32_t* const o_counter_write0_adaptive_very_fast,
			       atrshmlog_int32_t* const o_counter_write1,
			       atrshmlog_int32_t* const o_counter_write1_discard,
			       atrshmlog_int32_t* const o_counter_write1_wait,
			       atrshmlog_int32_t* const o_counter_write1_adaptive,
			       atrshmlog_int32_t* const o_counter_write1_adaptive_fast,
			       atrshmlog_int32_t* const o_counter_write1_adaptive_very_fast,
			       atrshmlog_int32_t* const o_counter_write2,
			       atrshmlog_int32_t* const o_counter_write2_discard,
			       atrshmlog_int32_t* const o_counter_write2_wait,
			       atrshmlog_int32_t* const o_counter_write2_adaptive,
			       atrshmlog_int32_t* const o_counter_write2_adaptive_fast,
			       atrshmlog_int32_t* const o_counter_write2_adaptive_very_fast
			       )
{
  ATRSHMLOGSTAT(atrshmlog_counter_read);

  *o_target_length = 0;

  atrshmlog_time_t start;
  atrshmlog_time_t end;
  
  int result = -1;

  if(i_area == 0)
    return atrshmlog_error_read_1;

  // we do here a terrible thing.
  // we cast const away.
  // but its ok here.
  volatile atrshmlog_area_t * restrict i_shm = (volatile atrshmlog_area_t*)i_area;

  /* Ok. We can now do the transfer */

  atrshmlog_g_tl_t* g  = (atrshmlog_g_tl_t* )atrshmlog_get_thread_locals_adress();

#if ATRSHMLOG_THREAD_LOCAL == 0

  // this can happen if pthread specific is used
  if (g == NULL)
    {
      return atrshmlog_error_read_6;
    }

#endif
  
  if (g->atrshmlog_idnotok)
    {
      int tlret = atrshmlog_init_thread_local (g);
      
      /* From now on we are the not ok flag */
      if (tlret)
	return tlret;
    }
  
  if (i_index_buffer < 0)
    return atrshmlog_error_read_2;
  
  if (i_shm->shmcount <= i_index_buffer)
    return atrshmlog_error_read_3;

  if (i_shm->shmsafeguard != ATRSHMLOGSAFEGUARDVALUE)
    return atrshmlog_error_read_4;

  volatile atrshmlog_buffer_t* restrict b = &(i_shm->logbuffers[i_index_buffer]);

  int lock_holds = atrshmlog_full;

  int lock_wanted = atrshmlog_illegal;

  _Bool atomic_lock_result;


  atomic_lock_result
    = atomic_compare_exchange_weak_explicit(&(b->state),
					      &lock_holds,
					      lock_wanted,
					      memory_order_relaxed,
					      memory_order_relaxed);

  if (atomic_lock_result != 1)
    {
      return atrshmlog_error_ok; // o_length is 0, so this is an ok case
    }


  if (atrshmlog_thread_fence_3)
    atomic_thread_fence(memory_order_acquire);
  
  int state = atomic_load_explicit(&(b->state), memory_order_acquire);

  size_t size = b->shmsize;

  // We must use here the theoretical maximum size. period.
  if (size > ATRSHMLOGBUFFER_INFOSIZE)
    {
      result = atrshmlog_error_read_5;
      
      /* This is bad. Someone has spoiled the buffer */
      goto ende;
    }

  /* Ok. We are clear to go to copy */
  
  /* Is the length ok for copy */ 

  if (size > 0)
    {
      ATRSHMLOGSTAT(atrshmlog_counter_read_doit);

      /* We have to do the copy now */

      start = ATRSHMLOG_GET_TSC_CALL();

      char* source = (char*)i_shm + b->info;
      
      #if 1
      (void)memcpy(o_target, source, size );
      #else
	      for (int zippo = 0; zippo < size; zippo++)
		((char*)o_target)[zippo] = source[zippo];
      
      #endif

      *o_pid = b->pid;
      *o_tid = b->tid;
      *o_inittime = b->inittime;
      *o_inittimetsc_before = b->inittimetsc_before;
      *o_inittimetsc_after = b->inittimetsc_after;
      *o_lasttime = b->lasttime;
      *o_lasttimetsc_before = b->lasttimetsc_before;
      *o_lasttimetsc_after = b->lasttimetsc_after;
      *o_target_length = size;
      *o_starttransfer = b->starttransfer;
      *o_acquiretime = b->acquiretime;
      *o_id = b->id;
      
      end = ATRSHMLOG_GET_TSC_CALL();

      *o_difftimetransfer = (end - start);

    }

  result = atrshmlog_error_ok;

 ende:
  b->shmsize = 0;

  atomic_store_explicit(&b->state, atrshmlog_writeable, memory_order_release);

  return result;
}
