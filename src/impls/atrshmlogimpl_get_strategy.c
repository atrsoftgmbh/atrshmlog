
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
 * The value 
 */
atrshmlog_ret_t atrshmlog_get_strategy()
{
  atrshmlog_g_tl_t* g  = (atrshmlog_g_tl_t* )atrshmlog_get_thread_locals_adress();

  int old = g->strategy;

  return old;
}
  
