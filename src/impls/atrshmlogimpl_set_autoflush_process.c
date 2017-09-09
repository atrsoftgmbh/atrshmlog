
#include "../atrshmlog_internal.h"

/*******************************************************************/

/**
 * \file atrshmlogimpl_set_autoflush_process.c
 */

/** 
 * \n Main code:
 *
 * \brief Set the autoflush for the process
 *
 * test t_set_autoflush_process.c
 */
atrshmlog_ret_t atrshmlog_set_autoflush_process(const int i_flag)
{
  ATRSHMLOGSTAT(atrshmlog_counter_set_autoflush);

  int old  = atrshmlog_autoflush;

  atrshmlog_autoflush = i_flag;

  return old;
}
