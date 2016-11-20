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

/** \file atrshmlogreader.c
 * \brief We are the simple reader.
 *
 * We connect to the shared meory and read in a loop any 
 * full buffer and write it to the file system.
 */


#include "atrshmlog.h"

#if ATRSHMLOG_PLATFORM_LINUX_X86_64_GCC == 1

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE 1
#endif

/* tell them how it is */
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 1
#endif


#ifndef _POSIX_C_SOURCE
#define   _POSIX_C_SOURCE 199309L
#endif

/* for the mkdir and its mode flags */
#include <sys/stat.h>


#include <unistd.h>

/* file creation masks */
#include <fcntl.h>

#endif

#if ATRSHMLOG_PLATFORM_CYGWIN_X86_64_GCC == 1

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE 1
#endif

/* tell them how it is */
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 1
#endif


#ifndef _POSIX_C_SOURCE
#define   _POSIX_C_SOURCE 199309L
#endif

/* for the mkdir and its mode flags */
#include <sys/stat.h>


#include <unistd.h>

/* file creation masks */
#include <fcntl.h>

#endif

#if ATRSHMLOG_PLATFORM_MINGW_X86_64_GCC == 1

#include <direct.h>

#define mkdir(__f,__m) _mkdir((__f))

#include <io.h>

/* file creation masks */
#include <fcntl.h>

#include <sys/stat.h>

#define access _access

#define open _open

#undef O_CREAT
#define O_CREAT (_O_CREAT | _O_BINARY)

#define write _write

#include <windows.h>

#define sleep(__s) Sleep((__s) * 1000)

#endif

#if ATRSHMLOG_PLATFORM_BSD_AMD64_CLANG == 1

/* for the mkdir and its mode flags */
#include <sys/stat.h>

#include <unistd.h>

/* file creation masks */
#include <fcntl.h>

#endif

#if ATRSHMLOG_PLATFORM_BSD_AMD64_GCC == 1

/* for the mkdir and its mode flags */
#include <sys/stat.h>

#include <unistd.h>

/* file creation masks */
#include <fcntl.h>

#endif

/* c standard */
#include <stdio.h>


/* time meaturement */
#include <time.h>

/* low level string ops , namely memcpy */
#include <string.h>

/* file io */

/* error code */
#include <errno.h>

/* checks for types of chars */
#include <ctype.h>

/* standard c stuff */  
#include <stdlib.h>


static char * transferbuffer = 0;

static char *ob = 0;


