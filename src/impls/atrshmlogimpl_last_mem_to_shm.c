
#include "../atrshmlog_internal.h"


/**
 * \file atrshmlogimpl_last_mem_to_shm.c
 */

/**
 * \n Main code:
 *
 * \brief  We hold the last clicktime for a transfer of logbuffer to area.
 *
 */
_Alignas(128) atomic_int atrshmlog_last_mem_to_shm = ATOMIC_VAR_INIT(ATRSHMLOG_TRANSFER_TO_SHM_START_DEFAULT);
