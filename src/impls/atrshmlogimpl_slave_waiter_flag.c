
#include "../atrshmlog_internal.h"


/********************************************************************/

/**
 * \file atrshmlogimpl_slave_waiter_flag.c
 */

/**
 * \n Main code:
 *
 * \brief We wait that long in a nanosleep in the slave if the f list is empty.
 *
 * There is an env in attach to set this.
 */
int atrshmlog_f_list_buffer_slave_wait = ATRSHMLOG_INIT_BUFFER_SLAVE_WAIT;

