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

#ifndef INCLUDED_ATRSHMLOG_H
#define INCLUDED_ATRSHMLOG_H

/** \file atrshmlog.h
 * \brief The interface for the ATRSHMLOG module.
 * 
 * The file contains some of the parameters in form of
 * defines and makes available the macro definitions and the 
 * functions of the module.
 *
 * Check the documentation for things that you can or even have to 
 * adjust before you compile it.
 *
 * Use this file to access the functions via macros. 
 */


/*********************************************************************/

// The defines set by user.

#define ATRSHMLOGENTRYSIZELIMIT (255)


// After setting those according to documentation
// you have to recompile the module.

// Some additional defines are in the atrshmlog_internal header,
// So check this header too.

/** 
 * \brief We define the level of the module here.
 *
 * - Level 0 is only a client logging module.
 *   We dont use any unnecessary header, too ...
 *   so no printf etc ...
 *   This will crash on build if you want to DEBUG ...
 * - Level 1 is more for a full user.
 *   You can still not use DEBUG here.
 *   But its all here now for the basic stuff.
 * - Level 2
 *   All is in. 
 */
#ifndef ATRSHMLOG_LEVEL
#define ATRSHMLOG_LEVEL 2
#endif

// The platforms supportet.
// Only one can be active.
// You have to know at least the hardware platform
// and optional the os
// and if you are depending on compiler stuff.

/** 
 * \brief We have a x86 64 bit, a linux and a gnu c 
 *
 * Testet.
 */
#define ATRSHMLOG_PLATFORM_LINUX_X86_64_GNU 1

/** 
 * \brief We have a x86 32 bit, a linux  
 *
 * Untestet.
 */
#define ATRSHMLOG_PLATFORM_LINUX_X86_32_GNU 0

/** 
 * \brief We have a ia64 with linux,
 *
 * Untestet.
 */
#define ATRSHMLOG_PLATFORM_LINUX_IA64_64_GNU 0

/** 
 * \brief We have a arm 32.
 *
 * Untestet.
 */
#define ATRSHMLOG_PLATFORM_ARM_32_GNU 0

/** 
 * \brief We have a arm 64 
 *
 * Untestet.
 */
#define ATRSHMLOG_PLATFORM_ARM_64_GNU 0

/** 
 * \brief We have a sparc 32.
 *
 * Untestet.
 */
#define ATRSHMLOG_PLATFORM_SPARC_32_GNU 0

/** 
 * \brief We have a sparc 64,
 *
 * Untestet.
 */
#define ATRSHMLOG_PLATFORM_SPARC_64_GNU 0

/**
 * \brief We have a power 32
 *
 * Untestet.
 */
#define ATRSHMLOG_PLATFORM_POWER_32_GNU 0

/** 
 * \brief We have a power 64
 *
 * Untestet.
 */
#define ATRSHMLOG_PLATFORM_POWER_64_GNU 0

/********************************************************/

/** 
 * \brief What is our compilers inline attribut 
 */
#define ATRSHMLOG_INLINE __inline__

/** 
 * \brief Do we use inlined click timers 
 */
#define ATRSHMLOG_INLINE_TSC_CODE 1

/** 
 * \brief Do we inline the gettime 
 */
#define ATRSHMLOG_INLINE_GETTIME 1

/** The threading model */

/** 
 * \brief Do we use c11 threads 
 */
#define ATRSHMLOG_USE_C11_THREAD 0

/** 
 * \brief Do we use the pthread lib 
 */
#define ATRSHMLOG_USE_PTHREAD 1


/** 
 * \brief Do we use the syscall interface 
 */
#define ATRSHMLOG_SYSCALL 1

/** 
 * \brief Do we use the clock_gettime here 
 */
#define ATRSHMLOG_USE_CLOCK_GETTIME 1

/**
 * \brief  Do we use gettimeofday 
 */
#define ATRSHMLOG_USE_GETTIMEOFDAY 0

/** 
 * \brief Do we use the syscall for linux to get TID 
 */
#define ATRSHMLOG_USE_SYSCALL_TID 1

/** 
 * \brief Do we use the pthread tid 
 */
#define ATRSHMLOG_USE_PTHREAD_TID 0

/** 
 * \brief We use what low level for click time.
 *
 * This will be replaced by get_clicktime eventually.
 */

// #define ATRSHMLOG_GET_TSC_CALL atrshmlog_get_tsc_par_x86_64_gnu
// #define ATRSHMLOG_GET_TSC_CALL atrshmlog_get_tsc_fence_x86_64_gnu
#define ATRSHMLOG_GET_TSC_CALL atrshmlog_get_tsc_x86_64_gnu
// #define ATRSHMLOG_GET_TSC_CALL atrshmlog_get_clicktime

/* end of the defines */ 

/**********************************************************************/

/* 
 * We need the 64 bit unsigned type here.
 * So this is the include to deliver it.
 */
#include <stdint.h>


/* end of includes */

/***************************************************************/

