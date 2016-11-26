
#include "../atrshmlog_internal.h"


/*******************************************************************/

/**
 * \file atrshmlogimpl_read_fetch.c
 */

/* 
 * \n Main code:
 *
 * \brief This is the actual reader function now.
 * 
 * It transfers the top buffer of the f list in shm.
 * 
 * So if there are buffers it delivers a transfered buffer and puts the 
 * shm buffer on the a list again.
 * 
 *  The memory is transfered into the reader process.
 *
 * so the reader then has to handle it.
 *
 * test t_read_fetch.c
 */
atrshmlog_ret_t atrshmlog_read_fetch(volatile const void* i_area,
				     atrshmlog_int32_t *o_index_buffer,
				     void *o_target,
				     atrshmlog_int32_t *o_target_length,
				     atrshmlog_pid_t* o_pid,
				     atrshmlog_tid_t* o_tid,
				     atrshmlog_internal_time_t* o_inittime,
				     atrshmlog_time_t* o_inittimetsc_before,
				     atrshmlog_time_t* o_inittimetsc_after,
				     atrshmlog_internal_time_t* o_lasttime,
				     atrshmlog_time_t* o_lasttimetsc_before,
				     atrshmlog_time_t* o_lasttimetsc_after,
				     atrshmlog_time_t* o_difftimetransfer,
				     atrshmlog_time_t* o_starttransfer,
				     atrshmlog_time_t* o_acquiretime,
				     atrshmlog_int32_t* o_id,
				     atrshmlog_int32_t* o_number_dispatched,
				     atrshmlog_int32_t* o_counter_write0,
				     atrshmlog_int32_t* o_counter_write0_discard,
				     atrshmlog_int32_t* o_counter_write0_wait,
				     atrshmlog_int32_t* o_counter_write0_adaptive,
				     atrshmlog_int32_t* o_counter_write0_adaptive_fast,
				     atrshmlog_int32_t* o_counter_write0_adaptive_very_fast,
				     atrshmlog_int32_t* o_counter_write1,
				     atrshmlog_int32_t* o_counter_write1_discard,
				     atrshmlog_int32_t* o_counter_write1_wait,
				     atrshmlog_int32_t* o_counter_write1_adaptive,
				     atrshmlog_int32_t* o_counter_write1_adaptive_fast,
				     atrshmlog_int32_t* o_counter_write1_adaptive_very_fast,
				     atrshmlog_int32_t* o_counter_write2,
				     atrshmlog_int32_t* o_counter_write2_discard,
				     atrshmlog_int32_t* o_counter_write2_wait,
				     atrshmlog_int32_t* o_counter_write2_adaptive,
				     atrshmlog_int32_t* o_counter_write2_adaptive_fast,
				     atrshmlog_int32_t* o_counter_write2_adaptive_very_fast
				     )
{
  ATRSHMLOGSTAT(atrshmlog_counter_read_fetch);

  *o_target_length = 0;

  atrshmlog_time_t start;
  atrshmlog_time_t end;
  
  int result = -1;

  if(i_area == 0)
    return atrshmlog_error_read_fetch_1;

  // we do here a terrible thing.
  // we cast const away.
  // but its ok here.
  volatile atrshmlog_area_t * restrict i_shm = (volatile atrshmlog_area_t*)i_area;

  /* Ok. We can now do the transfer */

  atrshmlog_g_tl_t* g  = (atrshmlog_g_tl_t* )atrshmlog_get_thread_locals_adress();
  
#if ATRSHMLOG_THREAD_LOCAL == 0

  // this can happen if thread specific is in
  if (g == NULL)
    return atrshmlog_error_read_fetch_6;

#endif
  
  if (g->atrshmlog_idnotok)
    {
      int tlret = atrshmlog_init_thread_local (g);
      
      /* From now on we are the not ok flag */
      if (tlret)
	return tlret;
    }
  
  if (i_shm->shmsafeguard != ATRSHMLOGSAFEGUARDVALUE)
    return atrshmlog_error_read_fetch_2;

  /* We pop from list */
  int isfull = atomic_load_explicit(&i_shm->shmf, memory_order_consume);

  if (isfull == -1)
    return atrshmlog_error_read_fetch_3; /* No buffer on list */

  while (isfull > -1
	 && !atomic_compare_exchange_weak_explicit(&i_shm->shmf,
						   &isfull,
						   i_shm->logbuffers[isfull].next_full,
						   memory_order_acq_rel,
						   memory_order_relaxed))
    ;

  if (isfull == -1)
    return atrshmlog_error_read_fetch_3; /* No buffer on list */
  
  volatile atrshmlog_buffer_t* restrict b = &(i_shm->logbuffers[isfull]);

  *o_index_buffer = isfull;

  if (atrshmlog_thread_fence_4)
    atomic_thread_fence(memory_order_acquire);

  
  int state = atomic_load_explicit(&(b->state), memory_order_acquire);


  size_t size = b->shmsize;

  // We must use here the theoretical maximum size. period.
  if (size > ATRSHMLOGBUFFER_INFOSIZE)
    {
      result = atrshmlog_error_read_fetch_4;
      
      /* This is bad. Someone has spoiled the buffer */
      goto ende;
    }

  /* Ok. We are clear to go to copy */
  
  /* Is the length ok for copy */ 

  result = atrshmlog_error_read_fetch_5;
  
  if (size > 0)
    {
      ATRSHMLOGSTAT(atrshmlog_counter_read_fetch_doit);

      /* We have to do the copy now */

      start = ATRSHMLOG_GET_TSC_CALL();

      char* restrict source = (char*)i_shm + b->info;

      int old_chksum = b->chksum;
      
#if 1
      (void)memcpy(o_target, source, size );
#else
      for (int zippo = 0; zippo < size; zippo++)
	((char*)o_target)[zippo] = source[zippo];
      
#endif

      // we calc the checksum trivial. but this is ok for our needs
      if (atrshmlog_checksum)
	{
	  int chksum = 0;
	  
	  for (int k = 0; k < size; k++)
	    {
	      chksum += ((char*)o_target)[k];
	    }
	  
	  if (old_chksum != chksum)
	    {
	      ATRSHMLOGSTAT(atrshmlog_counter_fence_alarm_2);
	    }
	}
  
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
      *o_number_dispatched = b->number_dispatched;
      *o_counter_write0 = b->counter_write0;
      *o_counter_write0_discard = b->counter_write0_discard;
      *o_counter_write0_wait = b->counter_write0_wait;
      *o_counter_write0_adaptive = b->counter_write0_adaptive;
      *o_counter_write0_adaptive_fast = b->counter_write0_adaptive_fast;
      *o_counter_write0_adaptive_very_fast = b->counter_write0_adaptive_very_fast;
      *o_counter_write1 = b->counter_write1;
      *o_counter_write1_discard = b->counter_write1_discard;
      *o_counter_write1_wait = b->counter_write1_wait;
      *o_counter_write1_adaptive = b->counter_write1_adaptive;
      *o_counter_write1_adaptive_fast = b->counter_write1_adaptive_fast;
      *o_counter_write1_adaptive_very_fast = b->counter_write1_adaptive_very_fast;
      *o_counter_write2 = b->counter_write2;
      *o_counter_write2_discard = b->counter_write2_discard;
      *o_counter_write2_wait = b->counter_write2_wait;
      *o_counter_write2_adaptive = b->counter_write2_adaptive;
      *o_counter_write2_adaptive_fast = b->counter_write2_adaptive_fast;
      *o_counter_write2_adaptive_very_fast = b->counter_write2_adaptive_very_fast;
      
      b->next_full = -1;

      end = ATRSHMLOG_GET_TSC_CALL();

      *o_difftimetransfer = (end - start);

      result = atrshmlog_error_ok; /* We have succesful transfered a buffer */

    }

 ende:

  b->shmsize =  0;

  atomic_store_explicit(&b->state, atrshmlog_writeable, memory_order_release);

  /* We push it now on the available list again */

  b->next_append = atomic_load_explicit(&i_shm->shma, memory_order_acquire);
      
  while (!atomic_compare_exchange_weak_explicit(&i_shm->shma,
						(int*)&b->next_append,
						isfull,
						memory_order_release,
						memory_order_relaxed))
    ;

  return result;
}
