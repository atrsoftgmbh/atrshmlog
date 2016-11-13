
#include "../atrshmlog_internal.h"

/*******************************************************************/

/**
 * \file atrshmlogimpl_get_wait_for_slaves.c
 */

/** 
 * \n Main code:
 *
 * \brief Get the wait for slaves flag
 */
atrshmlog_ret_t atrshmlog_get_wait_for_slaves(void)
{
  return atrshmlog_wait_for_slaves;
}

