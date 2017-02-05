/***********************************************************************
*             This software is part of the atrshmlog package           *
*                   Copyright (c) 2016  ATRSoft GmbH                   *
*                      and is licensed under the                       *
*                     Apache License, Version 2.0                      *
*                         by ATRSoft GmbH                              *
*                                                                      *
*                A copy of the License is available at                 *
*             http://www.apache.org/licenses/LICENSE-2.0               *
*                                                                      *
*               Anton Rommerskirchen <atr@atrsoft.de>                  *
*                                                                      *
***********************************************************************/

/** \file atrshmlogc.c 
 * \brief The implementation of the module
 *
 * Today we only hold the thread local and its getter.
 * In case we do not have thread local we try the thread specific for 
 * pthread.
 *
 * The rest of the module is now in impls.
 */

/* our includes are here */

#include "atrshmlog_internal.h"

/* end of includes */

/********************************************************************/

#if ATRSHMLOG_THREAD_LOCAL == 1

/**
 * \brief The one struct containing all thread locals.
 *
 * We do this because the lookup of thread locals 
 * is relative expensive compared to the use of a 
 * simple struct pointer.
 *
 * So we put all those in one struct and use this from 
 * the initial get on.
 */
_Thread_local static atrshmlog_g_tl_t atrshmlog_g_tl = { .atrshmlog_idnotok = -1,
				     .atrshmlog_targetbuffer_arr = { 0 },
				     0
                                    };


/*******************************************************************/

/** 
 * \n Main code:
 *
 * \brief We get the thread locals adress of a thread
 *
 * We use this with the tid to identify the thread and its so called
 * g_tl struct.
 *
 * We can then switch logging off for the thread with a helper
 *
 * test t_get_tid.c
 */
volatile const void* atrshmlog_get_thread_locals_adress(void)
{
  return (volatile const void*)&atrshmlog_g_tl;
}

#endif

#if ATRSHMLOG_THREAD_LOCAL == 0

extern int atrshmlog_key_once;

// the hard way. we use pthreads key and specifics here
extern pthread_key_t atrshmlog_pthread_key;

// the destructor
extern void atrshmlog_destruct_specific(void* i_data);

// this is now a new thing. we use internal the key from  - hopeful - attach.
// then we check for it and get our data ptr.
// if no ptr is there we malloc and deliver the thing
// to the caller.
// we need a destroy here, but this is for later ...
volatile const void* atrshmlog_get_thread_locals_adress(void)
{
  // are we before attach ? 
  if (atrshmlog_key_once == 0)
    {
      int ret = pthread_key_create(&atrshmlog_pthread_key, atrshmlog_destruct_specific);

      ++atrshmlog_key_once; 

      if (ret != 0)
	{

	  // sorry pal - but we are simply out of options.

	  // no more tries
	
	  return 0;
	}
    }
  
  void *my_tls = pthread_getspecific(atrshmlog_pthread_key);

  if (my_tls == NULL)
    {
      atrshmlog_g_tl_t * p = (atrshmlog_g_tl_t*)calloc(1, sizeof(atrshmlog_g_tl_t));

      if (p)
	{
	  p->atrshmlog_idnotok = -1;

	  int ret = pthread_setspecific(atrshmlog_pthread_key, (const void*)p);

	  if (ret == 0)
	    {
	      // ok. we have at least now a valid buffer.
	      return (volatile const void*)p;
	    }
	  else
	    {
	      free(p);

	      return 0;
	    }
	}
      else
	{
	  return 0;
	}
    }
  else
    {
      return (volatile const void*)my_tls;
    }
}

#endif


/*******************************************************************/

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
#if ATRSHMLOG_THREAD_LOCAL == 1

  atrshmlog_g_tl_t* const g  = (atrshmlog_g_tl_t* )&atrshmlog_g_tl;

#endif
      
#if ATRSHMLOG_THREAD_LOCAL == 0

  
  atrshmlog_g_tl_t* const g  = (atrshmlog_g_tl_t* )atrshmlog_get_thread_locals_adress();
  

  // this can happen if pthread specific is in
  if (g == NULL)
    return  atrshmlog_error_write0_10;

