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

/** \file atrshmlog.c 
 * \brief The implementation of the module
 *
 * We implement The module in one file.
 * This makes thigs simple for source code inclusions and 
 * leaves all open for the user.
 * In a later version we might create multiple files and 
 * add a lib and a shared lib module too.
 *
 * This code needs at least a C11 compiler.
 * With atomics.
 * For posix systems with pthreds.
 * Rest is open as an exercise to the user.
 */

/* our includes are here */

#include "atrshmlog_internal.h"

/* end of includes */

/**************************************************************/

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
struct tbuff_s {
  /** 
   * The next in line for cleanup processing or 0.
   * This is only once setted static or in the alloc. 
   */
  struct tbuff_s* next_cleanup;

  /** 
   * The next in line for full processing or 0.
   */
  struct tbuff_s *next_full;

  /** 
   * The next in line for allocate or 0. 
   */
  struct tbuff_s* next_append;

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
   * The last transfer to shm time.
   * We use this in the adaptive strategy.
   */
  atrshmlog_time_t transfer_to_shm_time;
  
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
   * The memory for the payload starts there.
   */
  char *b;
};

/** We define the type of the struct here */
typedef struct tbuff_s tbuff_t;


// end of local defined types


/***************************************************************/


/** 
 * \n Main code:
 *
 * \brief This is the base address of the used shm buffer .
 *
 * This is read to determine if the logging is attached
 * to a buffer. If not it is NULL .
 * So this is a read only for all users .
 *
 * Only reason to make it available it to speed up the macros.
 * Ok, its posible to use inline functions today, but they are
 * problematic if you try to hide such a thing as the pointer - 
 * you come from hiding it to the fact that the  impl of your
 * inline has to be known anyway. So its not a real better solution ..
 *
 * Simply use it as its declared : volatile , never cache it
 * never change it - use it like its const ... 
 *
 * Use it as it where readonly.
 */
volatile const void* atrshmlog_base_ptr = 0;


/********************************************************************/

/** 
 * \n Main code:
 *
 * \brief We save the shm id here for checks.
 *
 * We get it from the attach op.
 * Its the ID from the environment.
 *
 * This can be different to the shm id in the buffer ( which indicate 
 * an error - we never should use a shm buffer with the wrong id
 * because it is a strong hint someone gave a wrong shmid to the init
 * and we dont use that shm then ).
 *
 * Use this as it where readonly.
 */
static volatile int atrshmlog_id = 0;


/********************************************************************/

/**
 * \n Main code:
 *
 * \brief The global pid of the attach call.
 *
 * We are checked in a thread init against our pid.
 *
 * So if we are different we can assume we are a fork clone,
 * still not overwritten by an exec.
 *
 * In this case we must start the new slaves and put our pid 
 * in this global as the new process identifyer if WE fork and then clone ...
 *
 * This a cheap trick, so i assume there is at least one way
 * to turn it upside down, but for now it is our way
 * to detect a fork ...
 * So dont try to trick this by u self.
 *
 * Use it as it where readonly.
 */
static volatile atrshmlog_pid_t atrshmlog_attach_pid;

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
struct g_tl_s {

  /** 
   * This is used as an initialized flag and a not ok flag .
   * For the initialize it must be -1. 
   * After the initialize it is 0 for ok and not -1 and not 0 for an error.
   * As a side effect we stop logging for a thread by setting it 
   * to a non 0 value and free the buffers. 
   * See the atrshmlog_stop for this.
   */
  int atrshmlog_idnotok;
  
  /** 
   * The used buffers for a thread.
   * We use two buffers concurrently for now.
   * Later we will switch to a n buffer design if the tests are happy with it.
   * If you think its ok you can redefine the constant.
   */
  tbuff_t* atrshmlog_targetbuffer_arr[ATRSHMLOGTARGETBUFFERMAX];
  
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
};

/** the type for the thread locals struct */
typedef struct g_tl_s g_tl_t;

/**
 * \brief The one struct containing all thread locals.
 *
 * We do this because the lookup of thread locals 
 * is relative expensive compared to the use of a 
 * simple struct pointer.
 *
 * So we put all those in one struct and use this from 
 * the initial get on.
 */
_Thread_local static g_tl_t g_tl = { .atrshmlog_idnotok = -1,
				     .atrshmlog_targetbuffer_arr = { 0 },
				     0
                                    };

/*******************************************************************/

/** 
 * \n Main code:
 *
 * \brief The number of buffers we have - theoretically  - in use now.
 *
 * At start its the number of buffers on the static buffer list.
 * Later on this can be growing in case we need dyn memory .
 */
static volatile int atrshmlog_acquire_count = ATRSHMLOGBUFFER_PREALLOCED_COUNT;


/********************************************************************/

/**
 * \n Main code:
 *
 * \brief The actual last given id for a buffer.
 *
 * This starts with the preallocated and is raising with every dyn
 * allocated buffer in the connect function.
 * So in theory they are diffrent but after the connect should be same.
*/
static volatile int atrshmlog_buffer_id = ATRSHMLOGBUFFER_PREALLOCED_COUNT;

/********************************************************************/

/* some helper macros */

/**
 * \brief We set the buffer element to defaults.
 *
 * The only variable thing is the index.
 *
 * \param __x
 * Index of the buffer in the array
 *
 * \return
 * void
 */
#define ATRSHMLOGBUFFERS_PRE(__x) \
  { .next_cleanup = &atrshmlog_buffers_prealloced[(__x) + 1], \
    .next_full =  0, \
    .next_append =  &atrshmlog_buffers_prealloced[(__x) + 1],	 \
    .safeguardfront =  0, \
    .pid =  0, \
    .tid =  0, \
    .acquiretime =   0, \
    .id =  (__x) + 1, \
    .size =  0, \
    .maxsize =  ATRSHMLOGBUFFER_INFOSIZE, \
    .dispose =   ATRSHMLOG_DISPATCH_REUSE, \
    .dispatched =  ATOMIC_VAR_INIT(0) , \
    .b =  &atrshmlog_buffers_prealloced_chunks[(__x) * ATRSHMLOGBUFFER_INFOSIZE ] \
      }

/**
 * \brief We set the buffer element to defaults for the last element.
 *
 * The only variable thing is the index.
 * We have to set the link fields to 0.
 *
 * \param __x
 * Index of the buffer in the array
 *
 * \return
 * void
 */
#define ATRSHMLOGBUFFERS_PRE_LAST(__x) \
  { .next_cleanup = 0,		       \
    .next_full =  0, \
    .next_append =  0, \
    .safeguardfront =  0, \
    .pid =  0, \
    .tid =  0, \
    .acquiretime =  0, \
    .id =  (__x) + 1, \
    .size =  0, \
    .maxsize =  ATRSHMLOGBUFFER_INFOSIZE, \
    .dispose =  ATRSHMLOG_DISPATCH_REUSE, \
    .dispatched =  ATOMIC_VAR_INIT(0) , \
    .b =  &atrshmlog_buffers_prealloced_chunks[(__x) * ATRSHMLOGBUFFER_INFOSIZE ] \
      }

/**
 * \n Main code:
 *
 * \brief The memory for the first buffers to hold log info.
 * 
 * We use static allocated memory here, so the startup is
 * slowed down a bit.
 *
 * This memory is then connected in the descriptors.
 */
static char atrshmlog_buffers_prealloced_chunks[ATRSHMLOGBUFFER_PREALLOCED_COUNT * ATRSHMLOGBUFFER_INFOSIZE ];

/**
 * \n Main code:
 *
 * \brief This is the descriptor list for the static allocated
 * buffers.
 *
 * We use a fully static approch.
 *
 * This has one problem.
 * If you change the define for
 * ATRSHMLOGBUFFER_PREALLOCED_COUNT
 * you have to change the code here, too.
 *
 * The number of buffers must match
 * and so the number of buffer initializations too.
 *
 * So you have to take care for the initializers.
 *
 * These are made with the macro
 * ATRSHMLOGBUFFERS_PRE and ATRSHMLOGBUFFERS_PRE_LAST.
 *
 * So dont try to be too clever.
 * Simply adjust here if needed.
 *
 * We use actually 64 buffers in the first run.
 * This could be a little bit overdue in case of simple programs.
 * So try to reduce this if needed.
 * For a single thread program 2 is ok.
 * For a multi thread program you have to decide
 * the more you allocate static, the faster the allocate works
 * for the first threads.
 *
 * If you do too much it simply waste memory.
 * So i have done it for a - theoretical - szenario with 32 threads here
 * That is the max of CPU i have in my big box ...
 * For normal threaded programs its ok to do it this way,
 * for high concurrent threading programs its not enough.
 * But be prepared: you pay a price for using too much static memory
 * in program startup.
 *
 * So decide to target the usual 80 % of need here
 * and if possible use the dispose and free recycle of buffers if
 * you always start new threads - or you will be having a bad memory
 * footprint - its a running fragmenter then 
 *
 */
static tbuff_t atrshmlog_buffers_prealloced[ATRSHMLOGBUFFER_PREALLOCED_COUNT]
= {
  ATRSHMLOGBUFFERS_PRE(0),
  ATRSHMLOGBUFFERS_PRE(1),
  ATRSHMLOGBUFFERS_PRE(2),
  ATRSHMLOGBUFFERS_PRE(3),
  ATRSHMLOGBUFFERS_PRE(4),
  ATRSHMLOGBUFFERS_PRE(5),
  ATRSHMLOGBUFFERS_PRE(6),
  ATRSHMLOGBUFFERS_PRE(7),
  ATRSHMLOGBUFFERS_PRE(8),
  ATRSHMLOGBUFFERS_PRE(9),
  ATRSHMLOGBUFFERS_PRE(10),
  ATRSHMLOGBUFFERS_PRE(11),
  ATRSHMLOGBUFFERS_PRE(12),
  ATRSHMLOGBUFFERS_PRE(13),
  ATRSHMLOGBUFFERS_PRE(14),
  ATRSHMLOGBUFFERS_PRE(15),
  ATRSHMLOGBUFFERS_PRE(16),
  ATRSHMLOGBUFFERS_PRE(17),
  ATRSHMLOGBUFFERS_PRE(18),
  ATRSHMLOGBUFFERS_PRE(19),
  ATRSHMLOGBUFFERS_PRE(20),
  ATRSHMLOGBUFFERS_PRE(21),
  ATRSHMLOGBUFFERS_PRE(22),
  ATRSHMLOGBUFFERS_PRE(23),
  ATRSHMLOGBUFFERS_PRE(24),
  ATRSHMLOGBUFFERS_PRE(25),
  ATRSHMLOGBUFFERS_PRE(26),
  ATRSHMLOGBUFFERS_PRE(27),
  ATRSHMLOGBUFFERS_PRE(28),
  ATRSHMLOGBUFFERS_PRE(29),
  ATRSHMLOGBUFFERS_PRE(30),
  ATRSHMLOGBUFFERS_PRE(31),
  ATRSHMLOGBUFFERS_PRE(32),
  ATRSHMLOGBUFFERS_PRE(33),
  ATRSHMLOGBUFFERS_PRE(34),
  ATRSHMLOGBUFFERS_PRE(35),
  ATRSHMLOGBUFFERS_PRE(36),
  ATRSHMLOGBUFFERS_PRE(37),
  ATRSHMLOGBUFFERS_PRE(38),
  ATRSHMLOGBUFFERS_PRE(39),
  ATRSHMLOGBUFFERS_PRE(40),
  ATRSHMLOGBUFFERS_PRE(41),
  ATRSHMLOGBUFFERS_PRE(42),
  ATRSHMLOGBUFFERS_PRE(43),
  ATRSHMLOGBUFFERS_PRE(44),
  ATRSHMLOGBUFFERS_PRE(45),
  ATRSHMLOGBUFFERS_PRE(46),
  ATRSHMLOGBUFFERS_PRE(47),
  ATRSHMLOGBUFFERS_PRE(48),
  ATRSHMLOGBUFFERS_PRE(49),
  ATRSHMLOGBUFFERS_PRE(50),
  ATRSHMLOGBUFFERS_PRE(51),
  ATRSHMLOGBUFFERS_PRE(52),
  ATRSHMLOGBUFFERS_PRE(53),
  ATRSHMLOGBUFFERS_PRE(54),
  ATRSHMLOGBUFFERS_PRE(55),
  ATRSHMLOGBUFFERS_PRE(56),
  ATRSHMLOGBUFFERS_PRE(57),
  ATRSHMLOGBUFFERS_PRE(58),
  ATRSHMLOGBUFFERS_PRE(59),
  ATRSHMLOGBUFFERS_PRE(60),
  ATRSHMLOGBUFFERS_PRE(61),
  ATRSHMLOGBUFFERS_PRE(62),
  ATRSHMLOGBUFFERS_PRE_LAST(63)
};


/********************************************************************/

/**
 * \n Main code:
 * 
 * \brief The anchor of the buffer s list ( sequential ).
 *
 * We use this in the atexit cleanup for a complete check
 * for the buffers we have created for logging.
 *
 * So we have a simple thing if buffers are not full
 * at end of running the program - we put them on the shm 
 * and so get the rest of logging done.
 *
 * The atexit function moves them to shm and when its done we dont use
 * the logging any more.
 *
 * Thats the theory.
 *
 * Be sure you know what you do if you use the logging 
 * AFTER an exit call ...
 *
 */
static atomic_intptr_t atrshmlog_tps = ATOMIC_VAR_INIT(atrshmlog_buffers_prealloced);


/********************************************************************/

/**
 * \n Main code:
 *
 * \brief The anchor of the a list ( available ).
 *
 * We allocate if needed these buffers in advance.
 * 
 * So the static allocated are first on the list. 
 *
 * When we have consumed all we start to dyn allocate.
 *
 * So from then on the dyn alloc takes preallocated count
 * of buffers in one move on the list.
 *
 * This is to reduce the costs fro allocate.
 *
 * If we dispatch a buffer and set the dispose flag the buffer
 * is dispatched but not simply given back then, but put on the
 * a list too.
 *
 * So we can recycle buffers in case we know the thread ends 
 * by calling the dispatch (in fact there is the 
 * atrshmlog_stop function for that).
 * Be sure you know what you do if you try to use a buffer after a dispose 
 * in the old thread ....
 */
static atomic_intptr_t atrshmlog_tpa = ATOMIC_VAR_INIT(atrshmlog_buffers_prealloced);


/********************************************************************/

/** 
 * \n Main code:
 *
 * \brief This is the anchor for the full list.
 *
 * We use an atomic here, so its safe to do the mt thing.
 * 
 * Its the list of full buffers that must be transfered to shm with the slave
 * threads. 
 * 
 * The slaves check this and then grab a buffer and transfer it
 * to the shm buffers.
 *
 * So we are empty at begining.
 */
static atomic_intptr_t atrshmlog_tpf = ATOMIC_VAR_INIT(0);


/********************************************************************/

/**
 * \n Main code:
 *
 * \brief The size of buffers in char for the malloc. 
 * 
 * If you want to reduce
 * memory use its better to reduce the buffer size to half or quater ...
 *
 * By change of the define ATRSHMLOGBUFFER_INFOSIZE....
 *
 * But you can also use a dynamic approch, its only a problem to
 * get bigger after a reduce, so be warned that you can reduce 
 * memory usage this way by setting it to a smaller value.
 *
 * As always with mt systems : too much too big is a performance
 * killer, so to make it suiteable in size is an option at least
 * worth a try. 
 * 
 * But the major memory should come from the
 * preallocated static, so check this first ....
 * 
 * There is an env in attach to set it external.
 * It works only for the dyn allocated memory.
 */
static int atrshmlog_buffer_infosize = ATRSHMLOGBUFFER_INFOSIZE;


/********************************************************************/

/** A TODO.
 * \n Main code:
 *
 * The last thread id of a slave  after creation.
 *
 * For now we only have the last thread id here.
 * In a future version i think its a good thing
 * to have a fourth list with the threads that are 
 * running as a slave. 
 *
 * But for now its only this last one.
 */
static volatile atrshmlog_tid_t atrshmlog_f_list_buffer_slave;


/********************************************************************/

/**
 * \n Main code:
 *
 * \brief We use this many buffers to be allocated at once in case
 * we need new buffers from the dyn memory area.
 *
 * So we can reduce the overhead by allocate a bunch in one call.
 * This is ok if you need to use the module without recompile.
 * Its better to adjust the static buffers for the performance
 * if you can do the recompile.
 *
 * There is an env in attach to set this.
 */
static  int atrshmlog_prealloc_buffer_count = ATRSHMLOG_INIT_PREALLOC_COUNT;


/********************************************************************/

/**
 * \n Main code:
 *
 * \brief The flag to stop the slaves.
 *
 * This can be used to make a shutdown by 
 * stopping first the slaves, then do the
 * things you want to do without slave threads,
 * and then stop complete.
 *
 * Be sure you know what you do after you decide to stop
 * the slaves. If any thread runs into full buffers after
 * that you have a busy wait thread running.
 */
static volatile int atrshmlog_f_list_buffer_slave_run = 1;


/********************************************************************/

/**
 * \n Main code:
 *
 * \brief The number of slaves threads to start.
 *
 * This is a primitive approch, so dont overdue it.
 * Normally one thread is ok.
 * 
 * If you reach a starvation situation because of the transfer from
 * buffers to the shm you can raise this 
 * but be sure you know what you do.
 * 
 * You have at least to consider the number of buffers in shm here.
 *
 * It makes no sense if you make more threads than buffers available
 * and of course the number of threads in the reader have to
 * be high enough so that the reader can come up or you
 * simply shift the starvation to the shm buffer cleanup.
 *
 * There is an env in attach to set this.
 */
static  int atrshmlog_f_list_buffer_slave_count = 1;


/********************************************************************/

/** 
 * \n Main code:
 *
 * \brief The number of active slave threads.
 *
 * Be carefull to maintain this if you
 * KILL threads.
 */
static atomic_int atrshmlog_f_list_active_slaves = ATOMIC_VAR_INIT(0);


/********************************************************************/

/**
 * \n Main code:
 *
 * \brief We wait that long in a nanosleep in the slave if the f list is empty.
 *
 * There is an env in attach to set this.
 */
static int atrshmlog_f_list_buffer_slave_wait = ATRSHMLOG_INIT_BUFFER_SLAVE_WAIT;


/********************************************************************/

/**
 * \n Main code:
 *
 * \brief We wait this long if we have no shm buffer in the mem to shm function.
 * 
 * So we have at least no busy wait here.
 *
 */
static int atrshmlog_slave_to_shm_wait  = ATRSHMLOG_INIT_SLAVE_TO_SHM_WAIT;

/********************************************************************/

/**
 * \n Main code:
 *
 * \brief We wait this long in a strategy wait  situation when our buffers are full
 */
static int atrshmlog_strategy_wait_wait_time = ATRSHMLOG_STRATEGY_WAIT_WAIT_TIME_DEFAULT;
/********************************************************************/

/**
 * \n Main code:
 *
 * \brief Our simple slave function.
 */

#if ATRSHMLOG_USE_PTHREAD == 1
typedef void* ATRSHMLOG_thread_ret_t;
#endif

#if ATRSHMLOG_USE_C11_THREAD == 1
typedef int ATRSHMLOG_thread_ret_t;
#endif

static ATRSHMLOG_thread_ret_t atrshmlog_f_list_buffer_slave_proc(void* i_arg);

/********************************************************************/

/**
 * \n Main code:
 *
 * \brief This is a flag to wait in atexit for end of all slaves.
 *
 * Normally we dont do this. But if the cleanup
 * seems to interact with still running slaves and you 
 * want to solve this you can set it to non zero.
 *
 * Then you will wait in the atexit till the count of slaves
 * is 0 . 
 *
 * There is an env in attach to set this.
 */
static int atrshmlog_wait_for_slaves = 0;


/********************************************************************/

/**
 * \n Main code:
 *
 * \brief The strategy to handle buffer switching for the process.
 *
 * There are several strategies defined as enum atrshmlog_strategy
 *
 * We use them when we switch buffers and hit the worst case,
 * all full.
 *
 * So we can discard, spin loop, wait loop, and choose for one
 * of the adaptive strategies here.
 *
 * This is the process value, it is a prototype for the threads.
 * So you have to set it diffrent in the thread if you have
 * a thread doing something different.
 *
 * For starters we wait loop.
 */
static int atrshmlog_buffer_strategy = atrshmlog_strategy_wait;



/********************************************************************/

/**
 * \n Main code:
 *
 * \brief Use of fence 1 flag.
 *
 * For the fences see the doc.
 */
static int atrshmlog_thread_fence_1 = 0;


/********************************************************************/

/**
 * \n Main code:
 *
 * \brief Use of fence 2 flag.
 *
 * For the fences see the doc.
 */
static int atrshmlog_thread_fence_2 = 0;


/********************************************************************/

/**
 * \n Main code:
 *
 * \brief Use of fence 3 flag.
 *
 * For the fences see the doc.
 */
static int atrshmlog_thread_fence_3 = 0;


/********************************************************************/

/**
 * \n Main code:
 *
 * \brief Use of fence 4 flag.
 *
 * For the fences see the doc.
 */
static int atrshmlog_thread_fence_4 = 0;


/********************************************************************/

/**
 * \n Main code:
 *
 * \brief Use of fence 5 flag.
 *
 * For the fences see the doc.
 */
static int atrshmlog_thread_fence_5 = 0;


/********************************************************************/

/**
 * \n Main code:
 *
 * \brief Use of fence 6 flag.
 *
 * For the fences see the doc.
 */
static int atrshmlog_thread_fence_6 = 0;


/********************************************************************/

/**
 * \n Main code:
 *
 * \brief Use of fence 7 flag.
 *
 * For the fences see the doc.
 */
static int atrshmlog_thread_fence_7 = 0;


/********************************************************************/

/**
 * \n Main code:
 *
 * \brief Use of fence 8 flag.
 *
 * For the fences see the doc.
 */
static int atrshmlog_thread_fence_8 = 0;


/********************************************************************/

/**
 * \n Main code:
 *
 * \brief Use of fence 9 flag.
 *
 * For the fences see the doc.
 */
static int atrshmlog_thread_fence_9 = 0;


/********************************************************************/

/**
 * \n Main code:
 *
 * \brief Use of fence 10 flag.
 *
 * For the fences see the doc.
 */
static int atrshmlog_thread_fence_10 = 0;


/********************************************************************/

