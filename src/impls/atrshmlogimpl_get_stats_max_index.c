
#include "../atrshmlog_internal.h"


/*******************************************************************/

/**
 * \file atrshmlogimpl_get_stats_max_index.c
 */

/** 
 * \n Main code:
 *
 * \brief We deliver the g_counter data max index
 *
 * test NA
 */
atrshmlog_int32_t atrshmlog_get_statistics_max_index(void)
{
  return atrshmlog_counter_end;
}
