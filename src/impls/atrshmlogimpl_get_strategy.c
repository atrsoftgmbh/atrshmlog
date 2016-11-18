
#include "../atrshmlog_internal.h"

/**********************************************************/

/**
 * \file atrshmlogimpl_get_strategy.c
 */

/**
 * \n Main code:
 *
 * \brief We get the strategy for this thread
 *
 * \return
 * - negativ for error
 * - The value 
 */
atrshmlog_ret_t atrshmlog_get_strategy()
{
  atrshmlog_g_tl_t* g  = (atrshmlog_g_tl_t* )atrshmlog_get_thread_locals_adress();

#if ATRSHMLOG_THREAD_LOCAL == 0

  // if we use pthread specific this can happen
  if (g == NULL)
    return atrshmlog_error_get_strategy_1;

#endif
  
  int old = g->strategy;

  return old;
}
  
