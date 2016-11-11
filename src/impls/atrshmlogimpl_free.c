
#include "../atrshmlog_internal.h"


/**
 * \file atrshmlogimpl_free.c
 */

/**
 * \n Main code:
 *
 * \brief We set the buffer free.
 *
 * So we put it on the a list for the next user - we never 
 * release a buffer in real life, simply try to recycle it.
 *
 * This is only a problem if you hit the dyn memory alloc
 * and forget to free buffers you no longer need.
 *
 * Then your process will blow up and fragment a lot.
 *
 * If you handle logging serious you free the logging 
 * with atrshmlog_stop at the end of the thread  .
 *
 * \param t
 * The buffer adress.
 *
 * \return 
 * void
 */
void atrshmlog_free(atrshmlog_tbuff_t* restrict t)
{
  if (t == 0)
    return;

  ATRSHMLOGSTAT(atrshmlog_counter_free);
  
  // we make clear this buffer IS initialized and on the linked lists
  // already so we NOT have to relink it and NOT reinit it
  t->dispose = ATRSHMLOG_DISPATCH_REUSE;
  t->size = 0;
  
  // we push it now only on the a list for reuse
  t->next_append = (atrshmlog_tbuff_t*)atomic_load(&atrshmlog_tpa);

  while (!atomic_compare_exchange_weak(&atrshmlog_tpa, &t->next_append, t))
    ;
}

