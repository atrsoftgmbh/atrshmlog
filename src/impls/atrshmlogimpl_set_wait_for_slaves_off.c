
#include "../atrshmlog_internal.h"

/*******************************************************************/

/**
 * \file atrshmlogimpl_set_wait_for_slaves_off.c
 */

/** 
 * \n Main code:
 *
 * \brief Set the wait for slaves flag to off
 *
 * test t_set_wait_for_slaves.c
 */
atrshmlog_ret_t atrshmlog_set_wait_for_slaves_off(void)
{
  ATRSHMLOGSTAT(atrshmlog_counter_set_wait_slaves_off);

  int old  = atrshmlog_wait_for_slaves;

  atrshmlog_wait_for_slaves = 0;

  return old;
}
