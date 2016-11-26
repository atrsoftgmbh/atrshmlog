
#include "../atrshmlog_internal.h"

/*******************************************************************/

/**
 * \file atrshmlogimpl_get_shmid.c
 */

/**
 * \n Main code:
 *
 * \brief The shmid according to attach and the used env variable
 * 
 * test t_get_shmid.c
 */
int atrshmlog_get_shmid(void)
{
  return atrshmlog_id;
}