/**
 * \n Main code:
 *
 * \brief Use of fence 11 flag.
 *
 * For the fences see the doc.
 */
static int atrshmlog_thread_fence_11 = 0;


/********************************************************************/

/**
 * \n Main code:
 *
 * \brief Use of fence 12 flag.
 *
 * For the fences see the doc.
 */
static int atrshmlog_thread_fence_12 = 0;

/********************************************************************/

/** 
 * \n Main code:
 *
 * \brief We have for the adaptive strategy a default for start.
 *
 * This is used when we init a buffer.
 * After the first transfer its gone.
 */
static atrshmlog_time_t atrshmlog_transfer_to_shm_time = ATRSHMLOG_TRANSFER_TO_SHM_START_DEFAULT;

/********************************************************************/

/** 
 * \n Main code:
 *
 * \brief We have a per program switch for log on or log off .
 *
 * This can be used by the program to stop logging for a special 
 * path by using the functions to switch on and off.
 *
 * This is a read only for the user, use the functions if
 * you need to switch it.
 *
 * This is used in the macro code for a check
 * so it works near full speed for the logging process.
 *
 * At least we can this way test a process without the
 * overhead of logging at all and check for memory problems
 * without the need for taking logging into account.
 *
 * There is an env in attach to set this.
 */
volatile atrshmlog_int32_t atrshmlog_logging_process = 0;


/********************************************************************/

/** 
 * \n Main code:
 *
 * \brief We have a per program switch for final log off .
 *
 * This can be used by the program to stop logging finally.
 *
 * We use this in all loop situations. So it works for the 
 * logging even if you are stuck in a mem to shm thing.
 *
 * But you have still to wait till all threads are awakening
 * and recognise it.
 * 
 * So this works not instantan, but with a little time shift.
 */
static int atrshmlog_logging_process_off_final = 0;


/********************************************************************/

/** 
 * \n Main code:
 *
 * \brief The  clock id.
 *
 * - 0 : null
 * - 1 : tsc
 * - 2 : tsc and a lfence
 * - 3 : par tsc
 *
 * We start with 1, set it to a better value with the setter
 * We have to expose this because we use inline code - at least we try
 * in the gettime thing.
 *
 * There is an env in attach to set this.
 */
volatile  int atrshmlog_clock_id = 1; 


/********************************************************************/

/**
 * \n Main code:
 *
 * \brief The init time values.
 *
 * Our time is meatured in tsc clicks.
 *
 * So we make one corresponding gettime and use two tsc 
 * to cornerstone this gettime.
 *
 * The medium of the two tsc is then assumed the real tsc for the gettime
 * which is of course wrong, but will reduce error somehow.
 *
 * With one from program start and one from buffer full we have a rough
 * estimation for the calc of real time for a tsc.
 *
 * Its better than trying to calc on a hypothetical base
 * but even this can go wrong if the machine is not
 * GOOD ENOUGH 
 * for holding tsc nice.
 *
 * The calculation for the ral time approximation is done in the convert.
 * So we dont do any of that in the module.
 */
static  atrshmlog_internal_time_t atrshmlog_inittime;


/********************************************************************/

/**
 * \n Main code:
 *
 * \brief The click before we get the real time.
 */
static  atrshmlog_time_t atrshmlog_inittimetsc_before;


/********************************************************************/

/**
 * \n Main code:
 *
 * \brief The click after we get the ral time.
 */
static  atrshmlog_time_t atrshmlog_inittimetsc_after;


/********************************************************************/

/**
 * \n Main code:
 *
 * \brief Our delimiter character for the argv string parts in write2.
 *
 * There is an env in attach to set this.
 */
static int atrshmlog_delimiter = ' ';


/********************************************************************/

/**
 * \n Main code:
 *
 * \brief The prefix for all used environment variables.
 *
 * We hold here the prefix for all of our environment variables
 * and files so we can simply switch from one to another by 
 * changing the major prefix name.
 *
 * This is done by setting the ATRSHMLOG variable before 
 * attaching / getting the env value ...
 *
 * Dont change this if you dont need to (for a corporate env name
 * you can use the setter in advance of the attach ).
 *
 * Size is some chars big, so its ok for now.
 *
 * Use it as it is a read only after the attach.
 *
 * If you dont do it it is set in the first call in attach.
 */
static char atrshmlog_prefix_name_buffer[ATRSHMLOG_PREFIX_SIZE];


/********************************************************************/

/**
 * \n Main code:
 *
 * \brief The number of lock event flags.
 *
 * You can change this by program or env.
 *
 * The default should fit at least most needs.
 *
 * There is an env in attach to set this.
 */
static int atrshmlog_event_locks_max = ATRSHMLOGEVENTCOUNT;


/********************************************************************/

/** 
 * \n Main code:
 *
 * \brief We use this hold the events in the default version usage.
 *
 * This is done at attach and is then set for the rest of
 * the runtime - at least in normal cases.
 *
 * You can switch on off and get with three functions
 * the individual values in your program if you have to.
 *
 * So dont touch this if not needed.
 *
 * You can set this before attach to a dyn allocatd buffer.
 * You can set this with an env in attach.
 */
static atrshmlog_event_t atrshmlog_event_locks_buffer [ ATRSHMLOGEVENTCOUNT ];


/********************************************************************/

/** 
 * \n Main code:
 *
 * \brief The global var for the events is here.
 *
 * Because the buffer is static its normally not reachable.
 *
 * We use this pointer for checking the flags in macros.
 *
 * Please use the functions to change if needed.
 *
 * Use this as it is a read only.
 */
volatile const atrshmlog_event_t* atrshmlog_event_locks = atrshmlog_event_locks_buffer; 





/********************************************************************/

/** 
 * \n Main code:
 *
 * \brief We can switch statistics on if needed
 *
 * So we have here the flag. We do start off.
 * Statistics are a inter thread locking thing.
 * so we have to use it only in test and development.
 *
 */
/********************************************************************/
static int atrshmlog_statistics_on = 0;

/** 
 * \n Main code:
 *
 * \brief We need some statistical counters here.
 *
 * So we have to do the atomic thing here again.
 *
 */
static atomic_int g_counter[100] = {
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0),
  ATOMIC_VAR_INIT(0)
};


/**
 * \brief We add to the statistics counter
 *
 * \param __e
 * The event we add to. 
 */
#define ATRSHMLOGSTAT(__e) \
  (atrshmlog_statistics_on ? (++g_counter[(__e)]) : 0)


/*******************************************************************/

/* we begin the helper functions here */

/*******************************************************************/


/**
 * \brief We are the cleaner.
 *
 * We kill, we destroy, we get rid of the garbage.
 *
 * So in case of the log we declaire all for done.
 *
 * We check for valid shm.
 * We check for valid in main .
 * We transfer all main buffers to shm till it is done.
 *
 * For this we need a clean memory image per buffer
 * so this is somehow a dangerous game.
 *
 * We block till we get the clean buffer.
 *
 * If we dont have clean buffers
 * the process will hang till we hit it hard.
 *
 * You should use the reader at least to get full shm buffers clean.
 */
static void atrshmlog_exit_cleanup(void);

/*******************************************************************/

/**
 * \n Main code:
 *
 * \brief We wait nanoseconds.
 * 
 * This is done by  loop because at least on my box
 * I found that longer intervals did not work as expected.
 * So we use a loop for wait of 50000 nanos and then the rest..
 */
void atrshmlog_nanosleep(atrshmlog_int32_t i_nanos)
{

  const int timeframe = 50000;

  if (i_nanos < 0)
    i_nanos = 1;

  int limit = i_nanos / timeframe;
  int rest = i_nanos % timeframe;
  
  struct timespec ts;
  ts.tv_sec = 0;
  ts.tv_nsec = timeframe;
  
  for (int i = 0; i < limit; i++) 
    nanosleep(&ts, NULL);

  if (rest > 0)
    {
      ts.tv_nsec = rest;

      nanosleep(&ts, NULL);
    }
}  

/*******************************************************************/

/**
 * \n Main code:
 *
 * \brief We get memory from the dyn memory area.
 *
 * We are a raw helper, so we have no lock because we are inside 
 * a lock.
 *
 * We get memory for buffers and descriptors and give it back.
 * No list stuff is done so far.
 *
 * \param i_buffer_count
 * The count of buffers we want
 *
 * \param i_buffer_size
 * The size of the buffer.
 *
 * \return
 * The pointer for the first buffer
 */
static tbuff_t* atrshmlog_il_get_raw_buffers(const int i_buffer_count,
					     const int i_buffer_size)
{
  const int save = 16;

  ATRSHMLOGSTAT(atrshmlog_counter_get_raw);
  
  tbuff_t* n = malloc(i_buffer_count  * (sizeof(tbuff_t) + i_buffer_size + save));

  if ( n != 0)
     atrshmlog_acquire_count += i_buffer_count;
   
  return n;
}


/*******************************************************************/

/**
 * \n Main code:
 *
 * \brief  We conect desriptors.
 *
 * Create the list of 
 * next_availabe buffers and then put it on the a list.
 *
 * We are an in lock function, so we dont lock ourself.
 * We are now a super push for evtl. multiple buffers.
 *
 * \param raw
 * The adress of the memory.
 *
 * \param i_buffer_count
 * The count of buffers.
 *
 * \param i_chunkbuffer
 * The start of the memory for the chunks.
 *
 * \param i_buffer_size
 * The size for a chunk.
 *
 * \return 
 * \li 0 : ok
 * \li non zero : error
 */
static int atrshmlog_il_connect_buffers_list(tbuff_t* const raw,
					     const int i_buffer_count,
					     char* i_chunkbuffer,
					     const int i_buffer_size)
{
  tbuff_t* n = raw;

  if (n == 0)
    return atrshmlog_error_connect_1;
  
  if (i_buffer_count < 1 )
    return atrshmlog_error_connect_2;

  tbuff_t* t = n;

  tbuff_t* tnext = t + 1;

  // We connect the buffers and attach if needed the chunks
  for (int i = 0; i < i_buffer_count - 1; t++, tnext++ , i++)
    {
      t->next_append = tnext;
      t->next_cleanup = tnext;
      
      if (i_chunkbuffer)
	{
	  t->b = i_chunkbuffer;
	  t->id = ++atrshmlog_buffer_id;
	  t->maxsize = i_buffer_size;
	  t->size = 0;

	  atomic_init(&t->dispatched, 0);

	  i_chunkbuffer += i_buffer_size;
	}
    }

  // Now do it for the last buffer
  if (i_chunkbuffer)
    {
      t->b = i_chunkbuffer;
      t->id = ++atrshmlog_buffer_id;
      t->maxsize = i_buffer_size;
      t->size = 0;

      atomic_init(&t->dispatched, 0);
	  
      i_chunkbuffer += i_buffer_size;
    }
  
  // We push all members in one move on the s list for cleanup
  t->next_cleanup = (tbuff_t*)atomic_load(&atrshmlog_tps);

  while (!atomic_compare_exchange_weak(&atrshmlog_tps, &t->next_cleanup, n))
    ;

  // We push all members in one move on the a list for use
  t->next_append = (tbuff_t*)atomic_load(&atrshmlog_tpa);

  while (!atomic_compare_exchange_weak(&atrshmlog_tpa, &t->next_append, n))
    ;

  return atrshmlog_error_ok;
}


/*******************************************************************/

/**
 * \n Main code:
 *
 * \brief We set the buffer free.
 *
 * So we put it on the a list for the next user - we never 
 * release a buffer in real life, simply try to recycle it.
 *
 * This is only a problem if you hit the dyn memory alloc
 * and forget to free buffers you no longer need.
 *
 * Then your process will blow up and fragment a lot.
 *
 * If you handle logging serious you free the logging 
 * with atrshmlog_stop at the end of the thread  .
 *
 * \param t
 * The buffer adress.
 *
 * \return 
 * void
 */
static void atrshmlog_free(tbuff_t* restrict t)
{
  if (t == 0)
    return;

  ATRSHMLOGSTAT(atrshmlog_counter_free);
  
  // we make clear this buffer IS initialized and on the linked lists
  // already so we NOT have to relink it and NOT reinit it
  t->dispose = ATRSHMLOG_DISPATCH_REUSE;
  t->size = 0;
  
  // we push it now only on the a list for reuse
  t->next_append = (tbuff_t*)atomic_load(&atrshmlog_tpa);

  while (!atomic_compare_exchange_weak(&atrshmlog_tpa, &t->next_append, t))
    ;
}


/*******************************************************************/

/**
 * \n Main code:
 *
 * We need a once init flag here 
 */
static atomic_flag atrshmlog_alloc_init = ATOMIC_FLAG_INIT;

/** 
 * \brief We allocate a buffer for the use of thread loging.
 *
 * We check the a list first.
 *
 * If we are lucky we are done ( thats a 200 click thing ...).
 *
 * If we are not lucky, we have to dyn alloc.
 *
 * We do this with prealloc count of buffers.
 *
 * So its a costly or the one thread that hits the a list,
 * but for all others after it its cheap again.
 *
 * Be sure you undersand if you need more buffers.
 *
 * There is allways a way to recycle if your thread ends.
 *
 * So try to use the atrshmlog_stop in any case possible.
 *
 * Then check which is the avarage number you need and adjust
 * the static buffers number first. 
 *
 * \return
 * - 0 : error
 * - non zero : adress of the new buffer for the thread
 */
static tbuff_t* atrshmlog_alloc(void)
{
  static volatile int tryalloc = 1;

  ATRSHMLOGSTAT(atrshmlog_counter_alloc);
  
 getcandidate:
  ;
  
  tbuff_t* ta = (tbuff_t*)atomic_load_explicit(&atrshmlog_tpa, memory_order_consume);

  // Is the a stack empty ?
  if (ta == 0)
    {
      // Do we have the alloc option ?
      if (!tryalloc)
	{
	  // Sorry, no more buffer for now
	  return 0;
	}

      // We fill the a stack with new entrys

      // We lock. This is needed to prevent multiple mallocs ...
      while (atomic_flag_test_and_set(&atrshmlog_alloc_init))
	;

      // If multiple threads are in a lock all but one should see a valid ptr
      // The first will see a null - or another thread did push an disposed buffer...
      ta = (tbuff_t*)atomic_load(&atrshmlog_tpa);

      // Are we the first thread reaching here ?
      if (ta == 0)
	{
	  // Are we still alloc ing ?
	  if (!tryalloc)
	    {
	      // Sorry, no option left ...
	      atomic_flag_clear(&atrshmlog_alloc_init);

	      return 0;
	    }

	  int size = atrshmlog_buffer_infosize;

	  // We have to do the alloc
	  tbuff_t* n = atrshmlog_il_get_raw_buffers(atrshmlog_prealloc_buffer_count, size);

	  if (n == 0)
	    {
	      // We are out of buffers ....
	      
	      tryalloc = 0; // We dont do this any longer
	      
	      atomic_flag_clear(&atrshmlog_alloc_init);

	      return 0;
	    }
      
	  // We init the ids and push the list on the stacks for cleanup and append
	  atrshmlog_il_connect_buffers_list(n, atrshmlog_prealloc_buffer_count, (char*)&n[atrshmlog_prealloc_buffer_count], size);

	  atomic_flag_clear(&atrshmlog_alloc_init);

	  if(atrshmlog_thread_fence_8)
	    atomic_thread_fence (memory_order_release);
	  
	  // The new candidates arrived. We try to pick one

	  goto getcandidate;
	}

      // We have a non null ta ... give lock free
      atomic_flag_clear(&atrshmlog_alloc_init);
    }

  // We pop from  stack now
  while (ta
	 && !atomic_compare_exchange_weak_explicit(&atrshmlog_tpa,
						   &ta,
						   ta->next_append,
						   memory_order_consume,
						   memory_order_relaxed))
    ;

  // The stack was empty ?
  if (ta == 0)
    goto getcandidate;

  // This must be non 0 and we have used a consume, so no more fence here

  return ta;
}


/*******************************************************************/

/**
 * \n Main code:
 *
 * \brief Put this buffer on the f list, so the thing is copied to shm 
 * by a slave.
 *
 * This is a helper, it only puts the buffer on the list.
 * So we normally get back in no time.
 *
 * But this is of course a thing of illusion.
 * In realety the slave needs time to do the shm copy.
 * And the reader needs time to get the buffer from the shm.
 * So those two limit the through put of log.
 *
 * Try to check for the times for the slave and the reader and then
 * see what time you really need for the transfer.
 *
 * \param i_atrshmlog_targetbuffer
 * The adress of the buffer
 *
 * \return 
 * void
 */ 
static void atrshmlog_dispatch_buffer(tbuff_t* restrict i_atrshmlog_targetbuffer)
{
  ATRSHMLOGSTAT(atrshmlog_counter_dispatch);
  
  // We check for a valid buffer and for a non dispatched.
  // In case the buffer is already dispatched we have to see
  // where we made this call..
  // .. but for now we simply ignore the call
  if (i_atrshmlog_targetbuffer
      && (atomic_load_explicit(&i_atrshmlog_targetbuffer->dispatched, memory_order_relaxed) == 0))
    {
      // We sync the buffer content itself - hard sync if needed.
      if (atrshmlog_thread_fence_1)
	atomic_thread_fence (memory_order_release);

      // We set the buffer dispatched. 
      atomic_store_explicit(&i_atrshmlog_targetbuffer->dispatched, 1, memory_order_release);

      // Do we really have to do this ? or is it empty ?
      if (i_atrshmlog_targetbuffer->size > 0)
	{
	  // We put the buffer on the f list
	  i_atrshmlog_targetbuffer->next_full = (tbuff_t*)atomic_load_explicit(&atrshmlog_tpf, memory_order_relaxed);


	  // Push on stack ...
	  while(!atomic_compare_exchange_weak_explicit(&atrshmlog_tpf,
						       &i_atrshmlog_targetbuffer->next_full,
						       i_atrshmlog_targetbuffer,
						       memory_order_relaxed,
						       memory_order_relaxed))
	    ;
	}
      else
	{
	  // No. We are empty. Simply switch back to useable.
	  atomic_store_explicit(&i_atrshmlog_targetbuffer->dispatched, 0, memory_order_release);
	}
    }
}


/*******************************************************************/

/**
 * \n Main code:
 *
 * \brief We want a buffer from the a list.
 * 
 * We then initialize it.
 *
 * \param i_g
 * The adress of the thread local struct.
 *
 * \return
 * The adress of the buffer to use now for the thread.
 */
static tbuff_t *atrshmlog_acquire_buffer(const g_tl_t* restrict i_g)
{
  atrshmlog_time_t start;
  atrshmlog_time_t end;

  // We meature the acquire time in clicks
  // we switch later on to get clicktime evtl.
  start = ATRSHMLOG_GET_TSC_CALL();

  tbuff_t* targetbuffer= atrshmlog_alloc();

  if (!targetbuffer)
    return targetbuffer; // null - nothing goes here ...

  // Ok. We init the buffer.
  targetbuffer->next_full = 0;
  
  targetbuffer->next_append = 0;

  targetbuffer->safeguardfront = ATRSHMLOGSAFEGUARDVALUE;

  targetbuffer->pid = i_g->atrshmlog_thread_pid;
  
  targetbuffer->tid = i_g->atrshmlog_thread_tid ;

  targetbuffer->transfer_to_shm_time = atrshmlog_transfer_to_shm_time;
  
  targetbuffer->dispose = 0;

  targetbuffer->size = 0;
  
  atomic_store_explicit(&targetbuffer->dispatched, 0, memory_order_relaxed);

  end = ATRSHMLOG_GET_TSC_CALL();

  targetbuffer->acquiretime = end - start;
  
  return targetbuffer;
}

/************************************************************************/

/**
 * \n Main code:
 *
 * \brief We init our thread local vars - the everyone needs helper. 
 *
 * \param i_g
 * The adress of the thread local struct.
 *
 * \return 
 * - 0 : ok
 * - non zero : error
 */
static int atrshmlog_init_thread_local (g_tl_t* restrict i_g)
{
  /* Are we the flag or the initialize needed flag */
  if (i_g->atrshmlog_idnotok == -1)
    {

      if (atrshmlog_thread_fence_9)
	atomic_thread_fence(memory_order_acquire); 

      atrshmlog_area_t * a_shm = ATRSHMLOG_GETAREA;
      
      int versionnotok = (atrshmlog_base_ptr == 0) || (a_shm->shmversion != ATRSHMLOGVERSION);
      i_g->atrshmlog_idnotok = versionnotok || (a_shm->shmid != atrshmlog_id) ;

      // We do NOT overwrite in this case. Perhaps we can do something
      // in debugging with the info.
      if ( i_g->atrshmlog_idnotok == 0)
	{
	  i_g->atrshmlog_shm_count = a_shm->shmcount;

	  i_g->atrshmlog_thread_pid = getpid();

	  i_g->atrshmlog_thread_tid = ATRSHMLOG_GETTHREADID;

	  i_g->strategy = atrshmlog_buffer_strategy; 
	}
    }

  /* From now on we are the not ok flag - 0 is ok, else is not */
  if (i_g->atrshmlog_idnotok)
    return atrshmlog_error_init_thread_local_1;

  return atrshmlog_error_ok;
}

/*******************************************************************/


/** 
 * \n Main code:
 *
 * \brief helper :  init the events
 * 
 * \param i_use_file
 * The flag to decide if we use the env variables or the flag files.
 *
 * \return 
 * void
 */
