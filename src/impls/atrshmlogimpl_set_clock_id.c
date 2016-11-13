
#include "../atrshmlog_internal.h"


/*******************************************************************/

/**
 * \file atrshmlogimpl_set_clock_id.c
 */

/**
 * \n Main code:
 *
 * \brief The clock id can be set with this.
 */
atrshmlog_ret_t atrshmlog_set_clock_id(atrshmlog_int32_t i_id)
{
  ATRSHMLOGSTAT(atrshmlog_counter_set_clock_id);

  int old = atrshmlog_clock_id;
  
  if (i_id >= ATRSHMLOG_CLOCK_ID_MIN && i_id <= ATRSHMLOG_CLOCK_ID_MAX)
    { 
      *(atrshmlog_int32_t*)&atrshmlog_clock_id = i_id;
    }

  return old;
}

