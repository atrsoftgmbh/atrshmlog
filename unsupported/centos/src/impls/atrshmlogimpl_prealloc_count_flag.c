
#include "../atrshmlog_internal.h"


/********************************************************************/

/**
 * \file atrshmlogimpl_prealloc_count_flag.c
 */

/**
 * \n Main code:
 *
 * \brief We use this many buffers to be allocated at once in case
 * we need new buffers from the dyn memory area.
 *
 * So we can reduce the overhead by allocate a bunch in one call.
 * This is ok if you need to use the module without recompile.
 * Its better to adjust the static buffers for the performance
 * if you can do the recompile.
 *
 * There is an env in attach to set this.
 */
int atrshmlog_prealloc_buffer_count = ATRSHMLOG_INIT_PREALLOC_COUNT;