/* a targetstring 
  "%s/%d/atrshmlog_%ld_%llx_%d.bin"
   directory/dxxxx/atrshmlog_%ld_%llx_%d.bin
*/
int atrshmlog_read_and_transfer(char *target,
				int i_buffer_index,
				int i_version,
				int i_filenumber)
{
  int read_result ;
	      
  int o_length;

  atrshmlog_pid_t o_pid;

  atrshmlog_tid_t o_tid;

  atrshmlog_internal_time_t o_inittime;

  atrshmlog_time_t o_inittsc_before;
	      
  atrshmlog_time_t o_inittsc_after;
	      
  atrshmlog_internal_time_t o_lasttime;
	      
  atrshmlog_time_t o_lasttsc_before;
	      
  atrshmlog_time_t o_lasttsc_after;

  atrshmlog_time_t o_difftimetransfer;

  atrshmlog_time_t o_starttransfer;

  atrshmlog_time_t o_acquiretime;

  atrshmlog_int32_t o_id;

  atrshmlog_int32_t o_number_dispatched;

  /**
   * thread specific statistics are here.
   * so we can colect them later from the buffers in files.
   *
   * It was too bad for performance to have them in atomics global.
   * Too much interaction.
   * so I have them now local
   * We only set them in the beginning, never reset them.
   */

  atrshmlog_int32_t counter_write0;
  atrshmlog_int32_t counter_write0_discard;
  atrshmlog_int32_t counter_write0_wait;
  atrshmlog_int32_t counter_write0_adaptive;
  atrshmlog_int32_t counter_write0_adaptive_fast;
  atrshmlog_int32_t counter_write0_adaptive_very_fast;
  atrshmlog_int32_t counter_write1;
  atrshmlog_int32_t counter_write1_discard;
  atrshmlog_int32_t counter_write1_wait;
  atrshmlog_int32_t counter_write1_adaptive;
  atrshmlog_int32_t counter_write1_adaptive_fast;
  atrshmlog_int32_t counter_write1_adaptive_very_fast;
  atrshmlog_int32_t counter_write2;
  atrshmlog_int32_t counter_write2_discard;
  atrshmlog_int32_t counter_write2_wait;
  atrshmlog_int32_t counter_write2_adaptive;
  atrshmlog_int32_t counter_write2_adaptive_fast;
  atrshmlog_int32_t counter_write2_adaptive_very_fast;
  
  int o_chksum;
	      
  o_length = 0;

  read_result = ATRSHMLOG_READ(ATRSHMLOG_GET_AREA(),
			       i_buffer_index,
			       &transferbuffer[0],
			       &o_length,
			       &o_pid,
			       &o_tid,
			       &o_inittime,
			       &o_inittsc_before,
			       &o_inittsc_after,
			       &o_lasttime,
			       &o_lasttsc_before,
			       &o_lasttsc_after,
			       &o_difftimetransfer,
			       &o_starttransfer,
			       &o_acquiretime,
			       &o_id,
			       &o_number_dispatched,
			       &counter_write0,
			       &counter_write0_discard,
			       &counter_write0_wait,
			       &counter_write0_adaptive,
			       &counter_write0_adaptive_fast,
			       &counter_write0_adaptive_very_fast,
			       &counter_write1,
			       &counter_write1_discard,
			       &counter_write1_wait,
			       &counter_write1_adaptive,
			       &counter_write1_adaptive_fast,
			       &counter_write1_adaptive_very_fast,
			       &counter_write2,
			       &counter_write2_discard,
			       &counter_write2_wait,
			       &counter_write2_adaptive,
			       &counter_write2_adaptive_fast,
			       &counter_write2_adaptive_very_fast);
			       
			       

  if (read_result < 0)
    {
      return -1;
    }

  if (o_length > 0)
    {
      size_t lcount;
      size_t write_total;
      size_t write_done;

      lcount = (size_t)o_length;
          
      char *start = &transferbuffer[0];

      size_t maxbuffersize = ATRSHMLOG_GET_BUFFER_MAX_SIZE();

      char * obp = ob;
      
      if (lcount > 0 && lcount <=  maxbuffersize)
	{
	  int fout;
	  int chksum = 0;
#if 0

	  for (int k = 0; k < o_length; k++)
	    {
	      chksum += transferbuffer[k];
	    }
	  
	  if (o_chksum != chksum)
	    printf("chksum too late \n");
#endif	      
	  size_t write_result;
      
	  long lpid = (long)o_pid;

	  long long ltid = 0;

	  memcpy(&ltid, &o_tid, sizeof(o_tid));

	  char filename[255];
	  
	  snprintf(filename, 255, target, lpid, ltid); 

	  fout = open(filename ,O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR|S_IWUSR);

	  if (fout < 0 )
	    {
	      return -2;
	    }

	  
	  /* we write out the version first */
	  memcpy(obp, (void*)&i_version, sizeof(i_version));
	  obp += sizeof(i_version);
	  
	  /* we write out the length */
	  memcpy(obp, (void*)&o_length, sizeof(o_length));
	  obp += sizeof(o_length);
		      
	  /* we write out the pid */
	  memcpy (obp,  (void*)&o_pid, sizeof(o_pid));
	  obp += sizeof(o_pid);
		      
	  /* we write out the tid */
	  memcpy(obp,  (void*)&o_tid, sizeof(o_tid));
	  obp +=  sizeof(o_tid);

	  /* we write out the buffer number */
	  memcpy(obp,  (void*)&i_buffer_index, sizeof(i_buffer_index));
	  obp += sizeof(i_buffer_index);
		      
	  /* we write out the filenumber */
	  memcpy(obp,  (void*)&i_filenumber, sizeof(i_filenumber));
	  obp += sizeof(i_filenumber);
	  
	  /* we write out the initttime */
	  memcpy(obp,  (void*)&o_inittime, sizeof(o_inittime));
	  obp += sizeof(o_inittime);
	  
	  /* we write out the tsc before init */
	  memcpy(obp,  (void*)&o_inittsc_before, sizeof(o_inittsc_before));
	  obp +=  sizeof(o_inittsc_before);
		      
	  /* we write out the tsc before init */
	  memcpy(obp,  (void*)&o_inittsc_after, sizeof(o_inittsc_after));
	  obp +=  sizeof(o_inittsc_before);
	  
	  /* we write out the tsc before init */
	  memcpy(obp,  (void*)&o_lasttime, sizeof(o_lasttime));
	  obp += sizeof(o_lasttime);
	  
	  /* we write out the tsc before init */
	  memcpy(obp, (void*)&o_lasttsc_before, sizeof(o_lasttsc_before));
	  obp +=  sizeof(o_lasttsc_before);
	  
	  /* we write out the tsc before init */
	  memcpy(obp,  (void*)&o_lasttsc_after, sizeof(o_lasttsc_after));
	  obp +=  sizeof(o_lasttsc_after);

	  /* we write out the transfertime */
	  memcpy(obp,  (void*)&o_difftimetransfer, sizeof(o_difftimetransfer));
	  obp +=  sizeof(o_difftimetransfer);
	  
	  /* we write out the starttransfer */
	  memcpy(obp,  (void*)&o_starttransfer, sizeof(o_starttransfer));
	  obp +=  sizeof(o_starttransfer);
	  
	  /* we write out the acquiretime */
	  memcpy(obp,  (void*)&o_acquiretime, sizeof(o_acquiretime));
	  obp +=  sizeof(o_acquiretime);
	  
	  /* we write out the id */
	  memcpy(obp,  (void*)&o_id, sizeof(o_id));
	  obp +=  sizeof(o_id);
	  
	  
	  memcpy(obp, start, lcount);
	  obp += lcount;

	  start = ob;

	  lcount = obp - ob;

	  write_total = lcount;

	  write_done = 0;
          
	  
	  do {
	    write_result = write(fout, start, lcount);
	    
	    if (write_result == lcount)
	      {
		write_done += write_result;
		lcount -= write_result;
		start += (int)write_result;
	      }
	    else
	      {
		if (errno != EINTR)
		  {
		    return -15;
		  }
		
		/* only a signal .. */
		
		write_done += write_result;
		lcount -= write_result;
		start += (int) write_result;
	      }
	    
	  } while (write_total != write_done);
	  
	  close(fout);
	}
      return 0;
    }
  else
    {
      // we signal all was ok, but nothing to do 
      return 1;
    }
}

