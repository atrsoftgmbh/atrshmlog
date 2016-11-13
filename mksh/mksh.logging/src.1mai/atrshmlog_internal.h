#ifndef INCLUDED_ATRSHMLOG_INTERNAL_H
#define INCLUDED_ATRSHMLOG_INTERNAL_H

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

/*
 * major version number. defines structural identity for all
 * accessing programs for the structure in the log buffer
 */
#define ATRSHMLOGVERSION (1)

/*
 * minor version number
 * defines level of functionality on top of the buffer structures.
 */
#define ATRSHMLOGMINORVERSION (0)

/*
 * patch level for handling of errors and changes internal 
 */
#define ATRSHMLOGPATCHVERSION (0)

/*
 * define setings to make use of the needed additional
 * stuff in the unistd header.
 * if it hurts you simply remove not needed ones
 */

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

/* include of types for unix stuff */
#include <sys/types.h>

/* the ipc stuff */
#include <sys/ipc.h>

/* the shm stuff */
#include <sys/shm.h>

/* all the rest of unix */
#include <unistd.h>

/* file creation masks */
#include <fcntl.h>

/* time meaturement */
#include <time.h>

/* low level string ops , namely memcpy */
#include <string.h>

/* file io */
#include <stdio.h>

/* error code */
#include <errno.h>

/* checks for types of chars */
#include <ctype.h>

/* standard c stuff */  
#include <stdlib.h>

/*
 * we use posix threads here. 
 * if you want to do it on a platform
 * without simply check for the existance 
 * of 
 * mutex, 
 * syncronisation of process
 * etc.
 * 
 * then rewrite the code that is based on the 
 * pthread functions.
 */

/* pthread */

#include <pthread.h>

/*
 * platform dependent stuff here 
 */

typedef pthread_t atrshmlog_tid_t;

#define ATRSHMLOG_GETTHREADID (pthread_self())

typedef pthread_mutex_t atrshmlog_mutex_t;

typedef struct timespec atrshmlog_internal_time_t;

/* we use the pid here of posix land. thats ok, even if we are not in 
 * linux land 
 */
typedef pid_t atrshmlog_pid_t;

/* the shm log struct and the defines */

/* buffer size for data 
 * we know that 32 mb is a old school max limit for shm
 * so we use something nearby 100 buffer in it
 */
#define ATRSHMLOGBUFFER_INFOSIZE (1024 * 256 * 1)

/* we need the half size here. this is used
 * by the write to decide to use the halffull state 
 * the reader uses normally only the halffull buffers and full buffers
 * so this means we try to let the log not succeed more than 
 * this limit 
 */
#define ATRSHMLOGBUFFER_INFOSIZE_HALF (1024 * 128 * 1)

/* the technical minimum count of buffers -> ask atr */
#define ATRSHMLOGBUFFER_MINCOUNT (2)

/* the technical maximum count -> ask atr */
#define ATRSHMLOGBUFFER_MAXCOUNT (255)

/* the access mode flags for the shm buffer */
#define ATRSHMLOG_ACCESS (0666)

/* we give access to all users - this is done for sequences
 * with su and su -  usage ... 
 * so dont misinterpret it.
 * it is save to use the buffer because everyone on
 * the box is interestet to make progress for the 
 * bugfix and perf problem stuff.
 * so all people are nice and do not misinterpret the open
 * shm block as a playground ...
 * at least i hope this
 */

/* sizes */

/* all relevant defines, from atrshmlogdefect, size in bytes 
 * we use hard numbers, see output of atrshmlogdefect and then adust.
 */


#define ATRSHMLOGCONTROLDATASIZE (64)

/* helper for binary transfers */
struct atrshmlog_header_s {
  atrshmlog_internal_time_t starttime;
  atrshmlog_internal_time_t endtime;
  atrshmlog_tid_t tid;
  atrshmlog_pid_t pid;
  int totallen;
  int eventnumber;
  int userevent;
  char eventflag;
};

typedef struct atrshmlog_header_s atrshmlog_header_t;

/* we use a buffer to collect the argv data first in the 
 * process side part.
 * then we move this in one operation to target
 * this reduces the contention time in the mutex lock
 * so this is a way to reduce lock time, buut it has a cost
 * compared to directly moving to target in shm.
 * we use heire a len
 */
#define ATRSHMLOGARGVBUFFERLEN (65536)

/* if this is null / (void*)0  we have no log.
 * this is used in macro check.
 * so its accessible from everywhere.
 * please dont misuse it.
 */
extern volatile void* atrshmlog_base_ptr;

/* helper : we are loging */
#define ATRSHMLOGLOGGING (!(atrshmlog_base_ptr == (void*)0))

/* helper : we are the shm area */
#define ATRSHMLOGGETAREA ((atr_shm_log_area_t *)atrshmlog_base_ptr)

/* the number of events. from 0 to this - 1 */
#define ATRSHMLOGEVENTCOUNT (10000)

/* the env variable prefix default
 * and the env var to overwrite it 
 */
#define ATRSHMLOG_ENV_PREFIX "ATRSHMLOG"

/* the env variable with the shmid */
#define ATRSHMLOGENVSUFFIX "_ID"

/* in case we have a login shell we need a file instead */
#define ATRSHMLOGENVFILESUFFIX "_ID.TXT"