static void atrshmlog_init_events(const int i_use_file)
{
  /* 
   * Switch on or off determination.
   * If this is null we switch off individual.
   * That means the event flags are on by default.
   * If we do set this it means we invert. 
   * The flags are off by default
   * so we have to switch them on individual
   */
  const char *evn = NULL;

  /*
   * This is the list of on and off switchers.
   * The main logic is determined by event null - see above
   * This is a list of events to be shut off in default mode
   * Is evn set - the thing is turned on instead.
   * So we can default the buffer to one of the 
   * two logical states and then switch individual off or on
   * counterwise.
   */
  const char *onoff = NULL;

  /* 
   * Helperbuffer for names with prefixes
   * Ok, this is dirty
   * I know
   * But i dont want to use dyn memory in here.
   * So i use a prefix_name_buffer that is some bytes and 
   * this local buffers... 
   */
  char buff[255];

  /* 
   * If we are a login shell, there are no env variables.
   * So in this case we use files
   * This is done here in case the flag is given
   * any thing else needs only the env vars 
   * See for the code in the case the env vars are used, then check the code
   * here. 
   * There is one diffrence, the use of the env for onoff
   * and the file is different - the file does not use a : as a 
   * delimiter.
   * Beside this it works the same - or should at least..
   */
      
  if (i_use_file)
    {
#if ATRSHMLOG_LEVEL > 0
      
      strcat(strcat(strcpy(buff,atrshmlog_prefix_name_buffer), ATRSHMLOGENVSUFFIX),".TXT");
      
      FILE *f = fopen(buff, "r");

      if (f)
	{
	  fclose(f);
	  evn = "on";
	}

      int flag = (evn == NULL) ? 1 : 0; /* on or off ... */

      memset((void*)atrshmlog_event_locks, flag, atrshmlog_event_locks_max);

      strcat(strcat(strcpy(buff, atrshmlog_prefix_name_buffer), ATRSHMLOGEVENTONOFFSUFFIX),".TXT");

      f = fopen(buff, "r");

      if (f)
	{
	  long lindex;
      
	  flag = flag ? 0 : 1;

	  while (!feof(f) && !ferror(f)) {

	    if (1 == fscanf(f, "%ld", &lindex))
	      {
		long ev = lindex;

		if (ev < 0 || ev >= atrshmlog_event_locks_max)
		  ;
		else
		  ((atrshmlog_event_t*)atrshmlog_event_locks)[(int)ev] = flag;
	      }
	  }
      
	  fclose(f);
	}

#endif
      
      return;
    }

  /* 
   * Ok. We do not use a file, so we are not a login shell - or
   * we simply do not log at all. 
   * In that case we never reach this.
   * In the case we dont use a flag file we try to check the env
   */

  evn = atrshmlog_get_env(ATRSHMLOGEVENTNULLSUFFIX);

  onoff =  atrshmlog_get_env(ATRSHMLOGEVENTONOFFSUFFIX);

  int flag = (evn == NULL) ? 1 : 0; /* on or off ... */
  
  memset((void*)atrshmlog_event_locks, flag, atrshmlog_event_locks_max);

  /* The buffer is initialized. We can now switch on or off */

  /* 
   * We use a : separated string. 
   * Every event number is in the string
   * delimited by : or begin or end of the string.
   *
   * So we simply iterate from : to : to end and then have the number.
   *
   * If number is not in range we simply ignore it 
   *
   * Example ATRSHMLOG_EVENT_ONOFF=1:2:3:13:25:4711
   *
   * This will try to switch of the flags for event 1, 2 , 3, 13, 25 and 4711
   * (ok, we have a buffer of 10 k so this should be possible ... 
   * please dont abuse  - you only hurt yourself )
   *
   * If the env is set the thing turns upside down.
   * Then the buffer flag are all 0 and we turn these events on... 
   */
  if (onoff)
    {
      int limit = strlen(onoff);
      int j;
      int ende = 0;
      long ev;
      char* ptr;

      char buff[255];

      /* We invert the flag value meaning */
      flag = flag ? 0 : 1;
      
      for (j = 0; j < limit; j = ende + 1)
	{
	  for (ende = j;
	       onoff[ende] != '\0'
		 && onoff[ende] != ':'; 
	       ende++)
	    ;

	  // We handle the last separately
	  if (onoff[ende] == '\0')
	    break;

	  // Ignore an empty one
	  if ((ende - j) == 0)
	    continue;

	  // Ok. Copy candidate to helper
	  
	  int sz = ende - j ;

	  if (sz > 250)
	    sz = 0; // We ignore this one
	  else 
	    memcpy(buff, onoff + j, sz);

	  buff[sz + 1] = '\0';
	  
	  ev = strtol(buff, &ptr, 10);

	  if ((ptr - buff) != (ende - j))
	    {
	      ;  // We ignore non numbers 
	    }
	  else
	    {
	      if (ev < 0 || ev >= atrshmlog_event_locks_max)
		;
	      else
		((atrshmlog_event_t*)atrshmlog_event_locks)[(int)ev] = flag;
	    }
	}
      
      // We ignore empty entry
      if ((ende - j) == 0)
	return;

      if (onoff[j] != '\0')
	{
	  ev = strtol(onoff + j, &ptr, 10);

	  if ((ptr - (onoff + j)) != (ende - j))
	    {
	      ;  // We ignore non numbers 
	    }
	  else
	    {
	      if (ev < 0 || ev >= atrshmlog_event_locks_max)
		;
	      else
		((atrshmlog_event_t*)atrshmlog_event_locks)[(int)ev] = flag;
	    }
	}
    }
}


/*******************************************************************/

/** 
 * \n Main code:
 *
 * \brief helper : transfer local memory buffer to a shm buffer
 *
 * We transfer the buffer into a shm buffer .
 *
 * We use the a list in shm to get a free buffer.
 * 
 * Then we transfer the stuff from the in memory buffer to that buffer.
 *
 * We also add statistical data for the user to see if there
 * is a starvation in this (only times, in a special version i 
 * did counters too)
 *
 * Then we put the buffer on the f list in shm.
 * 
 * We free the buffer in memory and give it back to the caller then.
 *
 * \param i_mem
 * Adress of the buffer to transfer.
 *
 * \param i_g
 * Adress of the thread local struct.
 *
 * \return 
 * - 0: ok
 * - non zero : error
 */
static int atrshmlog_transfer_mem_to_shm(const tbuff_t* restrict i_mem,
					 const g_tl_t* restrict i_g)
{
  atrshmlog_time_t start;

  ATRSHMLOGSTAT(atrshmlog_counter_mem_to_shm);

  if (atrshmlog_base_ptr == 0)
    return atrshmlog_error_mem_to_shm_1;

  if (i_mem == NULL)
    return atrshmlog_error_mem_to_shm_2;

  if (i_mem->size == 0)
    return atrshmlog_error_mem_to_shm_3;

  if (i_mem->maxsize < i_mem->size)
    return atrshmlog_error_mem_to_shm_4;

  if (atrshmlog_logging_process_off_final != 0)
    return atrshmlog_error_mem_to_shm_8;
  
#if ATRSHMLOGDEBUG == 1
  printf("mem to %ld %ld %ld\n", (long)i_mem->id, (long)i_size, (long)i_g->atrshmlog_idnotok);
#endif
  
  /* Those normally never change */

  /* Bad thing. Wrong id */
  if (i_g->atrshmlog_idnotok)
    {
	return atrshmlog_error_mem_to_shm_5;
    } 

  /* Next. we check for initialized */
  atrshmlog_area_t * a = ATRSHMLOG_GETAREA;

  /* Those can change from run to run */
  /* Bad thing. Safeguard invalid */
  if (a->shmsafeguard != ATRSHMLOGSAFEGUARDVALUE)
    return atrshmlog_error_mem_to_shm_6;

  start = ATRSHMLOG_GET_TSC_CALL();

  ATRSHMLOGSTAT(atrshmlog_counter_mem_to_shm_doit);
  
  int chksum = 0;

#if ATRSHMLOGDEBUG == 1
  for (int k = 0; k < i_size; k++)
    {
      chksum += i_mem->b[k];
    }
#endif

  /* 
   * We loop till we hit a free buffer.
   */
  while (1)
    {
      int available = atomic_load_explicit(&a->shma, memory_order_consume);

      if (available == -1)
	{
	  ATRSHMLOGSTAT(atrshmlog_counter_mem_to_shm_full);
	  /* 
	   * We have to wait for a free one.
	   */
	  atrshmlog_nanosleep(atrshmlog_slave_to_shm_wait);

	  if (atomic_load_explicit(&a->ich_habe_fertig, memory_order_relaxed) != 0)
	    return atrshmlog_error_mem_to_shm_7;

	  if (atrshmlog_logging_process_off_final)
	    return atrshmlog_error_mem_to_shm_8;
	  
	  continue;
	}

      /* We pop from list */
      while ( available > -1
	      && !atomic_compare_exchange_weak_explicit(&a->shma,
							&available,
							a->logbuffers[available].next_append,
							memory_order_relaxed,
							memory_order_relaxed))
	;

      // Was the list non empty ? 
      if (available > -1)
	{
	  atrshmlog_buffer_t* restrict b = &(a->logbuffers[available]);

	  b->next_append = -1;
	  
	  b->shmsize = i_mem->size;

	  b->chksum = chksum;

	  b->inittime = atrshmlog_inittime;
	  b->inittimetsc_before = atrshmlog_inittimetsc_before;
	  b->inittimetsc_after = atrshmlog_inittimetsc_after;

	  b->pid = i_mem->pid;
	  
	  b->tid = i_mem->tid;

	  b->acquiretime = i_mem->acquiretime;

	  b->id = i_mem->id;
	  
	  b->starttransfer = start;

	  char* restrict target = (char*)a + b->info;
#if 1
	  memcpy(target, i_mem->b, i_mem->size);
#else
	  for (int zippo = 0; zippo < i_mem->size; zippo++)
	    target[zippo] = i_mem->b[zippo];
#endif

	  
	  b->lasttimetsc_before = ATRSHMLOG_GET_TSC_CALL();

	  b->lasttime = atrshmlog_get_realtime();
	  
	  b->lasttimetsc_after = ATRSHMLOG_GET_TSC_CALL();

	  // this is a hack. we know we have a const buffer,
	  // but on the other side we have to change some value
	  // so i do this the hard way here.
	  // its ok so far - no buffer is ever in const area of the process,
	  // so i can do this.
	  // but its better to do it locally only once ...
	  *(atrshmlog_time_t*)&i_mem->transfer_to_shm_time = b->lasttimetsc_before - start;
	  
	  if(atrshmlog_thread_fence_5)
	    atomic_thread_fence(memory_order_release);
	      
	  atomic_store_explicit(&(b->state), atrshmlog_full, memory_order_release);

	  // We push it now on the full list 

	  b->next_full = atomic_load_explicit(&a->shmf, memory_order_acquire);

	  while (!atomic_compare_exchange_weak_explicit(&a->shmf,
							&b->next_full, 
							available,
							memory_order_acq_rel,
							memory_order_relaxed))
	    ;
	  
	  return atrshmlog_error_ok;
	}
    }
  
  /* NOT REACHED */
}

/*******************************************************************/

/**
 * \brief We help the init to set a value from an file contents.
 * 
 * \param i_suffix
 * The Suffix of the file name after the envprefix.
 *
 * \param v
 * Points to an int that gets the value.
 *
 * \param i_min
 * Minimum allowed number to set.
 *
 * \param i_max
 * Maximum allowed number to set.
 *
 * \return 
 * void
 */
static void atrshmlog_init_via_file(const char *i_suffix,
				    int *v,
				    int i_min,
				    int i_max)
{
  char buff[256];
  
  strcat(strcat(strcpy(buff, atrshmlog_prefix_name_buffer), i_suffix),".TXT");
		    
  FILE* f= fopen(buff, "r");

  if (f)
    {
      long int new_max ;
			
      if (1 == fscanf(f,"%ld", &new_max))
	{
	  if (new_max >= i_min
	      && new_max <= i_max)
	    {
	      *v = new_max;
	      sprintf(buff, "%s%s=%ld", atrshmlog_prefix_name_buffer, i_suffix, new_max);
	      putenv(buff);
	    }
	}
      
      fclose(f);
    }
}

/*******************************************************************/

/**
 * \brief We help the init to set a value from an environment variable.
 * 
 * \param i_suffix
 * The Suffix of the variable name after the envprefix.
 *
 * \param v
 * Points to an int that gets the value.
 *
 * \param i_min
 * Minimum allowed number to set.
 *
 * \param i_max
 * Maximum allowed number to set.
 *
 * \return 
 * void
 */
static void atrshmlog_init_via_env(const char *i_suffix,
				   int *v,
				   int i_min,
				   int i_max)
{
  const char *p = atrshmlog_get_env(i_suffix);

  if (p)
    {
#if ATRSHMLOGDEBUG == 1
      printf("ATRSHMLOG%s %s\n", i_suffix, p);
#endif
      long val = strtol(p, NULL, 10);

      if (val >= i_min && val <= i_max)
	*v = val;
      
    }		      
}

/*******************************************************************/

/* We start real used functions here with linkage.
 *
 * So these functions are linkable from the user .
 *
 * We can use them in a c and c++ program directly.
 * 
 * For others we need a kind of layer.
 *
 * See the java example for this.
 *
 * All functions with linkage have the parameters documented in the header. 
 */


/**
 * We have this external, so we can use the info in setters to
 * stop setting values when attach is done.
 */
static volatile int atrshmlog_attach_once = 0;

/**
 * We need a safe guard against race conditions 
 */
static atomic_flag atrshmlog_attach_once_flag = ATOMIC_FLAG_INIT;

/**
 * \n Main code:
 *
 * \brief The attach to shm memory function - and initialize.
 *
 * Attach the buffer if logging is wanted and the buffer is not
 * attached before.
 *
 * Set the global prefix_name_buffer if not alredy set by the user.
 *
 * What has to be set before  : 
 *
 * Every setting the program wants to do before the thing starts
 * logging.
 *
 * Technical you can set things like sizes, counts and so on ...
 * Use the functions or use the env variables.
 *
 * With the call to attach you - should - no longer change the
 * loggings parameters - but you can change some of them.
 *
 * When you have made the changes then attach uses some of them
 * to do the important things like starting slaves.
 *
 * One thing that should not be changed after attach is the number of events.
 *
 * So check the code to see if a thing can be changed after.
 *
 */
