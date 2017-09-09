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


/** \file atrshmlogdump.c
 * \brief We are a full dump tool for the shared memory.
 *
 * If you have trouble with the shared memory you get a file dump from it
 * with this tool.
 */


#include "atrshmlog.h"

#include "atrshmlog_internal.h"

#include <stdio.h>

#include <stdlib.h>

/*
 *---------------------------------------------------------
 */

/**
 * write some info and then dump the whole log into a file
 * file name is fix buffer.bin
 * you can later on rename or remove the thing ...
 * the whole buffer is written according to the info about
 * he number of buffers in the log.
 * so be sure you dont try this with a corrupted buffer - at least 
 * if ou think the count is not valid any more ...
 * if this is the case, use the defekt to correct the count.
 * its a one byte value from 2 .. 255 - so its simple.
 * you use the defekt to set the byte in place and thats it.
 * use verify to check its ok and then dump it.
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

  if (argc != 2)
    {
      printf("usage: %s countbuffer\n", argv[0]);
      printf("argument count is wrong.\n");
      printf("logsystem version is %d.\n", ATRSHMLOGVERSION);
      exit(1);
    }

  lanz = strtol(argv[1], 0, 10);
  
  printf("shm log attach and dump %d.\n", (int)lanz);
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
    atrshmlog_area_t * a = (atrshmlog_area_t *)ATRSHMLOG_GET_AREA();
      
      int anzahl_buffer = (int)lanz;
      int anzahl_shm = a->shmcount;
      int i = 0;

      if (ATRSHMLOG_GET_AREA_VERSION(a) != ATRSHMLOGVERSION)
    {
      printf("verion is %d, not as needed %d.\n", ATRSHMLOG_GET_AREA_VERSION(a), ATRSHMLOGVERSION);
      exit(1);
    }
      
      if (anzahl_buffer != anzahl_shm)
    {
      printf("Count buffer wrong: expect is %d, in shm is %d.\n",
         anzahl_buffer, anzahl_shm);
      goto ende;
    }

      const int save = 256 ;
  
      size_t wantedsize = sizeof (atrshmlog_area_t) + (anzahl_shm - ATRSHMLOGBUFFER_MINCOUNT) * sizeof( atrshmlog_buffer_t) + (anzahl_shm * ATRSHMLOGBUFFER_INFOSIZE + save);

      /* ok. we dump the buffers into file with name buffer.bin */

      FILE* f = fopen("./buffer.bin", "wb");

      if (f == NULL)
    {
      printf("cannot open dump file buffer.bin...\n");
      goto ende;
    }

      if ( wantedsize != fwrite(a, 1, wantedsize, f))
    {
      printf("error, count written not correct.\n");
      goto ende;
    }

      fflush(f);

      fclose(f);

      printf("buffer written to file ./buffer.bin ...\n");
  }
  
 ende:
  
  return 0;
}

/* end of file */
