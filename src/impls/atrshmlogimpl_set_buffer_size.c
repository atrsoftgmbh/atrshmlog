
#include "../atrshmlog_internal.h"

/*******************************************************************/

/**
 * \file atrshmlogimpl_set_buffer_size.c
 */

/**
 * \n Main code:
 *
 * \brief We can reduce the buffer size here.
 *
 * This works from that moment on for all
 * buffers new alloced, so be sure to do it 
 * at the very beginning.
 *
 * Its bad to have buffers to be too small,
 * so reduce also the loging payload ....
 *
 * You cannot increase .
 * You cannot change the static allocated buffers sizes.
 *
 * test t_set_buffer_size.c
 */
atrshmlog_ret_t atrshmlog_set_buffer_size(const atrshmlog_int32_t i_size)
{
  ATRSHMLOGSTAT(atrshmlog_counter_set_buffer_size);

  int old = atrshmlog_buffer_infosize;

  if (i_size > 0 && i_size <= old)
    atrshmlog_buffer_infosize = i_size;

  return old;
}

