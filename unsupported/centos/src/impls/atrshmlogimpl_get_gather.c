
#include "../atrshmlog_internal.h"


/*******************************************************************/

/**
 * \file atrshmlogimpl_get_gather.c
 */

/** 
 * \n Main code:
 *
 * \brief get the statistic switch 
 */

atrshmlog_ret_t atrshmlog_get_statistics_gathering()
{
  int old = atrshmlog_statistics_on;

  return old;
}

