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
 * See the dot.platform.sh files for the setting of the platform.
 *
 * Use this file to access the functions via macros. 
 */


/*********************************************************************/

// The defines set by user.

// After setting those according to documentation
// you have to recompile the module.

// Some additional defines are in the atrshmlog_internal header,
// So check this header too.

/********************************************************/


// The platforms supportet.
// Only one can be active.
// You have to know at least the hardware platform
// and optional the os
// and if you are depending on compiler stuff.
// See the dot.platform.sh files for setting it and
// the related environment variables.
//
// If you are sure about it you can set one of the defines right
// in this place to 1.

#ifndef ATRSHMLOG_PLATFORM_LINUX_X86_64_GCC
/** 
 * \brief We have a x86 64 bit, a linux and a gnu c 
 *
 * the major development platform. in doubt its the master.
 * if you need a new platform try this as a clone first
 */
#define ATRSHMLOG_PLATFORM_LINUX_X86_64_GCC 0

#endif

#ifndef ATRSHMLOG_PLATFORM_CYGWIN_X86_64_GCC
/** 
 * \brief We have a x86 64 bit, a cygwin and a gnu c 
 *
 * Testet. Java wont work with it for now.
 */
#define ATRSHMLOG_PLATFORM_CYGWIN_X86_64_GCC 0

#endif


#ifndef ATRSHMLOG_PLATFORM_MINGW_X86_64_GCC
/** 
 * \brief We have a x86 64 bit, a cygwin mingw64 and a gnu c 
 *
 * Testet. java works with it for oracle jdk 1.8.0 b 102
 */
#define ATRSHMLOG_PLATFORM_MINGW_X86_64_GCC 0

#endif


#ifndef ATRSHMLOG_PLATFORM_BSD_AMD64_CLANG
/** 
 * \brief We have a x86 64 bit, a bsd and a clang c 
 *
 * No java support tested so far.
 */
#define ATRSHMLOG_PLATFORM_BSD_AMD64_CLANG 0

#endif

#ifndef ATRSHMLOG_PLATFORM_BSD_AMD64_GCC
/** 
 * \brief We have a x86 64 bit, a bsd and a gcc c 
 *
 * No java support tested so far.
 */
#define ATRSHMLOG_PLATFORM_BSD_AMD64_GCC 0

#endif


#ifndef ATRSHMLOG_PLATFORM_SOLARIS_X86_64_GCC
/** 
 * \brief We have a x86 64 bit, a solaris and a gnu c 
 *
 * No java support tested so far.
 */
#define ATRSHMLOG_PLATFORM_SOLARIS_X86_64_GCC 0

#endif

/*******************************************************/

#if ( ATRSHMLOG_PLATFORM_LINUX_X86_64_GCC		\
      + ATRSHMLOG_PLATFORM_CYGWIN_X86_64_GCC		\
      + ATRSHMLOG_PLATFORM_MINGW_X86_64_GCC		\
      + ATRSHMLOG_PLATFORM_BSD_AMD64_CLANG		\
      + ATRSHMLOG_PLATFORM_BSD_AMD64_GCC		\
      + ATRSHMLOG_PLATFORM_SOLARIS_X86_64_GCC ) == 1
// we are ok. one platform is active
#else
#error bumm bumm bumm bumm error in platform active count
#error did you not set the platform with a dot.platform file
#error check the documentation about the build and example use
#endif

/*******************************************************/


/** 
 * \brief What is our compilers inline attribut 
 * We use the c99 inline here 
 */
#ifndef ATRSHMLOG_INLINE
#define ATRSHMLOG_INLINE inline
#endif

/**
 * \brief What is our compilers function decoration
 * You can use this for the windows declspec for example
 */
#ifndef ATRSHMLOG_FUNCTION_DECORATOR
#define ATRSHMLOG_FUNCTION_DECORATOR 
#endif

/**
 * \brief What is our compilers variable decoration
 * You can use this for the windows declspec for example
 */
#ifndef ATRSHMLOG_VARIABLE_DECORATOR
#define ATRSHMLOG_VARIABLE_DECORATOR 
#endif

/** 
 * \brief Do we use inlined click timers 
 */
#ifndef ATRSHMLOG_INLINE_TSC_CODE
#define ATRSHMLOG_INLINE_TSC_CODE 1
#endif

/** 
 * \brief Do we inline the gettime 
 */
#ifndef ATRSHMLOG_INLINE_GETTIME
#define ATRSHMLOG_INLINE_GETTIME 1
#endif

/** The threading model */

/** 
 * \brief Do we use the pthread lib 
 * 
 * for mingw you can try this too. 
 * it uses the -pthread flag for the compiler and a dll
 * you have then clock_gettime and nanosleep too
 */
#ifndef ATRSHMLOG_USE_PTHREAD
#define ATRSHMLOG_USE_PTHREAD 0
#endif

#if ATRSHMLOG_PLATFORM_LINUX_X86_64_GCC == 1
# undef ATRSHMLOG_USE_PTHREAD
# define ATRSHMLOG_USE_PTHREAD 1
#endif

#if ATRSHMLOG_PLATFORM_CYGWIN_X86_64_GCC == 1
# undef ATRSHMLOG_USE_PTHREAD
# define ATRSHMLOG_USE_PTHREAD 1
#endif

#if ATRSHMLOG_PLATFORM_BSD_AMD64_CLANG == 1
# undef ATRSHMLOG_USE_PTHREAD
# define ATRSHMLOG_USE_PTHREAD 1
#endif

#if ATRSHMLOG_PLATFORM_BSD_AMD64_GCC == 1
# undef ATRSHMLOG_USE_PTHREAD
# define ATRSHMLOG_USE_PTHREAD 1
#endif

#if ATRSHMLOG_PLATFORM_SOLARIS_X86_64_GCC == 1
# undef ATRSHMLOG_USE_PTHREAD
# define ATRSHMLOG_USE_PTHREAD 1
#endif

/** 
 * \brief Do we use the windows thread lib 
 *
 * this is the mingw default
 */
#ifndef ATRSHMLOG_USE_WINTHREAD
#define ATRSHMLOG_USE_WINTHREAD 0
#endif

#if ATRSHMLOG_PLATFORM_MINGW_X86_64_GCC == 1
# if ATRSHMLOG_USE_PTHREAD == 1
# else
#  undef ATRSHMLOG_USE_WINTHREAD
#  define ATRSHMLOG_USE_WINTHREAD 1
# endif
#endif

/** 
 * \brief Do we use c11 threads - last resort
 */
#ifndef ATRSHMLOG_USE_C11_THREAD
#define ATRSHMLOG_USE_C11_THREAD 0
#endif

#if ATRSHMLOG_USE_PTHREAD == 0
# if ATRSHMLOG_USE_WINTHREAD == 0
#  undef ATRSHMLOG_USE_C11_THREAD
#  define ATRSHMLOG_USE_C11_THREAD 1
# endif
#endif

/** 
 * \brief Do we use the syscall interface 
 */
#ifndef ATRSHMLOG_SYSCALL
#define ATRSHMLOG_SYSCALL 0
#endif

#if ATRSHMLOG_PLATFORM_LINUX_X86_64_GCC == 1
# undef ATRSHMLOG_SYSCALL
# define ATRSHMLOG_SYSCALL 1
#endif

#if ATRSHMLOG_PLATFORM_BSD_AMD64_CLANG == 1
# undef ATRSHMLOG_SYSCALL
# define ATRSHMLOG_SYSCALL 1
#endif

#if ATRSHMLOG_PLATFORM_BSD_AMD64_GCC == 1
# undef ATRSHMLOG_SYSCALL
# define ATRSHMLOG_SYSCALL 1
#endif

/** 
 * \brief Do we use the clock_gettime here 
 */
#ifndef ATRSHMLOG_USE_CLOCK_GETTIME
#define ATRSHMLOG_USE_CLOCK_GETTIME 0
#endif

#if ATRSHMLOG_PLATFORM_LINUX_X86_64_GCC == 1
# undef ATRSHMLOG_USE_CLOCK_GETTIME
# define ATRSHMLOG_USE_CLOCK_GETTIME 1
#endif

#if ATRSHMLOG_PLATFORM_CYGWIN_X86_64_GCC == 1
# undef ATRSHMLOG_USE_CLOCK_GETTIME
# define ATRSHMLOG_USE_CLOCK_GETTIME 1
#endif

#if ATRSHMLOG_PLATFORM_MINGW_X86_64_GCC == 1
# if ATRSHMLOG_USE_PTHREAD == 1
#  undef ATRSHMLOG_USE_CLOCK_GETTIME
#  define ATRSHMLOG_USE_CLOCK_GETTIME 1
# endif
#endif

#if ATRSHMLOG_PLATFORM_BSD_AMD64_CLANG == 1
# undef ATRSHMLOG_USE_CLOCK_GETTIME
# define ATRSHMLOG_USE_CLOCK_GETTIME 1
#endif

#if ATRSHMLOG_PLATFORM_BSD_AMD64_GCC == 1
# undef ATRSHMLOG_USE_CLOCK_GETTIME
# define ATRSHMLOG_USE_CLOCK_GETTIME 1
#endif

#if ATRSHMLOG_PLATFORM_SOLARIS_X86_64_GCC == 1
# undef ATRSHMLOG_USE_CLOCK_GETTIME
# define ATRSHMLOG_USE_CLOCK_GETTIME 1
#endif

/** 
 * \brief Do we use the win filetime here 
 */
#ifndef ATRSHMLOG_USE_WIN_FILETIME
#define ATRSHMLOG_USE_WIN_FILETIME 0
#endif

#if ATRSHMLOG_PLATFORM_MINGW_X86_64_GCC == 1
# if ATRSHMLOG_USE_CLOCK_GETTIME == 1
# else
#  undef ATRSHMLOG_USE_WIN_FILETIME 
#  define ATRSHMLOG_USE_WIN_FILETIME 1
# endif
#endif

/**
 * \brief  Do we use gettimeofday as a last resort
 */
#ifndef ATRSHMLOG_USE_GETTIMEOFDAY
#define ATRSHMLOG_USE_GETTIMEOFDAY 0
#endif

#if ATRSHMLOG_USE_CLOCK_GETTIME == 0
# if ATRSHMLOG_USE_WIN_FILETIME == 0
#  undef ATRSHMLOG_USE_GETTIMEOFDAY
#  define ATRSHMLOG_USE_GETTIMEOFDAY 1
# endif
#endif

/** 
 * \brief We use the nanosleep on the platfrom, whatever it is
 *
 * if you use mingw and the pthread lib you can use this too
 * so then add the -pthread to the compiler switches and
 * have a nice time
 */
#ifndef ATRSHMLOG_USE_NANOSLEEP
#define ATRSHMLOG_USE_NANOSLEEP 0
#endif

#if ATRSHMLOG_PLATFORM_LINUX_X86_64_GCC == 1
# undef ATRSHMLOG_USE_NANOSLEEP
# define ATRSHMLOG_USE_NANOSLEEP 1
#endif

#if ATRSHMLOG_PLATFORM_CYGWIN_X86_64_GCC == 1
# undef ATRSHMLOG_USE_NANOSLEEP
# define ATRSHMLOG_USE_NANOSLEEP 1
#endif

#if ATRSHMLOG_PLATFORM_MINGW_X86_64_GCC == 1
# if ATRSHMLOG_USE_PTHREAD == 1
#  undef ATRSHMLOG_USE_NANOSLEEP
#  define ATRSHMLOG_USE_NANOSLEEP 1
# endif
#endif

#if ATRSHMLOG_PLATFORM_BSD_AMD64_CLANG == 1
# undef ATRSHMLOG_USE_NANOSLEEP
# define ATRSHMLOG_USE_NANOSLEEP 1
#endif

#if ATRSHMLOG_PLATFORM_BSD_AMD64_GCC == 1
# undef ATRSHMLOG_USE_NANOSLEEP
# define ATRSHMLOG_USE_NANOSLEEP 1
#endif

#if ATRSHMLOG_PLATFORM_SOLARIS_X86_64_GCC == 1
# undef ATRSHMLOG_USE_NANOSLEEP
# define ATRSHMLOG_USE_NANOSLEEP 1
#endif


/** 
 * \brief Do we use the syscall for linux to get TID 
 */
#ifndef ATRSHMLOG_USE_SYSCALL_TID
#define ATRSHMLOG_USE_SYSCALL_TID 0
#endif

#if ATRSHMLOG_PLATFORM_LINUX_X86_64_GCC == 1
# undef ATRSHMLOG_USE_SYSCALL_TID 
# define ATRSHMLOG_USE_SYSCALL_TID 1
#endif

/** 
 * \brief Do we use the pthread tid 
 */
#ifndef ATRSHMLOG_USE_PTHREAD_TID
#define ATRSHMLOG_USE_PTHREAD_TID 0
#endif

#if ATRSHMLOG_PLATFORM_CYGWIN_X86_64_GCC == 1
# undef ATRSHMLOG_USE_PTHREAD_TID
# define ATRSHMLOG_USE_PTHREAD_TID 1
#endif

/** 
 * \brief Do we use the winthread tid 
 */
#ifndef ATRSHMLOG_USE_WINTHREAD_TID
#define ATRSHMLOG_USE_WINTHREAD_TID 0
#endif

#if ATRSHMLOG_PLATFORM_MINGW_X86_64_GCC == 1
# undef ATRSHMLOG_USE_WINTHREAD_TID
# define ATRSHMLOG_USE_WINTHREAD_TID 1
#endif

/**
 * \brief We use the thr_self call of freebsd here
 */
#ifndef ATRSHMLOG_USE_THR_SELF_TID
#define ATRSHMLOG_USE_THR_SELF_TID 0
#endif

#if ATRSHMLOG_FLAVOUR == 3
# undef ATRSHMLOG_USE_THR_SELF_TID
# define ATRSHMLOG_USE_THR_SELF_TID 1
#endif

/**
 * \brief We use the getthrid call on openbsd here
 */
#ifndef ATRSHMLOG_USE_GETTHRID
#define ATRSHMLOG_USE_GETTHRID 0
#endif

#if ATRSHMLOG_FLAVOUR == 4
# undef ATRSHMLOG_USE_GETTHRID
# define ATRSHMLOG_USE_GETTHRID 1
#endif

/**
 * \brief We use the _lwp_self call on netbsd here
 */
#ifndef ATRSHMLOG_USE_LWP_SELF
#define ATRSHMLOG_USE_LWP_SELF 0
#endif

#if ATRSHMLOG_FLAVOUR == 5
# undef ATRSHMLOG_USE_LWP_SELF
# define ATRSHMLOG_USE_LWP_SELF 1
#endif

/**
 * \brief We use the thr_self call on opensolaris
 */
#ifndef ATRSHMLOG_USE_SOLARIS_THR_SELF
#define ATRSHMLOG_USE_SOLARIS_THR_SELF 0
#endif

#if ATRSHMLOG_FLAVOUR == 6
# undef ATRSHMLOG_USE_SOLARIS_THR_SELF 
# define ATRSHMLOG_USE_SOLARIS_THR_SELF 1
#endif


/**
 * \brief We have a system with thread loacls and the compiler support it
 */
#ifndef ATRSHMLOG_THREAD_LOCAL
#define ATRSHMLOG_THREAD_LOCAL 0
#endif

