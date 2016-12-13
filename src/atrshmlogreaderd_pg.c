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

/** \file atrshmlogreaderd_pg.c
 * \brief We are the concurrent buffer reader with use of shared 
 * memory lists.
 *
 * We are the reader to push into postgresql database
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

/*
 *--------------------------------------------------------
 * 
 * db specific part : incudes and definitions in dbsession
 */


// we are the postgreSQL 
#include <libpq-fe.h>

/*
 *--------------------------------------------------------
 */

struct atrshmlog_dbsession_s {
  PGconn *conn;
  PGresult *res;
  // we have a small state machine here
  // 0 : not connected
  // 1 : connect ready
  // 2 : statements prepared
  // 3 : in transaction
  int status;
  const char * const *keywords;
  const char * const *values;
};

typedef struct atrshmlog_dbsession_s atrshmlog_dbsession_t;

/**
 * db specific function : create
 */
static atrshmlog_dbsession_t* create_db(void);

/**
 * db specific function : cleanup
 */
static void cleanup_db(atrshmlog_dbsession_t* i_db);

/**
 * db specific function : begin
 */
static int begin_db(atrshmlog_dbsession_t* i_db);

/**
 * db specific function : commit
 */
static int commit_db(atrshmlog_dbsession_t* i_db);

/**
 * db specific function : rollback
 */
static int rollback_db(atrshmlog_dbsession_t* i_db);

/**
 * db specific function : get head seq
 */
static int get_head_seq_db(atrshmlog_dbsession_t* i_db,
			   uint64_t *o_head_id);

/**
 * db specific function : insert head
 */
static int insert_db_head(atrshmlog_dbsession_t* i_db,
			     uint64_t i_head_id,
			  atrshmlog_io_head_t *i_head);

/**
 * db specific function : insert cstring
 */
static int insert_db_cstring(atrshmlog_dbsession_t* i_db,
			     uint64_t i_head_id,
			     atrshmlog_io_head_t *i_head,
			     atrshmlog_chunk_head_t* i_chunk);

/**
 * db specific function : insert ucs2 string
 */
static int insert_db_ucs2string(atrshmlog_dbsession_t* i_db,
				uint64_t i_head_id,
				atrshmlog_io_head_t *i_head,
				atrshmlog_chunk_head_t* i_chunk,
				int len);


/**
 *  helper to generate lines from a chunk 
 */
static int operate(atrshmlog_dbsession_t *i_db,
		   char * const i_buffer,
		   atrshmlog_io_head_t head,
		   int sbyteorder,
		   int *io_insert_head,
		   int *io_insert_ucs2,
		   int *io_insert_cstring);


/**
 * helper : deallocat prepared statements in pq
 */
static int free_prepared_db(atrshmlog_dbsession_t *i_db);

/*
 *--------------------------------------------------------
 */

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

  atrshmlog_io_head_t head;
  
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
 * the actual highest given filenumber -- now we no longer use a number,
 * but its a kind of sequence number now...
 */
atomic_int filenumber = ATOMIC_VAR_INIT(0);

atomic_int g_hcount = ATOMIC_VAR_INIT(0);
atomic_int g_ucs2count = ATOMIC_VAR_INIT(0);
atomic_int g_ccount = ATOMIC_VAR_INIT(0);

/**
 * ok. we give it a free buffer. if it is successfull 
 * the buffer is used and it returns a value 0
 * if it was nothing there a 1 is given
 * if it is < 0 anerror hapens, the buffer is not used
 */
