
#include "../atrshmlog_internal.h"


/**
 * \file atrshmlogimpl_get_f_list_buffer_slave_count.c
 */

/**
 * \n Main code:
 *
 * \brief The number of slave threads for the process is delivered.
 */ 
atrshmlog_ret_t atrshmlog_get_f_list_buffer_slave_count(void)
{
  return atrshmlog_f_list_buffer_slave_count;
}
