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

/** \file atrshmlogfork.c
 * \brief We are a test tool to log and do the fork thing.
 */

#include "../atrshmlog.h"


#include "../atrshmlog_internal.h"


#if  ATRSHMLOG_PLATFORM_MINGW_X86_64_GCC == 1

// TODO make this work on mingw
// sorry, but we dont try this here. its a test program after all, but
// for now simply ignore it.

#define fork()  (0)

#define getppid() (0)

#endif

#include <stdio.h>

#include <errno.h>

#include <stdlib.h>

/** All the rest of unix */
# include <unistd.h>

/*
 *------------------------------------------------
 */

/**
 * attach and verify the struct parameters
 * then fork and play a little game with the shm to synchronize 
 * the two processes
 * after this the two processes should stop both.
 */
int main (int argc, char*argv[])
{
  const char *shmid_ptr = ATRSHMLOG_GET_ENV_SHMID();

  long lanz;
  
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
  
  printf("shm log attach and verify.\n");
  printf("logsystem version is %d.\n", ATRSHMLOGVERSION);
  
  ATRSHMLOG_ATTACH();

  if (!ATRSHMLOG_LOGGING)
    {
      printf ("attach not successful.\ncheck ENVIRONMENT VARIABLE %s_ID\n"
          "Value seems not to fit : '%s'\n", ATRSHMLOG_GET_ENV_PREFIX(),shmid_ptr);

      exit(1);
    }

  fflush(stdout);


  int cpid = fork();
  

  if (cpid == 0)
    {
      /* we are the father  */
      printf("father still alive \n");
      fflush(stdout);
    }

  if (cpid != 0)
    {
      /* we are the child  */
      printf("child still alive \n");
      fflush(stdout);
      
    }
  
  {
    int v_result = ATRSHMLOG_VERIFY();

    if (v_result != 0)
      {
    printf("%d verify error %d. errno %d\n", cpid, v_result, errno);
    printf("\n%ld : no attach\n"
           "%ld : shmid not same in log\n"
           "%ld : safeguard invalid in log\n"
           "%ld : lastused to big\n"
           "%ld : version wrong\n"
           "%ld ... : buffer invalid\n",
	   (long)atrshmlog_error_verify_1,
	   (long)atrshmlog_error_verify_2,
	   (long)atrshmlog_error_verify_3,
	   (long)atrshmlog_error_verify_4,
	   (long)atrshmlog_error_verify_5,
	   (long)atrshmlog_error_verify_6
	   );
      }

    if(v_result == atrshmlog_error_verify_5)
      exit(1);
    
    {
      atrshmlog_area_t * a = (atrshmlog_area_t *)ATRSHMLOG_GET_AREA();
      
      int anzahl_buffer = (int)lanz;
      int anzahl_shm = a->shmcount;
      int i = 0;
      
      if (anzahl_buffer != anzahl_shm)
	printf("%d count buffer wrong: expect is %d, in shm is %d.\n",
	       cpid, anzahl_buffer, anzahl_shm);

      if (a->shmsafeguard != ATRSHMLOGSAFEGUARDVALUE)
	printf("%d safeguard wrong : 0X%lx , expect 0X%lx\n",
	       cpid, a->shmsafeguard, ATRSHMLOGSAFEGUARDVALUE);
      
      for (i = 0; i < anzahl_shm ; i++)
    {
      int state = atomic_load(&a->logbuffers[i].state);
      long size;
          
      if (!(state == atrshmlog_writeable || state == atrshmlog_full))
        {
          printf("%d error in buffer %d : state is %d.\n"
             "  allowed %d, %d, %d, %d.\n",
             cpid,
             i,
             state,
             atrshmlog_uninit,
             atrshmlog_writeable,
             atrshmlog_full,
             atrshmlog_illegal);
        }

      if (a->logbuffers[i].safeguard != ATRSHMLOGSAFEGUARDVALUE)
        {
          printf("%d error in buffer %d, safeguard is 0X%lx, expect 0X%lx.\n",
             cpid,
             i,
             a->logbuffers[i].safeguard,
             ATRSHMLOGSAFEGUARDVALUE);
        }

      size = a->logbuffers[i].shmsize;

      printf("%d buffer %d : size is %ld, limit %d\n",
         cpid,
         i,
         size,
	 ATRSHMLOGBUFFER_INFOSIZE);
    }

  if (cpid == 0)
    {
      /* we are the child */
      printf("father has pid %d\n", getppid());
      
      while (a->readerpid != getpid())
    ;
      printf("got from father %d\n", a->readerflag);
      fflush(stdout);
      a->readerpid = 0;
      a->readerflag = 0;
    }
  
  if (cpid != 0)
    {
      /* we are the father */
      a->readerflag =4711;
      a->readerpid = cpid;

      printf("sended to client %ld.\n" , (long)a->readerpid);

      while (a->readerpid != 0)
    ;
    }
    }


  }


  if (cpid != 0)
    {
      /* we are the father  */
      printf("father still alive \n");
      fflush(stdout);

      // we no longer log
      ATRSHMLOG_STOP();
      
    }
  
  if (cpid == 0)
    {
      /* we are the child  */
      printf("child still alive \n");
      fflush(stdout);

      // we no longer log
      ATRSHMLOG_STOP();

    }

  return 0;
}

/* end of file */
