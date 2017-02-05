
#include "../atrshmlog_internal.h"


/********************************************************************/

/**
 * \file atrshmlogimpl_slave_list.c
 */

/**
 * \n Main code:
 *
 * \brief The anchor of the slave list ( slaves )
 *
 * We hold a list of slaves.
 *
 * So the program can iterate and decide to kill it.
 *
 * To do so it must either use an external kill via thread id
 * which it gets with the iteration
 * or switch the slave to stop via flag.
 */
_Alignas(128) atomic_intptr_t atrshmlog_tpslave = ATOMIC_VAR_INIT(0);

