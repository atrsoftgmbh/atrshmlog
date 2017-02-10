
#include "../atrshmlog_internal.h"


/********************************************************************/

/**
 * \file atrshmlogimpl_targetbuffer_max.c
 */

/** 
 * \n Main code:
 *
 * \brief We set this many buffers in a thread for logging
 *
 * We can set this on attach again and on reattach
 *
 * We are conservative about memory, so it is the smallest
 * possible value at start.
 *
 * Use this as it where readonly in the process.
 */
_Alignas(128) int atrshmlog_targetbuffer_max = 2;

