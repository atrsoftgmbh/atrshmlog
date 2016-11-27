
#include "../atrshmlog_internal.h"

/*******************************************************************/

/**
 * \file atrshmlogimpl_set_prealloc_count.c
 */

/** 
 * \n Main code:
 *
 * \brief Set the number of buffers for a dyn alloc To this.

 * Speed up by only doing one low level malloc.
 * But dont overdue this .
 * Its better to recomile and use static allocaed buffers.
 * And dont try to handle a leak of buffers because a thread does not
 * stop the logging by raising the count.
 *
 * test t_set_prealloc_count.c
 */
atrshmlog_ret_t atrshmlog_set_prealloc_buffer_count(atrshmlog_int32_t i_count)
{
  ATRSHMLOGSTAT(atrshmlog_counter_set_prealloc_count);

  int old = atrshmlog_prealloc_buffer_count;

  if (i_count > 1 && i_count <= ATRSHMLOG_MAX_PREALLOC_COUNT)
    atrshmlog_prealloc_buffer_count = i_count;

  return old;
}
