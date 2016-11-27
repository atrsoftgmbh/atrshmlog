
#include "../atrshmlog_internal.h"


/*******************************************************************/

/**
 * \file atrshmlogimpl_get_event_locks.c
 */

/**
 * \n Main code:
 *
 * \brief We have a limit for the number of event locks
 *
 * test t_get_event_locks_max.c
 */
atrshmlog_ret_t atrshmlog_get_event_locks_max(void)
{
  return atrshmlog_event_locks_max;
}