atrshmlog_ret_t atrshmlog_attach(void)
{
  ATRSHMLOGSTAT(atrshmlog_counter_attach);
  
  /* We attach the shm buffer. 
   * First we have to check for the env var and then for the
   * extern ptr  that holds the log base.
   * If the ptr is null we attach.
   * If the env is null we try the file atrshmlogshmid.txt
   * If we still have no hit we give up .
   * If env is ok, but if is 0 we give up.
   *
   * If the env is not set we set the ptr to null to prevent 
   * logging in case we have NOT been a top level process
   * and the log is already in a predecessor process open.
   *
   * Later on we can change this for getting subprocesses
   * to log too...
   */

  // we lock. this is needed to prevent races...
  while (atomic_flag_test_and_set(&atrshmlog_attach_once_flag) )
	;

  // one enters this first - and only one
  int result = atrshmlog_error_attach_2;

  /*
   * This is the first big catch.
   * We check for already attached.
   *
   * This is done simple by using the static here.
   *
   * Question: How are we already initialized ?
   *
   * Answer:
   * Hm this is not so simple.
   * We can be the second call or so ..
   * Or we are made but there was a fork before and the image 
   * of the process still has the attach memory.
   * So we make this from the value of attach once.
   * 
   */
  if (atrshmlog_attach_once != 0)
    {
      /*
       * We clear the lock.
       */
      atomic_flag_clear(&atrshmlog_attach_once_flag);

      return atrshmlog_error_attach_1;
    }

    
    
  const char *shmid_str;
  
  /* 
   * This does two things : first it initializes the prefix_name_buffer
   * with the value of default and if the ATRSHMLOG is set then this 
   * string value.
   * Then it uses the prefix with the suffix to get the env
   * value for this.
   *
   * Then the thing is used to make the shmid to identify 
   * the attaching buffer....
   *
   * See the code for details
   */
  shmid_str = atrshmlog_get_envshmid();
      
  /* Second thing after the init of vars : make timestamp */
  atrshmlog_inittimetsc_before = atrshmlog_get_clicktime();

  atrshmlog_inittime = atrshmlog_get_realtime();

  atrshmlog_inittimetsc_after = atrshmlog_get_clicktime();
  
  /* We check now for the shmid text : is text set we never do the file hings */
  if (shmid_str != 0)
    {
      int base = 10; 
      char *endptr;
      
      long int lshmid = strtol(shmid_str, &endptr, base);

      /* Ok. We have it, and we think its ok if its not false info. 
       * So we check for endptr points to a zero byte and lshmid is not 0
       */
      if (lshmid != 0 && *endptr == '\0')
	{
	  /* This is looking good .
	   * We try to attach now.
	   */
	  int shmid = (int)lshmid; 

	  void*shmaddr = (void*)0; /* We let decide the os */ 

	  int shmflg = 0; 
          
	  void *shmat_result = shmat(shmid, shmaddr, shmflg);

	  if (shmat_result != (void*)-1)
	    {
	      atrshmlog_base_ptr = shmat_result;

	      atrshmlog_id = shmid;

	      /* From now on we try to solve the initializations
	       * with the environment values - and only those.
	       *
	       * We dont check for interference from before the attach
	       * so consider to be warned. 
	       * 
	       * If you make an env setting
	       * then this is superior in the attach.
	       * No previous setting of the program will survive.
	       */

	      const char *p;
		  
	      /* 
	       * The buffer acquire strategy for shm buffers.
	       * This has been not used now, but evtl we need it
	       * again on the reader side. Then we can use it here
	       * to make a better decision on the producer side.
	       * So it is used for now, but the target changes
	       */
	      atrshmlog_init_via_env(ATRSHMLOGSTRATEGYSUFFIX,
				     &atrshmlog_buffer_strategy,
				     atrshmlog_strategy_first,
				     atrshmlog_strategy_last);

	      /* 
	       * The buffer acquire strategy wait nanos wait time.
	       * We can redefine this for the process here.
	       */
	      atrshmlog_init_via_env(ATRSHMLOGSTRATEGYWAITTIMESUFFIX,
				     &atrshmlog_strategy_wait_wait_time,
				     0,
				     999999999);

	      /*
	       * The delimiter character for the arg write call
	       */
	      
	      atrshmlog_init_via_env(ATRSHMLOGDELIMITERSUFFIX,
				     &atrshmlog_delimiter,
				     0,
				     255);
	      /*
	       * The max of events.
	       * The event buffer has a aximum size at runtime.
	       * So we hold this stable.
	       * Best is to simply use the events and be happy.
	       * If you need more you can set this before in 
	       * the program.
	       * If this is not what you want you can redefine the max 
	       * in the internal and recompile.
	       * If you dont want to do it you can here reset the thing
	       * but it has a cost: only dyn memory after this ...
	       */
	      
	      p = atrshmlog_get_env(ATRSHMLOGEVENTCOUNTSUFFIX);

	      if (p)
		{
#if ATRSHMLOGDEBUG == 1
		  printf("ATRSHMLOGEVENTCOUNTSUFFIX %s\n", p);
#endif
		  long new_max = strtol(p, NULL, 10);
		      
		  if (new_max >  ATRSHMLOGEVENTCOUNT
		      && new_max > atrshmlog_event_locks_max
		      && new_max <= ATRSHMLOGEVENTCOUNTMAXLIMIT)
		    {
		      int oldmax = atrshmlog_event_locks_max;
      
		      atrshmlog_event_t * p2 =  (atrshmlog_event_t*)malloc(new_max + 1);
		      
		      if (p2)
			{
			  memset(p2, 0 , new_max + 1);
			  
			  atrshmlog_event_locks = p2;
			  
			  atrshmlog_event_locks_max = new_max;
			}
		    }
		}
	      
	      /*
	       * We can redefine the memory we need for a dyn alloc buffer.
	       * We cannot redefine the compiled statics.
	       * So be sure you want it to be this way.
	       * We also cannot redefine the shm limit.
	       * So we can only reduce the dyn alloced size.
	       * Anything else and you have to recompile after
	       * change of the define
	       */
	      
	      atrshmlog_init_via_env(ATRSHMLOGBUFFER_INFOSIZE_SUFFIX,
				     &atrshmlog_buffer_infosize,
				     ATRSHMLOGBUFFER_INFOSIZE_MIN,
				     ATRSHMLOGBUFFER_INFOSIZE);

	      /*
	       * We preallocate in a malloc an amount of buffers
	       * to reduce alloc time per buffer.
	       * So its possible to reset the number .
	       * You cannot reset the number the static has - you
	       * have to recomile for this.
	       * But for the dyn buffers you can do it 
	       */
	      atrshmlog_init_via_env(ATRSHMLOG_INIT_PREALLOC_COUNT_SUFFIX,
				     &atrshmlog_prealloc_buffer_count,
				     ATRSHMLOG_INIT_PREALLOC_COUNT_MIN,
				     ATRSHMLOG_MAX_PREALLOC_COUNT);

	      /*
	       * We can set the wait nanos for the slave
	       * having it small consumes cpu.
	       * Having it big makes the check to bee late and later ...
	       * So i use 1000 as a start.
	       * But its not the only thing to know
	       * In case we use the maximum number of slaves
	       * it becomes clear that then no thread
	       * should interact with the others, so this
	       * makes a shift to a model with another 
	       * strategy.
	       */
	      atrshmlog_init_via_env(ATRSHMLOG_INIT_BUFFER_SLAVE_WAIT_SUFFIX,
				     &atrshmlog_f_list_buffer_slave_wait,
				     ATRSHMLOG_INIT_BUFFER_SLAVE_WAIT_MIN,
				     ATRSHMLOG_INIT_BUFFER_SLAVE_WAIT_MAX);
	      
	      /*
	       * We can set the count of slave threads for dispatching 
	       * full buffers to shm.
	       * This is depending to the strategy
	       * for now it simply works.
	       * Its up to you if you want to stop - meaning 0
	       * or use a rackmount of slaves - max.
	       * To use a good number is a thing you are told in the 
	       * documentation.
	       * So read it before you guess anything.
	       */
	      
	      atrshmlog_init_via_env(ATRSHMLOG_SLAVE_COUNT_SUFFIX,
				     &atrshmlog_f_list_buffer_slave_count,
				     ATRSHMLOG_SLAVE_COUNT_MIN,
				     ATRSHMLOG_SLAVE_COUNT_MAX);
	      
	      /*
	       * We normally do not wait for the slaves in the cleanup.
	       * In theory a progam should have stoppt
	       * them before dying ...
	       *
	       * And the atexit is a feature for a dying
	       * program.
	       * But if you insist you can
	       * wait for the slaves to finish.
	       */
	      atrshmlog_init_via_env(ATRSHMLOG_WAIT_FOR_SLAVES_SUFFIX,
				     &atrshmlog_wait_for_slaves,
				     0,
				     1);
	      
	      /*
	       * We can set the clock id here .
	       * If you copy the binary to another platform
	       * that is nearby you can use that to
	       * handle a different clock tick getter 
	       */
	      atrshmlog_init_via_env(ATRSHMLOG_CLOCK_ID_SUFFIX,
				     (int*)&atrshmlog_clock_id,
				     ATRSHMLOG_CLOCK_ID_MIN,
				     ATRSHMLOG_CLOCK_ID_MAX);

	      /*
	       * We can set the statistics flag here.
	       * If you want to have statistics you have to switch
	       * it on.
	       */
	      atrshmlog_init_via_env(ATRSHMLOG_STATISTICS_ON_OFF_SUFFIX,
				     &atrshmlog_statistics_on,
				     0,
				     1);


	      /*
	       * We can set the fence flag.
	       * For details to the fence see the doc
	       */
	      atrshmlog_init_via_env(ATRSHMLOG_FENCE_1_SUFFIX,
				     &atrshmlog_thread_fence_1,
				     0,
				     1);

	      atrshmlog_init_via_env(ATRSHMLOG_FENCE_2_SUFFIX,
				     &atrshmlog_thread_fence_2,
				     0,
				     1);

	      atrshmlog_init_via_env(ATRSHMLOG_FENCE_3_SUFFIX,
				     &atrshmlog_thread_fence_3,
				     0,
				     1);

	      atrshmlog_init_via_env(ATRSHMLOG_FENCE_4_SUFFIX,
				     &atrshmlog_thread_fence_4,
				     0,
				     1);

	      atrshmlog_init_via_env(ATRSHMLOG_FENCE_5_SUFFIX,
				     &atrshmlog_thread_fence_5,
				     0,
				     1);

	      atrshmlog_init_via_env(ATRSHMLOG_FENCE_6_SUFFIX,
				     &atrshmlog_thread_fence_6,
				     0,
				     1);

	      atrshmlog_init_via_env(ATRSHMLOG_FENCE_7_SUFFIX,
				     &atrshmlog_thread_fence_7,
				     0,
				     1);

	      atrshmlog_init_via_env(ATRSHMLOG_FENCE_8_SUFFIX,
				     &atrshmlog_thread_fence_8,
				     0,
				     1);

	      atrshmlog_init_via_env(ATRSHMLOG_FENCE_9_SUFFIX,
				     &atrshmlog_thread_fence_9,
				     0,
				     1);

	      atrshmlog_init_via_env(ATRSHMLOG_FENCE_10_SUFFIX,
				     &atrshmlog_thread_fence_10,
				     0,
				     1);

	      atrshmlog_init_via_env(ATRSHMLOG_FENCE_11_SUFFIX,
				     &atrshmlog_thread_fence_11,
				     0,
				     1);

	      atrshmlog_init_via_env(ATRSHMLOG_FENCE_12_SUFFIX,
				     &atrshmlog_thread_fence_12,
				     0,
				     1);
	      
	      /**************************************/
	      
	      
	      /*
	       * Now we are done with the env settings.
	       * So we initialize now the events ( which uses some 
	       * envs by itself, but should be selfcontained
	       * be happy if this works )...
	       */
	      atrshmlog_init_events(0); 
	      
	      if (atexit (atrshmlog_exit_cleanup) != 0)
		result = atrshmlog_error_attach_3;
	      else
		result = atrshmlog_error_ok;
	      
	      // start buffer slave(s)
	      
	      int ret_slave = 0;
	      
	      for (int slaves = 0; ret_slave == 0
		     && slaves < atrshmlog_f_list_buffer_slave_count;
		   slaves++)
		{
		  ret_slave = atrshmlog_create_slave();
		}
	      
	      /*
	       * This is the exception env variable.
	       *
	       * We use it after the thing is running.
	       *
	       * We can switch logging off
	       * for the attach.
	       *
	       * Then its a question of the process to turn it on
	       * this can handle problems with self instantiation
	       * functions before you are ready to log.
	       *
	       * Set it off till you are sure you want to log.
	       *
	       * So this is a case we want to do it after 
	       * the events are ready.
	       *
	       * You can also set slaves to 0 in env or before in process
	       * and then have control over the process of creation of
	       * slaves, then swith logging on ...
	       */
	      p = atrshmlog_get_env(ATRSHMLOG_LOGGING_OFF_SUFFIX);
	      
	      if (!p)
		{
#if ATRSHMLOGDEBUG == 1
		  printf("ATRSHMLOG_LOGGING_OFF_SUFFIX 0\n", p);
#endif
		  atrshmlog_set_logging_process_on();
		}
	      else
		{
#if ATRSHMLOGDEBUG == 1
		  printf("ATRSHMLOG_LOGGING_OFF_SUFFIX 1\n", p);
#endif
		  atrshmlog_set_logging_process_off();
		}
	    }
	}
      
      /* 
       * Little but effectiv trick.
       *
       * We store the pid of the attach in a global read only var 
       * So a thread in a fork can detect the diffrence in pid
       * and restart  slave thread in its init.
       *
       * This should do the job at least for the shell fork
       * problem.
       */
      
      atrshmlog_attach_pid = getpid();

      ++atrshmlog_attach_once;

      // From now on all others see only the initialized module
      // But we still are in the spinlock....
      
      atomic_flag_clear(&atrshmlog_attach_once_flag);

      return result;
    }

  // Ok. We have no env settings. If we have a higher level we can still
  // try the file approach now.

#if ATRSHMLOG_LEVEL > 1

  /* We are through the env. Check for a file as a last resort */
  
  char buff[255];

  strcat(strcat(strcpy(buff, atrshmlog_prefix_name_buffer), ATRSHMLOGENVSUFFIX),".TXT");
    
  FILE *f = fopen(buff, "r");

  if (f)
    {
      long int lshmid;
	    
      if (1 != fscanf(f,"%ld", &lshmid))
	{
	  fclose(f);

	  strcat(strcat(strcpy(buff, atrshmlog_prefix_name_buffer), ATRSHMLOGENVSUFFIX), "=0");
        
	  putenv(buff);
	  --result;

	  ++atrshmlog_attach_once;

	  // From now on all others see only the initialized module
	  // But we still are in the spinlock....
      
	  atomic_flag_clear(&atrshmlog_attach_once_flag);

	  return atrshmlog_error_attach_4;
	}

      /* We have a number. Try to connect now */
      fclose(f);

      /* Ok. We have it, and we think its ok if its not false info. 
       * So we check for endptr points to a zero byte and lshmid is not 0
       */
      if (lshmid != 0)
	{
	  /* This is looking good 
	   * We try to attach
	   */
	  int shmid = (int)lshmid;
		
	  void*shmaddr = (void*)0; /* We let decide the os */ 
		
	  int shmflg = 0; 
		
	  void *shmat_result = shmat(shmid, shmaddr, shmflg);
		
	  if (shmat_result != (void*)-1)
	    {

	      char buff[255];
		    
	      sprintf(buff, "%s%s=%ld", atrshmlog_prefix_name_buffer, ATRSHMLOGENVSUFFIX, lshmid);
	      atrshmlog_base_ptr = shmat_result;
		    
	      atrshmlog_id = shmid;
		    
	      putenv(buff);
	    

	      /*
	       * We dont check for interference from before the attach
	       * so consider to e warned. If you make an env setting
	       * then this is superior in the attach.
	       * No previous setting of the program will survive
	       */

	      /* 
	       * The buffer acquire strategy for shm buffers
	       * This has been not used now, but evtl we need it
	       * again on the reader side. Then we can use it here
	       * to make a better decision on the producer side.
	       * so it is used for now, but the target changes
	       */
	      atrshmlog_init_via_file(ATRSHMLOGSTRATEGYSUFFIX,
				      &atrshmlog_buffer_strategy,
				      atrshmlog_strategy_first,
				      atrshmlog_strategy_last);

	      /* 
	       * The buffer acquire strategy wait nanos wait time.
	       * We can redefine this for the process here.
	       */
	      atrshmlog_init_via_file(ATRSHMLOGSTRATEGYWAITTIMESUFFIX,
				     &atrshmlog_strategy_wait_wait_time,
				     0,
				     999999999);

	      /*
	       * The delimiter character for the arg write call
	       */
	      atrshmlog_init_via_file(ATRSHMLOGDELIMITERSUFFIX,
				      &atrshmlog_delimiter,
				      0,
				      255);
	      /*
	       * The max of events
	       * The event buffer has a aximum size at runtime.
	       * So we hold this stable
	       * Best is to simply use the events and be happy
	       * If you need more you can set this before in 
	       * the program
	       * If this is not what you want you can redefine the max 
	       * in the internal and recompile.
	       * If you dont want to do it you can here reset the thing
	       * but it has a cost: only dyn memory after this ...
	       */
	      strcat(strcat(strcpy(buff, atrshmlog_prefix_name_buffer), ATRSHMLOGEVENTCOUNTSUFFIX), ".TXT");
		    
	      f= fopen(buff, "r");
		
	      if (f)
		{
		  long int new_max ;
		    
		  if (1 == fscanf(f,"%ld", &new_max))
		    {
		      if (new_max >  ATRSHMLOGEVENTCOUNT
			  && new_max > atrshmlog_event_locks_max
			  && new_max <= ATRSHMLOGEVENTCOUNTMAXLIMIT)
			{
			  int oldmax = atrshmlog_event_locks_max;
			    
			  atrshmlog_event_t * p2 =  (atrshmlog_event_t*)malloc( new_max + 1);

			  if (p2)
			    {
			      memset ( p2, 0, new_max + 1);
				
			      atrshmlog_event_locks = p2;

			      atrshmlog_event_locks_max = new_max;
			      sprintf(buff, "%s%s=%ld", atrshmlog_prefix_name_buffer, ATRSHMLOGEVENTCOUNTSUFFIX, new_max);
			      putenv(buff);
			    }
			}
		    }

		  fclose(f);
		}
			    

	      /*
	       * We can redefine the memory we need for a dyn alloc buffer
	       * We cannot redefine the compiled statics.
	       * So be sure you want it to be this way
	       * We also cannot redefine the shm limit.
	       * So we can only reduce the dyn alloced size.
	       * Anything else and you have to recompile after
	       * change of the define
	       */
	      atrshmlog_init_via_file(ATRSHMLOGBUFFER_INFOSIZE_SUFFIX,
				      &atrshmlog_buffer_infosize,
				      ATRSHMLOGBUFFER_INFOSIZE_MIN,
				      ATRSHMLOGBUFFER_INFOSIZE);

	      /*
	       * We preallocate in a malloc an amount of buffers
	       * to reduce alloc time per buffer
	       * So its possible to reset the number 
	       * You cannot reset the number the static has - you
	       * have to recomile for this
	       * But for the dyn buffers you can do it 
	       */
	      atrshmlog_init_via_file(ATRSHMLOG_INIT_PREALLOC_COUNT_SUFFIX,
				      &atrshmlog_prealloc_buffer_count,
				      ATRSHMLOG_INIT_PREALLOC_COUNT_MIN,
				      ATRSHMLOG_MAX_PREALLOC_COUNT);

	      /*
	       * We can set the wait nanos for the slave
	       * Having it small consumes cpu.
	       * Having it big makes the check to bee late and later ...
	       * So i use 1000 as a start.
	       * But its not the only thing to know
	       * in case we use the maximum number of slaves
	       * it becomes clear that then no thread
	       * should interact with the others, so this
	       * makes a shift to a model with another 
	       * strategy.
	       */
	      atrshmlog_init_via_file(ATRSHMLOG_INIT_BUFFER_SLAVE_WAIT_SUFFIX,
				      &atrshmlog_f_list_buffer_slave_wait,
				      ATRSHMLOG_INIT_BUFFER_SLAVE_WAIT_MIN,
				      ATRSHMLOG_INIT_BUFFER_SLAVE_WAIT_MAX);
	      /*
	       * We can set the count of slave threads for dispatching 
	       * full buffers to shm
	       * This is depending to the strategy
	       * For now it simply works
	       * Its up to you if you want to stop - meaning 0
	       * or use a rackmount of slaves - max
	       * to use a good number is a thing you are told in the 
	       * documentation.
	       * So read it before you guess anything.
	       */
	      atrshmlog_init_via_file(ATRSHMLOG_SLAVE_COUNT_SUFFIX,
				      &atrshmlog_f_list_buffer_slave_count,
				      ATRSHMLOG_SLAVE_COUNT_MIN,
				      ATRSHMLOG_SLAVE_COUNT_MAX);
		    
	      /*
	       * We normally do not wait for the slaves in the cleanup.
	       * In theory a progam should have stoppt
	       * them before dieing ...
	       * And the atexit is a feature for a dieing
	       * program
	       * but if you insist you can
	       * wait for the slaves to finish.
	       */
	      atrshmlog_init_via_file(ATRSHMLOG_WAIT_FOR_SLAVES_SUFFIX,
				      &atrshmlog_wait_for_slaves,
				      0,
				      1);

	      /*
	       * We can set the clock id here 
	       * If you copy the binary to another platform
	       * that is nearby you can use that to
	       * handle a diffrent clock tick getter 
	       */
	      atrshmlog_init_via_file(ATRSHMLOG_CLOCK_ID_SUFFIX,
				      (int*)&atrshmlog_clock_id,
				      ATRSHMLOG_CLOCK_ID_MIN,
				      ATRSHMLOG_CLOCK_ID_MAX);
		
	      /*
	       * We can set the statistics flag here.
	       * If you want to have statistics you have to switch
	       * it on.
	       */
	      atrshmlog_init_via_file(ATRSHMLOG_STATISTICS_ON_OFF_SUFFIX,
				      &atrshmlog_statistics_on,
				      0,
				      1);

	      /*
	       * We can set the fence flag
	       * For details to the fence see the doc
	       */
	      atrshmlog_init_via_file(ATRSHMLOG_FENCE_1_SUFFIX,
				      &atrshmlog_thread_fence_1,
				      0,
				      1);

	      atrshmlog_init_via_file(ATRSHMLOG_FENCE_2_SUFFIX,
				      &atrshmlog_thread_fence_2,
				      0,
				      1);

	      atrshmlog_init_via_file(ATRSHMLOG_FENCE_3_SUFFIX,
				      &atrshmlog_thread_fence_3,
				      0,
				      1);

	      atrshmlog_init_via_file(ATRSHMLOG_FENCE_4_SUFFIX,
				      &atrshmlog_thread_fence_4,
				      0,
				      1);

	      atrshmlog_init_via_file(ATRSHMLOG_FENCE_5_SUFFIX,
				      &atrshmlog_thread_fence_5,
				      0,
				      1);

	      atrshmlog_init_via_file(ATRSHMLOG_FENCE_6_SUFFIX,
				      &atrshmlog_thread_fence_6,
				      0,
				      1);

	      atrshmlog_init_via_file(ATRSHMLOG_FENCE_7_SUFFIX,
				      &atrshmlog_thread_fence_7,
				      0,
				      1);

	      atrshmlog_init_via_file(ATRSHMLOG_FENCE_8_SUFFIX,
				      &atrshmlog_thread_fence_8,
				      0,
				      1);

	      atrshmlog_init_via_file(ATRSHMLOG_FENCE_9_SUFFIX,
				      &atrshmlog_thread_fence_9,
				      0,
				      1);

	      atrshmlog_init_via_file(ATRSHMLOG_FENCE_10_SUFFIX,
				      &atrshmlog_thread_fence_10,
				      0,
				      1);

	      atrshmlog_init_via_file(ATRSHMLOG_FENCE_11_SUFFIX,
				      &atrshmlog_thread_fence_11,
				      0,
				      1);

	      atrshmlog_init_via_file(ATRSHMLOG_FENCE_12_SUFFIX,
				      &atrshmlog_thread_fence_12,
				      0,
				      1);

	      /*
	       * We are through the setting stuff.
	       * We now init the events and then press the major
	       * button ...
	       */
		    
	      atrshmlog_init_events(1);

	      if (atexit (atrshmlog_exit_cleanup) != 0)
		result = atrshmlog_error_attach_6;
	      else 
		result = atrshmlog_error_ok;


	      // start buffer slave(s)

	      int ret_slave = 0;
		    
	      for (int slaves = 0; ret_slave == 0
		     && slaves < atrshmlog_f_list_buffer_slave_count;
		   slaves++)
		{
		  ret_slave = atrshmlog_create_slave();
		}

	      /*
	       * This is the exception env variable.
	       * We use it after the thing is running
	       * We can switch logging off
	       * for the attach.
	       * Then its a question of the process to turn it on
	       * This can handle problems with self instantiation
	       * functions before you are ready to log
	       * Set it off till you are sure you want to log
	       * So this is a case we want to do it after 
	       * the events are ready.
	       * You can also set slaves to 0 in env or before in process
	       * and then have control over the process of creation of
	       * slaves, then swith logging on ...
	       */
	      strcat(strcat(strcpy(buff, atrshmlog_prefix_name_buffer), ATRSHMLOG_LOGGING_OFF_SUFFIX), ".TXT");
	      f= fopen(buff, "r");

	      if (!f)
		{
#if ATRSHMLOGDEBUG == 1
		  printf("ATRSHMLOG_LOGGING_OFF_SUFFIX 0\n", !f);
#endif
		  atrshmlog_set_logging_process_on();
		}
	      else
		{
#if ATRSHMLOGDEBUG == 1
		  printf("ATRSHMLOG_LOGGING_OFF_SUFFIX 1\n", !f);
#endif
		  atrshmlog_set_logging_process_off();

		  fclose(f);
		}
	    }
	  else
	    {
	      // the attach failed...
	      strcat(strcat(strcpy(buff, atrshmlog_prefix_name_buffer), ATRSHMLOGENVSUFFIX), "=0");
        
        
	      putenv(buff);
	    }         
	}
      else
	{
	  // The shmid was still not there
	  strcat(strcat(strcpy(buff, atrshmlog_prefix_name_buffer), ATRSHMLOGENVSUFFIX), "=0");
        
	  putenv(buff);
	}         
    }

#else
  result = atrshmlog_error_attach_5;
#endif
      
  ++atrshmlog_attach_once;

  // From now on all others see only the initialized module
  // But we still are in the spinlock....
      
  atomic_flag_clear(&atrshmlog_attach_once_flag);

  return result; /* ok. we did it */
}

/*******************************************************************/

/**
 * \n Main code:
 *
 * \brief The start of a slave thread.
 * 
 * You can use this if you have very diffrent needs for the logging
 * and you can judge from the program to do it.
 * 
 * Sometimes a program needs only few threads for logging and so few for
 * the slave activity.
 *
 * When then a hot spot of logging comes up you want to raise the number
 * of threads for the slaves too.
 *
 * So you call this one and a new slave is created.
 */
int atrshmlog_create_slave(void)
{
  ATRSHMLOGSTAT(atrshmlog_counter_create_slave);

#if  ATRSHMLOG_USE_PTHREAD  == 1

  int ret = pthread_create((pthread_t *)&atrshmlog_f_list_buffer_slave,
		 NULL,
		 atrshmlog_f_list_buffer_slave_proc,
		 NULL);
#endif

#if  ATRSHMLOG_USE_C11_THREAD == 1
  int ret = thrd_create( (thrd_t*)&atrshmlog_f_list_buffer_slave,
			 atrshmlog_f_list_buffer_slave_proc,
			 NULL);
#endif
  return ret;
}



/*******************************************************************/


/**
 * \n Main code:
 *
 * \brief We can stop logging for a thread with this function.
 *
 * The thread then release the buffers and thats it.
 *
 * No more logging after this from this thread.
 *
 * So be sure you do this in thread end.
 *
 * And be sure not to do this before you have to. 
 */
void atrshmlog_stop(void)
{
  ATRSHMLOGSTAT(atrshmlog_counter_stop);
  
  g_tl_t* g  = &g_tl;
  
  if (g->atrshmlog_idnotok == 0)
    {
      for (int i = 0; i < ATRSHMLOGTARGETBUFFERMAX; i++)
	{
	  tbuff_t* t = g->atrshmlog_targetbuffer_arr[i];

	  if (t)
	    t->dispose = 1;

	  atrshmlog_dispatch_buffer(t);
	}
    }      

  // We ultimately stop logging for this thread.
  g->atrshmlog_idnotok = 1;
}

/**********************************************************/

/**
 * \n Main code:
 *
 * \brief We flush the used buffers with this function.
 *
 * So this is a small slow down if we have to do it.
 *
 * Normally one buffer should be empty any way...
 *
 */
void atrshmlog_flush(void)
{
  ATRSHMLOGSTAT(atrshmlog_counter_flush);
  
  g_tl_t* g = &g_tl;
  
  for (int i = 0; i < ATRSHMLOGTARGETBUFFERMAX; i++)
    {
      tbuff_t* t = g->atrshmlog_targetbuffer_arr[i];

      atrshmlog_dispatch_buffer(t);
    }
}

/**********************************************************/

/**
 * \n Main code:
 *
 * \brief We set a new strategy for this thread.
 *
 * \param i_strategy
 * see enum atrshmlog_strategy
 *
 */
atrshmlog_ret_t atrshmlog_set_strategy(const enum atrshmlog_strategy i_strategy)
{
  ATRSHMLOGSTAT(atrshmlog_counter_set_strategy);
  
  g_tl_t* g = &g_tl;

  int old = g->strategy;

  if (i_strategy >= atrshmlog_strategy_first && i_strategy <= atrshmlog_strategy_last)
    {
      g->strategy = i_strategy;
    }

  return old;
}

/**********************************************************/

/**
 * \n Main code:
 *
 * \brief We set a new strategy for the process.
 *
 * \param i_strategy
 * see enum atrshmlog_strategy
 */
atrshmlog_ret_t atrshmlog_set_strategy_process(const enum atrshmlog_strategy i_strategy)
{
  ATRSHMLOGSTAT(atrshmlog_counter_set_strategy_process);

  int old = atrshmlog_buffer_strategy;

  if (i_strategy >= atrshmlog_strategy_first
      && i_strategy <= atrshmlog_strategy_last)
    {
       atrshmlog_buffer_strategy= i_strategy;
    }

  return old;
}


/*******************************************************************/

/**
 * \brief helper : We init in write
 * This is the normal way for a writing process .
 * The thread that wants to log has to do the init thing
 * So the first call is a bit more time consuming.
 * But we dont need a thread attach or so. 
 * We simply execute a write.
 *
 * \param g
 * The adress of the thread local struct for this thread.
 *
 * \return
 * - 0: ok
 * - non zero : error
 */
