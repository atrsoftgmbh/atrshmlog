
#include "../atrshmlog_internal.h"


/*******************************************************************/

/**
 * \file atrshmlogimpl_set_f_list_buffer_slave_count.c
 */

/**
 * \n Main code:
 *
 * \brief We can set the count of slave threads.
 *
 * This has to be done before attach.
 * Be sure you understand the consequences.
 * If no slaves are running and you hit the buffer size
 * your buffer will be dispatched, but not cleared.
 * If you hit it twice your process is spinning. 
 * So this is only ok if you know that you have little logging
 * and dont want to have additional threads.
 * In case you need more set it to a save limit, but consider
 * the maximum count of shm buffers and the number of threads of the
 * reader process too.
 * If you only raise this you shift the problem to the shm
 * and in the latter case to the reader.
 * The reader has not only to move the mem, but
 * also to write it to disk - so this can be the real limit.
 * If you see the buffer number in the log at the shm buffer count 
 * you need to set the reader threads number higher.
 * After this you can raise the slaves count with this before 
 * you attach ...
 * 
 * There is an env for attach to set this.
 */
atrshmlog_ret_t atrshmlog_set_f_list_buffer_slave_count(atrshmlog_int32_t i_count)
{
  ATRSHMLOGSTAT(atrshmlog_counter_set_slave_count);
  
  int old = atrshmlog_f_list_buffer_slave_count;
  
  if (i_count >= 0 && atrshmlog_attach_once == 0)
    atrshmlog_f_list_buffer_slave_count = i_count;
  
  return old;
}
