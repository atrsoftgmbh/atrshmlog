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

/** \file atrshmlog_internal.h 
 * \brief The details for internal usage are here.
 *
 * We normally dont change these things in this header.
 * Its only done if you really have to change additional code too.
 * So we have these things separated from the normal interface.
 */

// needed includes

#ifndef INCLUDED_ATRSHMLOG_INTERNAL_H
# define INCLUDED_ATRSHMLOG_INTERNAL_H

#ifndef INCLUDED_ATRSHMLOG_H
# include "atrshmlog.h"
#endif

/************************************************************************/

/* These are the major defines to parametrise the shm log.
 * This is for recompile only, so its in this header 
 * and not in the main header - the internals after all.
 *
 * You can redefine them and then simply recompile the module.
 * For some of them you have to change also the code.
 * But this is documented where the code is.
 * So first check you understand the code, then change the define
 * for your need.
 */

/**
 * \brief The major version number. 
 *
 * Defines structural identity for all
 * accessing programs for the structure in the log buffer.
 *
 * Change this when you add or remove or change any
 * - log buffer struct
 * - type in a buffer
 * - reader output additional to the buffer
 * - converter input
 */
#define ATRSHMLOGVERSION (1)

/**
 * \brief The minor version number.
 *
 * Defines level of functionality on top of the buffer structures.
 * Can mean additional functions for fences, sleeps etc.
 * Not meaning a simple bugfix.
 */
#define ATRSHMLOGMINORVERSION (1)

/**
 * \brief Patch level version for handling of errors and changes internal .
 *
 * No change for the  structure or any additional
 * functionality.
 */
#define ATRSHMLOGPATCHVERSION (0)


/** 
 * \brief The maximum buffer size for data.
 *
 * This is a hard default. You can use an env or the
 * set_buffer_infosize proc to chance it to another value
 * but be sure you really have that memory !
 *
 * And you can only make it smaller that way.
 *
 * Because we use a maxsize in the buffer
 * you can have different sizes in a system - after
 * reducing the size and making dyn call allocs.
 * But this is the fixed limit for this VERSION.
 *
 * So you can after all make a new version and set it bigger her.
 * Check the documentation for that.
 */
#define ATRSHMLOGBUFFER_INFOSIZE (1024 * 512)

/**
 * \brief The count of buffers that are preallocated as static buffers 
 * in the program. 
 *
 * You can allocate more by setting this to an higher value and
 * maintain the buffer allocation in the atrshmlog.c file.
 *
 * You have to do both, or you end up either with a corrupt 
 * list or with only the standard number of buffers.
 * See the doc for more about this.
 */
#define ATRSHMLOGBUFFER_PREALLOCED_COUNT (64)

/** 
 * \brief The technical minimum count of buffers in shm.
 */
#define ATRSHMLOGBUFFER_MINCOUNT (2)

/** 
 * \brief The technical maximum count of buffers in shm.
 */
#define ATRSHMLOGBUFFER_MAXCOUNT (1024)


/** 
 * \brief We give access to all users - this is done for sequences
 * With su and su -  usage ... 
 *
 * So dont misinterpret it.
 * It is save to use the buffer because everyone on
 * the box is interestet to make progress for the 
 * bugfix and perf problem stuff.
 * So all people are nice and do not misinterpret the open
 * shm block as a playground ...
 * At least i hope this.
 * If you need some knd of protection redefine this to 660
 * or 600 at least.
 */
#define ATRSHMLOG_ACCESS (0666)


/** 
 * \brief Wait default for the slave thread.
 *
 * Make it smaller for a prompt reaction.
 * But this means also a more or less higher use
 * of the CPU for checking and a pollution of caches.
 * You have to test this if you want to change it.
 */
#define ATRSHMLOG_INIT_BUFFER_SLAVE_WAIT (20000)


/** 
 * \brief Wait for shm free in mem to shm.
 *
 * Make it smaller for a better response time.
 * But this means a more CPU usage and cache pollution.
 */
#define ATRSHMLOG_INIT_SLAVE_TO_SHM_WAIT (50000)

/**
 * \brief Wait for this time in the strategy wait situation when
 * buffers are full
 */
#define ATRSHMLOG_STRATEGY_WAIT_WAIT_TIME_DEFAULT (125000)

/**
 * \brief Wait this long in the first adaptive strategy run before you
 * have a time for this buffers transfer time
 */
#define ATRSHMLOG_TRANSFER_TO_SHM_START_DEFAULT (250000)

/**
 * \brief The count of buffers that are fetched in a alloc call.
 *
 * Make it bigger to reduce overhead  when you dyn allocate
 * buffers. But the first alloc then takes all the 
 * price to pay for the alloc and the init stuff.
 * So its better to have the static buffers in place
 * instead. But if you have a dynamic application and 
 * no clue about the number of threads it is at least a 
 * start.
 */
#define ATRSHMLOG_INIT_PREALLOC_COUNT (64)
  

/**
 * \brief The max number of buffers that are fetched. 
 */
#define ATRSHMLOG_MAX_PREALLOC_COUNT (4096)
  
/**
 * \brief Helper max size for the argv handling in write2.
 *
 * We use a buffer to collect the argv data first in the 
 * process side part.
 * Then we move this in one operation to target.
 * This reduces the contention time in the lock
 * so this is a way to reduce lock time, but it has a cost
 * compared to directly moving to target in shm.
 */
#define ATRSHMLOGARGVBUFFERLEN (65536)

/** 
 * \brief Max events number ever possible
 */
#define ATRSHMLOGEVENTCOUNTMAXLIMIT (64L * 1024L * 1024L)

/** 
 * \brief The smallest info size we will use
 */
#define ATRSHMLOGBUFFER_INFOSIZE_MIN (16L * 1024L)

/** 
 * \brief The minimum number of prealloced buffers
 */
#define ATRSHMLOG_INIT_PREALLOC_COUNT_MIN (4L)

/** 
 * \brief The minimum number of nanos to wait in slave
 */
#define ATRSHMLOG_INIT_BUFFER_SLAVE_WAIT_MIN (23L)

/** 
 * \brief The maximum number of nanos to wait in slave
 */
#define ATRSHMLOG_INIT_BUFFER_SLAVE_WAIT_MAX (999999999UL)

/** 
 * \brief We can start with no slaves at all
 */
#define ATRSHMLOG_SLAVE_COUNT_MIN (0L)

/** 
 * \brief We use a max of this slaves 
 */
#define ATRSHMLOG_SLAVE_COUNT_MAX (4096L)

/** 
 * \brief The min id for the get clocktime
 */
#define ATRSHMLOG_CLOCK_ID_MIN (1)

/** 
 * \brief The max id for the get clocktime
 */
#define ATRSHMLOG_CLOCK_ID_MAX (3)