#if  ATRSHMLOG_PLATFORM_LINUX_X86_64_GCC == 1
# undef ATRSHMLOG_THREAD_LOCAL
# define ATRSHMLOG_THREAD_LOCAL 1
#endif

#if  ATRSHMLOG_PLATFORM_CYGWIN_X86_64_GCC == 1
# undef ATRSHMLOG_THREAD_LOCAL
# define ATRSHMLOG_THREAD_LOCAL 1
#endif

#if  ATRSHMLOG_PLATFORM_MINGW_X86_64_GCC == 1
# undef ATRSHMLOG_THREAD_LOCAL
# define ATRSHMLOG_THREAD_LOCAL 1
#endif

#if ATRSHMLOG_PLATFORM_BSD_AMD64_CLANG == 1
// freebsd has it
# if ATRSHMLOG_FLAVOUR == 3
#  undef ATRSHMLOG_THREAD_LOCAL
#  define ATRSHMLOG_THREAD_LOCAL 1
# endif
#endif

#if ATRSHMLOG_PLATFORM_BSD_AMD64_GCC == 1
// netbsd has it
# undef ATRSHMLOG_THREAD_LOCAL
# define ATRSHMLOG_THREAD_LOCAL 1
#endif

#if ATRSHMLOG_PLATFORM_SOLARIS_X86_64_GCC == 1
# undef ATRSHMLOG_THREAD_LOCAL
# define ATRSHMLOG_THREAD_LOCAL 1
#endif


/**
 * \brief We have safer string functions
 *
 * BSD unix like a way of copy for strings that they believe is safer
 * than the old one. 
 * Perhaps we will use it on fenster;plural too.
 */
#ifndef ATRSHMLOG_USE_SAFER_COPY
#define ATRSHMLOG_USE_SAFER_COPY 0
#endif

#if ATRSHMLOG_PLATFORM_BSD_AMD64_CLANG == 1 || ATRSHMLOG_PLATFORM_BSD_AMD64_GCC == 1 || ATRSHMLOG_PLATFORM_SOLARIS_X86_64_GCC == 1
# if ATRSHMLOG_FLAVOUR == 3 || ATRSHMLOG_FLAVOUR == 4 || ATRSHMLOG_FLAVOUR == 5 || ATRSHMLOG_FLAVOUR == 6
#  undef ATRSHMLOG_USE_SAFER_COPY
#  define ATRSHMLOG_USE_SAFER_COPY 1
# endif
#endif

/**
 * \brief We have to know the platforms order of bytes
 * 
 * So we have today only the fact that all systems use
 * the DEC order - which is also intel order - which is not
 * the network order.
 * The network order is the IBM order - which was motorola in the old days
 *
 * If you are on a platform that make it different 
 * add your if blblplatform == 1 , undef , define 1 , endif after it.
 */ 
#ifndef ATRSHMLOG_H_ORDER_IS_N_ORDER
#define ATRSHMLOG_H_ORDER_IS_N_ORDER 0
#endif

#if  ATRSHMLOG_PLATFORM_LINUX_X86_64_GCC == 1
// nothing to change
#endif

#if  ATRSHMLOG_PLATFORM_CYGWIN_X86_64_GCC == 1
// nothing to change
#endif

#if  ATRSHMLOG_PLATFORM_MINGW_X86_64_GCC == 1
// nothing to change
#endif

#if ATRSHMLOG_PLATFORM_BSD_AMD64_CLANG == 1
// nothing to change
#endif

#if ATRSHMLOG_PLATFORM_BSD_AMD64_GCC == 1
// nothing to change
#endif

#if ATRSHMLOG_PLATFORM_SOLARIS_X86_64_GCC == 1
// nothing to change
#endif

/**
 * We switch the fence in write off if we are in intel land
 */
#ifndef ATRSHMLOG_FENCE_2_OFF
#define ATRSHMLOG_FENCE_2_OFF 0
#endif

#if  ATRSHMLOG_PLATFORM_LINUX_X86_64_GCC == 1

#undef ATRSHMLOG_FENCE_2_OFF
#define ATRSHMLOG_FENCE_2_OFF 1

#endif

#if  ATRSHMLOG_PLATFORM_CYGWIN_X86_64_GCC == 1

#undef ATRSHMLOG_FENCE_2_OFF
#define ATRSHMLOG_FENCE_2_OFF 1

#endif

#if  ATRSHMLOG_PLATFORM_MINGW_X86_64_GCC == 1

#undef ATRSHMLOG_FENCE_2_OFF
#define ATRSHMLOG_FENCE_2_OFF 1

#endif

#if ATRSHMLOG_PLATFORM_BSD_AMD64_CLANG == 1

#undef ATRSHMLOG_FENCE_2_OFF
#define ATRSHMLOG_FENCE_2_OFF 1

#endif

#if ATRSHMLOG_PLATFORM_BSD_AMD64_GCC == 1

#undef ATRSHMLOG_FENCE_2_OFF
#define ATRSHMLOG_FENCE_2_OFF 1

#endif

#if ATRSHMLOG_PLATFORM_SOLARIS_X86_64_GCC == 1

#undef ATRSHMLOG_FENCE_2_OFF
#define ATRSHMLOG_FENCE_2_OFF 1

#endif

// candidates for the platform tsc call function
// #define ATRSHMLOG_GET_TSC_CALL atrshmlog_get_tsc_par_x86_64_gnu
// #define ATRSHMLOG_GET_TSC_CALL atrshmlog_get_tsc_fence_x86_64_gnu
// #define ATRSHMLOG_GET_TSC_CALL atrshmlog_get_tsc_x86_64_gnu
// #define ATRSHMLOG_GET_TSC_CALL atrshmlog_get_clicktime


/** 
 * \brief We use what low level for click time.
 *
 * This will be replaced by get_clicktime eventually.
 */
#ifndef ATRSHMLOG_GET_TSC_CALL
#define ATRSHMLOG_GET_TSC_CALL atrshmlog_get_clicktime
#endif

#if ATRSHMLOG_PLATFORM_LINUX_X86_64_GCC == 1
# undef ATRSHMLOG_GET_TSC_CALL
# define  ATRSHMLOG_GET_TSC_CALL atrshmlog_get_tsc_x86_64_gnu
#endif

#if ATRSHMLOG_PLATFORM_CYGWIN_X86_64_GCC == 1
# undef ATRSHMLOG_GET_TSC_CALL
# define  ATRSHMLOG_GET_TSC_CALL atrshmlog_get_tsc_x86_64_gnu
#endif

#if ATRSHMLOG_PLATFORM_MINGW_X86_64_GCC == 1
# undef ATRSHMLOG_GET_TSC_CALL
# define  ATRSHMLOG_GET_TSC_CALL atrshmlog_get_tsc_x86_64_gnu
#endif


#if ATRSHMLOG_PLATFORM_BSD_AMD64_CLANG == 1
# undef ATRSHMLOG_GET_TSC_CALL
# define  ATRSHMLOG_GET_TSC_CALL atrshmlog_get_tsc_amd64_clang
#endif

#if ATRSHMLOG_PLATFORM_BSD_AMD64_GCC == 1
# undef ATRSHMLOG_GET_TSC_CALL
# define  ATRSHMLOG_GET_TSC_CALL atrshmlog_get_tsc_amd64_gnu
#endif

#if ATRSHMLOG_PLATFORM_SOLARIS_X86_64_GCC == 1
# undef ATRSHMLOG_GET_TSC_CALL
# define  ATRSHMLOG_GET_TSC_CALL atrshmlog_get_tsc_x86_64_gnu
#endif

/********************************************************************/

/* start of inludes */

/* 
 * We need the 64 bit unsigned type here.
 * So this is the include to deliver it.
 * Dont try do do it without a include ...
 */
#include <stdint.h>


/* end of includes */

/***************************************************************/

