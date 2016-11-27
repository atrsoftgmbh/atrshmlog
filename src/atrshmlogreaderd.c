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

/** \file atrshmlogreaderd.c
 * \brief We are the concurrent buffer reader with use of shared 
 * memory lists.
 *
 * We are the actual used reader. 
 */


#include "atrshmlog.h"

#include "atrshmlog_internal.h"

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

#if ATRSHMLOG_PLATFORM_SOLARIS_X86_64_GCC == 1

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE 1
#endif

/* for the mkdir and its mode flags */
#include <sys/stat.h>

#include <unistd.h>

/* file creation masks */
#include <fcntl.h>

#endif


/* standard c */
#include <stdio.h>


/* time meaturement */
#include <time.h>

/* low level string ops , namely memcpy */
#include <string.h>

/* error code */
#include <errno.h>

/* checks for types of chars */
#include <ctype.h>

/* standard c stuff */  
#include <stdlib.h>

/****************************************************************/

/** 
 * this is the internal buffer used for data in the reader
 * we have 3 lists.
 * the cleanup uses the s list
 * the available buffers are on the a list
 * the full buffers are on the f list
 * side note
 * this is nearly a 1:1 of the tbuff_t in the client code
 * of atrshmlog.c
 * so you can guess it is doing things similar.
 * end sidenote
 * we have at least some additional parts here ...
 * but most important is ob - the object buffer
 * we have a dense collectio of the values from read,
 * with the payload at last.
 * after the thing is fetched from the a list
 * it is put on the f list after it has been filled.
 * the writer threads then push it to 
 * perm storage and then you have it there.
 * after this is it pushed on the a list again, and pulled from
 * the f list.
 * the s list is here a simple test and check helper,
 * there is no cleanup via the main program.
 */
struct mb_s {
  /* list of all buffers for introspective */
  struct mb_s* next; 
  /* list of available buffers */
  struct mb_s* next_append;
  /* list of full buffers */
  struct mb_s* next_full ; 
  
  atrshmlog_pid_t pid;

  atrshmlog_tid_t tid;

  atrshmlog_int32_t i_buffer_index;

  atrshmlog_int32_t i_filenumber;

  atrshmlog_int32_t number_dispatched;
  
  /* the total count in the object buffer, which is about 200 bytes plus payload */
  size_t lcount;
  
  char *ob;
};

/**
 * the buffer type
 */
typedef struct mb_s mb_t;

/**
 * We use this many spave tor the buffer data
 */
#define MB_INFOSIZE (ATRSHMLOGBUFFER_INFOSIZE + 1024L)

/**
 * anchor of the buffer list
 * we use this in the atexit cleanup
 * so we have a simple thing if buffers are not full
 * at end of running.
 * the atexit moves them to shm and when its done we dont use
 * the logging any more
 * thats the theory
 * be sure you know what you do if you use the logging 
 * AFTER an exit call ...
 */
static atomic_intptr_t tps = ATOMIC_VAR_INIT(0);


/**
 * we allocate if needed these buffers in advance
 * this is the so called a list anchor
 */
static atomic_intptr_t tpa = ATOMIC_VAR_INIT(0);

/** 
 * this is the full list anchor
 * we use an atomic here, so its safe to do the mt thing
 * this is now a stack
 * its the list of full buffers that must be transfered to shm with the slave
 * threads
 */
static atomic_intptr_t tpf = ATOMIC_VAR_INIT(0);

static int version = 0;

static int shm_limit = 0;

static volatile int buffer_infosize = 1024L + ATRSHMLOGBUFFER_INFOSIZE;

static volatile int prealloc_buffer_count = 1024;

static volatile int f_list_buffer_fetch_run = 1;

static volatile int f_list_buffer_fetch_count = 0;

static atomic_int f_list_active_fetches = ATOMIC_VAR_INIT(0);

static int f_list_buffer_fetch_wait = 500000; // ATRSHMLOG_INIT_BUFFER_SLAVE_WAIT;

static volatile int f_list_buffer_write_run = 1;

static volatile int f_list_buffer_write_count = 0;

static atomic_int f_list_active_writes = ATOMIC_VAR_INIT(0);

static int f_list_buffer_write_wait = 100000;  //ATRSHMLOG_INIT_BUFFER_SLAVE_WAIT;


static atrshmlog_thread_ret_t f_list_buffer_fetch_proc(void* i_arg);

static atrshmlog_thread_ret_t f_list_buffer_write_proc(void* i_arg);


static atrshmlog_tid_t f_list_buffer_fetch;
static atrshmlog_tid_t f_list_buffer_write;

