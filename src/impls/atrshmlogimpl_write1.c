
#include "../atrshmlog_internal.h"


/*******************************************************************/

/**
 * \file atrshmlogimpl_write1.c
 */

/** 
 * \n Main code:
 *
 * \brief We write to the log. 
 *
 * This is the second major writer. 
 * This is the write with one payload buffer.
 * We can put any kind of info on the payload here
 * which gives us a simple way to handle all those 
 * language info forms. 
 * For this to work
 * we only need start and length of the used info.
 * This is then transfered binary.
 * The first thing you need to do is then to clone the convert and
 * hack into it a way to interpret the binary.
 * The default is using the data as text in a c program.
 * See the java example for this kind of stuff.
 */
atrshmlog_ret_t atrshmlog_write1(const atrshmlog_int32_t i_eventnumber,
				 const atrshmlog_int32_t i_eventflag,
				 const atrshmlog_int32_t i_userflag,
				 atrshmlog_time_t i_starttime,
				 atrshmlog_time_t i_endtime,
				 const void* i_local,
				 const atrshmlog_int32_t i_size)
{
  atrshmlog_g_tl_t* g  = (atrshmlog_g_tl_t* )atrshmlog_get_thread_locals_adress();

#if ATRSHMLOG_THREAD_LOCAL == 0

  // this can happen if pthread specific is in
  if (g == NULL)
    return atrshmlog_error_write1_13;

#endif

  ATRSHMLOGSTATLOCAL(g,counter_write1);

  /* This is a two way used one. Its the size and the flag too */
  int copy_local = i_size;

  /* No logging , wrong parameter ... */
  if (i_size < 0) {
    ATRSHMLOGSTAT(atrshmlog_counter_write1_abort5);
    
    return atrshmlog_error_write1_1;
  }
  
  /* If the pointer is null we set the length to 0 */
  if (!i_local)
    copy_local = 0;

  if (i_eventnumber < 0) {
    ATRSHMLOGSTAT(atrshmlog_counter_write1_abort1);
    
    return atrshmlog_error_write1_2;
  }

  if (i_eventnumber >= atrshmlog_event_locks_max) {
    ATRSHMLOGSTAT(atrshmlog_counter_write1_abort2);
    
    return atrshmlog_error_write1_3;
  }
  
  if (i_size >= ATRSHMLOGBUFFER_INFOSIZE - ATRSHMLOGCONTROLDATASIZE - 1)
    {
      ATRSHMLOGSTAT(atrshmlog_counter_write1_abort6);
      // Cannot work.
      return atrshmlog_error_write1_4;
    }

  /* We use this flag also as the initialized flag
   * So we have all the initialize stuff hereafter 
   */
  if (g->atrshmlog_idnotok != 0)
    {
      if (g->atrshmlog_idnotok != -1) {
	ATRSHMLOGSTAT(atrshmlog_counter_write1_abort3);
	
	return atrshmlog_error_write1_5;
      }
      
      int ret = atrshmlog_init_in_write(g);

      if (ret != 0)
	{
	  ATRSHMLOGSTAT(atrshmlog_counter_write1_abort4);

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

  totallen = ATRSHMLOGCONTROLDATASIZE + copy_local;

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
	      ATRSHMLOGSTATLOCAL(g,counter_write1_discard);
	      
 	      // we discard
	      return atrshmlog_error_write1_6;
	    }

	  case atrshmlog_strategy_spin_loop:
	    // we spin loop
	    break;

	  case atrshmlog_strategy_wait:
	    {
	      ATRSHMLOGSTATLOCAL(g,counter_write1_wait);
	      
	      // we wait fix time
	      ATRSHMLOG_SLEEP_NANOS(atrshmlog_strategy_wait_wait_time);
	    }
	    break;

	  case atrshmlog_strategy_adaptive:
	    {
	      ATRSHMLOGSTATLOCAL(g,counter_write1_adaptive);

	      int t = atomic_load_explicit(&atrshmlog_last_mem_to_shm,memory_order_acquire);
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
	      ATRSHMLOGSTATLOCAL(g,counter_write1_adaptive_fast);

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
	      ATRSHMLOGSTATLOCAL(g,counter_write1_adaptive_very_fast);

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
	return atrshmlog_error_write1_7;

      atrshmlog_area_t * a_shm = ATRSHMLOG_GETAREA;

      /* Can be happen : end of logging anounced by user via flag in shm */
      if (atomic_load_explicit(&a_shm->ich_habe_fertig, memory_order_acquire) != 0) 
	return atrshmlog_error_write1_8;
      
      // There is no way to do the things follow for a dispatched buffer
      goto testagain_dispatched;
    }

  if (atrshmlog_thread_fence_2)
    atomic_thread_fence (memory_order_acquire);

  // special case for the payload thing
  if (totallen > tbuff->maxsize) {
    ATRSHMLOGSTAT(atrshmlog_counter_write1_abort7);
    
    return atrshmlog_error_write1_9; // Cannot work.
  }
  
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

	return atrshmlog_error_write1_10;
      }

      atrshmlog_area_t * a_shm = ATRSHMLOG_GETAREA;

      /* Bad thing. Safeguard invalid */
      if (a_shm->shmsafeguard != ATRSHMLOGSAFEGUARDVALUE) {
	ATRSHMLOGSTAT(atrshmlog_counter_write_safeguard_shm);

	return atrshmlog_error_write1_11;
      }
      
      /* Can be happen : end of logging anounced by user via flag in shm */
      if (atomic_load_explicit(&a_shm->ich_habe_fertig, memory_order_acquire) != 0) 
	return atrshmlog_error_write1_12;

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

  // Ok. We have an undispatched and not full buffer.
  //  s = ATRSHMLOG_GET_TSC_CALL();
    
  register char* p = tbuff->b + akindex;

  // Transfer the log data
  memcpy(p, &i_starttime, sizeof(atrshmlog_time_t));
  memcpy(p + sizeof(atrshmlog_time_t), &i_endtime, sizeof(atrshmlog_time_t));
  memcpy(p + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_time_t), &totallen, sizeof(atrshmlog_int32_t));
  memcpy(p + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_int32_t) , &i_eventnumber, sizeof(atrshmlog_int32_t));
  memcpy(p + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_int32_t) + sizeof(atrshmlog_int32_t), &i_userflag, sizeof(atrshmlog_int32_t));

  *(p + ( ATRSHMLOGCONTROLDATASIZE - 1 ) )= (char)i_eventflag;

  if (copy_local)
    {
      memcpy(p + ATRSHMLOGCONTROLDATASIZE, i_local, copy_local);
    }

  akindex += totallen;

  tbuff->size = akindex;

  return atrshmlog_error_ok;
}