/** 
 * \brief The number of events default. 
 *
 * From 0 to this - 1 
 */
#define ATRSHMLOGEVENTCOUNT (10000)


/** 
 * \brief The number of buffers per thread.
 *
 * You can give more a try. Then you should 
 * have more buffers, too.
 * This can make the whole process looking smoth
 * but at the price of using more memory.
 * In theory the simple use of 2 should be enough.
 */
#define ATRSHMLOGTARGETBUFFERMAX (2)


/**
 * \brief This is the first constant for the point in time event
 */
#define ATRSHMLOGPOINTINTIMEP 'P'

/**
 * \brief This is the second constant for the point in time event
 */
#define ATRSHMLOGPOINTINTIMEp 'p'

/**
 * \brief This is the first constant for the interval in time event
 */
#define ATRSHMLOGPOINTINTIMEI 'I'

/**
 * \brief This is the second constant for the interval in time event
 */
#define ATRSHMLOGPOINTINTIMEi 'i'

/**************************************************************/

/* os specific defines */

#if ATRSHMLOG_PLATFORM_LINUX_X86_64_GCC == 1

/** 
 * This is for the gnu linux compiler system 
 */
#ifndef _DEFAULT_SOURCE
# define _DEFAULT_SOURCE 1
#endif

/** 
 * This is for our audiance the standard
 */
#ifndef _XOPEN_SOURCE
# define _XOPEN_SOURCE 1
#endif

/** 
 * We are on a posix system
 *
 *  We need at least a posix level 1993 
 */
#ifndef _POSIX_C_SOURCE
# define   _POSIX_C_SOURCE 199309L
#endif

#define ATRSHMLOGCDECLBINDING /**/

#endif

#if ATRSHMLOG_PLATFORM_CYGWIN_X86_64_GCC == 1

/** 
 * This is for the gnu linux compiler system 
 */
#ifndef _DEFAULT_SOURCE
# define _DEFAULT_SOURCE 1
#endif

/** 
 * This is for our audiance the standard
 */
#ifndef _XOPEN_SOURCE
# define _XOPEN_SOURCE 1
#endif

/** 
 * We are on a posix system
 *
 *  We need at least a posix level 1993 
 */
#ifndef _POSIX_C_SOURCE
# define   _POSIX_C_SOURCE 199309L
#endif

#define ATRSHMLOGCDECLBINDING /**/

#endif

#if ATRSHMLOG_PLATFORM_MINGW_X86_64_GCC == 1

// nothing so far of predefines here

#define ATRSHMLOGCDECLBINDING __cdecl

#endif


#if ATRSHMLOG_PLATFORM_BSD_AMD64_CLANG == 1

#define ATRSHMLOGCDECLBINDING /**/

#endif

#if ATRSHMLOG_PLATFORM_BSD_AMD64_GCC == 1

#define ATRSHMLOGCDECLBINDING /**/

#endif



/* os specifics includes */

#if ATRSHMLOG_PLATFORM_LINUX_X86_64_GCC == 1

/** Include of types for unix stuff */
# include <sys/types.h>

/** The ipc stuff */
# include <sys/ipc.h>

/** The shm stuff */
# include <sys/shm.h>

/** All the rest of unix */
# include <unistd.h>

/** File creation masks */
# include <fcntl.h>

/* the shared memory key in posix */

typedef key_t atrshmlog_key_t;

# define ATRSHMLOGINITINADVANCEDEFAULT 0

#define ATRSHMLOGSCALECLICKTONANO(__click) (((__click) * 5LL ) / 2LL)

#endif

#if ATRSHMLOG_PLATFORM_CYGWIN_X86_64_GCC == 1

/** Include of types for unix stuff */
# include <sys/types.h>

/** The ipc stuff */
# include <sys/ipc.h>

/** The shm stuff */
# include <sys/shm.h>

/** All the rest of unix */
# include <unistd.h>

/** File creation masks */
# include <fcntl.h>

/* the shared memory key in posix XSI with cygserver and cygrunsrv */

typedef key_t atrshmlog_key_t;

# define ATRSHMLOGINITINADVANCEDEFAULT 1

#define ATRSHMLOGSCALECLICKTONANO(__click) (((__click) * 5LL ) / 2LL)

#endif

#if ATRSHMLOG_PLATFORM_MINGW_X86_64_GCC == 1

/* the shared memory key in winemulation - index in name array */

# include <windows.h>
# include <process.h>

typedef long atrshmlog_key_t;

# if ATRSHMLOG_USE_PTHREAD == 1
#  include <unistd.h>
# else
// for windows we need the native sleep
#  define sleep(__s) Sleep((__s) * 1000)
# endif

# define ATRSHMLOGINITINADVANCEDEFAULT 1

#define ATRSHMLOGSCALECLICKTONANO(__click) (((__click) * 5LL ) / 2LL)

#endif

#if ATRSHMLOG_PLATFORM_BSD_AMD64_CLANG == 1

/** Include of types for unix stuff */
# include <sys/types.h>

/** The ipc stuff */
# include <sys/ipc.h>

/** The shm stuff */
# include <sys/shm.h>

// bsd freebsd 
#if ATRSHMLOG_FLAVOUR == 3
/** The thread stuff */
#include <sys/thr.h>
#endif

// bsd openbsd
#if ATRSHMLOG_FLAVOUR == 4
#include <unistd.h>
#endif

/** All the rest of unix */
# include <unistd.h>

/** File creation masks */
# include <fcntl.h>

/* the shared memory key in posix */

typedef key_t atrshmlog_key_t;

# define ATRSHMLOGINITINADVANCEDEFAULT 0

# define ATRSHMLOGSCALECLICKTONANO(__click) (((__click) * 5LL ) / 2LL)

#endif

#if ATRSHMLOG_PLATFORM_BSD_AMD64_GCC == 1

/** Include of types for unix stuff */
# include <sys/types.h>

/** The ipc stuff */
# include <sys/ipc.h>

/** The shm stuff */
# include <sys/shm.h>

// bsd netbsd 
#if ATRSHMLOG_FLAVOUR == 5
/** The thread stuff */
#include <lwp.h>
#endif

/** All the rest of unix */
# include <unistd.h>

/** File creation masks */
# include <fcntl.h>

/* the shared memory key in posix */

typedef key_t atrshmlog_key_t;

# define ATRSHMLOGINITINADVANCEDEFAULT 0

# define ATRSHMLOGSCALECLICKTONANO(__click) (((__click) * 5LL ) / 2LL)

#endif

/**************************************************************/

#if ATRSHMLOG_USE_PTHREAD == 1
// pthread needed for thread starting and pthread id 

#include <pthread.h>

typedef void* atrshmlog_thread_ret_t;

