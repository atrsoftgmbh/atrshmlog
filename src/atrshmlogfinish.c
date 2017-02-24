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


/** \file atrshmlogfinish.c
 * \brief We give any resource free in the shared memory buffer.
 *
 * This is needed if we use resources that are high level constucts like
 * Mutexes or condition variables.
 */


#include "atrshmlog.h"

#include "atrshmlog_internal.h"


#include <stdio.h>

#include <stdlib.h>

/*
 *---------------------------------------------------
 */

/**
 * attach and then log off from the internal locks
 * this means you destroy themutex / futex / semaphore / whatever
 * that synchronise the buffer.
 * you should not use it then any more.
 * reinit it if you want - perhaps you need to switch some flag values 
 * so you can use defekt for this.
 * in practise this is only needed if your os cannot cleanup the resources
 * of the locks if you remove the buffer before this step.
 * on a normal os this should be not needed.
 */
int main (int argc, char*argv[])
{
  const char *shmid_ptr = ATRSHMLOG_GET_ENV_SHMID();

  if (shmid_ptr == (void*)0)
    {
      printf("usage: %s\n", argv[0]);
      printf("ENVIRONMENT VARIABLE NOT SET !!!\n");
      printf("%s_ID\n",ATRSHMLOG_GET_ENV_PREFIX());
      printf("logsystem version is %d.\n", ATRSHMLOGVERSION);
      exit(1);
    }

  if (argc != 1)
    {
      printf("usage: %s\n", argv[0]);
      printf("parameter count wrong.\n");
      printf("logsystem version is %d.\n", ATRSHMLOGVERSION);
      exit(1);
    }

  printf("shm log attach and finish mutexe.\n");
  printf("logsystem version is %d.\n", ATRSHMLOGVERSION);
  
  ATRSHMLOG_ATTACH();

  if (!ATRSHMLOG_LOGGING)
    {
      printf ("attach not successful.\ncheck ENVIRONMENT VARIABLE %s_ID\n"
          "Value seems not to fit : '%s'\n", ATRSHMLOG_GET_ENV_PREFIX(),shmid_ptr);

      exit(1);
    }

  fflush(stdout);

  volatile const void* a = ATRSHMLOG_GET_AREA();
  
  ATRSHMLOG_CLEANUP_LOCKS(a);

  return 0;
}

/* end of file */
