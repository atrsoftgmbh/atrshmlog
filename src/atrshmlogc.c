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

/** \file atrshmlogc.c 
 * \brief The implementation of the module
 *
 * Today we only hold the thread local and its getter.
 * In case we do not have thread local we try the thread specific for 
 * pthread.
 *
 * The rest of the module is now in impls.
 */

/* our includes are here */

#include "atrshmlog_internal.h"

/* end of includes */

/********************************************************************/

#if ATRSHMLOG_THREAD_LOCAL == 1

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
 *
 * test t_get_tid.c
 */
volatile const void* atrshmlog_get_thread_locals_adress(void)
{
  return (volatile const void*)&atrshmlog_g_tl;
}

#endif

#if ATRSHMLOG_THREAD_LOCAL == 0

extern int atrshmlog_key_once;

// the hard way. we use pthreads key and specifics here
extern pthread_key_t atrshmlog_pthread_key;

// the destructor
extern void atrshmlog_destruct_specific(void* i_data);

// this is now a new thing. we use internal the key from  - hopeful - attach.
// then we check for it and get our data ptr.
// if no ptr is there we malloc and deliver the thing
// to the caller.
// we need a destroy here, but this is for later ...
volatile const void* atrshmlog_get_thread_locals_adress(void)
{
  // are we before attach ? 
  if (atrshmlog_key_once == 0)
    {
      int ret = pthread_key_create(&atrshmlog_pthread_key, atrshmlog_destruct_specific);

      ++atrshmlog_key_once; 

      if (ret != 0)
	{

	  // sorry pal - but we are simply out of options.

	  // no more tries
	
	  return 0;
	}
    }
  
  void *my_tls = pthread_getspecific(atrshmlog_pthread_key);

  if (my_tls == NULL)
    {
      atrshmlog_g_tl_t * p = (atrshmlog_g_tl_t*)calloc(1, sizeof(atrshmlog_g_tl_t));

      if (p)
	{
	  p->atrshmlog_idnotok = -1;

	  int ret = pthread_setspecific(atrshmlog_pthread_key, (const void*)p);

	  if (ret == 0)
	    {
	      // ok. we have at least now a valid buffer.
	      return (volatile const void*)p;
	    }
	  else
	    {
	      free(p);

	      return 0;
	    }
	}
      else
	{
	  return 0;
	}
    }
  else
    {
      return (volatile const void*)my_tls;
    }
}

#endif


/*******************************************************************/



/* end of file */
