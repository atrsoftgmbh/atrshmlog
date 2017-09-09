
#include "../atrshmlog_internal.h"

/**********************************************************/

/**
 * \file atrshmlogimpl_set_slave_to_shm_wait.c
 */

/**
 * \n Main code:
 *
 * \brief We set the wait time 
 *
 * \param i_nanos
 * new wait time
 *
 * test t_slave_to_shm_wait.c
 */
atrshmlog_ret_t atrshmlog_set_slave_to_shm_wait(const atrshmlog_int32_t i_nanos)
{
  int old = atrshmlog_slave_to_shm_wait;

  if (i_nanos >= 0
      && i_nanos < 1000000000)
    {
      atrshmlog_slave_to_shm_wait = i_nanos;
    }

  return old;
}