static int atrshmlog_init_in_write(g_tl_t* g)
{
  int tlret = atrshmlog_init_thread_local (g);

  /* from now on we are the not ok flag */
  if (tlret)
    return tlret;

  /*
   * Interrim. We need to check if we are a fork clone 
   * This is bit dirty
   * We use the process pid and the pid we got in thread init
   * and check if these are same.
   * If we are a fork clone we have different numbers.
   * So we can then do the needed thing .
   */
  if (atrshmlog_attach_pid != g->atrshmlog_thread_pid)
    {
      /* We have to change the pid */
      atrshmlog_attach_pid = g->atrshmlog_thread_pid;

      /* This is NOT mt safe, but i assume it does not matter 
       * Ok. This is risky. But for now we assume its a SHELL, 
       * and shells are NORMALLY not mt things ...
       * So we have come so far, we need a slave thread to do 
       * the transfer work.
       */
      int ret_slave = 0;
		  
      for (int slaves = 0; ret_slave == 0
	     && slaves < atrshmlog_f_list_buffer_slave_count;
	   slaves++)
	{
	  ret_slave = atrshmlog_create_slave();
	}
#if ATRSHMLOGDEBUG == 1
      printf("Process %ld startet slaves %ld .\n" , (long)atrshmlog_thread_pid, (long)ret_slave);
#endif
    }
      

  /* We reach this only if we are still ok 
   * and we are the initialized flag before 
   * so we now have to make all those buffers and list linking
   */
  for (int i = 0; i < ATRSHMLOGTARGETBUFFERMAX; i++)
    {
      g->atrshmlog_targetbuffer_arr[i] = atrshmlog_acquire_buffer(g);
      
      if (g->atrshmlog_targetbuffer_arr[i] == 0)
	{
	  g->atrshmlog_idnotok = 1; // no logging for this thread
	  return atrshmlog_error_init_in_write_1;
	}
    }
	    
  // we start by using first buffer
  g->atrshmlog_targetbuffer_index = 0;

  return atrshmlog_error_ok;
}


/********************************************************************/

/** 
 * \n Main code:
 *
 * \brief We write to the log. 
 *
 * This is the major writer. 
 * In this form we only write the basic log info.
 *
 * There is a hidden mechanism.
 *
 * If we do a log in P or p eventflag, we use only starttime.
 * and if this is 0 we call internal to get the time.
 *
 * If the eventflag is not P or p we check the endtime.
 * If it is 0 we get the time and use it for the endtime.
 *
 * These two help us to minimize calls in case we are bound
 * in a fat layer for another languge than c or c++.
 * Its a fact that you then have to minimize the number of calls.
 * In numbers: gettime naket 40 clicks. Good. 
 * In JNI 300 clicks minimum . BAD.
 * So we use the hidden mechanism to make use of time without a 
 * call in P or p and with one call in other events.
 * this is a very last late chnge, i allways beleived it would be BAD 
 * to mix up calls and their responsibility.
 * So i check this first in an early state of the module. 
 * But after the thing was nearly ready a test drive with jni 
 * showed me the truth. 
 * It was necessary to reduce JNI calls.
 * so i gave up my resistance against this BORG like Java mentality
 * and did the change - forgive me you joung and
 * innovent language lawyer - but its a fact of simple survival 
 * to make things like this when it comes to an audiance that 
 * make their part so BAD that they insist that you make some 
 * kind of a hybrid of a swiss army nife and a toolbox...
 *
 */
atrshmlog_ret_t atrshmlog_write0(const atrshmlog_int32_t i_eventnumber,
				 const atrshmlog_int32_t i_eventflag,
				 const atrshmlog_int32_t i_userflag,
				 atrshmlog_time_t i_starttime,
				 atrshmlog_time_t i_endtime)
{
  ATRSHMLOGSTAT(atrshmlog_counter_write0);
  
  if (i_eventnumber < 0) {
    ATRSHMLOGSTAT(atrshmlog_counter_write0_abort1);
    
    return atrshmlog_error_write0_1;
  }
  
  if (i_eventnumber >= atrshmlog_event_locks_max) {
    ATRSHMLOGSTAT(atrshmlog_counter_write0_abort2);

    return atrshmlog_error_write0_2;
  }

  g_tl_t* g  = &g_tl;
  
  /* We use this flag also as the initialized flag
   * So we have all the initialize stuff hereafter 
   */
  if (g->atrshmlog_idnotok != 0)
    {
      if (g->atrshmlog_idnotok != -1) {
	ATRSHMLOGSTAT(atrshmlog_counter_write0_abort3);

	return atrshmlog_error_write0_3;
      }
      
      int ret = atrshmlog_init_in_write(g);

      if (ret != 0)
	{
	  ATRSHMLOGSTAT(atrshmlog_counter_write0_abort4);

	  return ret;
	}
    }

  // Initialized

  // The hidden mechanism to get things minimised
  // in case we are bound to a layer for another language.
  if (i_eventflag == ATRSHMLOGPOINTINTIMEP
       || i_eventflag == ATRSHMLOGPOINTINTIMEp
      )
    {
      if (i_starttime == 0)
	i_starttime = ATRSHMLOG_GET_TSC_CALL();

      i_endtime = i_starttime;
    }
  else
    {
      if (i_endtime == 0)
	i_endtime = ATRSHMLOG_GET_TSC_CALL();
    }

  
  atrshmlog_int32_t totallen;

  // Do the normal stuff now ...

  totallen = ATRSHMLOGCONTROLDATASIZE;

  int strategy_count = 0;
  
  // We use some goto jumping here, so this is a first target
 testagain_dispatched:

  ;
    
  register tbuff_t* tbuff = g->atrshmlog_targetbuffer_arr[g->atrshmlog_targetbuffer_index];

  if (atomic_load_explicit(&(tbuff->dispatched), memory_order_acquire) != 0)
    {
      g->atrshmlog_targetbuffer_index++;

      if (g->atrshmlog_targetbuffer_index >= ATRSHMLOGTARGETBUFFERMAX)
	g->atrshmlog_targetbuffer_index = 0;

      ++strategy_count;

      if ((strategy_count % ATRSHMLOGTARGETBUFFERMAX) == 0)
	switch (g->strategy)
	  {
	  case atrshmlog_strategy_discard :
	    {
	      ATRSHMLOGSTAT(atrshmlog_counter_write0_discard);
	      
	      // we discard
	      return atrshmlog_error_write0_4;
	    }

	  case atrshmlog_strategy_spin_loop:
	    // we spin loop
	    break;

	  case atrshmlog_strategy_wait:
	    {
	      ATRSHMLOGSTAT(atrshmlog_counter_write0_wait);
	      
	      // we wait fix time
	      atrshmlog_nanosleep(atrshmlog_strategy_wait_wait_time);
	    }
	    break;

	  case atrshmlog_strategy_adaptive:
	    {
	      ATRSHMLOGSTAT(atrshmlog_counter_write0_adaptive);

	      atrshmlog_time_t t = g->atrshmlog_targetbuffer_arr[g->atrshmlog_targetbuffer_index]->transfer_to_shm_time ;

	      t /=  ATRSHMLOGTARGETBUFFERMAX;

#if 0
	      printf("adapotive %ld\n", (long)t);
#endif
	      
	      if (t > 999999999)
		t = 999999999;
	      
	      // adaptive : take the last transfer time and divide it
	      atrshmlog_nanosleep(t);
	    }
	    break;
	    
	  case atrshmlog_strategy_adaptive_fast:
	    {
	      ATRSHMLOGSTAT(atrshmlog_counter_write0_adaptive_fast);

	      atrshmlog_time_t t = g->atrshmlog_targetbuffer_arr[g->atrshmlog_targetbuffer_index]->transfer_to_shm_time ;

	      t /=  ATRSHMLOGTARGETBUFFERMAX * 2;

#if 0
	      printf("adapotive %ld\n", (long)t);
#endif
	      
	      if (t > 999999999)
		t = 999999999;
	      
	      // adaptive fast : take the last transfer time and divide it 2
	      atrshmlog_nanosleep(t);
	    }
	    break;

	  case atrshmlog_strategy_adaptive_very_fast:
	    {
	      ATRSHMLOGSTAT(atrshmlog_counter_write0_adaptive_very_fast);

	      // adaptive very fast : take the last transfer time and divide it 10
	      atrshmlog_time_t t = g->atrshmlog_targetbuffer_arr[g->atrshmlog_targetbuffer_index]->transfer_to_shm_time ;

	      t /=  ATRSHMLOGTARGETBUFFERMAX * 10;

#if 0
	      printf("adapotive %ld\n", (long)t);
#endif
	      
	      if (t > 999999999)
		t = 999999999;
	      
	      // adaptive fast : take the last transfer time and divide it 2
	      atrshmlog_nanosleep(t);
	    }
	    break;
	    
	  default:
	    // we spin loop
	    break;
	  }
      
      if (atrshmlog_logging_process_off_final)
	return atrshmlog_error_write0_5;

      atrshmlog_area_t * a_shm = ATRSHMLOG_GETAREA;

      /* Can be happen : end of logging anounced by user via flag in shm */
      if (atomic_load_explicit(&a_shm->ich_habe_fertig, memory_order_acquire) != 0) 
	return atrshmlog_error_write0_6;
      
      // There is no way to do the things follow for a dispatched buffer
      goto testagain_dispatched;
    }

  if (atrshmlog_thread_fence_2)
    atomic_thread_fence (memory_order_acquire);

  // Ok. We have an undispatched buffer 
  register size_t akindex = tbuff->size;

  // Is the size limit reached ? 
  if (akindex + totallen > tbuff->maxsize )
    {
#if ATRSHMLOGDEBUG == 1
      printf("size hit %ld %ld\n", (long)tbuff->id, (long)akindex);
#endif
	 
      // Checkings: valid buffer 
      if (tbuff->safeguardfront != ATRSHMLOGSAFEGUARDVALUE) {
	ATRSHMLOGSTAT(atrshmlog_counter_write_safeguard);
	
	return atrshmlog_error_write0_7;
      }
      
      atrshmlog_area_t * a_shm = ATRSHMLOG_GETAREA;

      /* Bad thing. safeguard invalid */
      if (a_shm->shmsafeguard != ATRSHMLOGSAFEGUARDVALUE) {
	ATRSHMLOGSTAT(atrshmlog_counter_write_safeguard_shm);

	return atrshmlog_error_write0_8;
      }
  
      /* Can be happen : end of logging anounced by user via flag in shm */
      if (atomic_load_explicit(&a_shm->ich_habe_fertig, memory_order_acquire) != 0) 
	return atrshmlog_error_write0_9;

      atrshmlog_dispatch_buffer(tbuff);

      // Switch the targetbuffer and try again
      g->atrshmlog_targetbuffer_index++;

      if (g->atrshmlog_targetbuffer_index >= ATRSHMLOGTARGETBUFFERMAX)
	g->atrshmlog_targetbuffer_index = 0;

      // End of full buffer handling
      goto testagain_dispatched;
    } 

  // Ok. we have an undispatched and not full buffer.
    
  register char* p = tbuff->b + akindex;

  // Transfer the log data
  memcpy(p, &i_starttime, sizeof(atrshmlog_time_t));
  memcpy(p + sizeof(atrshmlog_time_t), &i_endtime, sizeof(atrshmlog_time_t));
  memcpy(p + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_time_t), &totallen, sizeof(atrshmlog_int32_t));
  memcpy(p + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_int32_t) , &i_eventnumber, sizeof(atrshmlog_int32_t));
  memcpy(p + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_int32_t) + sizeof(atrshmlog_int32_t), &i_userflag, sizeof(atrshmlog_int32_t));

  *(p + ( ATRSHMLOGCONTROLDATASIZE - 1 ) )= (char)i_eventflag;

  akindex += totallen;

  tbuff->size = akindex;

  return atrshmlog_error_ok;
}

/*******************************************************************/

/** 
 * \n Main code:
 *
 * \brief We write to the log. 
 *
 * This is the second major writer. 
 * This is the write with one payload buffer.
 * We can put any kind of info on the payload here
 * which gives us a simple way to handle all those 
 * language info forms. 
 * For this to work
 * we only need start and length of the used info.
 * This is then transfered binary.
 * The first thing you need to do is then to clone the convert and
 * hack into it a way to interpret the binary.
 * The default is using the data as text in a c program.
 * See the java example for this kind of stuff.
 */
atrshmlog_ret_t atrshmlog_write1(const atrshmlog_int32_t i_eventnumber,
				 const atrshmlog_int32_t i_eventflag,
				 const atrshmlog_int32_t i_userflag,
				 atrshmlog_time_t i_starttime,
				 atrshmlog_time_t i_endtime,
				 const void* i_local,
				 const atrshmlog_int32_t i_size)
{
  ATRSHMLOGSTAT(atrshmlog_counter_write1);

  /* This is a two way used one. Its the size and the flag too */
  int copy_local = i_size;

  /* No logging , wrong parameter ... */
  if (i_size < 0) {
    ATRSHMLOGSTAT(atrshmlog_counter_write1_abort5);
    
    return atrshmlog_error_write1_1;
  }
  
  /* If the pointer is null we set the length to 0 */
  if (!i_local)
    copy_local = 0;

  if (i_eventnumber < 0) {
    ATRSHMLOGSTAT(atrshmlog_counter_write1_abort1);
    
    return atrshmlog_error_write1_2;
  }

  if (i_eventnumber >= atrshmlog_event_locks_max) {
    ATRSHMLOGSTAT(atrshmlog_counter_write1_abort2);
    
    return atrshmlog_error_write1_3;
  }
  
  if (i_size >= ATRSHMLOGBUFFER_INFOSIZE - ATRSHMLOGCONTROLDATASIZE - 1)
    {
      ATRSHMLOGSTAT(atrshmlog_counter_write1_abort6);
      // Cannot work.
      return atrshmlog_error_write1_4;
    }

  g_tl_t* g = &g_tl;

  /* We use this flag also as the initialized flag
   * So we have all the initialize stuff hereafter 
   */
  if (g->atrshmlog_idnotok != 0)
    {
      if (g->atrshmlog_idnotok != -1) {
	ATRSHMLOGSTAT(atrshmlog_counter_write1_abort3);
	
	return atrshmlog_error_write1_5;
      }
      
      int ret = atrshmlog_init_in_write(g);

      if (ret != 0)
	{
	  ATRSHMLOGSTAT(atrshmlog_counter_write1_abort4);

	  return ret;
	}
    }

  // Initialized

  // The hidden mechanism to get things minimised
  // in case we are bound to a layer for another language.
  if (i_eventflag == ATRSHMLOGPOINTINTIMEP
       || i_eventflag == ATRSHMLOGPOINTINTIMEp
      )
    {
      if (i_starttime == 0)
	i_starttime = ATRSHMLOG_GET_TSC_CALL();

      i_endtime = i_starttime;
    }
  else
    {
      if (i_endtime == 0)
	i_endtime = ATRSHMLOG_GET_TSC_CALL();
    }


  atrshmlog_int32_t totallen;
  
  // Do the normal stuff now ...

  totallen = ATRSHMLOGCONTROLDATASIZE + copy_local;

  int strategy_count = 0;
  
  // We use some goto jumping here, so this is a first target
 testagain_dispatched:

  ;
    
  register tbuff_t* tbuff = g->atrshmlog_targetbuffer_arr[g->atrshmlog_targetbuffer_index];

  if (atomic_load_explicit(&(tbuff->dispatched), memory_order_acquire) != 0)
    {
      g->atrshmlog_targetbuffer_index++;

      if (g->atrshmlog_targetbuffer_index >= ATRSHMLOGTARGETBUFFERMAX)
	g->atrshmlog_targetbuffer_index = 0;


      ++strategy_count;

      if ((strategy_count % ATRSHMLOGTARGETBUFFERMAX) == 0)
	switch (g->strategy)
	  {
	  case atrshmlog_strategy_discard :
	    {
	      ATRSHMLOGSTAT(atrshmlog_counter_write1_discard);
	      
 	      // we discard
	      return atrshmlog_error_write1_6;
	    }

	  case atrshmlog_strategy_spin_loop:
	    // we spin loop
	    break;

	  case atrshmlog_strategy_wait:
	    {
	      ATRSHMLOGSTAT(atrshmlog_counter_write1_wait);
	      
	      // we wait fix time
	      atrshmlog_nanosleep(atrshmlog_strategy_wait_wait_time);
	    }
	    break;

	  case atrshmlog_strategy_adaptive:
	    {
	      ATRSHMLOGSTAT(atrshmlog_counter_write1_adaptive);

	      atrshmlog_time_t t = g->atrshmlog_targetbuffer_arr[g->atrshmlog_targetbuffer_index]->transfer_to_shm_time ;

	      t /=  ATRSHMLOGTARGETBUFFERMAX;

#if 0
	      printf("adapotive %ld\n", (long)t);
#endif
	      
	      if (t > 999999999)
		t = 999999999;
	      
	      // adaptive : take the last transfer time and divide it
	      atrshmlog_nanosleep(t);
	    }
	    break;
	    
	  case atrshmlog_strategy_adaptive_fast:
	    {
	      ATRSHMLOGSTAT(atrshmlog_counter_write1_adaptive_fast);

	      atrshmlog_time_t t = g->atrshmlog_targetbuffer_arr[g->atrshmlog_targetbuffer_index]->transfer_to_shm_time ;

	      t /=  ATRSHMLOGTARGETBUFFERMAX * 2;

#if 0
	      printf("adapotive %ld\n", (long)t);
#endif
	      
	      if (t > 999999999)
		t = 999999999;
	      
	      // adaptive fast : take the last transfer time and divide it 2
	      atrshmlog_nanosleep(t);
	    }
	    break;

	  case atrshmlog_strategy_adaptive_very_fast:
	    {
	      ATRSHMLOGSTAT(atrshmlog_counter_write1_adaptive_very_fast);

	      // adaptive very fast : take the last transfer time and divide it 10
	      atrshmlog_time_t t = g->atrshmlog_targetbuffer_arr[g->atrshmlog_targetbuffer_index]->transfer_to_shm_time ;

	      t /=  ATRSHMLOGTARGETBUFFERMAX * 10;

#if 0
	      printf("adapotive %ld\n", (long)t);
#endif
	      
	      if (t > 999999999)
		t = 999999999;
	      
	      // adaptive fast : take the last transfer time and divide it 2
	      atrshmlog_nanosleep(t);
	    }
	    break;
	    
	  default:
	    // we spin loop
	    break;
	  }
      
      if (atrshmlog_logging_process_off_final)
	return atrshmlog_error_write1_7;

      atrshmlog_area_t * a_shm = ATRSHMLOG_GETAREA;

      /* Can be happen : end of logging anounced by user via flag in shm */
      if (atomic_load_explicit(&a_shm->ich_habe_fertig, memory_order_acquire) != 0) 
	return atrshmlog_error_write1_8;
      
      // There is no way to do the things follow for a dispatched buffer
      goto testagain_dispatched;
    }

  if (atrshmlog_thread_fence_2)
    atomic_thread_fence (memory_order_acquire);

  // special case for the payload thing
  if (totallen > tbuff->maxsize) {
    ATRSHMLOGSTAT(atrshmlog_counter_write1_abort7);
    
    return atrshmlog_error_write1_9; // Cannot work.
  }
  
  // Ok. We have an undispatched buffer 
  register size_t akindex = tbuff->size;

  // Is the size limit reached ? 
  if (akindex + totallen > tbuff->maxsize )
    {
#if ATRSHMLOGDEBUG == 1
      printf("size hit %ld %ld\n", (long)tbuff->id, (long)akindex);
#endif
	 
      // Checkings: valid buffer 
      if (tbuff->safeguardfront != ATRSHMLOGSAFEGUARDVALUE) {
	ATRSHMLOGSTAT(atrshmlog_counter_write_safeguard);

	return atrshmlog_error_write1_10;
      }

      atrshmlog_area_t * a_shm = ATRSHMLOG_GETAREA;

      /* Bad thing. Safeguard invalid */
      if (a_shm->shmsafeguard != ATRSHMLOGSAFEGUARDVALUE) {
	ATRSHMLOGSTAT(atrshmlog_counter_write_safeguard_shm);

	return atrshmlog_error_write1_11;
      }
      
      /* Can be happen : end of logging anounced by user via flag in shm */
      if (atomic_load_explicit(&a_shm->ich_habe_fertig, memory_order_acquire) != 0) 
	return atrshmlog_error_write1_12;


      atrshmlog_dispatch_buffer(tbuff);

      // Switch the targetbuffer and try again
      g->atrshmlog_targetbuffer_index++;

      if (g->atrshmlog_targetbuffer_index >= ATRSHMLOGTARGETBUFFERMAX)
	g->atrshmlog_targetbuffer_index = 0;

      // End of full buffer handling
      goto testagain_dispatched;
    } 

  // Ok. We have an undispatched and not full buffer.
    
  register char* p = tbuff->b + akindex;

  // Transfer the log data
  memcpy(p, &i_starttime, sizeof(atrshmlog_time_t));
  memcpy(p + sizeof(atrshmlog_time_t), &i_endtime, sizeof(atrshmlog_time_t));
  memcpy(p + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_time_t), &totallen, sizeof(atrshmlog_int32_t));
  memcpy(p + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_int32_t) , &i_eventnumber, sizeof(atrshmlog_int32_t));
  memcpy(p + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_int32_t) + sizeof(atrshmlog_int32_t), &i_userflag, sizeof(atrshmlog_int32_t));

  *(p + ( ATRSHMLOGCONTROLDATASIZE - 1 ) )= (char)i_eventflag;

  if (copy_local)
    {
      memcpy(p + ATRSHMLOGCONTROLDATASIZE, i_local, copy_local);
    }
    

  akindex += totallen;

  tbuff->size = akindex;
    
  return atrshmlog_error_ok;
}

/*******************************************************************/

/** 
 * \n Main code:
 *
 * \brief We write to the log .
 *
 * This is the major writer. 
 * This is a special version for our main audiance.
 * Its for the argv array approch found in so many c 
 * programs.
 * So this is somehow broken for info that tries to use
 * null bytes. 
 * Sorry, but this is how c works.
 * If you need fancy info with null you have to put it in the 
 * first payload buffer.
 */
