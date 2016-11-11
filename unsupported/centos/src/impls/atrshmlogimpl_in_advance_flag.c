
#include "../atrshmlog_internal.h"


/********************************************************************/

/**
 * \file atrshmlogimpl_in_advance_flag.c
 */

/**
 * \n Main code:
 *
 * \brief We decide if we init in advance
 *
 * this is big slow down for the attach and a dyn alloc call for buffers
 * But in return if your system place the cost for using memory
 * on the using operation it can shift thath cost from the 
 * phase of running system to the phase of initialization.
 * this was encountered on mingw port.
 */
int atrshmlog_init_buffers_in_advance = ATRSHMLOGINITINADVANCEDEFAULT;

