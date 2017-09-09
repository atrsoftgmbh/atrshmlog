
#include "../atrshmlog_internal.h"


/********************************************************************/

/**
 * \file atrshmlogimpl_slaves_counter_flag.c
 */

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
_Alignas(128) int atrshmlog_f_list_buffer_slave_count = 1;

