
#include "../atrshmlog_internal.h"


/*******************************************************************/

/**
 * \file atrshmlogimpl_get_prealloc_count.c
 */

/** 
 * \n Main code:
 *
 * \brief The number of buffers that are alloced in one 
 * dyn memory allocation to reduce overhead.
 *
 * test t_get_prealloc_count.c
 */
atrshmlog_ret_t atrshmlog_get_prealloc_buffer_count(void)
{
  return atrshmlog_prealloc_buffer_count;
}

