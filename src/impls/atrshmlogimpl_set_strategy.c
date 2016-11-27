
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
 * \return
 * - negativ error
 * - old value
 *
 * test t_set_strategy.c
 */
atrshmlog_ret_t atrshmlog_set_strategy(const enum atrshmlog_strategy i_strategy)
{
  ATRSHMLOGSTAT(atrshmlog_counter_set_strategy);
  
  atrshmlog_g_tl_t* g  = (atrshmlog_g_tl_t* )atrshmlog_get_thread_locals_adress();

#if ATRSHMLOG_THREAD_LOCAL == 0

  // this can happen if pthread specific is in
  if (g == NULL)
    return atrshmlog_error_set_strategy_1;

#endif
  
  int old = g->strategy;

  if (i_strategy >= atrshmlog_strategy_first && i_strategy <= atrshmlog_strategy_last)
    {
      g->strategy = i_strategy;
    }

  return old;
}

