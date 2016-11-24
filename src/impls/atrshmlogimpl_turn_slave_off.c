
#include "../atrshmlog_internal.h"

/*******************************************************************/

/**
 * \file atrshmlogimpl_turn_slave_off.c
 */

/** 
 * \n Main code:
 *
 * \brief We switch the thread off and dispatch its buffers
 *
 * This works for normal threads as expected. 
 * For a slave it switches the flag, and no buffers are 
 * dispateched, they are all NULL ptrs
 */
 void atrshmlog_turn_slave_off(volatile const void* i_slave)
 {
   if (i_slave == NULL)
     return;

   atrshmlog_slave_t* i =  (atrshmlog_slave_t*)i_slave;
   
   atrshmlog_g_tl_t* g  =  i->g;

   g->atrshmlog_idnotok = 1;
 }

