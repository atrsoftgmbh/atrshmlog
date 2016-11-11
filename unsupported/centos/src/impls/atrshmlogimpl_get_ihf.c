
#include "../atrshmlog_internal.h"


/*******************************************************************/

/**
 * \file atrshmlogimpl_get_ihf.c
 */

/**
 * \n Main code:
 *
 * \brief Get the ich habe fertig flag
 */
atrshmlog_ret_t atrshmlog_get_area_ich_habe_fertig(volatile const void* i_area)
{
  if (i_area && atrshmlog_attach_once != 0)
    return atomic_load(&((volatile const atrshmlog_area_t *)i_area)->ich_habe_fertig);
  
  return 0;
}
