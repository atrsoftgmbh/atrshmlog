
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
 */
atrshmlog_tid_t atrshmlog_get_slave_tid (volatile const void *i_local)
{
  atrshmlog_slave_t *i = (atrshmlog_slave_t*) i_local;

  if (i == NULL)
    return 0;

  return i->tid;
}

