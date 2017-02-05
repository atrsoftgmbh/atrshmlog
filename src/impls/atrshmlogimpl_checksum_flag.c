
#include "../atrshmlog_internal.h"


/**
 * \file atrshmlogimpl_checksum_flag.c
 */

/**
 * \n Main code:
 *
 * \brief We have a checksum flag.
 *
 * So we can set it and in mem to shm we calc a checksum then.
 */
_Alignas(128) int atrshmlog_checksum = 0;

