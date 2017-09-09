
#include "../atrshmlog_internal.h"

/***************************************************************/

/**
 * \file atrshmlogimpl_get_slave_tid.c
 */

/** 
 * \n Main code:
 *
 * \brief We get the tid of a slave
 *
 * test t_buffer_slave_proc.c
 */
atrshmlog_tid_t atrshmlog_get_slave_tid (volatile const void * const i_local)
{
  atrshmlog_slave_t *i = (atrshmlog_slave_t*) i_local;

  if (i == NULL)
    return 0;

  return i->tid;
}

