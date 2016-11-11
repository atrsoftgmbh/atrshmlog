
#include "../atrshmlog_internal.h"

/***************************************************************/

/**
 * \file atrshmlogimpl_get_next_sl.c
 */

/** 
 * \n Main code:
 *
 * \brief We get the next element on slave list
 *
 * If we deliver a 0 we start from top.
 * 
 */
volatile const void* atrshmlog_get_next_slave_local(volatile const void* i_actual)
{
  volatile const void *n = 0;

  if (i_actual == NULL)
    {
      // get top.
      n = (g_tl_t*)atomic_load_explicit(&atrshmlog_tpslave, memory_order_relaxed);      
    }
  else
    {
      g_tl_t* a = (g_tl_t*)i_actual;

      n = a->next;
    }
    
  return n;
}

