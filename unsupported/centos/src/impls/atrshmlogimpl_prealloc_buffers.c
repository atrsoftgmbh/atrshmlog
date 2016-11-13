
#include "../atrshmlog_internal.h"


/**
 * \file atrshmlogimpl_prealloc_buffers.c
 */

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
char atrshmlog_buffers_prealloced_chunks[ATRSHMLOGBUFFER_PREALLOCED_COUNT * ATRSHMLOGBUFFER_INFOSIZE ];

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
tbuff_t atrshmlog_buffers_prealloced[ATRSHMLOGBUFFER_PREALLOCED_COUNT]
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
atomic_intptr_t atrshmlog_tps = ATOMIC_VAR_INIT(atrshmlog_buffers_prealloced);


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
atomic_intptr_t atrshmlog_tpa = ATOMIC_VAR_INIT(atrshmlog_buffers_prealloced);

void atrshmlog_memset_prealloced(void)
{
  memset(atrshmlog_buffers_prealloced_chunks, 0, ATRSHMLOGBUFFER_PREALLOCED_COUNT * ATRSHMLOGBUFFER_INFOSIZE);
}