#define ATRSHMLOG_THREADFUNCTION_RETURN(__p) return (__p)

#endif

#if ATRSHMLOG_USE_C11_THREAD == 1

#include <threads.h>

typedef int atrshmlog_thread_ret_t;

#define ATRSHMLOG_THREADFUNCTION_RETURN(__p) return (__p)

#endif

#if ATRSHMLOG_USE_WINTHREAD == 1

#include <windows.h>
#include <process.h>

typedef void atrshmlog_thread_ret_t;

#define ATRSHMLOG_THREADFUNCTION_RETURN(__p) return

#endif


#if ATRSHMLOG_SYSCALL == 1

// we use syscall interface - gettid call
#include <sys/syscall.h>

#endif

#if ATRSHMLOG_USE_PTHREAD_TID == 1
#define ATRSHMLOG_GETTHREADID(__o) ((__o) = (pthread_self()))
#endif

#if ATRSHMLOG_USE_SYSCALL_TID == 1
#define ATRSHMLOG_GETTHREADID(__o) ((__o) = (syscall(SYS_gettid)))
#endif

#if ATRSHMLOG_USE_WINTHREAD_TID == 1
#define ATRSHMLOG_GETTHREADID(__o) ((__o) = ((long)GetCurrentThreadId()))
#endif 

#if ATRSHMLOG_USE_THR_SELF_TID == 1
#define ATRSHMLOG_GETTHREADID(__o) do { (__o) = 0; thr_self((long*)&(__o)); } while(0)
#endif

#if ATRSHMLOG_USE_GETTHRID == 1
#define ATRSHMLOG_GETTHREADID(__o) ((__o) = (getthrid()))
#endif

#if ATRSHMLOG_USE_LWP_SELF == 1
#define ATRSHMLOG_GETTHREADID(__o) ((__o) = (_lwp_self()))
#endif


#if ATRSHMLOG_LEVEL > 0

/** File io needed */
#include <stdio.h>

#endif

/** Time meaturement - this includes structs */
#include <time.h>

/** Low level string ops , namely memcpy */
#include <string.h>

/** Error code */
#include <errno.h>

/** Checks for types of chars */
#include <ctype.h>

/** Standard c stuff for alloc */  
#include <stdlib.h>

/** Standard types */
#include <stdint.h>

/** 
 * atomics
 *
 * This is risky. We NEED atomics. Period. So if the header is not there
 * you have to switch to another compiler. 
 * Sorry, but this is a hard fact. 
 * And if your favorite compiler does not support atomics
 * you can try to get a free implt that does.
 * So simply try to use what you can and then mix it up.
 */
#include <stdatomic.h>



/**************************************************************/

/* the shm log struct and the defines */


/**
 * The fixed constant for this VERSION of the 
 * size of the logging info head size.
 * We have actually 8 byte for a timer buffer so far
 * so we need two of them.
 * Next are the 3 32 bit values for the length, the event,
 * the userevent / userflag.
 * Last the char from the eventflag.
 * This has to match the sizes if you change any of them.
 * It is used in the write code, the converter and so its
 * a system wide info.
 */
#define ATRSHMLOGCONTROLDATASIZE (sizeof(atrshmlog_time_t) \
				  + sizeof(atrshmlog_time_t) \
				  + sizeof(atrshmlog_int32_t) \
				  + sizeof(atrshmlog_int32_t) \
				  + sizeof(atrshmlog_int32_t) \
				  + sizeof(atrshmlog_event_t))

/** 
 * Helper : we are loging 
 *
 * \return
 * - Zero if not attached
 * - Non zero if we are attached
 */
#define ATRSHMLOG_LOGGING (!(atrshmlog_base_ptr == (void*)0))

/** 
 * Helper : we are the shm area.
 *
 * \return
 * The pointer to the shm area.
 */
#define ATRSHMLOG_GETAREA ((atrshmlog_area_t *)atrshmlog_base_ptr)


/**
 * \brief The env variable prefix default
 *
 * and the env var to overwrite it .
 * Setting this Environment variable let you overwrite the prefix
 * with its value.
 * If you need a production logging and use of names is
 * not allowing the default names change this define.
 * Be sure you understand the use of variables before this.
 */
#define ATRSHMLOG_ENV_PREFIX "ATRSHMLOG"

/** 
 * \brief The env variable with the shmid has this suffix.
 */
#define ATRSHMLOGENVSUFFIX "_ID"


/**
 * \brief we need a count variable in case of mingw
 */
#define ATRSHMLOG_BUFFER_COUNT_SUFFIX "_COUNT"

/**
 * \brief we need to init in advance buffers 
 */ 
#define ATRSHMLOGINITINADVANCESUFFIX "_INIT_IN_ADVANCE"

/** 
 * \brief The flag for inverting the event mechanism 
 */
#define ATRSHMLOGEVENTNULLSUFFIX "_EVENT_NULL"

/** 
 * \brief The vector of events to switch on or off 
 */
#define ATRSHMLOGEVENTONOFFSUFFIX "_EVENT_ONOFF"

/** 
 * \brief We can change the delimiter if we want for the dynamic part 
 */ 
#define ATRSHMLOGDELIMITERSUFFIX "_DELIMITER_VALUE"

/** 
 * \brief The strategy rfo buffer handling - not used now
 */ 
#define ATRSHMLOGSTRATEGYSUFFIX "_STRATEGY"

/** 
 * \brief The strategy wait wait time nanos defauld redefine
 */ 
#define ATRSHMLOGSTRATEGYWAITTIMESUFFIX "_STRATEGY_WAIT_TIME"


/** 
 * \brief The numbers of events we reserve
 */
#define ATRSHMLOGEVENTCOUNTSUFFIX "_EVENT_COUNT_MAX"

/** 
 * \brief We use this to get the new infosize 
 */
#define ATRSHMLOGBUFFER_INFOSIZE_SUFFIX "_BUFFER_SIZE"

/** 
 * \brief The number of buffers we will preallocate in one malloc all
 */
#define ATRSHMLOG_INIT_PREALLOC_COUNT_SUFFIX "_PREALLOC_COUNT"

/** 
 * \brief We set the nanos a slave waits 
 */
#define ATRSHMLOG_INIT_BUFFER_SLAVE_WAIT_SUFFIX "_SLAVE_WAIT_NANOS"

/** 
 * \brief We can switch logging of
 */
#define ATRSHMLOG_LOGGING_OFF_SUFFIX "_LOGGING_IS_OFF_AT_START"

/** 
 * \brief We can start with a higher count of slaves
 */
#define ATRSHMLOG_SLAVE_COUNT_SUFFIX "_SLAVE_COUNT"

/** 
 * \brief Switch to wait for slaves on cleanup
 */