#endif

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

  ATRSHMLOGSTATLOCAL(g,counter_write0);
  

  // Initialized

  volatile atrshmlog_area_t * const a_shm = ATRSHMLOG_GETAREA;

  // can happen - we are detached
  if (a_shm == 0)
    {
      return atrshmlog_error_write0_6;
    }
  
  /* Can be happen : end of logging anounced by user via flag in shm */
  if (a_shm->ich_habe_fertig != 0) 
    return atrshmlog_error_write0_6;
  
  // The hidden mechanism to get things minimised
  // in case we are bound to a layer for another language.
  if (((char)i_eventflag & ~0x20) == ATRSHMLOGPOINTINTIMEP)
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

  
  // Do the normal stuff now ...

  const atrshmlog_int32_t totallen = ATRSHMLOGCONTROLDATASIZE;

  int strategy_count = 0;
  
  int null_buffers = 0;
  
  // We use some goto jumping here, so this is a first target
 testagain_dispatched:

  ;
    
  register atrshmlog_tbuff_t* tbuff = g->atrshmlog_targetbuffer_arr[g->atrshmlog_targetbuffer_index];

  if (tbuff == NULL || atomic_load_explicit(&(tbuff->dispatched), memory_order_acquire) != 0)
    {
      ++g->atrshmlog_targetbuffer_index;
      
      if (g->atrshmlog_targetbuffer_index >= ATRSHMLOGTARGETBUFFERMAX)
	g->atrshmlog_targetbuffer_index = 0;

      if (tbuff == NULL)
	++null_buffers;

      ++strategy_count;

      if ((strategy_count % ATRSHMLOGTARGETBUFFERMAX) == 0)
	{
	  // no buffer available ...
	  if (null_buffers >= ATRSHMLOGTARGETBUFFERMAX)
	    return atrshmlog_error_write0_4;

	  null_buffers = 0;
	  
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
	}
      
      if (atrshmlog_logging_process_off_final)
	return atrshmlog_error_write0_5;

      // can happen - we are detached
      if (ATRSHMLOG_GETAREA == 0)
	{
	  return atrshmlog_error_write0_6;
	}
  
      /* Can be happen : end of logging anounced by user via flag in shm */
      if (a_shm->ich_habe_fertig != 0) 
	return atrshmlog_error_write0_6;
      
      // There is no way to do the things follow for a dispatched buffer
      goto testagain_dispatched;
    }

#if ATRSHMLOG_FENCE_2_OFF == 0  
  if (atrshmlog_thread_fence_2)
    atomic_thread_fence (memory_order_acquire);
