
#include "../atrshmlog_internal.h"

/*******************************************************************/

/**
 * \file atrshmlogimpl_get_logging.c
 */

/** 
 * \n Main code:
 *
 * \brief Get the process logging status.
 *
 * Get also the ich habe fertig.
 *
 * Check also for attached shm buffer.
 */
atrshmlog_ret_t atrshmlog_get_logging(void)
{
  if (!ATRSHMLOG_LOGGING)
    return atrshmlog_error_get_logging_1;

  if(atrshmlog_thread_fence_10)
    atomic_thread_fence(memory_order_acquire); // ok

  atrshmlog_area_t * a = ATRSHMLOG_GETAREA;

  if (atomic_load(&a->ich_habe_fertig) != 0)
    return atrshmlog_error_get_logging_2;
  
  if (atrshmlog_logging_process_off_final != 0)
    return atrshmlog_error_get_logging_3;

  if (atrshmlog_logging_process == 0)
    return atrshmlog_error_get_logging_4;

  return atrshmlog_error_ok;
}

