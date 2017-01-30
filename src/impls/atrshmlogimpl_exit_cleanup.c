
#include "../atrshmlog_internal.h"



/************************************************************/

/**
 * \file atrshmlogimpl_exit_cleanup.c
 */

/**
 * \n Main code:
 *
 * \brief We are the atexit function to clean up 
 * what was not cleaned up so far.
 *
 * We assume no more running
 * slaves - but we switch at least them off...
 *
 * Its complicated to make this right in an mt env.
 *
 * So this is a simplified and _wrong_ version.
 *
 * It does no locking and so there is a small chance to do it
 * parallel to a still running slave.
 *
 * Worse, the free can torpedoise a slave.
 *
 * So be sure to finish only if you have no problems with the 
 * slaves.
 *
 * test t_exit_cleanup.c
 */
void atrshmlog_exit_cleanup (void)
{
  ATRSHMLOGSTAT(  atrshmlog_counter_exit_cleanup);
  
  if (atrshmlog_base_ptr == 0)
    return;

  atrshmlog_g_tl_t* g  = (atrshmlog_g_tl_t* )atrshmlog_get_thread_locals_adress();

#if ATRSHMLOG_THREAD_LOCAL == 0

  // if we need the pthread specific this can happen
  if (g == NULL)
    {
      return;
    }

#endif
  
  // Do we have to initialize ?
  if (g->atrshmlog_idnotok == -1)
    {
      atrshmlog_init_thread_local (g);

      // bad luck. no transfer possible.
      if(g->atrshmlog_idnotok)
	return;
    }

  // And now we have to switch the flag for THIS thread to on again.
  // or we never make it if someone made a stop for us.
  g->atrshmlog_idnotok = 0;
  
  // No more logging from threads
  atrshmlog_set_logging_process_off();

  
  // No more buffer freeing from slaves
  atrshmlog_set_f_list_buffer_slave_run_off();

  if (atrshmlog_wait_for_slaves)
    while (atomic_load(&atrshmlog_f_list_active_slaves) > 0)
      ATRSHMLOG_SLEEP_NANOS(100000);

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
  
  // We are serious. We cut off the list
  atrshmlog_tbuff_t* tp = (atrshmlog_tbuff_t*)atomic_exchange(&atrshmlog_tps, 0);
  
  while (tp)
    {
      int dispatched = atomic_load(&tp->dispatched);

      if (atrshmlog_thread_fence_10)
	atomic_thread_fence(memory_order_acquire); 
      
      atrshmlog_tbuff_t* tpnext = tp->next_cleanup;

      // We take every buffer
      // Even the dispatched
      // This assumes the slaves are no longer working
      // and they have done their job
      // If you end a program before this is true be sure
      // to switch the slaves off, wait at least a little bit to be
      // sure the threads have been stopped,
      // evtl check for still running threads ...
      // and then after seeing them go down do the thing here
      
      // We skip broken buffers
      if (tp->safeguardfront != ATRSHMLOGSAFEGUARDVALUE)
	goto cleanit;

      int dodispatch = (tp->pid == mypid);

      int mret ;
      
      if (dodispatch)
        mret = atrshmlog_transfer_mem_to_shm(tp, g);

      tp->size = 0;
      
    cleanit:
      ;

      tp = tpnext;
    }

  // No more waiting in loops
  // this has to be AFTER the last use of mem to shm ...
  atrshmlog_set_logging_process_off_final();

  // we detach regulary from the shm

  atrshmlog_detach();
}

