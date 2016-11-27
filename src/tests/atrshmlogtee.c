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

/** \file atrshmlogtee.c
 * \brief We are a demo program. 
 *
 * We imitate the tee program. We read in via stdin,
 * and copy to stdout.
 * We write the throughput also to a shared memory.
 * So we dublicate the input. 
 */

#include "../atrshmlog.h"


#include <stdlib.h>
#include <stdio.h>


/*
 * -------------------------------------------------------
 */

/** 
 * the size we use for one entry
 *
 * this is short, but realistic for most loggers 
 */
#define CHUNKSIZE (40)


/*
 * -------------------------------------------------------
 */

/**
 * attach and write input stream in log and echo
 *
 * this a testing program that uses the shm buffer the same way
 * the tee program uses the output file.
 *
 * this said a big - nearly ! - is needed.
 * you have a chunksize limit of 40 bytes.
 * so your logging is very fine grained and therefor the thing
 * is far from perfect in delivering the payload
 * but for testing its ok. simply convert the output,
 * then use a cut or similar to remove the headers and replace 
 * the new introduced nl and you are in with the copy.
 *
 * for bigger files the thing needs a running reader to make it.
 * the overhead is definetely bigger than expected.
 * but its the only way to do it without internal buffering inside the
 * write and then try to write only bigger chunks of info....
 * 
 * so i think its a realistic test, not a nice one ...
 * you can after passing the converted output do a sort, then
 * cut and then remove for example whitespace.
 * this was done by me to checkif the thing works with a bigger file 
 * ( 40 mb) composed of a thounds time cat ed source file ...
 * so if you think there is something wrongwit hte log - use
 * this and then test01 and then fork and then defekt to make your own
 * tests. and verify and dump to see for the states and contents.. 
 *
 * beside for testing the thing is not really hepful - for now.
 * perhaps i change this in the future to a really tee version - but
 * then i will need to check it some more ...
 */
int main (int argc, char*argv[])
{
  const char *shmid_ptr = ATRSHMLOG_GET_ENV_SHMID();

  int line = 0;
  char flag;
  
  ssize_t read_result;
  ssize_t write_result;

  if (shmid_ptr == (void*)0)
    {
      fprintf(stderr, "usage: %s\n", argv[0]);
      fprintf(stderr, "ENVIRONMENT VARIABLE NOT SET !!!\n");
      fprintf(stderr, "%s_ID\n", ATRSHMLOG_GET_ENV_PREFIX());
      printf("logsystem version is %d.\n", ATRSHMLOG_GET_VERSION());
      exit(1);
    }

  if (argc != 1)
    {
      fprintf(stderr, "usage: %s\n", argv[0]);
      fprintf(stderr, "parameter count wrong.\n");
      exit(1);
    }

  ATRSHMLOG_ATTACH();

  if (!ATRSHMLOG_GET_LOGGING())
    {
      fprintf (stderr, "attach not successful.\ncheck ENVIRONMENT VARIABLE %s_ID\n"
           "Value seems not to fit : '%s'\n", ATRSHMLOG_GET_ENV_PREFIX(), shmid_ptr);

      exit(1);
    }

  /* we enter the loop and read from fd 0 and write to fd 1 in 40 byte chuncks */

  do  {
    atrshmlog_time_t starttime, endtime;

      char buffer [ CHUNKSIZE ];
      
      ATRSHMLOG_GETTIME(0,starttime);
      
      write_result = read_result = fread(&buffer[0], 1, CHUNKSIZE, stdin);


      if (read_result > 0)
	write_result = fwrite(&buffer[0], 1, read_result, stdout);
      
      ATRSHMLOG_GETTIME(0,endtime);

      ++line ;

      flag = 'N';

      if (read_result < CHUNKSIZE && write_result < CHUNKSIZE) flag = 'E';

      if (read_result < 0) flag = 'R';

      if (write_result < 0) flag = 'W';

      if (read_result > 0) {
	ATRSHMLOG_WRITE(0, flag, line, starttime, endtime, buffer, read_result);
      }
    } while (!(write_result <= 0 || read_result <= 0));

  return 0;
}

/* end of file */
