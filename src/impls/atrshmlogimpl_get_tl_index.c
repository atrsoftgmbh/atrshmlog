
#include "../atrshmlog_internal.h"

/***************************************************************/

/**
 * \file atrshmlogimpl_get_tl_index.c
 */

/** 
 * \n Main code:
 *
 * \brief We get the tid of a thread local
 *
 * test t_get_tid.c
 */

atrshmlog_ret_t atrshmlog_get_thread_local_index (volatile const void * const i_local)
{
  atrshmlog_g_tl_t *g = (atrshmlog_g_tl_t*) i_local;

  if (g == NULL)
    return 0;

  return g->atrshmlog_targetbuffer_index;
}
