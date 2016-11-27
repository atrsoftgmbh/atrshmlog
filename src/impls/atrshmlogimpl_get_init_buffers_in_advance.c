
#include "../atrshmlog_internal.h"


/*******************************************************************/

/**
 * \file atrshmlogimpl_get_init_buffers_in_advance.c
 */

/** 
 * \n Main code:
 *
 * \brief we get the init in advance flag
 *
 * test t_get_init_buffers_in_advance.c
 */
atrshmlog_ret_t atrshmlog_get_init_buffers_in_advance(void)
{
  atrshmlog_ret_t old = atrshmlog_init_buffers_in_advance;

  return old;
}

