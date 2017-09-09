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

/** \file atrshmlogoff.c
 * \brief We are the flag switcher to off for the system wide loging.
 *
 * We connect to the shared memory and switch the flag to off.
 */
#include "atrshmlog.h"

#include "atrshmlog_internal.h"

#include <stdio.h>

#include <stdlib.h>

/*
 * -----------------------------------------------
 */


/**
 * attach and switch ich_habe_fertig to 1
 * this means all programs try this before they do anything
 * with the buffer. 
 * only the simple tools ignore it and can do something 
 * its like the big red button to no longer log for all programs
 * the supporter of logging, the reader will stop its loop and exit.
 * so you cannot switch the buffer on and off.
 * at least with this version of the shm log its only a off...
 * if the reader has cleaned up the buffers it is safe to switch the log
 * on again and to use it for the next run.
 * if no reader was attached the content is still intact.
 * so you can incremental log ...
 * but then you loose the inbetween loging while its off.
 * or start the reader with the flag off.
 * then the reader simply cleans the log. period.
 * so for small tests that dont need the reader in parallel
 * simply switch after the logging the flag to off
 * and start the reader. it will then cleanup the buffer and exit.
 * so the cleanup is done and the reader assumes there is no logging 
 * after it.
 * in fact its possible to attach t a buffer thats off.
 * so you can start with the connection, but every time the
 * process wants to log it short circuts and looses the info
 * if you want you can then switch to on and the next logging will
 * do the job.
 *
 * sidenote:
 * there was once a famous italian socker trainer.
 * and there was a terrible spirit in the team after some months.
 * so the trainer made a big speech in a german tv interview.
 * and the last famous words were ...
 * ... ich habe fertig
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

  printf("shm log attach and set ich_habe_fertig to 1.\n");
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
    {
      volatile const void * a = ATRSHMLOG_GET_AREA();

      int was_before = ATRSHMLOG_GET_AREA_ICH_HABE_FERTIG(a);

      if (ATRSHMLOG_GET_AREA_VERSION(a) != ATRSHMLOGVERSION)
    {
      printf("version is %d, not as needed %d.\n", ATRSHMLOG_GET_AREA_VERSION(a), ATRSHMLOGVERSION);
      exit(1);
    }
      
      
	ATRSHMLOG_SET_AREA_ICH_HABE_FERTIG(a, 1);

      printf("ich_habe_fertig was before %d\n", was_before);
    }
  }

  return 0;
}

/* end of file */
