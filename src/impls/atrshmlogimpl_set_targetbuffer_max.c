
#include "../atrshmlog_internal.h"

/*******************************************************************/

/**
 * \file atrshmlogimpl_set_targetbuffer_max.c
 */

/** 
 * \n Main code:
 *
 * \brief Set the targetbuffer max
 *
 * test t_set_checksum.c
 */
atrshmlog_ret_t atrshmlog_set_targetbuffer_max(int i_flag)
{
  int old  = atrshmlog_targetbuffer_max;

  if (i_flag >= 2 && i_flag <= ATRSHMLOGTARGETBUFFERMAX)
      atrshmlog_targetbuffer_max = i_flag;

  return old;
}
