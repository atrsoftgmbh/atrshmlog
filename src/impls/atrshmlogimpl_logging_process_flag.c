
#include "../atrshmlog_internal.h"

/********************************************************************/

/**
 * \file atrshmlogimpl_logging_process_flag.c
 */

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

