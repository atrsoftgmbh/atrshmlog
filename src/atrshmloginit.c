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


/** \file atrshmloginit.c
 * \brief We initialize an naket shared memory buffer.
 *
 * We attach and then check if the buffer is naket.
 * We then initialize all needed variables in the buffer.
 */

#include "atrshmlog.h"

#include "atrshmlog_internal.h"

#include <stdio.h>

#include <errno.h>

#include <stdlib.h>

/*
 * --------------------------------------------------------------
 */


/**
 * attach and then init the shm log buffers
 * we need the count of buffers for that.
 * so dont be upset.
 * yes, you have given the create the count - but we dont init
 * the buffer in that.
 * so we dont have info about that.
 * yes we could use the env like in case of the shmid - but ..
 * this would be realy unnecessary . simply give the count here
 * yes, we could have used part of the allocated shm for that - but ..
 * if there is ever a connect to a not initialized buffer - you dont have
 * to trust the info in the buffer - it could be random garbage ... 
 * so again. give the number here please.
 * if you are really annoyed, use a script to combine create and init.
 * thats what scripts are for.
 * programs should do the best and minimum for a unx system.
 * keep it simple and that stuff ....
 * so the init really does not know about the size - you give it as a 
 * paramter. 
 */
int main (int argc, char*argv[])
{
  const char *shmid_ptr = ATRSHMLOG_GET_ENV_SHMID();

  long lanz ;
  
  if (shmid_ptr == (void*)0)
    {
      printf("usage: %s countbuffer\n", argv[0]);
      printf("ENVIRONMENT VARIABLE NOT SET !!!\n");
      printf("%s_ID\n",ATRSHMLOG_GET_ENV_PREFIX());
      printf("logsystem version is %d.\n", ATRSHMLOGVERSION);
      exit(1);
    }

  const char *s_anz;
  
  if (argc != 2)
    {
      if (ATRSHMLOG_GET_ENV("_COUNT"))
	{
	  s_anz = ATRSHMLOG_GET_ENV("_COUNT");
	}
      else
	{
	  printf("usage: %s countbuffer\n", argv[0]);
	  printf("parameter count wrong.\n");
	  printf("logsystem version is %d.\n", ATRSHMLOGVERSION);
	  exit(1);
	}
    }
  else
    {
      s_anz = argv[1];
    }
  
  lanz = strtol(s_anz, 0, 10);
  
  printf("shm log attach and init.\n");
  printf("logsystem version is %d.\n", ATRSHMLOGVERSION);
  
  ATRSHMLOG_ATTACH();

  if (!ATRSHMLOG_LOGGING)
    {
      printf ("attach not successful.\ncheck ENVIRONMENT VARIABLE %s_ID\n"
          "Value seems not to fit : '%s'\n", ATRSHMLOG_GET_ENV_PREFIX(),shmid_ptr);

      exit(1);
    }

  fflush(stdout);
  
  {
    int anzahl_buffer = (int)lanz;

    const volatile void * a = ATRSHMLOG_GET_AREA();

    int init_result = ATRSHMLOG_INIT_SHM_LOG(a, anzahl_buffer);

    if (init_result != 0)
      {
    printf("init error %d. errno %d\n",  init_result, errno);

      }
  }

  return 0;
}

/* end of file */