atrshmlog_ret_t atrshmlog_write2(const atrshmlog_int32_t i_eventnumber,
				 const atrshmlog_int32_t i_eventflag,
				 const atrshmlog_int32_t i_userflag,
				 atrshmlog_time_t i_starttime,
				 atrshmlog_time_t i_endtime,
				 const void* i_local,
				 const atrshmlog_int32_t i_size,
				 const char* i_argv[],
				 const atrshmlog_int32_t i_argc_hint)
{
  ATRSHMLOGSTAT(atrshmlog_counter_write2);

  // this is a bit a compromise now.
  // in a later version i try to move the argv directly,
  // so we spare time and space. But for now
  // we use a helper here ...
  char atrshmlog_argvbuffer[ATRSHMLOGARGVBUFFERLEN];
  
  /* This is a two way used one. Its the size and the flag too */
  int copy_local = i_size;

  /* No logging , wrong parameter ... */
  if (i_size < 0) {
    ATRSHMLOGSTAT(atrshmlog_counter_write2_abort5);

    return atrshmlog_error_write2_1;
  }
  
  /* If the pointer is null we set the length to 0 */
  if (!i_local)
    copy_local = 0;

  if (i_eventnumber < 0) {
    ATRSHMLOGSTAT(atrshmlog_counter_write2_abort1);
    
    return atrshmlog_error_write2_2;
  }
  
  if (i_eventnumber >= atrshmlog_event_locks_max) {
    ATRSHMLOGSTAT(atrshmlog_counter_write2_abort2);
    
    return atrshmlog_error_write2_3;
  }
  
  if (i_size >= ATRSHMLOGBUFFER_INFOSIZE - ATRSHMLOGCONTROLDATASIZE - 1)
    {
      ATRSHMLOGSTAT(atrshmlog_counter_write2_abort6);

      // Cannot work.
      return atrshmlog_error_write2_4;
    }

  g_tl_t* g = &g_tl;

  /* We use this flag also as the initialized flag
   * So we have all the initialize stuff hereafter 
   */
  if (g->atrshmlog_idnotok != 0)
    {
      if (g->atrshmlog_idnotok != -1) {
	ATRSHMLOGSTAT(atrshmlog_counter_write2_abort3);
	
	return atrshmlog_error_write2_5;
      }
      
      int ret = atrshmlog_init_in_write(g);

      if (ret != 0)
	{
	  ATRSHMLOGSTAT(atrshmlog_counter_write2_abort4);

	  return ret;
	}
    }

  // Initialized
  
  // The hidden mechanism to get things minimised
  // in case we are bound to a layer for another language.
  if (i_eventflag == ATRSHMLOGPOINTINTIMEP
       || i_eventflag == ATRSHMLOGPOINTINTIMEp
      )
    {
      if (i_starttime == 0)
	i_starttime = ATRSHMLOG_GET_TSC_CALL();

      i_endtime = i_starttime;
    }
  else
    {
      if (i_endtime == 0)
	i_endtime = ATRSHMLOG_GET_TSC_CALL();
    }

  /* We collect at max the length of the argv.
   * Having more data in the argv is simply ignored, the max is the buffer
   * len. No more 
   * So its possible to use arbitrary number of argv, but only 
   * the first of them go into the buffer till the length limit is hit.
   */
    
  int argvbufferlen = 0;
    
  if (i_argv)
    goto doargv;
    
 afterargv:
  ;


  atrshmlog_int32_t totallen;
  
  // Do the normal stuff now ...

  totallen = ATRSHMLOGCONTROLDATASIZE + argvbufferlen + copy_local;

  int strategy_count = 0;
  
  // We use some goto jumping here, so this is a first target
 testagain_dispatched:

  ;
    
  register tbuff_t* tbuff = g->atrshmlog_targetbuffer_arr[g->atrshmlog_targetbuffer_index];

  if (atomic_load_explicit(&(tbuff->dispatched), memory_order_acquire) != 0)
    {
      g->atrshmlog_targetbuffer_index++;

      if (g->atrshmlog_targetbuffer_index >= ATRSHMLOGTARGETBUFFERMAX)
	g->atrshmlog_targetbuffer_index = 0;


      ++strategy_count;

      if ((strategy_count % ATRSHMLOGTARGETBUFFERMAX) == 0)
	switch (g->strategy)
	  {
	  case atrshmlog_strategy_discard :
	    {
	      ATRSHMLOGSTAT(atrshmlog_counter_write2_discard);
	      
	    // we discard
	    return atrshmlog_error_write2_6;
	    }
	    
	  case atrshmlog_strategy_spin_loop:
	    // we spin loop
	    break;

	  case atrshmlog_strategy_wait:
	    {
	      ATRSHMLOGSTAT(atrshmlog_counter_write2_wait);

	      // we wait fix time
	      atrshmlog_nanosleep(atrshmlog_strategy_wait_wait_time);
	    }
	    break;

	  case atrshmlog_strategy_adaptive:
	    {
	      ATRSHMLOGSTAT(atrshmlog_counter_write2_adaptive);

	      atrshmlog_time_t t = g->atrshmlog_targetbuffer_arr[g->atrshmlog_targetbuffer_index]->transfer_to_shm_time ;

	      t /=  ATRSHMLOGTARGETBUFFERMAX;

#if 0
	      printf("adapotive %ld\n", (long)t);
#endif
	      
	      if (t > 999999999)
		t = 999999999;
	      
	      // adaptive : take the last transfer time and divide it
	      atrshmlog_nanosleep(t);
	    }
	    break;
	    
	  case atrshmlog_strategy_adaptive_fast:
	    {
	      ATRSHMLOGSTAT(atrshmlog_counter_write2_adaptive_fast);

	      atrshmlog_time_t t = g->atrshmlog_targetbuffer_arr[g->atrshmlog_targetbuffer_index]->transfer_to_shm_time ;

	      t /=  ATRSHMLOGTARGETBUFFERMAX * 2;

#if 0
	      printf("adapotive %ld\n", (long)t);
#endif
	      
	      if (t > 999999999)
		t = 999999999;
	      
	      // adaptive fast : take the last transfer time and divide it 2
	      atrshmlog_nanosleep(t);
	    }
	    break;

	  case atrshmlog_strategy_adaptive_very_fast:
	    {
	      ATRSHMLOGSTAT(atrshmlog_counter_write2_adaptive_very_fast);

	      // adaptive very fast : take the last transfer time and divide it 10
	      atrshmlog_time_t t = g->atrshmlog_targetbuffer_arr[g->atrshmlog_targetbuffer_index]->transfer_to_shm_time ;

	      t /=  ATRSHMLOGTARGETBUFFERMAX * 10;

#if 0
	      printf("adapotive %ld\n", (long)t);
#endif
	      
	      if (t > 999999999)
		t = 999999999;
	      
	      // adaptive fast : take the last transfer time and divide it 2
	      atrshmlog_nanosleep(t);
	    }
	    break;
	    
	  default:
	    // we spin loop
	    break;
	  }
      
      if (atrshmlog_logging_process_off_final)
	return atrshmlog_error_write2_7;

      atrshmlog_area_t * a_shm = ATRSHMLOG_GETAREA;

      /* Can be happen : end of logging anounced by user via flag in shm */
      if (atomic_load_explicit(&a_shm->ich_habe_fertig, memory_order_acquire) != 0) 
	return atrshmlog_error_write2_8;
      
      // There is no way to do the things follow for a dispatched buffer
      goto testagain_dispatched;
    }

  if (atrshmlog_thread_fence_2)
    atomic_thread_fence (memory_order_acquire);

  // special case for the argv thing
  if (totallen > tbuff->maxsize) {
    ATRSHMLOGSTAT(atrshmlog_counter_write2_abort7);
    
    return atrshmlog_error_write2_9; // Cannot work.
  }
  
  // Ok. We have an undispatched buffer 
  register size_t akindex = tbuff->size;

  // Is the size limit reached ? 
  if (akindex + totallen > tbuff->maxsize )
    {
      // Checkings: valid buffer 
      if (tbuff->safeguardfront != ATRSHMLOGSAFEGUARDVALUE) {
	ATRSHMLOGSTAT(atrshmlog_counter_write_safeguard);

	return atrshmlog_error_write2_10;
      }
      
      atrshmlog_area_t * a_shm = ATRSHMLOG_GETAREA;

      /* Bad thing. Safeguard invalid */
      if (a_shm->shmsafeguard != ATRSHMLOGSAFEGUARDVALUE) {
	ATRSHMLOGSTAT(atrshmlog_counter_write_safeguard_shm);

	return atrshmlog_error_write2_11;
      }
      
      /* Can be happen : end of logging anounced by user via flag in shm */
      if (atomic_load_explicit(&a_shm->ich_habe_fertig, memory_order_acquire) != 0) 
	return atrshmlog_error_write2_12;


      atrshmlog_dispatch_buffer(tbuff);

      // Switch the targetbuffer and try again
      g->atrshmlog_targetbuffer_index++;

      if (g->atrshmlog_targetbuffer_index >= ATRSHMLOGTARGETBUFFERMAX)
	g->atrshmlog_targetbuffer_index = 0;

      // End of full buffer handling
      goto testagain_dispatched;
    }

  // Ok. We have an undispatched and not full buffer.
    
  register char* p = tbuff->b + akindex;

  // Transfer the log data
  memcpy(p, &i_starttime, sizeof(atrshmlog_time_t));
  memcpy(p + sizeof(atrshmlog_time_t), &i_endtime, sizeof(atrshmlog_time_t));
  memcpy(p + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_time_t), &totallen, sizeof(atrshmlog_int32_t));
  memcpy(p + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_int32_t) , &i_eventnumber, sizeof(atrshmlog_int32_t));
  memcpy(p + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_int32_t) + sizeof(atrshmlog_int32_t), &i_userflag, sizeof(atrshmlog_int32_t));

  *(p + ( ATRSHMLOGCONTROLDATASIZE - 1 ) )= (char)i_eventflag;

  if (copy_local)
    {
      memcpy(p + ATRSHMLOGCONTROLDATASIZE, i_local, copy_local);
    }
    
  if (argvbufferlen)
    {
      memcpy(p + ATRSHMLOGCONTROLDATASIZE + copy_local, atrshmlog_argvbuffer, argvbufferlen);
    }


  akindex += totallen;

  tbuff->size = akindex;

  return atrshmlog_error_ok;
  
  /************************/  
 doargv:
  {
    int index = 0;
    
    char* dest = atrshmlog_argvbuffer;

    char* limit = dest + ATRSHMLOGARGVBUFFERLEN;
        
    if (i_argc_hint <= 0 )
      {
	for (const char *p = i_argv[0];
	     p;
	     p = i_argv[++index])
	  {
	    if (dest == limit)
	      {
		goto argvtransfered0;
	      }
            
	    *dest++ = (char)atrshmlog_delimiter;
        
	    int len  = strlen(p);
            
	    if (len > limit - dest)
	      {
		len = limit - dest;
	      }
            
	    memcpy(dest, p, len);
            
	    dest += len;
	    /* count_memcpy++; */
	  }
      argvtransfered0:
	argvbufferlen = dest - atrshmlog_argvbuffer;
      }
    else
      {
	for (const char *p = i_argv[0];
	     p && (index < i_argc_hint);
	     p = i_argv[++index])
	  {
	    if (dest == limit)
	      {
		goto argvtransfered1;
	      }
            
	    *dest++ = (char)atrshmlog_delimiter;
            
	    int len  = strlen(p);
            
	    if (len > limit - dest)
	      {
		len = limit - dest;
	      }
        
	    memcpy(dest, p, len);
        
	    dest += len;
	    /*count_memcpy++; */
	  }

      argvtransfered1:
	argvbufferlen = dest - atrshmlog_argvbuffer;
      }
  }
  goto afterargv;
}

/*******************************************************************/


/**
 * \n Main code:
 *
 * \brief The number of slave threads for the process is delivered.
 */ 
atrshmlog_ret_t atrshmlog_get_f_list_buffer_slave_count(void)
{
  return atrshmlog_f_list_buffer_slave_count;
}

/*******************************************************************/

/**
 * \n Main code:
 *
 * \brief We can set the count of slave threads.
 *
 * This has to be done before attach.
 * Be sure you understand the consequences.
 * If no slaves are running and you hit the buffer size
 * your buffer will be dispatched, but not cleared.
 * If you hit it twice your process is spinning. 
 * So this is only ok if you know that you have little logging
 * and dont want to have additional threads.
 * In case you need more set it to a save limit, but consider
 * the maximum count of shm buffers and the number of threads of the
 * reader process too.
 * If you only raise this you shift the problem to the shm
 * and in the latter case to the reader.
 * The reader has not only to move the mem, but
 * also to write it to disk - so this can be the real limit.
 * If you see the buffer number in the log at the shm buffer count 
 * you need to set the reader threads number higher.
 * After this you can raise the slaves count with this before 
 * you attach ...
 * 
 * There is an env for attach to set this.
 */
atrshmlog_ret_t atrshmlog_set_f_list_buffer_slave_count(atrshmlog_int32_t i_count)
{
  ATRSHMLOGSTAT(atrshmlog_counter_set_slave_count);
  
  int old = atrshmlog_f_list_buffer_slave_count;
  
  if (i_count >= 0 && atrshmlog_attach_once == 0)
    atrshmlog_f_list_buffer_slave_count = i_count;
  
  return old;
}

/*******************************************************************/

/**
 * \n Main code:
 *
 * \brief The clock id we use to get time.
 */
atrshmlog_ret_t atrshmlog_get_clock_id(void)
{
  return atrshmlog_clock_id;
}

/*******************************************************************/

/**
 * \n Main code:
 *
 * \brief The clock id can be set with this.
 */
atrshmlog_ret_t atrshmlog_set_clock_id(atrshmlog_int32_t i_id)
{
  ATRSHMLOGSTAT(atrshmlog_counter_set_clock_id);

  int old = atrshmlog_clock_id;
  
  if (i_id >= ATRSHMLOG_CLOCK_ID_MIN && i_id <= ATRSHMLOG_CLOCK_ID_MAX)
    { 
      *(atrshmlog_int32_t*)&atrshmlog_clock_id = i_id;
    }

  return old;
}

/*******************************************************************/

/**
 * \n Main code:
 *
 * \brief Switch off the slaves.
 *
 * This is no kill op, so you have to wait for threads that are running
 * to end their work.
 *
 * This can cost time if the number of threads is high,
 * the waiting number of buffers is high and reader is not 
 * coming up. so use it as a "i do it in the near future"
 * wish. 
 * 
 * It is NOT working directly after the call is made
 */
void atrshmlog_set_f_list_buffer_slave_run_off(void)
{
  ATRSHMLOGSTAT(atrshmlog_counter_slave_off);

  atrshmlog_f_list_buffer_slave_run = 0;
}

/*******************************************************************/
  
/**
 * \n Main code:
 *
 * \brief We have a limit for the number of event locks
 */
atrshmlog_ret_t atrshmlog_get_event_locks_max(void)
{
  return atrshmlog_event_locks_max;
}

/*******************************************************************/

/**
 * \n Main code:
 *
 * \brief We can set the limit of event locks.
 *
 * This should be done at the beginning, best before
 * attach.
 *
 * You should have at least set the size to the highest 
 * event number you use in logging calls for gettime and write.
 *
 * If you dont hit the build in buffer size nothing is done.
 * If you set a bigger size it cannot be reduced after.
 */
atrshmlog_ret_t atrshmlog_set_event_locks_max(atrshmlog_int32_t i_max)
{
  ATRSHMLOGSTAT(atrshmlog_counter_set_event_locks);

  int old = atrshmlog_event_locks_max;
  
  if (i_max >  ATRSHMLOGEVENTCOUNT && i_max > atrshmlog_event_locks_max && atrshmlog_attach_once == 0)
    {
      int oldmax = atrshmlog_event_locks_max;
      
      atrshmlog_event_locks_max = i_max;

      atrshmlog_event_t * p =  (atrshmlog_event_t*)malloc(atrshmlog_event_locks_max + 1);

      memset(p, 0, atrshmlog_event_locks_max + 1);
      
      memcpy(p, (void*)atrshmlog_event_locks, oldmax);
      
      atrshmlog_event_locks = p;
    }
  
  return old;
}

/*******************************************************************/

/**
 * \n Main code:
 *
 * \brief This is the versions high limit for a log buffer size.
 */
atrshmlog_ret_t atrshmlog_get_logbuffermaxsize(void)
{
  return ATRSHMLOGBUFFER_INFOSIZE;
}

/*******************************************************************/

/**
 * \n Main code:
 *
 * \brief This is the actual limit, can be smaller than max.
 *
 * If this is set high
 * its good to have memory for loging,
 * but if the logging fills buffers the memory of the
 * buffers become a problem to cache write.
 *
 * So in case of many threads we reduce the size to get
 * the memory still into the caches...
 *
 * At least we can try this for small things.
 */
atrshmlog_ret_t atrshmlog_get_buffer_infosize(void)
{
  return atrshmlog_buffer_infosize;
}

/*******************************************************************/

/**
 * \n Main code:
 *
 * \brief We can reduce the buffer size here.
 *
 * This works from that moment on for all
 * buffers new alloced, so be sure to do it 
 * at the very beginning.
 *
 * Its bad to have buffers to be too small,
 * so reduce also the loging payload ....
 *
 * You cannot increase .
 * You cannot change the static allocated buffers sizes.
 */
atrshmlog_ret_t atrshmlog_set_buffer_infosize(atrshmlog_int32_t i_size)
{
  ATRSHMLOGSTAT(atrshmlog_counter_set_infosize);

  int old = atrshmlog_buffer_infosize;

  if (i_size > 0 && i_size <= old)
    atrshmlog_buffer_infosize = i_size;

  return old;
}

/*******************************************************************/

/**
 * \n Main code:
 *
 * \brief Get the major version from shm.
 *
 * This has to be the same as the programs major or we should stop at once
 */
atrshmlog_ret_t atrshmlog_get_area_version(volatile const void* i_area)
{
  if (i_area && atrshmlog_attach_once != 0)
    return ((volatile const atrshmlog_area_t *)i_area)->shmversion;

  return atrshmlog_error_area_version_1;
}

/*******************************************************************/

/**
 * \n Main code:
 *
 * \brief Get the shm log buffer count.
 */
atrshmlog_ret_t atrshmlog_get_area_count(volatile const void* i_area)
{
  if (i_area && atrshmlog_attach_once != 0)
    return ((volatile const atrshmlog_area_t *)i_area)->shmcount;

  return atrshmlog_error_area_count_1;
}

/*******************************************************************/

/**
 * \n Main code:
 *
 * \brief Get the ich habe fertig flag
 */
atrshmlog_ret_t atrshmlog_get_area_ich_habe_fertig(volatile const void* i_area)
{
  if (i_area && atrshmlog_attach_once != 0)
    return atomic_load(&((volatile const atrshmlog_area_t *)i_area)->ich_habe_fertig);
  
  return 0;
}

/*******************************************************************/

/**
 * \n Main code:
 *
 * \brief Set the ich habe fertig flag .
 *
 * This is 0 for go on, non 0 for stop.
 */
atrshmlog_ret_t atrshmlog_set_area_ich_habe_fertig(volatile const void* i_area, atrshmlog_int32_t i_flag)
{
  if (!(i_area && atrshmlog_attach_once != 0))
    return atrshmlog_error_area_ich_habe_fertig_1;
  
  int ret = atomic_load(&((volatile const atrshmlog_area_t *)i_area)->ich_habe_fertig);

  // we cast const away. const is used to hinder the user of the module to
  // accidently write. but this function really has to write.
  // so we use a bad thing here, cast const away...
  atomic_store(&((volatile atrshmlog_area_t *)i_area)->ich_habe_fertig, i_flag);

  return ret;
}


/*******************************************************************/

static atrshmlog_event_t atrshmlog_set_event(const atrshmlog_int32_t i_index,
					     const int i_ev)
{
  ATRSHMLOGSTAT(atrshmlog_counter_set_event);
  
  if (i_index < 0 || i_index >= atrshmlog_event_locks_max)
    return 0;

  int old = atrshmlog_event_locks[i_index];

  ((atrshmlog_event_t*)atrshmlog_event_locks)[i_index] = i_ev;

  return old;
}


/********************************************************************/

/**
 * \n Main code:
 *
 * \brief Set the event flag to non zero. 
 *
 * So from this on we use the event.
 * This is to be used by the program after it was set to zero before.
 * The default is to have all events on at start.
 */
atrshmlog_event_t atrshmlog_set_event_on(atrshmlog_int32_t i_index)
{
  return atrshmlog_set_event(i_index, 1);
}

/*******************************************************************/

/**
 * \n Main code:
 *
 * \brief Set the event flag to zero. 
 *
 * So the event is no longer used.
 * This is to be used by the program if you want to switch off an event
 */
atrshmlog_event_t atrshmlog_set_event_off(atrshmlog_int32_t i_index)
{
  return atrshmlog_set_event(i_index, 0);
}

/*******************************************************************/

/**
 * \n Main code:
 *
 * \brief Get the event flag state.
 */
atrshmlog_event_t atrshmlog_get_event(atrshmlog_int32_t i_index)
{
  if (i_index < 0 || i_index >= atrshmlog_event_locks_max)
    return atrshmlog_error_get_event_1;
  
  return atrshmlog_event_locks[i_index];
}

/*******************************************************************/

/**
 * \n Main code:
 *
 * \brief Set the logging flag for the process to on
 */
atrshmlog_ret_t atrshmlog_set_logging_process_on(void)
{
  ATRSHMLOGSTAT(atrshmlog_counter_logging_process_on);
  
  int old  = atrshmlog_logging_process;
  
  *((atrshmlog_int32_t*)&atrshmlog_logging_process) = 1;

  return old;
}

/*******************************************************************/

/** 
 * \n Main code:
 *
 * \brief Set the logging flag for the process to off
 */
atrshmlog_ret_t atrshmlog_set_logging_process_off(void)
{
  ATRSHMLOGSTAT(atrshmlog_counter_logging_process_off);
  
  int old  = atrshmlog_logging_process;
  
  *((atrshmlog_int32_t*)&atrshmlog_logging_process) = 0;

  return old;
}
/*******************************************************************/

/** 
 * \n Main code:
 *
 * \brief Set the logging flag for the process final to on
 */
atrshmlog_ret_t atrshmlog_set_logging_process_off_final(void)
{
  ATRSHMLOGSTAT(atrshmlog_counter_logging_process_off_final);
  
  int old  = atrshmlog_logging_process_off_final;
  
  atrshmlog_logging_process_off_final = 1;

  return old;
}

/*******************************************************************/

