
#include "../atrshmlog_internal.h"

/**********************************************************/

/**
 * \file atrshmlogimpl_get_strategy_wait_wait_time.c
 */

/**
 * \n Main code:
 *
 * \brief We get the nanos to wait
 *
 * test t_strategy_wait_wait_time.c
 */
atrshmlog_ret_t atrshmlog_get_strategy_wait_wait_time(void)
{
  int old = atrshmlog_strategy_wait_wait_time;

  return old;
}

