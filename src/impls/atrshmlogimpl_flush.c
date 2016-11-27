
#include "../atrshmlog_internal.h"

/**********************************************************/

/**
 * \file atrshmlogimpl_flush.c
 */

/**
 * \n Main code:
 *
 * \brief We flush the used buffers with this function.
 *
 * So this is a small slow down if we have to do it.
 *
 * Normally one buffer should be empty any way...
 *
 * test t_flush.c
 */
void atrshmlog_flush(void)
{
  ATRSHMLOGSTAT(atrshmlog_counter_flush);
  
  atrshmlog_g_tl_t* g  = (atrshmlog_g_tl_t* )atrshmlog_get_thread_locals_adress();
  
#if ATRSHMLOG_THREAD_LOCAL == 0
  
  // if we use pthread specific this can happen
  if (g == NULL)
    return;

#endif
  
  for (int i = 0; i < ATRSHMLOGTARGETBUFFERMAX; i++)
    {
      atrshmlog_tbuff_t* t = g->atrshmlog_targetbuffer_arr[i];

      if(t && t->size > 0)
	{
	  t->number_dispatched = g->number_dispatched++;  // we have a new highest here
 	  
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
	}
      
      atrshmlog_dispatch_buffer(t);
    }
}

