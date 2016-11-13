#ifndef INCLUDED_ATRSHMLOG_H
#define INCLUDED_ATRSHMLOG_H

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


#ifdef __cplusplus
extern "C" {
#endif
  
  // for the mksh hack
#define   ATRSHMLOGENTRYSIZELIMIT (255)
/*
 * we need memory to hold a time information
 * but we dont want to use a type directly
 * so we define this vanilla time info 
 * its big enough for all major os 
 * and it has a reserve of factor 2
 * so i am confident that it will be some time till
 * this is no longer valid to hold
 * any time type that is used in logging
 * if thats the case - simply make it big enough..
 * and then wait till the technologie reach the limit again
 * for now we have a simple type to be used in our
 * logging without depending on any internal info...
 */

struct atrshmlog_time_s {
  char buffer[32];
};

typedef struct atrshmlog_time_s atrshmlog_time_t;

  
/*
 *----------------------------------------------------------
 */

/* 
 * we need to know this to check if an event is to be used 
 * events are strictly spoken position parameters for the 
 * system we want to log in
 * so this could be some number
 * internal there is a buffer with that many flags - in char
 * type, so the cpu dont have to do much to check it to be 
 * 0 or not....
 * we use then the pointer to the buffer and the 
 * index in the buffer as check informations.
 * well, the index is given in form of an int...
 * thats big enough for many evnets.
 * but for now the limit is about 10000 ...
 * se impl for this.
 * 
 * we need that pointer directly, so its the minimum overhead 
 * if logging is OFF ....
 * please dont try to write to the buffer until you know what you are doing
 * its possible to do it because there are  szenarios
 * when an application should be allowed to stear the logging
 * by itself, for example by getting a parameter file in the 
 * application or an context of some db infos or or or
 * 
 * this is of course possible by use of the helpers
 * set event and clear event...
 *
 */
extern volatile const char* const atrshmlog_event_locks;

 /*
  * temporary logging on off switch for process
  */
extern volatile const int atrshmlog_logging_process;

/************************************************************************/
/* helper macros */

  /* 
   * we use a speed up here, binary and ... 
   * this is ahelper in fact, but if you want to make something dependent
   * from an event and have not timeto use the 
   * procedure interface, use this ...
   */
#define ATRSHMLOGEVENTCHECK(__ev) \
  ((atrshmlog_event_locks[(__ev)]) & atrshmlog_logging_process)

  /*
   * this is normal used way to get a time stamp into a time buffer.
   * we ignore here the speed for the use of the cleaner interface
   * to say this simple: a gettime needs rough between 30 to 400 nanos,
   * a function call is cheap compared to this...
   */
#define ATRSHMLOGGETTIME(__ev,__x)\
  do {\
    if ( ATRSHMLOGEVENTCHECK((__ev)) )   \
      atrshmlog_gettime(&(__x));    \
  } while (1 != 1)

  /*
   * the writing of a log entry, all in all covered for now
   */
#define ATRSHMLOGWRITE(__ev,__evf,__usr,__s,__e,__b,__sz)\
    ( ATRSHMLOGEVENTCHECK((__ev)) ?				\
      (atrshmlog_write)((__ev),(__evf),(__usr),&(__s),&(__e),(__b),(__sz),\
			NULL,0) : -99 					\
    )

  /*
   * the second way to log - some primitive dynamic way with a char* array...
   * should be clear for a c programer.
   * for the rest:
   * use an array of pointers.
   * put at last position a NULL pointer.
   * then deliver the start adress of that array to the macro
   * if you have the count of pointers you can deliver that too
   * if not use a 0 or negative number. 
   * in that case the null pointer is MUST and not a nice ...
   */
#define ATRSHMLOGWRITE2(__ev,__evf,__usr,__s,__e,__b,__sz,__b2,__sz2)	\
    ( ATRSHMLOGEVENTCHECK((__ev)) ?				\
      (atrshmlog_write)((__ev),(__evf),(__usr),&(__s),&(__e),(__b),(__sz),\
			(__b2),(__sz2)) :  -99				\
    )



/************************************************************************/

/* the funcs */

/* we connect to the shm buffer. only connect, no use */
extern int atrshmlog_attach();

/* we disconnect from the buffer */
extern void atrshmlog_detach();


/* we write to the log */
extern int atrshmlog_write(const int i_eventnumber,
			    const int i_eventflag,
			    const int i_userflag,
			    const atrshmlog_time_t* i_starttime,
			    const atrshmlog_time_t* i_endtime,
			    const void* i_local,
			    const int i_size,
			    const char* i_argv[],
			    const int i_argc_hint);


/* we get a timestamp from the os */
extern void atrshmlog_gettime(atrshmlog_time_t* o_t);

/* we set the prefix for name lookup in the program */
extern void atrshmlog_set_prefix (const char*i_prefix);

/* we get the actual name of the environment variable holding the shm id */  
extern const char* atrshmlog_the_env();

/* we get the value of the environment variable holding the shmid in text */  
extern char* atrshmlog_getenv();

/* the version of the buffer system 
 * if we change anything of the buffer layout we have to change this too
 */   
extern int atrshmlog_version();

/*
 * the functionality of the version f the log
 * we can have diffrent versions here if only the interface
 * is up compatible and the buffer layout is the same
 */  
extern int atrshmlog_minor_version();

/*
 * this is the patch level, it inform of important changes in functionality 
 * for rem oving errors from the log
 */  
extern int atrshmlog_patch_version();

/*
 * helper to set a event from the program
 */  
extern void atrshmlog_set_event(int index);

/*
 * helper to clear an event from the program
 */
extern void atrshmlog_clear_event(int index);

/*
 * deliver the value of the event  - its normally 0 or 1
 * 0 mean to ignore the things based on this event
 * 1 mean to do the things...
 * all others are ormally errors ... 
 */
extern int atrshmlog_get_event(int index);

/*
 * deliver logging state for the program
 */
extern int atrshmlog_logging();
  
/*
 * switch the logging for the process on 
 */  
extern void atrshmlog_logging_on();

/*
 * switch the logging for the process off
 */
extern void atrshmlog_logging_off();

/*
 * deliver the shm id of the used buffer
 */  
extern int atrshmlog_shmid();

#ifdef __cplusplus
}
#endif
  
#endif

/* end of file */
