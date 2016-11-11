
#include "../atrshmlog_internal.h"

/*******************************************************************/

/**
 * \file atrshmlogimpl_set_gather_off.c
 */

/** 
 * \n Main code:
 *
 * \brief Set the statistic switch 
 */

atrshmlog_ret_t atrshmlog_set_statistics_gathering_off()
{
  int old = atrshmlog_statistics_on;

  atrshmlog_statistics_on = 0;

  return old;
}