#define ATRSHMLOG_WAIT_FOR_SLAVES_SUFFIX "_WAIT_FOR_SLAVES_ON"

/** 
 * \brief The clock id in gettime
 */
#define ATRSHMLOG_CLOCK_ID_SUFFIX "_CLOCK_ID"

/** 
 * \brief The fence 1
 */
#define ATRSHMLOG_FENCE_1_SUFFIX "_FENCE_1"

/** 
 * \brief The fence 2
 */
#define ATRSHMLOG_FENCE_2_SUFFIX "_FENCE_2"


/** 
 * \brief The fence 3
 */
#define ATRSHMLOG_FENCE_3_SUFFIX "_FENCE_3"

/** 
 * \brief The fence 4
 */
#define ATRSHMLOG_FENCE_4_SUFFIX "_FENCE_4"


/** 
 * \brief The fence 5
 */
#define ATRSHMLOG_FENCE_5_SUFFIX "_FENCE_5"


/** 
 * \brief The fence 6
 */
#define ATRSHMLOG_FENCE_6_SUFFIX "_FENCE_6"


/** 
 * \brief The fence 7
 */
#define ATRSHMLOG_FENCE_7_SUFFIX "_FENCE_7"


/** 
 * \brief The fence 8
 */
#define ATRSHMLOG_FENCE_8_SUFFIX "_FENCE_8"


/** 
 * \brief The fence 9
 */
#define ATRSHMLOG_FENCE_9_SUFFIX "_FENCE_9"

/** 
 * \brief The fence 10
 */
#define ATRSHMLOG_FENCE_10_SUFFIX "_FENCE_10"

/** 
 * \brief The fence 11
 */
#define ATRSHMLOG_FENCE_11_SUFFIX "_FENCE_11"


/**
 * \brief The fence 12
 */
#define ATRSHMLOG_FENCE_12_SUFFIX "_FENCE_12"

/**
 * \brief The fence 13
 */
#define ATRSHMLOG_FENCE_13_SUFFIX "_FENCE_13"


/**
 * \brief The dispose flag for reuse
 */
#define ATRSHMLOG_DISPATCH_REUSE (-42)

/**
 * \brief The buffer size for the prefix
 */
#define ATRSHMLOG_PREFIX_SIZE (256)

/**
 * \brief If we do a putenv in attach 
 *
 * This is 0 by default. so putenv is empty shell
 * 
 * In perl port it shows problem to make a putenv.
 */
#define ATRSHMLOG_PUTENV_IN_ATTACH 0

/** 
 * The last define is for testing : 0 off, 1 on
 */
#if ATRSHMLOG_LEVEL == 0
#define ATRSHMLOGDEBUG 0
#endif

#if ATRSHMLOG_LEVEL > 0

/* we can set it to 1 now ... */
# ifndef ATRSHMLOGDEBUG
# define ATRSHMLOGDEBUG 0
# endif

#endif


/*********************************************************************/
/**
 * \brief The pointer to the used shared memory area. 
 *
 * We can use one region for logging and one only.
 * If this is null / (void*)0  we have no log.
 * This is used in macro check.
 * So its accessible from everywhere.
 * Please dont misuse it.
 * You can corrupt the system by changing it by yourself.
 */
extern volatile const void* atrshmlog_base_ptr;

/*********************************************************************/

/** 
 * \brief The states of a buffer in shared memory
 */
enum atrshmlog_buffer_states {
  /**
   * The buffer is not initialized
   */
  atrshmlog_uninit = 0,

  /**
   * The buffer is initialized and not full
   */
  atrshmlog_writeable = 1 ,

  /**
   * The buffer is halffull - its in processing...
   */
  atrshmlog_write_halffull = 2,

  /** 
   * The buffer is full and waits for processiong
   */
  atrshmlog_full = 4,

  /**
   * We have the illegal state, only system function acesses
   */
  atrshmlog_illegal = 8
};

/** 
 * \brief The shm log buffer descriptor for one buffer
 */
struct atrshmlog_buffer_s {
  /**
   * The state stored as an atomic
   * we synchronize at this atomic the memory model things
   */
  atomic_int state;

  /**
   * The size of the data in the buffer.
   */
  int shmsize; 


  /**
   * Index of the next buffer on avilable list
   * Index -1 is end of list
   */
  int next_append;
  
  /**
   * Index of the full list.
   * Index -1 is end of list
   */
  int next_full;
  
  /**
   * The pid that write this buffer in the process
   */
  atrshmlog_pid_t pid;

  /**
   * The thread id that wrote this buffer in the process
   */
  atrshmlog_tid_t tid;

  /**
   * The initial time of the process that wrote the buffer
   */
  atrshmlog_internal_time_t inittime;

  /**
   * The click before the real time
   */
  atrshmlog_time_t inittimetsc_before;

  /**
   * The click after the realtime
   */
  atrshmlog_time_t inittimetsc_after;

  /**
   * The real time before the buffer was transfered to the shm
   */
  atrshmlog_internal_time_t lasttime;

  /** 
   * The click before the transfer
   */
  atrshmlog_time_t lasttimetsc_before;

  /**
   * The click after the transfer
   */
  atrshmlog_time_t lasttimetsc_after;

  /**
   * The click when the transfer to shm startet.
   * This is used with the before click of gettime to calc
   * the transfertime from process to shm
   */
  atrshmlog_time_t starttransfer;

  /**
   * The diffrence of click for the acquire of the buffer from the
   * thread that wrote the data to the buffer.
   */
  atrshmlog_time_t acquiretime;

  /**
   * The id of the buffer
   */
  atrshmlog_int32_t id;

  /**
   * Helper in debugging.
   */
  int chksum;

  /**
   * Check pattern to detect a memory overwrite 
   */
  long safeguard;

  /**
   * The number of dispatched buffers. 
   *
   * Start with 0.
   *
   * We need this to decide about the order of buffers final written 
   * and so about the one that holds the valid last thread statistics
   */
  int number_dispatched;
  
  /**
   * thread specific statistics are here.
   * so we can colect them later from the buffers in files.
   *
   * It was too bad for performance to have them in atomics global.
   * Too much interaction.
   * so I have them now local
   * We only set them in the beginning, never reset them.
   */

  //atrshmlog_counter_write0                    = 12,
  int counter_write0;
  /**< The number of calls to \ref atrshmlog_write0() */
  
  // atrshmlog_counter_write0_discard            = 17,
  int counter_write0_discard;
  /**< When \ref atrshmlog_write0() exits because strategy discard */

  // atrshmlog_counter_write0_wait               = 18,
  int counter_write0_wait;
  /**< When \ref atrshmlog_write0() waits because strategy  */

  // atrshmlog_counter_write0_adaptive           = 19,
  int counter_write0_adaptive;
  /**< When \ref atrshmlog_write0() waits because strategy  */

