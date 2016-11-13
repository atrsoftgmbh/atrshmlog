
#include "../atrshmlog_internal.h"

/*******************************************************************/

/**
 * \file atrshmlogimpl_set_advance_on.c
 */

/** 
 * \n Main code:
 *
 * \brief we turn the init in advance on
 *
 */
atrshmlog_ret_t atrshmlog_set_init_buffers_in_advance_on(void)
{
  ATRSHMLOGSTAT(atrshmlog_counter_init_in_advance_on);
  
  atrshmlog_ret_t old = atrshmlog_init_buffers_in_advance;

  atrshmlog_init_buffers_in_advance = 1;

  return old;
}

