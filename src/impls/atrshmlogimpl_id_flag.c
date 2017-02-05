
#include "../atrshmlog_internal.h"


/********************************************************************/

/**
 * \file atrshmlogimpl_id_flag.c
 */

/** 
 * \n Main code:
 *
 * \brief We save the shm id here for checks.
 *
 * We get it from the attach op.
 * Its the ID from the environment.
 *
 * This can be different to the shm id in the buffer ( which indicate 
 * an error - we never should use a shm buffer with the wrong id
 * because it is a strong hint someone gave a wrong shmid to the init
 * and we dont use that shm then ).
 *
 * Use this as it where readonly.
 */
_Alignas(128) volatile int atrshmlog_id = 0;

