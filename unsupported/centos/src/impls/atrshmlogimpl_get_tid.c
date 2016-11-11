
#include "../atrshmlog_internal.h"


/*******************************************************************/

/**
 * \file atrshmlogimpl_get_tid.c
 */

/** 
 * \n Main code:
 *
 * \brief We get the thread tid
 *
 */
 atrshmlog_tid_t atrshmlog_get_tid(void)
 {
   atrshmlog_tid_t t;

   t = ATRSHMLOG_GETTHREADID;

   return t;
 }

