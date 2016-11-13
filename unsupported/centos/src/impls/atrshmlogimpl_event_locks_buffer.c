
#include "../atrshmlog_internal.h"


/********************************************************************/

/**
 * \file atrshmlogimpl_event_locks_buffer.c
 */

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
int atrshmlog_event_locks_max = ATRSHMLOGEVENTCOUNT;


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
atrshmlog_event_t atrshmlog_event_locks_buffer [ ATRSHMLOGEVENTCOUNT ];


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


