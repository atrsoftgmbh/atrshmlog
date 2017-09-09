
#include "../atrshmlog_internal.h"

/**********************************************************/

/**
 * \file atrshmlogimpl_set_strategy_wait_wait_time.c
 */

/**
 * \n Main code:
 *
 * \brief We set the wait time 
 *
 * \param i_nanos
 * new wait time
 *
 * test t_strategy_wait_wait_time.c
 */
atrshmlog_ret_t atrshmlog_set_strategy_wait_wait_time(const atrshmlog_int32_t i_nanos)
{
  int old = atrshmlog_strategy_wait_wait_time;

  if (i_nanos >= 0
      && i_nanos < 1000000000)
    {
      atrshmlog_strategy_wait_wait_time = i_nanos;
    }

  return old;
}

