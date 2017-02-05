
#include "../atrshmlog_internal.h"


/*******************************************************************/

/**
 * \file atrshmlogimpl_set_ihf.c
 */

/**
 * \n Main code:
 *
 * \brief Set the ich habe fertig flag .
 *
 * This is 0 for go on, non 0 for stop.
 *
 * test t_set_area_ihf.c
 */
atrshmlog_ret_t atrshmlog_set_area_ich_habe_fertig(volatile const void* i_area, atrshmlog_int32_t i_flag)
{
  if (!(i_area && atrshmlog_attach_once != 0))
    return atrshmlog_error_area_ich_habe_fertig_1;
  
  int ret = ((volatile atrshmlog_area_t *)i_area)->ich_habe_fertig;

  // we cast const away. const is used to hinder the user of the module to
  // accidently write. but this function really has to write.
  // so we use a bad thing here, cast const away...
  ((volatile atrshmlog_area_t *)i_area)->ich_habe_fertig = i_flag;

  return ret;
}

