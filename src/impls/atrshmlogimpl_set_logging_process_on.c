
#include "../atrshmlog_internal.h"

/*******************************************************************/

/**
 * \file atrshmlogimpl_set_logging_process_on.c
 */

/**
 * \n Main code:
 *
 * \brief Set the logging flag for the process to on
 *
 * test t_set_logging_process.c
 */
atrshmlog_ret_t atrshmlog_set_logging_process_on(void)
{
  ATRSHMLOGSTAT(atrshmlog_counter_logging_process_on);
  
  int old  = atrshmlog_logging_process;
  
  *((atrshmlog_int32_t*)&atrshmlog_logging_process) = 1;

  return old;
}
