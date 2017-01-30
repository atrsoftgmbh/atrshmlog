
#include "../atrshmlog_internal.h"

/**********************************************************/

/**
 * \file atrshmlogimpl_get_slave_to_shm_wait.c
 */

/**
 * \n Main code:
 *
 * \brief We get the nanos to wait
 *
 * test t_slave_to_shm_wait.c
 */
atrshmlog_ret_t atrshmlog_get_slave_to_shm_wait(void)
{
  int old = atrshmlog_slave_to_shm_wait;

  return old;
}

