
#include "../atrshmlog_internal.h"

/*******************************************************************/

/**
 * \file atrshmlogimpl_get_area_count.c
 */

/**
 * \n Main code:
 *
 * \brief Get the shm log buffer count.
 *
 * test t_get_area_count.c
 */
atrshmlog_ret_t atrshmlog_get_area_count(volatile const void* const i_area)
{
  if (i_area && atrshmlog_attach_once != 0)
    return ((volatile const atrshmlog_area_t *)i_area)->shmcount;

  return atrshmlog_error_area_count_1;
}
