
#include "../atrshmlog_internal.h"

/********************************************************************/

/**
 * \file atrshmlogimpl_delimiter_flag.c
 */

/**
 * \n Main code:
 *
 * \brief Our delimiter character for the argv string parts in write2.
 *
 * There is an env in attach to set this.
 */
_Alignas(128) int atrshmlog_delimiter = ' ';

