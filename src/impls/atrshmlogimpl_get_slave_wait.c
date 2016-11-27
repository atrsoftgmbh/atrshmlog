
#include "../atrshmlog_internal.h"


/*******************************************************************/

/**
 * \file atrshmlogimpl_get_slave_wait.c
 */

/** 
 * \n Main code:
 *
 * \brief Get the wait time a slaves is in sleep in nanos
 * when the list was empty at the test.
 *
 * test t_get_f_list_buffer_slave_wait.c
 */
atrshmlog_ret_t atrshmlog_get_f_list_buffer_slave_wait(void)
{
  return atrshmlog_f_list_buffer_slave_wait;
}

