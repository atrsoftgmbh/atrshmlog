
#include "../atrshmlog_internal.h"


/**
 * \file atrshmlogimpl_get_fence_8.c
 */

/**
 * \brief We get a fence on or off flag
 *
 * \return
 * The old flag for fence.
 */
atrshmlog_ret_t atrshmlog_get_thread_fence_8(void)
{
  int old =  atrshmlog_thread_fence_8;

  return old;
}