  // atrshmlog_counter_write0_adaptive_fast      = 20,
  int counter_write0_adaptive_fast;
  /**< When \ref atrshmlog_write0() waits because strategy  */

  // atrshmlog_counter_write0_adaptive_very_fast = 21,
  int counter_write0_adaptive_very_fast;
  /**< When \ref atrshmlog_write0() waits because strategy  */


  // atrshmlog_counter_write1                    = 24,
  int counter_write1;
  /**< The number of calls to \ref atrshmlog_write1() */

  //atrshmlog_counter_write1_discard            = 29,
  int counter_write1_discard;
  /**< When \ref atrshmlog_write1() exits because strategy discard */

  // atrshmlog_counter_write1_wait               = 30,
  int counter_write1_wait;
  /**< When \ref atrshmlog_write1() waits because strategy */

  // atrshmlog_counter_write1_adaptive           = 31,
  int counter_write1_adaptive;
  /**< When \ref atrshmlog_write1() waits because strategy */

  //  atrshmlog_counter_write1_adaptive_fast      = 32,
  int counter_write1_adaptive_fast;
  /**< When \ref atrshmlog_write1() waits because strategy */

  // atrshmlog_counter_write1_adaptive_very_fast = 33,
  int counter_write1_adaptive_very_fast;
  /**< When \ref atrshmlog_write1() waits because strategy */

  // atrshmlog_counter_write2                    = 37,
  int counter_write2;
  /**< The number of calls to \ref atrshmlog_write2() */

  // atrshmlog_counter_write2_discard            = 42,
  int counter_write2_discard;
  /**< When \ref atrshmlog_write2() exits because strategy discard */

  // atrshmlog_counter_write2_wait               = 43,
  int counter_write2_wait;
  /**< When \ref atrshmlog_write2() waits because strategy */

  // atrshmlog_counter_write2_adaptive           = 44,
  int counter_write2_adaptive;
  /**< When \ref atrshmlog_write2() waits because strategy */

  // atrshmlog_counter_write2_adaptive_fast      = 45,
  int counter_write2_adaptive_fast;
  /**< When \ref atrshmlog_write2() waits because strategy */

  // atrshmlog_counter_write2_adaptive_very_fast = 46,
  int counter_write2_adaptive_very_fast;
  /**< When \ref atrshmlog_write2() waits because strategy */

  /**
   * Index of info relative to the start of the shared memory area.
   * We cannot use a pointer - its meaningless in shared memory.
   * So we use that index. We add this to the start and get the first byte
   * for the buffer data.
   */
   int info;

};

/** The typedef for the struct */ 
typedef struct atrshmlog_buffer_s atrshmlog_buffer_t;

/** 
 * \brief The shm buffer descriptor.
 *
 * We have the whole log area.
 * The shmid as a init is done flag.
 * The count of buffers.
 * And the array of buffers as a stub. Minimum is two.
 * 
 * \li ro read only, initialized once.
 * \li rw read and write
 */
struct atrshmlog_area_s {
  /** 
   * The version of the buffer system ro 
   */
  volatile int shmversion; 

  /** 
   * The reflexion. myself ro for users 
   */
  volatile int shmid; 

  /** 
   * The check pattern, ro for user 
   */
  volatile long shmsafeguard; 

  /**
   * The anchor of the available list
   * \li Index -1 is empty, 
   * \li else index of first buffer on list
   */
  atomic_int shma;
  
  /**
   * The anchor of the full list.
   * \li Index -1 is empty,
   * \li else is index of first buffer on list
   */
  atomic_int shmf;

  /** The buffer count is ... ro for users   */
  volatile int shmcount; 

  /** Flag to stop logging in the system, 
   * \li 0 run, 
   * \li <>0 stop 
   *
   * rw
   */
  atomic_int ich_habe_fertig;

  /** 
   * The info for the reader rw 
   */
  volatile int readerflag; 

  /** 
   * The pid of the reader 
   */
  volatile atrshmlog_pid_t readerpid; 

  /** 
   * The info for the writer rw 
   */
  volatile int writerflag ;

  /** 
   * The pid of the writer 
   */
  volatile atrshmlog_pid_t writerpid;

  /** 
   * The array of the descriptors for the buffers 
   */ 
  atrshmlog_buffer_t logbuffers[ ATRSHMLOGBUFFER_MINCOUNT ];

  /* old style c trick. use the buffer start, but it can be more than the 
   * number in the def.
   * to work : means there is nothing after the buffers in the struct
   */
};

/** The typedef for the struct */ 
typedef struct atrshmlog_area_s atrshmlog_area_t;

/**
 * \n Main code:
 *
 * \brief The internal logging buffer descriptor struct.
 * 
 * We have three lists in our client program.
 *
 * First is the list of buffers used for logging.
 * This list is called the s list (sequence ... ).
 * It is used by the cleanup function to make a proper
 * cleanup at the end of program via an atexit
 * hook function.
 *
 * So the buffers on the s list are testet then.
 * If there is content the buffer is moved to the shm 
 * and then the buffer is cleared.
 * To handle this simple we assume no threads
 * are logging after the init of the cleanup function.
 *
 * Second is the f list (full ... ).
 *
 * When the client is logging the buffer is testet.
 *
 * If it can handle the log payload it is simply used.
 * 
 * If this is no longer the case  - it can not handle the payload - 
 * the buffer is considerd
 * to be full.
 *
 * The logging call then moves the buffer on the f list.
 *
 * The f list itself is then used by the slaves threads to grab
 * a buffer and to move the content to shm buffer.
 *
 * So the f list contains the full buffers.
 *
 * After the content has moved to a shm buffer
 * the buffer is declared to be empty.
 *
 * After a check for dispose it is given back to the user thread.
 * Or in case a dispose 
 * is set moved to the a list.
 *
 * The third list is the a list (available ...) .
 *
 * A buffer is first put on the a list, then a client thread 
 * gets the buffer.
 *
 * So if a buffer is no longer used by a thread this can set a 
 * dispose flag to non zero and the buffer is after the next transfer 
 * to shm put back on the a list ( short: it is recycled ).
 *
 * If you check the first buffers are put on the a list by definition
 * of their append pointers directly. 
 * No dyn memory or so. Simple
 * static buffers. 
 *
 * The problem starts with becoming big.
 *
 * After you have consumed all static buffers you have to use dyn memory
 * so then the dyn allocated memory is linked and initialized and 
 * then put on the a list.
 *
 * In the end all buffers are on the a list till a thread uses a buffer -
 * for this it is pulled of the a list.
 *
 * Beside the three lists the buffer contains some flags 
 * and of course some basic info that is the same for the log in the
 * buffer - namely the thread id, the pid and size and max size.
 *
 * There are also some timing infos in the buffer.
 *
 * One is for the interval that the pull needs for the buffer 
 * from the a list.
 * 
 * We use this internal descriptor and pack them tight.
 *
 * This speeds up initialization of this lists.
 * The real memory buffer is then allocated elsewhere ..
 * 
 */
