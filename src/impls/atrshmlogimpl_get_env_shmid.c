
#include "../atrshmlog_internal.h"

/*******************************************************************/

/**
 * \file atrshmlogimpl_get_env_shmid.c
 */

/**
 * \n Main code:
 *
 * \brief Get the environment value for the shm id
 */
const char* atrshmlog_get_env_shmid(void)
{
  return atrshmlog_get_env(ATRSHMLOGENVSUFFIX);
}

