
#include "../atrshmlog_internal.h"

/*******************************************************************/

/**
 * \file atrshmlogimpl_set_wait_for_slaves_on.c
 */

/** 
 * \n Main code:
 *
 * \brief Set the flag for wait to on.
 *
 * So we wait in the cleanup till all slaves are done.
 *
 * This is not on by default.
 *
 * So you can switch this on
 * which means you start processing in cleanup after all
 * slaves have stopped.
 *
 * Of course there is a problem.
 * If you ever run in a hang of the slave you wait in the exit.
 */
atrshmlog_ret_t atrshmlog_set_wait_for_slaves_on(void)
{
  ATRSHMLOGSTAT(atrshmlog_counter_set_wait_slaves_on);

   int old  = atrshmlog_wait_for_slaves;

  atrshmlog_wait_for_slaves = 1;

  return old;
}

