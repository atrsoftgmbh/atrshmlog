
#include "../atrshmlog_internal.h"


/**
 * \file atrshmlogimpl_set_gather_on.c
 */

/*******************************************************************/

/** 
 * \n Main code:
 *
 * \brief Set the statistic switch 
 */

atrshmlog_ret_t atrshmlog_set_statistics_gathering_on()
{
  int old = atrshmlog_statistics_on;

  atrshmlog_statistics_on = 1;

  return old;
}

