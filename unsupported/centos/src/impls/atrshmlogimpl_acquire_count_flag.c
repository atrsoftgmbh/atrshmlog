
#include "../atrshmlog_internal.h"

/*******************************************************************/

/**
 * \file atrshmlogimpl_acquire_count_flag.c
 */

/** 
 * \n Main code:
 *
 * \brief The number of buffers we have - theoretically  - in use now.
 *
 * At start its the number of buffers on the static buffer list.
 * Later on this can be growing in case we need dyn memory .
 */
volatile int atrshmlog_acquire_count = ATRSHMLOGBUFFER_PREALLOCED_COUNT;