/*
 *----------------------------------------------------------
 */

/**
 * attach and read buffer in loop to file
 * stop read loop when ich_habe_fertig <> 0
 * read one more time all buffer to file then
 * exit
 *
 * this is the way to get all info from the buffer that the writes have 
 * filled to the filesytem.
 * we use a check and sleep approach. so the cpu load is small.
 * but its not zero.
 * also the thing uses a straight way to write the buffer to the file
 * system, so its tougth to beat it in transferspeed if the system is
 * really on load...
 * if you dont do anything at all its of course too much cpu costly
 * 
 */
int main(int argc, char*argv[])
{
  const char *shmid_ptr = ATRSHMLOG_GET_ENV_SHMID();

  FILE* fout = NULL;


  
  if (shmid_ptr == (void*)0)
    {
      printf("usage: %s outfileprefixdirectory [files_per_dir [startindex_subdirectories]]\n", argv[0]);
      printf("ENVIRONMENT VARIABLE NOT SET !!!\n");
      printf("%s_ID\n",ATRSHMLOG_GET_ENV_PREFIX());
      printf("logsystem version is %d.\n", ATRSHMLOG_GET_VERSION());
      exit(1);
    }

  if (argc < 2)
    {
      printf("usage: %s outfileprefixdirectory [files_per_dir [startindex_subdirectories]] \n", argv[0]);
      printf("parameter count wrong.\n");
      printf("logsystem version is %d.\n", ATRSHMLOG_GET_VERSION());
      exit(1);
    }

  transferbuffer = (char*)calloc(1, ATRSHMLOG_GET_BUFFER_MAX_SIZE() + 1 ) ;

  ob = (char*)calloc(1, ATRSHMLOG_GET_BUFFER_MAX_SIZE() + 2048);


  int diroffset = 1000;

  if( argc > 2)
    {
      long l = strtol (argv[2], NULL, 10);

      /* ok, we have something. now we check if its too small or too big */
      if (l > 1000 && l < 100000)
	diroffset = l;
    }
  
  int dirnumber = 0;
    
  if( argc > 3)
    {
      long l = strtol (argv[3], NULL, 10);

      dirnumber = l;
    }

  {
    int acc_result = access(argv[1], R_OK|W_OK);

    if (acc_result == 0)
      {
	/* access ok, we use it as a directory .. */
	;
      }
    else
      {
	fprintf(stderr,"directory not found or not accessible '%s'. Create it.\n", argv[1]);

	int result = mkdir (argv[1], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	if (result != 0)
	  {
	    fprintf(stderr, "error in mkdir %d\n", errno);
	    goto ende;
	  }
      }

    char buff[256];
    snprintf(buff, 256, "%s/%d", argv[1] , dirnumber);

    acc_result = access(buff, R_OK|W_OK);

    if (acc_result == 0)
      {
	;
      }
    else
      {
	int result = mkdir (buff, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	if (result != 0)
	  {
	    fprintf(stderr, "error in mkdir %d\n", errno);
	    goto ende;
	  }

      }
  }
  
  printf("shm log attach and loop write file.\n");
  printf("logsystem version is %d.\ndirectory is %s\nfiles_per_dir is %d\n", ATRSHMLOG_GET_VERSION(), argv[1], diroffset);
  
  ATRSHMLOG_ATTACH();

  if (!ATRSHMLOG_GET_AREA())
    {
      printf ("attach not successful.\ncheck ENVIRONMENT VARIABLE %s_ID\n"
          "Value seems not to fit : '%s'\n", ATRSHMLOG_GET_ENV_PREFIX(),shmid_ptr);

      exit(1);
    }

  fflush(stdout);

  {
    int i;

    int limit;

    int filenumber = 0;

    {
      volatile const void *a = ATRSHMLOG_GET_AREA();

      volatile int version = ATRSHMLOG_GET_AREA_VERSION(a);
      
      if (version != ATRSHMLOG_GET_VERSION())
	{
	  printf("verion is %d, not as needed %d.\n", version, ATRSHMLOG_GET_VERSION());
	  exit(1);
	}

      {
	int v_result = ATRSHMLOG_VERIFY();

	if (v_result != 0)
	  {
	    printf("verify error %d. errno %d\n",  v_result, errno);
	    exit(1);
	  }

      }
      
      i = 0;

      limit = ATRSHMLOG_GET_AREA_COUNT(a);

      /* int ihf = atrshmlog_get_area_ich_habe_fertig(a); */

      // printf("shm %p, %d %d \n", a , limit , ihf);
      
      if (ATRSHMLOG_GET_AREA_ICH_HABE_FERTIG(a) == 0)
	{
	  printf("enter transferloop ... \n");
	  do
	    {

	      char target[256];
	      snprintf(target, 256, "%s/%d/atrshmlog_p%%ld_t%%lld_f%d.bin", argv[1], dirnumber, filenumber);
	      
	      int ret = atrshmlog_read_and_transfer(target,
						    i,
						    version,
						    filenumber);
	      if (ret < 0)
		{
		  fprintf(stderr, "error in read %d\n", ret);
		  goto ende;
		}

	      if (ret == 0)
		{
		  ++filenumber;
		  
		  if ((filenumber % diroffset) == 0)
		    {
		      char buff[256];
                  
		      ++dirnumber;

		      printf("new directory number %d\n", dirnumber);
		      snprintf(buff, 256, "%s/%d", argv[1], dirnumber );

		      int result = mkdir (buff, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		      if (result != 0)
			{
			  fprintf(stderr, "error in mkdir %d\n", errno);
			  goto ende;
			}
		    }
		}
	      else
		{
		  // we had an empty buffer
		  ;
		}

	      i++;

	      ATRSHMLOG_SLEEP_NANOS(5000);
 
	      if (i >= limit)
		{
		  i = 0;
		}
	    }
	  while (ATRSHMLOG_GET_AREA_ICH_HABE_FERTIG(a) == 0);

	  fprintf(stderr, "logging stopped by ich_habe_fertig ...\n");
      
	  sleep(5);
	}
      
      /* ok. we are out. write the rest */
      for (i = 0; i < limit; i++)
	{
	  char target[256];
	  snprintf(target, 256, "%s/%d/atrshmlog_p%%ld_t%%lld_f%d.bin", argv[1], dirnumber, filenumber);
	      
	  int ret = atrshmlog_read_and_transfer(target,
						i,
						version,
						filenumber);
	  if (ret < 0)
	    {
	      fprintf(stderr, "error in read %d\n", ret);
	      goto ende;
	    }

	  if(ret == 0)
	    {
	      ++filenumber;

	      if ((filenumber % diroffset) == 0)
		{
		  char buff[256];
                  
		  ++dirnumber;

		  printf("new directory number %d\n", dirnumber);
		  snprintf(buff, 256, "%s/%d", argv[1], dirnumber );
		      
		  int result = mkdir (buff, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		  if (result != 0)
		    {
		      fprintf(stderr, "error in mkdir %d\n", errno);
		      goto ende;
		    }
		}
	    }
	}
    }
  }
  
 ende:

  printf("logging done.\n");
  
  return 0;
}

/* end of file */
