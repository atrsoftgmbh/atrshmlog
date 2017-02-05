
#include "../atrshmlog_internal.h"


/********************************************************************/

/**
 * \file atrshmlogimpl_clock_id_flag.c
 */

/** 
 * \n Main code:
 *
 * \brief The  clock id.
 *
 * - 0 : null
 * - 1 : tsc
 * - 2 : tsc and a lfence
 * - 3 : par tsc
 *
 * We start with 1, set it to a better value with the setter
 * We have to expose this because we use inline code - at least we try
 * in the gettime thing.
 *
 * There is an env in attach to set this.
 */
_Alignas(128) volatile  int atrshmlog_clock_id = 1; 

