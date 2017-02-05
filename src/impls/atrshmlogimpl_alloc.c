
#include "../atrshmlog_internal.h"


/*******************************************************************/

/**
 * \file atrshmlogimpl_alloc.c
 */

/**
 * \n Main code:
 *
 * We need a once init flag here 
 */
_Alignas(128) static atomic_flag atrshmlog_alloc_init = ATOMIC_FLAG_INIT;

/** 
 * \brief We allocate a buffer for the use of thread loging.
 *
 * We check the a list first.
 *
 * If we are lucky we are done ( thats a 200 click thing ...).
 *
 * If we are not lucky, we have to dyn alloc.
 *
 * We do this with prealloc count of buffers.
 *
 * So its a costly or the one thread that hits the a list,
 * but for all others after it its cheap again.
 *
 * Be sure you undersand if you need more buffers.
 *
 * There is allways a way to recycle if your thread ends.
 *
 * So try to use the atrshmlog_stop in any case possible.
 *
 * Then check which is the avarage number you need and adjust
 * the static buffers number first. 
 *
 * \return
 * - 0 : error
 * - non zero : adress of the new buffer for the thread
 *
 * test t_alloc.c
 */
atrshmlog_tbuff_t* atrshmlog_alloc(void)
{
  static volatile int tryalloc = 1;

  ATRSHMLOGSTAT(atrshmlog_counter_alloc);
  
 getcandidate:
  ;
  
  atrshmlog_tbuff_t* ta = (atrshmlog_tbuff_t*)atomic_load_explicit(&atrshmlog_tpa, memory_order_consume);

  // Is the a stack empty ?
  if (ta == 0)
    {
      // Do we have the alloc option ?
      if (!tryalloc)
	{
	  // Sorry, no more buffer for now
	  return 0;
	}

      // We fill the a stack with new entrys

      // We lock. This is needed to prevent multiple mallocs ...
      while (atomic_flag_test_and_set(&atrshmlog_alloc_init))
	;

      // If multiple threads are in a lock all but one should see a valid ptr
      // The first will see a null - or another thread did push an disposed buffer...
      ta = (atrshmlog_tbuff_t*)atomic_load(&atrshmlog_tpa);

      // Are we the first thread reaching here ?
      if (ta == 0)
	{
	  // Are we still alloc ing ?
	  if (!tryalloc)
	    {
	      // Sorry, no option left ...
	      atomic_flag_clear(&atrshmlog_alloc_init);

	      return 0;
	    }

	  int size = atrshmlog_buffer_infosize;

	  // We have to do the alloc
	  atrshmlog_tbuff_t* n = atrshmlog_il_get_raw_buffers(atrshmlog_prealloc_buffer_count, size);

	  if (n == 0)
	    {
	      // We are out of buffers ....
	      
	      tryalloc = 0; // We dont do this any longer
	      
	      atomic_flag_clear(&atrshmlog_alloc_init);

	      return 0;
	    }
      
	  // We init the ids and push the list on the stacks for cleanup and append
	  atrshmlog_il_connect_buffers_list(n, atrshmlog_prealloc_buffer_count, (char*)&n[atrshmlog_prealloc_buffer_count], size);

	  atomic_flag_clear(&atrshmlog_alloc_init);

	  if(atrshmlog_thread_fence_8)
	    atomic_thread_fence (memory_order_release);
	  
	  // The new candidates arrived. We try to pick one

	  goto getcandidate;
	}

      // We have a non null ta ... give lock free
      atomic_flag_clear(&atrshmlog_alloc_init);
    }

  // We pop from  stack now
  while (ta
	 && !atomic_compare_exchange_weak_explicit(&atrshmlog_tpa,
						   (intptr_t*)&ta,
						   (intptr_t)ta->next_append,
						   memory_order_consume,
						   memory_order_relaxed))
    ;

  // The stack was empty ?
  if (ta == 0)
    goto getcandidate;

  // This must be non 0 and we have used a consume, so no more fence here

  return ta;
}

