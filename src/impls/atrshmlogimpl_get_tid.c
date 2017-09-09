
#include "../atrshmlog_internal.h"

/** All the rest of unix */
# include <unistd.h>


/*******************************************************************/

/**
 * \file atrshmlogimpl_get_tid.c
 */

/** 
 * \n Main code:
 *
 * \brief We get the thread tid
 *
 * test t_get_tid.c
 */
 atrshmlog_tid_t atrshmlog_get_tid(void)
 {
   atrshmlog_tid_t t;

   ATRSHMLOG_GETTHREADID(t);

   return t;
 }