/**
 * We wait this many seconds after the ihf flag is set and recognized
 */
#define SLEEP_AFTER_IHF_INIT 5

static volatile int sleep_after_ihf = SLEEP_AFTER_IHF_INIT;

static atomic_llong writertimes = 0;

static atomic_long writerops = 0;

/**
 * the start of a fetch thread
 */
int create_fetcher()
{
#if  ATRSHMLOG_USE_PTHREAD  == 1
  pthread_t tmp;

  int ret = pthread_create(&tmp,
			   NULL,
			   f_list_buffer_fetch_proc,
			   NULL);

  f_list_buffer_fetch = (atrshmlog_tid_t)tmp;
  
#endif

#if  ATRSHMLOG_USE_C11_THREAD == 1
  int ret = thrd_create( (thrd_t*)&f_list_buffer_fetch,
			 f_list_buffer_fetch_proc,
			 NULL);
#endif

#if ATRSHMLOG_USE_WINTHREAD == 1
  int ret = 0;
  
  uintptr_t rethandle = _beginthread (
				      f_list_buffer_fetch_proc,
				      0, // stacksize by system
				      NULL // no arguments
				      );

  if (rethandle == 0)
    {
      ret = -1;
    }

  if (rethandle == -1L)
    {
      ret = -1;
    }

#endif

  return ret;
}

/**
 * the start of a write thread
 */
int create_writer()
{
#if  ATRSHMLOG_USE_PTHREAD  == 1
  
  pthread_t tmp;

  int ret = pthread_create(&tmp,
			   NULL,
			   f_list_buffer_write_proc,
			   NULL);

  f_list_buffer_write = (atrshmlog_tid_t) tmp;
  
#endif

#if  ATRSHMLOG_USE_C11_THREAD == 1
  int ret = thrd_create( (thrd_t*)&f_list_buffer_write,
			 f_list_buffer_write_proc,
			 NULL);
#endif

#if ATRSHMLOG_USE_WINTHREAD == 1
  int ret = 0;
  
  uintptr_t rethandle = _beginthread (
				      f_list_buffer_write_proc,
				      0, // stacksize by system
				      NULL // no arguments
				      );

  if (rethandle == 0)
    {
      ret = -1;
    }

  if (rethandle == -1L)
    {
      ret = -1;
    }

#endif

  return ret;
}

/**
 * We use that many files per directory
 */
#define DIROFFSETINIT 10000

/*
 * this many files per directory, starting with 1 in dir 0 
 */
static int diroffset = DIROFFSETINIT;

/**
 * we start with this number for the new directories
 */
#define DIRNUMBERINIT 0 

/*
 * the actual number of the last created directory
 * we use the main thread to monitor filenumber
 * and when its time to create new dirs in advace
 */
static int dirnumber = DIRNUMBERINIT;

/**
 * we start to create new directories when we reach that many full
 * from the last free
 */
#define DIRTHREASHOLDINIT 2

/* 
 * if we reach this threshold with filenumber we trigger 
 * the creation of new directorys
 */
static int dirthreashold = DIRTHREASHOLDINIT;

/**
 * we create this many directories when we encounter the reach of the
 * threshold
 */
#define DIRADVANCEINIT 5

/* 
 * if we hit the threashold we create this many dir in advance
 */
static int diradvance = DIRADVANCEINIT;

/**
 * the base path.
 */
static char basepath[256];

/* a targetstring 
  "%s/%d/atrshmlog_%ld_%llx_%d_%d.bin"
   directory/dxxxx/atrshmlog_%ld_%llx_%d_%d.bin
*/

/**
 * the actual highest given filenumber.
 */
atomic_int filenumber = ATOMIC_VAR_INIT(0);

/**
 * ok. we give it a free buffer. if it is successfull 
 * the buffer is used and it returns a value 0
 * if it was nothing there a 1 is given
 * if it is < 0 anerror hapens, the buffer is not used
 */
