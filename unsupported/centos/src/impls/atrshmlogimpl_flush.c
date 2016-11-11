
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
 */
void atrshmlog_flush(void)
{
  ATRSHMLOGSTAT(atrshmlog_counter_flush);
  
  g_tl_t* g  = (g_tl_t* )atrshmlog_get_thread_locals_adress();
  
  for (int i = 0; i < ATRSHMLOGTARGETBUFFERMAX; i++)
    {
      tbuff_t* t = g->atrshmlog_targetbuffer_arr[i];

      atrshmlog_dispatch_buffer(t);
    }
}

