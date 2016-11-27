
#include "../atrshmlog_internal.h"


/*******************************************************************/

/**
 * \file atrshmlogimpl_decrement_slave_count.c
 */

/** 
 * \n Main code:
 *
 * \brief We decrement the slave count.
 * 
 * We deliver the old one and decrement. Till 0.
 * This is for the case you kill slaves.
 *
 * test t_decrement_slave_count.c
 */
atrshmlog_ret_t atrshmlog_decrement_slave_count(void)
{
  int old = atrshmlog_f_list_active_slaves;

  if (atrshmlog_f_list_active_slaves > 0)
    atrshmlog_f_list_active_slaves--;

  return old;
}

