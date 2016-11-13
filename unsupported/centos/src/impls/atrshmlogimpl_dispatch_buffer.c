
#include "../atrshmlog_internal.h"


/**
 * \file atrshmlogimpl_dispatch_buffer.c
 */

/**
 * \n Main code:
 *
 * \brief Put this buffer on the f list, so the thing is copied to shm 
 * by a slave.
 *
 * This is a helper, it only puts the buffer on the list.
 * So we normally get back in no time.
 *
 * But this is of course a thing of illusion.
 * In realety the slave needs time to do the shm copy.
 * And the reader needs time to get the buffer from the shm.
 * So those two limit the through put of log.
 *
 * Try to check for the times for the slave and the reader and then
 * see what time you really need for the transfer.
 *
 * \param i_atrshmlog_targetbuffer
 * The adress of the buffer
 *
 * \return 
 * void
 */ 
void atrshmlog_dispatch_buffer(tbuff_t* restrict i_atrshmlog_targetbuffer)
{
  ATRSHMLOGSTAT(atrshmlog_counter_dispatch);
  
  // We check for a valid buffer and for a non dispatched.
  // In case the buffer is already dispatched we have to see
  // where we made this call..
  // .. but for now we simply ignore the call
  if (i_atrshmlog_targetbuffer
      && (atomic_load_explicit(&i_atrshmlog_targetbuffer->dispatched, memory_order_relaxed) == 0))
    {
      // We sync the buffer content itself - hard sync if needed.
      if (atrshmlog_thread_fence_1)
	atomic_thread_fence (memory_order_release);

      // We set the buffer dispatched. 
      atomic_store_explicit(&i_atrshmlog_targetbuffer->dispatched, 1, memory_order_release);

      // Do we really have to do this ? or is it empty ?
      // this is wrong. we never can release a buffer then      if (i_atrshmlog_targetbuffer->size > 0)

	  // We put the buffer on the f list
	  i_atrshmlog_targetbuffer->next_full = (tbuff_t*)atomic_load_explicit(&atrshmlog_tpf, memory_order_relaxed);


	  // Push on stack ...
	  while(!atomic_compare_exchange_weak_explicit(&atrshmlog_tpf,
						       &i_atrshmlog_targetbuffer->next_full,
						       i_atrshmlog_targetbuffer,
						       memory_order_relaxed,
						       memory_order_relaxed))
	    ;

    }
}

