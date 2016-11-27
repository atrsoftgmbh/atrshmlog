
#include "../atrshmlog_internal.h"

/**********************************************************/

/**
 * \file atrshmlogimpl_set_strategy_process.c
 */

/**
 * \n Main code:
 *
 * \brief We set a new strategy for the process.
 *
 * \param i_strategy
 * see enum atrshmlog_strategy
 *
 * test t_set_strategy_process.c
 */
atrshmlog_ret_t atrshmlog_set_strategy_process(const enum atrshmlog_strategy i_strategy)
{
  ATRSHMLOGSTAT(atrshmlog_counter_set_strategy_process);

  int old = atrshmlog_buffer_strategy;

  if (i_strategy >= atrshmlog_strategy_first
      && i_strategy <= atrshmlog_strategy_last)
    {
       atrshmlog_buffer_strategy= i_strategy;
    }

  return old;
}

