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


/** \file atrshmlogdelete.c
 * \brief We are the deleter of the shared memory.
 *
 * WE give free the shared memory. 
 * There is no testing so this is only for the last resort
 * if you need a fresh creation of an alredy set buffer.
 * Simply delete and then create it.
 */


#include "atrshmlog.h"

#include "atrshmlog_internal.h"


/*
 *---------------------------------------------------------
 */

/**
 * get rid of the shm buffer
 * give the shm id and the thing removes the buffer 
 * there is no cleanup.
 * so your mutex / futex / semaphores / whatsoever 
 * will be responsible for a clean remove by themselfs
 */
int main (int argc, char*argv[])
{
  long lshmid;

  if (argc != 2)
    {
      printf("usage: %s shmid\n", argv[0]);
      printf("logsystem version is %d.\n", ATRSHMLOGVERSION);
      exit(1);
    }

  lshmid = strtol(argv[1],0,10);

  printf("shm log delete.\n");
  printf("shmid is %d\n", (int)lshmid);
  printf("logsystem version is %d.\n", ATRSHMLOGVERSION);

  {
    int shmid = (int)lshmid;
    
    int delete_result = ATRSHMLOG_DELETE(shmid);

    if (delete_result < 0)
      {
    printf("error %d , errno %d.\n", delete_result, errno);
    printf("if your shmid is wrong use ipcs -a to find out whats is there...\n");
    exit(1);
      }

    printf("\n\nunset %s_ID\n\n", ATRSHMLOG_GET_ENV_PREFIX());
  }
  
  return 0;
}

/* end of file */
