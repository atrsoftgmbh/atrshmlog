
#include "../atrshmlog_internal.h"


/**
 * \file atrshmlogimpl_set_thread_fence.c
 */
/**
 * Helper: set a fence flag on or off 
 *
 * test t_set_fence_1.c
 */
atrshmlog_ret_t atrshmlog_set_thread_fence(int *v,
					   const atrshmlog_int32_t i_switch)
{
  ATRSHMLOGSTAT(atrshmlog_counter_set_thread_fence);

  int old = *v;

  *v = i_switch ? 1 : 0;

  return old;
}

