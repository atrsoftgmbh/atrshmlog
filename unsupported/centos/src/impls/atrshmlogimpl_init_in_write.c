
#include "../atrshmlog_internal.h"



/*******************************************************************/

/**
 * \file atrshmlogimpl_init_in_write.c
 */

/**
 * \brief helper : We init in write
 * This is the normal way for a writing process .
 * The thread that wants to log has to do the init thing
 * So the first call is a bit more time consuming.
 * But we dont need a thread attach or so. 
 * We simply execute a write.
 *
 * \param g
 * The adress of the thread local struct for this thread.
 *
 * \return
 * - 0: ok
 * - non zero : error
 */
int atrshmlog_init_in_write(g_tl_t* g)
{
  int tlret = atrshmlog_init_thread_local (g);

  /* from now on we are the not ok flag */
  if (tlret)
    return tlret;

  /*
   * Interrim. We need to check if we are a fork clone 
   * This is bit dirty
   * We use the process pid and the pid we got in thread init
   * and check if these are same.
   * If we are a fork clone we have different numbers.
   * So we can then do the needed thing .
   */
  if (atrshmlog_attach_pid != g->atrshmlog_thread_pid)
    {
      /* We have to change the pid */
      atrshmlog_attach_pid = g->atrshmlog_thread_pid;

      /* This is NOT mt safe, but i assume it does not matter 
       * Ok. This is risky. But for now we assume its a SHELL, 
       * and shells are NORMALLY not mt things ...
       * So we have come so far, we need a slave thread to do 
       * the transfer work.
       */
      int ret_slave = 0;
		  
      for (int slaves = 0; ret_slave == 0
	     && slaves < atrshmlog_f_list_buffer_slave_count;
	   slaves++)
	{
	  ret_slave = atrshmlog_create_slave();
	}
#if ATRSHMLOGDEBUG == 1
      printf("Process %ld startet slaves %ld .\n" , (long)atrshmlog_thread_pid, (long)ret_slave);
#endif
    }
      

  /* We reach this only if we are still ok 
   * and we are the initialized flag before 
   * so we now have to make all those buffers and list linking
   */
  for (int i = 0; i < ATRSHMLOGTARGETBUFFERMAX; i++)
    {
      g->atrshmlog_targetbuffer_arr[i] = atrshmlog_acquire_buffer(g);
      
      if (g->atrshmlog_targetbuffer_arr[i] == 0)
	{
	  g->atrshmlog_idnotok = 1; // no logging for this thread
	  return atrshmlog_error_init_in_write_1;
	}
    }
	    
  // we start by using first buffer
  g->atrshmlog_targetbuffer_index = 0;

  return atrshmlog_error_ok;
}

