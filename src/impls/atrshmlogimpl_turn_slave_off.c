
#include "../atrshmlog_internal.h"

/*******************************************************************/

/**
 * \file atrshmlogimpl_turn_slave_off.c
 */

/** 
 * \n Main code:
 *
 * \brief We switch the thread off 
 *
 * We set the stop flag and the thread runs next time into this.
 *
 * So we not have to do anything else, the thread itself makes the cleanup.
 *
 * test t_turn_slave_off.c
 */
 void atrshmlog_turn_slave_off(volatile const void* const i_slave)
 {
   if (i_slave == NULL)
     return;

   atrshmlog_slave_t* i =  (atrshmlog_slave_t*)i_slave;
   
   atrshmlog_g_tl_t* g  =  i->g;

   g->atrshmlog_idnotok = 1;

   
 }

