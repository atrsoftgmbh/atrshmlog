
#include "../atrshmlog_internal.h"

/************************************************************************/

/**
 * \file atrshmlogimpl_init_thread_local.c
 */

/**
 * \n Main code:
 *
 * \brief We init our thread local vars - the everyone needs helper. 
 *
 * \param i_g
 * The adress of the thread local struct.
 *
 * \return 
 * - 0 : ok
 * - non zero : error
 */
int atrshmlog_init_thread_local (atrshmlog_g_tl_t* restrict i_g)
{
  /* Are we the flag or the initialize needed flag */
  if (i_g->atrshmlog_idnotok == -1)
    {

      if (atrshmlog_thread_fence_9)
	atomic_thread_fence(memory_order_acquire); 

      // we are normally not linked
      i_g->next = 0;

      // we clear the pointers
      for (int i = 0; i < ATRSHMLOGTARGETBUFFERMAX; i++)
	i_g->atrshmlog_targetbuffer_arr[i] = NULL;

      // we clear the index
      i_g->atrshmlog_targetbuffer_index = 0;
      
      atrshmlog_area_t * a_shm = ATRSHMLOG_GETAREA;
      
      int versionnotok = (atrshmlog_base_ptr == 0) || (a_shm->shmversion != ATRSHMLOGVERSION);
      i_g->atrshmlog_idnotok = versionnotok || (a_shm->shmid != atrshmlog_id) ;

      // We do NOT overwrite in this case. Perhaps we can do something
      // in debugging with the info.
      if ( i_g->atrshmlog_idnotok == 0)
	{
	  i_g->atrshmlog_shm_count = a_shm->shmcount;

	  i_g->atrshmlog_thread_pid = getpid();

	  ATRSHMLOG_GETTHREADID(i_g->atrshmlog_thread_tid);

	  i_g->strategy = atrshmlog_buffer_strategy; 

	  i_g->autoflush = atrshmlog_autoflush;
	  
	  i_g->number_dispatched = 0;

	  i_g->counter_write0 = 0;

	  i_g->counter_write0_discard = 0;

	  i_g->counter_write0_wait = 0;

	  i_g->counter_write0_adaptive = 0;

	  i_g->counter_write0_adaptive_fast = 0;

	  i_g->counter_write0_adaptive_very_fast = 0;

	  i_g->counter_write1 = 0;

	  i_g->counter_write1_discard = 0;

	  i_g->counter_write1_wait = 0;

	  i_g->counter_write1_adaptive = 0;

	  i_g->counter_write1_adaptive_fast = 0;

	  i_g->counter_write1_adaptive_very_fast = 0;

	  i_g->counter_write2 = 0;

	  i_g->counter_write2_discard = 0;

	  i_g->counter_write2_wait = 0;

	  i_g->counter_write2_adaptive = 0;

	  i_g->counter_write2_adaptive_fast = 0;

	  i_g->counter_write2_adaptive_very_fast = 0;

	}
    }

  /* From now on we are the not ok flag - 0 is ok, else is not */
  if (i_g->atrshmlog_idnotok)
    return atrshmlog_error_init_thread_local_1;

  return atrshmlog_error_ok;
}

