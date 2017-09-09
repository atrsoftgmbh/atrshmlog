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

/** \file atrshmlogcreate.c
 * \brief We create the shared memory buffer.
 *
 * We create it with the given key and the count of buffers.
 * We not initialize it. This is done with another helper.
 */


#include "atrshmlog.h"

#include "atrshmlog_internal.h"

#include <stdio.h>

#include <errno.h>

#include <stdlib.h>


/*
 *------------------------------------------------
 */

/**
 * create the shm buffer, only vanilla, no initialization
 * use the init for the initialization after the create
 *
 * so all we do is creating a buffer from the given key value - which 
 * is NOT the shmid, this is given as a result ...
 *
 * we calc the size from the amount of buffers that shall be used
 * with this buffer. if you init you should use the same count,
 * but this is not mandatory. if you simply like to use only part
 * of the buffer you can go down to a minimum - which is 2 buffer
 * the maximum is 255 - if the sysem actually allows that much memory
 * for a shm buffer ...
 * using a relative small number like 16 is no problem. normally
 * the reader can run in parallel and move full buffers to a file
 * faster than the log is generated.
 * at least in a korn shell environment, tat was the first
 * use case for this system.
 * if you have trouble to get pace with the reader
 * you should try to increase the number - but not all systems
 * support that much me mory in a single shm buffer.
 * so best is to check this in the doc or by trying some binary
 * approch ... 
 * 
 * all in all :
 * use a well defined key value for create, then a reasonable buffer 
 * count and then you get a buffer that is naked vanilla and big enough.
 * or an error.
 */
int main (int argc, char*argv[])
{
  long ipckey;
  long anz_buff;

  if (argc < 3)
    {
      printf("usage: %s ipckey32bit count_of_buffers [dotfilename]\n", argv[0]);
      printf("logsystem version is %d.\n", ATRSHMLOGVERSION);
      exit(1);
    }

  ipckey = strtol(argv[1],0,10);
  anz_buff = strtol(argv[2],0,10);

  printf("shm log create.\n");
  printf("logsystem version is %d.\n", ATRSHMLOG_GET_VERSION());
  printf("ipckey is 0X%x\n", (int)ipckey);
  printf("count of buffer is %d\n", (int)anz_buff);

  {
    atrshmlog_key_t key = ipckey;
    int count = anz_buff;

    int creat_result = ATRSHMLOG_CREATE(key, count);

    if (creat_result < 1)
      {
	printf("error %d , errno %d.\n", creat_result, errno);
	exit(1);
      }

    printf("\npaste this into the user process environment startshell\n");
    printf("or into the profile or ENV file\n\n");
    printf("\n%s_ID=\"%d\"\n", ATRSHMLOG_GET_ENV_PREFIX(),creat_result);
    printf("export %s_ID\n\n", ATRSHMLOG_GET_ENV_PREFIX());

    
    printf("if you have to use a login shell you can use the files instead.\n");
    printf("see atrshmlog_attach for this\n");

    
    FILE * f;

    if ( argc > 3 )
      {
	f = fopen(argv[3],"w");
      }
    else
      {
	f = fopen("dot.atrshmlog","w");
      }
    
    if (f)
      {
	fprintf(f, "#shmid is\n%s_ID=\"%d\"\n", ATRSHMLOG_GET_ENV_PREFIX(),creat_result);
	fprintf(f, "export %s_ID\n", ATRSHMLOG_GET_ENV_PREFIX());

	fprintf(f, "%s_COUNT=%d\n", ATRSHMLOG_GET_ENV_PREFIX(), count);

	fprintf(f, "export %s_COUNT\n", ATRSHMLOG_GET_ENV_PREFIX());

	fprintf(f, "# end of file\n");

	fclose(f);
      }

#if ATRSHMLOG_PLATFORM_MINGW_X86_64_GCC == 1

    if ( argc > 3 )
      {
	f = fopen(argv[3],"wb");
      }
    else
      {
	f = fopen("atrshmlog.cmd","wb");
      }
    
    if (f)
      {
	fprintf(f, "rem shmid is\r\nSET %s_ID=%d\r\n", ATRSHMLOG_GET_ENV_PREFIX(),creat_result);
	
	fprintf(f, "SET %s_COUNT=%d\r\n", ATRSHMLOG_GET_ENV_PREFIX(), count);
	
	fprintf(f, "rem end of file\r\n");
	
	fclose(f);
      }
  
    // well, this is simple. If you dont backup with a real file
    // we are in the pagefile space and mapped to memory.
    // but if the ref counter falls to 0 its al other,
    // the thing is deleted by kernel.
    // so we have to do the thing here with waiting till our
    // client is up and running.
    // then we can gracefull kill the thing.
    printf("\nenter ctrl-C to kill it after ref counter is non zero.\n");
    printf("or use a kill util for this %ld.\n", (long)getpid());

    while (1) {
      Sleep(5000);
    }

#endif

  }

  return 0;
}

/* end of file */
