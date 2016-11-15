
#include "../atrshmlog_internal.h"

/***************************************************************/

/**
 * \file atrshmlogimpl_remove_sl.c
 */

/** 
 * \n Main code:
 *
 * \brief We remove the save from the list of slaves
 *
 */
atrshmlog_ret_t atrshmlog_remove_slave_via_local(volatile const void* i_thread_local)
{
  int result = 0;
  
  atrshmlog_g_tl_t* g = (atrshmlog_g_tl_t*) i_thread_local;

  if (g == NULL)
    return -1;
  
  atrshmlog_g_tl_t *l =  (atrshmlog_g_tl_t*)atomic_load_explicit(&atrshmlog_tpslave, memory_order_relaxed);

  while (l != NULL
	 && l != g
	 && l->next != g)
    l = l->next;

  if (l == NULL)
    return 1; // noting to do, list is empty
  else if (l == g)
    {
      // ok. we are on top. we have to be removed from the slave list only

      atrshmlog_g_tl_t* n = g;
      
#if ATRSHMLOGDEBUG == 1
      printf("we are on top.\n");
#endif
      
      // Pop from stack ...
      while(n
	    && !atomic_compare_exchange_weak_explicit(&atrshmlog_tpslave,
						   (intptr_t*)&n,
						   (intptr_t)n->next,
						   memory_order_relaxed,
						   memory_order_relaxed))
	;
    }
  else 
    {
      // ok. we are the l->next. skip it from now on
      l->next = g->next;
    }

  return result;
}