/* the flag for inverting the event mechanism */
#define ATRSHMLOGEVENTNULLSUFFIX "_EVENT_NULL"

/* in case we have a login shell we need it as a file */
#define ATRSHMLOGEVENTNULLFILESUFFIX "_EVENT_NULL.TXT"

/* the vector of eevnets to switch on or off */
#define ATRSHMLOGEVENTONOFFSUFFIX "_EVENT_ONOFF"

/* in case we have a login shell we need it as a file */ 
#define ATRSHMLOGEVENTONOFFFILESUFFIX "_EVENT_ONOFF.TXT"

/* we want the pid caching . no more getpid than one 
 * this is ok in a process that does not fork / spawn / create childs
 * that log themself. if the childs are logging and there is no
 * inbetween process that overwrite the image the child uses
 * the already connected buffer and all cached values (this mean
 * the event flag buffer, the pointers and - the cached pid ...
 * .. so dont think that this has to work in the case
 * of fork/ spawn et all ...
 * its up to you if you know it is allowed or not.
 */
#define ATRSHMLOGCACHEPIDSUFFIX "_CACHE_PID"

/* we can change the delimiter if we want for the dynamic part */ 
#define ATRSHMLOGDELIMITERSUFFIX "_DELIMITER_VALUE"


/* 
 * the states of a buffer
 */
enum atr_shm_log_buffer_states {
  atr_shm_log_uninit = 0,
  atr_shm_log_writeable= 1 ,
  atr_shm_log_write_halffull = 2,
  atr_shm_log_full = 4,
  atr_shm_log_illegal = 8
};

/* we have a buffer here
 * it has a state, an actual size between 0 and FULLSIZE
 * an mutex for locking
 * the user slots for signaling the reader to wait
 * and the buffer itself
 */
#define ATRSHMLOGSLOTCOUNT (128)

struct atr_shm_log_buffer_s {
  volatile int state;

  volatile int size;

  atrshmlog_mutex_t lock_mutex;

  volatile int count_writes ;

  volatile int count_uses;
  
  volatile  int count_memcpy;

  volatile int count_state_ok_first_check;

  volatile int count_state_not_ok;
  
  volatile int count_state_not_ok_second;
  
  volatile int count_mutex_lock;

  volatile int count_safeguard_problem;

  volatile int count_state_check_ok_second_check;

  volatile int count_buffer_filled;

  volatile int count_slot_checks;

  volatile int count_slots_full;


  /* we use a guard technique here in checks */
  volatile long slots[ATRSHMLOGSLOTCOUNT + 1];
  
  char info[ ATRSHMLOGBUFFER_INFOSIZE ];

  volatile long safeguard;
};

typedef struct atr_shm_log_buffer_s atr_shm_log_buffer_t;

/* we have the whole log area
 * the shmid as a init is done flag
 * the count of uffers
 * and the array of buffers as a stub. minimum is two
 * 
 * ro read only
 * rw read and write
 */
struct atr_shm_log_area_s {
  volatile int version; /* version of the buffer system ro */

  volatile int shmid; /* reflexion. myself ro for users */

  volatile long safeguard; /* a check pattern, ro for user */
  
  volatile int count; /* buffer count is ... ro for users
		       * we allow only the lowest byte to be used
		       * so never try to use more than 0 .. 255
		       */

  volatile int lastused; /* this is a hint to use the next buffer first 
			  * in a try, can be 0 to count minus 1 
			  * we allow only the lowest byte to be used
			  * so never try to use more than 0 .. 255
			  * ro
			  */

  volatile int ich_habe_fertig; /* flag to stop logging in the system, 0 run, <>0 stop 
				 * we allow only the lowest byte to be used
				 * so never try to use more than 0 .. 255
				 * rw
				 */

  volatile int readerflag; /* info for the reader rw */

  volatile atrshmlog_pid_t readerpid; /* the pid of the reader */
  
  volatile int writerflag ; /* info for the writer rw */

  volatile atrshmlog_pid_t writerpid; /* the pid of the writer */

  atr_shm_log_buffer_t logbuffers[ ATRSHMLOGBUFFER_MINCOUNT ];

  /* old style c trick. use the buffer start, but it can be more than the 
   * number in the def.
   * to work : means there is nothing after the buffers in the struct
   */
};

typedef struct atr_shm_log_area_s atr_shm_log_area_t;

#define SAFEGUARDVALUE (0xFE01FE01L)


/************************************************************************/
/* helper macros*/

#define ATRSHMLOGCUTOFFLOWBYTE(__x) (0xff & (__x))



/************************************************************************/

/* the funcs */

/* we create the shm */
extern int atrshmlog_create(const key_t i_key,
			 const int i_count);

/* we destroy the shm */
extern int atrshmlog_delete(const int i_shmid);

/* we verify the buffer is inited and ok */
extern int atrshmlog_verify();

/* we initialize the buffer after it is attached incl mutexes */
extern int atrshmlog_init_shm_log(const int i_count_buffers);

/* we have to clean up the mutexes after stopping */
extern void atrshmlog_cleanup_mutexes();

/* we read a buffer and write it to a local memory area */
extern int atrshmlog_read(const int i_index_buffer,
			  void *o_target,
			  int* o_target_length);


#endif
/* end of file */
