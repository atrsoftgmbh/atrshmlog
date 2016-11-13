
#include "../atrshmlog_internal.h"

/**********************************************************/

/**
 * \file atrshmlogimpl_set_strategy.c
 */

/**
 * \n Main code:
 *
 * \brief We set a new strategy for this thread.
 *
 * \param i_strategy
 * see enum atrshmlog_strategy
 *
 */
atrshmlog_ret_t atrshmlog_set_strategy(const enum atrshmlog_strategy i_strategy)
{
  ATRSHMLOGSTAT(atrshmlog_counter_set_strategy);
  
  g_tl_t* g  = (g_tl_t* )atrshmlog_get_thread_locals_adress();

  int old = g->strategy;

  if (i_strategy >= atrshmlog_strategy_first && i_strategy <= atrshmlog_strategy_last)
    {
      g->strategy = i_strategy;
    }

  return old;
}