struct atrshmlog_tbuff_s
{
  /** 
   * The next in line for cleanup processing or 0.
   * This is only once setted static or in the alloc. 
   */
  struct atrshmlog_tbuff_s* next_cleanup;

  /** 
   * The next in line for full processing or 0.
   */
  struct atrshmlog_tbuff_s *next_full;

  /** 
   * The next in line for allocate or 0. 
   */
  struct atrshmlog_tbuff_s* next_append;

  /**
   * A long embedded to signal abuse by memcopy et al.
   */
  long safeguardfront; 

  /**
   * The thread pid in init of the thread.
   *
   * This is used in transfering the pid to the shm (hm, what a wast ...).
   *
   * And to discover if we try in cleanup to write a buffer that was used
   * in a fork clone before in the father process ( ups. this is a real problem,
   * so we cannot ignore it - we NEED to know if the buffer was allocated
   * in the father - then it will be written in it too - or if 
   * it is really a buffer for this process. So simply spoken we NEED this here.
   * See for an example the use case in logging a ksh process ). 
   */
  atrshmlog_pid_t pid;
  
  /**
   * The thread id according to our mt system.
   */
  atrshmlog_tid_t tid;

  /**
   * The difference in clicks of the acquire intervall.
   *
   * This is a small number for a simple pop of the a list.
   *
   * But when dyn memory comes in place it gives us a 
   * chance to calculate the dyn memory overhead and so the
   * settings for the pre alloc and if needed the settings for static 
   * buffers after using the recompile switch ...
   */
  atrshmlog_time_t acquiretime;

  /**
   * The id of the buffer - only once setted.
   */
  atrshmlog_int32_t id;

  /**
   * The content size.
   *
   * This is used to decide if the buffer is empty  - is 0 - or not.
   */
  size_t size;

  /**
   * The max size possible for this buffer.
   *
   * If we use dyn alloc memory this can be smaller as the max default.
   * So we need this in tests.
   */
  size_t maxsize;
  
  /**
   * The flag for dispose. 
   *
   * \li 0 normal processing,
   * \li <> 0 we are free for reuse, put this buffer after the dispatch
   * on the a list.
   */
  int dispose;

  /**
   * The flag to handle mt use of the buffer with atomic synchronisations.
   *
   * This has to be an atomic for making the memory model happy to have a
   * release/consume/acquire point in time for the buffer.
   */
  atomic_int dispatched;

  /**
   * The number of dispatched buffers. 
   *
   * Start with 0.
   *
   * We need this to decide about the order of buffers final written 
   * and so about the one that holds the valid last thread statistics
   */
  int number_dispatched;
  
  /**
   * thread specific statistics are here.
   * so we can colect them later from the buffers in files.
   *
   * It was too bad for performance to have them in atomics global.
   * Too much interaction.
   * so I have them now local
   * We only set them in the beginning, never reset them.
   */

  //atrshmlog_counter_write0                    = 12,
  int counter_write0;
  /**< The number of calls to \ref atrshmlog_write0() */
  
  // atrshmlog_counter_write0_discard            = 17,
  int counter_write0_discard;
  /**< When \ref atrshmlog_write0() exits because strategy discard */

  // atrshmlog_counter_write0_wait               = 18,
  int counter_write0_wait;
  /**< When \ref atrshmlog_write0() waits because strategy  */

  // atrshmlog_counter_write0_adaptive           = 19,
  int counter_write0_adaptive;
  /**< When \ref atrshmlog_write0() waits because strategy  */

  // atrshmlog_counter_write0_adaptive_fast      = 20,
  int counter_write0_adaptive_fast;
  /**< When \ref atrshmlog_write0() waits because strategy  */

  // atrshmlog_counter_write0_adaptive_very_fast = 21,
  int counter_write0_adaptive_very_fast;
  /**< When \ref atrshmlog_write0() waits because strategy  */


  // atrshmlog_counter_write1                    = 24,
  int counter_write1;
  /**< The number of calls to \ref atrshmlog_write1() */

  //atrshmlog_counter_write1_discard            = 29,
  int counter_write1_discard;
  /**< When \ref atrshmlog_write1() exits because strategy discard */

  // atrshmlog_counter_write1_wait               = 30,
  int counter_write1_wait;
  /**< When \ref atrshmlog_write1() waits because strategy */

  // atrshmlog_counter_write1_adaptive           = 31,
  int counter_write1_adaptive;
  /**< When \ref atrshmlog_write1() waits because strategy */

  //  atrshmlog_counter_write1_adaptive_fast      = 32,
  int counter_write1_adaptive_fast;
  /**< When \ref atrshmlog_write1() waits because strategy */

  // atrshmlog_counter_write1_adaptive_very_fast = 33,
  int counter_write1_adaptive_very_fast;
  /**< When \ref atrshmlog_write1() waits because strategy */

  // atrshmlog_counter_write2                    = 37,
  int counter_write2;
  /**< The number of calls to \ref atrshmlog_write2() */

  // atrshmlog_counter_write2_discard            = 42,
  int counter_write2_discard;
  /**< When \ref atrshmlog_write2() exits because strategy discard */

  // atrshmlog_counter_write2_wait               = 43,
  int counter_write2_wait;
  /**< When \ref atrshmlog_write2() waits because strategy */

  // atrshmlog_counter_write2_adaptive           = 44,
  int counter_write2_adaptive;
  /**< When \ref atrshmlog_write2() waits because strategy */

  // atrshmlog_counter_write2_adaptive_fast      = 45,
  int counter_write2_adaptive_fast;
  /**< When \ref atrshmlog_write2() waits because strategy */

  // atrshmlog_counter_write2_adaptive_very_fast = 46,
  int counter_write2_adaptive_very_fast;
  /**< When \ref atrshmlog_write2() waits because strategy */

  /** 
   * The memory for the payload starts there.
   */
  char *b;
};

/** We define the type of the struct here */
typedef struct atrshmlog_tbuff_s atrshmlog_tbuff_t;

/*******************************************************************/


/** 
 * thread locals
 */

/**
 * \n Main code:
 *
 * \brief We use a struct for all thread locals.
 *
 * So we have a lookup for the struct variable, and then
 * switch over to simple pointer use.
 * 
 * This is a speed up to overcome the hash map that is 
 * normally used to give the thread locals adress.
 */
struct atrshmlog_g_tl_s {

  /**
   * This is normally not used by simple threads, only by the slaves.
   * So slaves can be iterated and killed by an owner process.
   * Simply switch the idnotok to 1.
   */
  struct atrshmlog_g_tl_s* next;

