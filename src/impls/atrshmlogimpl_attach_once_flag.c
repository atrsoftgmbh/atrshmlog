
#include "../atrshmlog_internal.h"


/**
 * \file atrshmlogimpl_attach_once_flag.c
 */

/**
 * \n Main code:
 *
 * \brief We are the flag for attach has been done
 *
 */
_Alignas(128) volatile int atrshmlog_attach_once = 0;