#endif
  
  // Ok. We have an undispatched buffer 
  register size_t akindex = tbuff->size;

  // Is the size limit reached ? 
  if ((akindex + totallen) > tbuff->maxsize )
    {
#if ATRSHMLOGDEBUG == 1
      printf("size hit %ld %ld\n", (long)tbuff->id, (long)akindex);
#endif

      // Checkings: valid buffer 
      if (tbuff->safeguardfront != ATRSHMLOGSAFEGUARDVALUE) {
	ATRSHMLOGSTAT(atrshmlog_counter_write_safeguard);
	
	return atrshmlog_error_write0_7;
      }
      
      /* Bad thing. safeguard invalid */
      if (a_shm->shmsafeguard != ATRSHMLOGSAFEGUARDVALUE) {
	ATRSHMLOGSTAT(atrshmlog_counter_write_safeguard_shm);

	return atrshmlog_error_write0_8;
      }
  
      tbuff->number_dispatched = ++g->number_dispatched;

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
    
  tbuff->size += totallen;
    
  register char* p = tbuff->b + akindex;

  // Transfer the log data
  memcpy(p, &i_starttime, sizeof(atrshmlog_time_t));
  memcpy(p + sizeof(atrshmlog_time_t), &i_endtime, sizeof(atrshmlog_time_t));
  memcpy(p + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_time_t), &totallen, sizeof(atrshmlog_int32_t));
  memcpy(p + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_int32_t) , &i_eventnumber, sizeof(atrshmlog_int32_t));
  memcpy(p + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_int32_t) + sizeof(atrshmlog_int32_t), &i_userflag, sizeof(atrshmlog_int32_t));

  *(p + ( ATRSHMLOGCONTROLDATASIZE - 1 ) ) = (char)i_eventflag;

  if (g->autoflush)
    {
      // Checkings: valid buffer 
      if (tbuff->safeguardfront != ATRSHMLOGSAFEGUARDVALUE) {
	ATRSHMLOGSTAT(atrshmlog_counter_write_safeguard);
	
	return atrshmlog_error_write0_7;
      }
      
      // can happen - we are detached
      if (ATRSHMLOG_GETAREA == 0)
	{
	  return atrshmlog_error_write0_8;
	}
  
      /* Bad thing. safeguard invalid */
      if (a_shm->shmsafeguard != ATRSHMLOGSAFEGUARDVALUE) {
	ATRSHMLOGSTAT(atrshmlog_counter_write_safeguard_shm);

	return atrshmlog_error_write0_8;
      }
  
      tbuff->number_dispatched = ++g->number_dispatched;

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
	  ++g->atrshmlog_targetbuffer_index;

	  if (g->atrshmlog_targetbuffer_index >= ATRSHMLOGTARGETBUFFERMAX)
	    g->atrshmlog_targetbuffer_index = 0;
	}
      else if (g->autoflush == 2)
	{
	  int chksum = 0;

	  // we calc the checksum trivial. but this is ok for our needs
	  if (atrshmlog_checksum)
	    {
	      for (int k = 0; k < tbuff->size; ++k)
		{
		  chksum += tbuff->b[k];
		}
	    }

	  tbuff->chksum = chksum;
	  
	  int rettm = atrshmlog_transfer_mem_to_shm(tbuff, g);
	}
    }

  // we check the shared mem writer 
  if (a_shm->writerpid == g->atrshmlog_thread_pid)
    {
      // we use the lower 16 bit. the upper are bit flag to tell us what to do.
      // so we have 16 different things to do. and 64k sub values
      if ((a_shm->writerflag & ATRSHMLOG_WRITER_SLAVE) != 0)
	{
	  // ok. we want to change the number of slaves.
	  // a value is set from the lower 16 bits.
	  // so be carefull: you can start in theory 64 k slaves ....
	  int newslaves = (a_shm->writerflag & ATRSHMLOG_WRITER_SUB);

	  a_shm->writerflag = 0;

	  a_shm->writerpid = 0;
	  
	  int old = atrshmlog_set_f_list_buffer_slave_count(newslaves);

	  return atrshmlog_error_ok;
	}

      if ((a_shm->writerflag & ATRSHMLOG_WRITER_DETACH) != 0)
	{
	  // ok. we want to detach this one.
	  a_shm->writerflag = 0;

	  a_shm->writerpid = 0;
	  
	  atrshmlog_detach();

	  return atrshmlog_error_ok;
	}

      a_shm->writerflag = 0;

      a_shm->writerpid = 0;
    }
  
  return atrshmlog_error_ok;
}

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
 *
 * test t_write1.c
 */
