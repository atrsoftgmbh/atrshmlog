
#include "../atrshmlog_internal.h"


/********************************************************************/

/**
 * \file atrshmlogimpl_stop_slaves_flag.c
 */

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
volatile int atrshmlog_f_list_buffer_slave_run = 1;

