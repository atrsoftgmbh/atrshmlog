
#include "../atrshmlog_internal.h"


/*******************************************************************/

/**
 * \file atrshmlogimpl_slave_run_off.c
 */

/**
 * \n Main code:
 *
 * \brief Switch off the slaves.
 *
 * This is no kill op, so you have to wait for threads that are running
 * to end their work.
 *
 * This can cost time if the number of threads is high,
 * the waiting number of buffers is high and reader is not 
 * coming up. so use it as a "i do it in the near future"
 * wish. 
 * 
 * It is NOT working directly after the call is made
 *
 * test t_exit_cleanup.c
 */
void atrshmlog_set_f_list_buffer_slave_run_off(void)
{
  ATRSHMLOGSTAT(atrshmlog_counter_slave_off);

  atrshmlog_f_list_buffer_slave_run = 0;
}