  /** 
   * This is used as an initialized flag and a not ok flag .
   * For the initialize it must be -1. 
   * After the initialize it is 0 for ok and not -1 and not 0 for an error.
   * As a side effect we stop logging for a thread by setting it 
   * to a non 0 value and free the buffers. 
   * See the atrshmlog_stop for this.
   */
  volatile int atrshmlog_idnotok;
  
  /** 
   * The used buffers for a thread.
   * We use two buffers concurrently for now.
   * Later we will switch to a n buffer design if the tests are happy with it.
   * If you think its ok you can redefine the constant.
   */
  atrshmlog_tbuff_t* atrshmlog_targetbuffer_arr[ATRSHMLOGTARGETBUFFERMAX];
  
  /**
   * The actual buffer.
   * This is the actually used buffer.
   */
  int atrshmlog_targetbuffer_index;
  
  /**
   * This is the shm count for buffers to be used as mailbox buffers.
   * This is info from the shm area itself.
   */
  int atrshmlog_shm_count;

  /**
   * The strategy for this thread.
   */
  int strategy;
  
  /** 
   * This is the thread pid on init.
   *
   * We use this also to determine a fork clone constellation.
   *
   * We are allways the real thing - we are set in the local thread
   * variable init function - and so a check for the global pid
   * is the same as long as we are in the father of the fork clone 
   * szenarion. If we are the child we will be init in case of a new thread run.
   * So we are then different to the globel pid - which was set in the father .
   *
   * Ok. We then reinit the slaves and then set the global pid to
   * our pid - we are now the master in the child and do this hopefully
   * only with one thread. The others then see that we are in a valid father.
   * So they dont start slaves, and we are happy till we fork clone ourself.
   *
   * This is a cheap trick, so i assume there is at least one trick to turn
   * this upside down, but for now its our way to test for the fork clone 
   * szenario.
   */
  atrshmlog_pid_t atrshmlog_thread_pid;
  
  /**
   * This is the thread tid.
   */
  atrshmlog_tid_t atrshmlog_thread_tid;

  /**
   * The number of dispatched buffers. 
   *
   * Start with 0.
   *
   * We need this to decide about the order of buffers final written 
   * and so about the one that holds the valid last thread statistics
   */
  int number_dispatched;
  
  /**
   * thread specific statistics are here.
   * so we can colect them later from the buffers in files.
   *
   * It was too bad for performance to have them in atomics global.
   * Too much interaction.
   * so I have them now local
   * We only set them in the beginning, never reset them.
   */

  //atrshmlog_counter_write0                    = 12,
  int counter_write0;
  /**< The number of calls to \ref atrshmlog_write0() */
  
  // atrshmlog_counter_write0_discard            = 17,
  int counter_write0_discard;
  /**< When \ref atrshmlog_write0() exits because strategy discard */

  // atrshmlog_counter_write0_wait               = 18,
  int counter_write0_wait;
  /**< When \ref atrshmlog_write0() waits because strategy  */

  // atrshmlog_counter_write0_adaptive           = 19,
  int counter_write0_adaptive;
  /**< When \ref atrshmlog_write0() waits because strategy  */

  // atrshmlog_counter_write0_adaptive_fast      = 20,
  int counter_write0_adaptive_fast;
  /**< When \ref atrshmlog_write0() waits because strategy  */

  // atrshmlog_counter_write0_adaptive_very_fast = 21,
  int counter_write0_adaptive_very_fast;
  /**< When \ref atrshmlog_write0() waits because strategy  */


  // atrshmlog_counter_write1                    = 24,
  int counter_write1;
  /**< The number of calls to \ref atrshmlog_write1() */

  //atrshmlog_counter_write1_discard            = 29,
  int counter_write1_discard;
  /**< When \ref atrshmlog_write1() exits because strategy discard */

  // atrshmlog_counter_write1_wait               = 30,
  int counter_write1_wait;
  /**< When \ref atrshmlog_write1() waits because strategy */

  // atrshmlog_counter_write1_adaptive           = 31,
  int counter_write1_adaptive;
  /**< When \ref atrshmlog_write1() waits because strategy */

  //  atrshmlog_counter_write1_adaptive_fast      = 32,
  int counter_write1_adaptive_fast;
  /**< When \ref atrshmlog_write1() waits because strategy */

  // atrshmlog_counter_write1_adaptive_very_fast = 33,
  int counter_write1_adaptive_very_fast;
  /**< When \ref atrshmlog_write1() waits because strategy */

  // atrshmlog_counter_write2                    = 37,
  int counter_write2;
  /**< The number of calls to \ref atrshmlog_write2() */

  // atrshmlog_counter_write2_discard            = 42,
  int counter_write2_discard;
  /**< When \ref atrshmlog_write2() exits because strategy discard */

  // atrshmlog_counter_write2_wait               = 43,
  int counter_write2_wait;
  /**< When \ref atrshmlog_write2() waits because strategy */

  // atrshmlog_counter_write2_adaptive           = 44,
  int counter_write2_adaptive;
  /**< When \ref atrshmlog_write2() waits because strategy */

  // atrshmlog_counter_write2_adaptive_fast      = 45,
  int counter_write2_adaptive_fast;
  /**< When \ref atrshmlog_write2() waits because strategy */

  // atrshmlog_counter_write2_adaptive_very_fast = 46,
  int counter_write2_adaptive_very_fast;
  /**< When \ref atrshmlog_write2() waits because strategy */
};

/** the type for the thread locals struct */
typedef struct atrshmlog_g_tl_s atrshmlog_g_tl_t;


/* 
 * the following is a pure result of the breaking of the module into
 * separate implementation files.
 * so we need for the files external linkable variables.
 * the following were all static before.
 * so now we have them here as declarations.
 * DONT USE THEM.
 */
