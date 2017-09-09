
#include "../atrshmlog_internal.h"


/**
 * \file atrshmlogimpl_autoflush_flag.c
 */

/**
 * \n Main code:
 *
 * \brief We have a autoflush flag.
 *
 * So we can try at least to get the log directly into memory.
 * This is still a job for the slave, but its better than nothing.
 *
 * Be sure you understand the performance problems this makes.
 * Switch it on only for the things you need to do this.
 * this is the per process switch.
 * We have also a thread switch.
 * The thread starts with this as default.
*/
_Alignas(128) int atrshmlog_autoflush = 0;

