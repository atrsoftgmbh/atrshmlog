
#include "../atrshmlog_internal.h"

/*******************************************************************/

/**
 * \file atrshmlogimpl_get_area_version.c
 */

/**
 * \n Main code:
 *
 * \brief Get the major version from shm.
 *
 * This has to be the same as the programs major or we should stop at once
 *
 * test t_get_area_version.c
 */
atrshmlog_ret_t atrshmlog_get_area_version(volatile const void* const i_area)
{
  if (i_area && atrshmlog_attach_once != 0)
    return ((volatile const atrshmlog_area_t *)i_area)->shmversion;

  return atrshmlog_error_area_version_1;
}

