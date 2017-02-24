
#include "../atrshmlog_internal.h"

/***************************************************************/

/**
 * \file atrshmlogimpl_get_tl_tid.c
 */

/** 
 * \n Main code:
 *
 * \brief We get the tid of a thread local
 *
 * test t_get_tid.c
 */
atrshmlog_ret_t atrshmlog_get_thread_buffer_safeguard (volatile const void * const i_buffer)
{
  atrshmlog_tbuff_t* b = ( atrshmlog_tbuff_t* ) i_buffer;
  if (b == NULL)
    return 0;

  return b->safeguardfront;
}
