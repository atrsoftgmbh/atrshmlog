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

/** \file atrshmlogdefect.c
 * \brief We are a testing tool.
 *
 * We can do a simple logging test fro the major functions
 * with given parameters from start.
 *
 * In the second form we can show the offsets for variables in the shared memory.
 *
 * In the third form we can change bytes in the shared memory.
 *
 * So its possible to use this to do a simple test.
 * 
 * Or to change something in the shared memory so that a function in a 
 * diffterent  program can handle this change.
 */

#include "atrshmlog.h"

#include "atrshmlog_internal.h"  


/*
 *---------------------------------------------------------
 */


/**
 * attach and print out interesting offsets
 * if supported with offset and value change the byte at offset to value
 *
 * this is a testing tool and an info tool
 *
 * the first part is info
 * 
 * if you want to test the shm buffer you can use this to manipulate
 * the bytesyou want to change
 * by doing it you get the old content, so you can change it back
 * could use some aditinal stuff, but for now its enough for me
 * 
 * for the info first compile the system, then start a create, an init
 * and then try a defekt.
 * thats all for the sizes then.
 *
 * after the info there is a list of offsets for you to see 
 * where is what in the shm buffer.
 *
 * you have then the info about offsets from the individual log buffers.
 *  
 * after this you have all you need to play some poke and peek
 * games with the buffer. this was manly for the testings...
 *
 * if you change the code and want to see how it interacts
 * you can use this to change some values in the buffer - at least in the
 * view that the system presents the defekt tool.
 *
 * remember : you are playing with a shm buffer.
 * so you should first understand the needs of numa architecture
 * the so called caching hierarchy of your system,
 * the needs of memory barriers, syncronisation points,
 * even memory models in modern c++ ISO standards ...
 * and then you can blame me if it does not work
 * for your system ... 
 * and fix it then of course....
 * and submit a patch to me ...
 * see my email in the header and in the downloaded archives doc files ...
 *
 */
