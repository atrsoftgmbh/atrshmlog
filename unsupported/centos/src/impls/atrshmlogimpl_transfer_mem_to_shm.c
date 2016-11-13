
#include "../atrshmlog_internal.h"


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
 * - 0: ok
 * - non zero : error
 */
int atrshmlog_transfer_mem_to_shm(const tbuff_t* restrict i_mem,
					 const g_tl_t* restrict i_g)
{
  atrshmlog_time_t start;

  ATRSHMLOGSTAT(atrshmlog_counter_mem_to_shm);

  if (atrshmlog_base_ptr == 0)
    return atrshmlog_error_mem_to_shm_1;

  if (i_mem == NULL)
    return atrshmlog_error_mem_to_shm_2;

  if (i_mem->size == 0)
    return atrshmlog_error_mem_to_shm_3;

  if (i_mem->maxsize < i_mem->size)
    return atrshmlog_error_mem_to_shm_4;

  if (atrshmlog_logging_process_off_final != 0)
    return atrshmlog_error_mem_to_shm_8;
  
#if ATRSHMLOGDEBUG == 1
  printf("mem to %ld %ld %ld\n", (long)i_mem->id, (long)i_size, (long)i_g->atrshmlog_idnotok);
#endif
  
  /* Those normally never change */

  /* Bad thing. Wrong id */
  if (i_g->atrshmlog_idnotok)
    {
	return atrshmlog_error_mem_to_shm_5;
    } 

  /* Next. we check for initialized */
  atrshmlog_area_t * a = ATRSHMLOG_GETAREA;

  /* Those can change from run to run */
  /* Bad thing. Safeguard invalid */
  if (a->shmsafeguard != ATRSHMLOGSAFEGUARDVALUE)
    return atrshmlog_error_mem_to_shm_6;

  start = ATRSHMLOG_GET_TSC_CALL();

  ATRSHMLOGSTAT(atrshmlog_counter_mem_to_shm_doit);
  
  int chksum = 0;

#if ATRSHMLOGDEBUG == 1
  for (int k = 0; k < i_size; k++)
    {
      chksum += i_mem->b[k];
    }
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

	  if (atomic_load_explicit(&a->ich_habe_fertig, memory_order_relaxed) != 0)
	    return atrshmlog_error_mem_to_shm_7;

	  if (atrshmlog_logging_process_off_final)
	    return atrshmlog_error_mem_to_shm_8;
	  
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

	  b->chksum = chksum;

	  b->inittime = atrshmlog_inittime;
	  b->inittimetsc_before = atrshmlog_inittimetsc_before;
	  b->inittimetsc_after = atrshmlog_inittimetsc_after;

	  b->pid = i_mem->pid;
	  
	  b->tid = i_mem->tid;

	  b->acquiretime = i_mem->acquiretime;

	  b->id = i_mem->id;
	  
	  b->starttransfer = start;

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

	  // this is a hack. we know we have a const buffer,
	  // but on the other side we have to change some value
	  // so i do this the hard way here.
	  // its ok so far - no buffer is ever in const area of the process,
	  // so i can do this.
	  // but its better to do it locally only once ...
	  *(atrshmlog_time_t*)&i_mem->transfer_to_shm_time = b->lasttimetsc_before - start;
	  
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
	  
	  return atrshmlog_error_ok;
	}
    }
  
  /* NOT REACHED */
}
