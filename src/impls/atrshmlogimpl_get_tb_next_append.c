
#include "../atrshmlog_internal.h"

/***************************************************************/

/**
 * \file atrshmlogimpl_get_tb_next_append.c
 */

/** 
 * \n Main code:
 *
 * \brief We get the tid of a thread local
 *
 * test t_get_tid.c
 */
volatile const void *atrshmlog_get_thread_buffer_next_append (volatile const void *i_buffer)
{
  atrshmlog_tbuff_t* b = ( atrshmlog_tbuff_t* ) i_buffer;
  if (b == NULL)
    return 0;

  return b->next_append;
}
