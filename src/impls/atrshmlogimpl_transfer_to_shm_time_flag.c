
#include "../atrshmlog_internal.h"

/********************************************************************/

/**
 * \file atrshmlogimpl_transfer_to_shm_time_flag.c
 */

/** 
 * \n Main code:
 *
 * \brief We have for the adaptive strategy a default for start.
 *
 * This is used when we init a buffer.
 * After the first transfer its gone.
 */
_Alignas(128) atrshmlog_time_t atrshmlog_transfer_to_shm_time = ATRSHMLOG_TRANSFER_TO_SHM_START_DEFAULT;

