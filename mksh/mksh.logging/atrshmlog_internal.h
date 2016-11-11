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
#define ATRSHMLOGMINORVERSION (0)

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

/**************************************************************/

/* os specific defines */

#if ATRSHMLOG_PLATFORM_LINUX_X86_64_GNU == 1

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

#endif


/**************************************************************/

#if ATRSHMLOG_USE_PTHREAD == 1
// pthread needed for thread starting and pthread id 

#include <pthread.h>

#endif

#if ATRSHMLOG_USE_C11_THREAD == 1

#include <threads.h>

#endif

#if ATRSHMLOG_SYSCALL == 1

// we use syscall interface - gettid call
#include <sys/syscall.h>

#endif

/* os specifics */

#if ATRSHMLOG_PLATFORM_LINUX_X86_64_GNU == 1

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


/*
 * platform dependent stuff here 
 */

#if ATRSHMLOG_USE_PTHREAD_TID == 1
#define ATRSHMLOG_GETTHREADID (pthread_self())
#endif

#if ATRSHMLOG_USE_SYSCALL_TID == 1
#define ATRSHMLOG_GETTHREADID (syscall(SYS_gettid))
#endif

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
#define ATRSHMLOGBUFFER_INFOSIZE_SUFFIX "_INFOSIZE"

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
 * \brief Swith statistics on or off
 */
#define ATRSHMLOG_STATISTICS_ON_OFF_SUFFIX "_STATISTICS_ON_OFF"


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
 * \brief The dispose flag for reuse
 */
#define ATRSHMLOG_DISPATCH_REUSE (-42)

/**
 * \brief The buffer size for the prefix
 */
#define ATRSHMLOG_PREFIX_SIZE (256)

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
 */
#define ATRSHMLOG_CLEANUP_LOCKS()  atrshmlog_cleanup_locks()

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
 * - Zero for success
 * - negative for error
 */
extern int atrshmlog_create(const key_t i_key,
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
 * \return
 * void
 */
extern void atrshmlog_cleanup_locks(void);

/** 
 * \brief We initialize the buffer after it is attached 
 *
 * \param i_area
 * Points to the start of the area.
 *
 * \param i_count_buffers
 * The number of buffers for the shm are.
 */
extern atrshmlog_ret_t atrshmlog_init_shm_log(volatile atrshmlog_area_t *i_area,
					      const atrshmlog_ret_t i_count_buffers);

#endif

#endif
/* end of file */
