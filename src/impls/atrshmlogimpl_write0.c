
#include "../atrshmlog_internal.h"


/********************************************************************/

/**
 * \file atrshmlogimpl_write0.c
 */

/** 
 * \n Main code:
 *
 * \brief We write to the log. 
 *
 * This is the major writer. 
 * In this form we only write the basic log info.
 *
 * There is a hidden mechanism.
 *
 * If we do a log in P or p eventflag, we use only starttime.
 * and if this is 0 we call internal to get the time.
 *
 * If the eventflag is not P or p we check the endtime.
 * If it is 0 we get the time and use it for the endtime.
 *
 * These two help us to minimize calls in case we are bound
 * in a fat layer for another languge than c or c++.
 * Its a fact that you then have to minimize the number of calls.
 * In numbers: gettime naket 40 clicks. Good. 
 * In JNI 300 clicks minimum . BAD.
 * So we use the hidden mechanism to make use of time without a 
 * call in P or p and with one call in other events.
 * this is a very last late chnge, i allways beleived it would be BAD 
 * to mix up calls and their responsibility.
 * So i check this first in an early state of the module. 
 * But after the thing was nearly ready a test drive with jni 
 * showed me the truth. 
 * It was necessary to reduce JNI calls.
 * so i gave up my resistance against this BORG like Java mentality
 * and did the change - forgive me you joung and
 * innovent language lawyer - but its a fact of simple survival 
 * to make things like this when it comes to an audiance that 
 * make their part so BAD that they insist that you make some 
 * kind of a hybrid of a swiss army nife and a toolbox...
 *
 * test t_write0.c
 */
