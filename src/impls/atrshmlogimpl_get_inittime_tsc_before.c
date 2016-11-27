
#include "../atrshmlog_internal.h"

/*******************************************************************/

/**
 * \file atrshmlogimpl_get_inittime_tsc_before.c
 */

/** 
 * \n Main code:
 *
 * \brief Get the click before inittime
 *
 * test t_get_inittime_tsc_before.c
 */
atrshmlog_time_t atrshmlog_get_inittime_tsc_before(void)
{
  return atrshmlog_inittimetsc_before;
}

