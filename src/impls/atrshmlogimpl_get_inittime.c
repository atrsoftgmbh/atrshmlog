
#include "../atrshmlog_internal.h"


/*******************************************************************/

/**
 * \file atrshmlogimpl_get_inittime.c
 */

/** 
 * \n Main code:
 *
 * \brief Get the inittime for the process
 * 
 * test t_get_inittime.c
 */
atrshmlog_internal_time_t atrshmlog_get_inittime(void)
{
  return atrshmlog_inittime;
}
