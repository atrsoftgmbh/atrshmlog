
#include "../atrshmlog_internal.h"


/*******************************************************************/

/**
 * \file atrshmlogimpl_set_event.c
 */


static atrshmlog_event_t atrshmlog_set_event(const atrshmlog_int32_t i_index,
					     const int i_ev)
{
  ATRSHMLOGSTAT(atrshmlog_counter_set_event);
  
  if (i_index < 0 || i_index >= atrshmlog_event_locks_max)
    return 0;

  int old = atrshmlog_event_locks[i_index];

  ((atrshmlog_event_t*)atrshmlog_event_locks)[i_index] = i_ev;

  return old;
}


/********************************************************************/

/**
 * \n Main code:
 *
 * \brief Set the event flag to non zero. 
 *
 * So from this on we use the event.
 * This is to be used by the program after it was set to zero before.
 * The default is to have all events on at start.
 */
atrshmlog_event_t atrshmlog_set_event_on(atrshmlog_int32_t i_index)
{
  return atrshmlog_set_event(i_index, 1);
}

/*******************************************************************/

/**
 * \n Main code:
 *
 * \brief Set the event flag to zero. 
 *
 * So the event is no longer used.
 * This is to be used by the program if you want to switch off an event
 */
atrshmlog_event_t atrshmlog_set_event_off(atrshmlog_int32_t i_index)
{
  return atrshmlog_set_event(i_index, 0);
}
