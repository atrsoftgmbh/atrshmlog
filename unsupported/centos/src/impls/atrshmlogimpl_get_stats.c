
#include "../atrshmlog_internal.h"

/*******************************************************************/

/**
 * \file atrshmlogimpl_get_stats.c
 */

/** 
 * \n Main code:
 *
 * \brief We deliver the g_counter data 
 * 
 * We set the now time tsc in 0 low part and 1 high part.
 */
void atrshmlog_get_statistics(atrshmlog_int32_t* o_target)
{
  
  atrshmlog_time_t n =  ATRSHMLOG_GET_TSC_CALL();
  
  atomic_store(&atrshmlog_counter[0], n & 0xffffffff);
  atomic_store(&atrshmlog_counter[1], (n >> 32) & 0xffffffff);
  
  for (int i = atrshmlog_counter_start; i <= atrshmlog_counter_end; i++)
    {
      atrshmlog_int32_t value = atomic_load(&atrshmlog_counter[i]);

      o_target[i] = value;
    }
}