atrshmlog_ret_t atrshmlog_write1(const atrshmlog_int32_t i_eventnumber,
				 const atrshmlog_int32_t i_eventflag,
				 const atrshmlog_int32_t i_userflag,
				 atrshmlog_time_t i_starttime,
				 atrshmlog_time_t i_endtime,
				 const void* i_local,
				 const atrshmlog_int32_t i_size)
{
#if ATRSHMLOG_THREAD_LOCAL == 1

  atrshmlog_g_tl_t* const g  = (atrshmlog_g_tl_t* )&atrshmlog_g_tl;

#endif
      
#if ATRSHMLOG_THREAD_LOCAL == 0

  atrshmlog_g_tl_t* const g  = (atrshmlog_g_tl_t* )atrshmlog_get_thread_locals_adress();

  // this can happen if pthread specific is in
  if (g == NULL)
    return atrshmlog_error_write1_13;

#endif

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

  ATRSHMLOGSTATLOCAL(g,counter_write1);

  // Initialized

  volatile atrshmlog_area_t * const a_shm = ATRSHMLOG_GETAREA;

  // can happen - we are detached
  if (a_shm == 0)
    {
      return atrshmlog_error_write1_8;
    }
  
  /* Can be happen : end of logging anounced by user via flag in shm */
  if (a_shm->ich_habe_fertig != 0) 
    return atrshmlog_error_write1_8;
  
  // The hidden mechanism to get things minimised
  // in case we are bound to a layer for another language.
  if (((char)i_eventflag & ~0x20) == ATRSHMLOGPOINTINTIMEP)
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

  // Do the normal stuff now ...

  /* This is a two way used one. Its the size and the flag too */
  size_t copy_local = i_size;

  /* If the pointer is null we set the length to 0 */
  if (!i_local)
    copy_local = 0;

  const atrshmlog_int32_t totallen = ATRSHMLOGCONTROLDATASIZE + copy_local;

  int strategy_count = 0;
  
  int null_buffers = 0;
  
    // We use some goto jumping here, so this is a first target
 testagain_dispatched:

  ;
    
  register atrshmlog_tbuff_t* tbuff = g->atrshmlog_targetbuffer_arr[g->atrshmlog_targetbuffer_index];

  if (tbuff == NULL || atomic_load_explicit(&(tbuff->dispatched), memory_order_acquire) != 0)
    {
      ++g->atrshmlog_targetbuffer_index;

      if (g->atrshmlog_targetbuffer_index >= ATRSHMLOGTARGETBUFFERMAX)
	g->atrshmlog_targetbuffer_index = 0;

      if (tbuff == NULL)
	++null_buffers;

      ++strategy_count;

      if ((strategy_count % ATRSHMLOGTARGETBUFFERMAX) == 0)
	{
	  // no buffer available ...
	  if (null_buffers >= ATRSHMLOGTARGETBUFFERMAX)
	    return  atrshmlog_error_write1_6;

	  null_buffers = 0;
	  
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
	}
      
      if (atrshmlog_logging_process_off_final)
	return atrshmlog_error_write1_7;

      // can happen - we are detached
      if (ATRSHMLOG_GETAREA == 0)
	{
	  return atrshmlog_error_write1_8;
	}
  
      /* Can be happen : end of logging anounced by user via flag in shm */
      if (a_shm->ich_habe_fertig != 0) 
	return atrshmlog_error_write1_8;
      
      // There is no way to do the things follow for a dispatched buffer
      goto testagain_dispatched;
    }

#if ATRSHMLOG_FENCE_2_OFF == 0  
  if (atrshmlog_thread_fence_2)
    atomic_thread_fence (memory_order_acquire);
#endif
  
  // special case for the payload thing
  if ((unsigned int)totallen > tbuff->maxsize) {
    ATRSHMLOGSTAT(atrshmlog_counter_write1_abort7);
    
    return atrshmlog_error_write1_9; // Cannot work.
  }
  
  // Ok. We have an undispatched buffer 
  register size_t akindex = tbuff->size;

  // Is the size limit reached ? 
  if ((akindex + totallen) > tbuff->maxsize )
    {
#if ATRSHMLOGDEBUG == 1
      printf("size hit %ld %ld\n", (long)tbuff->id, (long)tbuff->size);
#endif
	 
      // Checkings: valid buffer 
      if (tbuff->safeguardfront != ATRSHMLOGSAFEGUARDVALUE) {
	ATRSHMLOGSTAT(atrshmlog_counter_write_safeguard);

	return atrshmlog_error_write1_10;
      }

      // can happen - we are detached
      if (ATRSHMLOG_GETAREA == 0)
	{
	  return atrshmlog_error_write1_8;
	}
  
      /* Bad thing. Safeguard invalid */
      if (a_shm->shmsafeguard != ATRSHMLOGSAFEGUARDVALUE) {
	ATRSHMLOGSTAT(atrshmlog_counter_write_safeguard_shm);

	return atrshmlog_error_write1_11;
      }
      
      tbuff->number_dispatched = ++g->number_dispatched;

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
      ++g->atrshmlog_targetbuffer_index;

      if (g->atrshmlog_targetbuffer_index >= ATRSHMLOGTARGETBUFFERMAX)
	g->atrshmlog_targetbuffer_index = 0;

      // End of full buffer handling
      goto testagain_dispatched;
    } 

  // Ok. We have an undispatched and not full buffer.

  tbuff->size += totallen;
    
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
  
  if (g->autoflush)
    {
      // Checkings: valid buffer 
      if (tbuff->safeguardfront != ATRSHMLOGSAFEGUARDVALUE) {
	ATRSHMLOGSTAT(atrshmlog_counter_write_safeguard);
	
	return atrshmlog_error_write0_7;
      }
      
      // can happen - we are detached
      if (ATRSHMLOG_GETAREA == 0)
	{
	  return atrshmlog_error_write1_8;
	}
  
      /* Bad thing. safeguard invalid */
      if (a_shm->shmsafeguard != ATRSHMLOGSAFEGUARDVALUE) {
	ATRSHMLOGSTAT(atrshmlog_counter_write_safeguard_shm);

	return atrshmlog_error_write1_11;
      }
  
      tbuff->number_dispatched = ++g->number_dispatched;

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
	  ++g->atrshmlog_targetbuffer_index;

	  if (g->atrshmlog_targetbuffer_index >= ATRSHMLOGTARGETBUFFERMAX)
	    g->atrshmlog_targetbuffer_index = 0;
	}
      else if (g->autoflush == 2)
	{
	  int chksum = 0;

	  // we calc the checksum trivial. but this is ok for our needs
	  if (atrshmlog_checksum)
	    {
	      for (int k = 0; k < tbuff->size; ++k)
		{
		  chksum += tbuff->b[k];
		}
	    }

	  tbuff->chksum = chksum;
	  
	  int rettm = atrshmlog_transfer_mem_to_shm(tbuff, g);
	}
    }
  
  // we check the shared mem writer 
  if (a_shm->writerpid == g->atrshmlog_thread_pid)
    {
      // we use the lower 16 bit. the upper are bit flag to tell us what to do.
      // so we have 16 different things to do. and 64k sub values
      if ((a_shm->writerflag & ATRSHMLOG_WRITER_SLAVE) != 0)
	{
	  // ok. we want to change the number of slaves.
	  // a value is set from the lower 16 bits.
	  // so be carefull: you can start in theory 64 k slaves ....
	  int newslaves = (a_shm->writerflag & ATRSHMLOG_WRITER_SUB);

	  a_shm->writerflag = 0;

	  a_shm->writerpid = 0;
	  
	  int old = atrshmlog_set_f_list_buffer_slave_count(newslaves);

	  return atrshmlog_error_ok;
	}

      if ((a_shm->writerflag & ATRSHMLOG_WRITER_DETACH) != 0)
	{
	  // ok. we want to detach this one.
	  a_shm->writerflag = 0;

	  a_shm->writerpid = 0;
	  
	  atrshmlog_detach();

	  return atrshmlog_error_ok;
	}

      a_shm->writerflag = 0;

      a_shm->writerpid = 0;
    }

  return atrshmlog_error_ok;
}

