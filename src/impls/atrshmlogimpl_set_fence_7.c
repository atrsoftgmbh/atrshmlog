
#include "../atrshmlog_internal.h"


/**
 * \file atrshmlogimpl_set_fence_7.c
 */

/**
 * \brief We switch a fence on or off
 *
 * \return
 * The old flag for fence.
 */
atrshmlog_ret_t atrshmlog_set_thread_fence_7(atrshmlog_int32_t i_switch)
{
  return atrshmlog_set_thread_fence(&atrshmlog_thread_fence_7, i_switch);
}
