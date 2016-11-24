
#include "../atrshmlog_internal.h"

/***************************************************************/

/**
 * \file atrshmlogimpl_remove_sl.c
 */

/** 
 * \n Main code:
 *
 * \brief We remove the slave from the list of slaves
 *
 */
atrshmlog_ret_t atrshmlog_remove_slave_via_local(volatile const void* i_slave)
{
  int result = 0;

  atrshmlog_slave_t* i = (atrshmlog_slave_t*)i_slave;
  
  atrshmlog_slave_t *l =  (atrshmlog_slave_t*)atomic_load_explicit(&atrshmlog_tpslave, memory_order_relaxed);

  while (l != NULL
	 && l != i
	 && l->next != i)
    l = l->next;

  if (l == NULL)
    return 1; // noting to do, list is empty
  else if (l == i)
    {
      // ok. we are on top. we have to be removed from the slave list only

      atrshmlog_slave_t* n = i;
      
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
      l->next = i->next;
    }

  // we can now get rid of the memory
  free(i);
  
  return result;
}