#ifdef __cplusplus
extern "C" { 
#endif

  /** 
   * \brief Our seconds in the real time struct.
   *
   * We use an 64 bit int, this is most likely big enough
   * on all platforms to hold the seconds of the local time call
   */
  typedef uint64_t atrshmlog_time_seconds_t;

  /** 
   * \brief Our nanoseconds in the real time struct.
   *
   * We use a 32 bit int which should be big enough on all
   * platforms to hold the nanosecond fraction 
   */
  typedef uint32_t atrshmlog_time_nanoseconds_t;
  
  /** 
   * \brief Our time info for realtime .
   *
   * We use a different time for real time events.
   * It should be compatible for system time functions in most cases.
   * We borrow from struct timespec on linux ...
   * We use a 64 bit for seconds - that is a well know thing 
   * We use a 32 bit for nanoseconds - that is ok for 0 to 999999999
   */
  struct atrshmlog_internal_time_s {
    /** The seconds */
    atrshmlog_time_seconds_t tv_sec;
    /** The nanoseconds */
    atrshmlog_time_nanoseconds_t tv_nsec;
  };

  /** 
   * \brief Our real time info.
   *
   * We use this only for an aproximation with the click times.
   */
  typedef struct atrshmlog_internal_time_s atrshmlog_internal_time_t;

  /** 
   * \brief Our click timer info.
   *
   * The time used is normally the so called tick time of the cpu clock counter.
   * This is a hardware approach and it is nearly done for all cpu on the
   * market. So we use a 64 bit number here.
   * \n Side note: There is no clock in the cpu. It is a counter. So I prefer 
   * to speak of clicks.
   * If there were a real clock it would be different for me.
   * But this is the last time I will use the term tick for it.
   *
   * See for the get \ref atrshmlog_get_clicktime()
   */
  typedef uint64_t atrshmlog_time_t;


  /** 
   * \brief Our process id number.
   *
   * Our process id is a number. We use a 64 bit integer.
   */
  typedef uint64_t atrshmlog_pid_t;

  /** 
   * \brief Our thread id number.
   *
   * Our thread id is a number. We use a 64 bit integer.
   */
  typedef uint64_t atrshmlog_tid_t;

  
  /** 
   * \brief Our basic integer type.
   *
   * The basic integer type in logging data, a 32 bit integer.
   * Using something other is ok. 
   * But you will not gain much from it.
   * A 32 bit covers a big range and can be subdivided in 
   * smaller groups easy. 
   * Using a 64 bit would be a wast of space in most uses.
   * Using a 16 bit is a bit too small when automatic
   * instumentation comes in place.
   * So I decided to use a 32 bit int here.
   * If you really need a bigger number: redefine it.
   * Adjust the macros and then adjust the readers and the converter.
   * In case it is done the DB module, too.
   * And thats it then.
   * But as long as you can do it with 32 bit simply try not
   * to make it too big.
   * Use a map to make meaning of it.
   * Then you can live long with 32 bits. 
   */
  typedef int32_t atrshmlog_int32_t; 

  /** 
   * \brief An event value.
   *
   * Its in fact a one bit number, but to make 
   * things simple we use a character.
   */
  typedef signed char atrshmlog_event_t;


  /** 
   * \brief The basic return type. 
   *
   * Normally it is a int of the system.
   * So we can deliver the return code for the system calls 
   * as it is needed in unix land.
   */
  typedef int atrshmlog_ret_t;

  /*******************************************************************/
  
  /** 
   * \brief The event locks. 
   *
   * If an event is ok to do someting its 1.
   * If the event has to be ignored its 0.
   * 
   * We need to know this to check if an event is to be used.
   * 
   * Events are strictly spoken position parameters for the 
   * system we want to log. You can think of a coordinate
   * for your sourcecode. Event x is this function and 
   * event y is that one.
   * 
   * So this could be some number type.
   *
   * Internal there is a buffer with that many flags 
   * in signed char type,
   * so the cpu dont has to do much to check it to be 
   * 0 or not....
   *
   * We use then the pointer to the buffer and the 
   * index in the buffer as check informations.
   *
   * Well, the index is given in form of an int...
   * Thats big enough for many events.
   * But for now the limit is about 10000 ...
   * See the impl for this \ref atrshmlog_get_event_locks_max()
   * 
   * We need that pointer directly, so its the minimum overhead 
   * if logging is OFF ....
   * 
   * If you try an instrumentalisation you will need more.
   * So you can redefine it here - or use the switch to dyn.
   * memory with the setter function.
   * This can be done by environment variable in the attach, too.
   *
   * Please dont try to write to the buffer until you know what you are doing.
   * Its possible to do it because there are szenarios
   * when an application should be allowed to stear the logging
   * by itself, for example by getting a parameter file in the 
   * application or a context of some db infos or or or...
   * 
   * This is of course possible by use of the helpers,
   * set event and clear event...
   *
   * But for the access from our macros we use this pointer.
   */
  extern volatile const atrshmlog_event_t*  atrshmlog_event_locks;

  
  /** 
   * \brief Our process wide log on off flag. 
   *
   * Temporary logging on off switch for process for timing and logging.
   * We can switch off all logging write calls by setting this to zero.
   * This int is exposed because we use it directly in macros.
   * So dont write directly, use the setter instead to switch it.
   * We can switch it on again. So this is a thing to be used
   * when your process does a critical thing and you now you cannot
   * use logging for the speed or making any loops.
   * See for operation the \ref atrshmlog_set_logging_process_on()
   * and \ref atrshmlog_set_logging_process_off()
   */
  extern volatile atrshmlog_int32_t atrshmlog_logging_process;


  /** 
   * \brief The clock id for the get clicktime. 
   *
   * We need to expose it 
   * for the inlining thing.
   * See for use \ref atrshmlog_set_clock_id()
   */
  extern volatile int atrshmlog_clock_id;
  

  /************************************************************/

  /**
   * \brief We define the counter index as an enum.
   * 
   * There is an array of ints that act as statistical counters.
   * 
   * You can get the counters with a helper.
   * 
   * To interpret the content you can use this enum.
   */
  enum atrshmlog_counter {
    atrshmlog_counter_start                     = 0,
    /**< We start our index in table by this */
    atrshmlog_counter_time_low                  = 0,
    /**< We deliver in the get function the actual click time low part */
    atrshmlog_counter_time_high                 = 1,
    /**< We deliver in the get function the actual click time high part */
    atrshmlog_counter_attach                    = 2,
    /**< The number of calls to \ref atrshmlog_attach() */
    atrshmlog_counter_get_raw                   = 3,
    /**< The number of calls to \ref atrshmlog_il_get_raw_buffers() */
    atrshmlog_counter_free                      = 4,
    /**< The number of calls to \ref atrshmlog_free() */
    atrshmlog_counter_alloc                     = 5,
    /**< The number of calls to \ref atrshmlog_alloc() */
    atrshmlog_counter_dispatch                  = 6,
    /**< The number of calls to \ref atrshmlog_dispatch_buffer() */
    atrshmlog_counter_mem_to_shm                = 7,
    /**< The number of calls to \ref atrshmlog_transfer_mem_to_shm() */
    atrshmlog_counter_mem_to_shm_doit           = 8,
    /**< When \ref atrshmlog_transfer_mem_to_shm() actually starts to transfer */
    atrshmlog_counter_mem_to_shm_full           = 9,
    /**< When \ref atrshmlog_transfer_mem_to_shm() runs into a full shm buffer system and has to wait */
    atrshmlog_counter_create_slave              = 10,
    /**< The number of calls to \ref atrshmlog_create_slave() */
    atrshmlog_counter_stop                      = 11,
    /**< The number of calls to \ref atrshmlog_stop() */
    atrshmlog_counter_write0                    = 12,
    /**< The number of calls to \ref atrshmlog_write0() */
    atrshmlog_counter_write0_abort1             = 13,
    /**< When \ref atrshmlog_write0() exits because of error */
    atrshmlog_counter_write0_abort2             = 14,
    /**< When \ref atrshmlog_write0() exits because of error */
    atrshmlog_counter_write0_abort3             = 15,
    /**< When \ref atrshmlog_write0() exits because of error */
    atrshmlog_counter_write0_abort4             = 16,
    /**< When \ref atrshmlog_write0() exits because of error */
    atrshmlog_counter_write0_discard            = 17,
    /**< When \ref atrshmlog_write0() exits because strategy discard */
    atrshmlog_counter_write0_wait               = 18,
    /**< When \ref atrshmlog_write0() waits because strategy  */
    atrshmlog_counter_write0_adaptive           = 19,
    /**< When \ref atrshmlog_write0() waits because strategy  */
    atrshmlog_counter_write0_adaptive_fast      = 20,
    /**< When \ref atrshmlog_write0() waits because strategy  */
    atrshmlog_counter_write0_adaptive_very_fast = 21,
    /**< When \ref atrshmlog_write0() waits because strategy  */
    atrshmlog_counter_write_safeguard           = 22,
    /**< When \ref atrshmlog_write0() exits because safeguard error */
    atrshmlog_counter_write_safeguard_shm       = 23,
    /**< When \ref atrshmlog_write0() exits because safeguard error */
    atrshmlog_counter_write1                    = 24,
    /**< The number of calls to \ref atrshmlog_free() */
    atrshmlog_counter_write1_abort1             = 25,
    /**< When \ref atrshmlog_write1() exits because of error */
    atrshmlog_counter_write1_abort2             = 26,
    /**< When \ref atrshmlog_write1() exits because of error */
    atrshmlog_counter_write1_abort3             = 27,
    /**< When \ref atrshmlog_write1() exits because of error */
    atrshmlog_counter_write1_abort4             = 28,
    /**< When \ref atrshmlog_write1() exits because of error */
    atrshmlog_counter_write1_discard            = 29,
    /**< When \ref atrshmlog_write1() exits because strategy discard */
    atrshmlog_counter_write1_wait               = 30,
    /**< When \ref atrshmlog_write1() waits because strategy */
    atrshmlog_counter_write1_adaptive           = 31,
    /**< When \ref atrshmlog_write1() waits because strategy */
    atrshmlog_counter_write1_adaptive_fast      = 32,
    /**< When \ref atrshmlog_write1() waits because strategy */
    atrshmlog_counter_write1_adaptive_very_fast = 33,
    /**< When \ref atrshmlog_write1() waits because strategy */
    atrshmlog_counter_write1_abort5             = 34,
    /**< When \ref atrshmlog_write1() exits because of error */
    atrshmlog_counter_write1_abort6             = 35,
    /**< When \ref atrshmlog_write1() exits because of error */
    atrshmlog_counter_write1_abort7             = 36,
    /**< When \ref atrshmlog_write1() exits because of error */
    atrshmlog_counter_write2                    = 37,
    /**< The number of calls to \ref atrshmlog_free() */
    atrshmlog_counter_write2_abort1             = 38,
    /**< When \ref atrshmlog_write2() exits because of error */
    atrshmlog_counter_write2_abort2             = 39,
    /**< When \ref atrshmlog_write2() exits because of error */
    atrshmlog_counter_write2_abort3             = 40,
    /**< When \ref atrshmlog_write2() exits because of error */
    atrshmlog_counter_write2_abort4             = 41,
    /**< When \ref atrshmlog_write2() exits because of error */
    atrshmlog_counter_write2_discard            = 42,
    /**< When \ref atrshmlog_write2() exits because strategy discard */
    atrshmlog_counter_write2_wait               = 43,
    /**< When \ref atrshmlog_write2() waits because strategy */
    atrshmlog_counter_write2_adaptive           = 44,
    /**< When \ref atrshmlog_write2() waits because strategy */
    atrshmlog_counter_write2_adaptive_fast      = 45,
    /**< When \ref atrshmlog_write2() waits because strategy */
    atrshmlog_counter_write2_adaptive_very_fast = 46,
    /**< When \ref atrshmlog_write2() waits because strategy */
    atrshmlog_counter_write2_abort5             = 47,
    /**< When \ref atrshmlog_write2() exits because of error */
    atrshmlog_counter_write2_abort6             = 48,
    /**< When \ref atrshmlog_write2() exits because of error */
    atrshmlog_counter_write2_abort7             = 49,
    /**< When \ref atrshmlog_write2() exits because of error */
    atrshmlog_counter_set_slave_count           = 50,
    /**< The number of calls to \ref atrshmlog_set_f_list_buffer_slave_count() */
    atrshmlog_counter_set_clock_id              = 51,
    /**< The number of calls to \ref atrshmlog_set_clock_id() */
    atrshmlog_counter_slave_off                 = 52,
    /**< The number of calls to \ref atrshmlog_set_f_list_buffer_slave_run_off() */
    atrshmlog_counter_set_event_locks           = 53,
    /**< The number of calls to \ref atrshmlog_set_event_locks_max() */
    atrshmlog_counter_set_infosize              = 54,
    /**< The number of calls to \ref atrshmlog_set_buffer_infosize() */
    atrshmlog_counter_set_wait_slaves_on        = 55,
    /**< The number of calls to \ref atrshmlog_set_wait_for_slaves_on() */
    atrshmlog_counter_set_wait_slaves_off       = 56,
    /**< The number of calls to \ref atrshmlog_set_wait_for_slaves_off() */
    atrshmlog_counter_set_slave_wait            = 57,
    /**< The number of calls to \ref atrshmlog_set_f_list_buffer_slave_wait() */
    atrshmlog_counter_set_prealloc_count        = 58,
    /**< The number of calls to \ref atrshmlog_set_prealloc_buffer_count() */
    atrshmlog_counter_set_thread_fence          = 59,
    /**< The number of calls to \ref atrshmlog_set_thread_fence() */
    atrshmlog_counter_create                    = 60,
    /**< The number of calls to \ref atrshmlog_create() */
    atrshmlog_counter_create_abort1             = 61,
    /**< The \ref atrshmlog_create() exits because of error */
    atrshmlog_counter_create_abort2             = 62,
    /**< The \ref atrshmlog_create() exits because of error */
    atrshmlog_counter_create_abort3             = 63,
    /**< The \ref atrshmlog_create() exits because of error */
    atrshmlog_counter_create_abort4             = 64,
    /**< The \ref atrshmlog_create() exits because of error */
    atrshmlog_counter_delete                    = 65,
    /**< The number of calls to \ref atrshmlog_delete() */
    atrshmlog_counter_cleanup_locks             = 66,
    /**< The number of calls to \ref atrshmlog_cleanup_locks() */
    atrshmlog_counter_init_shm                  = 67,
    /**< The number of calls to \ref atrshmlog_init_shm_log() */
    atrshmlog_counter_read                      = 68,
    /**< The number of calls to \ref atrshmlog_read() */
    atrshmlog_counter_read_doit                 = 69,
    /**< The \ref atrshmlog_read() transfers a buffer */
    atrshmlog_counter_read_fetch                = 70,
    /**< The number of calls to \ref atrshmlog_read_fetch() */
    atrshmlog_counter_read_fetch_doit           = 71,
    /**< The  \ref atrshmlog_read_fetch() transfers a buffer */
    atrshmlog_counter_verify                    = 72,
    /**< The number of calls to \ref atrshmlog_verify() */
    atrshmlog_counter_logging_process_on        = 73,
    /**< The number of calls to \ref atrshmlog_set_logging_process_on() */
    atrshmlog_counter_logging_process_off       = 74,
    /**< The number of calls to \ref atrshmlog_set_logging_process_off() */
    atrshmlog_counter_set_strategy              = 75,
    /**< The number of calls to \ref atrshmlog_set_strategy() */
    atrshmlog_counter_set_strategy_process      = 76,
    /**< The number of calls to \ref atrshmlog_set_strategy_process() */
    atrshmlog_counter_set_event                 = 77,
    /**< The number of calls to \ref atrshmlog_set_event_on() */
    atrshmlog_counter_set_envprefix             = 78,
    /**< The number of calls to \ref atrshmlog_set_event_off() */
    atrshmlog_counter_exit_cleanup              = 79,
    /**< The number of calls to \ref atrshmlog_exit_cleanup() */
    atrshmlog_counter_flush                     = 80,
    /**< The number of calls to \ref atrshmlog_flush() */
    atrshmlog_counter_logging_process_off_final = 81,
    /**< The number of calls to \ref atrshmlog_set_logging_process_off_final() */
    atrshmlog_counter_end                       = 81
    /**< The highes index in use */
  };

  /************************************************************************/

  /**
   * \brief We have the enum for return codes here.
   *
   * We use an enum to match the number returned by the function
   * to an meaningful value.
   *
   * To be specific, the 0 is OK. No error.
   * And all alues positiv are minor glitches, no errors in 
   * this sense. We have for example a 0 as input and do nothing.
   *
   * All negative values are indeed errors.
   */
  enum atrshmlog_error {
    atrshmlog_error_ok = 0,
    atrshmlog_error_connect_1 = -11,
    atrshmlog_error_connect_2 = -12,
    atrshmlog_error_init_thread_local_1 = -21,
    atrshmlog_error_mem_to_shm_1 = -31,
    atrshmlog_error_mem_to_shm_2 = -32,
    atrshmlog_error_mem_to_shm_3 = 31,
    atrshmlog_error_mem_to_shm_4 = -33,
    atrshmlog_error_mem_to_shm_5 = -34,
    atrshmlog_error_mem_to_shm_6 = -35,
    atrshmlog_error_mem_to_shm_7 = 32,
    atrshmlog_error_mem_to_shm_8 = 33,
    atrshmlog_error_attach_1 = 41,
    atrshmlog_error_attach_2 = -41,
    atrshmlog_error_attach_3 = -42,
    atrshmlog_error_attach_4 = -43,
    atrshmlog_error_attach_5 = -44,
    atrshmlog_error_attach_6 = -45,
    atrshmlog_error_init_in_write_1 = -51,
    atrshmlog_error_write0_1 = -61,
    atrshmlog_error_write0_2 = -62,
    atrshmlog_error_write0_3 = -63,
    atrshmlog_error_write0_4 = 61,
    atrshmlog_error_write0_5 = 62,
    atrshmlog_error_write0_6 = 63,
    atrshmlog_error_write0_7 = -64,
    atrshmlog_error_write0_8 = -65,
    atrshmlog_error_write0_9 = 64,
    atrshmlog_error_write1_1 = -71,
    atrshmlog_error_write1_2 = -72,
    atrshmlog_error_write1_3 = -73,
    atrshmlog_error_write1_4 = -74,
    atrshmlog_error_write1_5 = -75,
    atrshmlog_error_write1_6 = 71,
    atrshmlog_error_write1_7 = 72,
    atrshmlog_error_write1_8 = 73,
    atrshmlog_error_write1_9 = -76,
    atrshmlog_error_write1_10 = -77,
    atrshmlog_error_write1_11 = -78,
    atrshmlog_error_write1_12 = 74,
    atrshmlog_error_write2_1 = -81,
    atrshmlog_error_write2_2 = -82,
    atrshmlog_error_write2_3 = -83,
    atrshmlog_error_write2_4 = -84,
    atrshmlog_error_write2_5 = -85,
    atrshmlog_error_write2_6 = 81,
    atrshmlog_error_write2_7 = 82,
    atrshmlog_error_write2_8 = 83,
    atrshmlog_error_write2_9 = -86,
    atrshmlog_error_write2_10 = -87,
    atrshmlog_error_write2_11 = -88,
    atrshmlog_error_write2_12 = 84,
    atrshmlog_error_area_version_1 = -91,
    atrshmlog_error_area_count_1 = -101,
    atrshmlog_error_area_ich_habe_fertig_1 = -111,
    atrshmlog_error_get_event_1 = -121,
    atrshmlog_error_get_logging_1 = 131,
    atrshmlog_error_get_logging_2 = 132,
    atrshmlog_error_get_logging_3 = 133,
    atrshmlog_error_get_logging_4 = 134,
    atrshmlog_error_create_1 = -141,
    atrshmlog_error_create_2 = -142,
    atrshmlog_error_create_3 = -143,
    atrshmlog_error_create_4 = -144,
    atrshmlog_error_init_shm_1 = -151,
    atrshmlog_error_init_shm_2 = -152,
    atrshmlog_error_init_shm_3 = -153,
    atrshmlog_error_read_1 = -161,
    atrshmlog_error_read_2 = -162,
    atrshmlog_error_read_3 = -163,
    atrshmlog_error_read_4 = -164,
    atrshmlog_error_read_5 = -165,
    atrshmlog_error_read_fetch_1 = -171,
    atrshmlog_error_read_fetch_2 = -172,
    atrshmlog_error_read_fetch_3 = 171,
    atrshmlog_error_read_fetch_4 = -173,
    atrshmlog_error_read_fetch_5 = 172,
    atrshmlog_error_verify_1 = -181,
    atrshmlog_error_verify_2 = -182,
    atrshmlog_error_verify_3 = -183,
    atrshmlog_error_verify_4 = -184,
    atrshmlog_error_verify_5 = -185,
    atrshmlog_error_verify_6 = -186
    
  };
  
  /**
   * \brief We have these strategy options in waiting for the log 
   * to have a free buffer .
   *
   * So we need to hold these in the process
   * and in the thread scope.
   *
   * See the doc for info.
   */
  enum atrshmlog_strategy {
    
    /**
     * We define the first strategy
     */
    atrshmlog_strategy_first = 0,
    
    /**
     * We discard the log 
     */
    atrshmlog_strategy_discard = 0,
    
    /**
     * We spin loop till one is free
     */
    atrshmlog_strategy_spin_loop = 1,
    
    /**
     * We wait for a defined time in loop
     */
    atrshmlog_strategy_wait = 2,
    
    /**
     * We use an adaptiv approch with the slavetime
     */
    atrshmlog_strategy_adaptive = 3,

    /**
     * We use an adaptive approch with double speed.
     */
    atrshmlog_strategy_adaptive_fast = 4,
    
    /**
     * We use an adaptive approch with ten times  speed.
     */
    atrshmlog_strategy_adaptive_very_fast = 5,

    /**
     * The last defined strategy
     */
    atrshmlog_strategy_last = 5
  };


  /************************************************************************/
  /* LOGGING macros */

  /* these are the major macros you need to use in the own source code */

  /* if we ever need to make changes to prototypes, but wont break code
   * the macro will still be the same but additional functions are behind the scene
   * so we recommend the use of the macro.
   */

  /** 
   * \brief The check if an event is on and logging is on, too. 
   *
   * We use a speed up here, binary and ... 
   * This is a helper in fact, but if you want to make something dependent
   * from an event and have not time to use the 
   * function interface, use this ...
   *
   * \param __ev
   * The event number for this check. 
   *
   * \return
   * The bitwise and of the event flag and the global logging switch.
   * So it is a wise thing to set an event only to 0 or 1 and to set
   * the global logging switch only to 0 or 1 , too.
   */
#define ATRSHMLOG_EVENTCHECK(__ev)				\
  ((atrshmlog_event_locks[(__ev)]) & atrshmlog_logging_process)

  /** 
   * \brief Get the click time into the time variable.
   *
   * We use implicit an assignment, so you dont have to type all this many 
   * by yourself.
   * This is hard wired to the low level macro, so you can either
   * change here or in that macro if you need.
   * And we do the check for logging.
   * On my box a low level time get is 40 clicks, so a check for
   * lets say 2 clicks is worth the thing.
   * We can reduce register polution and cache pollution by 
   * doing this if a time is not needed.. 
   *
   * \param __ev
   * The event for this gettime.
   *
   * \param __x
   * The variable to hold the click time. 
   *
   * \return
   * The value of the time.
  */
#define ATRSHMLOG_GETTIME(__ev,__x)		\
  ((__x) = ((ATRSHMLOG_EVENTCHECK((__ev)) ?	\
	     ATRSHMLOG_GET_TSC_CALL() : 0 )))


  /** 
   * \brief Write a short log entry.
   *
   * We write to log buffer memory here.
   * The writing of a log entry, all in all covered for now.
   *
   * \param  __ev  
   *  The event number, 0 ... event count  - 1
   *
   * \param __evf 
   *  The event flag , for now 'I' for interval and 'P' for point.
   *  A plain c char for now.
   *
   *  \param __usr 
   * The user supplied event number, a 32 bit int.
   *
   *  \param __s
   * The startime. 
   *
   *  \param __e 
   * The endtime. 
   *
   * \return
   * The return code of the atrshmlog_write0 function or -99 in case
   * we have no logging active either for the event or for the
   * process.
   */
#define ATRSHMLOG_WRITE0(__ev,__evf,__usr,__s,__e) \
  ( ATRSHMLOG_EVENTCHECK((__ev)) ?			\
    (atrshmlog_write0)((__ev),(__evf),(__usr),(__s),(__e)) \
		      : -99 \
    )

  /** 
   * \brief Write a log entry with payload.
   *
   * We write to log buffer memory here.
   * The writing of a log entry, all in all covered for now.
   * With a binary payload, so you can log everything you want.
   *
   * \param  __ev  
   *  The event number, 0 ... event count  - 1
   *
   * \param __evf 
   *  The event flag , for now 'I' for interval and 'P' for point.
   *  A plain c char for now.
   *
   *  \param __usr 
   * The user supplied event number, a 32 bit int.
   *
   *  \param __s
   * The startime. 
   *
   *  \param __e 
   * The endtime. 
   *
   *  \param __b
   * The payload buffer adress or NULL.
   *
   * \param  __sz
   * The payload buffer size or 0.
   * 
   * \return
   * The return code of the atrshmlog_write1 function or -99 in case
   * we have no logging active either for the event or for the
   * process.
   */
#define ATRSHMLOG_WRITE(__ev,__evf,__usr,__s,__e,__b,__sz) \
  ( ATRSHMLOG_EVENTCHECK((__ev)) ?	\
    (atrshmlog_write1)((__ev),(__evf),(__usr),(__s),(__e),(__b),(__sz)) \
		      : -99 	\
    )

  /** 
   * \brief Write a log entry with a payload and an argv.
   *
   * Write to the memorylog.
   * The second way to log - some primitive dynamic way with a char* array...
   * Should be clear for a c programmer.
   *
   * For the rest:
   *
   * Use an array of pointers.
   * Put at last position a NULL pointer.
   * Then deliver the start adress of that array to the macro.
   * If you have the count of pointers you can deliver that too.
   * If not use a 0 or negative number. 
   * In that case the null pointer is a MUST and not a NICE ...
   *
   * \param  __ev  
   *  The event number, 0 ... event count  - 1
   *
   * \param __evf 
   *  The event flag , for now 'I' for interval and 'P' for point.
   *  A plain c char for now.
   *
   *  \param __usr 
   * The user supplied event number, a 32 bit int.
   *
   *  \param __s
   * The startime.
   *
   *  \param __e 
   * The endtime. 
   *
   *  \param __b
   * The payload buffer adress or NULL.
   *
   * \param  __sz
   * The payload buffer size or 0.
   * 
   *  \param __b2 
   * The adress of an array of pointers to char - see argv definitions.
   *
   *  \param __sz2 
   * The argc count hint - can be used to cut of argv part or 0. See
   * the definitions for an argv array.
   *
   * \return
   * The return code of the atrshmlog_write2 function or -99 in case
   * we have no logging active either for the event or for the
   * process.
   */
#define ATRSHMLOG_WRITE2(__ev,__evf,__usr,__s,__e,__b,__sz,__b2,__sz2)	\
  ( ATRSHMLOG_EVENTCHECK((__ev)) ?					\
    (atrshmlog_write2)((__ev),(__evf),(__usr),(__s),(__e),(__b),(__sz), \
		      (__b2),(__sz2)) :  -99				\
    )

  /** 
   * \brief We initialize and attach to shm.
   *
   * Attach to the shm buffer and initialize internal
   * with the function
   *
   * \return
   * The return code of the function
   */
#define ATRSHMLOG_ATTACH() atrshmlog_attach()


  /**
   * \brief We wait nanoseconds.
   * 
   * \param __n
   * The nanosecs till 2^31.
   *
   * \return
   * void
   */
#define ATRSHMLOG_NANOSLEEP(__n)   atrshmlog_nanosleep((__n))

  /**
   * \brief We switch the statistics gathering on or off
   *
   * \param __s
   * - 0 : switch off
   * - non zero : switch on
   *
   * \return
   * Old flag value
   */
#define ATRSHMLOG_SET_STATISTICS_ON(__s) atrshmlog_set_statistics_on((__s))
  
  /**
   * \brief We deliver the max index of the statistics buffer
   *
   * \return 
   * Maximum index of the statistics buffer.
   */
#define ATRSHMLOG_GET_G_COUNTERS_MAX_INDEX() atrshmlog_get_g_counters_max_index()

  /**
   * \brief We deliver copy of the statistics counter.
   *
   * Its up to you to have enough space for them.
   * So use the get max index to do the right allocation.
   *
   * \param __o
   * The start Adress of an int32 array for the counters.
   *
   * \return 
   * void
   */
#define ATRSHMLOG_GET_G_COUNTERS(__o) atrshmlog_get_g_counters((__o))
   
  
  /** 
   * \brief Set the prefix for use of variables and flagfiles.
   *
   * \param __p
   * Start of the new prefix.
   *
   * \return
   * void
   */
#define ATRSHMLOG_SET_ENVPREFIX(__p) atrshmlog_set_envprefix((__p))

  /** 
   * \brief Get the prefix for variables and flagfiles.
   *
   * \return 
   * Points to the used prefix.
   */
#define ATRSHMLOG_GET_ENVPREFIX() atrshmlog_get_envprefix()

  /** 
   * \brief Get an variable value with prefix.
   *
   * \param __p
   * Points to the suffix after the prefix for the variable name.
   *
   * \return
   * - Points to the value of the variable.
   * - Zero if no variable defined.
   */
#define ATRSHMLOG_GET_ENV(__p) atrshmlog_get_env((__p))

  /** 
   * \brief Get the shmid variable with prefix.
   *
   * \return
   * - Points to the shmid variable value
   * - Zero if no variable defined.
   */
#define ATRSHMLOG_GET_ENVSHMID() atrshmlog_get_envshmid()

#define ATRSHMLOG_GETENVIDSUFFIX()  atrshmlog_get_envidsuffix()

  /** 
   * \brief Get the major version.
   *
   * \return
   * The number of the major version of the shmlog.
   */
#define ATRSHMLOG_GET_VERSION() atrshmlog_get_version()

  /** 
   * \brief Get the minor version.
   *
   * \return
   * The number of the minor version of the shmlog.
   */
#define ATRSHMLOG_GET_MINOR_VERSION() atrshmlog_get_minor_version()

  /** 
   * \brief Get the patch version.
   *
   * \return
   * The number of the patch version of the shmlog.
   */
#define ATRSHMLOG_GET_PATCH_VERSION() atrshmlog_get_patch_version()

  /** 
   * \brief Get the max of event locks - 1.
   *
   * \return
   * The number of the highest available event + 1.
   */
#define ATRSHMLOG_GET_EVENT_LOCKS_MAX() atrshmlog_get_event_locks_max()

  /** 
   * \brief Set the max of event locks.
   *
   * \param __max
   * Number of the new maximum event plus 1.
   *
   * \return
   * Number of the old max event + 1.
   */
#define ATRSHMLOG_SET_EVENT_LOCKS_MAX(__max) atrshmlog_set_event_locks_max((__max))

  /** 
   * \brief Get an event value.
   *
   * \param __i
   * Number of the event.
   *
   * \return
   * Number of the event value.
   * Normal values is 0 fro event inactiv, 1 for event active.
   */
#define ATRSHMLOG_GET_EVENT(__i) atrshmlog_get_event((__i))

  /** 
   * \brief Set an event to 1.
   *
   * \param __i
   * Number of the event that is set to 1.
   *
   * \return
   * void
   */
#define ATRSHMLOG_SET_EVENT_ON(__i)  atrshmlog_set_event_on((__i))

  /** 
   * \brief Set an event to 0.
   *
   * \param __i
   * Number of the event that is set to 0.
   *
   * \return
   * void
   */
#define ATRSHMLOG_SET_EVENT_OFF(__i) atrshmlog_set_event_off((__i))

  /** 
   * \brief Get the logging status via function.
   *
   * \return
   * Number of the logging status for the process.
   * - 0 : ok, we are logging
   * - non zero : we dont log
   */
#define ATRSHMLOG_GET_LOGGING()  atrshmlog_get_logging()

  /** 
   * \brief Set the logging to on.
   *
   * \return
   * The old number of the logging flag.
   */
#define ATRSHMLOG_SET_LOGGING_PROCESS_ON()  atrshmlog_set_logging_process_on()

  /** 
   * \brief Set the logging to off.
   *
   * \return
   * The old number of the logging flag.
   */
#define ATRSHMLOG_SET_LOGGING_PROCESS_OFF()  atrshmlog_set_logging_process_off()

  /**
   * \brief Switch the logging for the process off finally.
   *
   * \return
   * Old flag.
   */
#define ATRSHMLOG_SET_LOGGING_PROCESS_OFF_FINAL()  atrshmlog_set_logging_process_off_final()

  
  /** 
   * \brief Get the shmid code.
   *
   * \return
   * Number of the shared memory id.
   */
#define ATRSHMLOG_GET_SHMID()  atrshmlog_get_shmid()

  /** 
   * \brief Get the shm area start adress.
   *
   * \return
   * Points to the shared memory area.
   */
#define ATRSHMLOG_AREA()  atrshmlog_area()

  /** 
   * \brief Get a shm area flag ich_habe_fertig.
   *
   * \param __a
   * Points to the start of the shared memory area.
   *
   * \return
   * The number of the flag. 
   */
#define ATRSHMLOG_GET_AREA_ICH_HABE_FERTIG(__a) atrshmlog_get_area_ich_habe_fertig((__a))

  /** 
   * \brief Set the shm area flag ich_habe_fertig.
   *
   * \param __a
   * Points to the start of the shared memory area.
   *
   * \param __f
   * The new number for the flag.
   *
   * \return
   * The old number of the flag. 
   */
#define ATRSHMLOG_SET_AREA_ICH_HABE_FERTIG(__a,__f) atrshmlog_set_area_ich_habe_fertig((__a),(__f))

  /** 
   * \brief Get the shm count of buffers.
   *
   * \param __a
   * Points to the start of the shared memory area.
   *
   * \return
   * The number of buffers in the area.
   */
#define ATRSHMLOG_GET_AREA_COUNT(__a) atrshmlog_get_area_count((__a))

  /** 
   * \brief Get the shm version.
   *
   * \param __a
   * Points to the start of the shared memory area.
   *
   * \return
   * The number of the version in the shared memory area.
   */
#define ATRSHMLOG_GET_AREA_VERSION(__a) atrshmlog_get_area_version((__a))

  /** 
   * \brief Get the maximum size of logbuffer.
   *
   * \return
   * The number of the maximum size of log buffers.
   */
#define ATRSHMLOG_GET_LOGBUFFERMAXSIZE() atrshmlog_get_logbuffermaxsize()

  /** 
   * \brief Get the actual size of logbuffer in process.
   *
   * \return
   * Number of the actual size for log buffers.
   */
#define ATRSHMLOG_GET_BUFFER_INFOSIZE() atrshmlog_get_buffer_infosize()

  /** 
   * \brief Set the actual size for log buffer in process.
   *
   * \param __size
   * The new number f bytes for log buffers.
   *
   * \return
   * The old number of bytes fro log buffers.
   */
#define ATRSHMLOG_SET_BUFFER_INFOSIZE(__size) atrshmlog_set_buffer_infosize((__size))

  /** 
   * \brief Get the number of slave threads running.
   *
   * \return
   * The number of threads running the function for slave proc.
   */
#define ATRSHMLOG_GET_F_LIST_BUFFER_SLAVE_COUNT()  atrshmlog_get_f_list_buffer_slave_count()

  /** 
   * \brief Set the number of slave threads startet.
   *
   * \param __count
   * The new count of slave threads to start.
   *
   * \return
   * The old number of slave threads to start.
   */
#define ATRSHMLOG_SET_F_LIST_BUFFER_SLAVE_COUNT(__count) atrshmlog_set_f_list_buffer_slave_count((__count))

  /** 
   * \brief Get the clock id used in get time.
   *
   * \return
   * The id of the clock to use in get clock call.
   */
#define ATRSHMLOG_GET_CLOCK_ID()  atrshmlog_get_clock_id()

  /** 
   * \brief Set the clock id in get time.
   *
   * \param __i
   * The id for the get clock call.
   *
   * \return
   * The old id for the get clock call.
   */
#define ATRSHMLOG_SET_CLOCK_ID(__i) atrshmlog_set_clock_id((__i))

  /** 
   * \brief Set the slave run flag to off.
   *
   * \return
   * void
   */
#define ATRSHMLOG_SET_F_LIST_BUFFER_SLAVE_RUN_OFF()  atrshmlog_set_f_list_buffer_slave_run_off()


  /** 
   * \brief Set the wait for slaves flag to on.
   *
   * \return
   * The old flag.
   */
#define ATRSHMLOG_SET_WAIT_FOR_SLAVES_ON()   atrshmlog_set_wait_for_slaves_on()

  /** 
   * \brief Set the wait for slaves flag to off.
   *
   * \return
   * The old flag.
   */
#define ATRSHMLOG_SET_WAIT_FOR_SLAVES_OFF()  atrshmlog_set_wait_for_slaves_off()

  /** 
   * \brief Get the wait for slaves flag.
   *
   * \return
   * The flag for the wait for slaves.
   */
#define ATRSHMLOG_GET_WAIT_FOR_SLAVES()  atrshmlog_get_wait_for_slaves()

  /** 
   * \brief Get the nanos the slave waits after going to sleep.
   *
   * \return
   * The number of nanos the slave sleeps when nothing has to be done.
   */
#define ATRSHMLOG_GET_F_LIST_BUFFER_SLAVE_WAIT() atrshmlog_get_f_list_buffer_slave_wait()

  /** 
   * \brief Set the nanos the slave waits in sleep.
   *
   * \param __nanos
   * The number of nanos the slave has to sleep when nothing has
   * to be done.
   *
   * \return
   * The old number of nanos the slave had to wait.
   */
#define ATRSHMLOG_SET_F_LIST_BUFFER_SLAVE_WAIT(__nanos) atrshmlog_set_f_list_buffer_slave_wait((__nanos))

  /** 
   * \brief Get the number of buffers for the process.
   *
   * \return
   * The number of buffers for logging in the process.
   */
#define ATRSHMLOG_GET_ACQUIRE_COUNT()  atrshmlog_get_acquire_count()

  /** 
   * \brief Get the number of buffers fetched from dyn memory in one alloc.
   *
   * \return
   * The number of buffers a dyn alloc call gets in one malloc
   * from the dyn memory area.
   */
#define ATRSHMLOG_GET_PREALLOC_BUFFER_COUNT()   atrshmlog_get_prealloc_buffer_count()

  /** 
   * \brief Set the number of buffers fetched with one alloc from dyn memory.
   *
   * \param __count
   * The new number of buffers the alloc fetches from dyn memory
   * area in one malloc.
   *
   * \return
   * The old number of buffers fetched in one malloc.
   */
#define ATRSHMLOG_SET_PREALLOC_BUFFER_COUNT(__count) atrshmlog_set_prealloc_buffer_count((__count))

  /** 
   * \brief Get the inittime.
   *
   * \return
   * The internal time struct with the inittime of the process.
   */
#define ATRSHMLOG_GET_INITTIME()  atrshmlog_get_inittime()

  /** 
   * \brief Get the click time before inittime.
   * 
   * \return
   * The time for the click before the get clock call to fill inittime.
   */
#define ATRSHMLOG_GET_INITTIMETSC_BEFORE()  atrshmlog_get_inittimetsc_before()

  /** 
   * \brief Get the click time after inittime.
   *
   * \return
   * The time for the click after the get clock call to fill inittime.
   */
#define ATRSHMLOG_GET_INITTIMETSC_AFTER()   atrshmlog_get_inittimetsc_after()

  /**
   * \brief Get the buffer id.
   *
   * \return
   * Get the id of the last buffer initialized.
   */
#define ATRSHMLOG_GET_BUFFER_ID()   atrshmlog_get_buffer_id()

  /** 
   * \brief Stop logging for this thread.
   *
   * \return
   * void
   */
#define ATRSHMLOG_STOP()  atrshmlog_stop()

  /** 
   * \brief Flush buffers for this thread.
   *
   * \return
   * void
   */
#define ATRSHMLOG_FLUSH()  atrshmlog_flush()

  /** 
   * \brief Set the strategy for this thread.
   *
   * See also \ref atrshmlog_strategy
   *
   * \param __s
   * - 0: discard
   * - 1: spin loop
   * - 2: wait
   * - 3: adaptive
   * - 4: adaptive fast
   * - 5: adaptive very fast
   *
   * \return
   * The old strategy value 
   */
#define ATRSHMLOG_SET_STRATEGY(__s)  atrshmlog_set_strategy((__s))

  /** 
   * \brief Set the strategy for the process.
   *
   * \param __s
   * - 0: discard
   * - 1: spin loop
   * - 2: wait
   * - 3: adaptive
   * - 4: adaptive fast
   * - 5: adaptive very fast
   *
   * \return
   * The old strategy value 
   */
#define ATRSHMLOG_SET_STRATEGY_PROCESS(__s)  atrshmlog_set_strategy_process((__s))

  /** 
   * \brief Start a new slave thread.
   *
   * \return
   * The return code of the used function to start the thread.
   */
#define ATRSHMLOG_CREATE_SLAVE()  atrshmlog_create_slave()
    
  /** 
   * \brief Get the real time with clicktime.
   *
   * \return
   * the 64 bit click time 
   */
#define ATRSHMLOG_GET_CLICKTIME() atrshmlog_get_clicktime()

  /** 
   * \brief Set the fence 1 flag on.
   *
   * \param __s
   * The new value for the flag.
   *
   * \return
   * The old flag for fence.
   */
#define ATRSHMLOG_SET_THREAD_FENCE_1(__s) atrshmlog_set_thread_fence_1((__s))

  /**  
   * \brief Set the fence 2 flag on.
   *
   * \param __s
   * The new value for the flag.
   *
   * \return
   * The old flag for fence.
   */
#define ATRSHMLOG_SET_THREAD_FENCE_2(__s) atrshmlog_set_thread_fence_2((__s))

  /**  
   * \brief Set the fence 3 flag on.
   *
   * \param __s
   * The new value for the flag.
   *
   * \return
   * The old flag for fence.
   */
#define ATRSHMLOG_SET_THREAD_FENCE_3(__s) atrshmlog_set_thread_fence_3((__s))

  /**  
   * \brief Set the fence 4 flag on.
   *
   * \param __s
   * The new value for the flag.
   *
   * \return
   * The old flag for fence.
   */
#define ATRSHMLOG_SET_THREAD_FENCE_4(__s) atrshmlog_set_thread_fence_4((__s))

  /**  
   * \brief Set the fence 5 flag on.
   *
   * \param __s
   * The new value for the flag.
   *
   * \return
   * The old flag for fence.
   */
#define ATRSHMLOG_SET_THREAD_FENCE_5(__s) atrshmlog_set_thread_fence_5((__s))

  /**
   * \brief Set the fence 6 flag on.
   *
   * \param __s
   * The new value for the flag.
   *
   * \return
   * The old flag for fence.
   */
#define ATRSHMLOG_SET_THREAD_FENCE_6(__s) atrshmlog_set_thread_fence_6((__s))

  /** 
   * \brief Set the fence 7 flag on.
   *
   * \param __s
   * The new value for the flag.
   *
   * \return
   * The old flag for fence.
   */
#define ATRSHMLOG_SET_THREAD_FENCE_7(__s) atrshmlog_set_thread_fence_7((__s))
  
  /** 
   * \brief Set the fence 8 flag on.
   *
   * \param __s
   * The new value for the flag.
   *
   * \return
   * The old flag for fence.
   */
#define ATRSHMLOG_SET_THREAD_FENCE_8(__s) atrshmlog_set_thread_fence_8((__s))
  
  /**
   * \brief Set the fence 9 flag on.
   *
   * \param __s
   * The new value for the flag.
   *
   * \return
   * The old flag for fence.
   */
#define ATRSHMLOG_SET_THREAD_FENCE_9(__s) atrshmlog_set_thread_fence_9((__s))
  
  /**
   * \brief Set the fence 10 flag on.
   *
   * \param __s
   * The new value for the flag.
   *
   * \return
   * The old flag for fence.
   */
#define ATRSHMLOG_SET_THREAD_FENCE_10(__s) atrshmlog_set_thread_fence_10((__s))
  
  /** 
   * \brief Set the fence 11 flag on.
   *
   * \param __s
   * The new value for the flag.
   *
   * \return
   * The old flag for fence.
   */
#define ATRSHMLOG_SET_THREAD_FENCE_11(__s) atrshmlog_set_thread_fence_11((__s))
  
  /**
   * \brief Set the fence 12 flag on.
   *
   * \param __s
   * The new value for the flag.
   *
   * \return
   * The old flag for fence.
   */
#define ATRSHMLOG_SET_THREAD_FENCE_12(__s) atrshmlog_set_thread_fence_12((__s))
  

  /**
   * \brief  We get the real time
   *
   * \return
   * The real time.
   */
#define ATRSHMLOG_GET_REALTIME() atrshmlog_get_realtime()

  /* from here on we have stuff for higher level of use */
#if ATRSHMLOG_LEVEL > 0


  /**
   * \brief We read a buffer from shm.
   *
   * Read shm buffer into normal memory and free shm buffer for next transfer.
   * this call checks the f list in shm and returns if nothing is 
   * on the list.
   * So there is no wait.
   *
   * \param __a
   * Points to an shm area
   *
   *  \param __i
   * Points to an 32 int for index of the buffer.
   *
   *  \param __t
   * Points to the start of a buffer big enough to hold the shm buffer
   * if any is present.
   *
   *  \param __tl
   * Points to an int32 that holds the length of the shm buffer 
   * after we fetched one. A 0 indicates that we didnt get one.
   *
   *  \param  __pid
   * Points to a pid that is set to the pid from shm buffer if
   * we get a real buffer.
   *
   *  \param  __tid
   * Points to a tid that holds the tid for the shm buffer if we really
   * got a buffer.
   *
   *  \param  __it
   * Points to a time struct that holds the inittime from the process
   * that has written the shm buffer.
   *
   *  \param  __itb
   * Points to a click time that was fetched before we got the inittime.
   * With the after click we have a so called point in time triple.
   *
   *  \param  __ita
   * Points to a click time that was fetched after we got the inittime. 
   * With the before click we have a so called point in time triple.
   *
   *  \param  __lt
   * Points to a time struct that holds the final time that we got before
   * the buffer was placed into the shm.
   *
   *  \param  __ltb
   * Points to a click time that was fetched before we got the lasttime.
   * With the after click we have a so called point in time triple.
   *
   *  \param  __lta
   * Points to a click time that was fetched after we got the lasttime.
   * With the before click we have a so called point in time triple.
   *
   *  \param  __dft
   * Points to a click time that holds the difference between the
   * start of the transfer and the end, mainly the memcpy time
   * for the buffer.
   *
   *  \param  __stf
   * Points to a click time that holds the start of the transfer from
   * the client thread to the shm. This is used with the lasttime before 
   * to calculate the transfer time in the client for the buffer into
   * the shm buffer.
   *
   *  \param  __at
   * Points to a click time that holds the difference from the 
   * acquire call of the thread to get the buffer from the a list.
   * This can be the simple pop time or in worst cse the dyn memory
   * and initialization time for the pre allocated buffers.
   *
   *  \param  __id
   * Points to an int32 that holds the buffer id from the 
   * transfered client buffer which was given in the initialization
   * for the buffer.
   *
   * \return
   * - Negative if an error occured.
   * - Zero if a buffer could be fetched.
   * - Positiv if the call didnt fetch a buffer. In this case the 
   *   buffer length is 0 and all other values are meaningless.
   * 
   */
#define ATRSHMLOG_READ_FETCH(__a,__i,__t,__tl,__pid,__tid,__it,__itb,__ita,__lt,__ltb,__lta,__dft,__stf,__at,__id) \
  atrshmlog_read((__a),	  \
		 (__i),	   \
		 (__t),    \
		 (__tl),   \
		 (__pid),  \
		 (__tid),  \
		 (__it),    \
		 (__itb),   \
		 (__ita),   \
		 (__lt),   \
		 (__ltb),  \
		 (__lta),  \
		 (__dft),    \
		 (__stf), \
		 (__at),  \
		 (__id))


  /** 
   * \brief Read a buffer from shm.
   *
   * Get a shm buffer into normal memory and free shm buffer for next transfer.
   * This is the old call.
   *
   * \param __a
   * Points to an shm area
   *
   *  \param __i
   * Index of the buffer to check.
   *
   *  \param __t
   * Points to the start of a buffer big enough to hold the shm buffer
   * if any is present.
   *
   *  \param __tl
   * Points to an int32 that holds the length of the shm buffer 
   * after we fetched one. A 0 indicates that we didnt get one.
   *
   *  \param  __pid
   * Points to a pid that is set to the pid from shm buffer if
   * we get a real buffer.
   *
   *  \param  __tid
   * Points to a tid that holds the tid for the shm buffer if we really
   * got a buffer.
   *
   *  \param  __it
   * Points to a time struct that holds the inittime from the process
   * that has written the shm buffer.
   *
   *  \param  __itb
   * Points to a click time that was fetched before we got the inittime.
   * With the after click we have a so called point in time triple.
   *
   *  \param  __ita
   * Points to a click time that was fetched after we got the inittime. 
   * With the before click we have a so called point in time triple.
   *
   *  \param  __lt
   * Points to a time struct that holds the final time that we got before
   * the buffer was placed into the shm.
   *
   *  \param  __ltb
   * Points to a click time that was fetched before we got the lasttime.
   * With the after click we have a so called point in time triple.
   *
   *  \param  __lta
   * Points to a click time that was fetched after we got the lasttime.
   * With the before click we have a so called point in time triple.
   *
   *  \param  __dft
   * Points to a click time that holds the difference between the
   * start of the transfer and the end, mainly the memcpy time
   * for the buffer.
   *
   *  \param  __stf
   * Points to a click time that holds the start of the transfer from
   * the client thread to the shm. This is used with the lasttime before 
   * to calculate the transfer time in the client for the buffer into
   * the shm buffer.
   *
   *  \param  __at
   * Points to a click time that holds the difference from the 
   * acquire call of the thread to get the buffer from the a list.
   * This can be the simple pop time or in worst cse the dyn memory
   * and initialization time for the pre allocated buffers.
   *
   *  \param  __id
   * Points to an int32 that holds the buffer id from the 
   * transfered client buffer which was given in the initialization
   * for the buffer.
   *
   * \return
   * - Negative if an error occured.
   * - Zero if a buffer could be fetched.
   * - Positiv if the call didnt fetch a buffer. In this case the 
   *   buffer length is 0 and all other values are meaningless.
   * 
   */
#define ATRSHMLOG_READ(__a,__i,__t,__tl,__pid,__tid,__it,__itb,__ita,__lt,__ltb,__lta,__dft,__stf,__at,__id) \
  atrshmlog_read((__a),	  \
		 (__i),	   \
		 (__t),    \
		 (__tl),   \
		 (__pid),  \
		 (__tid),  \
		 (__it),    \
		 (__itb),   \
		 (__ita),   \
		 (__lt),   \
		 (__ltb),  \
		 (__lta),  \
		 (__dft),    \
		 (__stf), \
		 (__at),  \
		 (__id))


  /** 
   * \brief Verify the shm struct is ok.
   *
   * \return
   * - zero for ok
   * - Negative for en big error.
   * - Positiv for minor errors.
   */
#define ATRSHMLOG_VERIFY() atrshmlog_verify()

#endif
  
  /************************************************************************/

  /* the functions */

  /** 
   * \brief We connect to the shm buffer. Only connect, no use or checking.
   * 
   * We also do all the major initialisation stuff here, use all the 
   * environment variables and the flag files in case we do the login thing..
   *
   * So this is a simple but very important function.
   * 
   * All is covered inside.
   *
   * \return
   * - Zero if all went well.
   * - Negative if no connect was possible.
   * - Positive if we were already connected.
   */
  extern atrshmlog_ret_t atrshmlog_attach(void);


  /**
   * \brief We switch the statistics gathering on or off
   *
   * \param i_switch
   * - 0 : swith off
   * - non zero : switch on
   *
   * \return
   * Old flag value
   */
  extern atrshmlog_ret_t atrshmlog_set_statistics_on(const int i_switch);


  /**
   * \brief We deliver the max index of the statistics buffer
   *
   * \return 
   * Maximum index of the statistics buffer.
   */
  extern atrshmlog_int32_t atrshmlog_get_g_counters_max_index(void);

  /**
   * \brief We deliver the statistics counter.
   *
   * Its up to you to have enough space for them.
   * So use the get max index to do the right allocation.
   *
   * \param o_target
   * The start adress of an int32 array for the counters.
   *
   * \return 
   * void
   */
  extern void atrshmlog_get_g_counters(atrshmlog_int32_t* o_target);

  
  /** 
   * \brief We write an entry, no payload.
   *
   * This is the first of the three write functions. It makes the write
   * to the internal buffer for the thread.
   *
   * We have no additinal payload beside the standard infos.
   *
   *  \param  i_eventnumber
   * A 32 bit event number. This should not be overused, check for 
   * the event flag limit. You should hold this stable, meaning
   * it should identify the spot of logging.
   * So you can swith it on or off via events. And it identifies 
   * the spot where you log.
   *
   *  \param  i_eventflag
   * The flag to identify the kind of logging.
   * Its a well known fact that we use normally an interval. So the 
   * flag is indead a char and it is hten a big I ....
   * If you need a instant info - like an additional info or so - you 
   * normally use no interval. This is then a point in time entry, so I use a
   * big P for this.
   * Anything else is left for the user. But you are limited to a char, and 
   * it is a BAD THING t switch to unprintable values ....
   * Update: a small i and a small p is used in the Java jni layer.
   * So dont use them. The converter knows little about it,
   * but it is at least aware of it and trys its best.
   *
   *  \param  i_userflag
   * This is a 32 integer and its totally for the user.
   * So you can put inside as an info what you think is so important 
   * that it must come out.
   * Because it is the ONLY user info that is free of limitations
   * beside the size use it wisely - I suggest you use it together with
   * the event to make some kind of combined has map key and then 
   * use it to give you rough 10000 * 4 billion possible strings as 
   * real info ( if this is not enough you have to use write 1 or write 2 ...)
   * which is a lot of informafion for many people ...
   *
   *  \param  i_starttime
   * a click time. 
   *
   *  \param  i_endtime
   * a click time.
   * Side note: per convention the convert use a starttime and a endtime to 
   * do some calculation to interpret this as the start of the log entry
   * and end of the log entra. IT calcs the diffrence as the duration.
   * And it try to calc the real times with a linear approch from the 
   * time points init and last and the values for start and end assuming
   * they were taken with the same click counter - i.e. it claculates
   * an interpretes real time assuming you got the tick time 
   * and put it into start and ende...
   * So dont play games with those. They are used and if you insist you can
   * even corrupt the converter by using some nonsence data.
   *
   * \return
   * - 0 for a working log write
   * - else for an error or a suppressed one
   */
  extern atrshmlog_ret_t atrshmlog_write0(const atrshmlog_int32_t i_eventnumber,
					  const atrshmlog_int32_t i_eventflag,
					  const atrshmlog_int32_t i_userflag,
					  atrshmlog_time_t i_starttime,
					  atrshmlog_time_t i_endtime);

  /** 
   * \brief We write an entry, one buffer as additional payload.
   *
   * We have to do something we cannot solve with a payload of
   * only the userflag and the event alone. So we add an info.
   * Its a buffer of memory, and we can put inside whatever we want.
   * It is defined by the pointer and the size only.
   * You can embed whatever you want. Its transfered binary, so
   * you can use embedded zeros, unicode chars, structs ...
   * The only problem is that binary data will corrupt the converter.
   * But not the reader. So its up to you to use a self taylored 
   * converter if binary data is in place.
   * If you ONLY use printable char data you can live with the 
   * standard converter.
   * But its up to you if you need more - then take the converter code
   * and hack it to your needs.
   * Its only up to you.
   *
   *  \param  i_eventnumber
   * A 32 bit event number. This should not be overused, check for 
   * the event flag limit. You should hold this stable, meaning
   * it should identify the spot of logging.
   * So you can swith it on or off via events. And it identifies 
   * the spot where you log.
   *
   *  \param  i_eventflag
   * The flag to identify the kind of logging.
   * Its a well known fact that we use normally an interval. So the 
   * flag is indead a char and it is then a big I ....
   * If you need a instant info - like an additional info or so - you 
   * normally use no interval. This is then a point in time entry, so I use a
   * big P for this.
   * Anything else is left for the user. But you are limited to a char, and 
   * it is a BAD THING t switch to unprintable values ....
   * Update: a small i and a small p is used in the Java jni layer.
   * So dont use them. The converter knows little about it,
   * but it is at least aware of it and trys its best.
   *
   *  \param  i_userflag
   * This is a 32 integer and its totally for the user.
   * So you can put inside as an info what you think is so important 
   * that it must come out.
   * Because it is the ONLY user info that is free of limitations
   * beside the size use it wisely - I suggest you use it together with
   * the event to make some kind of combined has map key and then 
   * use it to give you rough 10000 * 4 billion possible strings as 
   * real info 
   * which is a lot of informafion for many people ...
   * And because you use write1 there is more info in the additional payload.
   * 
   *  \param  i_starttime
   *  a click time. 
   *
   *  \param  i_endtime
   *  a click time.
   * Side note: per convention the convert use a starttime and a endtime to 
   * do some calculation to interpret this as the start of the log entry
   * and end of the log entra. IT calcs the diffrence as the duration.
   * And it try to calc the real times with a linear approch from the 
   * time points init and last and the values for start and end assuming
   * they were taken with the same click counter - i.e. it claculates
   * an interpretes real time assuming you got the tick time 
   * and put it into start and ende...
   * So dont play games with those. They are used and if you insist you can
   * even corrupt the converter by using some nonsence data.
   *
   *  \param  i_local
   * Points to the data that will be embedded as additional payload 
   * by memcpy it.
   *
   *  \param  i_size
   * The size of the embedded payload starting at i_local interpreted as
   * an adress in memory. size 0 is allowed.
   *
   * \return
   * - 0 for a working log write
   * - else for an error or a suppressed one
   */
  extern atrshmlog_ret_t atrshmlog_write1(const atrshmlog_int32_t i_eventnumber,
					  const atrshmlog_int32_t i_eventflag,
					  const atrshmlog_int32_t i_userflag,
					  atrshmlog_time_t i_starttime,
					  atrshmlog_time_t i_endtime,
					  const void* i_local,
					  const atrshmlog_int32_t i_size);

  /** 
   * \brief We write an entry, one buffer and one argv array payload.
   *
   *  \param  i_eventnumber
   * A 32 bit event number. This should not be overused, check for 
   * the event flag limit. You should hold this stable, meaning
   * it should identify the spot of logging.
   * So you can swith it on or off via events. And it identifies 
   * the spot where you log.
   *
   *  \param  i_eventflag
   * The flag to identify the kind of logging.
   * Its a well known fact that we use normally an interval. So the 
   * flag is indead a char and it is hten a big I ....
   * If you need a instant info - like an additional info or so - you 
   * normally use no interval. This is then a point in time entry, so I use a
   * big P for this.
   * Anything else is left for the user. But you are limited to a char, and 
   * it is a BAD THING t switch to unprintable values ....
   * Update: a small i and a small p is used in the Java jni layer.
   * So dont use them. The converter knows little about it,
   * but it is at least aware of it and trys its best.
   *
   *  \param  i_userflag
   * This is a 32 integer and its totally for the user.
   * So you can put inside as an info what you think is so important 
   * that it must come out.
   * Because it is the ONLY user info that is free of limitations
   * beside the size use it wisely - I suggest you use it together with
   * the event to make some kind of combined has map key and then 
   * use it to give you rough 10000 * 4 billion possible strings as 
   * real info 
   * which is a lot of informafion for many people ...
   * And because you use write1 there is more info in the additional payload.
   * 
   *  \param  i_starttime
   *  a click time. 
   *
   *  \param  i_endtime
   *  a click time.
   * Side note: per convention the convert use a starttime and a endtime to 
   * do some calculation to interpret this as the start of the log entry
   * and end of the log entra. IT calcs the diffrence as the duration.
   * And it try to calc the real times with a linear approch from the 
   * time points init and last and the values for start and end assuming
   * they were taken with the same click counter - i.e. it claculates
   * an interpretes real time assuming you got the tick time 
   * and put it into start and ende...
   * So dont play games with those. They are used and if you insist you can
   * even corrupt the converter by using some nonsence data.
   *
   *  \param  i_local
   * Points to the data that will be embedded as additional payload 
   * by memcpy it.
   *
   *  \param  i_size
   * The size of the embedded payload starting at i_local interpreted as
   * an adress in memory. size 0 is allowed.
   *
   * \param i_argv
   * The char** for the additional c-style string array.
   *
   * \param i_argc_hint
   * The count hint for the argv processing. Zero or negative no hint,
   * positive the limit.
   *
   * \return
   * - for a working log write
   * - else for an error or a suppressed one
   */
  extern atrshmlog_ret_t atrshmlog_write2(const atrshmlog_int32_t i_eventnumber,
					  const atrshmlog_int32_t i_eventflag,
					  const atrshmlog_int32_t i_userflag,
					  atrshmlog_time_t i_starttime,
					  atrshmlog_time_t i_endtime,
					  const void* i_local,
					  const atrshmlog_int32_t i_size,
					  const char* i_argv[],
					  const atrshmlog_int32_t i_argc_hint);


  /**
   * \brief We wait nanoseconds
   *
   * \param i_nanos
   * The nanosecs till 2^31
   *
   * \return
   * void
   */
  extern void atrshmlog_nanosleep (atrshmlog_int32_t i_nanos);
  
  /** 
   * \brief We set the prefix for name lookup in the program. 
   *
   * \param i_prefix
   * Start of the new prefix.
   *
   * \return
   * void
   */
  extern void atrshmlog_set_envprefix (const char *i_prefix);

  /** 
   * \brief We get the actual name of the environment variable prefix.
   *
   * \return 
   * Points to the used prefix.
   */  
  extern const char* atrshmlog_get_envprefix(void);

  /** 
   * \brief We get the env variable value of prefix and the supported suffix.
   *
   * \param i_suffix
   * Points to the suffix after the prefix for the variable name.
   *
   * \return
   * - Points to the value of the variable.
   * - Zero if no variable defined.
   */
  extern const char* atrshmlog_get_env(const char* i_suffix); 

  /** 
   * \brief We get the value of the environment variable holding the shmid in text.
   *
   * \return
   * - Points to the shmid variable value
   * - Zero if no variable defined.
   */  
  extern const char* atrshmlog_get_envshmid(void);

  extern const char* atrshmlog_get_envidsuffix(void);
  
  /** 
   * \brief The version of the buffer system .
   *
   * If we change anything of the buffer layout we have to change this too.
   * Its for layout compatible processing of buffers. 
   * From the internal shm buffer tot eh readers to the converters.
   * Any change you need is a new layout. Even a simple additional number.
   * 
   * \return
   * Number of the major version of the shmlog.
   */   
  extern atrshmlog_ret_t atrshmlog_get_version(void);

  /**
   * \brief The functionality of the version of the log.
   *
   * We can have different versions here if only the interface
   * is up compatible and the buffer layout is the same.
   * For example on can have additional fences. Or additional
   * sleeps in processing.
   * 
   * \return
   * Number of the minor version of the shmlog.
   */  
  extern atrshmlog_ret_t atrshmlog_get_minor_version(void);

  /**
   *  \brief This is the patch level, it inform of important changes in functionality 
   * for removing errors from the log.
   *
   * So no additional functionality, no new fences or new waits.
   * But some bugfix...
   *
   * \return
   * Number of the patch version of the shmlog.
   */  
  extern atrshmlog_ret_t atrshmlog_get_patch_version(void);

  /** 
   * \brief Get the event flag maximum index number
   *
   * \return
   * Number of the max event + 1 
   */
  extern atrshmlog_ret_t atrshmlog_get_event_locks_max();

  /** 
   * \brief Set the event flags maximum index number.
   *
   * This can be done to make the buffer bigger.
   * We normally live with a fixed static buffer and 10000 events.
   * In case of instrumentalization it can be too small.
   * Then we have to set it before the first use - best before 
   * attach if any happens.
   * Or simply change the defien and recompile.
   *
    * \param i_max
   * Number of the new maximum event plus 1.
   *
   * \return
   * Number of the old max event + 1 
   */
  extern atrshmlog_ret_t atrshmlog_set_event_locks_max(atrshmlog_int32_t i_max);


  /**
   * \brief Deliver the value of the event  - its normally 0 or 1.
   *
   * 0 mean to ignore the things based on this event.
   * 1 mean to do the things...
   * All others are normally errors ... 
   * Be sure not to use more than max for the event number.
   *
   * \param i_index
   * Number of the event that is set to 1.
   *
   * \return
   * void
   */
  extern atrshmlog_event_t atrshmlog_get_event(atrshmlog_int32_t i_index);

  /**
   * \brief Helper to set an event from the program.
   *
   * \param i_index
   * Number of the event that is set to 1.
   *
   * \return
   * Old event
   */  
  extern atrshmlog_event_t atrshmlog_set_event_on(atrshmlog_int32_t i_index);

  /**
   * \brief Helper to clear an event from the program.
   *
   * \param i_index
   * Number of the event that is set to 0.
   *
   * \return
   * old event
   */
  extern atrshmlog_event_t atrshmlog_set_event_off(atrshmlog_int32_t i_index);


  /**
   * \brief Deliver logging state for the program.
   *
   * \return
   * Number of the logging flag for the process. Zero is
   * no ligging, not zero is logging.
   * Its normally 1 for the bit and with events.
   */
  extern atrshmlog_ret_t atrshmlog_get_logging(void);
  
  /**
   * \brief Switch the logging for the process on .
   *
   * \return
   * The old number of the logging flag.
   */  
  extern atrshmlog_ret_t atrshmlog_set_logging_process_on(void);

  /**
   * \brief Switch the logging for the process off.
   *
   * \return
   * Number of the shared memory id.
   */
  extern atrshmlog_ret_t atrshmlog_set_logging_process_off(void);

  /**
   * \brief Switch the logging for the process off finally.
   *
   * \return
   * Old flag.
   */
  extern atrshmlog_ret_t atrshmlog_set_logging_process_off_final(void);

  /**
   * \brief Deliver the shm id of the process.
   *
   * This is the variable shmid.
   *
   * \return
   * Number of the shared memory id.
   */  
  extern int atrshmlog_get_shmid(void);


  /**
   * \brief The area buffer in shm start adress.
   *
   * \return
   * Points to the shared memory area.
   */
  extern volatile const void* atrshmlog_area(void);

  /**
   * \brief The value of the flag in shm.
   *
   * \param i_area
   * Points to the start of the shared memory area.
   *
   * \return
   * The number of the flag. Zero means we are running, Not zero we are 
   * no longer running the log.
   */
  extern atrshmlog_ret_t atrshmlog_get_area_ich_habe_fertig(volatile const void* i_area);

  /**
   * \brief The value of the flag of the shm area
   * and set a new value.
   *
   * \param i_area
   * Points to the start of the shared memory area.
   *
   * \param i_flag
   * The new flag , 0 means go on, not 0 means stop
   *
   * \return
   * The number of buffers in the area.
   */
  extern atrshmlog_ret_t atrshmlog_set_area_ich_habe_fertig(volatile const void* i_area, atrshmlog_int32_t i_flag);

  /**
   * \brief The count of buffers in the shm log area.
   *
   * \param i_area
   * Points to the start of the shared memory area.
   *
   * \return
   * The number of the version in the shared memory area.
   */
  extern atrshmlog_ret_t atrshmlog_get_area_count(volatile const void* i_area);

  /**
   * \brief The version of the log area in shm.
   *
   * \param i_area
   * Points to the start of the shared memory area.
   *
   * \return
   * The number of the version in the shared memory area.
   */
  extern atrshmlog_ret_t atrshmlog_get_area_version(volatile const void* i_area);

  /**
   * \brief The maximum size of log buffer.
   *
   * \return
   * The number of the maximum size of log buffers.
   */
  extern atrshmlog_ret_t atrshmlog_get_logbuffermaxsize(void);
  
  /**
   * \brief We get the actual set info size for a log buffer.
   *
   * \return
   * Number of the actual size for log buffers.
   */
  extern atrshmlog_ret_t atrshmlog_get_buffer_infosize(void);

  /**
   * \brief We get the old size and set the new size if between 0 and 
   * logbuffer_size.
   *
   * \param i_size
   * The new number f bytes for log buffers.
   *
   * \return
   * The old number of bytes for log buffers.
   */
  extern atrshmlog_ret_t atrshmlog_set_buffer_infosize(atrshmlog_int32_t i_size);

  /**
   * \brief Get the count of slave threads that the logging use.
   *
   * \return
   * The number of threads running the function for slave proc.
   */ 
  extern atrshmlog_ret_t atrshmlog_get_f_list_buffer_slave_count(void);

  /**
   * \brief Set the number of logging slave threads.
   *
   * This has to be before we start the threads. 
   * This means you have to do it before attach.
   *
   * \param i_count
   * The new count of slave threads to start.
   *
   * \return
   * The old number of slave threads to start.
   */
  extern atrshmlog_ret_t atrshmlog_set_f_list_buffer_slave_count(atrshmlog_int32_t i_count);

  /**
   * \brief Get the used clock id for the timing info from real time clock.
   *
   * \return
   * The id of the clock to use in get clock call.
   */
  extern atrshmlog_ret_t atrshmlog_get_clock_id(void);

  /**
   * \brief Set the clock id for the gettime call.
   *
   * \param i_id
   * The id for the get clock call.
   *
   * \return
   * The old id for the get clock call.
   */
  extern atrshmlog_ret_t atrshmlog_set_clock_id(atrshmlog_int32_t i_id);

  /**
   * \brief Shut off the slave threads that are not working right now.
   *
   * This will not terminate threads.
   * It only sets the slave exit to on, so a slave from its 
   * wake up directly exits, does not do any more work.
   * Actually running slaves are not affected, they do their work
   * for the actual buffer and then terminate.
   *
   * \return
   * void
   */
  extern void atrshmlog_set_f_list_buffer_slave_run_off(void);

  /**
   * \brief This sets the flag to wait for slaves count down to 0
   * in the atexit.
   *
   * \return
   * The old flag.
   */
  extern atrshmlog_ret_t atrshmlog_set_wait_for_slaves_on(void);

  /**
   * \brief This switches the waiting flag off in the atexit.
   *
   * \return
   * The old flag.
   */
  extern atrshmlog_ret_t atrshmlog_set_wait_for_slaves_off(void);

  /**
   * \brief This is the flag value for waiting in atexit for slaves down.
   *
   * \return
   * The flag for the wait for slaves.
   */
  extern atrshmlog_ret_t atrshmlog_get_wait_for_slaves(void);

  /**
   * \brief Get the wait time in nanoseconds for the slave .
   *
   * \return
   * The number of nanos the slave sleeps when nothing has to be done.
   */
  extern atrshmlog_ret_t atrshmlog_get_f_list_buffer_slave_wait(void);

  /**
   * \brief Set the wait time for the slaves in nanoseconds.
   *
   * \param i_wait_nanos
   * The number of nanos the slave has to sleep when nothing has
   * to be done.
   *
   * \return
   * The old number of nanos the slave had to wait.
   */
  extern atrshmlog_ret_t atrshmlog_set_f_list_buffer_slave_wait(atrshmlog_int32_t i_wait_nanos);

  /** 
   * \brief Count of buffers ready for acquire.
   *
   * \return
   * The number of buffers for logging in the threads.
   */
  extern atrshmlog_ret_t atrshmlog_get_acquire_count(void);

  /**
   * \brief Get count of preallocate buffers in one alloc.
   *
   * \return
   * The number of buffers a dyn alloc count gets in one malloc
   * from the dyn memory area.
   */
  extern atrshmlog_ret_t atrshmlog_get_prealloc_buffer_count(void);

  /**
   * \brief Set count of buffers in prealloc in alloc.
   *
   * \param i_count
   * The new number of buffers the alloc fetches from dyn memory
   * area in one malloc.
   *
   * \return
   * The old number of buffers fetched in one malloc.
   */
  extern atrshmlog_ret_t atrshmlog_set_prealloc_buffer_count(atrshmlog_int32_t i_count);

  /**
   * \brief Initial time for the module .
   *
   * \return
   * The internal time struct with the inittime of the process.
   */
  extern atrshmlog_internal_time_t atrshmlog_get_inittime(void);

  /**
   * \brief Initial click before get inittime.
   *
   * \return
   * The time for the click before the get clock call to fill inittime.
   */
  extern atrshmlog_time_t atrshmlog_get_inittimetsc_before(void);

  /**
   * \brief Initial click after get initttime.
   *
   * \return
   * The time for the click after the get clock call to fill inittime.
   */
  extern atrshmlog_time_t atrshmlog_get_inittimetsc_after(void);

  /**
   * \brief Get highest id for buffers.
   *
   * \return
   * get the id of the last buffer initialized.
   */
  extern atrshmlog_ret_t atrshmlog_get_buffer_id(void);

  /**
   * \brief We stop loging for the calling thread.
   *
   * \return
   * void
   */
  extern  void atrshmlog_stop(void);

  /**
   * \brief We flush the buffers for the calling thread.
   *
   * \return
   * void
   */
  extern  void atrshmlog_flush(void);

  /**
   * \brief Set the strategy for this thread.
   *
   * \param i_strategy
   * - 0: discard
   * - 1: spin loop
   * - 2: wait
   * - 3: adaptive
   * - 4: adaptive fast
   *
   * \return
   * The old strategy value 
   */
  extern  atrshmlog_ret_t atrshmlog_set_strategy(const enum atrshmlog_strategy i_strategy);

  /**
   * \brief Set the strategy for the process
   *
   * \param i_strategy
   * - 0: discard
   * - 1: spin loop
   * - 2: wait
   * - 3: adaptive
   * - 4: adaptive fast
   *
   * \return
   * The old strategy value 
   */
  extern  atrshmlog_ret_t atrshmlog_set_strategy_process(const enum atrshmlog_strategy i_strategy);

  /**
   * \brief We can start a slave thread with it.
   *
   * \return
   * The return code of the used function to start the thread.
   */
  extern int atrshmlog_create_slave(void);

  /**
   * \brief We switch a fence on or off
   *
   * \return
   * The old flag for fence.
   */
  extern atrshmlog_ret_t atrshmlog_set_thread_fence_1(atrshmlog_int32_t i_switch);
  
  /**
   * \brief We switch a fence on or off
   *
   * \return
   * The old flag for fence.
   */
  extern atrshmlog_ret_t atrshmlog_set_thread_fence_2(atrshmlog_int32_t i_switch);
  
  /**
   * \brief We switch a fence on or off
   *
   * \return
   * The old flag for fence.
   */
  extern atrshmlog_ret_t atrshmlog_set_thread_fence_3(atrshmlog_int32_t i_switch);
  
  /**
   * \brief We switch a fence on or off
   *
   * \return
   * The old flag for fence.
   */
  extern atrshmlog_ret_t atrshmlog_set_thread_fence_4(atrshmlog_int32_t i_switch);
  
  /**
   * \brief We switch a fence on or off
   *
   * \return
   * The old flag for fence.
   */
  extern atrshmlog_ret_t atrshmlog_set_thread_fence_5(atrshmlog_int32_t i_switch);
  
  /**
   * \brief We switch a fence on or off
   *
   * \return
   * The old flag for fence.
   */
  extern atrshmlog_ret_t atrshmlog_set_thread_fence_6(atrshmlog_int32_t i_switch);
  
  /**
   * \brief We switch a fence on or off
   *
   * \return
   * The old flag for fence.
   */
  extern atrshmlog_ret_t atrshmlog_set_thread_fence_7(atrshmlog_int32_t i_switch);
  
  /**
   * \brief We switch a fence on or off
   *
   * \return
   * The old flag for fence.
   */
  extern atrshmlog_ret_t atrshmlog_set_thread_fence_8(atrshmlog_int32_t i_switch);
  
  /**
   * \brief We switch a fence on or off
   *
   * \return
   * The old flag for fence.
   */
  extern atrshmlog_ret_t atrshmlog_set_thread_fence_9(atrshmlog_int32_t i_switch);
  
  /**
   * \brief We switch a fence on or off
   *
   * \return
   * The old flag for fence.
   */
  extern atrshmlog_ret_t atrshmlog_set_thread_fence_10(atrshmlog_int32_t i_switch);
  
  /**
   * \brief We switch a fence on or off
   *
   * \return
   * The old flag for fence.
   */
  extern atrshmlog_ret_t atrshmlog_set_thread_fence_11(atrshmlog_int32_t i_switch);
  
  /**
   * \brief We switch a fence on or off
   *
   * \return
   * The old flag for fence.
   */
  extern atrshmlog_ret_t atrshmlog_set_thread_fence_12(atrshmlog_int32_t i_switch);

  /**
   * \brief WE get the real time
   *
   * \return 
   * The real time in our format
   */
  extern  atrshmlog_internal_time_t atrshmlog_get_realtime(void);

#if ATRSHMLOG_LEVEL > 0

  /** 
   * \brief We read a buffer and write it to a local memory area .
   *
   * We do all checks too.
   *
   * We use the lock algorithm via state for this, not the f list in shm.
   *
   * This is an obsolet function, but if you insist to transfer buffers
   * to the shm after stop logging and you dont want to play games
   * with the a and f list in shm you can pull them off with this one.
   * It was the thing used before the s and a list came to the shm.
   *
   * \param i_area
   * Points to an shm area
   *
   *  \param i_index_buffer
   * The index of the buffer to check.
   *
   *  \param o_target
   * Points to the start of a buffer big enough to hold the shm buffer
   * if any is present.
   *
   *  \param o_target_length
   * Points to an int32 that holds the length of the shm buffer 
   * after we fetched one. A 0 indicates that we didnt get one.
   *
   *  \param  o_pid
   * Points to a pid that is set to the pid from shm buffer if
   * we get a real buffer.
   *
   *  \param  o_tid
   * Points to a tid that holds the tid for the shm buffer if we really
   * got a buffer.
   *
   *  \param  o_inittime
   * Points to a time struct that holds the inittime from the process
   * that has written the shm buffer.
   *
   *  \param  o_inittimetsc_before
   * Points to a click time that was fetched before we got the inittime.
   * With the after click we have a so called point in time triple.
   *
   *  \param  o_inittimetsc_after
   * Points to a click time that was fetched after we got the inittime. 
   * With the before click we have a so called point in time triple.
   *
   *  \param  o_lasttime
   * Points to a time struct that holds the final time that we got before
   * the buffer was placed into the shm.
   *
   *  \param  o_lasttimetsc_before
   * Points to a click time that was fetched before we got the lasttime.
   * With the after click we have a so called point in time triple.
   *
   *  \param  o_lasttimetsc_after
   * Points to a click time that was fetched after we got the lasttime.
   * With the before click we have a so called point in time triple.
   *
   *  \param  o_difftimetransfer
   * Points to a click time that holds the difference between the
   * start of the transfer and the end, mainly the memcpy time
   * for the buffer.
   *
   *  \param  o_starttransfer
   * Points to a click time that holds the start of the transfer from
   * the client thread to the shm. This is used with the lasttime before 
   * to calculate the transfer time in the client for the buffer into
   * the shm buffer.
   *
   *  \param  o_acquiretime
   * Points to a click time that holds the difference from the 
   * acquire call of the thread to get the buffer from the a list.
   * This can be the simple pop time or in worst cse the dyn memory
   * and initialization time for the pre allocated buffers.
   *
   *  \param  o_id
   * Points to an int32 that holds the buffer id from the 
   * transfered client buffer which was given in the initialization
   * for the buffer.
   *
   * \return
   * - Negative if an error occured.
   * - Zero if a buffer could be fetched.
   * - Positiv if the call didnt fetch a buffer. In this case the 
   *   buffer length is 0 and all other values are meaningless.
   */
  extern atrshmlog_ret_t atrshmlog_read(volatile const void* i_area,
					const atrshmlog_int32_t i_index_buffer,
					void *o_target,
					atrshmlog_int32_t* o_target_length,
					atrshmlog_pid_t* o_pid,
					atrshmlog_tid_t* o_tid,
					atrshmlog_internal_time_t* o_inittime,
					atrshmlog_time_t* o_inittimetsc_before,
					atrshmlog_time_t* o_inittimetsc_after,
					atrshmlog_internal_time_t* o_lasttime,
					atrshmlog_time_t* o_lasttimetsc_before,
					atrshmlog_time_t* o_lasttimetsc_after,
					atrshmlog_time_t* o_difftimetransfer,
					atrshmlog_time_t* o_starttransfer,
					atrshmlog_time_t* o_acquiretime,
					atrshmlog_int32_t* o_id);

  /** 
   * \brief We read a buffer and write it to a local memory area. 
   *
   * We do all checks too.
   *
   * We get it from the shmf list. We dont wait. If there is no
   * buffer we return quite.
   * 
   * \param i_area
   * Points to an shm area
   *
   *  \param o_index_buffer
   * Points to an int32 to hold the index of the shm buffer if we succeed
   * and really get a buffer.
   *
   *  \param o_target
   * Points to the start of a buffer big enough to hold the shm buffer
   * if any is present.
   *
   *  \param o_target_length
   * Points to an int32 that holds the length of the shm buffer 
   * after we fetched one. A 0 indicates that we didnt get one.
   *
   *  \param  o_pid
   * Points to a pid that is set to the pid from shm buffer if
   * we get a real buffer.
   *
   *  \param  o_tid
   * Points to a tid that holds the tid for the shm buffer if we really
   * got a buffer.
   *
   *  \param  o_inittime
   * Points to a time struct that holds the inittime from the process
   * that has written the shm buffer.
   *
   *  \param  o_inittimetsc_before
   * Points to a click time that was fetched before we got the inittime.
   * With the after click we have a so called point in time triple.
   *
   *  \param  o_inittimetsc_after
   * Points to a click time that was fetched after we got the inittime. 
   * With the before click we have a so called point in time triple.
   *
   *  \param  o_lasttime
   * Points to a time struct that holds the final time that we got before
   * the buffer was placed into the shm.
   *
   *  \param  o_lasttimetsc_before
   * Points to a click time that was fetched before we got the lasttime.
   * With the after click we have a so called point in time triple.
   *
   *  \param  o_lasttimetsc_after
   * Points to a click time that was fetched after we got the lasttime.
   * With the before click we have a so called point in time triple.
   *
   *  \param  o_difftimetransfer
   * Points to a click time that holds the difference between the
   * start of the transfer and the end, mainly the memcpy time
   * for the buffer.
   *
   *  \param  o_starttransfer
   * Points to a click time that holds the start of the transfer from
   * the client thread to the shm. This is used with the lasttime before 
   * to calculate the transfer time in the client for the buffer into
   * the shm buffer.
   *
   *  \param  o_acquiretime
   * Points to a click time that holds the difference from the 
   * acquire call of the thread to get the buffer from the a list.
   * This can be the simple pop time or in worst cse the dyn memory
   * and initialization time for the pre allocated buffers.
   *
   *  \param  o_id
   * Points to an int32 that holds the buffer id from the 
   * transfered client buffer which was given in the initialization
   * for the buffer.
   *
   * \return
   * - Negative if an error occured.
   * - Zero if a buffer could be fetched.
   * - Positiv if the call didnt fetch a buffer. In this case the 
   *   buffer length is 0 and all other values are meaningless.
   */
  extern atrshmlog_ret_t atrshmlog_read_fetch(volatile const void* i_area,
					      atrshmlog_int32_t *o_index_buffer,
					      void *o_target,
					      atrshmlog_int32_t *o_target_length,
					      atrshmlog_pid_t* o_pid,
					      atrshmlog_tid_t* o_tid,
					      atrshmlog_internal_time_t* o_inittime,
					      atrshmlog_time_t* o_inittimetsc_before,
					      atrshmlog_time_t* o_inittimetsc_after,
					      atrshmlog_internal_time_t* o_lasttime,
					      atrshmlog_time_t* o_lasttimetsc_before,
					      atrshmlog_time_t* o_lasttimetsc_after,
					      atrshmlog_time_t* o_difftimetransfer,
					      atrshmlog_time_t* o_starttransfer,
					      atrshmlog_time_t* o_acquiretime,
					      atrshmlog_int32_t* o_id);

  /** 
   * \brief We verify the buffer is inited and structural ok .
   *
   * \return
   * - Zero if ok.
   * - Positive for minor error.
   * - Negative for major error.
   */
  extern atrshmlog_ret_t atrshmlog_verify(void);

#endif

  /**************************************************************/
  // inline code 

  /*
   * our internal helpers. we need them in the testdrivers 
   * for checking the right available call on this box
   */
  #if ATRSHMLOG_INLINE_TSC_CODE == 1

  #if ATRSHMLOG_PLATFORM_LINUX_X86_64_GNU == 1 

  /**
   * We use the modern cpu version of reading the click counter
   * register tsc.
   *
   * \return
   * The 64 bit number with the tick count.
   */
  inline uint64_t  atrshmlog_get_tsc_par_x86_64_gnu(void)
  {
    uint32_t hi, lo;

    __asm volatile
      ("rdtscp" : "=a" (lo), "=d" (hi));

    return  ((uint64_t)hi << 32) | lo;
  }


  /**
   * We use the old cpu version of reading the click counter
   * register tsc. We use a lfence for multicore here.
   *
   * \return
   * The 64 bit number with the tick count.
   */
  inline uint64_t  atrshmlog_get_tsc_fence_x86_64_gnu(void)
  {
    uint32_t hi, lo;
    
    __asm volatile
      ("lfence");
    
    __asm volatile
      ("rdtsc" : "=a" (lo), "=d" (hi));
    
    return  ((uint64_t)hi << 32) | lo;
  }

  /**
   * We use the old cpu version of reading the click counter
   * register tsc. No syncronization here.
   *
   * \return
   * The 64 bit number with the tick count.
   */
  inline  uint64_t atrshmlog_get_tsc_x86_64_gnu(void)
  {
    uint32_t hi, lo;

    __asm volatile
      ("rdtsc" : "=a" (lo), "=d" (hi));

    return  ((uint64_t)hi << 32) | lo;
  }

  /**
   * The well know dummy 
   * \return 
   * Null.
   */
  inline  uint64_t  atrshmlog_get_tsc_null_x86_64_gnu(void)
  {
    return 0;
  }
#endif
  // linux x86_64_gnu
  
  #else

  // no inline, prototypes needed
# if ATRSHMLOG_PLATFORM_LINUX_X86_64_GNU == 1 
  
  /**
   * We use the modern cpu version of reading the click counter
   * register tsc.
   *
   * \return
   * The 64 bit number with the tick count.
   */
  extern uint64_t atrshmlog_get_tsc_par_x86_64_gnu(void);

  /**
   * We use the old cpu version of reading the click counter
   * register tsc. We use a lfence for multicore here.
   *
   * \return
   * The 64 bit number with the tick count.
   */
  extern uint64_t atrshmlog_get_tsc_fence_x86_64_gnu(void);

  /**
   * We use the old cpu version of reading the click counter
   * register tsc. No syncronization here.
   *
   * \return
   * The 64 bit number with the tick count.
   */
  extern uint64_t atrshmlog_get_tsc_x86_64_gnu(void);

  /**
   * The well know dummy 
   * \return 
   * Null.
   */
  extern uint64_t atrshmlog_get_tsc_null_x86_64_gnu(void);

# endif
  // linux 86 64 gnu
  
#endif

#if ATRSHMLOG_INLINE_GETTIME == 1  
  
# if ATRSHMLOG_PLATFORM_LINUX_X86_64_GNU == 1 


  /**
   * \brief We get the click time from here.
   *
   * This is used in later versions, for now its a start.
   * We adress here the possibility to use code for 
   * similar architecture on diffrent systems.
   * So we need to choose a low level function by id.
   *
   * \return
   * The 64 bit  click time 
   */
  inline atrshmlog_time_t  atrshmlog_get_clicktime(void)
  {
    if (atrshmlog_clock_id == 3)
      {
	return (atrshmlog_time_t)atrshmlog_get_tsc_par_x86_64_gnu();
      }
    
    if (atrshmlog_clock_id == 2)
      {
	return (atrshmlog_time_t)atrshmlog_get_tsc_fence_x86_64_gnu();
      }
    
    if (atrshmlog_clock_id == 1)
      {
	return (atrshmlog_time_t)atrshmlog_get_tsc_x86_64_gnu();
      }
    
    return (atrshmlog_time_t)atrshmlog_get_tsc_null_x86_64_gnu();
  }

# endif
  
#else
  /** 
   * We get a timestamp from the system -- prototpe only 
   *
   * \param __t
   * Variable of type struct time to get the time.
   *
   * \return
   * void
   */
  extern atrshmlog_time_t atrshmlog_get_clicktime(void);
  
#endif
  
  
#ifdef __cplusplus
}
#endif
  
#endif

/* end of file */