atrshmlog_ret_t atrshmlog_write0(const atrshmlog_int32_t i_eventnumber,
				 const atrshmlog_int32_t i_eventflag,
				 const atrshmlog_int32_t i_userflag,
				 atrshmlog_time_t i_starttime,
				 atrshmlog_time_t i_endtime)
{
  atrshmlog_g_tl_t* g  = (atrshmlog_g_tl_t* )atrshmlog_get_thread_locals_adress();
  

#if ATRSHMLOG_THREAD_LOCAL == 0

  // this can happen if pthread specific is in
  if (g == NULL)
    return  atrshmlog_error_write0_10;

#endif

  ATRSHMLOGSTATLOCAL(g,counter_write0);
  
  if (i_eventnumber < 0) {
    ATRSHMLOGSTAT(atrshmlog_counter_write0_abort1);
    
    return atrshmlog_error_write0_1;
  }
  
  if (i_eventnumber >= atrshmlog_event_locks_max) {
    ATRSHMLOGSTAT(atrshmlog_counter_write0_abort2);

    return atrshmlog_error_write0_2;
  }

  /* We use this flag also as the initialized flag
   * So we have all the initialize stuff hereafter 
   */
  if (g->atrshmlog_idnotok != 0)
    {
      if (g->atrshmlog_idnotok != -1) {
	ATRSHMLOGSTAT(atrshmlog_counter_write0_abort3);

	return atrshmlog_error_write0_3;
      }
      
      int ret = atrshmlog_init_in_write(g);

      if (ret != 0)
	{
	  ATRSHMLOGSTAT(atrshmlog_counter_write0_abort4);

	  return ret;
	}
    }

  // Initialized

  // The hidden mechanism to get things minimised
  // in case we are bound to a layer for another language.
  if (i_eventflag == ATRSHMLOGPOINTINTIMEP
       || i_eventflag == ATRSHMLOGPOINTINTIMEp
      )
    {
      if (i_starttime == 0)
	i_starttime = ATRSHMLOG_GET_TSC_CALL();

      i_endtime = i_starttime;
    }
  else
    {
      if (i_endtime == 0)
	i_endtime = ATRSHMLOG_GET_TSC_CALL();
    }

  
  atrshmlog_int32_t totallen;

  // Do the normal stuff now ...

  totallen = ATRSHMLOGCONTROLDATASIZE;

  int strategy_count = 0;
  
  // We use some goto jumping here, so this is a first target
 testagain_dispatched:

  ;
    
  register atrshmlog_tbuff_t* tbuff = g->atrshmlog_targetbuffer_arr[g->atrshmlog_targetbuffer_index];

  if (atomic_load_explicit(&(tbuff->dispatched), memory_order_acquire) != 0)
    {
      g->atrshmlog_targetbuffer_index++;

      if (g->atrshmlog_targetbuffer_index >= ATRSHMLOGTARGETBUFFERMAX)
	g->atrshmlog_targetbuffer_index = 0;

      ++strategy_count;

      if ((strategy_count % ATRSHMLOGTARGETBUFFERMAX) == 0)
	switch (g->strategy)
	  {
	  case atrshmlog_strategy_discard :
	    {
	      ATRSHMLOGSTATLOCAL(g,counter_write0_discard);
	      
	      // we discard
	      return atrshmlog_error_write0_4;
	    }

	  case atrshmlog_strategy_spin_loop:
	    // we spin loop
	    break;

	  case atrshmlog_strategy_wait:
	    {
	      ATRSHMLOGSTATLOCAL(g,counter_write0_wait);
	      
	      // we wait fix time
	      ATRSHMLOG_SLEEP_NANOS(atrshmlog_strategy_wait_wait_time);
	    }
	    break;

	  case atrshmlog_strategy_adaptive:
	    {
	      ATRSHMLOGSTATLOCAL(g,counter_write0_adaptive);

	      int t = atomic_load_explicit(&atrshmlog_last_mem_to_shm, memory_order_acquire);
	      t = ATRSHMLOGSCALECLICKTONANO(t);

	      t /=  ATRSHMLOGTARGETBUFFERMAX;

#if 0
	      printf("adapotive %ld\n", (long)t);
#endif
	      
	      if (t > 999999999)
		t = 999999999;
	      
	      // adaptive : take the last transfer time and divide it
	      ATRSHMLOG_SLEEP_NANOS(t);
	    }
	    break;
	    
	  case atrshmlog_strategy_adaptive_fast:
	    {
	      ATRSHMLOGSTATLOCAL(g,counter_write0_adaptive_fast);

	      int t = atomic_load_explicit(&atrshmlog_last_mem_to_shm,memory_order_acquire);
	      t = ATRSHMLOGSCALECLICKTONANO(t);

	      t /=  ATRSHMLOGTARGETBUFFERMAX * 2;

#if 0
	      printf("adapotive %ld\n", (long)t);
#endif
	      
	      if (t > 999999999)
		t = 999999999;
	      
	      // adaptive fast : take the last transfer time and divide it 2
	      ATRSHMLOG_SLEEP_NANOS(t);
	    }
	    break;

	  case atrshmlog_strategy_adaptive_very_fast:
	    {
	      ATRSHMLOGSTATLOCAL(g,counter_write0_adaptive_very_fast);

	      // adaptive very fast : take the last transfer time and divide it 10
	      int t = atomic_load_explicit(&atrshmlog_last_mem_to_shm,memory_order_acquire);
	      t = ATRSHMLOGSCALECLICKTONANO(t);

	      t /=  ATRSHMLOGTARGETBUFFERMAX * 10;

#if 0
	      printf("adapotive %ld\n", (long)t);
#endif
	      
	      if (t > 999999999)
		t = 999999999;
	      
	      // adaptive fast : take the last transfer time and divide it 2
	      ATRSHMLOG_SLEEP_NANOS(t);
	    }
	    break;
	    
	  default:
	    // we spin loop
	    break;
	  }
      
      if (atrshmlog_logging_process_off_final)
	return atrshmlog_error_write0_5;

      atrshmlog_area_t * a_shm = ATRSHMLOG_GETAREA;

      /* Can be happen : end of logging anounced by user via flag in shm */
      if (atomic_load_explicit(&a_shm->ich_habe_fertig, memory_order_acquire) != 0) 
	return atrshmlog_error_write0_6;
      
      // There is no way to do the things follow for a dispatched buffer
      goto testagain_dispatched;
    }

  if (atrshmlog_thread_fence_2)
    atomic_thread_fence (memory_order_acquire);

  // Ok. We have an undispatched buffer 
  register size_t akindex = tbuff->size;

  // Is the size limit reached ? 
  if (akindex + totallen > tbuff->maxsize )
    {
#if ATRSHMLOGDEBUG == 1
      printf("size hit %ld %ld\n", (long)tbuff->id, (long)akindex);
#endif

      // Checkings: valid buffer 
      if (tbuff->safeguardfront != ATRSHMLOGSAFEGUARDVALUE) {
	ATRSHMLOGSTAT(atrshmlog_counter_write_safeguard);
	
	return atrshmlog_error_write0_7;
      }
      
      atrshmlog_area_t * a_shm = ATRSHMLOG_GETAREA;

      /* Bad thing. safeguard invalid */
      if (a_shm->shmsafeguard != ATRSHMLOGSAFEGUARDVALUE) {
	ATRSHMLOGSTAT(atrshmlog_counter_write_safeguard_shm);

	return atrshmlog_error_write0_8;
      }
  
      /* Can be happen : end of logging anounced by user via flag in shm */
      if (atomic_load_explicit(&a_shm->ich_habe_fertig, memory_order_acquire) != 0) 
	return atrshmlog_error_write0_9;

      tbuff->number_dispatched = g->number_dispatched++;

      tbuff->counter_write0 = g->counter_write0;

      tbuff->counter_write0_discard = g->counter_write0_discard;

      tbuff->counter_write0_wait = g->counter_write0_wait;

      tbuff->counter_write0_adaptive = g->counter_write0_adaptive;

      tbuff->counter_write0_adaptive_fast = g->counter_write0_adaptive_fast;

      tbuff->counter_write0_adaptive_very_fast = g->counter_write0_adaptive_very_fast;

      tbuff->counter_write1 = g->counter_write1;

      tbuff->counter_write1_discard = g->counter_write1_discard;

      tbuff->counter_write1_wait = g->counter_write1_wait;

      tbuff->counter_write1_adaptive = g->counter_write1_adaptive;

      tbuff->counter_write1_adaptive_fast = g->counter_write1_adaptive_fast;

      tbuff->counter_write1_adaptive_very_fast = g->counter_write1_adaptive_very_fast;

      tbuff->counter_write2 = g->counter_write2;

      tbuff->counter_write2_discard = g->counter_write2_discard;

      tbuff->counter_write2_wait = g->counter_write2_wait;

      tbuff->counter_write2_adaptive = g->counter_write2_adaptive;

      tbuff->counter_write2_adaptive_fast = g->counter_write2_adaptive_fast;

      tbuff->counter_write2_adaptive_very_fast = g->counter_write2_adaptive_very_fast;

      atrshmlog_dispatch_buffer(tbuff);

      // Switch the targetbuffer and try again
      g->atrshmlog_targetbuffer_index++;

      if (g->atrshmlog_targetbuffer_index >= ATRSHMLOGTARGETBUFFERMAX)
	g->atrshmlog_targetbuffer_index = 0;

      // End of full buffer handling
      goto testagain_dispatched;
    } 

  // Ok. we have an undispatched and not full buffer.
    
  register char* p = tbuff->b + akindex;

  // Transfer the log data
  memcpy(p, &i_starttime, sizeof(atrshmlog_time_t));
  memcpy(p + sizeof(atrshmlog_time_t), &i_endtime, sizeof(atrshmlog_time_t));
  memcpy(p + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_time_t), &totallen, sizeof(atrshmlog_int32_t));
  memcpy(p + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_int32_t) , &i_eventnumber, sizeof(atrshmlog_int32_t));
  memcpy(p + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_int32_t) + sizeof(atrshmlog_int32_t), &i_userflag, sizeof(atrshmlog_int32_t));

  *(p + ( ATRSHMLOGCONTROLDATASIZE - 1 ) )= (char)i_eventflag;

  akindex += totallen;

  tbuff->size = akindex;

  if (g->autoflush)
    {
      // Checkings: valid buffer 
      if (tbuff->safeguardfront != ATRSHMLOGSAFEGUARDVALUE) {
	ATRSHMLOGSTAT(atrshmlog_counter_write_safeguard);
	
	return atrshmlog_error_write0_7;
      }
      
      atrshmlog_area_t * a_shm = ATRSHMLOG_GETAREA;

      /* Bad thing. safeguard invalid */
      if (a_shm->shmsafeguard != ATRSHMLOGSAFEGUARDVALUE) {
	ATRSHMLOGSTAT(atrshmlog_counter_write_safeguard_shm);

	return atrshmlog_error_write0_8;
      }
  
      /* Can be happen : end of logging anounced by user via flag in shm */
      if (atomic_load_explicit(&a_shm->ich_habe_fertig, memory_order_acquire) != 0) 
	return atrshmlog_error_write0_9;

      tbuff->number_dispatched = g->number_dispatched++;

      tbuff->counter_write0 = g->counter_write0;

      tbuff->counter_write0_discard = g->counter_write0_discard;

      tbuff->counter_write0_wait = g->counter_write0_wait;

      tbuff->counter_write0_adaptive = g->counter_write0_adaptive;

      tbuff->counter_write0_adaptive_fast = g->counter_write0_adaptive_fast;

      tbuff->counter_write0_adaptive_very_fast = g->counter_write0_adaptive_very_fast;

      tbuff->counter_write1 = g->counter_write1;

      tbuff->counter_write1_discard = g->counter_write1_discard;

      tbuff->counter_write1_wait = g->counter_write1_wait;

      tbuff->counter_write1_adaptive = g->counter_write1_adaptive;

      tbuff->counter_write1_adaptive_fast = g->counter_write1_adaptive_fast;

      tbuff->counter_write1_adaptive_very_fast = g->counter_write1_adaptive_very_fast;

      tbuff->counter_write2 = g->counter_write2;

      tbuff->counter_write2_discard = g->counter_write2_discard;

      tbuff->counter_write2_wait = g->counter_write2_wait;

      tbuff->counter_write2_adaptive = g->counter_write2_adaptive;

      tbuff->counter_write2_adaptive_fast = g->counter_write2_adaptive_fast;

      tbuff->counter_write2_adaptive_very_fast = g->counter_write2_adaptive_very_fast;

      if (g->autoflush == 1)
	{
	  atrshmlog_dispatch_buffer(tbuff);

	  // Switch the targetbuffer and try again
	  g->atrshmlog_targetbuffer_index++;

	  if (g->atrshmlog_targetbuffer_index >= ATRSHMLOGTARGETBUFFERMAX)
	    g->atrshmlog_targetbuffer_index = 0;
	}
      else if (g->autoflush == 2)
	{
	  int rettm = atrshmlog_transfer_mem_to_shm(tbuff, g);
	}
    }
  
  return atrshmlog_error_ok;
}
