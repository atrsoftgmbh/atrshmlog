
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
 * dispatched, they are all NULL ptrs
 *
 * test t_turn_logging_off.c
 */
 void atrshmlog_turn_logging_off(volatile const void* i_thread_locals)
 {
   ATRSHMLOGSTAT(atrshmlog_counter_turn_logging_off);
  
   if (i_thread_locals == NULL)
     return;

   atrshmlog_g_tl_t* g  =  (atrshmlog_g_tl_t*)i_thread_locals;

   int logging = (g->atrshmlog_idnotok == 0);

   g->atrshmlog_idnotok = 1;
    
   if(atrshmlog_thread_fence_13)
     atomic_thread_fence (memory_order_release);
	  
   if (logging)
     {
       for (int i = 0; i < ATRSHMLOGTARGETBUFFERMAX; i++)
	 {
	   atrshmlog_tbuff_t* t = g->atrshmlog_targetbuffer_arr[i];

	   if (t)
	     {
	       t->dispose = 1;

	       t->number_dispatched = g->number_dispatched++;

	       t->counter_write0 = g->counter_write0;

	       t->counter_write0_discard = g->counter_write0_discard;

	       t->counter_write0_wait = g->counter_write0_wait;

	       t->counter_write0_adaptive = g->counter_write0_adaptive;

	       t->counter_write0_adaptive_fast = g->counter_write0_adaptive_fast;

	       t->counter_write0_adaptive_very_fast = g->counter_write0_adaptive_very_fast;

	       t->counter_write1 = g->counter_write1;

	       t->counter_write1_discard = g->counter_write1_discard;

	       t->counter_write1_wait = g->counter_write1_wait;

	       t->counter_write1_adaptive = g->counter_write1_adaptive;

	       t->counter_write1_adaptive_fast = g->counter_write1_adaptive_fast;

	       t->counter_write1_adaptive_very_fast = g->counter_write1_adaptive_very_fast;

	       t->counter_write2 = g->counter_write2;

	       t->counter_write2_discard = g->counter_write2_discard;

	       t->counter_write2_wait = g->counter_write2_wait;

	       t->counter_write2_adaptive = g->counter_write2_adaptive;

	       t->counter_write2_adaptive_fast = g->counter_write2_adaptive_fast;

	       t->counter_write2_adaptive_very_fast = g->counter_write2_adaptive_very_fast;

	       atrshmlog_dispatch_buffer(t);

	       g->atrshmlog_targetbuffer_arr[i] = 0;
	     }
	 }
     }      
 }

