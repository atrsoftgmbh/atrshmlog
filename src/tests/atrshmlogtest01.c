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

/** \file atrshmlogtest01.c
 * \brief We are a test program.
 */

#include "../atrshmlog.h"

#include "../atrshmlog_internal.h"

/*
 * ------------------------------------------------------------
 */

/**
 * attach and make some logging
 *
 * this is a testing programm to check for write errors...
 * 
 * simply see it and then try it...
 *
 */
int main (int argc, char*argv[])
{
  const char *shmid_ptr = ATRSHMLOG_GET_ENV_SHMID();

  
  if (shmid_ptr == (void*)0)
    {
      printf("usage: %s [ ... ]\n", argv[0]);
      printf("ENVIRONMENT VARIABLE NOT SET !!!\n");
      printf("%s_ID\n", ATRSHMLOG_GET_ENV_PREFIX());
      printf("logsystem version is %d.\n", ATRSHMLOGVERSION);
      exit(1);
    }

  printf("shm log attach and set ich_habe_fertig to 1. parametercount %d\n",argc);
  
  ATRSHMLOG_ATTACH();

  if (!ATRSHMLOG_LOGGING)
    {
      printf ("attach not successful.\ncheck ENVIRONMENT VARIABLE %s_ID\n"
          "Value seems not to fit : '%s'\n", ATRSHMLOG_GET_ENV_PREFIX(),shmid_ptr);

      exit(1);
    }

  fflush(stdout);


  /* first 32 times a hello world */
  {
    atrshmlog_time_t starttime;
    atrshmlog_time_t endtime;

    ATRSHMLOG_GETTIME(0, starttime);
    
    for (int i = 0; i < 32 ; i++)
      {
      const int event = 0;
      const char eventflag = 'I';

      const int userevent = i;
      
      ATRSHMLOG_GETTIME(0, endtime);

      ATRSHMLOG_WRITE(event,
             eventflag,
             userevent,
             starttime,
             endtime,
             "hallo world",
             sizeof("hallo world") - 1
             );
      }

    printf("done 32 write.\n");

    fflush(stdout);


    
    ATRSHMLOG_GETTIME(0, starttime);


    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);

        ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);

        ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);

        ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);

        ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);

        ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);

        ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);

        ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);

        ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);

        ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);
    ATRSHMLOG_GETTIME(0, endtime);

    /* ok. we have now an estimate for 100 times getting the time ... */
    ATRSHMLOG_WRITE2(1,
            'I',
            0,
            starttime,
            endtime,
            "commandlineargs -1 time 100",
            sizeof("commandlineargs -1 time 100") - 1,
            (const char**)argv,
            -1
            );

    printf("after argv\n");
    fflush(stdout);


    ATRSHMLOG_GETTIME(0, endtime);

    ATRSHMLOG_WRITE2(1,
            'I',
            0,
            starttime,
            endtime,
            "commandlineargs argc",
            sizeof("commandlineargs argc") - 1,
            (const char**)argv,
            argc
            );

    ATRSHMLOG_GETTIME(0, endtime);

    ATRSHMLOG_WRITE2(1,
            'I',
            0,
            starttime,
            endtime,
            "commandlineargs 2",
            sizeof("commandlineargs 2") - 1,
            (const char**)argv,
            2
            );


      ATRSHMLOG_GETTIME(0, starttime);

      atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );

    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );
    atrshmlog_write1(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );

    ATRSHMLOG_GETTIME(0, endtime);


      ATRSHMLOG_WRITE0(1,
             'I',
             5555,
             starttime,
		       endtime);

      printf("non logging \n");
    
    ATRSHMLOG_SET_LOGGING_PROCESS_OFF();
    
     ATRSHMLOG_WRITE(1,
             'P',
             4711,
             starttime,
             endtime,
             "hallo world non logged",
             sizeof("hallo world non logged") - 1
             );

    printf("logging \n");
    
      ATRSHMLOG_SET_LOGGING_PROCESS_ON();
    
      ATRSHMLOG_WRITE(1,
             'P',
             4812,
             starttime,
             endtime,
             "hallo world logged",
             sizeof("hallo world logged") - 1
             );


      ATRSHMLOG_WRITE(999,
		     'I',
		     4913,
		     starttime,
		     endtime,
		     NULL,
		     0);

      ATRSHMLOG_WRITE(1001,
		     'P',
		     4711,
		     0,
		     0,
		     NULL,
		     0);

      ATRSHMLOG_WRITE(1002,
		     'p',
		     4711,
		     0,
		     0,
		     NULL,
		     0);

    ATRSHMLOG_GETTIME(1003, starttime);
    ATRSHMLOG_GETTIME(1003, endtime);

    ATRSHMLOG_WRITE(1003,
		     'I',
		     4711,
		     starttime,
		     0,
		     "only start",
		    sizeof("only start") - 1);


  }
  
  {
    volatile const void * a = ATRSHMLOG_GET_AREA();

    int was_before = ATRSHMLOG_GET_AREA_ICH_HABE_FERTIG(a);

    if (ATRSHMLOG_GET_AREA_VERSION(a) != ATRSHMLOGVERSION)
    {
      printf("verion is %d, not as needed %d.\n", ATRSHMLOG_GET_AREA_VERSION(a), ATRSHMLOGVERSION);
      exit(1);
    }
      
    ATRSHMLOG_SET_AREA_ICH_HABE_FERTIG(a, 1);

    printf("ich_habe_fertig was before %d\n", was_before);
  }

  // we no longer log
  ATRSHMLOG_STOP();
      
  /* we give out the statistics */


  atrshmlog_int32_t maxindex = atrshmlog_get_statistics_max_index();


  atrshmlog_int32_t *dynstat = calloc(sizeof(atrshmlog_int32_t), maxindex + 1);

  atrshmlog_get_statistics(dynstat);

  for (int i = 0; i <= maxindex; ++i)
    {
      printf("atrshmlogstat %i %ld\n", i, (long)dynstat[i]);
    }
  
  return 0;
}

/* end of file */
