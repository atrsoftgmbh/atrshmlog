
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
  
  g_tl_t* g  = (g_tl_t* )atrshmlog_get_thread_locals_adress();
  
  if (g->atrshmlog_idnotok == 0)
    {
      for (int i = 0; i < ATRSHMLOGTARGETBUFFERMAX; i++)
	{
	  tbuff_t* t = g->atrshmlog_targetbuffer_arr[i];

	  if (t)
	    t->dispose = 1;

	  atrshmlog_dispatch_buffer(t);
	}
    }      

  // We ultimately stop logging for this thread.
  g->atrshmlog_idnotok = 1;
}

