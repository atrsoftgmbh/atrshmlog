
#include "../atrshmlog_internal.h"


/**
 * \file atrshmlogimpl_get_fence_12.c
 */

/**
 * \brief We get a fence on or off flag
 *
 * \return
 * The old flag for fence.
 *
 * test t_get_thread_fence_12.c
 */
atrshmlog_ret_t atrshmlog_get_thread_fence_12(void)
{
  int old =  atrshmlog_thread_fence_12;

  return old;
}
