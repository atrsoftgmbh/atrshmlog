
#include "../atrshmlog_internal.h"

/*******************************************************************/

/**
 * \file atrshmlogimpl_buffer_slave_proc.c
 */

/**
 * \n Main code:
 *
 * \brief We are a slave thread function.
 *
 * We wait for a time slice, then we check the f list.
 * 
 * If its empty we wait again.
 *
 * If its not empty we try to grap a buffer.
 *
 * We check this and if we got one we start the transfer.
 *
 * After the transfer we  check the dispose flag and give the 
 * buffer free if it is set.
 *
 * Then we start to wait again.
 */
atrshmlog_thread_ret_t atrshmlog_f_list_buffer_slave_proc(void* i_arg)
{
  // We have to initialize the thread locals

#if ATRSHMLOG_USE_PTHREAD == 1
  
  union u { atrshmlog_thread_ret_t p; int64_t ui; };

  union u conv;
  
  atrshmlog_g_tl_t* g  = (atrshmlog_g_tl_t* )atrshmlog_get_thread_locals_adress();
  
  if (g->atrshmlog_idnotok)
    {
      int tlret = atrshmlog_init_thread_local (g);
      
      /* From now on we are the not ok flag */
      if (tlret)
	{
	  conv.ui = tlret;
	  
	  return conv.p;
	}
    }

#endif
  
#if ATRSHMLOG_USE_C11_THREAD == 1
  
  union u { atrshmlog_thread_ret_t p; int64_t ui; };

  union u conv;
  
  atrshmlog_g_tl_t* g  = (atrshmlog_g_tl_t* )atrshmlog_get_thread_locals_adress();
  
  if (g->atrshmlog_idnotok)
    {
      int tlret = atrshmlog_init_thread_local (g);
      
      /* From now on we are the not ok flag */
      if (tlret)
	{
	  conv.ui = tlret;
	  
	  return conv.p;
	}
    }

#endif
  
#if ATRSHMLOG_USE_WINTHREAD == 1
  
  atrshmlog_g_tl_t* g  = (atrshmlog_g_tl_t* )atrshmlog_get_thread_locals_adress();
  
  if (g->atrshmlog_idnotok)
    {
      int tlret = atrshmlog_init_thread_local (g);
      
      /* From now on we are the not ok flag */
      if (tlret)
	{
	  return;
	}
    }

#endif

  // we connect to the slave list
  
  g->next = (atrshmlog_g_tl_t*)atomic_load_explicit(&atrshmlog_tpslave, memory_order_relaxed);

  // Push on stack ...
  while(!atomic_compare_exchange_weak_explicit(&atrshmlog_tpslave,
					       &g->next,
					       g,
					       memory_order_relaxed,
					       memory_order_relaxed))
    ;

  // we are on the list now
  
  atrshmlog_f_list_active_slaves++;

#if ATRSHMLOGDEBUG == 1
  printf("in slave\n");
#endif
  
  while (atrshmlog_f_list_buffer_slave_run)
    {
      // we can be stopped by an external this way.
      // we give our tl to the iterator and thats it.
      // the external checks the tid and then sets a tun off on us
      // so from then on we have to exit
      if (g->atrshmlog_idnotok)
	{
	  goto leave;
	}
      
      // We use f list 
      atrshmlog_tbuff_t* tbuff = (atrshmlog_tbuff_t*)atomic_load_explicit(&atrshmlog_tpf, memory_order_relaxed);

      if (tbuff == 0)
	{
	  ATRSHMLOG_SLEEP_NANOS(atrshmlog_f_list_buffer_slave_wait);

	  if (atrshmlog_logging_process_off_final)
	    goto leave;
	  
	  continue;
	}

      // Pop from list
      while (tbuff &&
	     !atomic_compare_exchange_weak_explicit(&atrshmlog_tpf,
						    &tbuff,
						    tbuff->next_full,
						    memory_order_relaxed,
						    memory_order_relaxed))
	;
	  
      if (tbuff)
	{
	  volatile int f = atomic_load_explicit(&tbuff->dispatched, memory_order_acquire);
	  // We get the buffer content synced

	  if (atrshmlog_thread_fence_6)
	    atomic_thread_fence (memory_order_acquire);
	      
	  size_t aksize = tbuff->size;

	  if (aksize > 0)
	    atrshmlog_transfer_mem_to_shm(tbuff, g);

	  tbuff->size = 0;

	  if (atrshmlog_thread_fence_7)
	     atomic_thread_fence (memory_order_release);

          atomic_store_explicit(&tbuff->dispatched , 0, memory_order_release);
	  
	  if (tbuff->dispose == 1)
	    {
	      atrshmlog_free(tbuff); 
	    }
	}
    }

 leave:
  ;

  // remove from list 
  atrshmlog_remove_slave_via_local(g);
  
  // balance the counter
  atrshmlog_f_list_active_slaves--;
  
  ATRSHMLOG_THREADFUNCTION_RETURN(NULL);
}



