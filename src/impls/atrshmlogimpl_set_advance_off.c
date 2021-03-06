
#include "../atrshmlog_internal.h"

/*******************************************************************/

/**
 * \file atrshmlogimpl_set_advance_off.c
 */

/** 
 * \n Main code:
 *
 * \brief we turn the init in advance on
 *
 * test t_set_advance_off.c
 */
atrshmlog_ret_t atrshmlog_set_init_buffers_in_advance_off(void)
{
  ATRSHMLOGSTAT(atrshmlog_counter_init_in_advance_off);
  
  atrshmlog_ret_t old = atrshmlog_init_buffers_in_advance;

  atrshmlog_init_buffers_in_advance = 0;

  return old;
}