/** 
 * \n Main code:
 *
 * \brief Get the process logging status.
 *
 * Get also the ich habe fertig.
 *
 * Check also for attached shm buffer.
 */
atrshmlog_ret_t atrshmlog_get_logging(void)
{
  if (!ATRSHMLOG_LOGGING)
    return atrshmlog_error_get_logging_1;

  if(atrshmlog_thread_fence_10)
    atomic_thread_fence(memory_order_acquire); // ok

  atrshmlog_area_t * a = ATRSHMLOG_GETAREA;

  if (atomic_load(&a->ich_habe_fertig) != 0)
    return atrshmlog_error_get_logging_2;
  
  if (atrshmlog_logging_process_off_final != 0)
    return atrshmlog_error_get_logging_3;

  if (atrshmlog_logging_process == 0)
    return atrshmlog_error_get_logging_4;

  return atrshmlog_error_ok;
}

/******************************************************************/

/** 
 * \n Main code:
 *
 * \brief Get the start of the shm area with the shm buffers
 */
volatile const void* atrshmlog_area(void)
{
  return  atrshmlog_base_ptr;
}


/******************************************************************/

/** 
 * \n Main code:
 *
 * \brief Set the prefix for all env variable lookups we do in our
 * attach and initialize part.
 *
 * This has to be done before we use any env stuff. 
 * 
 * so you have to do this before you attach.
 */
void atrshmlog_set_envprefix (const char*i_prefix)
{
  ATRSHMLOGSTAT(atrshmlog_counter_set_envprefix);

  /* set it if all is ok and if not already set */
  if (i_prefix && *i_prefix != '\0' && *atrshmlog_prefix_name_buffer == '\0')
    {
      const int limit = ( ATRSHMLOG_PREFIX_SIZE / 2 );
      
      int len = strlen(i_prefix);

      if (len > limit )
	len = limit;
      
      memcpy(atrshmlog_prefix_name_buffer, i_prefix, len);
      atrshmlog_prefix_name_buffer[len] = 0;
    }
}

/*******************************************************************/

/**
 * \n Main code:
 *
 * \brief Get the name of the environment variable.
 * 
 * We can have - in future - multiple buffers to log to.
 *
 * So to make life simple we have a default buffer value,
 * this is ATRSHMLOG_ID in the environment.
 * 
 * If you need more buffers and in concurrency you can do that 
 * by overwriting ATRSHMLOG_ID in it...
 * 
 * But sometimes its not that simple, 
 * then you can use the prefix part via ATRSHMLOG.
 *
 * This means you dont have to know the value, its already
 * set somewhere else and you only switch the prefix to
 * something diffrent. 
 *
 * You can use 128 chars as the new prefix
 * that replace ATRSHMLOG in the lookups....
 * 
 * Every program starts its logging with either 
 * a getenv or a attach (which itself starts with a getenv.
 *
 * So we use inside the getenv the following function
 * that constructs the name of the prefix dynamically
 * for the first time it is run. 
 *
 * And after this it is never
 * changing the given value any more...
 *
 * ATRSHMLOG=hugo 
 *
 * is a perfectly working option....
 *
 * This changes the prefix for all of the getenv and file lookups 
 * yes, the files too ... 
 *
 * So you can set it somehow and then use it even in the case the thing is
 * a logging shell (to be onest, i could not achive this, i leave it as
 * an option for someone better in configuring the system ) 
 */
const char* atrshmlog_get_envprefix(void)
{
  static char buff[ATRSHMLOG_PREFIX_SIZE];

  static int once = 0;

  if (once == 0)
    {
      ++once;

      if (once == 1)
        {
	  /* alredy set via program ? */
	  if (*atrshmlog_prefix_name_buffer == '\0')
	    strcpy(atrshmlog_prefix_name_buffer, ATRSHMLOG_ENV_PREFIX);
      
	  if (getenv(ATRSHMLOG_ENV_PREFIX))
	    {
	      int len = strlen(getenv(ATRSHMLOG_ENV_PREFIX));

	      if(len > 0)
		{
		  memcpy(atrshmlog_prefix_name_buffer, getenv(ATRSHMLOG_ENV_PREFIX), (len < ( ATRSHMLOG_PREFIX_SIZE / 2)) ? len : ( ATRSHMLOG_PREFIX_SIZE / 2));
		  atrshmlog_prefix_name_buffer[(len < ( ATRSHMLOG_PREFIX_SIZE / 2)) ? len : ( ATRSHMLOG_PREFIX_SIZE / 2) ] = '\0';
		}
	    }

	  strcpy(buff, atrshmlog_prefix_name_buffer);
	}
    }
  
  return buff;
}

/*******************************************************************/

/**
 * \n Main code:
 *
 * \brief Get the environment value for the shm id
 */
const char* atrshmlog_get_envshmid(void)
{
  return atrshmlog_get_env(ATRSHMLOGENVSUFFIX);
}


const char* atrshmlog_get_envidsuffix(void)
{
  return ATRSHMLOGENVSUFFIX;
}

/*******************************************************************/

/**
 * \n Main code:
 *
 * \brief Get the environment value for the env var with the suffix.
 *
 * The prefix is set from attach, from the PREFIX Environment Var or
 * with set envprefix.
 */
const char* atrshmlog_get_env(const char* i_suffix)
{
  char buff[ATRSHMLOG_PREFIX_SIZE] ;

  if (strlen(i_suffix) < (ATRSHMLOG_PREFIX_SIZE / 2 - 2) && strlen(i_suffix) > 0)
    {
      strcat(strcpy(buff, atrshmlog_get_envprefix()), i_suffix);
      return getenv(buff);
    }

  return NULL;
}

/*******************************************************************/

/**
 * \n Main code:
 *
 * \brief The major version of the shmlog system
 */
atrshmlog_ret_t atrshmlog_get_version(void)
{
  return ATRSHMLOGVERSION;
}

/*******************************************************************/

/**
 * \n Main code:
 *
 * \brief The minor version
 */
atrshmlog_ret_t atrshmlog_get_minor_version(void)
{
  return ATRSHMLOGMINORVERSION;
}

/*******************************************************************/

/**
 * \n Main code:
 *
 * \brief The patch version
 */
atrshmlog_ret_t atrshmlog_get_patch_version(void)
{
  return ATRSHMLOGPATCHVERSION;
}

/*******************************************************************/

/**
 * \n Main code:
 *
 * \brief The shmid according to attach and the used env variable
 */
int atrshmlog_get_shmid(void)
{
  return atrshmlog_id;
}

/*******************************************************************/

/** 
 * \n Main code:
 *
 * \brief Get the wait for slaves flag
 */
atrshmlog_ret_t atrshmlog_get_wait_for_slaves(void)
{
  return atrshmlog_wait_for_slaves;
}

/*******************************************************************/

/** 
 * \n Main code:
 *
 * \brief Set the flag for wait to on.
 *
 * So we wait in the cleanup till all slaves are done.
 *
 * This is not on by default.
 *
 * So you can switch this on
 * which means you start processing in cleanup after all
 * slaves have stopped.
 *
 * Of course there is a problem.
 * If you ever run in a hang of the slave you wait in the exit.
 */
atrshmlog_ret_t atrshmlog_set_wait_for_slaves_on(void)
{
  ATRSHMLOGSTAT(atrshmlog_counter_set_wait_slaves_on);

   int old  = atrshmlog_wait_for_slaves;

  atrshmlog_wait_for_slaves = 1;

  return old;
}

/*******************************************************************/

/** 
 * \n Main code:
 *
 * \brief Set the wait for slaves flag to off
 */
atrshmlog_ret_t atrshmlog_set_wait_for_slaves_off(void)
{
  ATRSHMLOGSTAT(atrshmlog_counter_set_wait_slaves_off);

  int old  = atrshmlog_wait_for_slaves;

  atrshmlog_wait_for_slaves = 0;

  return old;
}

/*******************************************************************/

/** 
 * \n Main code:
 *
 * \brief Get the wait time a slaves is in sleep in nanos
 * when the list was empty at the test.
 */
atrshmlog_ret_t atrshmlog_get_f_list_buffer_slave_wait(void)
{
  return atrshmlog_f_list_buffer_slave_wait;
}

/*******************************************************************/

/** 
 * \n Main code:
 *
 * \brief Set the sleep time for a slave in nanos after a 
 * test of the f list was getting empty
 */
atrshmlog_ret_t atrshmlog_set_f_list_buffer_slave_wait(atrshmlog_int32_t i_wait_nanos)
{
  ATRSHMLOGSTAT(atrshmlog_counter_set_slave_wait);

  int old  = atrshmlog_f_list_buffer_slave_wait;

  if (i_wait_nanos >= ATRSHMLOG_INIT_BUFFER_SLAVE_WAIT_MIN
      && i_wait_nanos <= ATRSHMLOG_INIT_BUFFER_SLAVE_WAIT_MAX)
    atrshmlog_f_list_buffer_slave_wait = i_wait_nanos;

  return old;
}

/*******************************************************************/

/**
 * \n Main code:
 *
 * \brief The number of buffers from static and dyn memory
 */
atrshmlog_ret_t atrshmlog_get_acquire_count(void)
{
  return atrshmlog_acquire_count;
}

/*******************************************************************/

/** 
 * \n Main code:
 *
 * \brief The last id given to a buffer.
 *
 * This should be the same as the acquire count.
 * 
 */
atrshmlog_ret_t atrshmlog_get_buffer_id(void)
{
  int ret =  atrshmlog_buffer_id;

  return ret;
}

/*******************************************************************/

/** 
 * \n Main code:
 *
 * \brief The number of buffers that are alloced in one 
 * dyn memory allocation to reduce overhead.
 */
atrshmlog_ret_t atrshmlog_get_prealloc_buffer_count(void)
{
  return atrshmlog_prealloc_buffer_count;
}

/*******************************************************************/

/** 
 * \n Main code:
 *
 * \brief Set the number of buffers for a dyn alloc To this.

 * Speed up by only doing one low level malloc.
 * But dont overdue this .
 * Its better to recomile and use static allocaed buffers.
 * And dont try to handle a leak of buffers because a thread does not
 * stop the logging by raising the count.
 */
atrshmlog_ret_t atrshmlog_set_prealloc_buffer_count(atrshmlog_int32_t i_count)
{
  ATRSHMLOGSTAT(atrshmlog_counter_set_prealloc_count);

  int old = atrshmlog_prealloc_buffer_count;

  if (i_count > 1 && i_count <= ATRSHMLOG_MAX_PREALLOC_COUNT)
    atrshmlog_prealloc_buffer_count = i_count;

  return old;
}

/*******************************************************************/

/** 
 * \n Main code:
 *
 * \brief Get the inittime for the process
 */
atrshmlog_internal_time_t atrshmlog_get_inittime(void)
{
  return atrshmlog_inittime;
}

/*******************************************************************/

/** 
 * \n Main code:
 *
 * \brief Get the click before inittime
 */
atrshmlog_time_t atrshmlog_get_inittimetsc_before(void)
{
  return atrshmlog_inittimetsc_before;
}

/*******************************************************************/

/** 
 * \n Main code:
 *
 * \brief Get the click after inittime
 */
atrshmlog_time_t atrshmlog_get_inittimetsc_after(void)
{
  return atrshmlog_inittimetsc_after;
}

/*******************************************************************/

/** 
 * \n Main code:
 *
 * \brief Set the statistic switch 
 */

atrshmlog_ret_t atrshmlog_set_statistics_on(const int i_switch)
{
  int old = atrshmlog_statistics_on;

  atrshmlog_statistics_on = i_switch ? 1 : 0;

  return old;
}

/*******************************************************************/

static atrshmlog_ret_t atrshmlog_set_thread_fence(int *v,
						  const atrshmlog_int32_t i_switch)
{
  ATRSHMLOGSTAT(atrshmlog_counter_set_thread_fence);

  int old = *v;

  *v = i_switch ? 1 : 0;

  return old;
}

/**
 * \brief We switch a fence on or off
 *
 * \return
 * The old flag for fence.
 */
atrshmlog_ret_t atrshmlog_set_thread_fence_1(atrshmlog_int32_t i_switch)
{
  return atrshmlog_set_thread_fence(&atrshmlog_thread_fence_1, i_switch);
}

/**
 * \brief We switch a fence on or off
 *
 * \return
 * The old flag for fence.
 */
atrshmlog_ret_t atrshmlog_set_thread_fence_2(atrshmlog_int32_t i_switch)
{
  return atrshmlog_set_thread_fence(&atrshmlog_thread_fence_2, i_switch);
}

/**
 * \brief We switch a fence on or off
 *
 * \return
 * The old flag for fence.
 */
atrshmlog_ret_t atrshmlog_set_thread_fence_3(atrshmlog_int32_t i_switch)
{
  return atrshmlog_set_thread_fence(&atrshmlog_thread_fence_3, i_switch);
}

/**
 * \brief We switch a fence on or off
 *
 * \return
 * The old flag for fence.
 */
atrshmlog_ret_t atrshmlog_set_thread_fence_4(atrshmlog_int32_t i_switch)
{
  return atrshmlog_set_thread_fence(&atrshmlog_thread_fence_4, i_switch);
}

/**
 * \brief We switch a fence on or off
 *
 * \return
 * The old flag for fence.
 */
atrshmlog_ret_t atrshmlog_set_thread_fence_5(atrshmlog_int32_t i_switch)
{
  return atrshmlog_set_thread_fence(&atrshmlog_thread_fence_5, i_switch);
}

/**
 * \brief We switch a fence on or off
 *
 * \return
 * The old flag for fence.
 */
atrshmlog_ret_t atrshmlog_set_thread_fence_6(atrshmlog_int32_t i_switch)
{
  return atrshmlog_set_thread_fence(&atrshmlog_thread_fence_6, i_switch);
}

/**
 * \brief We switch a fence on or off
 *
 * \return
 * The old flag for fence.
 */
atrshmlog_ret_t atrshmlog_set_thread_fence_7(atrshmlog_int32_t i_switch)
{
  return atrshmlog_set_thread_fence(&atrshmlog_thread_fence_7, i_switch);
}

/**
 * \brief We switch a fence on or off
 *
 * \return
 * The old flag for fence.
 */
atrshmlog_ret_t atrshmlog_set_thread_fence_8(atrshmlog_int32_t i_switch)
{
  return atrshmlog_set_thread_fence(&atrshmlog_thread_fence_8, i_switch);
}

/**
 * \brief We switch a fence on or off
 *
 * \return
 * The old flag for fence.
 */
atrshmlog_ret_t atrshmlog_set_thread_fence_9(atrshmlog_int32_t i_switch)
{
  return atrshmlog_set_thread_fence(&atrshmlog_thread_fence_9, i_switch);
}

/**
 * \brief We switch a fence on or off
 *
 * \return
 * The old flag for fence.
 */
atrshmlog_ret_t atrshmlog_set_thread_fence_10(atrshmlog_int32_t i_switch)
{
  return atrshmlog_set_thread_fence(&atrshmlog_thread_fence_10, i_switch);
}

/**
 * \brief We switch a fence on or off
 *
 * \return
 * The old flag for fence.
 */
atrshmlog_ret_t atrshmlog_set_thread_fence_11(atrshmlog_int32_t i_switch)
{
  return atrshmlog_set_thread_fence(&atrshmlog_thread_fence_11, i_switch);
}

/**
 * \brief We switch a fence on or off
 *
 * \return
 * The old flag for fence.
 */
atrshmlog_ret_t atrshmlog_set_thread_fence_12(atrshmlog_int32_t i_switch)
{
  return atrshmlog_set_thread_fence(&atrshmlog_thread_fence_12, i_switch);
}


/************************************************************/

/**
 * \n Main code:
 *
 * \brief We are the atexit function to clean up 
 * what was not cleaned up so far.
 *
 * We assume no more running
 * slaves - but we switch at least them off...
 *
 * Its complicated to make this right in an mt env.
 *
 * So this is a simplified and _wrong_ version.
 *
 * It does no locking and so there is a small chance to do it
 * parallel to a still running slave.
 *
 * Worse, the free can torpedoise a slave.
 *
 * So be sure to finish only if you have no problems with the 
 * slaves.
 */
void atrshmlog_exit_cleanup (void)
{
  ATRSHMLOGSTAT(  atrshmlog_counter_exit_cleanup);
  
  if (atrshmlog_base_ptr == 0)
    return;

  g_tl_t* g = &g_tl;

  // Do we have to initialize ?
  if (g->atrshmlog_idnotok)
    atrshmlog_init_thread_local (g);
  
  // No more logging from threads
  atrshmlog_set_logging_process_off();

  
  // No more buffer freeing from slaves
  atrshmlog_set_f_list_buffer_slave_run_off();

  if (atrshmlog_wait_for_slaves)
    while (atomic_load(&atrshmlog_f_list_active_slaves) > 0)
      atrshmlog_nanosleep(100000);

  /* Ok. This is first strange.
   * But when i used it with a ksh hack i found a problem
   * with multile buffers written ...
   * And the reason is that a shell makes use of fork
   * and exec and also of the nice exit ...
   * So we are in the land of multiple exit calls to buffers
   * in diffrent processes.
   * This is why we only write our own buffers out.
   * Meaning : Our pid must match the pid of the buffer.
   * If we fork and exit the bufer has a diffrent pid...
   * So we can use this later as a trick o get info from the 
   * parent, but for now i restrict the write to buffers
   * with the same pid.
   * As a fact the pid is set when we alloc a buffer for a real 
   * thread to the init value from thread locals. 
   * So it is
   * done in the running process for vital threads.
   * This means we can ignore the rest for now.
   */
  atrshmlog_pid_t mypid = getpid();
  
  // We are serious. We cut off the list
  tbuff_t* tp = (tbuff_t*)atomic_exchange(&atrshmlog_tps, 0);
  
  while (tp)
    {
      int dispatched = atomic_load(&tp->dispatched);

      if (atrshmlog_thread_fence_10)
	atomic_thread_fence(memory_order_acquire); 
      
      tbuff_t* tpnext = tp->next_cleanup;

      // We take every buffer
      // Even the dispatched
      // This assumes the slaves are no longer working
      // and they have done their job
      // If you end a program before this is true be sure
      // to switch the slaves off, wait at least a little bit to be
      // sure the threads have been stopped,
      // evtl check for still running threads ...
      // and then after seeing them go down do the thing here
      
      size_t  aksize = tp->size;
      
      // We skip broken buffers
      if (tp->safeguardfront != ATRSHMLOGSAFEGUARDVALUE)
	goto cleanit;

      int dodispatch = (tp->pid == mypid)
	&& (aksize > 0);

      if (dodispatch)
	atrshmlog_transfer_mem_to_shm(tp, g);

      tp->size =  0;

    cleanit:
      ;

      tp = tpnext;
    }

  // No more waiting in loops
  // this has to be AFTER the last use of mem to shm ...
  atrshmlog_set_logging_process_off_final();
}

/*******************************************************************/

/**
 * \n Main code:
 *
 * \brief We are a slave thread function.
 *
 * We wait for a time slice, then we check the f list.
 * 
 * If its empty we wait again.
 *
 * If its not empty we try to grap a buffer.
 *
 * We check this and if we got one we start the transfer.
 *
 * After the transfer we  check the dispose flag and give the 
 * buffer free if it is set.
 *
 * Then we start to wait again.
 */
ATRSHMLOG_thread_ret_t atrshmlog_f_list_buffer_slave_proc(void* i_arg)
{
  // We have to initialize the thread locals

  union u { ATRSHMLOG_thread_ret_t p; int64_t ui; };

  union u conv;
  
  g_tl_t* g = &g_tl;
  
  if (g->atrshmlog_idnotok)
    {
      int tlret = atrshmlog_init_thread_local (g);
      
      /* From now on we are the not ok flag */
      if (tlret)
	{
	  conv.ui = tlret;
	  
	  return conv.p;
	}
    }
  
  atrshmlog_f_list_active_slaves++;
  
  while (atrshmlog_f_list_buffer_slave_run)
    {
      // We use f list 
      tbuff_t* tbuff = (tbuff_t*)atomic_load_explicit(&atrshmlog_tpf, memory_order_relaxed);

      if (tbuff == 0)
	{
	  atrshmlog_nanosleep(atrshmlog_f_list_buffer_slave_wait);

	  if (atrshmlog_logging_process_off_final)
	    goto leave;
	  
	  continue;
	}

      // Pop from list
      while (tbuff &&
	     !atomic_compare_exchange_weak_explicit(&atrshmlog_tpf,
						    &tbuff,
						    tbuff->next_full,
						    memory_order_relaxed,
						    memory_order_relaxed))
	;
	  
      if (tbuff)
	{
	  volatile int f = atomic_load_explicit(&tbuff->dispatched, memory_order_acquire);
	  // We get the buffer content synced

	  if (atrshmlog_thread_fence_6)
	    atomic_thread_fence (memory_order_acquire);
	      
	  size_t aksize = tbuff->size;

	  if (aksize > 0)
	    atrshmlog_transfer_mem_to_shm(tbuff, g);

	  tbuff->size = 0;

	  if (atrshmlog_thread_fence_7)
	     atomic_thread_fence (memory_order_release);

          atomic_store_explicit(&tbuff->dispatched , 0, memory_order_release);
	  
	  if (tbuff->dispose == 1)
	    {
	      atrshmlog_free(tbuff); 
	    }
	}
    }

 leave:

  atrshmlog_f_list_active_slaves--;
  
  return NULL; 
}

/*******************************************************************/

/** 
 * \n Main code:
 *
 * \brief We deliver the g_counter data 
 * 
 * We set the now time tsc in 0 low part and 1 high part.
 */
void atrshmlog_get_g_counters(atrshmlog_int32_t* o_target)
{
  
  atrshmlog_time_t n =  ATRSHMLOG_GET_TSC_CALL();
  
  g_counter[0] = n & 0xffffffff;
  g_counter[1] = (n >> 32) & 0xffffffff;
  
  for (int i = atrshmlog_counter_start; i <= atrshmlog_counter_end; i++)
    {
      atrshmlog_int32_t value = atomic_load(&g_counter[i]);

      o_target[i] = value;
    }
}

/*******************************************************************/

/** 
 * \n Main code:
 *
 * \brief We deliver the g_counter data max index
 */
atrshmlog_int32_t atrshmlog_get_g_counters_max_index(void)
{
  return atrshmlog_counter_end;
}

