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

/** \file atrshmlogsignalreader.c
 * \brief We are a communication helper for a reader.
 */
#include "atrshmlog.h"

#include "atrshmlog_internal.h"


/*
 * ------------------------------------------------------------
 */


/**
 * attach and read the signal
 * this is for the reader slot
 * 
 * a signal is a thing to tell some process that is attached a thing
 * so we need the pid and tid of the thing - at least pid - 
 * and give it a value too
 * after the signal is send its up to the
 * process to do whatever has to be done and then reset the thing
 *
 * the sender checks for a pending signal, but still overwrite it.
 * so you get the info about a former signal was overwritten if
 * pid and value are not zero ...
 *
 * its up to the user what has to be done if a process ignore
 * and does not clean up the slot.
 *
 * see this as a very primitive messaging system, dont be too smart
 * with it.
 *
 * btw.: here is one rease to make a bigger create than init.
 * you then have a buffer - or perhaps more - for the communication.
 * copy this thing, decide haow to use the free shm buffer
 * and you are in the messaging business between your apps
 *
 * but still remember that its only plain memory, so its up to you
 * to make a synchronisation mechanism that really works.
 *
 */
int main (int argc, char*argv[])
{
  const char *shmid_ptr = ATRSHMLOG_GET_ENV_SHMID();

  long lpid;
  long lflag;
  
  if (shmid_ptr == (void*)0)
    {
      printf("usage: %s pid flag\n", argv[0]);
      printf("ENVIRONMENT VARIABLE NOT SET !!!\n");
      printf("%s_ID\n",ATRSHMLOG_GET_ENV_PREFIX());
      printf("logsystem version is %d.\n", ATRSHMLOGVERSION);
      exit(1);
    }

  if (argc != 3)
    {
      printf("usage: %s pid flag\n", argv[0]);
      printf("parameter count wrong.\n");
      printf("logsystem version is %d.\n", ATRSHMLOGVERSION);
      exit(1);
    }

  lpid = strtol(argv[1], 0, 10);
  lflag = strtol(argv[2], 0, 10);
  
  printf("shm log attach and set reader flag and pid.\n");
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
      atrshmlog_area_t * a = (atrshmlog_area_t *)ATRSHMLOG_GET_AREA();

      atrshmlog_pid_t pid_was_before = a->readerpid;

      int flag_was_before = a->readerflag;

      if (ATRSHMLOG_GET_AREA_VERSION(a) != ATRSHMLOGVERSION)
    {
      printf("verion is %d, not as needed %d.\n", ATRSHMLOG_GET_AREA_VERSION(a), ATRSHMLOGVERSION);
      exit(1);
    }
      
      
      a->readerpid = 0;

      a->readerflag = (int)lflag;

      a->readerpid = (atrshmlog_pid_t)lpid;
      
      printf("pid  before %ld\n", (long)pid_was_before);
      printf("flag before %d\n", flag_was_before);
    }
  }

  
  return 0;
}

/* end of file */
