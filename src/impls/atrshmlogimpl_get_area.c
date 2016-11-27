
#include "../atrshmlog_internal.h"

/******************************************************************/

/**
 * \file atrshmlogimpl_get_area.c
 */

/** 
 * \n Main code:
 *
 * \brief Get the start of the shm area with the shm buffers
 *
 * test t_get_area.c
 */
volatile const void* atrshmlog_get_area(void)
{
  return  atrshmlog_base_ptr;
}

