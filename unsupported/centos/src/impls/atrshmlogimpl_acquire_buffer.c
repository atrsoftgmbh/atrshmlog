
#include "../atrshmlog_internal.h"

/*******************************************************************/

/**
 * \file atrshmlogimpl_acquire_buffer.c
 */

/**
 * \n Main code:
 *
 * \brief We want a buffer from the a list.
 * 
 * We then initialize it.
 *
 * \param i_g
 * The adress of the thread local struct.
 *
 * \return
 * The adress of the buffer to use now for the thread.
 */
tbuff_t *atrshmlog_acquire_buffer(const g_tl_t* restrict i_g)
{
  atrshmlog_time_t start;
  atrshmlog_time_t end;

  // We meature the acquire time in clicks
  // we switch later on to get clicktime evtl.
  start = ATRSHMLOG_GET_TSC_CALL();

  tbuff_t* targetbuffer= atrshmlog_alloc();

  if (!targetbuffer)
    return targetbuffer; // null - nothing goes here ...

  // Ok. We init the buffer.
  targetbuffer->next_full = 0;
  
  targetbuffer->next_append = 0;

  targetbuffer->safeguardfront = ATRSHMLOGSAFEGUARDVALUE;

  targetbuffer->pid = i_g->atrshmlog_thread_pid;
  
  targetbuffer->tid = i_g->atrshmlog_thread_tid ;

  targetbuffer->transfer_to_shm_time = atrshmlog_transfer_to_shm_time;
  
  targetbuffer->dispose = 0;

  targetbuffer->size = 0;
  
  atomic_store_explicit(&targetbuffer->dispatched, 0, memory_order_relaxed);

  end = ATRSHMLOG_GET_TSC_CALL();

  targetbuffer->acquiretime = end - start;
  
  return targetbuffer;
}

