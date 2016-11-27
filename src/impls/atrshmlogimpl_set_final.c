
#include "../atrshmlog_internal.h"

/*******************************************************************/

/**
 * \file atrshmlogimpl_set_final.c
 */

/** 
 * \n Main code:
 *
 * \brief Set the logging flag for the process final to on
 *
 * test t_exit_cleanup.c
 */
atrshmlog_ret_t atrshmlog_set_logging_process_off_final(void)
{
  ATRSHMLOGSTAT(atrshmlog_counter_logging_process_off_final);
  
  int old  = atrshmlog_logging_process_off_final;
  
  atrshmlog_logging_process_off_final = 1;

  return old;
}

