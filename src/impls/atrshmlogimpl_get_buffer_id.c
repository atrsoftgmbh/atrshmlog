
#include "../atrshmlog_internal.h"

/*******************************************************************/

/**
 * \file atrshmlogimpl_get_buffer_id.c
 */

/** 
 * \n Main code:
 *
 * \brief The last id given to a buffer.
 *
 * This should be the same as the acquire count.
 * 
 * test t_get_buffer_id.c
 */
atrshmlog_ret_t atrshmlog_get_buffer_id(void)
{
  int ret =  atrshmlog_buffer_id;

  return ret;
}

