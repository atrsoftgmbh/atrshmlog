
#include "../atrshmlog_internal.h"

/*******************************************************************/

/**
 * \file atrshmlogimpl_turn_logging.off.c
 */

/** 
 * \n Main code:
 *
 * \brief We switch the thread off and dispatch its buffers
 *
 * This works for normal threads as expected. 
 * For a slave it switches the flag, and no buffers are 
 * dispateched, they are all NULL ptrs
 */
 void atrshmlog_turn_logging_off(volatile const void* i_thread_locals)
 {
   ATRSHMLOGSTAT(atrshmlog_counter_turn_logging_off);
  
   if (i_thread_locals == NULL)
     return;

   g_tl_t* g  =  (g_tl_t*)i_thread_locals;

   int logging = (g->atrshmlog_idnotok == 0);

   g->atrshmlog_idnotok = 1;
    
   if(atrshmlog_thread_fence_13)
     atomic_thread_fence (memory_order_release);
	  
   if (logging)
     {
       for (int i = 0; i < ATRSHMLOGTARGETBUFFERMAX; i++)
	 {
	   tbuff_t* t = g->atrshmlog_targetbuffer_arr[i];

	   if (t)
	     t->dispose = 1;

	   atrshmlog_dispatch_buffer(t);
	 }
     }      
 }