#ifdef __cplusplus
extern "C" { 
#endif

  /* start typedefs */
  
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
  struct atrshmlog_internal_time_s
  {
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

  /* end typedefs */
  
  /*******************************************************************/

  #ifdef SWIG
  %immutable;
  #endif
  
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
  ATRSHMLOG_VARIABLE_DECORATOR extern
  volatile const atrshmlog_event_t*  atrshmlog_event_locks;

#ifdef SWIG
%mutable;
#endif
  

  
#ifdef SWIG
%immutable;
#endif
  
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
  ATRSHMLOG_VARIABLE_DECORATOR extern
  volatile atrshmlog_int32_t atrshmlog_logging_process;

#ifdef SWIG
%mutable;
#endif
  

#ifdef SWIG
%immutable;
#endif
  
    /** 
   * \brief The clock id for the get clicktime. 
   *
   * We need to expose it 
   * for the inlining thing.
   * See for use \ref atrshmlog_set_clock_id()
   */
  ATRSHMLOG_VARIABLE_DECORATOR extern
  volatile int atrshmlog_clock_id;
  
#ifdef SWIG
%mutable;
#endif
  

  /************************************************************/

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


  /*
   * Helper for write calls
   */

  enum atrshmlog_write_mode {
    /**
     * \brief This is the first constant for the point in time event
     */
    ATRSHMLOG_POINT_IN_TIME_C = (int)ATRSHMLOGPOINTINTIMEP,

    /**
     * \brief This is the second constant for the point in time event
     */
    ATRSHMLOG_POINT_IN_TIME_UCS2 =  (int)ATRSHMLOGPOINTINTIMEp,

    
    /**
     * \brief This is the first constant for the interval in time event
     */
    ATRSHMLOG_INTERVAL_IN_TIME_C = (int)ATRSHMLOGPOINTINTIMEI,

    /**
     * \brief This is the second constant for the interval in time event
     */
    ATRSHMLOG_INTERVAL_IN_TIME_UCS2 = (int)ATRSHMLOGPOINTINTIMEi
  };


  /************************************************************************/

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
    /**< The number of calls to \ref atrshmlog_write1() */
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
    /**< The number of calls to \ref atrshmlog_write2() */
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
    atrshmlog_counter_set_buffer_size              = 54,
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
    atrshmlog_counter_set_env_prefix            = 78,
    /**< The number of calls to \ref atrshmlog_set_event_off() */
    atrshmlog_counter_exit_cleanup              = 79,
    /**< The number of calls to \ref atrshmlog_exit_cleanup() */
    atrshmlog_counter_flush                     = 80,
    /**< The number of calls to \ref atrshmlog_flush() */
    atrshmlog_counter_logging_process_off_final = 81,
    /**< The number of calls to \ref atrshmlog_set_logging_process_off_final() */
    atrshmlog_counter_turn_logging_off          = 82,
    /**< The number of calls to \ref  atrshmlog_turn_logging_off() */
    atrshmlog_counter_init_in_advance_on        = 83,
    /**< The number of calls to \ref  atrshmlog_init_buffers_in_advance_on() */
    atrshmlog_counter_init_in_advance_off       = 84,
    /**< The number of calls to \ref  atrshmlog_init_buffers_in_advance_off() */
    atrshmlog_counter_reuse_thread_buffers      = 85,
    /**< The number of calls to \ref  atrshmlog_reuse_thread_buffers() */
    atrshmlog_counter_set_autoflush             = 86,
    /**< The number of calls to \ref  atrshmlog_set_autoflush() */
    atrshmlog_counter_fence_alarm_1             = 87,
    /**< The number of checksum errors in tranfer mem to shm part 1 */
    atrshmlog_counter_fence_alarm_2             = 88,
    /**< The number of checksum errors in tranfer shm to mem part 2 */
    atrshmlog_counter_detach                    = 89,
    /**< The number of calls to \ref  atrshmlog_detach() */
    atrshmlog_counter_reattach                  = 90,
    /**< The number of calls to \ref  atrshmlog_reattach() */
    atrshmlog_counter_end                       = 90
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
    /**< Operation was successful, no error. */

    atrshmlog_error_error = -1,
    /**< Generic error 1 happend. */

    atrshmlog_error_error2 = -2,
    /**< Generic error 2 happend. */

    atrshmlog_error_error3 = -3,
    /**< Generic error 3 happend. */
    
    atrshmlog_error_error4 = -4,
    /**< Generic error 4 happend. */
    
    atrshmlog_error_error5 = -5,
    /**< Generic error 5 happend. */
    
    atrshmlog_error_connect_1 = -11,
    /**< Buffer list is NULL in atrshmlog_il_connect_buffers_list.  */
    
    atrshmlog_error_connect_2 = -12,
    /**< Buffer count is negative in atrshmlog_il_connect_buffers_list.  */
    
    atrshmlog_error_init_thread_local_1 = -21,
    /**< The init was not successful in atrshmlog_init_thread_local. */
    
    atrshmlog_error_mem_to_shm_1 = -31,
    /**< The shared memory is not connected in atrshmlog_transfer_mem_to_shm. */
    
    atrshmlog_error_mem_to_shm_2 = -32,
    /**< The transfer buffer is NULL in atrshmlog_transfer_mem_to_shm. */
    
    atrshmlog_error_mem_to_shm_3 = 31,
    /**< The size is 0, no operation in atrshmlog_transfer_mem_to_shm. */

    atrshmlog_error_mem_to_shm_4 = -33,
    /**< The maximum size is exceeded in atrshmlog_transfer_mem_to_shm for the buffer itself. */
    
    atrshmlog_error_mem_to_shm_5 = -34,
    /**< The logging was off for the thread in atrshmlog_transfer_mem_to_shm. */
    
    atrshmlog_error_mem_to_shm_6 = -35,
    /**< The safeguard was corrupt in atrshmlog_transfer_mem_to_shm. */
    
    atrshmlog_error_mem_to_shm_7 = 32,
    /**< The logging was off for the system in area for atrshmlog_transfer_mem_to_shm. */
    
    atrshmlog_error_mem_to_shm_8 = 33,
    /**< The logging was final off in atrshmlog_transfer_mem_to_shm. */
    
    atrshmlog_error_attach_1 = 41,
    /**< The attach was already done in atrshmlog_attach. */
    
    atrshmlog_error_attach_2 = -41,
    /**< Could not find a valid environment or file approch in atrshmlog_attach. */
    
    atrshmlog_error_attach_3 = -42,
    /**< The cleanup could not sucessful made atexit in atrshmlog_attach. */

    atrshmlog_error_attach_4 = -43,
    /**< The flagfile seems to be corrupt in atrshmlog_attach. */
    
    atrshmlog_error_attach_5 = -44,
    /**< No flagfile option for this level in atrshmlog_attach. */
    
    atrshmlog_error_attach_6 = -45,
    /**< The cleanup could not sucessful made atexit in atrshmlog_attach.  */

    atrshmlog_error_init_in_write_1 = -51,
    /**< Buffer allocation failure in atrshmlog_init_in_write. */
    
    atrshmlog_error_write0_1 = -61,
    /**< Eventnumber negative in atrshmlog_write0. */
    
    atrshmlog_error_write0_2 = -62,
    /**< Eventnumber too big in atrshmlog_write0. */
    
    atrshmlog_error_write0_3 = -63,
    /**< Logging off for the thread in atrshmlog_write0.  */
    
    atrshmlog_error_write0_4 = 61,
    /**< Buffer full discard in atrshmlog_write0. */
    
    atrshmlog_error_write0_5 = 62,
    /**< Logging off final in atrshmlog_write0.  */
    
    atrshmlog_error_write0_6 = 63,
    /**< Logging off in area in atrshmlog_write0. */

    atrshmlog_error_write0_7 = -64,
    /**< Safeguard corrupt in atrshmlog_write0.  */
    
    atrshmlog_error_write0_8 = -65,
    /**< Area safeguard corrupt in atrshmlog_write0.      */
    
    atrshmlog_error_write0_9 = 64,
    /**< Logging off in area  atrshmlog_write0.  */

    atrshmlog_error_write1_1 = -71,
    /**< Size payload is negative in atrshmlog_write1.  */
    
    atrshmlog_error_write1_2 = -72,
    /**< Eventnumber negative in atrshmlog_write1. */
    
    atrshmlog_error_write1_3 = -73,
    /**< Eventnumber too big in atrshmlog_write1.  */
    
    atrshmlog_error_write1_4 = -74,
    /**< Payload too big for logging in atrshmlog_write1.  */
    
    atrshmlog_error_write1_5 = -75,
    /**< Logging off for the thread in atrshmlog_write1.  */
    
    atrshmlog_error_write1_6 = 71,
    /**< Buffer full discard in  atrshmlog_write1.  */
    
    atrshmlog_error_write1_7 = 72,
    /**< Logging off final in atrshmlog_write1. */
    
    atrshmlog_error_write1_8 = 73,
    /**< Logging off in area in atrshmlog_write1. */
    
    atrshmlog_error_write1_9 = -76,
    /**< Payload too big for logging in atrshmlog_write1. */
    
    atrshmlog_error_write1_10 = -77,
    /**< Safeguard corrupt in atrshmlog_write1. */
    
    atrshmlog_error_write1_11 = -78,
    /**< Area safeguard corrupt in atrshmlog_write1.  */
    
    atrshmlog_error_write1_12 = 74,
    /**< Logging off in area in atrshmlog_write1.  */
    
    atrshmlog_error_write2_1 = -81,
    /**< Size payload is negative in atrshmlog_write2.  */
    
    atrshmlog_error_write2_2 = -82,
    /**< Eventnumber negative in atrshmlog_write2.  */
    
    atrshmlog_error_write2_3 = -83,
    /**< Eventnumber too big in atrshmlog_write2.  */
    
    atrshmlog_error_write2_4 = -84,
    /**< Payload too big for logging in atrshmlog_write2. */
    
    atrshmlog_error_write2_5 = -85,
    /**< Logging off for the thread in atrshmlog_write2. */
    
    atrshmlog_error_write2_6 = 81,
    /**< Buffer full discard in atrshmlog_write2. */
    
    atrshmlog_error_write2_7 = 82,
    /**< Logging off final in  atrshmlog_write2.  */
    
    atrshmlog_error_write2_8 = 83,
    /**< Logging off in area in  atrshmlog_write2. */
    
    atrshmlog_error_write2_9 = -86,
    /**< Payload too big for logging in atrshmlog_write2.  */
    
    atrshmlog_error_write2_10 = -87,
    /**< Safeguard corrupt in atrshmlog_write2.  */
    
    atrshmlog_error_write2_11 = -88,
    /**< Area safeguard corrupt in atrshmlog_write2.  */
    
    atrshmlog_error_write2_12 = 84,
    /**< Logging off in area in  atrshmlog_write2. */
    
    atrshmlog_error_area_version_1 = -91,
    /**< Area not connected in atrshmlog_get_area_version.  */
    
    atrshmlog_error_area_count_1 = -101,
    /**< Area not connected in atrshmlog_get_area_count. */
    
    atrshmlog_error_area_ich_habe_fertig_1 = -111,
    /**< Area not connected in atrshmlog_set_area_ich_habe_fertig. */
    
    atrshmlog_error_get_event_1 = -121,
    /**< Index out of range in atrshmlog_get_event. */
    
    atrshmlog_error_get_logging_1 = 131,
    /**< Area not connected in atrshmlog_get_logging.  */
    
    atrshmlog_error_get_logging_2 = 132,
    /**< Area logging flag off in atrshmlog_get_logging.  */
    
    atrshmlog_error_get_logging_3 = 133,
    /**< Logging final off in atrshmlog_get_logging.  */
    
    atrshmlog_error_get_logging_4 = 134,
    /**< Process logging off in atrshmlog_get_logging.  */
    
    atrshmlog_error_create_1 = -141,
    /**< Ipc key out of range in atrshmlog_create.  */
    
    atrshmlog_error_create_2 = -142,
    /**< Buffer count to low in  atrshmlog_create.  */
    
    atrshmlog_error_create_3 = -143,
    /**< Buffer count to high in  atrshmlog_create. */
    
    atrshmlog_error_create_4 = -144,
    /**< Connect failed in low level call in atrshmlog_create.  */
    
    atrshmlog_error_init_shm_1 = -151,
    /**< Area is NULL in atrshmlog_init_shm_log.  */
    
    atrshmlog_error_init_shm_2 = -152,
    /**< Shared memory id diffrent for process and area in atrshmlog_init_shm_log. */
    
    atrshmlog_error_init_shm_3 = -153,
    /**< Low level init failed in atrshmlog_init_shm_log. */

    atrshmlog_error_read_1 = -161,
    /**< Area is NULL in atrshmlog_read. */
    
    atrshmlog_error_read_2 = -162,
    /**< Buffer index negative in atrshmlog_read.  */
    
    atrshmlog_error_read_3 = -163,
    /**< Buffer index too big in atrshmlog_read.  */
    
    atrshmlog_error_read_4 = -164,
    /**< Area safeguard corrupt in atrshmlog_read.  */
    
    atrshmlog_error_read_5 = -165,
    /**< Buffer size in shared memory too big in atrshmlog_read.  */
    
    atrshmlog_error_read_fetch_1 = -171,
    /**< Area is NULL in atrshmlog_read_fetch.  */
    
    atrshmlog_error_read_fetch_2 = -172,
    /**< Area safeguard corrupt in atrshmlog_read_fetch. */
    
    atrshmlog_error_read_fetch_3 = 171,
    /**< No buffer to be processed in atrshmlog_read_fetch.  */
    
    atrshmlog_error_read_fetch_4 = -173,
    /**< Buffer size in shared memory too big in atrshmlog_read_fetch. */
    
    atrshmlog_error_read_fetch_5 = 172,
    /**< Buffer has size 0 in shared memory too big in atrshmlog_read_fetch. */
    
    atrshmlog_error_verify_1 = -181,
    /**< Area not attached in atrshmlog_verify. */
    
    atrshmlog_error_verify_2 = -182,
    /**< Shared memory id differs to process shared memory id in atrshmlog_verify.  */
    
    atrshmlog_error_verify_3 = -183,
    /**< Area safeguard corrupted in atrshmlog_verify.  */
    
    atrshmlog_error_verify_4 = -184,
    /**< Version mismatch for area and module code in atrshmlog_verify.  */
    
    atrshmlog_error_verify_5 = -185,
    /**< Area buffer state corrupt in atrshmlog_verify. */
    
    atrshmlog_error_verify_6 = -186,
    /**< Buffer safeguard corrupted in atrshmlog_verify.      */

    atrshmlog_error_buffer_slave_1 = -190,
    /**< Pthread specific buffer not available.      */
    
    atrshmlog_error_get_strategy_1 = -191,
    /**< Pthread specific buffer not available.      */

    atrshmlog_error_read_6 = -166,
    /**< Pthread specific buffer not available.      */
    
    atrshmlog_error_read_fetch_6 = -174,
    /**< Pthread specific buffer not available.      */
    
    atrshmlog_error_set_strategy_1 = -192,
    /**< Pthread specific buffer not available.      */

    atrshmlog_error_write0_10 = -66,
    /**< Pthread specific buffer not available.      */

    atrshmlog_error_write1_13 = -79,
    /**< Pthread specific buffer not available.      */

    atrshmlog_error_write2_13 = -89,
    /**< Pthread specific buffer not available.      */

    atrshmlog_error_attach_7 = -46,
    /**< Pthread specific buffer not available.      */

    atrshmlog_error_get_autoflush_1 = -200,
    /**< Pthread specific buffer not available.      */

    atrshmlog_error_set_autoflush_1 = -201,
    /**< Pthread specific buffer not available.      */

    atrshmlog_error_reattach_1 = -210,
    /**< No reattach possible without a successful attach first.      */

    atrshmlog_error_reattach_2 = -211,
    /**< Reattach not successful for shm op          */

    atrshmlog_error_reattach_3 = 212,
    /**< Reattach or attach already done             */

    atrshmlog_error_reattach_4 = -213
    /**< Parameter array null not allowed            */
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

  /************************************************************************/

  /* start function macros */ 

  /* burocracy functions */

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

  /************************************************************************/
  /* write log related functions */
  
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
   * we use a binary and her, for real ...
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
   *  \brief The autoflush flag
   *
   * \return
   * The flag
   */
#define ATRSHMLOG_GET_AUTOFLUSH_PROCESS() atrshmlog_get_autoflush_process()
  
  /**
   * \brief Set the autoflush for the process
   *
   * \param __f
   * Our new autoflush flag
   *
   * \return 
   * The old flag
   */
#define ATRSHMLOG_SET_AUTOFLUSH_PROCESS(__f)  atrshmlog_set_autoflush_process((__f))

  /**
   *  \brief The autoflush flag
   *
   * \return
   * The flag
   */
#define ATRSHMLOG_GET_AUTOFLUSH() atrshmlog_get_autoflush()

  /**
   * \brief Set the autoflush for the thread
   *
   * \param __f
   * Our new autoflush flag
   *
   * \return 
   * The old flag
   */
#define ATRSHMLOG_SET_AUTOFLUSH(__f) atrshmlog_set_autoflush((__f))


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

  /************************************************************************/
  /* timing functions */
  
  /**
   * \brief We wait nanoseconds.
   * 
   * \param __n
   * The nanosecs till 2^31.
   *
   * \return
   * void
   */
#define ATRSHMLOG_SLEEP_NANOS(__n)   atrshmlog_sleep_nanos((__n))

  /** 
   * \brief Get the real time with clicktime.
   *
   * \return
   * the 64 bit click time 
   */
#define ATRSHMLOG_GET_CLICKTIME() atrshmlog_get_clicktime()

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
   * \brief  We get the real time
   *
   * \return
   * The real time.
   */
#define ATRSHMLOG_GET_REALTIME() atrshmlog_get_realtime()

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
#define ATRSHMLOG_GET_INITTIME_TSC_BEFORE()  atrshmlog_get_inittime_tsc_before()

  /** 
   * \brief Get the click time after inittime.
   *
   * \return
   * The time for the click after the get clock call to fill inittime.
   */
#define ATRSHMLOG_GET_INITTIME_TSC_AFTER()   atrshmlog_get_inittime_tsc_after()

  /************************************************************************/
  /* initialization and configuration functions */
  
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
   * \brief We disconnect to the shm buffer.
   * 
   * We do no cleanup.
   *
   * So if we attach again at another area
   * we deliver to that area.
   *
   * We do not log after we are detached
   *
   * \return
   * - Zero ok
   * - -1 for error
   */
#define ATRSHMLOG_DETACH() atrshmlog_detach()

  /** 
   * \brief We reattach to a shm buffer
   * 
   * We set the values and do most of the attach
   *
   * We can set the values, but we do NOT reinit buffers.
   * We do NOT restart slaves.
   * We DO resize the event locks array.
   *
   * \param __p
   * int array with 56 values.
   * - 0 : flag for use id
   * - 1 : new value for id
   * - 2 : flag for use count
   * - 3 : new value for count
   * - 4 : flag for use atrshmlog_init_buffers_in_advance
   * - 5 : new value for atrshmlog_init_buffers_in_advance
   * - 6 : flag for use atrshmlog_buffer_strategy
   * - 7 : new value for atrshmlog_buffer_strategy
   * - 8 : flag for use atrshmlog_strategy_wait_wait_time
   * - 9 : new value for atrshmlog_strategy_wait_wait_time
   * - 10 : flag for use atrshmlog_delimiter
   * - 11 : new value for atrshmlog_delimiter
   * - 12 : flag for use atrshmlog_event_locks_max
   * - 13 : new value for atrshmlog_event_locks_max
   * - 14 : flag for use atrshmlog_buffer_infosize
   * - 15 : new value for atrshmlog_buffer_infosize
   * - 16 : flag for use atrshmlog_prealloc_buffer_count
   * - 17 : new value for atrshmlog_prealloc_buffer_count
   * - 18 : flag for use atrshmlog_f_list_buffer_slave_wait
   * - 19 : new value for atrshmlog_f_list_buffer_slave_wait
   * - 20 : flag for use atrshmlog_f_list_buffer_slave_count
   * - 21 : new value for atrshmlog_f_list_buffer_slave_count
   * - 22 : flag for use atrshmlog_wait_for_slaves
   * - 23 : new value for atrshmlog_wait_for_slaves
   * - 24 : flag for use atrshmlog_clock_id
   * - 25 : new value for atrshmlog_clock_id
   * - 26 : flag for use atrshmlog_thread_fence_1
   * - 27 : new value for atrshmlog_thread_fence_1
   * - 28 : flag for use atrshmlog_thread_fence_2
   * - 29 : new value for atrshmlog_thread_fence_2
   * - 30 : flag for use atrshmlog_thread_fence_3
   * - 31 : new value for atrshmlog_thread_fence_3
   * - 32 : flag for use atrshmlog_thread_fence_4
   * - 33 : new value for atrshmlog_thread_fence_4
   * - 34 : flag for use atrshmlog_thread_fence_5
   * - 35 : new value for atrshmlog_thread_fence_5
   * - 36 : flag for use atrshmlog_thread_fence_6
   * - 37 : new value for atrshmlog_thread_fence_6
   * - 38 : flag for use atrshmlog_thread_fence_7
   * - 39 : new value for atrshmlog_thread_fence_7
   * - 40 : flag for use atrshmlog_thread_fence_8
   * - 41 : new value for atrshmlog_thread_fence_8
   * - 42 : flag for use atrshmlog_thread_fence_9
   * - 43 : new value for atrshmlog_thread_fence_9
   * - 44 : flag for use atrshmlog_thread_fence_10
   * - 45 : new value for atrshmlog_thread_fence_10
   * - 46 : flag for use atrshmlog_thread_fence_11
   * - 47 : new value for atrshmlog_thread_fence_11
   * - 48 : flag for use atrshmlog_thread_fence_12
   * - 49 : new value for atrshmlog_thread_fence_12
   * - 50 : flag for use atrshmlog_thread_fence_13
   * - 51 : new value for atrshmlog_thread_fence_13
   * - 52 : flag for use atrshmlog_checksum
   * - 53 : new value for atrshmlog_checksum
   * - 54 : flag for use logging process off
   * - 55 : new value for logging process off
   * - 56 : flag for use targetbuffer max
   * - 57 : new value for targetbuffer max 
   * 
   * \return
   * - Zero ok
   * - negative for error
   * - positiv for minor problem
   */
#define ATRSHMLOG_REATTACH(__p) atrshmlog_reattach((__p))


  /** 
   * \brief Get the prefix for variables and flagfiles.
   *
   * \return 
   * Points to the used prefix.
   */
#define ATRSHMLOG_GET_ENV_PREFIX() atrshmlog_get_env_prefix()

  /** 
   * \brief Set the prefix for use of variables and flagfiles.
   *
   * \param __p
   * Start of the new prefix.
   *
   * \return
   * void
   */
#define ATRSHMLOG_SET_ENV_PREFIX(__p) atrshmlog_set_env_prefix((__p))

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
#define ATRSHMLOG_GET_ENV_SHMID() atrshmlog_get_env_shmid()

  /** 
   * \brief We get the suffix for the shmid environment variable.
   *
   * \return
   * Points to the c string.
   */
#define ATRSHMLOG_GET_ENV_ID_SUFFIX()  atrshmlog_get_env_id_suffix()

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
   * \brief Get the shmid code.
   *
   * \return
   * Number of the shared memory id.
   */
#define ATRSHMLOG_GET_SHMID()  atrshmlog_get_shmid()

  /** 
   * \brief Get the maximum size of logbuffer.
   *
   * \return
   * The number of the maximum size of log buffers.
   */
#define ATRSHMLOG_GET_BUFFER_MAX_SIZE() atrshmlog_get_buffer_max_size()

  /** 
   * \brief Get the actual size of logbuffer in process.
   *
   * \return
   * Number of the actual size for log buffers.
   */
#define ATRSHMLOG_GET_BUFFER_SIZE() atrshmlog_get_buffer_size()

  /** 
   * \brief Set the actual size for log buffer in process.
   *
   * \param __size
   * The new number f bytes for log buffers.
   *
   * \return
   * The old number of bytes fro log buffers.
   */
#define ATRSHMLOG_SET_BUFFER_SIZE(__size) atrshmlog_set_buffer_size((__size))

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
   * \brief Get the buffer id.
   *
   * \return
   * Get the id of the last buffer initialized.
   */
#define ATRSHMLOG_GET_BUFFER_ID()   atrshmlog_get_buffer_id()

  /**
   * \brief we get the init in advance flag
   *
   * \return
   * The flag
   */
#define ATRSHMLOG_GET_INIT_BUFFERS_IN_ADVANCE() atrshmlog_get_init_buffers_in_advance()


  /**
   * \brief we turn the init in advance on
   *
   * \return
   * The old flag
   */
#define ATRSHMLOG_SET_INIT_BUFFERS_IN_ADVANCE_ON() atrshmlog_set_init_buffers_in_advance_on()
  
  /**
   * \brief we turn the init in advance off
   *
   * \return
   * The old flag
   */
#define ATRSHMLOG_SET_INIT_BUFFERS_IN_ADVANCE_OFF() atrshmlog_set_init_buffers_in_advance_off()


  /************************************************************************/
  /* thread related functions */
  
  /**
   * \brief We get the thread tid 
   *
   * \return 
   * The thread tid
   */
#define ATRSHMLOG_GET_TID()  atrshmlog_get_tid()

  /**
   * \brief We get the thread locals adress of a thread
   *
   * \return
   * the adress of the thread locals
   */
#define ATRSHMLOG_GET_THREAD_LOCALS_ADRESS()  atrshmlog_get_thread_locals_adress()

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
   * \brief We get the strategy for this thread
   *
   * \return
   * The value 
   */
#define ATRSHMLOG_GET_STRATEGY() atrshmlog_get_strategy()

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
   * \brief We get the strategy for the process
   *
   * \return
   * The value 
   */
#define ATRSHMLOG_GET_STRATEGY_PROCESS()   atrshmlog_get_strategy_process()
  
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
   * \brief Get the wait time in nanoseconds for the strategy wait
   *
   * \return
   * The number of nanos
   */
#define ATRSHMLOG_GET_STRATEGY_WAIT_WAIT_TIME() atrshmlog_get_strategy_wait_wait_time()

  /**
   * \brief Set the wait time for the wait strategy
   *
   * \param __wait_nanos
   * The number of nanos 
   *
   * \return
   * The old number of nanos the slave had to wait
   */
#define ATRSHMLOG_SET_STRATEGY_WAIT_WAIT_TIME(__wait_nanos) atrshmlog_set_strategy_wait_wait_time((__wait_nanos))

  /** 
   * \brief We get the tid of a thread local
   *
   * \param __tl
   * Pointer to a thread local or NULL
   *
   * \return
   * - 0 if pointer is NULL
   * - tid
   */
#define ATRSHMLOG_GET_THREAD_LOCAL_TID(__tl) atrshmlog_get_thread_local_tid ((__tl))
  

  /** 
   * \brief We get the pid of a thread local
   *
   * \param __thread_local
   * Pointer to a thread local or NULL
   *
   * \return
   * - 0 if pointer is NULL
   * - pid
   */
#define ATRSHMLOG_GET_THREAD_LOCAL_PID(__thread_local) atrshmlog_get_thread_local_pid ((__thread_local))

  /** 
   * \brief We get the buffer index of a thread local
   *
   * \param __thread_local
   * Pointer to a thread local or NULL
   *
   * \return
   * - 0 if pointer is NULL
   * - index
   */
#define ATRSHMLOG_GET_THREAD_LOCAL_INDEX(__thread_local) atrshmlog_get_thread_local_index ((__thread_local))

  /** 
   * \brief We get the buffer count of a thread local
   *
   * \param __thread_local
   * Pointer to a thread local or NULL
   *
   * \return
   * - 0 if pointer is NULL
   * - count
   */
#define ATRSHMLOG_GET_THREAD_LOCAL_COUNT(__thread_local) atrshmlog_get_thread_local_count ((__thread_local))

  /** 
   * \brief We get the buffer adress via index of a thread local
   *
   * \param __thread_local
   * Pointer to a thread local or NULL
   * \param __index
   * Index in the buffer pointer array
   *
   * \return
   * - 0 if pointer is NULL
   * - buffer pointer
   */
#define ATRSHMLOG_GET_THREAD_LOCAL_BUFFER(__thread_local,__index) atrshmlog_get_thread_local_buffer ((__thread_local),(__index))

  /**
   * \brief We switch the thread off and dispatch its buffers
   *
   * \param __a
   * The adress of the thread locals
   *
   * \return 
   * void
   */
#define ATRSHMLOG_TURN_LOGGING_OFF(__tl)  atrshmlog_turn_logging_off((__tl))

  /** 
   * \brief We make reuse of buffers of a dead thread
   *
   * \param __tid
   * Our tid for the thread
   *
   * \return
   * The number of found buffers
   */
#define ATRSHMLOG_REUSE_THREAD_BUFFERS(__tid)  atrshmlog_reuse_thread_buffers((__tid))


  /**
   * \brief We get a fence on or off flag
   *
   * \return
   * The old flag for fence.
   */
#define ATRSHMLOG_GET_THREAD_FENCE_1()  atrshmlog_get_thread_fence_1()
  
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
   * \brief We get a fence on or off flag
   *
   * \return
   * The old flag for fence.
   */
#define ATRSHMLOG_GET_THREAD_FENCE_2()  atrshmlog_get_thread_fence_2()
  
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
   * \brief We get a fence on or off flag
   *
   * \return
   * The old flag for fence.
   */
#define ATRSHMLOG_GET_THREAD_FENCE_3()  atrshmlog_get_thread_fence_3()
  
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
   * \brief We get a fence on or off flag
   *
   * \return
   * The old flag for fence.
   */
#define ATRSHMLOG_GET_THREAD_FENCE_4()  atrshmlog_get_thread_fence_4()
  
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
   * \brief We get a fence on or off flag
   *
   * \return
   * The old flag for fence.
   */
#define ATRSHMLOG_GET_THREAD_FENCE_5()  atrshmlog_get_thread_fence_5()
  
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
   * \brief We get a fence on or off flag
   *
   * \return
   * The old flag for fence.
   */
#define ATRSHMLOG_GET_THREAD_FENCE_6()  atrshmlog_get_thread_fence_6()
  
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
   * \brief We get a fence on or off flag
   *
   * \return
   * The old flag for fence.
   */
#define ATRSHMLOG_GET_THREAD_FENCE_7()  atrshmlog_get_thread_fence_7()
  
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
   * \brief We get a fence on or off flag
   *
   * \return
   * The old flag for fence.
   */
#define ATRSHMLOG_GET_THREAD_FENCE_8()  atrshmlog_get_thread_fence_8()
  
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
   * \brief We get a fence on or off flag
   *
   * \return
   * The old flag for fence.
   */
#define ATRSHMLOG_GET_THREAD_FENCE_9()  atrshmlog_get_thread_fence_9()
  
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
   * \brief We get a fence on or off flag
   *
   * \return
   * The old flag for fence.
   */
#define ATRSHMLOG_GET_THREAD_FENCE_10()  atrshmlog_get_thread_fence_10()
  
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
   * \brief We get a fence on or off flag
   *
   * \return
   * The old flag for fence.
   */
#define ATRSHMLOG_GET_THREAD_FENCE_11()  atrshmlog_get_thread_fence_11()
  
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
   * \brief We get a fence on or off flag
   *
   * \return
   * The old flag for fence.
   */
#define ATRSHMLOG_GET_THREAD_FENCE_12()  atrshmlog_get_thread_fence_12()
  
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
   * \brief We get a fence on or off flag
   *
   * \return
   * The old flag for fence.
   */
#define ATRSHMLOG_GET_THREAD_FENCE_13()  atrshmlog_get_thread_fence_13()

  /**
   * \brief Set the fence 13 flag on.
   *
   * \param __s
   * The new value for the flag.
   *
   * \return
   * The old flag for fence.
   */
#define ATRSHMLOG_SET_THREAD_FENCE_13(__s) atrshmlog_set_thread_fence_13((__s))

  /****************************************************/
  /* buffer related functions */
  
  /** 
   * \brief We get the buffers next cleanup pointer
   *
   * \param __buffer
   * Pointer to buffer
   *
   * \return
   * Pointer to buffer
   */
#define ATRSHMLOG_GET_THREAD_BUFFER_NEXT_CLEANUP(__buffer)  atrshmlog_get_thread_buffer_next_cleanup ((__buffer))
  
  /** 
   * \brief We get the buffers next full pointer
   *
   * \param __buffer
   * Pointer to buffer
   *
   * \return
   * Pointer to buffer
   */
#define ATRSHMLOG_GET_THREAD_BUFFER_NEXT_FULL(__buffer)  atrshmlog_get_thread_buffer_next_full ((__buffer))

  
  /** 
   * \brief We get the buffers next append pointer
   *
   * \param __buffer
   * Pointer to buffer
   *
   * \return
   * Pointer to buffer
   */
#define ATRSHMLOG_GET_THREAD_BUFFER_NEXT_APPEND(__buffer)  atrshmlog_get_thread_buffer_next_append ((__buffer))

  /** 
   * \brief We get the buffers safeguard
   *
   * \param __buffer
   * Pointer to buffer
   *
   * \return
   * safeguard
   */
#define ATRSHMLOG_GET_THREAD_BUFFER_SAFEGUARD(__buffer)  atrshmlog_get_thread_buffer_safeguard ((__buffer))
  
  /** 
   * \brief We get the buffers pid
   *
   * \param __buffer
   * Pointer to buffer
   *
   * \return
   * pid
   */
#define ATRSHMLOG_GET_THREAD_BUFFER_PID(__buffer)  atrshmlog_get_thread_buffer_pid ((__buffer))
  
  /** 
   * \brief We get the buffers tid
   *
   * \param __buffer
   * Pointer to buffer
   *
   * \return
   * tid
   */
#define ATRSHMLOG_GET_THREAD_BUFFER_TID(__buffer)  atrshmlog_get_thread_buffer_tid ((__buffer))
  
  /** 
   * \brief We get the buffers acquiretime
   *
   * \param __buffer
   * Pointer to buffer
   *
   * \return
   * acquiretime
   */
#define ATRSHMLOG_GET_THREAD_BUFFER_ACQUIRETIME(__buffer)  atrshmlog_get_thread_buffer_acquiretime ((__buffer))
  
  /** 
   * \brief We get the buffers id
   *
   * \param i_buffer
   * Pointer to buffer
   *
   * \return
   * id
   */
#define ATRSHMLOG_GET_THREAD_BUFFER_ID(__buffer)  atrshmlog_get_thread_buffer_id ((__buffer))

  
  /** 
   * \brief We get the buffers chksum
   *
   * \param __buffer
   * Pointer to buffer
   *
   * \return
   * id
   */
#define ATRSHMLOG_GET_THREAD_BUFFER_CHKSUM(__buffer)  atrshmlog_get_thread_buffer_chksum ((__buffer))
  
  /** 
   * \brief We get the buffers size
   *
   * \param __buffer
   * Pointer to buffer
   *
   * \return
   * size
   */
#define ATRSHMLOG_GET_THREAD_BUFFER_SIZE(__buffer)  atrshmlog_get_thread_buffer_size ((__buffer))
  
  
  /** 
   * \brief We get the buffers maxsize
   *
   * \param i_buffer
   * Pointer to buffer
   *
   * \return
   * maxsize
   */
#define ATRSHMLOG_GET_THREAD_BUFFER_MAXSIZE(__buffer)  atrshmlog_get_thread_buffer_maxsize ((__buffer))
  
  /** 
   * \brief We get the buffers dispose flag
   *
   * \param i_buffer
   * Pointer to buffer
   *
   * \return
   * dispose
   */
#define ATRSHMLOG_GET_THREAD_BUFFER_DISPOSE(__buffer)  atrshmlog_get_thread_buffer_dispose ((__buffer))
  
  /** 
   * \brief We get the buffers dispatched flag
   *
   * \param __buffer
   * Pointer to buffer
   *
   * \return
   * dispatched
   */
#define ATRSHMLOG_GET_THREAD_BUFFER_DISPATCHED(__buffer)  atrshmlog_get_thread_buffer_dispatched ((__buffer))
  
  /** 
   * \brief We get the buffers payload adress
   *
   * \param __buffer
   * Pointer to buffer
   *
   * \return
   * Pointer to payload
   */
#define ATRSHMLOG_GET_THREAD_BUFFER_PAYLOAD(__buffer)  atrshmlog_get_thread_buffer_payload ((__buffer))

  
  /**
   *  \brief The targetbuffer max
   *
   * \return
   * The flag
   */
#define ATRSHMLOG_GET_TARGETBUFFER_MAX()  atrshmlog_get_targetbuffer_max()

  /**
   * \brief Set the targetbuffer max
   *
   * \param __flag
   * Our new  flag
   *
   * \return 
   * The old flag
   */
#define ATRSHMLOG_SET_TARGETBUFFER_MAX(__flag) atrshmlog_set_targetbuffer_max((__flag))

      
  /**
   *  \brief The checksum flag
   *
   * \return
   * The flag
   */
#define ATRSHMLOG_GET_CHECKSUM()  atrshmlog_get_checksum()

  /**
   * \brief Set the checksum flag
   *
   * \param __flag
   * Our new  flag
   *
   * \return 
   * The old flag
   */
#define ATRSHMLOG_SET_CHECKSUM(__flag) atrshmlog_set_checksum((__flag))

  /** 
   * \brief We get the buffer cleanup anchor
   *
   * \return
   * Pointer to buffer
   */
#define ATRSHMLOG_GET_BUFFER_CLEANUP_ANCHOR() atrshmlog_get_buffer_cleanup_anchor ()

  /** 
   * \brief We get the buffer full anchor
   *
   * \return
   * Pointer to buffer
   */
#define ATRSHMLOG_GET_BUFFER_FULL_ANCHOR() atrshmlog_get_buffer_full_anchor ()


  /** 
   * \brief We get the buffer append anchor
   *
   * \return
   * Pointer to buffer
   */
#define ATRSHMLOG_GET_BUFFER_APPEND_ANCHOR() atrshmlog_get_buffer_append_anchor ()


  /************************************************************************/
  /* slave related functions */

  /** 
   * \brief We get the next element on slave list
   *
   * If we deliver a 0 we start from top.
   * 
   * \param __slave
   * Pointer to thread local or NULL
   *
   * \return
   * - top if parameter NULL
   * - next if parameter is thread local of slave
   */
#define ATRSHMLOG_GET_NEXT_SLAVE_LOCAL(__slave) atrshmlog_get_next_slave_local((__slave))

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
   * \brief Start a new slave thread.
   *
   * \return
   * The return code of the used function to start the thread.
   */
#define ATRSHMLOG_CREATE_SLAVE()  atrshmlog_create_slave()

  /** 
   * \brief decrement the slave count.
   * 
   * we deliver the old one and decrement. till 0.
   * this is for the case you kill slaves.
   */
#define ATRSHMLOG_DECREMENT_SLAVE_COUNT()  atrshmlog_decrement_slave_count()

/** 
 * \brief  We remove the save from the list of slaves
 *
 * \param __slave
 * Pointer to a thread local or NULL
 *
 * \return
 * - 0 is ok
 * - non zero is error
 */
#define ATRSHMLOG_REMOVE_SLAVE_VIA_LOCAL(__slave) atrshmlog_remove_slave_via_local((__slave))
  /* from here on we have stuff for higher level of use */

    
  /**
   * \brief We switch the thread off 
   *
   * \param __slave
   * The adress of the slave locals
   *
   * \return 
   * void
   */
#define ATRSHMLOG_TURN_SLAVE_OFF(__slave) atrshmlog_turn_slave_off((__slave))

  /** 
   * \brief We get the tid of a slave
   *
   * \param __slave
   * Pointer to a slave local or NULL
   *
   * \return
   * - 0 if pointer is NULL
   * - tid
   */
#define ATRSHMLOG_GET_SLAVE_TID(__slave)  atrshmlog_get_slave_tid((__slave))

  /** 
   * \brief Set the slave run flag to off.
   *
   * \return
   * void
   */
#define ATRSHMLOG_SET_F_LIST_BUFFER_SLAVE_RUN_OFF()  atrshmlog_set_f_list_buffer_slave_run_off()


  /** 
   * \brief Get the wait for slaves flag.
   *
   * \return
   * The flag for the wait for slaves.
   */
#define ATRSHMLOG_GET_WAIT_FOR_SLAVES()  atrshmlog_get_wait_for_slaves()

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
   * \brief Get the wait time in nanoseconds for the slave if shm full.
   *
   * \return
   * The number of nanos the slave sleeps when shm full
   */
#define ATRSHMLOG_GET_SLAVE_TO_SHM_WAIT() atrshmlog_get_slave_to_shm_wait()

  /**
   * \brief Set the wait time for the slaves in nanoseconds.
   *
   * \param __wait_nanos
   * The number of nanos the slave has to sleep when shm full.
   * to be done.
   *
   * \return
   * The old number of nanos the slave had to wait
   */
#define ATRSHMLOG_SET_SLAVE_TO_SHM_WAIT(__wait_nanos) atrshmlog_set_slave_to_shm_wait((__wait_nanos))


  /**
   * \brief Get the time for mem to shm
   *
   * \return
   * The number of nanos 
   */
#define ATRSHMLOG_GET_LAST_MEM_TO_SHM() atrshmlog_get_last_mem_to_shm()

  /************************************************************************/
  /** 
   * \brief Get the shm area start adress.
   *
   * \return
   * Points to the shared memory area.
   */
#define ATRSHMLOG_GET_AREA()  atrshmlog_get_area()

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
   * \brief Verify the shm struct is ok.
   *
   * \return
   * - zero for ok
   * - Negative for en big error.
   * - Positiv for minor errors.
   */
#define ATRSHMLOG_VERIFY() atrshmlog_verify()


  /************************************************************************/
  /* statistics functions */
  
  /**
   * \brief We deliver the max index of the statistics buffer
   *
   * \return 
   * Maximum index of the statistics buffer.
   */
#define ATRSHMLOG_GET_STATISTICS_MAX_INDEX() atrshmlog_get_statistics_max_index()

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
#define ATRSHMLOG_GET_STATISTICS(__o) atrshmlog_get_statistics((__o))

  /************************************************************************/
  /* reader transfer functions */
  
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
   *  \param  __nd
   * Points to an int32 that holds the number of dispatch from the 
   * thread.
   *
   *  \param __s1
   * Statistic value for counter_write0
   *
   *  \param __s2
   * Statistic value for counter_write0_discard
   *
   *  \param __s3
   * Statistic value for counter_write0_wait
   *
   *  \param __s4
   * Statistic value for counter_write0_adaptive
   *
   *  \param __s5
   * Statistic value for counter_write0_adaptive_fast
   *
   *  \param __s6
   * Statistic value for counter_write0_adaptive_very_fast
   *
   *  \param __s7
   * Statistic value for counter_write1
   *
   *  \param __s8
   * Statistic value for counter_write1_discard
   *
   *  \param __s9
   * Statistic value for counter_write1_wait
   *
   *  \param __s10
   * Statistic value for counter_write1_adaptive
   *
   *  \param __s11
   * Statistic value for counter_write1_adaptive_fast
   *
   *  \param __s12
   * Statistic value for counter_write1_adaptive_very_fast
   *
   *  \param __s13
   * Statistic value for counter_write2
   *
   *  \param __s14
   * Statistic value for counter_write2_discard
   *
   *  \param __s15
   * Statistic value for counter_write2_wait
   *
   *  \param __s16
   * Statistic value for counter_write2_adaptive
   *
   *  \param __s17
   * Statistic value for counter_write2_adaptive_fast
   *
   *  \param __s18
   * Statistic value for counter_write2_adaptive_very_fast
   *
   * \return
   * - Negative if an error occured.
   * - Zero if a buffer could be fetched.
   * - Positiv if the call didnt fetch a buffer. In this case the 
   *   buffer length is 0 and all other values are meaningless.
   * 
   */
#define ATRSHMLOG_READ_FETCH(__a,__i,__t,__tl,__pid,__tid,__it,__itb,__ita,__lt,__ltb,__lta,__dft,__stf,__at,__id,__nd,__s1,__s2,__s3,__s4,__s5,__s6,__s7,__s8,__s9,__s10,__s11,__s12,__s13,__s14,__s15,__s16,__s17,__s18) \
  atrshmlog_read_fetch((__a),	  \
		       (__i),	  \
		       (__t),	  \
		       (__tl),	  \
		       (__pid),	  \
		       (__tid),	  \
		       (__it),    \
		       (__itb),   \
		       (__ita),   \
		       (__lt),	  \
		       (__ltb),	  \
		       (__lta),	  \
		       (__dft),	  \
		       (__stf),	  \
		       (__at),	  \
		       (__id),	  \
		       (__nd),    \
		       (__s1),	  \
		       (__s2),	  \
		       (__s3),	  \
		       (__s4),	  \
		       (__s5),	  \
		       (__s6),	  \
		       (__s7),	  \
		       (__s8),	  \
		       (__s9),	  \
		       (__s10),	  \
		       (__s11),	  \
		       (__s12),	  \
		       (__s13),	  \
		       (__s14),	  \
		       (__s15),	  \
		       (__s16),	  \
		       (__s17),	  \
		       (__s18)	  \
		       )


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
   *  \param  __nd
   * Points to an int32 that holds the number of dispatch from the 
   * thread.
   *
   *  \param __s1
   * Statistic value for counter_write0
   *
   *  \param __s2
   * Statistic value for counter_write0_discard
   *
   *  \param __s3
   * Statistic value for counter_write0_wait
   *
   *  \param __s4
   * Statistic value for counter_write0_adaptive
   *
   *  \param __s5
   * Statistic value for counter_write0_adaptive_fast
   *
   *  \param __s6
   * Statistic value for counter_write0_adaptive_very_fast
   *
   *  \param __s7
   * Statistic value for counter_write1
   *
   *  \param __s8
   * Statistic value for counter_write1_discard
   *
   *  \param __s9
   * Statistic value for counter_write1_wait
   *
   *  \param __s10
   * Statistic value for counter_write1_adaptive
   *
   *  \param __s11
   * Statistic value for counter_write1_adaptive_fast
   *
   *  \param __s12
   * Statistic value for counter_write1_adaptive_very_fast
   *
   *  \param __s13
   * Statistic value for counter_write2
   *
   *  \param __s14
   * Statistic value for counter_write2_discard
   *
   *  \param __s15
   * Statistic value for counter_write2_wait
   *
   *  \param __s16
   * Statistic value for counter_write2_adaptive
   *
   *  \param __s17
   * Statistic value for counter_write2_adaptive_fast
   *
   *  \param __s18
   * Statistic value for counter_write2_adaptive_very_fast
   *
   * \return
   * - Negative if an error occured.
   * - Zero if a buffer could be fetched.
   * - Positiv if the call didnt fetch a buffer. In this case the 
   *   buffer length is 0 and all other values are meaningless.
   * 
   */
#define ATRSHMLOG_READ(__a,__i,__t,__tl,__pid,__tid,__it,__itb,__ita,__lt,__ltb,__lta,__dft,__stf,__at,__id,__nd,__s1,__s2,__s3,__s4,__s5,__s6,__s7,__s8,__s9,__s10,__s11,__s12,__s13,__s14,__s15,__s16,__s17,__s18) \
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
		 (__id), \
		 (__nd), \
		 (__s1), \
		 (__s2), \
		 (__s3), \
		 (__s4), \
		 (__s5), \
		 (__s6), \
		 (__s7), \
		 (__s8), \
		 (__s9), \
		 (__s10), \
		 (__s11), \
		 (__s12), \
		 (__s13), \
		 (__s14), \
		 (__s15), \
		 (__s16), \
		 (__s17), \
		 (__s18) \
    )

  /* end function macros */ 
  
  /************************************************************************/

  /* the real functions */

  /* burocracy functions */

  /** 
   * \brief The version of the buffer system.
   *
   * If we change anything of the buffer layout we have to change this too.
   * Its for layout compatible processing of buffers. 
   * From the internal shm buffer to the readers to the converters.
   * Any change you need is a new layout. Even a simple additional number.
   * 
   * \return
   * Number of the major version of the shmlog.
   */   
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_version(void);

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
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_minor_version(void);

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
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_patch_version(void);

  /* write log related functions */

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
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_write0(const atrshmlog_int32_t i_eventnumber,
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
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_write1(const atrshmlog_int32_t i_eventnumber,
				   const atrshmlog_int32_t i_eventflag,
				   const atrshmlog_int32_t i_userflag,
				   atrshmlog_time_t i_starttime,
				   atrshmlog_time_t i_endtime,
				   const void* const i_local,
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
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_write2(const atrshmlog_int32_t i_eventnumber,
				   const atrshmlog_int32_t i_eventflag,
				   const atrshmlog_int32_t i_userflag,
				   atrshmlog_time_t i_starttime,
				   atrshmlog_time_t i_endtime,
				   const void* const i_local,
				   const atrshmlog_int32_t i_size,
				   const char* const i_argv[],
				   const atrshmlog_int32_t i_argc_hint);


  /**
   *  \brief The autoflush flag
   *
   * \return
   * The flag
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_autoflush_process(void);
  
  /**
   * \brief Set the autoflush for the process
   *
   * \param i_flag
   * Our new autoflush flag
   *
   * \return 
   * The old flag
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_autoflush_process(const int i_flag);

  /**
   *  \brief The autoflush flag for this thread
   *
   * \return
   * The flag
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_autoflush(void);
  
  /**
   * \brief Set the autoflush for the thread
   *
   * \param i_flag
   * Our new autoflush flag
   *
   * \return 
   * The old flag
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_autoflush(const int i_flag);

  /**
   * \brief Deliver logging state for the program.
   *
   * \return
   * - Zero is logging
   * - not zero is not logging.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_logging(void);
  
  /**
   * \brief Switch the logging for the process on .
   *
   * \return
   * The old number of the logging flag.
   */  
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_logging_process_on(void);

  /**
   * \brief Switch the logging for the process off.
   *
   * \return
   * The old number of the logging flag.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_logging_process_off(void);

  /**
   * \brief Switch the logging for the process off finally.
   *
   * \return
   * Old flag.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_logging_process_off_final(void);

  /* timing functions */

  /**
   * \brief We wait nanoseconds
   *
   * \param i_nanos
   * The nanosecs till 2^31
   *
   * \return
   * void
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  void atrshmlog_sleep_nanos (const atrshmlog_int32_t i_nanos);
  
  /**
   * \brief Get the used clock id for the timing info from real time clock.
   *
   * \return
   * The id of the clock to use in get clock call.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_clock_id(void);

  /**
   * \brief Set the clock id for the gettime call.
   *
   * \param i_id
   * The id for the get clock call.
   *
   * \return
   * The old id for the get clock call.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_clock_id(const atrshmlog_int32_t i_id);

  /**
   * \brief We get the real time
   *
   * \return 
   * The real time in our format
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_internal_time_t atrshmlog_get_realtime(void);

  /**
   * \brief Initial time for the module .
   *
   * \return
   * The internal time struct with the inittime of the process.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_internal_time_t atrshmlog_get_inittime(void);

  /**
   * \brief Initial click before get inittime.
   *
   * \return
   * The time for the click before the get clock call to fill inittime.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_time_t atrshmlog_get_inittime_tsc_before(void);

  /**
   * \brief Initial click after get initttime.
   *
   * \return
   * The time for the click after the get clock call to fill inittime.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_time_t atrshmlog_get_inittime_tsc_after(void);

  /* initialization and configuration functions */

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
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_attach(void);

  /** 
   * \brief We disconnect to the shm buffer.
   * 
   * We do no cleanup.
   *
   * So if we attach again at another area
   * we deliver to that area.
   *
   * We do not log after we are detached
   *
   * \return
   * - Zero ok
   * - -1 for error
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_detach(void);


  /** 
   * \brief We reattach to a shm buffer
   * 
   * We set the values and do most of the attach
   *
   * We can set the values, but we do NOT reinit buffers.
   * We do NOT restart slaves.
   * We DO resize the event locks array.
   *
   * \param i_params
   * int array with 56 values.
   * - 0 : flag for use id
   * - 1 : new value for id
   * - 2 : flag for use count
   * - 3 : new value for count
   * - 4 : flag for use atrshmlog_init_buffers_in_advance
   * - 5 : new value for atrshmlog_init_buffers_in_advance
   * - 6 : flag for use atrshmlog_buffer_strategy
   * - 7 : new value for atrshmlog_buffer_strategy
   * - 8 : flag for use atrshmlog_strategy_wait_wait_time
   * - 9 : new value for atrshmlog_strategy_wait_wait_time
   * - 10 : flag for use atrshmlog_delimiter
   * - 11 : new value for atrshmlog_delimiter
   * - 12 : flag for use atrshmlog_event_locks_max
   * - 13 : new value for atrshmlog_event_locks_max
   * - 14 : flag for use atrshmlog_buffer_infosize
   * - 15 : new value for atrshmlog_buffer_infosize
   * - 16 : flag for use atrshmlog_prealloc_buffer_count
   * - 17 : new value for atrshmlog_prealloc_buffer_count
   * - 18 : flag for use atrshmlog_f_list_buffer_slave_wait
   * - 19 : new value for atrshmlog_f_list_buffer_slave_wait
   * - 20 : flag for use atrshmlog_f_list_buffer_slave_count
   * - 21 : new value for atrshmlog_f_list_buffer_slave_count
   * - 22 : flag for use atrshmlog_wait_for_slaves
   * - 23 : new value for atrshmlog_wait_for_slaves
   * - 24 : flag for use atrshmlog_clock_id
   * - 25 : new value for atrshmlog_clock_id
   * - 26 : flag for use atrshmlog_thread_fence_1
   * - 27 : new value for atrshmlog_thread_fence_1
   * - 28 : flag for use atrshmlog_thread_fence_2
   * - 29 : new value for atrshmlog_thread_fence_2
   * - 30 : flag for use atrshmlog_thread_fence_3
   * - 31 : new value for atrshmlog_thread_fence_3
   * - 32 : flag for use atrshmlog_thread_fence_4
   * - 33 : new value for atrshmlog_thread_fence_4
   * - 34 : flag for use atrshmlog_thread_fence_5
   * - 35 : new value for atrshmlog_thread_fence_5
   * - 36 : flag for use atrshmlog_thread_fence_6
   * - 37 : new value for atrshmlog_thread_fence_6
   * - 38 : flag for use atrshmlog_thread_fence_7
   * - 39 : new value for atrshmlog_thread_fence_7
   * - 40 : flag for use atrshmlog_thread_fence_8
   * - 41 : new value for atrshmlog_thread_fence_8
   * - 42 : flag for use atrshmlog_thread_fence_9
   * - 43 : new value for atrshmlog_thread_fence_9
   * - 44 : flag for use atrshmlog_thread_fence_10
   * - 45 : new value for atrshmlog_thread_fence_10
   * - 46 : flag for use atrshmlog_thread_fence_11
   * - 47 : new value for atrshmlog_thread_fence_11
   * - 48 : flag for use atrshmlog_thread_fence_12
   * - 49 : new value for atrshmlog_thread_fence_12
   * - 50 : flag for use atrshmlog_thread_fence_13
   * - 51 : new value for atrshmlog_thread_fence_13
   * - 52 : flag for use atrshmlog_checksum
   * - 53 : new value for atrshmlog_checksum
   * - 54 : flag for use logging process off
   * - 55 : new value for logging process off
   * - 56 : flag for use targetbuffer max
   * - 57 : new value for targetbuffer max 
   * 
   * \return
   * - Zero ok
   * - negative for error
   * - positiv for minor problem
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_reattach(const atrshmlog_int32_t* const i_params);



  /** 
   * \brief We get the actual name of the environment variable prefix.
   *
   * \return 
   * Points to the used prefix.
   */  
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  const char* atrshmlog_get_env_prefix(void);

  /** 
   * \brief We set the prefix for name lookup in the program. 
   *
   * \param i_prefix
   * Start of the new prefix.
   *
   * \return
   * void
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  void atrshmlog_set_env_prefix (const char * const i_prefix);


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
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  const char* atrshmlog_get_env(const char* const i_suffix); 

  /** 
   * \brief We get the value of the environment variable holding the shmid in text.
   *
   * \return
   * - Points to the shmid variable value
   * - Zero if no variable defined.
   */  
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  const char* atrshmlog_get_env_shmid(void);

  /** 
   * \brief We get the suffix for the shmid environment variable.
   *
   * \return
   * Points to the c string.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  const char* atrshmlog_get_env_id_suffix(void);
  
  /** 
   * \brief Get the event flag maximum index number
   *
   * \return
   * Number of the max event + 1 
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_event_locks_max(void);

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
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_event_locks_max(const atrshmlog_int32_t i_max);


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
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_event_t atrshmlog_get_event(const atrshmlog_int32_t i_index);

  /**
   * \brief Helper to set an event from the program.
   *
   * \param i_index
   * Number of the event that is set to 1.
   *
   * \return
   * Old event
   */  
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_event_t atrshmlog_set_event_on(const atrshmlog_int32_t i_index);

  /**
   * \brief Helper to clear an event from the program.
   *
   * \param i_index
   * Number of the event that is set to 0.
   *
   * \return
   * old event
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_event_t atrshmlog_set_event_off(const atrshmlog_int32_t i_index);


  /**
   * \brief Deliver the shm id of the process.
   *
   * This is the variable shmid.
   *
   * \return
   * Number of the shared memory id.
   */  
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  int atrshmlog_get_shmid(void);


  /**
   * \brief The maximum size of log buffer.
   *
   * \return
   * The number of the maximum size of log buffers.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_buffer_max_size(void);
  
  /**
   * \brief We get the actual set info size for a log buffer.
   *
   * \return
   * Number of the actual size for log buffers.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_buffer_size(void);

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
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_buffer_size(const atrshmlog_int32_t i_size);

  /** 
   * \brief Count of buffers ready for acquire.
   *
   * \return
   * The number of buffers for logging in the threads.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_acquire_count(void);

  /**
   * \brief Get count of preallocate buffers in one alloc.
   *
   * \return
   * The number of buffers a dyn alloc count gets in one malloc
   * from the dyn memory area.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_prealloc_buffer_count(void);

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
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_prealloc_buffer_count(const atrshmlog_int32_t i_count);

  /**
   * \brief Get highest id for buffers.
   *
   * \return
   * get the id of the last buffer initialized.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_buffer_id(void);

  /**
   * \brief we get the init in advance flag
   *
   * \return
   * The flag
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_init_buffers_in_advance(void);

  /**
   * \brief we turn the init in advance on
   *
   * \return
   * The old flag
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_init_buffers_in_advance_on(void);
  
  /**
   * \brief we turn the init in advance off
   *
   * \return
   * The old flag
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_init_buffers_in_advance_off(void);

  /* thread related functions */

  /**
   * \brief We get the thread tid 
   *
   * \return 
   * The thread tid
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_tid_t atrshmlog_get_tid(void);

  /**
   * \brief We get the thread locals adress of a thread
   *
   * \return
   * the adress of the thread locals
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  volatile const void* atrshmlog_get_thread_locals_adress(void);

  /**
   * \brief We stop loging for the calling thread.
   *
   * \return
   * void
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  void atrshmlog_stop(void);

  /**
   * \brief We flush the buffers for the calling thread.
   *
   * \return
   * void
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  void atrshmlog_flush(void);


  /** 
   * \brief We get the strategy for this thread
   *
   * \return
   * The value 
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_strategy(void);
  
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
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_strategy(const enum atrshmlog_strategy i_strategy);

  /** 
   * \brief We get the strategy for the process
   *
   * \return
   * The value 
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_strategy_process(void);

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
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_strategy_process(const enum atrshmlog_strategy i_strategy);

  

  /**
   * \brief Get the wait time in nanoseconds for the strategy wait
   *
   * \return
   * The number of nanos
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_strategy_wait_wait_time(void);

  /**
   * \brief Set the wait time for the wait strategy
   *
   * \param i_wait_nanos
   * The number of nanos 
   *
   * \return
   * The old number of nanos the slave had to wait
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_strategy_wait_wait_time(const atrshmlog_int32_t i_wait_nanos);

  /** 
   * \brief We get the tid of a thread local
   *
   * \param i_thread_local
   * Pointer to a thread local or NULL
   *
   * \return
   * - 0 if pointer is NULL
   * - tid
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_tid_t atrshmlog_get_thread_local_tid (volatile const void * const i_thread_local);

  /** 
   * \brief We get the pid of a thread local
   *
   * \param i_thread_local
   * Pointer to a thread local or NULL
   *
   * \return
   * - 0 if pointer is NULL
   * - pid
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_pid_t atrshmlog_get_thread_local_pid (volatile const void * const i_thread_local);

  /** 
   * \brief We get the buffer index of a thread local
   *
   * \param i_thread_local
   * Pointer to a thread local or NULL
   *
   * \return
   * - 0 if pointer is NULL
   * - index
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_thread_local_index (volatile const void * const i_thread_local);

  /** 
   * \brief We get the buffer count of a thread local
   *
   * \param i_thread_local
   * Pointer to a thread local or NULL
   *
   * \return
   * - 0 if pointer is NULL
   * - count
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_thread_local_count (volatile const void * const i_thread_local);

  /** 
   * \brief We get the buffer adress via index of a thread local
   *
   * \param i_thread_local
   * Pointer to a thread local or NULL
   * \param i_index
   * Index in the buffer pointer array
   *
   * \return
   * - 0 if pointer is NULL
   * - buffer pointer
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  volatile const void *atrshmlog_get_thread_local_buffer (volatile const void * const i_thread_local, const atrshmlog_int32_t i_index);


  /**
   * \brief We switch the thread off and dispatch its buffers
   *
   * \param i_thread_locals
   * The adress of the thread locals
   *
   * \return 
   * void
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  void atrshmlog_turn_logging_off(volatile const void* const i_thread_locals);

  /** 
   * \brief We make reuse of buffers of a dead thread
   *
   * \param i_tid
   * Our tid for the thread
   *
   * \return
   * The number of found buffers
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_reuse_thread_buffers(const atrshmlog_tid_t i_tid);

  
  /**
   * \brief We get a fence on or off flag
   *
   * \return
   * The old flag for fence.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_thread_fence_1(void);

  /**
   * \brief We switch a fence on or off
   *
   * \return
   * The old flag for fence.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_thread_fence_1(const atrshmlog_int32_t i_switch);
  
  
  /**
   * \brief We get a fence on or off flag
   *
   * \return
   * The old flag for fence.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_thread_fence_2(void);

  /**
   * \brief We switch a fence on or off
   *
   * \return
   * The old flag for fence.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_thread_fence_2(const atrshmlog_int32_t i_switch);
  
  /**
   * \brief We get a fence on or off flag
   *
   * \return
   * The old flag for fence.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_thread_fence_3(void);

  /**
   * \brief We switch a fence on or off
   *
   * \return
   * The old flag for fence.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_thread_fence_3(const atrshmlog_int32_t i_switch);
  
  
  /**
   * \brief We get a fence on or off flag
   *
   * \return
   * The old flag for fence.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_thread_fence_4(void);

  /**
   * \brief We switch a fence on or off
   *
   * \return
   * The old flag for fence.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_thread_fence_4(const atrshmlog_int32_t i_switch);
  
  
  /**
   * \brief We get a fence on or off flag
   *
   * \return
   * The old flag for fence.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_thread_fence_5(void);

  /**
   * \brief We switch a fence on or off
   *
   * \return
   * The old flag for fence.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_thread_fence_5(const atrshmlog_int32_t i_switch);
  
  
  /**
   * \brief We get a fence on or off flag
   *
   * \return
   * The old flag for fence.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_thread_fence_6(void);

  /**
   * \brief We switch a fence on or off
   *
   * \return
   * The old flag for fence.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_thread_fence_6(const atrshmlog_int32_t i_switch);
  
  
  /**
   * \brief We get a fence on or off flag
   *
   * \return
   * The old flag for fence.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_thread_fence_7(void);

  /**
   * \brief We switch a fence on or off
   *
   * \return
   * The old flag for fence.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_thread_fence_7(const atrshmlog_int32_t i_switch);
  
  
  /**
   * \brief We get a fence on or off flag
   *
   * \return
   * The old flag for fence.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_thread_fence_8(void);

  /**
   * \brief We switch a fence on or off
   *
   * \return
   * The old flag for fence.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_thread_fence_8(const atrshmlog_int32_t i_switch);
  
  
  /**
   * \brief We get a fence on or off flag
   *
   * \return
   * The old flag for fence.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_thread_fence_9(void);

  /**
   * \brief We switch a fence on or off
   *
   * \return
   * The old flag for fence.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_thread_fence_9(const atrshmlog_int32_t i_switch);
  
  
  /**
   * \brief We get a fence on or off flag
   *
   * \return
   * The old flag for fence.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_thread_fence_10(void);

  /**
   * \brief We switch a fence on or off
   *
   * \return
   * The old flag for fence.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_thread_fence_10(const atrshmlog_int32_t i_switch);
  
  
  /**
   * \brief We get a fence on or off flag
   *
   * \return
   * The old flag for fence.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_thread_fence_11(void);

  /**
   * \brief We switch a fence on or off
   *
   * \return
   * The old flag for fence.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_thread_fence_11(const atrshmlog_int32_t i_switch);
  
  
  /**
   * \brief We get a fence on or off flag
   *
   * \return
   * The old flag for fence.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_thread_fence_12(void);
  
  /**
   * \brief We switch a fence on or off
   *
   * \return
   * The old flag for fence.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_thread_fence_12(const atrshmlog_int32_t i_switch);

  /**
   * \brief We get a fence on or off flag
   *
   * \return
   * The old flag for fence.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_thread_fence_13(void);
  
  /**
   * \brief We switch a fence on or off
   *
   * \return
   * The old flag for fence.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_thread_fence_13(const atrshmlog_int32_t i_switch);

  /**********************************************/

  /* buffer related functions */
  
  /** 
   * \brief We get the buffers next cleanup pointer
   *
   * \param i_buffer
   * Pointer to buffer
   *
   * \return
   * Pointer to buffer
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  volatile const void *atrshmlog_get_thread_buffer_next_cleanup (volatile const void * const i_buffer);
  
  /** 
   * \brief We get the buffers next full pointer
   *
   * \param i_buffer
   * Pointer to buffer
   *
   * \return
   * Pointer to buffer
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  volatile const void *atrshmlog_get_thread_buffer_next_full (volatile const void * const i_buffer);
  
  /** 
   * \brief We get the buffers next append pointer
   *
   * \param i_buffer
   * Pointer to buffer
   *
   * \return
   * Pointer to buffer
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  volatile const void *atrshmlog_get_thread_buffer_next_append (volatile const void * const i_buffer);
  
  
  /** 
   * \brief We get the buffers safeguard
   *
   * \param i_buffer
   * Pointer to buffer
   *
   * \return
   * safeguard
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_thread_buffer_safeguard (volatile const void * const i_buffer);
  
  /** 
   * \brief We get the buffers pid
   *
   * \param i_buffer
   * Pointer to buffer
   *
   * \return
   * pid
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_pid_t atrshmlog_get_thread_buffer_pid (volatile const void * const i_buffer);
  
  /** 
   * \brief We get the buffers tid
   *
   * \param i_buffer
   * Pointer to buffer
   *
   * \return
   * tid
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_tid_t atrshmlog_get_thread_buffer_tid (volatile const void * const i_buffer);
  
  /** 
   * \brief We get the buffers acquiretime
   *
   * \param i_buffer
   * Pointer to buffer
   *
   * \return
   * acquiretime
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_time_t atrshmlog_get_thread_buffer_acquiretime (volatile const void * const i_buffer);
  
  /** 
   * \brief We get the buffers id
   *
   * \param i_buffer
   * Pointer to buffer
   *
   * \return
   * id
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_thread_buffer_id (volatile const void * const i_buffer);
  
  /** 
   * \brief We get the buffers chksum
   *
   * \param i_buffer
   * Pointer to buffer
   *
   * \return
   * id
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_thread_buffer_chksum (volatile const void * const i_buffer);
  
  /** 
   * \brief We get the buffers size
   *
   * \param i_buffer
   * Pointer to buffer
   *
   * \return
   * size
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_thread_buffer_size (volatile const void * const i_buffer);
  
  /** 
   * \brief We get the buffers maxsize
   *
   * \param i_buffer
   * Pointer to buffer
   *
   * \return
   * maxsize
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_thread_buffer_maxsize (volatile const void * const i_buffer);
  
  /** 
   * \brief We get the buffers dispose flag
   *
   * \param i_buffer
   * Pointer to buffer
   *
   * \return
   * dispose
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_thread_buffer_dispose (volatile const void * const i_buffer);
  
  /** 
   * \brief We get the buffers dispatched flag
   *
   * \param i_buffer
   * Pointer to buffer
   *
   * \return
   * dispatched
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_thread_buffer_dispatched (volatile const void * const i_buffer);
  
  /** 
   * \brief We get the buffers payload adress
   *
   * \param i_buffer
   * Pointer to buffer
   *
   * \return
   * Pointer to payload
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  volatile const void *atrshmlog_get_thread_buffer_payload (volatile const void * const i_buffer);
  
  /**
   *  \brief The targetbuffer max
   *
   * \return
   * The flag
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_targetbuffer_max(void);
    
  /**
   * \brief Set the targetbuffer max
   *
   * \param i_flag
   * Our new  flag
   *
   * \return 
   * The old flag
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_targetbuffer_max(const int i_flag);

  /**
   *  \brief The checksum flag
   *
   * \return
   * The flag
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_checksum(void);
    
  /**
   * \brief Set the checksum flag
   *
   * \param i_flag
   * Our new  flag
   *
   * \return 
   * The old flag
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_checksum(const int i_flag);

  /** 
   * \brief We get the buffer cleanup anchor
   *
   * \return
   * Pointer to buffer
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  volatile const void *atrshmlog_get_buffer_cleanup_anchor (void);

  /** 
   * \brief We get the buffer full anchor
   *
   * \return
   * Pointer to buffer
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  volatile const void *atrshmlog_get_buffer_full_anchor (void);

  /** 
   * \brief We get the buffer append anchor
   *
   * \return
   * Pointer to buffer
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  volatile const void *atrshmlog_get_buffer_append_anchor (void);

  /**********************************************/
  
  /* slave related functions */

  /** 
   * \brief We get the next element on slave list
   *
   * If we deliver a 0 we start from top.
   * 
   * \param i_slave_local
   * Pointer to slave local or NULL
   *
   * \return
   * - top if parameter NULL
   * - next if parameter is thread local of slave
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  volatile const void* atrshmlog_get_next_slave_local(volatile const void* const i_slave_local);

  /**
   * \brief Get the count of slave threads that the logging use.
   *
   * \return
   * The number of threads running the function for slave proc.
   */ 
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_f_list_buffer_slave_count(void);

  /**
   * \brief Set the number of logging slave threads.
   *
   * This has to be before we start the threads. 
   * This means you have to do it before attach.
   *
   * If we do it after attach or reattach it sets not only the number.
   * It also starts and stops slaves to adjust to the delivered number.
   *
   * \param i_count
   * The new count of slave threads to start.
   *
   * \return
   * The old number of slave threads to start.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_f_list_buffer_slave_count(const atrshmlog_int32_t i_count);

  /**
   * \brief We can start a slave thread with it.
   *
   * \return
   * The return code of the used function to start the thread.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  int atrshmlog_create_slave(void);


  /** 
   * \brief decrement the slave count.
   * 
   * we deliver the old one and decrement. till 0.
   * this is for the case you kill slaves.
   *
   * \return
   * old count
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_decrement_slave_count(void);


  /** 
   * \brief We remove the slave from the list of slaves
   *
   * \param i_thread_local
   * Pointer to thread local or NULL
   *
   * \return
   * - 0 ok
   * - 1 list was empty
   * - non zero error
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_remove_slave_via_local(volatile const void* const i_thread_local);

  /**
   * \brief We switch the thread off 
   *
   * \param i_slave
   * The adress of the slave locals
   *
   * \return 
   * void
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  void atrshmlog_turn_slave_off(volatile const void* const i_slave);

  /** 
   * \brief We get the tid of a slave
   *
   * \param i_slave
   * Pointer to a slave local or NULL
   *
   * \return
   * - 0 if pointer is NULL
   * - tid
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_tid_t atrshmlog_get_slave_tid (volatile const void * const i_slave);


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
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  void atrshmlog_set_f_list_buffer_slave_run_off(void);

  /**
   * \brief This is the flag value for waiting in atexit for slaves down.
   *
   * \return
   * The flag for the wait for slaves.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_wait_for_slaves(void);


  /**
   * \brief This sets the flag to wait for slaves count down to 0
   * in the atexit.
   *
   * \return
   * The old flag.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_wait_for_slaves_on(void);

  /**
   * \brief This switches the waiting flag off in the atexit.
   *
   * \return
   * The old flag.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_wait_for_slaves_off(void);

  /**
   * \brief Get the wait time in nanoseconds for the slave .
   *
   * \return
   * The number of nanos the slave sleeps when nothing has to be done.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_f_list_buffer_slave_wait(void);

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
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_f_list_buffer_slave_wait(const atrshmlog_int32_t i_wait_nanos);


  /**
   * \brief Get the wait time in nanoseconds for the slave if shm full.
   *
   * \return
   * The number of nanos the slave sleepswhen shm full
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_slave_to_shm_wait(void);


  /**
   * \brief Set the wait time for the slaves in nanoseconds.
   *
   * \param i_wait_nanos
   * The number of nanos the slave has to sleep when shm full.
   * to be done.
   *
   * \return
   * The old number of nanos the slave had to wait
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_slave_to_shm_wait(const atrshmlog_int32_t i_wait_nanos);


  /**
   * \brief Get the time for mem to shm
   *
   * \return
   * The number of nanos 
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_last_mem_to_shm(void);

  /* shared memory area related functions */
  
  /**
   * \brief The area buffer in shm start adress.
   *
   * \return
   * Points to the shared memory area.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  volatile const void* atrshmlog_get_area(void);

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
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_area_ich_habe_fertig(volatile const void* const i_area);

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
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_set_area_ich_habe_fertig(volatile const void* const i_area, const atrshmlog_int32_t i_flag);

  
  /**
   * \brief The count of buffers in the shm log area.
   *
   * \param i_area
   * Points to the start of the shared memory area.
   *
   * \return
   * The number of the version in the shared memory area.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_area_count(volatile const void* const i_area);

  /**
   * \brief The version of the log area in shm.
   *
   * \param i_area
   * Points to the start of the shared memory area.
   *
   * \return
   * The number of the version in the shared memory area.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_get_area_version(volatile const void* const i_area);
  /** 
   * \brief We verify the buffer is inited and structural ok .
   *
   * \return
   * - Zero if ok.
   * - Positive for minor error.
   * - Negative for major error.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_verify(void);

  /* statistics functions */

  /**
   * \brief We deliver the max index of the statistics buffer
   *
   * \return 
   * Maximum index of the statistics buffer.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_int32_t atrshmlog_get_statistics_max_index(void);

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
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  void atrshmlog_get_statistics(atrshmlog_int32_t* const o_target);

  /* reader transfer functions */

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
   *  \param  o_number_dispatched
   * Points to an int32 that holds the number of dispatch from the 
   * thread.
   *
   *  \param o_counter_write0
   * Statistic value for counter_write0
   *
   *  \param o_counter_write0_discard
   * Statistic value for counter_write0_discard
   *
   *  \param o_counter_write0_wait
   * Statistic value for counter_write0_wait
   *
   *  \param o_counter_write0_adaptive
   * Statistic value for counter_write0_adaptive
   *
   *  \param o_counter_write0_adaptive_fast
   * Statistic value for counter_write0_adaptive_fast
   *
   *  \param o_counter_write0_adaptive_very_fast
   * Statistic value for counter_write0_adaptive_very_fast
   *
   *  \param o_counter_write1
   * Statistic value for counter_write1
   *
   *  \param o_counter_write1_discard
   * Statistic value for counter_write1_discard
   *
   *  \param o_counter_write1_wait
   * Statistic value for counter_write1_wait
   *
   *  \param o_counter_write1_adaptive
   * Statistic value for counter_write1_adaptive
   *
   *  \param o_counter_write1_adaptive_fast
   * Statistic value for counter_write1_adaptive_fast
   *
   *  \param o_counter_write1_adaptive_very_fast
   * Statistic value for counter_write1_adaptive_very_fast
   *
   *  \param o_counter_write2
   * Statistic value for counter_write2
   *
   *  \param o_counter_write2_discard
   * Statistic value for counter_write2_discard
   *
   *  \param o_counter_write2_wait
   * Statistic value for counter_write2_wait
   *
   *  \param o_counter_write2_adaptive
   * Statistic value for counter_write2_adaptive
   *
   *  \param o_counter_write2_adaptive_fast
   * Statistic value for counter_write2_adaptive_fast
   *
   *  \param o_counter_write2_adaptive_very_fast
   * Statistic value for counter_write2_adaptive_very_fast
   *
   * \return
   * - Negative if an error occured.
   * - Zero if a buffer could be fetched.
   * - Positiv if the call didnt fetch a buffer. In this case the 
   *   buffer length is 0 and all other values are meaningless.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_read(volatile const void* const i_area,
				 const atrshmlog_int32_t i_index_buffer,
				 void * const o_target,
				 atrshmlog_int32_t* const o_target_length,
				 atrshmlog_pid_t* const o_pid,
				 atrshmlog_tid_t* const o_tid,
				 atrshmlog_internal_time_t* const o_inittime,
				 atrshmlog_time_t* const o_inittimetsc_before,
				 atrshmlog_time_t* const o_inittimetsc_after,
				 atrshmlog_internal_time_t* const o_lasttime,
				 atrshmlog_time_t* const o_lasttimetsc_before,
				 atrshmlog_time_t* const o_lasttimetsc_after,
				 atrshmlog_time_t* const o_difftimetransfer,
				 atrshmlog_time_t* const o_starttransfer,
				 atrshmlog_time_t* const o_acquiretime,
				 atrshmlog_int32_t* const o_id,
				 atrshmlog_int32_t* const o_number_dispatched,
				 atrshmlog_int32_t* const o_counter_write0,
				 atrshmlog_int32_t* const o_counter_write0_discard,
				 atrshmlog_int32_t* const o_counter_write0_wait,
				 atrshmlog_int32_t* const o_counter_write0_adaptive,
				 atrshmlog_int32_t* const o_counter_write0_adaptive_fast,
				 atrshmlog_int32_t* const o_counter_write0_adaptive_very_fast,
				 atrshmlog_int32_t* const o_counter_write1,
				 atrshmlog_int32_t* const o_counter_write1_discard,
				 atrshmlog_int32_t* const o_counter_write1_wait,
				 atrshmlog_int32_t* const o_counter_write1_adaptive,
				 atrshmlog_int32_t* const o_counter_write1_adaptive_fast,
				 atrshmlog_int32_t* const o_counter_write1_adaptive_very_fast,
				 atrshmlog_int32_t* const o_counter_write2,
				 atrshmlog_int32_t* const o_counter_write2_discard,
				 atrshmlog_int32_t* const o_counter_write2_wait,
				 atrshmlog_int32_t* const o_counter_write2_adaptive,
				 atrshmlog_int32_t* const o_counter_write2_adaptive_fast,
				 atrshmlog_int32_t* const o_counter_write2_adaptive_very_fast
				 );

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
   *  \param  o_number_dispatched
   * Points to an int32 that holds the number of dispatch from the 
   * thread.
   *
   *  \param o_counter_write0
   * Statistic value for counter_write0
   *
   *  \param o_counter_write0_discard
   * Statistic value for counter_write0_discard
   *
   *  \param o_counter_write0_wait
   * Statistic value for counter_write0_wait
   *
   *  \param o_counter_write0_adaptive
   * Statistic value for counter_write0_adaptive
   *
   *  \param o_counter_write0_adaptive_fast
   * Statistic value for counter_write0_adaptive_fast
   *
   *  \param o_counter_write0_adaptive_very_fast
   * Statistic value for counter_write0_adaptive_very_fast
   *
   *  \param o_counter_write1
   * Statistic value for counter_write1
   *
   *  \param o_counter_write1_discard
   * Statistic value for counter_write1_discard
   *
   *  \param o_counter_write1_wait
   * Statistic value for counter_write1_wait
   *
   *  \param o_counter_write1_adaptive
   * Statistic value for counter_write1_adaptive
   *
   *  \param o_counter_write1_adaptive_fast
   * Statistic value for counter_write1_adaptive_fast
   *
   *  \param o_counter_write1_adaptive_very_fast
   * Statistic value for counter_write1_adaptive_very_fast
   *
   *  \param o_counter_write2
   * Statistic value for counter_write2
   *
   *  \param o_counter_write2_discard
   * Statistic value for counter_write2_discard
   *
   *  \param o_counter_write2_wait
   * Statistic value for counter_write2_wait
   *
   *  \param o_counter_write2_adaptive
   * Statistic value for counter_write2_adaptive
   *
   *  \param o_counter_write2_adaptive_fast
   * Statistic value for counter_write2_adaptive_fast
   *
   *  \param o_counter_write2_adaptive_very_fast
   * Statistic value for counter_write2_adaptive_very_fast
   *
   * \return
   * - Negative if an error occured.
   * - Zero if a buffer could be fetched.
   * - Positiv if the call didnt fetch a buffer. In this case the 
   *   buffer length is 0 and all other values are meaningless.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_ret_t atrshmlog_read_fetch(volatile const void* const i_area,
				       atrshmlog_int32_t * const o_index_buffer,
				       void * const o_target,
				       atrshmlog_int32_t * const o_target_length,
				       atrshmlog_pid_t* const o_pid,
				       atrshmlog_tid_t* const o_tid,
				       atrshmlog_internal_time_t* const o_inittime,
				       atrshmlog_time_t* const o_inittimetsc_before,
				       atrshmlog_time_t* const o_inittimetsc_after,
				       atrshmlog_internal_time_t* const o_lasttime,
				       atrshmlog_time_t* const o_lasttimetsc_before,
				       atrshmlog_time_t* const o_lasttimetsc_after,
				       atrshmlog_time_t* const o_difftimetransfer,
				       atrshmlog_time_t* const o_starttransfer,
				       atrshmlog_time_t* const o_acquiretime,
				       atrshmlog_int32_t* const o_id,
				       atrshmlog_int32_t* const o_number_dispatched,
				       atrshmlog_int32_t* const o_counter_write0,
				       atrshmlog_int32_t* const o_counter_write0_discard,
				       atrshmlog_int32_t* const o_counter_write0_wait,
				       atrshmlog_int32_t* const o_counter_write0_adaptive,
				       atrshmlog_int32_t* const o_counter_write0_adaptive_fast,
				       atrshmlog_int32_t* const o_counter_write0_adaptive_very_fast,
				       atrshmlog_int32_t* const o_counter_write1,
				       atrshmlog_int32_t* const o_counter_write1_discard,
				       atrshmlog_int32_t* const o_counter_write1_wait,
				       atrshmlog_int32_t* const o_counter_write1_adaptive,
				       atrshmlog_int32_t* const o_counter_write1_adaptive_fast,
				       atrshmlog_int32_t* const o_counter_write1_adaptive_very_fast,
				       atrshmlog_int32_t* const o_counter_write2,
				       atrshmlog_int32_t* const o_counter_write2_discard,
				       atrshmlog_int32_t* const o_counter_write2_wait,
				       atrshmlog_int32_t* const o_counter_write2_adaptive,
				       atrshmlog_int32_t* const o_counter_write2_adaptive_fast,
				       atrshmlog_int32_t* const o_counter_write2_adaptive_very_fast
				       );

  /* end real functions */

  /**************************************************************/
  // inline code
  // We have inline what should be done reallly fast and has no
  // big impact in footprint.
  // So we inline the timing and the get clicktime.
  // We will not inline the write code.

  // Inline code can be a problem. We share this header with c and c++.
  // And with the language layers.
  // So if you encounter problems you can first shut off the thing.

  // Inline should be on for speed in the C and C++ world.
  // If you really sure it is of help you can do it in the layers too.
  

  /*
   * our internal helpers. we need them in the testdrivers 
   * for checking the right available call on this box
   */
  #if ATRSHMLOG_INLINE_TSC_CODE == 1

  #if ATRSHMLOG_PLATFORM_LINUX_X86_64_GCC == 1 

  /**
   * We use the modern cpu version of reading the click counter
   * register tsc.
   *
   * \return
   * The 64 bit number with the tick count.
   */
  ATRSHMLOG_INLINE uint64_t  atrshmlog_get_tsc_par_x86_64_gnu(void)
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
  ATRSHMLOG_INLINE uint64_t  atrshmlog_get_tsc_fence_x86_64_gnu(void)
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
  ATRSHMLOG_INLINE  uint64_t atrshmlog_get_tsc_x86_64_gnu(void)
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
  ATRSHMLOG_INLINE  uint64_t  atrshmlog_get_tsc_null_x86_64_gnu(void)
  {
    return 0;
  }
#endif
  // linux x86_64_gnu
  
#if ATRSHMLOG_PLATFORM_CYGWIN_X86_64_GCC == 1 

  /**
   * We use the modern cpu version of reading the click counter
   * register tsc.
   *
   * \return
   * The 64 bit number with the tick count.
   */
  ATRSHMLOG_INLINE uint64_t  atrshmlog_get_tsc_par_x86_64_gnu(void)
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
  ATRSHMLOG_INLINE uint64_t  atrshmlog_get_tsc_fence_x86_64_gnu(void)
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
  ATRSHMLOG_INLINE  uint64_t atrshmlog_get_tsc_x86_64_gnu(void)
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
  ATRSHMLOG_INLINE  uint64_t  atrshmlog_get_tsc_null_x86_64_gnu(void)
  {
    return 0;
  }
#endif
  // cygwin x86_64_gnu
  
#if ATRSHMLOG_PLATFORM_MINGW_X86_64_GCC == 1 

  /**
   * We use the modern cpu version of reading the click counter
   * register tsc.
   *
   * \return
   * The 64 bit number with the tick count.
   */
  ATRSHMLOG_INLINE uint64_t  atrshmlog_get_tsc_par_x86_64_gnu(void)
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
  ATRSHMLOG_INLINE uint64_t  atrshmlog_get_tsc_fence_x86_64_gnu(void)
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
  ATRSHMLOG_INLINE  uint64_t atrshmlog_get_tsc_x86_64_gnu(void)
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
  ATRSHMLOG_INLINE  uint64_t  atrshmlog_get_tsc_null_x86_64_gnu(void)
  {
    return 0;
  }
#endif
  // mingw x86_64_gnu
  
#if ATRSHMLOG_PLATFORM_BSD_AMD64_CLANG == 1 

  /**
   * We use the modern cpu version of reading the click counter
   * register tsc.
   *
   * \return
   * The 64 bit number with the tick count.
   */
  ATRSHMLOG_INLINE uint64_t  atrshmlog_get_tsc_par_amd64_clang(void)
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
  ATRSHMLOG_INLINE uint64_t  atrshmlog_get_tsc_fence_amd64_clang(void)
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
  ATRSHMLOG_INLINE  uint64_t atrshmlog_get_tsc_amd64_clang(void)
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
  ATRSHMLOG_INLINE  uint64_t  atrshmlog_get_tsc_null_amd64_clang(void)
  {
    return 0;
  }
#endif
  // bsd amd64 clang
  
#if ATRSHMLOG_PLATFORM_BSD_AMD64_GCC == 1 

  /**
   * We use the modern cpu version of reading the click counter
   * register tsc.
   *
   * \return
   * The 64 bit number with the tick count.
   */
  ATRSHMLOG_INLINE uint64_t  atrshmlog_get_tsc_par_amd64_gnu(void)
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
  ATRSHMLOG_INLINE uint64_t  atrshmlog_get_tsc_fence_amd64_gnu(void)
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
  ATRSHMLOG_INLINE  uint64_t atrshmlog_get_tsc_amd64_gnu(void)
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
  ATRSHMLOG_INLINE  uint64_t  atrshmlog_get_tsc_null_amd64_gnu(void)
  {
    return 0;
  }
#endif
  // bsd amd64 gnu

#if ATRSHMLOG_PLATFORM_SOLARIS_X86_64_GCC == 1 

  /**
   * We use the modern cpu version of reading the click counter
   * register tsc.
   *
   * \return
   * The 64 bit number with the tick count.
   */
  ATRSHMLOG_INLINE uint64_t  atrshmlog_get_tsc_par_x86_64_gnu(void)
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
  ATRSHMLOG_INLINE uint64_t  atrshmlog_get_tsc_fence_x86_64_gnu(void)
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
  ATRSHMLOG_INLINE  uint64_t atrshmlog_get_tsc_x86_64_gnu(void)
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
  ATRSHMLOG_INLINE  uint64_t  atrshmlog_get_tsc_null_x86_64_gnu(void)
  {
    return 0;
  }
#endif
  // solaris x86_64_gnu

#else

  // no inline, prototypes needed
# if ATRSHMLOG_PLATFORM_LINUX_X86_64_GCC == 1 
  
  /**
   * We use the modern cpu version of reading the click counter
   * register tsc.
   *
   * \return
   * The 64 bit number with the tick count.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  uint64_t atrshmlog_get_tsc_par_x86_64_gnu(void);

  /**
   * We use the old cpu version of reading the click counter
   * register tsc. We use a lfence for multicore here.
   *
   * \return
   * The 64 bit number with the tick count.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  uint64_t atrshmlog_get_tsc_fence_x86_64_gnu(void);

  /**
   * We use the old cpu version of reading the click counter
   * register tsc. No syncronization here.
   *
   * \return
   * The 64 bit number with the tick count.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  uint64_t atrshmlog_get_tsc_x86_64_gnu(void);

  /**
   * The well know dummy 
   * \return 
   * Null.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  uint64_t atrshmlog_get_tsc_null_x86_64_gnu(void);

# endif
  // linux 86 64 gnu

    // no inline, prototypes needed
# if ATRSHMLOG_PLATFORM_CYGWIN_X86_64_GCC == 1 
  
  /**
   * We use the modern cpu version of reading the click counter
   * register tsc.
   *
   * \return
   * The 64 bit number with the tick count.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  uint64_t atrshmlog_get_tsc_par_x86_64_gnu(void);

  /**
   * We use the old cpu version of reading the click counter
   * register tsc. We use a lfence for multicore here.
   *
   * \return
   * The 64 bit number with the tick count.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  uint64_t atrshmlog_get_tsc_fence_x86_64_gnu(void);

  /**
   * We use the old cpu version of reading the click counter
   * register tsc. No syncronization here.
   *
   * \return
   * The 64 bit number with the tick count.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  uint64_t atrshmlog_get_tsc_x86_64_gnu(void);

  /**
   * The well know dummy 
   * \return 
   * Null.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  uint64_t atrshmlog_get_tsc_null_x86_64_gnu(void);

# endif
  // cygwin 86 64 gnu

  // no inline, prototypes needed
# if ATRSHMLOG_PLATFORM_MINGW_X86_64_GCC == 1 
  
  /**
   * We use the modern cpu version of reading the click counter
   * register tsc.
   *
   * \return
   * The 64 bit number with the tick count.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  uint64_t atrshmlog_get_tsc_par_x86_64_gnu(void);

  /**
   * We use the old cpu version of reading the click counter
   * register tsc. We use a lfence for multicore here.
   *
   * \return
   * The 64 bit number with the tick count.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  uint64_t atrshmlog_get_tsc_fence_x86_64_gnu(void);

  /**
   * We use the old cpu version of reading the click counter
   * register tsc. No syncronization here.
   *
   * \return
   * The 64 bit number with the tick count.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  uint64_t atrshmlog_get_tsc_x86_64_gnu(void);

  /**
   * The well know dummy 
   * \return 
   * Null.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  uint64_t atrshmlog_get_tsc_null_x86_64_gnu(void);

# endif
  // mingw 86 64 gnu


# if ATRSHMLOG_PLATFORM_BSD_AMD64_CLANG == 1 
  
  /**
   * We use the modern cpu version of reading the click counter
   * register tsc.
   *
   * \return
   * The 64 bit number with the tick count.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  uint64_t atrshmlog_get_tsc_par_amd64_clang(void);

  /**
   * We use the old cpu version of reading the click counter
   * register tsc. We use a lfence for multicore here.
   *
   * \return
   * The 64 bit number with the tick count.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  uint64_t atrshmlog_get_tsc_fence_amd64_clang(void);

  /**
   * We use the old cpu version of reading the click counter
   * register tsc. No syncronization here.
   *
   * \return
   * The 64 bit number with the tick count.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  uint64_t atrshmlog_get_tsc_amd64_clang(void);

  /**
   * The well know dummy 
   * \return 
   * Null.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  uint64_t atrshmlog_get_tsc_null_amd64_clang(void);

# endif
  // bsd amd64 clang

# if ATRSHMLOG_PLATFORM_BSD_AMD64_GCC == 1 
  
  /**
   * We use the modern cpu version of reading the click counter
   * register tsc.
   *
   * \return
   * The 64 bit number with the tick count.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  uint64_t atrshmlog_get_tsc_par_amd64_gnu(void);

  /**
   * We use the old cpu version of reading the click counter
   * register tsc. We use a lfence for multicore here.
   *
   * \return
   * The 64 bit number with the tick count.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  uint64_t atrshmlog_get_tsc_fence_amd64_gnu(void);

  /**
   * We use the old cpu version of reading the click counter
   * register tsc. No syncronization here.
   *
   * \return
   * The 64 bit number with the tick count.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  uint64_t atrshmlog_get_tsc_amd64_gnu(void);

  /**
   * The well know dummy 
   * \return 
   * Null.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  uint64_t atrshmlog_get_tsc_null_amd64_gnu(void);

# endif
  // bsd amd64 clang

# if ATRSHMLOG_PLATFORM_SOLARIS_X86_64_GCC == 1 
  
  /**
   * We use the modern cpu version of reading the click counter
   * register tsc.
   *
   * \return
   * The 64 bit number with the tick count.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  uint64_t atrshmlog_get_tsc_par_x86_64_gnu(void);

  /**
   * We use the old cpu version of reading the click counter
   * register tsc. We use a lfence for multicore here.
   *
   * \return
   * The 64 bit number with the tick count.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  uint64_t atrshmlog_get_tsc_fence_x86_64_gnu(void);

  /**
   * We use the old cpu version of reading the click counter
   * register tsc. No syncronization here.
   *
   * \return
   * The 64 bit number with the tick count.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  uint64_t atrshmlog_get_tsc_x86_64_gnu(void);

  /**
   * The well know dummy 
   * \return 
   * Null.
   */
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  uint64_t atrshmlog_get_tsc_null_x86_64_gnu(void);

# endif
  // SOLARIS 86 64 gnu


#endif

#if ATRSHMLOG_INLINE_GETTIME == 1  
  
# if ATRSHMLOG_PLATFORM_LINUX_X86_64_GCC == 1 


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
  ATRSHMLOG_INLINE atrshmlog_time_t  atrshmlog_get_clicktime(void)
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

# if ATRSHMLOG_PLATFORM_CYGWIN_X86_64_GCC == 1 


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
  ATRSHMLOG_INLINE atrshmlog_time_t  atrshmlog_get_clicktime(void)
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

# if ATRSHMLOG_PLATFORM_MINGW_X86_64_GCC == 1 


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
  ATRSHMLOG_INLINE atrshmlog_time_t  atrshmlog_get_clicktime(void)
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

# if ATRSHMLOG_PLATFORM_BSD_AMD64_CLANG == 1 


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
  ATRSHMLOG_INLINE atrshmlog_time_t  atrshmlog_get_clicktime(void)
  {
    if (atrshmlog_clock_id == 3)
      {
	return (atrshmlog_time_t)atrshmlog_get_tsc_par_amd64_clang();
      }
    
    if (atrshmlog_clock_id == 2)
      {
	return (atrshmlog_time_t)atrshmlog_get_tsc_fence_amd64_clang();
      }
    
    if (atrshmlog_clock_id == 1)
      {
	return (atrshmlog_time_t)atrshmlog_get_tsc_amd64_clang();
      }
    
    return (atrshmlog_time_t)atrshmlog_get_tsc_null_amd64_clang();
  }

# endif

# if ATRSHMLOG_PLATFORM_BSD_AMD64_GCC == 1 


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
  ATRSHMLOG_INLINE atrshmlog_time_t  atrshmlog_get_clicktime(void)
  {
    if (atrshmlog_clock_id == 3)
      {
	return (atrshmlog_time_t)atrshmlog_get_tsc_par_amd64_gnu();
      }
    
    if (atrshmlog_clock_id == 2)
      {
	return (atrshmlog_time_t)atrshmlog_get_tsc_fence_amd64_gnu();
      }
    
    if (atrshmlog_clock_id == 1)
      {
	return (atrshmlog_time_t)atrshmlog_get_tsc_amd64_gnu();
      }
    
    return (atrshmlog_time_t)atrshmlog_get_tsc_null_amd64_gnu();
  }

# endif

# if ATRSHMLOG_PLATFORM_SOLARIS_X86_64_GCC == 1 


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
  ATRSHMLOG_INLINE atrshmlog_time_t  atrshmlog_get_clicktime(void)
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
  ATRSHMLOG_FUNCTION_DECORATOR  extern
  atrshmlog_time_t atrshmlog_get_clicktime(void);
  
#endif
  
  
#ifdef __cplusplus
}
#endif
  
#endif

/* end of file */