/*******************************************************************/

/**
 * \n Main code:
 *
 * \brief  We get the real time 
 */
atrshmlog_internal_time_t atrshmlog_get_realtime(void)
{
  atrshmlog_internal_time_t result;
  
#if ATRSHMLOG_USE_CLOCK_GETTIME == 1
  struct timespec hugotime ;
      
  clock_gettime(CLOCK_REALTIME, &hugotime);

  result.tv_sec = hugotime.tv_sec;
  result.tv_nsec = hugotime.tv_nsec;
#endif

  return result;
}

/*******************************************************************/
#if ATRSHMLOG_LEVEL > 0

/** 
 * \n Main code:
 *
 * \brief We create the shm .
 *
 * On error we deliver a -x, else a valid positive shmid.
 *
 * This uses the key from external source, normally a parameter
 * to the program for a number.
 *
 * Key is a 32 bit value.
 */
int atrshmlog_create(const key_t i_key,
		     const int i_count)
{
  ATRSHMLOGSTAT(atrshmlog_counter_create);
  
  if (i_key == 0 || i_key  == IPC_PRIVATE) {
    ATRSHMLOGSTAT(  atrshmlog_counter_create_abort1);
    
    return atrshmlog_error_create_1;
  }
  
  /* At least ATRSHMLOGBUFFER_MINCOUNT .. */
  if (i_count < ATRSHMLOGBUFFER_MINCOUNT) {
    ATRSHMLOGSTAT(  atrshmlog_counter_create_abort2);
    
    return atrshmlog_error_create_2;
  }
  
  /* Max ATRSHMLOGBUFFER_MAXCOUNT buffers */
  if (i_count > ATRSHMLOGBUFFER_MAXCOUNT) {
    ATRSHMLOGSTAT(  atrshmlog_counter_create_abort3);
    
    return atrshmlog_error_create_3;
  }
  
  /* We calc the buffer size.
   * Its a fixed part for our infos and a variabel count of
   * buffers.
   * Some systems dont have much memory for a shm buffer, 
   * some have...
   * So simply try to check for the size that is possible with the
   * double number check ...
   */

  const int save = 256 ;
  
  size_t wantedsize = sizeof (atrshmlog_area_t) + (i_count - ATRSHMLOGBUFFER_MINCOUNT) * sizeof( atrshmlog_buffer_t) + (i_count * ATRSHMLOGBUFFER_INFOSIZE + save);

  int shmflg = IPC_CREAT | IPC_EXCL;

  shmflg |= ATRSHMLOG_ACCESS; 
    
  int result_shmget = shmget(i_key, wantedsize, shmflg);

  if (result_shmget == -1)
    {
      ATRSHMLOGSTAT(  atrshmlog_counter_create_abort4);
    
      return atrshmlog_error_create_4;
    }
  
  return result_shmget;
}


/*******************************************************************/

/** 
 * \n Main code:
 *
 * \brief We destroy the shm 
 */
atrshmlog_ret_t atrshmlog_delete(const int i_shmid)
{
  ATRSHMLOGSTAT(atrshmlog_counter_delete);

  int shmctl_result = shmctl(i_shmid, IPC_RMID, (struct shmid_ds *)0);

  return shmctl_result;
}


/*******************************************************************/

/** 
 * \n Main code:
 *
 * \brief We have to clean up the mutexes after stopping 
 * in the pthread version.
 *
 * This is an optional step in shuting down the log.
 * It is itself not mt safe.
 * So use a simple program for that
 * see the atrshmlogfinish.c
 */
void atrshmlog_cleanup_locks(void)
{
  ATRSHMLOGSTAT(atrshmlog_counter_cleanup_locks);

  if (! ATRSHMLOG_LOGGING )
    return;

  /* Next. We check for initialized */
  atrshmlog_area_t *a = ATRSHMLOG_GETAREA;

  if (a->shmid != atrshmlog_get_shmid())
    return;

  if (a->shmsafeguard != ATRSHMLOGSAFEGUARDVALUE)
    return;

  if (a->shmversion != ATRSHMLOGVERSION)
    return;
  
  atrshmlog_buffer_t* b = &(a->logbuffers[0]);

  /* From now on the buffer is useless. So we switch to not logging */
  atomic_store(&a->ich_habe_fertig,  1);

  sleep(5); /* We give 5 seconds time for the reader to finish */
  
  for (int i = 0; i < a->shmcount; i++)
    {
      atomic_store(&b->state, atrshmlog_illegal); /* no leagal state, no op any more */

      b++;
    }

  b = &(a->logbuffers[0]);
  
  for (int i = 0; i < a->shmcount; i++)
    {
      b->safeguard = 0L;
      
      b++;
    }

  a->shmid = 0;

  if (atrshmlog_thread_fence_11)
    atomic_thread_fence(memory_order_release); 
  
  a->shmsafeguard = 0L;
}


/*******************************************************************/

/** 
 * \n Main code:
 *
 * \brief helper: we initialize one buffer
 *
 * \param i_shm
 * The adress of the shared memory buffer.
 *
 * \param i_index_buffer
 * The index of the buffer.
 *
 * \param i_chunkbase
 * The index in the shared memory for the chunk buffer.
 *
 * \return
 * \li 0 : ok
 * \li non zero : error
 */
static int atrshmlog_init_one_buffer(volatile atrshmlog_area_t *i_shm,
				     const int i_index_buffer,
				     const int i_chunkbase)
{
  int result = -1;
  
  volatile atrshmlog_buffer_t* b = &(i_shm->logbuffers[i_index_buffer]);

  int lock_wanted = 0;


  int count_users = 0;

  /* ok. we are through. we have to set the final infos and thats it */
  b->safeguard = ATRSHMLOGSAFEGUARDVALUE;

  atomic_init(&(b->state), atrshmlog_writeable);

  b->info = i_chunkbase;

  b->next_append = i_index_buffer + 1;

#if ATRSHMLOGDEBUG == 1
  printf("next shm is %ld and append is %ld\n", i_index_buffer, i_index_buffer + 1);
#endif
  b->next_full = -1;
  
  b->shmsize = 0;

  return atrshmlog_error_ok;
}

/*******************************************************************/


/** 
 * \n Main code:
 *
 * \brief We initialize the buffer after it is attached .
 *
 * This is not mt safe.
 *
 * So try to use an initialisation program that is
 * simple for it.
 *
 * See the atrshmloginit.c for this
 */
atrshmlog_ret_t atrshmlog_init_shm_log(volatile atrshmlog_area_t *i_area,
				       const int i_count_buffers)
{
  ATRSHMLOGSTAT(atrshmlog_counter_init_shm);

  /* We check first, then we initialize.
   * Its vital that we do NOT reinitialize the buffer ! 
   */
  
  if ( i_area == 0 )
    return  atrshmlog_error_init_shm_1;

  volatile atrshmlog_area_t *a = i_area;

  if (atrshmlog_thread_fence_12)
    atomic_thread_fence(memory_order_acquire); 
  
  if (a->shmid == atrshmlog_get_shmid())
    return atrshmlog_error_init_shm_2;

  /* Ok. We are in. The shm seems not to be initialized */

  // Chunks are after the desciptors
  unsigned int chunks = (char*)&a->logbuffers[i_count_buffers] - (char*)a;

  chunks = (chunks + 15) & ~15;

  int bufferlen = (ATRSHMLOGBUFFER_INFOSIZE + 15) & ~15;
  
  for (int i = 0; i < i_count_buffers ; i++, chunks += ATRSHMLOGBUFFER_INFOSIZE)
    {
      int initresult = atrshmlog_init_one_buffer(a, i, chunks);

#if ATRSHMLOGDEBUG == 1
      printf("%p %lx\n", a, (long)chunks);
#endif

      if (initresult < 0)
	return atrshmlog_error_init_shm_3;

    }
  
  /* We finish the list of available buffers with this one 
   */
  a->logbuffers[ i_count_buffers - 1].next_append = -1;
       

  /* Ok. We can now set the final info in the buffer */
  a->shmversion = ATRSHMLOGVERSION;
  
  a->shmcount = i_count_buffers;

  /* We put the buffers on the available list
   */
  atomic_init(&a->shma, 0);
  
  /* There is no buffer on the full list now */
  atomic_init(&a->shmf, -1);

  /* The flag that stops system wide logging */
  atomic_init(&a->ich_habe_fertig , 0);
  
  a->readerflag = 0;
  a->readerpid = 0;
  a->writerflag = 0;
  a->writerpid = 0;
  a->shmsafeguard = ATRSHMLOGSAFEGUARDVALUE;

  a->shmid = atrshmlog_get_shmid();

  if (atrshmlog_thread_fence_12)
    atomic_thread_fence(memory_order_release); 
  
  return atrshmlog_error_ok;
}

/*******************************************************************/


/** 
 * \n Main code:
 *
 * \brief We read a buffer and write it to a local memory area.
 *
 * We can read half full, full and writeable buffers.
 *
 * The latter is needed for the cleanup after finish of the 
 * buffers so far not cleared and only partly full.
 *
 * This was the  pthread version.
 *
 *  This is done in a safe way. so we can do this in parallel
 * to running writers.
 *
 * And we need to do it. 

 * If all buffers are full the writers have 
 * to loop forever. 
 *
 * So we have to use the mt safe way here.
 *
 * Thats a bit a blockade of the system, but if the number of
 * buffers is sufficient we dont disturb the writers much..
 *
 * They check from outside of the lock.
 * so we can simply lock, switch the flag to dont disturb me mode
 * and clenaup the buffer.
 *
 * Anything else is up to the writer process. 
 * So try to handle the data then as fast as you can.
 * See for a simple reader the
 * atrshmlogreader.c
 */
atrshmlog_ret_t atrshmlog_read(volatile const void* i_area,
			       const atrshmlog_int32_t i_index_buffer,
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
			       atrshmlog_int32_t* o_id)
{
  ATRSHMLOGSTAT(atrshmlog_counter_read);

  *o_target_length = 0;

  atrshmlog_time_t start;
  atrshmlog_time_t end;
  
  int result = -1;

  if(i_area == 0)
    return atrshmlog_error_read_1;

  // we do here a terrible thing.
  // we cast const away.
  // but its ok here.
  volatile atrshmlog_area_t * restrict i_shm = (volatile atrshmlog_area_t*)i_area;

  /* Ok. We can now do the transfer */

  g_tl_t* g = &g_tl;
  
  if (g->atrshmlog_idnotok)
    {
      int tlret = atrshmlog_init_thread_local (g);
      
      /* From now on we are the not ok flag */
      if (tlret)
	return tlret;
    }
  
  if (i_index_buffer < 0)
    return atrshmlog_error_read_2;
  
  if (g->atrshmlog_shm_count <= i_index_buffer)
    return atrshmlog_error_read_3;

  if (i_shm->shmsafeguard != ATRSHMLOGSAFEGUARDVALUE)
    return atrshmlog_error_read_4;

  
  volatile atrshmlog_buffer_t* restrict b = &(i_shm->logbuffers[i_index_buffer]);

  int lock_holds = atrshmlog_full;

  int lock_wanted = atrshmlog_illegal;

  _Bool atomic_lock_result;


  atomic_lock_result
    = atomic_compare_exchange_weak_explicit(&(b->state),
					      &lock_holds,
					      lock_wanted,
					      memory_order_relaxed,
					      memory_order_relaxed);

  if (atomic_lock_result != 1)
    return atrshmlog_error_ok; // o_length is 0, so this is an ok case


  if (atrshmlog_thread_fence_3)
    atomic_thread_fence(memory_order_acquire);
  
  int state = atomic_load_explicit(&(b->state), memory_order_acquire);

  size_t size = b->shmsize;

  // We must use here the theoretical maximum size. period.
  if (size > ATRSHMLOGBUFFER_INFOSIZE)
    {
      result = atrshmlog_error_read_5;
      
      /* This is bad. Someone has spoiled the buffer */
      goto ende;
    }

  /* Ok. We are clear to go to copy */
  
  /* Is the length ok for copy */ 

  if (size > 0)
    {
      ATRSHMLOGSTAT(atrshmlog_counter_read_doit);

      /* We have to do the copy now */

      start = ATRSHMLOG_GET_TSC_CALL();

      char* source = (char*)i_shm + b->info;
      
      #if 1
      (void)memcpy(o_target, source, size );
      #else
	      for (int zippo = 0; zippo < size; zippo++)
		((char*)o_target)[zippo] = source[zippo];
      
      #endif

      *o_pid = b->pid;
      *o_tid = b->tid;
      *o_inittime = b->inittime;
      *o_inittimetsc_before = b->inittimetsc_before;
      *o_inittimetsc_after = b->inittimetsc_after;
      *o_lasttime = b->lasttime;
      *o_lasttimetsc_before = b->lasttimetsc_before;
      *o_lasttimetsc_after = b->lasttimetsc_after;
      *o_target_length = size;
      *o_starttransfer = b->starttransfer;
      *o_acquiretime = b->acquiretime;
      *o_id = b->id;
      
      end = ATRSHMLOG_GET_TSC_CALL();

      *o_difftimetransfer = (end - start);

    }

  result = atrshmlog_error_ok;

 ende:
  b->shmsize = 0;

  atomic_store_explicit(&b->state, atrshmlog_writeable, memory_order_release);

  return result;
}

/*******************************************************************/

/* 
 * \n Main code:
 *
 * \brief This is the actual reader function now.
 * 
 * It transfers the top buffer of the f list in shm.
 * 
 * So if there are buffers it delivers a transfered buffer and puts the 
 * shm buffer on the a list again.
 * 
 *  The memory is transfered into the reader process.
 *
 * so the reader then has to handle it.
 */
atrshmlog_ret_t atrshmlog_read_fetch(volatile const void* i_area,
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
				     atrshmlog_int32_t* o_id)
{
  ATRSHMLOGSTAT(atrshmlog_counter_read_fetch);

  *o_target_length = 0;

  atrshmlog_time_t start;
  atrshmlog_time_t end;
  
  int result = -1;

  if(i_area == 0)
    return atrshmlog_error_read_fetch_1;

  // we do here a terrible thing.
  // we cast const away.
  // but its ok here.
  volatile atrshmlog_area_t * restrict i_shm = (volatile atrshmlog_area_t*)i_area;

  /* Ok. We can now do the transfer */

  g_tl_t *g = &g_tl;
  
  if (g->atrshmlog_idnotok)
    {
      int tlret = atrshmlog_init_thread_local (g);
      
      /* From now on we are the not ok flag */
      if (tlret)
	return tlret;
    }
  
  if (i_shm->shmsafeguard != ATRSHMLOGSAFEGUARDVALUE)
    return atrshmlog_error_read_fetch_2;

  /* We pop from list */
  int isfull = atomic_load_explicit(&i_shm->shmf, memory_order_consume);

  if (isfull == -1)
    return atrshmlog_error_read_fetch_3; /* No buffer on list */

  while (isfull > -1
	 && !atomic_compare_exchange_weak_explicit(&i_shm->shmf,
						   &isfull,
						   i_shm->logbuffers[isfull].next_full,
						   memory_order_acq_rel,
						   memory_order_relaxed))
    ;

  if (isfull == -1)
    return atrshmlog_error_read_fetch_3; /* No buffer on list */
  
  volatile atrshmlog_buffer_t* restrict b = &(i_shm->logbuffers[isfull]);

  *o_index_buffer = isfull;

  if (atrshmlog_thread_fence_4)
    atomic_thread_fence(memory_order_acquire);

  
  int state = atomic_load_explicit(&(b->state), memory_order_acquire);


  size_t size = b->shmsize;

  // We must use here the theoretical maximum size. period.
  if (size > ATRSHMLOGBUFFER_INFOSIZE)
    {
      result = atrshmlog_error_read_fetch_4;
      
      /* This is bad. Someone has spoiled the buffer */
      goto ende;
    }

  /* Ok. We are clear to go to copy */
  
  /* Is the length ok for copy */ 

  result = atrshmlog_error_read_fetch_5;
  
  if (size > 0)
    {
      ATRSHMLOGSTAT(atrshmlog_counter_read_fetch_doit);

      /* We have to do the copy now */

      start = ATRSHMLOG_GET_TSC_CALL();

      char* restrict source = (char*)i_shm + b->info;
      
      #if 1
      (void)memcpy(o_target, source, size );
      #else
	      for (int zippo = 0; zippo < size; zippo++)
		((char*)o_target)[zippo] = source[zippo];
      
      #endif

      *o_pid = b->pid;
      *o_tid = b->tid;
      *o_inittime = b->inittime;
      *o_inittimetsc_before = b->inittimetsc_before;
      *o_inittimetsc_after = b->inittimetsc_after;
      *o_lasttime = b->lasttime;
      *o_lasttimetsc_before = b->lasttimetsc_before;
      *o_lasttimetsc_after = b->lasttimetsc_after;
      *o_target_length = size;
      *o_starttransfer = b->starttransfer;
      *o_acquiretime = b->acquiretime;
      *o_id = b->id;

      b->next_full = -1;

      end = ATRSHMLOG_GET_TSC_CALL();

      *o_difftimetransfer = (end - start);

      result = atrshmlog_error_ok; /* We have succesful transfered a buffer */

    }

 ende:

  b->shmsize =  0;

  atomic_store_explicit(&b->state, atrshmlog_writeable, memory_order_release);

  /* We push it now on the available list again */

  b->next_append = atomic_load_explicit(&i_shm->shma, memory_order_acquire);
      
  while (!atomic_compare_exchange_weak_explicit(&i_shm->shma,
						&b->next_append,
						isfull,
						memory_order_release,
						memory_order_relaxed))
    ;

  return result;
}

/*******************************************************************/

/** 
 * \n Main code:
 *
 * \brief We verify the buffer is ok.
 *
 * We check the shmid and we check the states.
 * 
 * This can not be used in mt case, so use it only for simple checker 
 * programs.
 *
 * A basic version is already in the atrshmlogverify.c
 */
atrshmlog_ret_t atrshmlog_verify(void)
{
  ATRSHMLOGSTAT(atrshmlog_counter_verify);

  int result = -1;
  
  if ( ! ATRSHMLOG_LOGGING )
    return atrshmlog_error_verify_1;

  atrshmlog_area_t * a = ATRSHMLOG_GETAREA;
    
  int i = 0;

  int limit = 0;
    
  --result;

  if (atrshmlog_thread_fence_9)
    atomic_thread_fence(memory_order_acquire); 

  if (a->shmid != atrshmlog_get_shmid())
    return atrshmlog_error_verify_2;

  --result;

  if (a->shmsafeguard != ATRSHMLOGSAFEGUARDVALUE)
    return atrshmlog_error_verify_3;
    
  --result;

  limit = a->shmcount;
    
  if (a->shmversion != ATRSHMLOGVERSION)
    {
#if ATRSHMLOGDEBUG == 1      
      printf("version error\n");
#endif
      return atrshmlog_error_verify_4;
    }
    
  for (i = 0; i < limit ; i++)
    {
      int state = atomic_load(&(a->logbuffers[i].state));

      --result;
    
      if (!(state == atrshmlog_writeable
        || state == atrshmlog_write_halffull
        || state == atrshmlog_full ))
	return atrshmlog_error_verify_5;
      
      --result;

      if (a->logbuffers[i].safeguard != ATRSHMLOGSAFEGUARDVALUE)
	return atrshmlog_error_verify_6;

      
      /* TODO: 
       * check the list of info data too 
       * perhaps we will do this in the future.
       * for now we try to be not paranoid and think
       * the content is ok if all the numbers and safguards
       * are ok...
       */
    }

  return atrshmlog_error_ok;
}


#endif


/************************************************************************/

// non inline code

/*
 * get time fast. we use the tsc approch her.
 * later on we will have to adapt this for
 * POWER
 * SPARC
 * MIPS
 * ITANIUM
 * ARM
 * and any platform that is in need for the atrshmlog
 */

#if ATRSHMLOG_INLINE_TSC_CODE == 1

/* this is done in the header */

#else

# if ATRSHMLOG_PLATFORM_LINUX_X86_64_GNU == 1 


/********************************************************************/

/** 
 * \n Main code:
 *
 * The parallel version in modern cpu 
 */
uint64_t atrshmlog_get_tsc_par_x86_64_gnu(void)
{
  uint32_t hi, lo;

  __asm volatile
    ("rdtscp" : "=a" (lo), "=d" (hi));

  return ((uint64_t)hi << 32) | lo;
}


/********************************************************************/

/** 
 * \n Main code:
 *
 * The not so modern combination of fence and call 
 */
uint64_t atrshmlog_get_tsc_fence_x86_64_gnu(void)
{
  uint32_t hi, lo;

  __asm volatile
    ("lfence");
  
  __asm volatile
    ("rdtsc" : "=a" (lo), "=d" (hi));

  return ((uint64_t)hi << 32) | lo;
}


/********************************************************************/

/** 
 * \n Main code:
 *
 * The simple version, no check for fences 
 */
uint64_t atrshmlog_get_tsc_x86_64_gnu(void)
{
  uint32_t hi, lo;

  __asm volatile
    ("rdtsc" : "=a" (lo), "=d" (hi));

  return ((uint64_t)hi << 32) | lo;
}


/********************************************************************/

/** 
 * \n Main code:
 *
 * The well know dummy 
 */
uint64_t atrshmlog_get_tsc_null_x86_64_gnu(void)
{
  return 0;
}
# endif

#endif

/*******************************************************************/

/**
 * \n Main code:
 *
 * This is os dependent. 
 * We use a static here that can be set at attach or from the 
 * program. So be sure you know what time
 * you want
 */
#if ATRSHMLOG_INLINE_GETTIME == 1

/* this is done in the header */

#else

# if ATRSHMLOG_PLATFORM_LINUX_X86_64_GNU == 1

/** 
 * \n Main code:
 *
 * We get the internal tsc time according to clock id
 */
atrshmlog_time_t atrshmlog_get_clicktime(void)
{
  if (atrshmlog_clock_id == 3)
    {
      return atrshmlog_get_tsc_par_x86_64_gnu();
    }

  if (atrshmlog_clock_id == 2)
    {
      return atrshmlog_get_tsc_fence_x86_64_gnu();
    }

  if (atrshmlog_clock_id == 1)
    {
      return atrshmlog_get_tsc_x86_64_gnu();
    }

  return atrshmlog_get_tsc_null_x86_64_gnu();
}
# endif

#endif

/* end of file */
