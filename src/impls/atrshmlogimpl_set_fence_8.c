
#include "../atrshmlog_internal.h"


/**
 * \file atrshmlogimpl_set_fence_8.c
 */

/**
 * \brief We switch a fence on or off
 *
 * \return
 * The old flag for fence.
 *
 * test t_set_fence_8.c
 */
atrshmlog_ret_t atrshmlog_set_thread_fence_8(atrshmlog_int32_t i_switch)
{
  return atrshmlog_set_thread_fence(&atrshmlog_thread_fence_8, i_switch);
}