extern volatile int atrshmlog_id;
extern volatile atrshmlog_pid_t atrshmlog_attach_pid;
extern volatile int atrshmlog_acquire_count;
extern volatile int atrshmlog_buffer_id;
extern char atrshmlog_buffers_prealloced_chunks[];
extern atrshmlog_tbuff_t atrshmlog_buffers_prealloced[];
extern int atrshmlog_init_buffers_in_advance;
extern atomic_intptr_t atrshmlog_tps;
extern atomic_intptr_t atrshmlog_tpa;
extern atomic_intptr_t atrshmlog_tpf;
extern atomic_intptr_t atrshmlog_tpslave;
extern int atrshmlog_buffer_infosize;
extern volatile atrshmlog_tid_t atrshmlog_f_list_buffer_slave;
extern int atrshmlog_prealloc_buffer_count;
extern volatile int atrshmlog_f_list_buffer_slave_run;
extern int atrshmlog_f_list_buffer_slave_count;
extern atomic_int atrshmlog_f_list_active_slaves;
extern int atrshmlog_f_list_buffer_slave_wait;
extern int atrshmlog_slave_to_shm_wait;
extern int atrshmlog_strategy_wait_wait_time;
extern int atrshmlog_wait_for_slaves;
extern int atrshmlog_buffer_strategy;
extern int atrshmlog_thread_fence_1;
extern int atrshmlog_thread_fence_2;
extern int atrshmlog_thread_fence_3;
extern int atrshmlog_thread_fence_4;
extern int atrshmlog_thread_fence_5;
extern int atrshmlog_thread_fence_6;
extern int atrshmlog_thread_fence_7;
extern int atrshmlog_thread_fence_8;
extern int atrshmlog_thread_fence_9;
extern int atrshmlog_thread_fence_10;
extern int atrshmlog_thread_fence_11;
extern int atrshmlog_thread_fence_12;
extern int atrshmlog_thread_fence_13;
extern atrshmlog_time_t atrshmlog_transfer_to_shm_time;
extern int atrshmlog_logging_process_off_final;
extern atrshmlog_internal_time_t atrshmlog_inittime;
extern atrshmlog_time_t atrshmlog_inittimetsc_before;
extern atrshmlog_time_t atrshmlog_inittimetsc_after;
extern int atrshmlog_delimiter;
extern char atrshmlog_prefix_name_buffer[];
extern int atrshmlog_event_locks_max;
extern atrshmlog_event_t atrshmlog_event_locks_buffer [];
extern atomic_int atrshmlog_counter[];
extern const char *atrshmlog_mapped_files[];
extern volatile int atrshmlog_attach_once;
extern void atrshmlog_exit_cleanup(void);
extern atrshmlog_tbuff_t* atrshmlog_il_get_raw_buffers(const int i_buffer_count,
					    const int i_buffer_size);
extern int atrshmlog_il_connect_buffers_list(atrshmlog_tbuff_t* const raw,
					     const int i_buffer_count,
					     char* i_chunkbuffer,
					     const int i_buffer_size);
extern void atrshmlog_free(atrshmlog_tbuff_t* restrict t);
extern atrshmlog_tbuff_t* atrshmlog_alloc(void);
extern void atrshmlog_dispatch_buffer(atrshmlog_tbuff_t* restrict i_atrshmlog_targetbuffer);
extern atrshmlog_tbuff_t *atrshmlog_acquire_buffer(const atrshmlog_g_tl_t* restrict i_g);
extern int atrshmlog_init_thread_local (atrshmlog_g_tl_t* restrict i_g);
extern void atrshmlog_init_events(const int i_use_file);
extern int atrshmlog_transfer_mem_to_shm(const atrshmlog_tbuff_t* restrict i_mem,
					 const atrshmlog_g_tl_t* restrict i_g)  ;
extern void atrshmlog_init_via_file(const char *i_suffix,
				    int *v,
				    int i_min,
				    int i_max)  ;
extern void atrshmlog_init_via_env(const char *i_suffix,
				   int *v,
				   int i_min,
				   int i_max)  ;

extern atrshmlog_thread_ret_t atrshmlog_f_list_buffer_slave_proc(void* i_arg)  ;
extern int atrshmlog_init_in_write(atrshmlog_g_tl_t* g);
extern atrshmlog_ret_t atrshmlog_set_thread_fence(int *v,
						  const atrshmlog_int32_t i_switch)  ;
extern int atrshmlog_create_mapped_file(int index, int size, int *already);
extern void * atrshmlog_attach_mapped_file(int index, int size);
extern void atrshmlog_memset_prealloced(void);
extern atomic_int atrshmlog_last_mem_to_shm;

/**
 * \brief We add to the statistics counter
 *
 * \param __e
 * The event we add to. 
 */
#define ATRSHMLOGSTAT(__e) \
  (++atrshmlog_counter[(__e)])


/**
 * \brief We add to the thread local statistics counter
 *
 *\param __tl
 * the thread local pointer
 *
 * \param __e
 * The event we add to. 
 */
#define ATRSHMLOGSTATLOCAL(__tl,__e)			\
  (++(__tl->__e))


/** 
 * \brief The memory check pattern
 */
#define ATRSHMLOGSAFEGUARDVALUE (0xFE01FE01L)


/************************************************************************/
/* helper macros*/

#if ATRSHMLOG_LEVEL > 0

/**
 * \brief Create the shm area. 
 *
 * No initialize is done, only the create. 
 *
 * \param __k
 * The shmid of the system.
 *
 * \param __c
 * The number of buffers for this area.
 *
 * \return
 * - Zero for success
 * - negative for error
 */
#define ATRSHMLOG_CREATE(__k,__c) atrshmlog_create((__k),(__c))

/**
 * \brief We delete the shm area
 *
 * \param __i
 * The shm id of the system
 */
#define ATRSHMLOG_DELETE(__i) atrshmlog_delete((__i))

/**
 * \brief We cleanup any resources in the buffer.
 *
 * \param __a
 * Points to the start of the area.
 */
#define ATRSHMLOG_CLEANUP_LOCKS(__a)  atrshmlog_cleanup_locks((__a))

/**
 * \brief We init the area
 *
 * \param __a
 * Points to the start of the area.
 *
 * \param __c
 * The number of buffers for the shm are.
 */
#define ATRSHMLOG_INIT_SHM_LOG(__a,__c) atrshmlog_init_shm_log((__a),(__c))

/************************************************************************/

/* the functions */

/** 
 * \brief We create the shm  area.
 *
 * \param i_key
 * The shmid of the system.
 *
 * \param i_count
 * The number of buffers for this area.
 *
 * \return
 * - Positive number for the shmid in case of success
 * - negative for error
 */
extern int atrshmlog_create(const atrshmlog_key_t i_key,
			    const int i_count);

/** 
 * \brief We destroy the shm 
 *
 * \param i_shmid
 * The shm id of the system
 *
 * \return
 * The error code of the delete
 */
extern atrshmlog_ret_t atrshmlog_delete(const int i_shmid);

/** 
 * \brief We have to clean up the locks after error condition 
 *
 * \param i_area
 * Points to the start of the area.
 *
 * \return
 * void
 */
extern void atrshmlog_cleanup_locks(volatile const void* i_area);

/** 
 * \brief We initialize the buffer after it is attached 
 *
 * \param i_area
 * Points to the start of the area.
 *
 * \param i_count_buffers
 * The number of buffers for the shm are.
 */
extern atrshmlog_ret_t atrshmlog_init_shm_log(volatile const void *i_area,
					      const atrshmlog_int32_t i_count_buffers);

#endif

#endif
/* end of file */
