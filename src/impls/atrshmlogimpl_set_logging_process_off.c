
#include "../atrshmlog_internal.h"


/*******************************************************************/

/**
 * \file atrshmlogimpl_set_logging_process_off.c
 */

/** 
 * \n Main code:
 *
 * \brief Set the logging flag for the process to off
 */
atrshmlog_ret_t atrshmlog_set_logging_process_off(void)
{
  ATRSHMLOGSTAT(atrshmlog_counter_logging_process_off);
  
  int old  = atrshmlog_logging_process;
  
  *((atrshmlog_int32_t*)&atrshmlog_logging_process) = 0;

  return old;
}
