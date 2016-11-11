
#include "../atrshmlog_internal.h"


/********************************************************************/

/**
 * \file atrshmlogimpl_attach_pid_flag.c
 */

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
volatile atrshmlog_pid_t atrshmlog_attach_pid;

