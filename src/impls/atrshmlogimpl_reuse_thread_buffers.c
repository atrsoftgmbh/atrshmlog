
#include "../atrshmlog_internal.h"



/************************************************************/

/**
 * \file atrshmlogimpl_reuse_thread_buffers.c
 */

/**
 * \n Main code:
 *
 * \brief  We check and push buffers from a thread on the full list with dispose on.
 * This is to make buffers available again.
 * 
 * You need the tid f the thread. And its the module tid.
 *
 * Be sure the thread is dead. Its a race if not. consider be warned.
 */
atrshmlog_ret_t atrshmlog_reuse_thread_buffers(atrshmlog_tid_t i_tid)
{
  ATRSHMLOGSTAT(  atrshmlog_counter_reuse_thread_buffers);
  
  int found = 0;
  
  /* Ok. This is first strange.
   * But when i used it with a ksh hack i found a problem
   * with multile buffers written ...
   * And the reason is that a shell makes use of fork
   * and exec and also of the nice exit ...
   * So we are in the land of multiple exit calls to buffers
   * in diffrent processes.
   * This is why we only write our own buffers out.
   * Meaning : Our pid must match the pid of the buffer.
   * If we fork and exit the bufer has a diffrent pid...
   * So we can use this later as a trick o get info from the 
   * parent, but for now i restrict the write to buffers
   * with the same pid.
   * As a fact the pid is set when we alloc a buffer for a real 
   * thread to the init value from thread locals. 
   * So it is
   * done in the running process for vital threads.
   * This means we can ignore the rest for now.
   */
  atrshmlog_pid_t mypid = getpid();
  
  atrshmlog_tbuff_t* tp = (atrshmlog_tbuff_t*)atomic_load(&atrshmlog_tps);
  
  while (tp)
    {
      int dispatched = atomic_load(&tp->dispatched);

      if (atrshmlog_thread_fence_10)
	atomic_thread_fence(memory_order_acquire); 
      
      atrshmlog_tbuff_t* tpnext = tp->next_cleanup;

      // We skip broken buffers
      if (tp->safeguardfront != ATRSHMLOGSAFEGUARDVALUE)
	goto cleanit;

      // not our target
      if (tp->tid != i_tid)
	goto cleanit;

      // old process 
      if (tp->pid != mypid)
	goto cleanit;

      // ok. we have it.
      // we will reuse it after this
      tp->dispose = 1;
      
      atrshmlog_dispatch_buffer(tp);

      ++found;
      
    cleanit:
      ;

      tp = tpnext;
    }

  return found;
}