int main (int argc, char*argv[])
{
  const char *shmid_ptr = ATRSHMLOG_GET_ENV_SHMID();

  long lindex_bytes;
  long ldefekt;
  
  if (shmid_ptr == (void*)0)
    {
      printf("usage: %s index_buffer newbytevalue\n", argv[0]);
      printf("ENVIRONMENT VARIABLE NOT SET !!!\n");
      printf("%s_ID\n",ATRSHMLOG_GET_ENV_PREFIX());
      printf("logsystem version is %d.\n", ATRSHMLOGVERSION);
      exit(1);
    }

  if (argc != 3)
    {
      printf("usage: %s index_buffer newbytevalue\n", argv[0]);
      printf("parameter count wrong.\n");
      printf("index_buffer in bytes from start.\n");
      printf("newbytevalue 0 .. 255\n");
      printf("logsystem version is %d.\n", ATRSHMLOGVERSION);

      ATRSHMLOG_ATTACH();

      if (!ATRSHMLOG_LOGGING)
	{
	  printf ("attach not successful.\ncheck ENVIRONMENT VARIABLE %s_ID\n"
		  "Value seems not to fit : '%s'\n", ATRSHMLOG_GET_ENV_PREFIX(),shmid_ptr);
	  
	  exit(1);
	}

      {

	atrshmlog_area_t * a = (atrshmlog_area_t *)ATRSHMLOG_GET_AREA();
      
	int anzahl_shm = a->shmcount;

	int v = ATRSHMLOG_GET_AREA_VERSION(a);
    
	int i = 0;
	
	printf("\ninfo\n");
	printf("sizeof atrshmlog_internal_time_t  %d \n", (int)sizeof(atrshmlog_internal_time_t));
	printf("sizeof atrshmlog_time_t           %d \n", (int)sizeof(atrshmlog_time_t));
	printf("sizeof atrshmlog_pid_t            %d \n", (int)sizeof(atrshmlog_pid_t));
	printf("sizeof atrshmlog_tid_t            %d \n", (int)sizeof(atrshmlog_tid_t));
	printf("sizeof atrshmlog_time_t           %d \n", (int)sizeof(atrshmlog_time_t));
	printf("sizeof atrshmlog_int32_t          %d \n", (int)sizeof(atrshmlog_int32_t));
	printf("sizeof atr_shm_log_buffer_t       %d \n", (int)sizeof(atrshmlog_buffer_t));
	printf("sizeof atr_shm_log_area_t         %d \n", (int)sizeof(atrshmlog_area_t));

	printf("sizeof time_t                     %d \n", (int)sizeof(time_t));
	printf("sizeof atomic_int                 %d \n", (int)sizeof(atomic_int));
	printf("sizeof atomic_flag                %d \n", (int)sizeof(atomic_flag));
	printf("sizeof atomic_intptr_t            %d \n", (int)sizeof(atomic_intptr_t));
	printf("sizeof time_t                     %d \n", (int)sizeof(time_t));
    
	printf("transferbufferheader size        %d \n", (int)ATRSHMLOGCONTROLDATASIZE);
	printf("argv buffer max len             : %d\n", (int)ATRSHMLOGARGVBUFFERLEN);
	printf("\noffsets\n"
	       "version         : %ld\n" 
	       "shmid           : %ld\n" 
	       "safeguard       : %ld\n"
	       "count           : %ld\n"
	       "ich_habe_fertig : %ld\n"
	       "reader flag     : %ld\n"
	       "reader pid      : %ld\n"
	       "writer flag     : %ld\n"
	       "writer pid      : %ld\n",
	       (long)((char*)&(a->shmversion) - (char*)a),
	       (long)((char*)&(a->shmid) - (char*)a),
	       (long)((char*)&(a->shmsafeguard) - (char*)a),
	       (long)((char*)&(a->shmcount) - (char*)a),
	       (long)((char*)&(a->ich_habe_fertig) - (char*)a),
	       (long)((char*)&(a->readerflag) - (char*)a),
	       (long)((char*)&(a->readerpid) - (char*)a),
	       (long)((char*)&(a->writerflag) - (char*)a),
	       (long)((char*)&(a->writerpid) - (char*)a)
	       );

	if (v != ATRSHMLOGVERSION)
	  {
	    printf("version is %d, not this programs version %d\n", v, ATRSHMLOGVERSION);
	  }
    
	if ( anzahl_shm == 0)
	  {
	    printf("count buffer in shm is 0, assume not initialized.\n");
	    return 0;
	  }
    
	printf("\ninfo\ncount buffer : %d\n", anzahl_shm);
    
	for (i = 0; i < anzahl_shm; i++)
	  {
	    printf("\nbuffer %d offsets\n"
		   "  data       : %ld\n"
		   "  state      : %ld\n"
		   "  size       : %ld\n"
		   "  info       : %ld\n"
		   "  safeguard  : %ld\n",
		   i,
		   (long)a->logbuffers[i].info,
		   (long)((char*)&(a->logbuffers[i].state) - (char*)a),
		   (long)((char*)&(a->logbuffers[i].shmsize) - (char*)a),
		   (long)((char*)&(a->logbuffers[i].info) - (char*)a),
		   (long)((char*)&(a->logbuffers[i].safeguard) - (char*)a));
           
	  }

	printf("\ntotalsize (see dump file size in atrshmlogdump) : %ld\n",
	       (long)((char*)(&(a->logbuffers[anzahl_shm])) + (anzahl_shm * ATRSHMLOGBUFFER_INFOSIZE)  - (char*)a));
           
           
      }

      /* now we calc the times for the operation of atrshmlog */

     
      volatile int hugo = -3;
      
      for (int k = 0; k < (1000 * 1000) ; k++ )
	for (int j = 0; j < (1000 * 5); j++)
	  hugo += 3;
      
      atrshmlog_time_t starttime;

      atrshmlog_time_t endtime;

      atrshmlog_time_t totaltime;

      ATRSHMLOG_GETTIME(0,starttime);

      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);

      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);

      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);

      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);

      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);

      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);

      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);

      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);

      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);

      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);
      ATRSHMLOG_GETTIME(1,endtime);

      ATRSHMLOG_WRITE(2,
		      'I',
		      1,
		      starttime,
		      endtime,
		      "hundret times gettime",
		      sizeof("hundret times gettime") - 1
		      );

      /* now we init the buffers in shm by writing at least one hundret times
       */

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );

      ATRSHMLOG_WRITE(6,
		      'P',
		      1,
		      endtime,
		      endtime,
		      "Point prepare",
		      sizeof("Point prepare") - 1
		      );


      ATRSHMLOG_GETTIME(3,starttime);
      
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );

      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );


      
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );

      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );

      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );

      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );

      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );

      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );

      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );

      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );

      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );

      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );
      ATRSHMLOG_WRITE(4,
		      'P',
		      1,
		      starttime,
		      starttime,
		      "Point in time logging",
		      sizeof("Point in time logging") -1
		      );

      ATRSHMLOG_GETTIME(4,endtime);

      ATRSHMLOG_WRITE(5,
		      'I',
		      1,
		      starttime,
		      endtime,
		      "hundret times write to buffer",
		      sizeof("hundret times write to buffer") -1
		      );


      ATRSHMLOG_GETTIME(7,starttime);
      
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );

      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );


      
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );

      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );

      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );

      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );

      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );

      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );

      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );

      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );

      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );

      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );
      ATRSHMLOG_WRITE2(8,
		       'P',
		       1,
		       starttime,
		       starttime,
		       "Point in time logging",
		       sizeof("Point in time logging") -1,
		       (const char**)argv,
		       argc
		       );

      ATRSHMLOG_GETTIME(8,endtime);

      ATRSHMLOG_WRITE(9,
		      'I',
		      1,
		      starttime,
		      endtime,
		      "hundret times write to buffer argv",
		      sizeof("hundret times write to buffer argv") -1
		      );


      // we no longer log
      ATRSHMLOG_STOP();
      
      exit(1);
    }

  printf("logsystem version is %d.\n", ATRSHMLOGVERSION);

  lindex_bytes = strtol(argv[1], 0, 10);

  ldefekt = strtol(argv[2], 0, 10);
  
  printf("shm log defekt setting.\n");
  
  ATRSHMLOG_ATTACH();

  if (!ATRSHMLOG_LOGGING)
    {
      printf ("attach not successful.\ncheck ENVIRONMENT VARIABLE %s_ID\n"
	      "Value seems not to fit : '%s'\n", ATRSHMLOG_GET_ENV_PREFIX(), shmid_ptr);

      exit(1);
    }

  fflush(stdout);
  
  
  {
    char *a = (char*)atrshmlog_base_ptr;

    int orig = (unsigned char)a[ lindex_bytes ];

    int check;
    
    a[lindex_bytes ] = (0xff & ldefekt);

    check = (unsigned char)a[ lindex_bytes ];

    if ((check & 0xff) != (ldefekt & 0xff) )
      {
	printf("write failed ... \n");
      }
    else
      {
	printf("before : %d , now %d \n",
	       (unsigned)(orig & 0xff),
	       (unsigned)(check & 0xff));
      }
  }


  return 0;
}

/* end of file */
