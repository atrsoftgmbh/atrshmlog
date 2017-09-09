
#include "../atrshmlog_internal.h"

#include <string.h>

#include <stdlib.h>

/*******************************************************************/

/**
 * \file atrshmlogimpl_set_event_locks.c
 */

/**
 * \n Main code:
 *
 * \brief We can set the limit of event locks.
 *
 * This should be done at the beginning, best before
 * attach.
 *
 * You should have at least set the size to the highest 
 * event number you use in logging calls for gettime and write.
 *
 * If you dont hit the build in buffer size nothing is done.
 * If you set a bigger size it cannot be reduced after.
 *
 * test t_set_event_locks_max.c
 */
atrshmlog_ret_t atrshmlog_set_event_locks_max(atrshmlog_int32_t i_max)
{
  ATRSHMLOGSTAT(atrshmlog_counter_set_event_locks);

  int old = atrshmlog_event_locks_max;
  
  if (i_max >  ATRSHMLOGEVENTCOUNT && i_max > atrshmlog_event_locks_max && atrshmlog_attach_once == 0)
    {
      int oldmax = atrshmlog_event_locks_max;
      
      atrshmlog_event_locks_max = i_max;

      atrshmlog_event_t * p =  (atrshmlog_event_t*)malloc(atrshmlog_event_locks_max + 1);

      memset(p, 0, atrshmlog_event_locks_max + 1);
      
      memcpy(p, (void*)atrshmlog_event_locks, oldmax);
      
      atrshmlog_event_locks = p;
    }
  
  return old;
}

