
#include "../atrshmlog_internal.h"


/*******************************************************************/

/**
 * \file atrshmlogimpl_get_inittime_tsc_after.c
 */

/** 
 * \n Main code:
 *
 * \brief Get the click after inittime
 *
 * test t_get_inittime_tsc_after.c
 */
atrshmlog_time_t atrshmlog_get_inittime_tsc_after(void)
{
  return atrshmlog_inittimetsc_after;
}
