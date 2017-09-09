
#include "../atrshmlog_internal.h"


/********************************************************************/

/**
 * \file atrshmlogimpl_wait_for_slaves_flag.c
 */

/**
 * \n Main code:
 *
 * \brief This is a flag to wait in atexit for end of all slaves.
 *
 * Normally we dont do this. But if the cleanup
 * seems to interact with still running slaves and you 
 * want to solve this you can set it to non zero.
 *
 * Then you will wait in the atexit till the count of slaves
 * is 0 . 
 *
 * There is an env in attach to set this.
 */
_Alignas(128) int atrshmlog_wait_for_slaves = 0;

