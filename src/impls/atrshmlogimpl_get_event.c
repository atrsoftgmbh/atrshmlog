
#include "../atrshmlog_internal.h"


/*******************************************************************/

/**
 * \file atrshmlogimpl_get_event.c
 */

/**
 * \n Main code:
 *
 * \brief Get the event flag state.
 *
 * test t_get_event.c
 */
atrshmlog_event_t atrshmlog_get_event(atrshmlog_int32_t i_index)
{
  if (i_index < 0 || i_index >= atrshmlog_event_locks_max)
    return atrshmlog_error_get_event_1;
  
  return atrshmlog_event_locks[i_index];
}

