
#include "../atrshmlog_internal.h"

/**********************************************************/

/**
 * \file atrshmlogimpl_get_strategy_process.c
 */

/**
 * \n Main code:
 *
 * \brief We get the strategy for the process
 *
 * \return
 * The value 
 */
atrshmlog_ret_t atrshmlog_get_strategy_process(void)
{
  int old = atrshmlog_buffer_strategy;

  return old;
}
  
