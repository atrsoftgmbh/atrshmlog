
#include "../atrshmlog_internal.h"

/*******************************************************************/

/**
 * \file atrshmlogimpl_get_autoflush.c
 */

/**
 * \n Main code:
 *
 * \brief The autoflush flag
 *
 * test t_get_autoflush_process.c
 */
atrshmlog_ret_t atrshmlog_get_autoflush_process(void)
{
  return atrshmlog_autoflush;
}

