
#include "../atrshmlog_internal.h"


/********************************************************************/

/**
 * \file atrshmlogimpl_slave_shm_wait_flag.c
 */

/**
 * \n Main code:
 *
 * \brief We wait this long if we have no shm buffer in the mem to shm function.
 * 
 * So we have at least no busy wait here.
 *
 */
_Alignas(128) int atrshmlog_slave_to_shm_wait  = ATRSHMLOG_INIT_SLAVE_TO_SHM_WAIT;

