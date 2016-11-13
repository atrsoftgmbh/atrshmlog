
#include "../atrshmlog_internal.h"


/********************************************************************/

/**
 * \file atrshmlogimpl_active_slaves_flag.c
 */

/** 
 * \n Main code:
 *
 * \brief The number of active slave threads.
 *
 * Be carefull to maintain this if you
 * KILL threads.
 */
atomic_int atrshmlog_f_list_active_slaves = ATOMIC_VAR_INIT(0);

