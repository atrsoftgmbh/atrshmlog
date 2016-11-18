
#include "../atrshmlog_internal.h"

/*******************************************************************/

/**
 * \file atrshmlogimpl_stop.c
 */


/**
 * \n Main code:
 *
 * \brief We can stop logging for a thread with this function.
 *
 * The thread then release the buffers and thats it.
 *
 * No more logging after this from this thread.
 *
 * So be sure you do this in thread end.
 *
 * And be sure not to do this before you have to. 
 */
void atrshmlog_stop(void)
{
  ATRSHMLOGSTAT(atrshmlog_counter_stop);
  
  atrshmlog_g_tl_t* g  = (atrshmlog_g_tl_t* )atrshmlog_get_thread_locals_adress();
  
#if ATRSHMLOG_THREAD_LOCAL == 0

  // this can happen if pthread specific is in
  if (g == NULL)
    return ;

#endif

  if (g->atrshmlog_idnotok == 0)
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
	    }

	  atrshmlog_dispatch_buffer(t);
	}
    }      

  // We ultimately stop logging for this thread.
  g->atrshmlog_idnotok = 1;
}