/** 
 * \n Main code:
 *
 * \brief We write to the log .
 *
 * This is the major writer. 
 * This is a special version for our main audiance.
 * Its for the argv array approch found in so many c 
 * programs.
 * So this is somehow broken for info that tries to use
 * null bytes. 
 * Sorry, but this is how c works.
 * If you need fancy info with null you have to put it in the 
 * first payload buffer.
 *
 * test t_write2.c
 */
atrshmlog_ret_t atrshmlog_write2(const atrshmlog_int32_t i_eventnumber,
				 const atrshmlog_int32_t i_eventflag,
				 const atrshmlog_int32_t i_userflag,
				 atrshmlog_time_t i_starttime,
				 atrshmlog_time_t i_endtime,
				 const void* i_local,
				 const atrshmlog_int32_t i_size,
				 const char* i_argv[],
				 const atrshmlog_int32_t i_argc_hint)
{
#if ATRSHMLOG_THREAD_LOCAL == 1

  atrshmlog_g_tl_t* const g  = (atrshmlog_g_tl_t* )&atrshmlog_g_tl;

#endif
      
#if ATRSHMLOG_THREAD_LOCAL == 0

  atrshmlog_g_tl_t* const g  = (atrshmlog_g_tl_t* )atrshmlog_get_thread_locals_adress();

  // this can happen if pthread specific is in
  if (g == NULL)
    return atrshmlog_error_write2_13;

#endif

  /* We use this flag also as the initialized flag
   * So we have all the initialize stuff hereafter 
   */
  if (g->atrshmlog_idnotok != 0)
    {
      if (g->atrshmlog_idnotok != -1) {
	ATRSHMLOGSTAT(atrshmlog_counter_write2_abort3);
	
	return atrshmlog_error_write2_5;
      }
      
      int ret = atrshmlog_init_in_write(g);

      if (ret != 0)
	{
	  ATRSHMLOGSTAT(atrshmlog_counter_write2_abort4);

	  return ret;
	}
    }

  ATRSHMLOGSTATLOCAL(g,counter_write2);

  // Initialized

  volatile atrshmlog_area_t * const a_shm = ATRSHMLOG_GETAREA;

  // can happen - we are detached
  if (a_shm == 0)
    {
      return atrshmlog_error_write2_8;
    }
  
  /* Can be happen : end of logging anounced by user via flag in shm */
  if (a_shm->ich_habe_fertig != 0) 
    return atrshmlog_error_write2_8;
  
  // The hidden mechanism to get things minimised
  // in case we are bound to a layer for another language.
  if (((char)i_eventflag & ~0x20) == ATRSHMLOGPOINTINTIMEP)
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

  // this is a bit a compromise now.
  // in a later version i try to move the argv directly,
  // so we spare time and space. But for now
  // we use a helper here ...
  char atrshmlog_argvbuffer[ATRSHMLOGARGVBUFFERLEN];
  
  /* This is a two way used one. Its the size and the flag too */
  size_t copy_local = i_size;

  /* If the pointer is null we set the length to 0 */
  if (!i_local)
    copy_local = 0;

  /* We collect at max the length of the argv.
   * Having more data in the argv is simply ignored, the max is the buffer
   * len. No more 
   * So its possible to use arbitrary number of argv, but only 
   * the first of them go into the buffer till the length limit is hit.
   */
    
  int argvbufferlen = 0;
    
  if (i_argv)
    goto doargv;
    
 afterargv:
  ;


    
  // Do the normal stuff now ...

  const atrshmlog_int32_t totallen = ATRSHMLOGCONTROLDATASIZE + argvbufferlen + copy_local;

  int strategy_count = 0;
  
  int null_buffers = 0;
  
  // We use some goto jumping here, so this is a first target
 testagain_dispatched:

  ;
    
  register atrshmlog_tbuff_t* tbuff = g->atrshmlog_targetbuffer_arr[g->atrshmlog_targetbuffer_index];

  if (tbuff == NULL || atomic_load_explicit(&(tbuff->dispatched), memory_order_acquire) != 0)
    {
      ++g->atrshmlog_targetbuffer_index;

      if (g->atrshmlog_targetbuffer_index >= ATRSHMLOGTARGETBUFFERMAX)
	g->atrshmlog_targetbuffer_index = 0;

      if (tbuff == NULL)
	++null_buffers;

      ++strategy_count;

      if ((strategy_count % ATRSHMLOGTARGETBUFFERMAX) == 0)
	{
	  // no buffer available ...
	  if (null_buffers >= ATRSHMLOGTARGETBUFFERMAX)
	    return atrshmlog_error_write2_6;

	  null_buffers = 0;
	  
	  switch (g->strategy)
	    {
	    case atrshmlog_strategy_discard :
	      {
		ATRSHMLOGSTATLOCAL(g,counter_write2_discard);
	      
		// we discard
		return atrshmlog_error_write2_6;
	      }
	    
	    case atrshmlog_strategy_spin_loop:
	      // we spin loop
	      break;

	    case atrshmlog_strategy_wait:
	      {
		ATRSHMLOGSTATLOCAL(g,counter_write2_wait);

		// we wait fix time
		ATRSHMLOG_SLEEP_NANOS(atrshmlog_strategy_wait_wait_time);
	      }
	      break;

	    case atrshmlog_strategy_adaptive:
	      {
		ATRSHMLOGSTATLOCAL(g,counter_write2_adaptive);

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
		ATRSHMLOGSTATLOCAL(g,counter_write2_adaptive_fast);

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
		ATRSHMLOGSTATLOCAL(g,counter_write2_adaptive_very_fast);

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
	}
      
      if (atrshmlog_logging_process_off_final)
	return atrshmlog_error_write2_7;

      // can happen - we are detached
      if (ATRSHMLOG_GETAREA == 0)
	{
	  return atrshmlog_error_write2_8;
	}
  
      /* Can be happen : end of logging anounced by user via flag in shm */
      if (a_shm->ich_habe_fertig != 0) 
	return atrshmlog_error_write2_8;
      
      // There is no way to do the things follow for a dispatched buffer
      goto testagain_dispatched;
    }

#if ATRSHMLOG_FENCE_2_OFF == 0  
  if (atrshmlog_thread_fence_2)
    atomic_thread_fence (memory_order_acquire);
#endif
  
  // special case for the argv thing
  if ((unsigned int)totallen > tbuff->maxsize) {
    ATRSHMLOGSTAT(atrshmlog_counter_write2_abort7);
    
    return atrshmlog_error_write2_9; // Cannot work.
  }
  
  // Ok. We have an undispatched buffer 
  register size_t akindex = tbuff->size;

  // Is the size limit reached ? 
  if ((akindex + totallen) > tbuff->maxsize )
    {
      // Checkings: valid buffer 
      if (tbuff->safeguardfront != ATRSHMLOGSAFEGUARDVALUE) {
	ATRSHMLOGSTAT(atrshmlog_counter_write_safeguard);

	return atrshmlog_error_write2_10;
      }
      
      // can happen - we are detached
      if (ATRSHMLOG_GETAREA == 0)
	{
	  return atrshmlog_error_write2_8;
	}
  
      /* Bad thing. Safeguard invalid */
      if (a_shm->shmsafeguard != ATRSHMLOGSAFEGUARDVALUE) {
	ATRSHMLOGSTAT(atrshmlog_counter_write_safeguard_shm);

	return atrshmlog_error_write2_11;
      }
      
      tbuff->number_dispatched = ++g->number_dispatched;

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
      ++g->atrshmlog_targetbuffer_index;

      if (g->atrshmlog_targetbuffer_index >= ATRSHMLOGTARGETBUFFERMAX)
	g->atrshmlog_targetbuffer_index = 0;

      // End of full buffer handling
      goto testagain_dispatched;
    }

  // Ok. We have an undispatched and not full buffer.
    
  tbuff->size += totallen;
    
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
    
  if (argvbufferlen)
    {
      memcpy(p + ATRSHMLOGCONTROLDATASIZE + copy_local, atrshmlog_argvbuffer, argvbufferlen);
    }

  if (g->autoflush)
    {
      // Checkings: valid buffer 
      if (tbuff->safeguardfront != ATRSHMLOGSAFEGUARDVALUE) {
	ATRSHMLOGSTAT(atrshmlog_counter_write_safeguard);
	
	return atrshmlog_error_write2_7;
      }

      // can happen - we are detached
      if (ATRSHMLOG_GETAREA == 0)
	{
	  return atrshmlog_error_write2_8;
	}
  
      /* Bad thing. safeguard invalid */
      if (a_shm->shmsafeguard != ATRSHMLOGSAFEGUARDVALUE) {
	ATRSHMLOGSTAT(atrshmlog_counter_write_safeguard_shm);

	return atrshmlog_error_write2_11;
      }
  
      tbuff->number_dispatched = ++g->number_dispatched;

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
	  ++g->atrshmlog_targetbuffer_index;

	  if (g->atrshmlog_targetbuffer_index >= ATRSHMLOGTARGETBUFFERMAX)
	    g->atrshmlog_targetbuffer_index = 0;
	}
      else if (g->autoflush == 2)
	{
	  int chksum = 0;

	  // we calc the checksum trivial. but this is ok for our needs
	  if (atrshmlog_checksum)
	    {
	      for (int k = 0; k < tbuff->size; ++k)
		{
		  chksum += tbuff->b[k];
		}
	    }

	  tbuff->chksum = chksum;
	  
	  int rettm = atrshmlog_transfer_mem_to_shm(tbuff, g);
	}
    }

  // we check the shared mem writer 
  if (a_shm->writerpid == g->atrshmlog_thread_pid)
    {
      // we use the lower 16 bit. the upper are bit flag to tell us what to do.
      // so we have 16 different things to do. and 64k sub values
      if ((a_shm->writerflag & ATRSHMLOG_WRITER_SLAVE) != 0)
	{
	  // ok. we want to change the number of slaves.
	  // a value is set from the lower 16 bits.
	  // so be carefull: you can start in theory 64 k slaves ....
	  int newslaves = (a_shm->writerflag & ATRSHMLOG_WRITER_SUB);

	  a_shm->writerflag = 0;

	  a_shm->writerpid = 0;
	  
	  int old = atrshmlog_set_f_list_buffer_slave_count(newslaves);

	  return atrshmlog_error_ok;
	}

      if ((a_shm->writerflag & ATRSHMLOG_WRITER_DETACH) != 0)
	{
	  // ok. we want to detach this one.
	  a_shm->writerflag = 0;

	  a_shm->writerpid = 0;
	  
	  atrshmlog_detach();

	  return atrshmlog_error_ok;
	}

      a_shm->writerflag = 0;

      a_shm->writerpid = 0;
    }

  return atrshmlog_error_ok;
  
  /************************/  
 doargv:
  {
    int index = 0;
    
    char* dest = atrshmlog_argvbuffer;

    char* limit = dest + ATRSHMLOGARGVBUFFERLEN;
        
    if (i_argc_hint <= 0 )
      {
	for (const char *p = i_argv[0];
	     p;
	     p = i_argv[++index])
	  {
	    if (dest == limit)
	      {
		goto argvtransfered0;
	      }
            
	    *dest++ = (char)atrshmlog_delimiter;
        
	    int len  = strlen(p);
            
	    if (len > limit - dest)
	      {
		len = limit - dest;
	      }
            
	    memcpy(dest, p, len);
            
	    dest += len;
	    /* count_memcpy++; */
	  }
      argvtransfered0:
	argvbufferlen = dest - atrshmlog_argvbuffer;
      }
    else
      {
	for (const char *p = i_argv[0];
	     p && (index < i_argc_hint);
	     p = i_argv[++index])
	  {
	    if (dest == limit)
	      {
		goto argvtransfered1;
	      }
            
	    *dest++ = (char)atrshmlog_delimiter;
            
	    int len  = strlen(p);
            
	    if (len > limit - dest)
	      {
		len = limit - dest;
	      }
        
	    memcpy(dest, p, len);
        
	    dest += len;
	    /*count_memcpy++; */
	  }

      argvtransfered1:
	argvbufferlen = dest - atrshmlog_argvbuffer;
      }
  }
  goto afterargv;
}

/* end of file */
