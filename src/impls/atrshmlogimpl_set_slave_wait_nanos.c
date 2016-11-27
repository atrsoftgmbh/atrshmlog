
#include "../atrshmlog_internal.h"

/*******************************************************************/

/**
 * \file atrshmlogimpl_set_slave_wait_nanos.c
 */

/** 
 * \n Main code:
 *
 * \brief Set the sleep time for a slave in nanos after a 
 * test of the f list was getting empty
 *
 * test t_set_slave_wait_nanos.c
 */
atrshmlog_ret_t atrshmlog_set_f_list_buffer_slave_wait(atrshmlog_int32_t i_wait_nanos)
{
  ATRSHMLOGSTAT(atrshmlog_counter_set_slave_wait);

  int old  = atrshmlog_f_list_buffer_slave_wait;

  if (i_wait_nanos >= ATRSHMLOG_INIT_BUFFER_SLAVE_WAIT_MIN
      && i_wait_nanos <= ATRSHMLOG_INIT_BUFFER_SLAVE_WAIT_MAX)
    atrshmlog_f_list_buffer_slave_wait = i_wait_nanos;

  return old;
}

