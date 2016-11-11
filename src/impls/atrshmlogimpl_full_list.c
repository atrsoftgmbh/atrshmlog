
#include "../atrshmlog_internal.h"


/********************************************************************/

/**
 * \file atrshmlogimpl_full_list.c
 */

/** 
 * \n Main code:
 *
 * \brief This is the anchor for the full list.
 *
 * We use an atomic here, so its safe to do the mt thing.
 * 
 * Its the list of full buffers that must be transfered to shm with the slave
 * threads. 
 * 
 * The slaves check this and then grab a buffer and transfer it
 * to the shm buffers.
 *
 * So we are empty at begining.
 */
atomic_intptr_t atrshmlog_tpf = ATOMIC_VAR_INIT(0);

