
#include "../atrshmlog_internal.h"




/********************************************************************/

/**
 * \file atrshmlogimpl_statistics_flag.c
 */

/** 
 * \n Main code:
 *
 * \brief We can switch statistics on if needed
 *
 * So we have here the flag. We do start off.
 * Statistics are a inter thread locking thing.
 * so we have to use it only in test and development.
 *
 */
/********************************************************************/
int atrshmlog_statistics_on = 0;

