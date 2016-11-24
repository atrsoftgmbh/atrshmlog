
#include "../atrshmlog_internal.h"

/*******************************************************************/

/**
 * \file atrshmlogimpl_set_checksum.c
 */

/** 
 * \n Main code:
 *
 * \brief Set the checksum flag
 */
atrshmlog_ret_t atrshmlog_set_checksum(int i_flag)
{
  int old  = atrshmlog_checksum;

  atrshmlog_checksum = i_flag ? 1 : 0;

  return old;
}
