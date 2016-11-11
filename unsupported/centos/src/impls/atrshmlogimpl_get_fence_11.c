
#include "../atrshmlog_internal.h"


/**
 * \file atrshmlogimpl_get_fence_11.c
 */

/**
 * \brief We get a fence on or off flag
 *
 * \return
 * The old flag for fence.
 */
atrshmlog_ret_t atrshmlog_get_thread_fence_11(void)
{
  int old =  atrshmlog_thread_fence_11;

  return old;
}
