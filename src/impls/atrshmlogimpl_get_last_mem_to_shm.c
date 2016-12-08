
#include "../atrshmlog_internal.h"


/*******************************************************************/

/**
 * \file atrshmlogimpl_get_last_mem_to_shm.c
 */

/** 
 * \n Main code:
 *
 * \brief the time in click we needed last to shm transfer
 *
 * test t_get_last_mem_to_shm.c
 */
atrshmlog_ret_t atrshmlog_get_last_mem_to_shm(void)
{
  return atomic_load(&atrshmlog_last_mem_to_shm);
}

