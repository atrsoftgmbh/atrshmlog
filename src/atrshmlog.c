/***********************************************************************
*             This software is part of the atrshmlog package           *
*                   Copyright (c) 2016  ATRSoft GmbH                   *
*                      and is licensed under the                       *
*                     Apache License, Version 2.0                      *
*                         by ATRSoft GmbH                              *
*                                                                      *
*                A copy of the License is available at                 *
*             http://www.apache.org/licenses/LICENSE-2.0               *
*                                                                      *
*               Anton Rommerskirchen <atr@atrsoft.de>                  *
*                                                                      *
***********************************************************************/

/** \file atrshmlog.c 
 * \brief The implementation of the module
 *
 * We implement The module in one file.
 * This makes thigs simple for source code inclusions and 
 * leaves all open for the user.
 * In a later version we might create multiple files and 
 * add a lib and a shared lib module too.
 *
 * This code needs at least a C11 compiler.
 * With atomics.
 * For posix systems with pthreds.
 * Rest is open as an exercise to the user.
 */

/* our includes are here */

#include "atrshmlog_internal.h"

/* end of includes */

/********************************************************************/



/**
 * \brief The one struct containing all thread locals.
 *
 * We do this because the lookup of thread locals 
 * is relative expensive compared to the use of a 
 * simple struct pointer.
 *
 * So we put all those in one struct and use this from 
 * the initial get on.
 */
_Thread_local static atrshmlog_g_tl_t atrshmlog_g_tl = { .atrshmlog_idnotok = -1,
				     .atrshmlog_targetbuffer_arr = { 0 },
				     0
                                    };


/*******************************************************************/

/** 
 * \n Main code:
 *
 * \brief We get the thread locals adress of a thread
 *
 * We use this with the tid to identify the thread and its so called
 * g_tl struct.
 *
 * We can then switch logging off for the thread with a helper
 */
volatile const void* atrshmlog_get_thread_locals_adress(void)
{
  return (volatile const void*)&atrshmlog_g_tl;
}



/*******************************************************************/



/* end of file */
