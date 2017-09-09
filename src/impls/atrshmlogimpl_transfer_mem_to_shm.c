
#include "../atrshmlog_internal.h"

#include <string.h>

/*******************************************************************/

/**
 * \file atrshmlogimpl_transfer_mem_to_shm.c
 */

/** 
 * \n Main code:
 *
 * \brief helper : transfer local memory buffer to a shm buffer
 *
 * We transfer the buffer into a shm buffer .
 *
 * We use the a list in shm to get a free buffer.
 * 
 * Then we transfer the stuff from the in memory buffer to that buffer.
 *
 * We also add statistical data for the user to see if there
 * is a starvation in this (only times, in a special version i 
 * did counters too)
 *
 * Then we put the buffer on the f list in shm.
 * 
 * We free the buffer in memory and give it back to the caller then.
 *
 * \param i_mem
 * Adress of the buffer to transfer.
 *
 * \param i_g
 * Adress of the thread local struct.
 *
 * \return 
 * >= 1000: ok, index of buffer is value - 1000
 * - negative ... 999 : error
 *
 * test t_transfer_mem_to_shm.c
 */
int atrshmlog_transfer_mem_to_shm(const atrshmlog_tbuff_t* restrict i_mem,
					 volatile const atrshmlog_g_tl_t* restrict i_g)
{
  int result = 0;
  
  atrshmlog_time_t start;

  ATRSHMLOGSTAT(atrshmlog_counter_mem_to_shm);

  atrshmlog_base_ptr_use_flag++;
  
  if (atrshmlog_base_ptr == 0)
    {
      result =  atrshmlog_error_mem_to_shm_1;
      goto ende;
    }

  if (i_mem == NULL)
    {
      result =  atrshmlog_error_mem_to_shm_2;
      goto ende;
    }

  if (i_mem->size == 0)
    {
      result =  atrshmlog_error_mem_to_shm_3;
      goto ende;
    }

  if (i_mem->maxsize < i_mem->size)
    {
      result =  atrshmlog_error_mem_to_shm_4;
      goto ende;
    }

  if (atrshmlog_logging_process_off_final != 0)
    {
      result =  atrshmlog_error_mem_to_shm_8;
      goto ende;
    }
  
  
  /* Those normally never change */

  /* Bad thing. Wrong id */
  if (i_g->atrshmlog_idnotok)
    {
      result =  atrshmlog_error_mem_to_shm_5;
      goto ende;
    }

  /* Next. we check for initialized */
  atrshmlog_area_t * a = ATRSHMLOG_GETAREA;

  /* Those can change from run to run */
  /* Bad thing. Safeguard invalid */
  if (a->shmsafeguard != ATRSHMLOGSAFEGUARDVALUE)
    {
      result =  atrshmlog_error_mem_to_shm_6;
      goto ende;
    }

  start = ATRSHMLOG_GET_TSC_CALL();

  ATRSHMLOGSTAT(atrshmlog_counter_mem_to_shm_doit);
  
  // we calc the checksum trivial. but this is ok for our needs
  if (atrshmlog_checksum)
    {
      int chksum = 0;

      for (int k = 0; k < i_mem->size; k++)
	{
	  chksum += i_mem->b[k];
	}

      if (i_mem->chksum != chksum)
	{
	  ATRSHMLOGSTAT(atrshmlog_counter_fence_alarm_1);
	}
    }
  
#if ATRSHMLOGDEBUG == 1
  printf("mem to %ld %ld %ld %ld\n", (long)i_mem->id, (long)i_mem->size, (long)chksum, (long)i_g->atrshmlog_idnotok);
#endif

  /* 
   * We loop till we hit a free buffer.
   */
  while (1)
    {
      int available = atomic_load_explicit(&a->shma, memory_order_consume);

      if (available == -1)
	{
	  ATRSHMLOGSTAT(atrshmlog_counter_mem_to_shm_full);
	  /* 
	   * We have to wait for a free one.
	   */
	  ATRSHMLOG_SLEEP_NANOS(atrshmlog_slave_to_shm_wait);

	  if (a->ich_habe_fertig != 0)
	    {
	      result =  atrshmlog_error_mem_to_shm_7;
	      goto ende;
	    }

	  if (atrshmlog_logging_process_off_final)
	    {
	      result =  atrshmlog_error_mem_to_shm_8;
	      goto ende;
	    }
	  
	  continue;
	}

      /* We pop from list */
      while ( available > -1
	      && !atomic_compare_exchange_weak_explicit(&a->shma,
							&available,
							a->logbuffers[available].next_append,
							memory_order_relaxed,
							memory_order_relaxed))
	;

      // Was the list non empty ? 
      if (available > -1)
	{
	  atrshmlog_buffer_t* restrict b = &(a->logbuffers[available]);

	  b->next_append = -1;
	  
	  b->shmsize = i_mem->size;

	  b->chksum = i_mem->chksum;

	  b->inittime = atrshmlog_inittime;
	  b->inittimetsc_before = atrshmlog_inittimetsc_before;
	  b->inittimetsc_after = atrshmlog_inittimetsc_after;

	  b->pid = i_mem->pid;
	  
	  b->tid = i_mem->tid;

	  b->acquiretime = i_mem->acquiretime;

	  b->id = i_mem->id;
	  
	  b->starttransfer = start;

          b->number_dispatched = i_mem->number_dispatched;

          b->counter_write0 = i_mem->counter_write0;

	  b->counter_write0_discard = i_mem->counter_write0_discard;

	  b->counter_write0_wait = i_mem->counter_write0_wait;

	  b->counter_write0_adaptive = i_mem->counter_write0_adaptive;

	  b->counter_write0_adaptive_fast = i_mem->counter_write0_adaptive_fast;

	  b->counter_write0_adaptive_very_fast = i_mem->counter_write0_adaptive_very_fast;

	  b->counter_write1 = i_mem->counter_write1;

	  b->counter_write1_discard = i_mem->counter_write1_discard;

	  b->counter_write1_wait = i_mem->counter_write1_wait;

	  b->counter_write1_adaptive = i_mem->counter_write1_adaptive;

	  b->counter_write1_adaptive_fast = i_mem->counter_write1_adaptive_fast;

	  b->counter_write1_adaptive_very_fast = i_mem->counter_write1_adaptive_very_fast;

	  b->counter_write2 = i_mem->counter_write2;

	  b->counter_write2_discard = i_mem->counter_write2_discard;

	  b->counter_write2_wait = i_mem->counter_write2_wait;

	  b->counter_write2_adaptive = i_mem->counter_write2_adaptive;

	  b->counter_write2_adaptive_fast = i_mem->counter_write2_adaptive_fast;

	  b->counter_write2_adaptive_very_fast = i_mem->counter_write2_adaptive_very_fast;

	  char* restrict target = (char*)a + b->info;
#if 1
	  memcpy(target, i_mem->b, i_mem->size);
#else
	  for (int zippo = 0; zippo < i_mem->size; zippo++)
	    target[zippo] = i_mem->b[zippo];
#endif

	  
	  b->lasttimetsc_before = ATRSHMLOG_GET_TSC_CALL();

	  b->lasttime = atrshmlog_get_realtime();
	  
	  b->lasttimetsc_after = ATRSHMLOG_GET_TSC_CALL();

	  // for the adaptive strategy
	  atomic_store_explicit(&atrshmlog_last_mem_to_shm, (int)(b->lasttimetsc_before - start) , memory_order_release);
	  
	  if(atrshmlog_thread_fence_5)
	    atomic_thread_fence(memory_order_release);
	      
	  atomic_store_explicit(&(b->state), atrshmlog_full, memory_order_release);

	  // We push it now on the full list 

	  b->next_full = atomic_load_explicit(&a->shmf, memory_order_acquire);

	  while (!atomic_compare_exchange_weak_explicit(&a->shmf,
							&b->next_full, 
							available,
							memory_order_acq_rel,
							memory_order_relaxed))
	    ;
	  
	  result = 1000 + available; // >= 0 ...
	  goto ende;
	}
    }
  
 ende:

  // however we made it to this point : the buffer is no longer to be transfered.
  ((atrshmlog_tbuff_t*)i_mem)->size = 0;

  atrshmlog_base_ptr_use_flag--;

  return result;
  
}
