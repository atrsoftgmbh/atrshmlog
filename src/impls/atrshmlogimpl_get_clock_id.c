
#include "../atrshmlog_internal.h"


/*******************************************************************/

/**
 * \file atrshmlogimpl_get_clock_id.c
 */

/**
 * \n Main code:
 *
 * \brief The clock id we use to get time.
 */
atrshmlog_ret_t atrshmlog_get_clock_id(void)
{
  return atrshmlog_clock_id;
}
