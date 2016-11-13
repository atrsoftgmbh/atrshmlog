
#include "../atrshmlog_internal.h"

/*******************************************************************/

/**
 * \file atrshmlogimpl_get_acquire_count.c
 */

/**
 * \n Main code:
 *
 * \brief The number of buffers from static and dyn memory
 */
atrshmlog_ret_t atrshmlog_get_acquire_count(void)
{
  return atrshmlog_acquire_count;
}