int read_fetch_and_transfer(mb_t* nb,
			    atrshmlog_int32_t* o_buffer_index,
			    atrshmlog_int32_t i_version)
{
  int read_result ;
	      
  atrshmlog_int32_t o_length;

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

  atrshmlog_int32_t fn;
  
  /* we use the calced size of individual fields here.
   * so if you chancge it you MUST adjust the offset
   * calculation here too.
   */
  const int offset = 2 + sizeof(i_version)
    + sizeof(o_length)
    + sizeof(o_pid)
    + sizeof(o_tid)
    + sizeof(*o_buffer_index)
    + sizeof(fn)
    + sizeof(o_inittime.tv_sec)
    + sizeof(o_inittime.tv_nsec)
    + sizeof(o_inittsc_before)
    + sizeof(o_inittsc_after)
    + sizeof(o_lasttime.tv_sec)
    + sizeof(o_lasttime.tv_nsec)
    + sizeof(o_lasttsc_before)
    + sizeof(o_lasttsc_after)
    + sizeof(o_difftimetransfer)
    + sizeof(o_starttransfer)
    + sizeof(o_acquiretime)
    + sizeof(o_id)
    + sizeof(o_number_dispatched)
    + sizeof(counter_write0)
    + sizeof(counter_write0_discard)
    + sizeof(counter_write0_wait)
    + sizeof(counter_write0_adaptive)
    + sizeof(counter_write0_adaptive_fast)
    + sizeof(counter_write0_adaptive_very_fast)
    + sizeof(counter_write1)
    + sizeof(counter_write1_discard)
    + sizeof(counter_write1_wait)
    + sizeof(counter_write1_adaptive)
    + sizeof(counter_write1_adaptive_fast)
    + sizeof(counter_write1_adaptive_very_fast)
    + sizeof(counter_write2)
    + sizeof(counter_write2_discard)
    + sizeof(counter_write2_wait)
    + sizeof(counter_write2_adaptive)
    + sizeof(counter_write2_adaptive_fast)
    + sizeof(counter_write2_adaptive_very_fast);

  // printf("offset %ld \n", (long)offset);

  // we use a well known offset after the head date.
  //head data is about 194 byte
  // offset is 512, so we have a filler gab of 318 byte
  // we use this in the convert too
  // so do not mess with it
  char* transferbuffer = nb->ob
    + 512;

  o_length = 0;

  volatile const void *a = ATRSHMLOG_GET_AREA();
  
  read_result = ATRSHMLOG_READ_FETCH(a,
    o_buffer_index,
    transferbuffer,
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

  if (read_result > 0 )
    {
      /* we had nothing to do, no buffers */
      return 1;
    }
  
  if (o_length > 0)
    {
      size_t lcount;

      lcount = (size_t)o_length;
          
      char *start = &transferbuffer[0];

      size_t maxbuffersize = ATRSHMLOG_GET_BUFFER_MAX_SIZE();

      if (lcount > 0 && lcount <=  maxbuffersize)
	{
	  nb->i_buffer_index = *o_buffer_index;
	  fn = filenumber++;
	  nb->i_filenumber = fn;

	  nb->pid = o_pid;
	  nb->tid = o_tid;

	  nb->number_dispatched = o_number_dispatched;

	  //	  printf("%d %p\n" , i_filenumber, nb);

	  // printf("ob %p\n",  nb->ob);
  
	  char *ob = nb->ob;
	  
	  char * obp = ob;
      
	  uint16_t order = 1;

	  /* we write the order first */
	  /* in a later version we can convert this on any platform */
	  memcpy(obp, (void*)&order, 2);
	  obp += 2;
	  
	  /* we write out the version first */
	  memcpy(obp, (void*)&i_version, sizeof(atrshmlog_int32_t));
	  obp += sizeof(atrshmlog_int32_t);
	  
	  /* we write out the length */
	  memcpy(obp, (void*)&o_length, sizeof(atrshmlog_int32_t));
	  obp += sizeof(atrshmlog_int32_t);
		      
	  /* we write out the pid */
	  memcpy (obp,  (void*)&o_pid, sizeof(atrshmlog_pid_t));
	  obp += sizeof(atrshmlog_pid_t);
		      
	  /* we write out the tid */
	  memcpy(obp,  (void*)&o_tid, sizeof(atrshmlog_tid_t));
	  obp +=  sizeof(atrshmlog_tid_t);

	  /* we write out the buffer number */
	  memcpy(obp,  (void*)o_buffer_index, sizeof(atrshmlog_int32_t));
	  obp += sizeof(atrshmlog_int32_t);
		      
	  /* we write out the filenumber */
	  memcpy(obp,  (void*)&fn, sizeof(atrshmlog_int32_t));
	  obp += sizeof(atrshmlog_int32_t);
	  
	  /* we write out the initttime */
	  memcpy(obp,  (void*)&o_inittime.tv_sec, sizeof(atrshmlog_time_seconds_t));
	  obp += sizeof(atrshmlog_time_seconds_t);
	  memcpy(obp,  (void*)&o_inittime.tv_nsec, sizeof(atrshmlog_time_nanoseconds_t));
	  obp += sizeof(atrshmlog_time_nanoseconds_t);
	  /*
	    #if 	  ATRSHMLOGDEBUG ==  1
	    printf("%ld %ld\n", (long)o_inittime.tv_sec, (long)o_inittime.tv_nsec);
	    #endif
	  */
	  /* we write out the tsc before init */
	  memcpy(obp,  (void*)&o_inittsc_before, sizeof(atrshmlog_time_t));
	  obp +=  sizeof(atrshmlog_time_t);
		      
	  /* we write out the tsc before init */
	  memcpy(obp,  (void*)&o_inittsc_after, sizeof(atrshmlog_time_t));
	  obp +=  sizeof(atrshmlog_time_t);
	  
	  /* we write out the tsc before init */
	  memcpy(obp,  (void*)&o_lasttime.tv_sec, sizeof(atrshmlog_time_seconds_t));
	  obp += sizeof(atrshmlog_time_seconds_t);
	  memcpy(obp,  (void*)&o_lasttime.tv_nsec, sizeof(atrshmlog_time_nanoseconds_t));
	  obp += sizeof(atrshmlog_time_nanoseconds_t);
	  
	  /* we write out the tsc before init */
	  memcpy(obp, (void*)&o_lasttsc_before, sizeof(atrshmlog_time_t));
	  obp +=  sizeof(atrshmlog_time_t);
	  
	  /* we write out the tsc before init */
	  memcpy(obp,  (void*)&o_lasttsc_after, sizeof(atrshmlog_time_t));
	  obp +=  sizeof(atrshmlog_time_t);

	  /* we write out the transfertime */
	  memcpy(obp,  (void*)&o_difftimetransfer, sizeof(atrshmlog_time_t));
	  obp +=  sizeof(atrshmlog_time_t);
	  
	  /* we write out the starttransfer */
	  memcpy(obp,  (void*)&o_starttransfer, sizeof(atrshmlog_time_t));
	  obp +=  sizeof(atrshmlog_time_t);
	  
	  /* we write out the acquiretime */
	  memcpy(obp,  (void*)&o_acquiretime, sizeof(atrshmlog_time_t));
	  obp +=  sizeof(atrshmlog_time_t);
	  
	  /* we write out the id */
	  memcpy(obp,  (void*)&o_id, sizeof(atrshmlog_int32_t));
	  obp +=  sizeof(atrshmlog_int32_t);

	  /* we write out the o_number_dispatched */
	  memcpy(obp,  (void*)&o_number_dispatched, sizeof(atrshmlog_int32_t));
	  obp +=  sizeof(atrshmlog_int32_t);
	  	  
	  /* we write out the counter_write0 */
	  memcpy(obp,  (void*)&counter_write0, sizeof(atrshmlog_int32_t));
	  obp +=  sizeof(atrshmlog_int32_t);
	  
	  /* we write out the counter_write0_discard */
	  memcpy(obp,  (void*)&counter_write0_discard, sizeof(atrshmlog_int32_t));
	  obp +=  sizeof(atrshmlog_int32_t);
	  
	  /* we write out the counter_write0_wait */
	  memcpy(obp,  (void*)&counter_write0_wait, sizeof(atrshmlog_int32_t));
	  obp +=  sizeof(atrshmlog_int32_t);
	  
	  /* we write out the counter_write0_adaptive */
	  memcpy(obp,  (void*)&counter_write0_adaptive, sizeof(atrshmlog_int32_t));
	  obp +=  sizeof(atrshmlog_int32_t);
	  
	  /* we write out the counter_write0_adaptive_fast */
	  memcpy(obp,  (void*)&counter_write0_adaptive_fast, sizeof(atrshmlog_int32_t));
	  obp +=  sizeof(atrshmlog_int32_t);
	  
	  /* we write out the counter_write0_adaptive_very_fast */
	  memcpy(obp,  (void*)&counter_write0_adaptive_very_fast, sizeof(atrshmlog_int32_t));
	  obp +=  sizeof(atrshmlog_int32_t);
	  
	  /* we write out the counter_write1 */
	  memcpy(obp,  (void*)&counter_write1, sizeof(atrshmlog_int32_t));
	  obp +=  sizeof(atrshmlog_int32_t);
	  
	  /* we write out the counter_write1_discard */
	  memcpy(obp,  (void*)&counter_write1_discard, sizeof(atrshmlog_int32_t));
	  obp +=  sizeof(atrshmlog_int32_t);
	  
	  /* we write out the counter_write1_wait */
	  memcpy(obp,  (void*)&counter_write1_wait, sizeof(atrshmlog_int32_t));
	  obp +=  sizeof(atrshmlog_int32_t);
	  
	  /* we write out the counter_write1_adaptive */
	  memcpy(obp,  (void*)&counter_write1_adaptive, sizeof(atrshmlog_int32_t));
	  obp +=  sizeof(atrshmlog_int32_t);
	  
	  /* we write out the counter_write1_adaptive_fast */
	  memcpy(obp,  (void*)&counter_write1_adaptive_fast, sizeof(atrshmlog_int32_t));
	  obp +=  sizeof(atrshmlog_int32_t);
	  
	  /* we write out the counter_write1_adaptive_very_fast */
	  memcpy(obp,  (void*)&counter_write1_adaptive_very_fast, sizeof(atrshmlog_int32_t));
	  obp +=  sizeof(atrshmlog_int32_t);
	  
	  /* we write out the counter_write2 */
	  memcpy(obp,  (void*)&counter_write2, sizeof(atrshmlog_int32_t));
	  obp +=  sizeof(atrshmlog_int32_t);
	  
	  /* we write out the counter_write2_discard */
	  memcpy(obp,  (void*)&counter_write2_discard, sizeof(atrshmlog_int32_t));
	  obp +=  sizeof(atrshmlog_int32_t);
	  
	  /* we write out the counter_write2_wait */
	  memcpy(obp,  (void*)&counter_write2_wait, sizeof(atrshmlog_int32_t));
	  obp +=  sizeof(atrshmlog_int32_t);
	  
	  /* we write out the counter_write2_adaptive */
	  memcpy(obp,  (void*)&counter_write2_adaptive, sizeof(atrshmlog_int32_t));
	  obp +=  sizeof(atrshmlog_int32_t);
	  
	  /* we write out the counter_write2_adaptive_fast */
	  memcpy(obp,  (void*)&counter_write2_adaptive_fast, sizeof(atrshmlog_int32_t));
	  obp +=  sizeof(atrshmlog_int32_t);
	  
	  /* we write out the counter_write2_adaptive_very_fast */
	  memcpy(obp,  (void*)&counter_write2_adaptive_very_fast, sizeof(atrshmlog_int32_t));
	  obp +=  sizeof(atrshmlog_int32_t);
	  
	  
#if 	  ATRSHMLOGDEBUG ==  1
	  printf("%ld %p %ld\n", (long)fn, nb, (long)o_id);
#endif
  
	  /* this is now done by giving the offset to read */
	  obp += lcount;
	  
	  nb->lcount = 512 + lcount;
  
	  return 0;
  
	}
      else
	{
	  return -2;
	}
    }
  else
    {
      // we signal all was ok, but nothing to do 
      return 1;
    }
}

/**
 * we write one buffer to filesystem
 */
int atrshmlog_write_transfer(char *target, mb_t* ob)
{
  int read_result ;
	      
  int o_length;

  atrshmlog_pid_t o_pid;

  atrshmlog_tid_t o_tid;

  o_length = ob->lcount;
  
  if (o_length > 0)
    {
      size_t lcount;
      size_t write_total;
      size_t write_done;

      lcount = (size_t)o_length;
          
      char *start = ob->ob;

      int fout;

      size_t write_result;
      
      long lpid = ob->pid;

      long ltid = ob->tid;

      char filename[255];
	  
      snprintf(filename, 255, target, lpid, ltid); 

      fout = open(filename ,O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR|S_IWUSR);

      if (fout < 0 )
	{
	  return -2;
	}

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


/**
 * we get the buffers in the list
 */
 int init_buffers(int i_count)
 {
   mb_t * m = malloc(i_count * sizeof(mb_t));

   if (m)
     {
       char* buffers = malloc(i_count * MB_INFOSIZE);

       if (buffers)
	 {
	   mb_t* mb = m;
	   
	   for (int i = 0; i  < i_count; i++, mb++, buffers += MB_INFOSIZE)
	     {
	       mb->next = &mb[1]; 
	       mb->next_append = &mb[1];
	       mb->next_full = 0;
	       mb->lcount = 0;
	       mb->ob = buffers;
	     }

	   /* we correct the last buffer */
	   mb--;
	   mb->next = 0;
	   mb->next_append = 0;

	   atomic_store(&tps, (intptr_t)m);
	   atomic_store(&tpa, (intptr_t)m);
	   
	   return 0;
	 }
     }

   return -1;
 }

/**
 * we need additional buffers and we deliver them
 */
 int add_buffers(int i_count)
 {
   mb_t * m = malloc(i_count * sizeof(mb_t));

   if (m)
     {
       char* buffers = malloc(i_count * MB_INFOSIZE);

       if (buffers)
	 {
	   mb_t* mb = m;
	   
	   for (int i = 0; i  < i_count; i++, mb++, buffers += MB_INFOSIZE)
	     {
	       mb->next = &mb[1]; 
	       mb->next_append = &mb[1];
	       mb->next_full = 0;
	       mb->lcount = 0;
	       mb->ob = buffers;
	     }

	   /* we correct the last buffer */
	   mb--;
	   mb->next = (mb_t*)atomic_load(&tps);
	   mb->next_append = (mb_t*)atomic_load(&tpa);

	   // push on stack ...
	   while(!atomic_compare_exchange_weak_explicit(&tps,
							(intptr_t*)&mb->next,
							(intptr_t)m,
						       memory_order_release,
						       memory_order_relaxed))
	    ;

	   // push on stack ...
	   while(!atomic_compare_exchange_weak_explicit(&tpa,
						       (intptr_t*)&mb->next_append,
						       (intptr_t)m,
						       memory_order_release,
						       memory_order_relaxed))
	    ;
	   
	   return 0;
	 }
     }

   return -1;
 }


/**
 * we create one directory
 */
int create_dir (char *i_dir, int i_verbose)
{
  int acc_result = access(i_dir, R_OK|W_OK);

  if (acc_result == 0)
    {
      /* access ok, we use it as a directory .. */
      ;
    }
  else
    {
      if(i_verbose)
	fprintf(stderr,"directory not found or not accessible '%s'. Create it.\n", i_dir);

      int result = mkdir (i_dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

      if (result != 0)
	{
	  if (i_verbose)
	    fprintf(stderr, "error in mkdir %d\n", errno);
	  return -1;
	}
    }
  
  return 0;
}

/**
 * we create a bunch of directories
 */
int create_dirs(char* i_basepath, int i_start, int i_count, int i_verbose)
{
  char buff[256];

  for (int index = 0; index < i_count; index++)
    {
      int newdir = i_start + index;
      
      snprintf(buff, 256, "%s/%d", i_basepath , newdir);

      int acc_result = access(buff, R_OK|W_OK);

      if (acc_result == 0)
	{
	  dirnumber = newdir;
	}
      else
	{
	  int result = mkdir (buff, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	  
	  if (result != 0)
	    {
	      fprintf(stderr, "error in mkdir %d\n", errno);
	      return -1;
	    }

	  dirnumber = newdir;
	}
    }

  return 0;
}


static int reader_ist_fertig(volatile const void* i_area)
{

  int result = 0;

  static int ihf = 0;
  
  atrshmlog_area_t * a = (atrshmlog_area_t *)i_area;

  atrshmlog_pid_t pid_was_before = a->readerpid;

  int flag_was_before = a->readerflag;

  if (pid_was_before == 9999)
    if (flag_was_before == 9999)
      {
	a->readerflag = 0;

	a->readerpid = 0;

	result = 1;
      }

  int newihf  = ATRSHMLOG_GET_AREA_ICH_HABE_FERTIG(a);

  if (newihf != ihf)
    {
      printf("global log switched to %s\n", (newihf == 0) ? "on" : "off");
      ihf = newihf;
    }
  
  return result;
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

#if ATRSHMLOG_USE_SAFER_COPY == 1
  strlcpy(basepath, argv[1],256);
#else
  strcpy(basepath, argv[1]);
#endif
  
  version = ATRSHMLOG_GET_VERSION();
  
  if( argc > 2)
    {
      long l = strtol (argv[2], NULL, 10);

      /* ok, we have something. now we check if its too small or too big */
      if (l > 100 && l < 1000000)
	diroffset = l;
    }

  printf("shm log attach and loop write file.\n");
  printf("logsystem version is %d.\ndirectory is %s\nfiles_per_dir is %d\n", ATRSHMLOG_GET_VERSION(), argv[1], diroffset);

  printf("stop via signal reader for pid 9999 with value 9999.\n");
  
  ATRSHMLOG_ATTACH();

  if (!ATRSHMLOG_GET_AREA())
    {
      printf ("attach not successful.\ncheck ENVIRONMENT VARIABLE %s_ID\n"
          "Value seems not to fit : '%s'\n", ATRSHMLOG_GET_ENV_PREFIX(),shmid_ptr);

      exit(1);
    }

  fflush(stdout);

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

      /*********************************************************/

      if (create_dir(basepath, 1) != 0)
	goto ende;

      if (create_dirs(basepath, 0, diradvance, 1) != 0)
	goto ende;
      
      
      shm_limit = ATRSHMLOG_GET_AREA_COUNT(a);

      const char *p = ATRSHMLOG_GET_ENV("_FETCH_COUNT");

      if (p)
	{
	  long new_val = strtol(p, NULL, 10);

	  if (new_val >= 0)
	    f_list_buffer_fetch_count = new_val;
	}
      
      p = ATRSHMLOG_GET_ENV("_WRITE_COUNT");

      if (p)
	{
	  long new_val = strtol(p, NULL, 10);

	  if (new_val >= 0)
	    f_list_buffer_write_count = new_val;
	}

      long advanced = 0;
      
      p = ATRSHMLOG_GET_ENV("_ALLOC_ADVANCED");

      if (p)
	{
	  long new_val = strtol(p, NULL, 10);

	  if (new_val >= 1)
	     advanced = new_val;
	}
      
      /* ok. we see if we have limits or 0 
       * if its 0 it means to do the half approch 
       */

      if (f_list_buffer_fetch_count == 0)
	{
	  f_list_buffer_fetch_count = shm_limit / 2;

	  if (f_list_buffer_write_count == 0)
	    f_list_buffer_write_count = f_list_buffer_fetch_count * 3;
	}

      printf("count of initial used fetchers is %ld\n", (long)f_list_buffer_fetch_count);
      /* ok. we check if we have a write count
       * if not we use 3 times the count for fetchers
       */
      

      printf("count of initial used writes is %ld\n", (long)f_list_buffer_write_count);

      init_buffers(prealloc_buffer_count);

      for (int jk = 0; jk < advanced; jk++)
	add_buffers(prealloc_buffer_count);
      
      /* ok. we start the fetcher */
      int ret_fetch = 0;
		  
      for (int slaves = 0;
	   ret_fetch == 0
	     && slaves < f_list_buffer_fetch_count;
	   slaves++)
	{
          ret_fetch = create_fetcher();
        }
      
      /* ok. we start the writer */
      int ret_write = 0;
      
      for (int slaves = 0;
	   ret_write == 0
	     && slaves < f_list_buffer_write_count;
	   slaves++)
	{
          ret_write = create_writer();
        }
      
      if (reader_ist_fertig(a) == 0)
	{
          printf("enter transferloop ... \n");
          do
	    {
	      ATRSHMLOG_SLEEP_NANOS(500000);

	       int fn = atomic_load(&filenumber);

	       if ((fn / diroffset) > dirnumber - dirthreashold)
		 {
		   if (create_dirs(basepath, dirnumber + 1, diradvance, 1) != 0)
		     {
		       goto ende;
		     }
		 }
            }
	  while (reader_ist_fertig(a) == 0);

	  fprintf(stderr, "logging stopped by signal 9999 9999 ...\n");
	  
	}

      /* we start 10 writers. so its possible to start with 0.
       * we then wait till all buffers are in use ...
       * or the ihf is triggered.
       * then we start some and are ready for output...
       */
      for (int k = 0; k < 10; k++)
	create_writer();
   
  }

  /* we give the client time to write the final buffers */
  
  sleep(sleep_after_ihf);
  
  /* from now on we assume the thing is done if the f list is empty */

  //  printf ("%p %d %d \n", atomic_load(&tpf), atomic_load(&f_list_active_fetches), atomic_load(&f_list_active_writes));
  
  while ((mb_t*)atomic_load(&tpf) != NULL )
    {
      ATRSHMLOG_SLEEP_NANOS(50000);
    }

  // printf ("%p %d %d \n", atomic_load(&tpf), atomic_load(&f_list_active_fetches), atomic_load(&f_list_active_writes));
  
  f_list_buffer_fetch_run = 0;
  
  while (atomic_load(&f_list_active_fetches) != 0)
    {
      ATRSHMLOG_SLEEP_NANOS(50000);
    }

  /* no more fetcher are running. */

  // printf ("%p %d %d \n", atomic_load(&tpf), atomic_load(&f_list_active_fetches), atomic_load(&f_list_active_writes));
  
  f_list_buffer_write_run = 0;

  while (atomic_load(&f_list_active_writes) != 0)
    {
      ATRSHMLOG_SLEEP_NANOS(50000);
    }

  /* no more writers are running */ 
  
 ende:

  printf("logging done.\n");
  long long wt  = atomic_load(&writertimes);
  long wc  = atomic_load(&writerops);

  if (wc == 0)
    ++wc;

  printf("writer data : time %lld  count %ld  per use %lld\n", wt, wc, wt / wc);

  // check the counter for errors
  int c = atomic_load(&atrshmlog_counter[atrshmlog_counter_fence_alarm_2]);

  if (c != 0)
    {
      printf("ALARM: fence checksum error in transfer shm to mem found : %d\n", c);
    }
  
  return 0;
}

static atrshmlog_thread_ret_t f_list_buffer_fetch_proc(void* i_arg)
{
  f_list_active_fetches++;

  while (f_list_buffer_fetch_run)
    {
      // we use f list 
      mb_t* tbuff = (mb_t*)atomic_load_explicit(&tpa, memory_order_relaxed);

      if (tbuff == 0)
	{
	  ATRSHMLOG_SLEEP_NANOS(f_list_buffer_fetch_wait);

	  continue;
	}

      // pop from list
      while (tbuff &&
	     !atomic_compare_exchange_weak_explicit(&tpa,
						    (intptr_t*)&tbuff,
						    (intptr_t)tbuff->next_append,
						    memory_order_acq_rel,
						    memory_order_relaxed))
	;

      if (tbuff)
	{
	  tbuff->next_append = 0;
	  
	  /* ok. we have an empty buffer. 
	   * now we can iterate till we hit a full payload in shm
	   * then we have to dispatch the buffer to the
	   * full list
	   */
          int gotone = 0;
	  
	  do {
	    atrshmlog_int32_t index;
	    
	    int read_ret = read_fetch_and_transfer(tbuff,
						   &index,
						   version);
	
	    if (read_ret == 0)
	      {
		gotone = 1;
	      }
	    else
	      {
		ATRSHMLOG_SLEEP_NANOS(f_list_buffer_fetch_wait);

		if (!f_list_buffer_fetch_run)
		  goto leave;
	      }
	  } while (gotone == 0);

	  
	  tbuff->next_full = (mb_t*)atomic_load_explicit(&tpf, memory_order_relaxed);

	  atomic_thread_fence(memory_order_release);

	  // push on stack ...
	  while(!atomic_compare_exchange_weak_explicit(&tpf,
						       (intptr_t*)&tbuff->next_full,
						       (intptr_t)tbuff,
						       memory_order_release,
						       memory_order_relaxed))
	    ;
	}
    }

 leave:

  f_list_active_fetches--;
  
  ATRSHMLOG_THREADFUNCTION_RETURN(NULL); 
}

static atrshmlog_thread_ret_t f_list_buffer_write_proc(void* i_arg)
{
  f_list_active_writes++;

  while (f_list_buffer_write_run)
    {
      // we use f list 
      mb_t* tbuff = (mb_t*)atomic_load_explicit(&tpf, memory_order_consume);

      if (tbuff == 0)
	{
	  ATRSHMLOG_SLEEP_NANOS(f_list_buffer_write_wait);

	  continue;
	}

      // pop from list
      while (tbuff &&
    !atomic_compare_exchange_weak_explicit(&tpf,
					   (intptr_t*)&tbuff,
					   (intptr_t)tbuff->next_full, memory_order_acq_rel, memory_order_relaxed))
	;

      if (tbuff)
	{
	  atrshmlog_time_t start;
	  atrshmlog_time_t end;
	  
	  start = ATRSHMLOG_GET_TSC_CALL();
	  
	  atomic_thread_fence(memory_order_acquire);
	  
	  tbuff->next_full = 0;

	  /* we do the write thing
	   */
	  int write_ret;

	  char target[256];
	  int filenumber = tbuff->i_filenumber;
	  
	  int dirnumber = filenumber / diroffset;

	  int seq = tbuff->number_dispatched;
	  
	  snprintf(target, 256, "%s/%d/atrshmlog_p%%ld_t%%ld_s%d_f%d.bin", basepath, dirnumber, seq, filenumber);

	  #if ATRSHMLOGDEBUG == 1
	  printf("write %ld\n", (long)filenumber);
	  #endif
	  do
	    {
	       write_ret = atrshmlog_write_transfer(target, tbuff);
	    }
	  while (write_ret != 0);

	  tbuff->lcount = 0;
	  
	  tbuff->next_append = (mb_t*)atomic_load_explicit(&tpa, memory_order_relaxed);


	  // push on stack ...
	  while(!atomic_compare_exchange_weak_explicit(&tpa,
						       (intptr_t*)&tbuff->next_append,
						       (intptr_t)tbuff,
						       memory_order_release,
						       memory_order_relaxed))
	    ;
	  end = ATRSHMLOG_GET_TSC_CALL();

	  writerops++;

	  writertimes += (end - start);
	}
    }

 leave:

  f_list_active_writes--;
  
  ATRSHMLOG_THREADFUNCTION_RETURN(NULL); // we exit here
}

/* end of file */
