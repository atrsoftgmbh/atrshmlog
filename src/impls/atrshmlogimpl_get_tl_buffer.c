
#include "../atrshmlog_internal.h"

/***************************************************************/

/**
 * \file atrshmlogimpl_get_tl_buffer.c
 */

/** 
 * \n Main code:
 *
 * \brief We get the tid of a thread local
 *
 * test t_get_tid.c
 */

volatile const void *atrshmlog_get_thread_local_buffer (volatile const void *i_local, atrshmlog_int32_t i_index)
{
  atrshmlog_g_tl_t *g = (atrshmlog_g_tl_t*) i_local;

  if (g == NULL || i_index >= ATRSHMLOGTARGETBUFFERMAX || i_index < 0)
    return 0;

  return g->atrshmlog_targetbuffer_arr[i_index];
}