int read_fetch_and_transfer(mb_t* nb,
			    atrshmlog_int32_t i_version)
{
  int read_result ;
	      
  
  atrshmlog_int32_t fn;

  char* transferbuffer = nb->ob;

  atrshmlog_io_head_t* h = &nb->head;
  
  h->order[0] = (ATRSHMLOG_H_ORDER_IS_N_ORDER) ? 0 : 1;
  h->order[1] = (ATRSHMLOG_H_ORDER_IS_N_ORDER) ? 1 : 0;

  h->version = i_version;
  
  h->tlen = 0;

  volatile const void *a = ATRSHMLOG_GET_AREA();
  
  read_result = ATRSHMLOG_READ_FETCH(a,
				     &h->buffernumber,
				     transferbuffer,
				     &h->tlen,
				     &h->pid,
				     &h->tid,
				     &h->inittime,
				     &h->inittsc_before,
				     &h->inittsc_after,
				     &h->lasttime,
				     &h->lasttsc_before,
				     &h->lasttsc_after,
				     &h->difftimetransfer,
				     &h->starttransfer,
				     &h->acquiretime,
				     &h->id,
				     &h->number_dispatched,
				     &h->counter_write0,
				     &h->counter_write0_discard,
				     &h->counter_write0_wait,
				     &h->counter_write0_adaptive,
				     &h->counter_write0_adaptive_fast,
				     &h->counter_write0_adaptive_very_fast,
				     &h->counter_write1,
				     &h->counter_write1_discard,
				     &h->counter_write1_wait,
				     &h->counter_write1_adaptive,
				     &h->counter_write1_adaptive_fast,
				     &h->counter_write1_adaptive_very_fast,
				     &h->counter_write2,
				     &h->counter_write2_discard,
				     &h->counter_write2_wait,
				     &h->counter_write2_adaptive,
				     &h->counter_write2_adaptive_fast,
				     &h->counter_write2_adaptive_very_fast);
			       
  
  
  if (read_result < 0)
    {
      return -1;
    }

  if (read_result > 0 )
    {
      /* we had nothing to do, no buffers */
      return 1;
    }
  
  if (h->tlen > 0)
    {
      size_t lcount;

      lcount = (size_t)h->tlen;
          
      size_t maxbuffersize = ATRSHMLOG_GET_BUFFER_MAX_SIZE();

      if (lcount > 0 && lcount <=  maxbuffersize)
	{
	  h->filenumber = filenumber++;
  
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
 * we write one buffer to database
 */
int atrshmlog_write_transfer(atrshmlog_dbsession_t *i_db, mb_t* ob)
{
  int o_length;

  o_length = ob->head.tlen;
  
  if (o_length > 0)
    {
      int dbret = begin_db(i_db);

      if (dbret != 0 )
	return -1;
      
      size_t lcount;

      lcount = (size_t)o_length;
          
      char *start = ob->ob;

      int hcount = 0;
      int ucs2count = 0;
      int ccount = 0;
      
      if (operate(i_db,
		  start,
		  ob->head,
		  0,
		  &hcount,
		  &ucs2count,
		  &ccount) != 0)
	{
	  dbret = rollback_db(i_db);
	
	  return -1;
	}

      dbret = commit_db(i_db);

      g_hcount += hcount;
      g_ucs2count += ucs2count;
      g_ccount += ccount;
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
  printf("usage: %s \n", argv[0]);
      printf("ENVIRONMENT VARIABLE NOT SET !!!\n");
      printf("%s_ID\n",ATRSHMLOG_GET_ENV_PREFIX());
      printf("logsystem version is %d.\n", ATRSHMLOG_GET_VERSION());
      exit(1);
    }

  version = ATRSHMLOG_GET_VERSION();
  
  printf("shm log attach and loop write file.\n");
  printf("logsystem version is %d.\n", ATRSHMLOG_GET_VERSION());

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

  printf("writer data : time %lld : count %ld : per use %lld : heads %d : ucs2string %d : cstring %d \n", wt, wc, wt / wc, atomic_load(&g_hcount), atomic_load(&g_ucs2count), atomic_load(&g_ccount));

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
	    
	    int read_ret = read_fetch_and_transfer(tbuff,
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


	  printf("push %p\n");
	  
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

  atrshmlog_dbsession_t *dbsession = NULL;

  // we create the db session and the insert commands.

  dbsession = create_db();

  if (dbsession == NULL)
    goto leave;


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

	  do
	    {
	       write_ret = atrshmlog_write_transfer(dbsession, tbuff);
	       printf("pop %p\n", tbuff);
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
  
  cleanup_db(dbsession);

  ATRSHMLOG_THREADFUNCTION_RETURN(NULL); // we exit here
}

/** 
 * we convert the content of buffer into output
 * we start with the controldata
 * then we use the length and calc the rest
 * can be empty 
 * we write this one info chunk.
 * we add a line number and write the lines out (?)
 * then we advandce 
 * we check if the thing is still complete.
 * if not we memove the rest t the beginning of the buffer and 
 * adjust end and newend
 * if anything goes wrong we return with -1
 */
int operate(atrshmlog_dbsession_t *i_db,
	    char * const i_buffer,
	    atrshmlog_io_head_t head,
	    int sbyteorder,
	    int *io_insert_head,
	    int *io_insert_ucs2,
	    int *io_insert_cstring)

{
  int result = -1;

  const int horder_is_n_order = ATRSHMLOG_H_ORDER_IS_N_ORDER;

  /* we calc the starttime and endtime in real */

  long long tscstart = head.inittsc_after;

  tscstart += head.inittsc_before;

  tscstart /= 2;

  long long tscend = head.lasttsc_before;

  tscend += head.lasttsc_after;

  tscend /= 2;

  long long tscdiff = tscend - tscstart;

  long double tscstart_d = tscstart;

  long double tscend_d =  tscend;

  long double tscdiff_d = tscdiff;
  
  /* we can now start to give the realtime a try
   * we assume the tsc is in the middle of the fetch of the real time
   * and so we give all tsc now a new realtime 
   */

  /* ok. we have now the tsc. we now need the real */

  long double realstart;

  realstart = head.inittime.tv_sec;
  
  realstart *=  1000000000.0;

  realstart += head.inittime.tv_nsec;

  long double realend;

  realend = head.lasttime.tv_sec;
  
  realend *=  1000000000.0;

  realend += head.lasttime.tv_nsec;

  long double realdiff = realend - realstart;
  
  /* and now we can start with a realtime adder */
  

  int chunkindex = 0;

  int tlen = head.tlen;
  
  if (!horder_is_n_order)
    {
      atrshmlog_io_head_change_order(&head);
    }

  uint64_t head_id;

  int retdb = get_head_seq_db(i_db, &head_id);

  if (retdb != 0)
    {
      return -1;
    }

  retdb = insert_db_head(i_db, head_id, &head);
	
  if (retdb != 0)
    {
      return -1;
    }

  ++*io_insert_head;
  
  atrshmlog_chunk_head_t c;

  int totallen ;
  
  for (int akindex = 0; akindex < tlen; akindex += totallen)
    {
      chunkindex++;

      atrshmlog_fill_chunk_head (i_buffer + akindex, &c);

      if (sbyteorder)
	{
	  atrshmlog_chunk_head_change_order(&c);
	}

      totallen = c.totallen;
      
      if (totallen < ATRSHMLOGCONTROLDATASIZE || totallen > ATRSHMLOGBUFFER_INFOSIZE)
	{
	  fprintf(stderr, "error c.totallen : size :  in %d : %d.\n", chunkindex, totallen);
	  return -1;
	}

      size_t lcount = totallen - ATRSHMLOGCONTROLDATASIZE;

      char payloadbuffer [ ATRSHMLOGBUFFER_INFOSIZE + 1];
 
      if (lcount > 0)
	{
	  memcpy(payloadbuffer, i_buffer + akindex + ATRSHMLOGCONTROLDATASIZE, lcount);
	}
    
      payloadbuffer[lcount] = '\0';

      
      int ucs2string = 0;
      
      if (c.eventflag == ATRSHMLOGPOINTINTIMEp || c.eventflag == ATRSHMLOGPOINTINTIMEi)
	ucs2string = 1;
	
      /* we now calc the real time
       * (reals - realstart)/  (realend - realstart)  
       * eq (s - tstart) / (tend - tstart)  
       * so 
       * (reals - realstart) = (s - tstart) * (realend - realstart) / (tend - tstart)
       * so
       * reals = realstart + ((s - tstart) * (realend - realstart) / (tend - tstart))
       */
      long double reals_d =  realstart;

      long double quots_d = c.starttime;

      quots_d -= tscstart_d;
	
      quots_d *= realdiff;
        
      reals_d += quots_d / tscdiff_d;

      c.startreal = reals_d;
	
      long double reale_d =  realstart;

      long double quote_d = c.endtime;

      quote_d -= tscstart_d;
	
      quote_d *= realdiff;
        
      reale_d += quote_d / tscdiff_d;

      c.endreal = reale_d;

      c.deltareal = c.endreal - c.startreal;

      c.deltatime = c.endtime - c.starttime;
      
      
      if (ucs2string)
	{
	  if (!horder_is_n_order)
	    {
	      atrshmlog_chunk_head_change_order(&c);
	    }
	  
	  c.payload = payloadbuffer;
	      
	  int retdb = insert_db_ucs2string(i_db, head_id, &head, &c, (totallen - ATRSHMLOGCONTROLDATASIZE));

	  ++*io_insert_ucs2;
	  
	  if (retdb != 0)
	    {
	      return -1;
	    }
	}
      else
	{
	  char* lastpos = payloadbuffer;
	  char* nextcontrol = payloadbuffer;

	  if (!horder_is_n_order)
	    {
	      atrshmlog_chunk_head_change_order(&c);
	    }
	  
	  /* simple text convert, somehow dirty done */ 
	  while ( nextcontrol != NULL )
	    {
	      nextcontrol = strchr(lastpos, '\n');

	      if (nextcontrol)
		*nextcontrol = '\0';

		
	      for (char *k = lastpos; *k; k++)
		{
		  if (!isprint(*k))
		    *k = ' ';
		  
		  if (*k == '\t' || *k == '\r')
		    *k = ' ';
		    
		}

	      c.payload = lastpos;
	      
	      int retdb = insert_db_cstring(i_db, head_id, &head, &c);

	      ++*io_insert_cstring;
	      
	      if (retdb != 0)
		{
		  return -1;
		}
	      
	      lastpos = nextcontrol + 1;
	    }
	}
      
    }
  
  /* ok. we are out normal 
  * we adjust end and newend and memove the bufer rest
  */
 leave1:

  return 0;
}


/*********************************************************************/

/* 
 * postgreSQL code is here 
 */

static char *kw[9] =
  {
    "host",
    "hostaddr",
    "port",
    "dbname",
    "user",
    "password",
    "connect_timeout",
    "application_name",
    0
  };

static char *val[9] = { 0 } ; 

static char appname[63] = { 0 } ;

/**
 * db specific funtion : create
 */
static atrshmlog_dbsession_t* create_db(void)
{
  atrshmlog_dbsession_t* r =   (atrshmlog_dbsession_t*)calloc(1, sizeof(  atrshmlog_dbsession_t));

  if (r == NULL)
    return r;

  //  printf("create_db alloc\n");
  
  // we have ram ...

  r->status  = 0; // no init so far.

  r->keywords = (const char* const *)kw;

  if (val[0] == NULL || val[0][0] == 0)
    {
      val[0] = (char*)ATRSHMLOG_GET_ENV("_PQHOST");
      val[1] = (char*)ATRSHMLOG_GET_ENV("_PQHOSTADDR");
      val[2] = (char*)ATRSHMLOG_GET_ENV("_PQPORT");
      val[3] = (char*)ATRSHMLOG_GET_ENV("_PQDBNAME");
      val[4] = (char*)ATRSHMLOG_GET_ENV("_PQUSER");
      val[5] = (char*)ATRSHMLOG_GET_ENV("_PQPWD");
      val[6] = "10"; // 10 seconds

      snprintf(appname, 63, "ATRSHMLOGCONVERT_PG%ld", (long)getpid());
    
      val[7] = appname;

      val[8] = 0;

      if (val[0][0] == 0)
	return NULL;
    }
  
  //   printf("create_db alloc %s\n", val[0]);
  // printf("create_db alloc %s\n", val[1]);
  // printf("create_db alloc %s\n", val[2]);
  // printf("create_db alloc %s\n", val[3]);
  // printf("create_db alloc %s\n", val[4]);
  // printf("create_db alloc %s\n", val[5]);
  // printf("create_db alloc %s\n", val[6]);
  // printf("create_db alloc %s\n", val[7]);
  
  r->values = (const char* const *)val;

  int expand_dbname = 0;
  
  r->conn = PQconnectdbParams(r->keywords,
			      r->values,
			      expand_dbname);

  if (r->conn == NULL)
    {
      free(r);
      printf("create_db no connect\n");
      return NULL;
    }

  ConnStatusType pqstat = PQstatus(r->conn);

  if (pqstat != CONNECTION_OK)
    {
      printf("create_db connect bad %s\n", PQerrorMessage(r->conn));
      free(r);
      return NULL;
    }

  // we are connected. from now on we need a PGfinish.
  r->status = 1;

  // prepare the statements
  r->res = PQprepare(r->conn,
		     "commit_db",
		     "COMMIT",
		     0,
		     NULL);

  if (r->res == NULL)
    {
      printf("create_db result null.\n");
      cleanup_db(r);
      return NULL;
    }

      
  ExecStatusType es = PQresultStatus(r->res);

  if (es != PGRES_COMMAND_OK)
    {
      printf("create_db commit bad %s\n", PQerrorMessage(r->conn));
      cleanup_db(r);
      return NULL;
    }

  PQclear(r->res);

  r->res = PQprepare(r->conn,
		     "rollback_db",
		     "ROLLBACK",
		     0,
		     NULL);

  if (r->res == NULL)
    {
      printf("create_db result null.\n");
      cleanup_db(r);
      return NULL;
    }

  es = PQresultStatus(r->res);

  if (es != PGRES_COMMAND_OK)
    {
      printf("create_db rollback bad %s\n", PQerrorMessage(r->conn));
      cleanup_db(r);
      return NULL;
    }

  PQclear(r->res);

  r->res = PQprepare(r->conn,
		     "begin_db",
		     "BEGIN",
		     0,
		     NULL);

  if (r->res == NULL)
    {
      printf("create_db result null.\n");
      cleanup_db(r);
      return NULL;
    }

  es = PQresultStatus(r->res);

  if (es != PGRES_COMMAND_OK)
    {
      printf("create_db begin bad %s\n", PQerrorMessage(r->conn));
      cleanup_db(r);
      return NULL;
    }

  PQclear(r->res);

  r->res = PQprepare(r->conn,
		     "get_head_seq_db",
		     "SELECT nextval('atrshmlog_head_seq')",
		     0,
		     NULL);

  if (r->res == NULL)
    {
      printf("create_db result null.\n");
      cleanup_db(r);
      return NULL;
    }

  es = PQresultStatus(r->res);

  if (es != PGRES_COMMAND_OK)
    {
      printf("create_db get_head_seq bad %s\n", PQerrorMessage(r->conn));
      cleanup_db(r);
      return NULL;
    }

  PQclear(r->res);

  r->res = PQprepare(r->conn,
		     "insert_db_head",
		     "insert into atrshmlog_head(head_id,"
		     "version,"
		     "totallen,"
		     "pid,"
		     "tid,"
		     "bnr,"
		     "fnr,"
		     "inittime_sec,"
		     "inittime_nsec,"
		     "inittime_before,"
		     "inittime_after,"
		     "lasttime_sec,"
		     "lasttime_nsec,"
		     "lasttime_before,"
		     "lasttime_after,"
		     "difftimetransfer,"
		     "starttransfer,"
		     "acquiretime,"
		     "bid,"
		     "number_dispatched,"
		     "counter_write0,"
		     "counter_write0_discard,"
		     "counter_write0_wait,"
		     "counter_write0_adaptive,"
		     "counter_write0_adaptive_fast,"
		     "counter_write0_adaptive_very_fast,"
		     "counter_write1,"
		     "counter_write1_discard,"
		     "counter_write1_wait,"
		     "counter_write1_adaptive,"
		     "counter_write1_adaptive_fast,"
		     "counter_write1_adaptive_very_fast,"
		     "counter_write2,"
		     "counter_write2_discard,"
		     "counter_write2_wait,"
		     "counter_write2_adaptive,"
		     "counter_write2_adaptive_fast,"
		     "counter_write2_adaptive_very_fast)"
		     " values ("
		     "$1::bigint,"
		     "$2::integer,"
		     "$3::integer,"
		     "$4::bigint,"
		     "$5::bigint,"
		     "$6::integer,"
		     "$7::integer,"
		     "$8::bigint,"
		     "$9::integer,"
		     "$10::bigint,"
		     "$11::bigint,"
		     "$12::bigint,"
		     "$13::integer,"
		     "$14::bigint,"
		     "$15::bigint,"
		     "$16::bigint,"
		     "$17::bigint,"
		     "$18::bigint,"
		     "$19::integer,"
		     "$20::integer,"
		     "$21::integer,"
		     "$22::integer,"
		     "$23::integer,"
		     "$24::integer,"
		     "$25::integer,"
		     "$26::integer,"
		     "$27::integer,"
		     "$28::integer,"
		     "$29::integer,"
		     "$30::integer,"
		     "$31::integer,"
		     "$32::integer,"
		     "$33::integer,"
		     "$34::integer,"
		     "$35::integer,"
		     "$36::integer,"
		     "$37::integer,"
		     "$38::integer)",
		     0,
		     NULL);

  if (r->res == NULL)
    {
      printf("create_db result null.\n");
      cleanup_db(r);
      return NULL;
    }

  es = PQresultStatus(r->res);

  if (es != PGRES_COMMAND_OK)
    {
      printf("create_db insert_db_head bad %s\n", PQerrorMessage(r->conn));
      cleanup_db(r);
      return NULL;
    }

  PQclear(r->res);

  r->res = PQprepare(r->conn,
		     "insert_db_cstring",
		     "insert into atrshmlog_log(head_id,pid,tid,startclick,endclick,deltaclick,startreal,endreal,deltareal,logevent,logflag,userflag,payload) "
		     " values ("
		     "$1::bigint,"
		     "$2::bigint,"
		     "$3::bigint,"
		     "$4::bigint,"
		     "$5::bigint,"
		     "$6::bigint,"
		     "$7::bigint,"
		     "$8::bigint,"
		     "$9::bigint,"
		     "$10::integer,"
		     "$11::char(1),"
		     "$12::integer,"
		     "$13)",
		     0,
		     NULL);

  if (r->res == NULL)
    {
      printf("create_db result null.\n");
      cleanup_db(r);
      return NULL;
    }

  es = PQresultStatus(r->res);

  if (es != PGRES_COMMAND_OK)
    {
      printf("create_db insert_db_cstring bad %s\n", PQerrorMessage(r->conn));
      cleanup_db(r);
      return NULL;
    }

  PQclear(r->res);

  r->res = PQprepare(r->conn,
		     "insert_db_ucs2string",
		     "insert into atrshmlog_log_ucs2(head_id,pid,tid,startclick,endclick,deltaclick,startreal,endreal,deltareal,logevent,logflag,userflag,payload) "
		     " values ("
		     "$1::bigint,"
		     "$2::bigint,"
		     "$3::bigint,"
		     "$4::bigint,"
		     "$5::bigint,"
		     "$6::bigint,"
		     "$7::bigint,"
		     "$8::bigint,"
		     "$9::bigint,"
		     "$10::integer,"
		     "$11::char(1),"
		     "$12::integer,"
		     "$13)",
		     0,
		     NULL);

  if (r->res == NULL)
    {
      printf("create_db result null.\n");
      cleanup_db(r);
      return NULL;
    }

  es = PQresultStatus(r->res);

  if (es != PGRES_COMMAND_OK)
    {
      printf("create_db insert_db_ucs2string bad %s\n", PQerrorMessage(r->conn));
      cleanup_db(r);
      return NULL;
    }

  PQclear(r->res);

  
  // we have prepared statements
  r->status = 2;
  
  return r;
}

/**
 * db specific funtion : cleanup
 */
static void cleanup_db(atrshmlog_dbsession_t* i_db)
{
  if (i_db == NULL)
    return;

  if (i_db->status == 0)
    return;

  if (i_db->status == 2)
    {
      free_prepared_db(i_db);
    }
  
  if (i_db->status == 3)
    {
      rollback_db(i_db);

      free_prepared_db(i_db);
    }
  
  PQfinish(i_db->conn);

  i_db->status = 0;

  free(i_db);
}

/**
 * db specific funtion : begin
 */
static int begin_db(atrshmlog_dbsession_t* i_db)
{
  if (i_db == NULL)
    return -1;

  if (i_db->status != 2)
    return -2;

  i_db->res = PQexecPrepared(i_db->conn,
			     "begin_db",
			     0, // no parameters
			     NULL, 
			     NULL,
			     NULL,
			     1);

  ExecStatusType es = PQresultStatus(i_db->res);

  if (es != PGRES_COMMAND_OK)
    {
      printf("begin_db bad %s\n", PQerrorMessage(i_db->conn));
    }
  
  PQclear(i_db->res);

  if (es != PGRES_COMMAND_OK)
    {
      return -3;
    }
  
  // we are now in a transaction
  i_db->status = 3;

  return 0;
}

/**
 * db specific funtion : commit
 */
static int commit_db(atrshmlog_dbsession_t* i_db)
{
  if (i_db == NULL)
    return -1;

  if (i_db->status != 3)
    return -2;

  i_db->res = PQexecPrepared(i_db->conn,
			     "commit_db",
			     0, // no parameters
			     NULL, 
			     NULL,
			     NULL,
			     1);
  
  ExecStatusType es = PQresultStatus(i_db->res);

  if (es != PGRES_COMMAND_OK)
    {
      printf("commit_db bad %s\n", PQerrorMessage(i_db->conn));
    }
  
  PQclear(i_db->res);

  if (es != PGRES_COMMAND_OK)
    {
      return -3;
    }

  // we have ended the transaction
  i_db->status = 2;

  return 0;
}

/**
 * db specific funtion : rollback
 */
static int rollback_db(atrshmlog_dbsession_t* i_db)
{
  if (i_db == NULL)
    return -1;

  if (i_db->status != 3)
    return -2 ;

  i_db->res = PQexecPrepared(i_db->conn,
			     "rollback_db",
			     0, // no parameters
			     NULL, 
			     NULL,
			     NULL,
			     1);
  ExecStatusType es = PQresultStatus(i_db->res);

  if (es != PGRES_COMMAND_OK)
    {
      printf("rollback_db bad %s\n", PQerrorMessage(i_db->conn));
    }
  
  PQclear(i_db->res);

  if (es != PGRES_COMMAND_OK)
    {
      return -3;
    }

  // we have ended the transaction
  i_db->status = 2;

  return 0;
}


/**
 * db specific funtion : insert_db_head
 */
static int insert_db_head(atrshmlog_dbsession_t* i_db,
			     uint64_t i_head_id,
			     atrshmlog_io_head_t *i_head)
{
  if (i_db  == NULL)
    return -1;

  if (i_db->status != 3)
    return -2;

  char*pv[39];

  pv[0] = (char*)&i_head_id,
  pv[1] = (char*)&i_head->version;
  pv[2] = (char*)&i_head->tlen;
  pv[3] = (char*)&i_head->pid;
  pv[4] = (char*)&i_head->tid;
  pv[5] = (char*)&i_head->buffernumber;
  pv[6] = (char*)&i_head->filenumber;
  pv[7] = (char*)&i_head->inittime.tv_sec;
  pv[8] = (char*)&i_head->inittime.tv_nsec;
  pv[9] = (char*)&i_head->inittsc_before;
  pv[10] = (char*)&i_head->inittsc_after;
  pv[11] = (char*)&i_head->lasttime.tv_sec;
  pv[12] = (char*)&i_head->lasttime.tv_nsec;
  pv[13] = (char*)&i_head->lasttsc_before;
  pv[14] = (char*)&i_head->lasttsc_after;
  pv[15] = (char*)&i_head->difftimetransfer;
  pv[16] = (char*)&i_head->starttransfer;
  pv[17] = (char*)&i_head->acquiretime;
  pv[18] = (char*)&i_head->id;
  pv[19] = (char*)&i_head->number_dispatched;
  pv[20] = (char*)&i_head->counter_write0;
  pv[21] = (char*)&i_head->counter_write0_discard;
  pv[22] = (char*)&i_head->counter_write0_wait;
  pv[23] = (char*)&i_head->counter_write0_adaptive;
  pv[24] = (char*)&i_head->counter_write0_adaptive_fast;
  pv[25] = (char*)&i_head->counter_write0_adaptive_very_fast;
  pv[26] = (char*)&i_head->counter_write1;
  pv[27] = (char*)&i_head->counter_write1_discard;
  pv[28] = (char*)&i_head->counter_write1_wait;
  pv[29] = (char*)&i_head->counter_write1_adaptive;
  pv[30] = (char*)&i_head->counter_write1_adaptive_fast;
  pv[31] = (char*)&i_head->counter_write1_adaptive_very_fast;
  pv[32] = (char*)&i_head->counter_write2;
  pv[33] = (char*)&i_head->counter_write2_discard;
  pv[34] = (char*)&i_head->counter_write2_wait;
  pv[35] = (char*)&i_head->counter_write2_adaptive;
  pv[36] = (char*)&i_head->counter_write2_adaptive_fast;
  pv[37] = (char*)&i_head->counter_write2_adaptive_very_fast;
  pv[38] = 0;

  int plen[39] ;

  plen[0] = 8;
  plen[1] = 4;
  plen[2] = 4;
  plen[3] = 8;
  plen[4] = 8;
  plen[5] = 4;
  plen[6] = 4;
  plen[7] = 8;
  plen[8] = 4;
  plen[9] = 8;
  plen[10] = 8;
  plen[11] = 8;
  plen[12] = 4;
  plen[13] = 8;
  plen[14] = 8;
  plen[15] = 8;
  plen[16] = 8;
  plen[17] = 8;
  plen[18] = 4;
  plen[19] = 4;
  plen[20] = 4;
  plen[21] = 4;
  plen[22] = 4;
  plen[23] = 4;
  plen[24] = 4;
  plen[25] = 4;
  plen[26] = 4;
  plen[27] = 4;
  plen[28] = 4;
  plen[29] = 4;
  plen[30] = 4;
  plen[31] = 4;
  plen[32] = 4;
  plen[33] = 4;
  plen[34] = 4;
  plen[35] = 4;
  plen[36] = 4;
  plen[37] = 4;
  plen[38] = 0;

  int pfor[39] ;

  pfor[0] = 1;
  pfor[1] = 1;
  pfor[2] = 1;
  pfor[3] = 1;
  pfor[4] = 1;
  pfor[5] = 1;
  pfor[6] = 1;
  pfor[7] = 1;
  pfor[8] = 1;
  pfor[9] = 1;
  pfor[10] = 1;
  pfor[11] = 1;
  pfor[12] = 1;
  pfor[13] = 1;
  pfor[14] = 1;
  pfor[15] = 1;
  pfor[16] = 1;
  pfor[17] = 1;
  pfor[18] = 1;
  pfor[19] = 1;
  pfor[20] = 1;
  pfor[21] = 1;
  pfor[22] = 1;
  pfor[23] = 1;
  pfor[24] = 1;
  pfor[25] = 1;
  pfor[26] = 1;
  pfor[27] = 1;
  pfor[28] = 1;
  pfor[29] = 1;
  pfor[30] = 1;
  pfor[31] = 1;
  pfor[32] = 1;
  pfor[33] = 1;
  pfor[34] = 1;
  pfor[35] = 1;
  pfor[36] = 1;
  pfor[37] = 1;
  pfor[38] = 1;
  
  i_db->res = PQexecPrepared(i_db->conn,
			     "insert_db_head",
			     38,
			     (const char * const *)pv,
			     (const int *)plen,
			     (const int *)pfor,
			     1); // we do this binary
  
  ExecStatusType es = PQresultStatus(i_db->res);

  if (es != PGRES_COMMAND_OK)
    {
      printf("insert_db_head bad %s\n", PQerrorMessage(i_db->conn));
    }
  
  PQclear(i_db->res);

  if (es != PGRES_COMMAND_OK)
    {
      return -3;
    }

  return 0;
}

/**
 * db specific funtion : insert_db_cstring
 */
static int insert_db_cstring(atrshmlog_dbsession_t* i_db,
			     uint64_t i_head_id,
			     atrshmlog_io_head_t *i_head,
			     atrshmlog_chunk_head_t* i_chunk)
{
  if (i_db  == NULL)
    return -1;

  if (i_db->status != 3)
    return -2;

  char*pv[15];

  pv[0] = (char*)&i_head_id,
  pv[1] = (char*)&i_head->pid;
  pv[2] = (char*)&i_head->tid;
  pv[3] = (char*)&i_chunk->starttime;
  pv[4] = (char*)&i_chunk->endtime;
  pv[5] = (char*)&i_chunk->deltatime;
  pv[6] = (char*)&i_chunk->startreal;
  pv[7] = (char*)&i_chunk->endreal;
  pv[8] = (char*)&i_chunk->deltareal;
  pv[9] = (char*)&i_chunk->eventnumber;
  pv[10] = (char*)&i_chunk->eventflag;
  pv[11] = (char*)&i_chunk->userflag;
  pv[12] = (char*)i_chunk->payload;
  pv[13] = 0;

  int plen[15] ;

  plen[0] = 8;
  plen[1] = 8;
  plen[2] = 8;
  plen[3] = 8;
  plen[4] = 8;
  plen[5] = 8;
  plen[6] = 8;
  plen[7] = 8;
  plen[8] = 8;
  plen[9] = 4;
  plen[10] = 1;
  plen[11] = 4;
  plen[12] = strlen(i_chunk->payload);
  plen[13] = 0;

  int pfor[15] ;

  pfor[0] = 1;
  pfor[1] = 1;
  pfor[2] = 1;
  pfor[3] = 1;
  pfor[4] = 1;
  pfor[5] = 1;
  pfor[6] = 1;
  pfor[7] = 1;
  pfor[8] = 1;
  pfor[9] = 1;
  pfor[10] = 1;
  pfor[11] = 1;
  pfor[12] = 0;
  pfor[13] = 1;
  
  i_db->res = PQexecPrepared(i_db->conn,
			     "insert_db_cstring",
			     13,
			     (const char * const *)pv,
			     (const int *)plen,
			     (const int *)pfor,
			     1); // we do this binary
  
  ExecStatusType es = PQresultStatus(i_db->res);

  if (es != PGRES_COMMAND_OK)
    {
      printf("insert_db_cstring bad %s\n", PQerrorMessage(i_db->conn));
    }
  
  PQclear(i_db->res);

  if (es != PGRES_COMMAND_OK)
    {
      return -3;
    }

  return 0;
}


/**
 * db specific funtion : insert_db_cstring
 */
static int insert_db_ucs2string(atrshmlog_dbsession_t* i_db,
				uint64_t i_head_id,
				atrshmlog_io_head_t *i_head,
				atrshmlog_chunk_head_t* i_chunk,
				int len)
{
  if (i_db  == NULL)
    return -1;

  if (i_db->status != 3)
    return -2;

  char*pv[15];

  pv[0] = (char*)&i_head_id,
  pv[1] = (char*)&i_head->pid;
  pv[2] = (char*)&i_head->tid;
  pv[3] = (char*)&i_chunk->starttime;
  pv[4] = (char*)&i_chunk->endtime;
  pv[5] = (char*)&i_chunk->deltatime;
  pv[6] = (char*)&i_chunk->startreal;
  pv[7] = (char*)&i_chunk->endreal;
  pv[8] = (char*)&i_chunk->deltareal;
  pv[9] = (char*)&i_chunk->eventnumber;
  pv[10] = (char*)&i_chunk->eventflag;
  pv[11] = (char*)&i_chunk->userflag;
  pv[12] = (char*)i_chunk->payload;
  pv[13] = 0;

  int plen[15] ;

  plen[0] = 8;
  plen[1] = 8;
  plen[2] = 8;
  plen[3] = 8;
  plen[4] = 8;
  plen[5] = 8;
  plen[6] = 8;
  plen[7] = 8;
  plen[8] = 8;
  plen[9] = 4;
  plen[10] = 1;
  plen[11] = 4;
  plen[12] = len;
  plen[13] = 0;

  int pfor[15] ;

  pfor[0] = 1;
  pfor[1] = 1;
  pfor[2] = 1;
  pfor[3] = 1;
  pfor[4] = 1;
  pfor[5] = 1;
  pfor[6] = 1;
  pfor[7] = 1;
  pfor[8] = 1;
  pfor[9] = 1;
  pfor[10] = 1;
  pfor[11] = 1;
  pfor[12] = 1;
  pfor[13] = 1;
  
  i_db->res = PQexecPrepared(i_db->conn,
			     "insert_db_ucs2string",
			     13,
			     (const char * const *)pv,
			     (const int *)plen,
			     (const int *)pfor,
			     1); // we do this binary
  
  ExecStatusType es = PQresultStatus(i_db->res);

  if (es != PGRES_COMMAND_OK)
    {
      printf("insert_db_cstring bad %s\n", PQerrorMessage(i_db->conn));
    }
  
  PQclear(i_db->res);

  if (es != PGRES_COMMAND_OK)
    {
      return -3;
    }

  return 0;
}

/**
 * db specific funtion : get_head_seq_db
 */
static int get_head_seq_db(atrshmlog_dbsession_t* i_db,
			   uint64_t *o_head_id)
{
  if (i_db  == NULL)
    return -1;

  if (i_db->status != 3)
    return -2;

  
  i_db->res = PQexecPrepared(i_db->conn,
			     "get_head_seq_db",
			     0,
			     (const char * const *)NULL,
			     (const int *)NULL,
			     (const int *)NULL,
			     1); // we do this binary
  
  
  ExecStatusType es = PQresultStatus(i_db->res);

  int i;
  
  if (es != PGRES_TUPLES_OK)
    {
      printf("get_head_seq_db bad %d : %s :\n", es, PQerrorMessage(i_db->conn));
    }
  else
    {
      // we get the new head_id

      int fnum = PQfnumber(i_db->res, "nextval");

      for (i = 0; i < PQntuples(i_db->res); i++)
	{
	  char *val = PQgetvalue(i_db->res, i, fnum);

	  // we get it in network order AND WE DONT CHANGE IT ...
	  *o_head_id = *(uint64_t*)val;
	}
    }

  
  PQclear(i_db->res);

  if (es != PGRES_TUPLES_OK)
    {
      return -3;
    }

  // empty result...
  if (i < 1)
    {
      return -4;
    }
  
  return 0;
}

/**
 * helper: free our session cached prepared statements
 */
static int free_prepared_db(atrshmlog_dbsession_t *i_db)
{
  i_db->res = PQexec(i_db->conn, "DEALLOCATE " "commit_db");
  PQclear(i_db->res);
  i_db->res = PQexec(i_db->conn, "DEALLOCATE " "rollback_db");
  PQclear(i_db->res);
  i_db->res = PQexec(i_db->conn, "DEALLOCATE " "begin_db");
  PQclear(i_db->res);
  i_db->res = PQexec(i_db->conn, "DEALLOCATE " "get_head_seq_db");
  PQclear(i_db->res);
  i_db->res = PQexec(i_db->conn, "DEALLOCATE " "insert_db_head");
  PQclear(i_db->res);
  i_db->res = PQexec(i_db->conn, "DEALLOCATE " "insert_db_cstring");
  PQclear(i_db->res);
  i_db->res = PQexec(i_db->conn, "DEALLOCATE " "insert_db_ucs2string");
  PQclear(i_db->res);

  return 0;
}


/* end of file */
