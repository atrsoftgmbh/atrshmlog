
#include "../atrshmlog_internal.h"

#include <stdio.h>

#include <string.h>

#include <stdlib.h>

/**
 * \file atrshmlogimpl_attach.c
 */


// this is for openbsd 6.0 for now.

#if ATRSHMLOG_THREAD_LOCAL == 0

int atrshmlog_key_once = 0;

pthread_key_t atrshmlog_pthread_key;

void atrshmlog_destruct_specific(void* i_data)
{
  if (i_data != NULL)
    free(i_data);

  return ;
}

#endif

/**
 * helper. we putenv
 */
void atrshmlog_putenv(const char* i_suffix, long val)
{
#if ATRSHMLOG_PUTENV_IN_ATTACH == 1
  // why ?
  // why do we leak memory here ?
  // the answer is : perl
  // they assume the thing is allocated.
  // so they simply do a free.
  // and so we need to alloc the memory ourself.
  // if you really need the putenv - then you have to pay this price
  // or the perl layer will not work - at least on my platform.
  char *b = (char*)calloc(1,512);

  snprintf(b, 512, "%s%s=%ld", atrshmlog_prefix_name_buffer, i_suffix, val);
  
  putenv(b);
#endif
}

/*
 * helper: we get a value from env / files and set it
 */
void atrshmlog_init(const char *i_suffix,
		    int *v,
		    int i_min,
		    int i_max,
		    int in_env)
{
  if (in_env)
    atrshmlog_init_via_env(i_suffix,
			   v,
			   i_min,
			   i_max);
  else
      atrshmlog_init_via_file(i_suffix,
			      v,
			      i_min,
			      i_max);
}

/**
 * the parameters for init
 */
struct atrshmlog_init_param_s
{
  const char* c;
  int* v;
  int minimum;
  int maximum;
};

typedef struct atrshmlog_init_param_s atrshmlog_init_param_t;

/*******************************************************************/

/* We start real used functions here with linkage.
 *
 * So these functions are linkable from the user .
 *
 * We can use them in a c and c++ program directly.
 * 
 * For others we need a kind of layer.
 *
 * See the java example for this.
 *
 * All functions with linkage have the parameters documented in the header. 
 */


/**
 * We need a safe guard against race conditions 
 */
_Alignas(128) static atomic_flag atrshmlog_attach_once_flag = ATOMIC_FLAG_INIT;

/**
 * \n Main code:
 *
 * \brief The attach to shm memory function - and initialize.
 *
 * Attach the buffer if logging is wanted and the buffer is not
 * attached before.
 *
 * Set the global prefix_name_buffer if not alredy set by the user.
 *
 * What has to be set before  : 
 *
 * Every setting the program wants to do before the thing starts
 * logging.
 *
 * Technical you can set things like sizes, counts and so on ...
 * Use the functions or use the env variables.
 *
 * With the call to attach you - should - no longer change the
 * loggings parameters - but you can change some of them.
 *
 * When you have made the changes then attach uses some of them
 * to do the important things like starting slaves.
 *
 * One thing that should not be changed after attach is the number of events.
 *
 * So check the code to see if a thing can be changed after.
 *
 * test t_attach.c
 */
atrshmlog_ret_t atrshmlog_attach(void)
{
  ATRSHMLOGSTAT(atrshmlog_counter_attach);

  // are we a reattch after a detach ?
  int reattach = (atrshmlog_id != 0);
  
  /* We attach the shm buffer. 
   * First we have to check for the env var and then for the
   * extern ptr  that holds the log base.
   * If the ptr is null we attach.
   * If the env is null we try the file atrshmlogshmid.txt
   * If we still have no hit we give up .
   * If env is ok, but if is 0 we give up.
   *
   * If the env is not set we set the ptr to null to prevent 
   * logging in case we have NOT been a top level process
   * and the log is already in a predecessor process open.
   *
   * Later on we can change this for getting subprocesses
   * to log too...
   */

  // we lock. this is needed to prevent races...
  while (atomic_flag_test_and_set(&atrshmlog_attach_once_flag) )
	;

  // one enters this first - and only one
  int result = atrshmlog_error_attach_2;

  /*
   * This is the first big catch.
   * We check for already attached.
   *
   * This is done simple by using the static here.
   *
   * Question: How are we already initialized ?
   *
   * Answer:
   * Hm this is not so simple.
   * We can be the second call or so ..
   * Or we are made but there was a fork before and the image 
   * of the process still has the attach memory.
   * So we make this from the value of attach once.
   * 
   */
  if (atrshmlog_attach_once != 0)
    {
      /*
       * We clear the lock.
       */
      atomic_flag_clear(&atrshmlog_attach_once_flag);

      return atrshmlog_error_attach_1;
    }

#if ATRSHMLOG_THREAD_LOCAL == 0
  // this is for openbsd. we need something similar to thread local
  // but they don't hve it for now.
  // so we use the function to get it in fact as a factory.
  // and this needs a call for pthread_getspecific.
  // so we need a valid key here.
  // and this is the deal : if you try it without first making the attach
  // then BUMMM

  if (atrshmlog_key_once == 0)
    {
      int ret = pthread_key_create(&atrshmlog_pthread_key, atrshmlog_destruct_specific);

      ++atrshmlog_key_once; 

      if (ret != 0)
	{
	  
	  // sorry pal - but we are simply out of options.

	  // We clear the lock.
	  atomic_flag_clear(&atrshmlog_attach_once_flag);
	  
	  return atrshmlog_error_attach_7;
	}
    }

  // ok. we have a key. now its up to the user to use the function...
  
#endif
  if (!reattach)
    {
      /* Second thing after the init of vars : make timestamp */
      atrshmlog_inittimetsc_before = atrshmlog_get_clicktime();
      
      atrshmlog_inittime = atrshmlog_get_realtime();

      atrshmlog_inittimetsc_after = atrshmlog_get_clicktime();

    }
  
  atrshmlog_id = 0;
    
  long int lshmid = 0;

  int in_env = 0;
    
  /* 
   * This does two things : first it initializes the prefix_name_buffer
   * with the value of default and if the ATRSHMLOG is set then this 
   * string value.
   * Then it uses the prefix with the suffix to get the env
   * value for this.
   *
   * Then the thing is used to make the shmid to identify 
   * the attaching buffer....
   *
   * See the code for details
   */
  const char *shmid_str = ATRSHMLOG_GET_ENV_SHMID();
      
  /* We check now for the shmid text : is text set we never do the file hings */
  if (shmid_str != 0)
    {
      int base = 10; 
      char *endptr;
      
      lshmid = strtol(shmid_str, &endptr, base);

      in_env = 1; // ok. we have at least a valid env. no fileops from here on
    }
  else
    {
      // we try the file approch
      char buff[255];

#if ATRSHMLOG_USE_SAFER_COPY == 1
      strlcpy(buff, atrshmlog_prefix_name_buffer, 255);
      strlcat(buff, ATRSHMLOGENVSUFFIX, 255);
      strlcat(buff, ".TXT", 255);
#else
      strcat(strcat(strcpy(buff, atrshmlog_prefix_name_buffer), ATRSHMLOGENVSUFFIX),".TXT");
#endif
      
      FILE *f = fopen(buff, "r");

      if (f)
	{
	  if (1 != fscanf(f,"%ld", &lshmid))
	    {
	      fclose(f);

	      lshmid = 0;

	      result = atrshmlog_error_attach_4;

	      goto leave;
	    }

	  /* We have a number. Try to connect now */
	  fclose(f);
	}
      else
	{
	  // neither nor ...
	  result = atrshmlog_error_attach_4;
	  
	  goto leave;
	}
    }
  
  /* Ok. We have it, and we think its ok if its not false info. 
   * a valid number not 0 is good enough for now
   */
  if (lshmid != 0)
    {
      /* This is looking good .
       * We try to attach now.
       */
      int shmid = (int)lshmid;
		
      void*shmaddr = (void*)0; /* We let decide the os */ 
		
# if ATRSHMLOG_PLATFORM_LINUX_X86_64_GCC == 1

      int shmflg = 0; 
		
      void *shmat_result = shmat(shmid, shmaddr, shmflg);

#endif
	  
# if ATRSHMLOG_PLATFORM_CYGWIN_X86_64_GCC == 1

      int shmflg = 0; 
		
      void *shmat_result = shmat(shmid, shmaddr, shmflg);

#endif

# if ATRSHMLOG_PLATFORM_MINGW_X86_64_GCC == 1


      int envsize;

      atrshmlog_init(ATRSHMLOG_BUFFER_COUNT_SUFFIX,
		     &envsize,
		     0,
		     4096,
		     in_env);
	  
      if (envsize < 1)
	{
	  envsize = 8;
	}
	  
      const int save = 256 ;
  
      size_t wantedsize = sizeof (atrshmlog_area_t) + (envsize - ATRSHMLOGBUFFER_MINCOUNT) * sizeof( atrshmlog_buffer_t) + (envsize * ATRSHMLOGBUFFER_INFOSIZE + save);

      int size = wantedsize;
	    
      void *shmat_result = atrshmlog_attach_mapped_file(shmid,size);

#endif

# if ATRSHMLOG_PLATFORM_BSD_AMD64_CLANG == 1

      int shmflg = 0; 
		
      void *shmat_result = shmat(shmid, shmaddr, shmflg);

#endif

# if ATRSHMLOG_PLATFORM_BSD_AMD64_GCC == 1

      int shmflg = 0; 
		
      void *shmat_result = shmat(shmid, shmaddr, shmflg);

#endif

# if ATRSHMLOG_PLATFORM_SOLARIS_X86_64_GCC == 1

      int shmflg = 0; 
		
      void *shmat_result = shmat(shmid, shmaddr, shmflg);

#endif
	  
      if (shmat_result != (void*)-1)
	{
	  atrshmlog_base_ptr = shmat_result;

	  atrshmlog_id = shmid;

	  result = atrshmlog_error_ok;
	      
	  atrshmlog_putenv(ATRSHMLOGENVSUFFIX, lshmid);

	  static int onoff;
	      
	  static int new_max;

	  static int my_clock_id;
	  
	  static atrshmlog_init_param_t atrshmlog_init_data[] =
	    {
	      /* 
	       * The advance init thing. if we need it we do it here
	       */
	      {
		ATRSHMLOGINITINADVANCESUFFIX,
		&atrshmlog_init_buffers_in_advance,
		0,
		1
	      },

	      /* 
	       * The buffer acquire strategy for shm buffers.
	       * This has been not used now, but evtl we need it
	       * again on the reader side. Then we can use it here
	       * to make a better decision on the producer side.
	       * So it is used for now, but the target changes
	       */
	      {
		ATRSHMLOGSTRATEGYSUFFIX,
		&atrshmlog_buffer_strategy,
		atrshmlog_strategy_first,
		atrshmlog_strategy_last
	      },

	      /* 
	       * The buffer acquire strategy wait nanos wait time.
	       * We can redefine this for the process here.
	       */
	      {
		ATRSHMLOGSTRATEGYWAITTIMESUFFIX,
		&atrshmlog_strategy_wait_wait_time,
		0,
		999999999
	      },

	      /*
	       * The delimiter character for the arg write call
	       */
	      {
		ATRSHMLOGDELIMITERSUFFIX,
		&atrshmlog_delimiter,
		0,
		255
	      },

	      /*
	       * The max of events.
	       * The event buffer has a aximum size at runtime.
	       * So we hold this stable.
	       * Best is to simply use the events and be happy.
	       * If you need more you can set this before in 
	       * the program.
	       * If this is not what you want you can redefine the max 
	       * in the internal and recompile.
	       * If you dont want to do it you can here reset the thing
	       * but it has a cost: only dyn memory after this ...
	       */
	      {
		ATRSHMLOGEVENTCOUNTSUFFIX,
		&new_max,
		0,
		ATRSHMLOGEVENTCOUNTMAXLIMIT
	      },

	      /*
	       * We can redefine the memory we need for a dyn alloc buffer.
	       * We cannot redefine the compiled statics.
	       * So be sure you want it to be this way.
	       * We also cannot redefine the shm limit.
	       * So we can only reduce the dyn alloced size.
	       * Anything else and you have to recompile after
	       * change of the define
	       */
	      {
		ATRSHMLOGBUFFER_INFOSIZE_SUFFIX,
		&atrshmlog_buffer_infosize,
		ATRSHMLOGBUFFER_INFOSIZE_MIN,
		ATRSHMLOGBUFFER_INFOSIZE
	      },

	      /*
	       * We preallocate in a malloc an amount of buffers
	       * to reduce alloc time per buffer.
	       * So its possible to reset the number .
	       * You cannot reset the number the static has - you
	       * have to recomile for this.
	       * But for the dyn buffers you can do it 
	       */
	      {
		ATRSHMLOG_INIT_PREALLOC_COUNT_SUFFIX,
		&atrshmlog_prealloc_buffer_count,
		ATRSHMLOG_INIT_PREALLOC_COUNT_MIN,
		ATRSHMLOG_MAX_PREALLOC_COUNT
	      },

	      /*
	       * We can set the wait nanos for the slave
	       * having it small consumes cpu.
	       * Having it big makes the check to bee late and later ...
	       * So i use 1000 as a start.
	       * But its not the only thing to know
	       * In case we use the maximum number of slaves
	       * it becomes clear that then no thread
	       * should interact with the others, so this
	       * makes a shift to a model with another 
	       * strategy.
	       */
	      {
		ATRSHMLOG_INIT_BUFFER_SLAVE_WAIT_SUFFIX,
		&atrshmlog_f_list_buffer_slave_wait,
		ATRSHMLOG_INIT_BUFFER_SLAVE_WAIT_MIN,
		ATRSHMLOG_INIT_BUFFER_SLAVE_WAIT_MAX
	      },

	      /*
	       * We can set the count of slave threads for dispatching 
	       * full buffers to shm.
	       * This is depending to the strategy
	       * for now it simply works.
	       * Its up to you if you want to stop - meaning 0
	       * or use a rackmount of slaves - max.
	       * To use a good number is a thing you are told in the 
	       * documentation.
	       * So read it before you guess anything.
	       */
	      {
		ATRSHMLOG_SLAVE_COUNT_SUFFIX,
		&atrshmlog_f_list_buffer_slave_count,
		ATRSHMLOG_SLAVE_COUNT_MIN,
		ATRSHMLOG_SLAVE_COUNT_MAX
	      },

	      /*
	       * We normally do not wait for the slaves in the cleanup.
	       * In theory a progam should have stoppt
	       * them before dying ...
	       *
	       * And the atexit is a feature for a dying
	       * program.
	       * But if you insist you can
	       * wait for the slaves to finish.
	       */
	      {
		ATRSHMLOG_WAIT_FOR_SLAVES_SUFFIX,
		&atrshmlog_wait_for_slaves,
		0,
		1
	      },

	      /*
	       * We can set the clock id here .
	       * If you copy the binary to another platform
	       * that is nearby you can use that to
	       * handle a different clock tick getter 
	       */
	      {
		ATRSHMLOG_CLOCK_ID_SUFFIX,
		&my_clock_id,
		ATRSHMLOG_CLOCK_ID_MIN,
		ATRSHMLOG_CLOCK_ID_MAX
	      },

	      /*
	       * We can set the fence flag.
	       * For details to the fence see the doc
	       */
	      {
		ATRSHMLOG_FENCE_1_SUFFIX,
		&atrshmlog_thread_fence_1,
		0,
		1
	      },
	      
	      {
		ATRSHMLOG_FENCE_2_SUFFIX,
		&atrshmlog_thread_fence_2,
		0,
		1
	      },
	      
	      {
		ATRSHMLOG_FENCE_3_SUFFIX,
		&atrshmlog_thread_fence_3,
		0,
		1
	      },
	      
	      {
		ATRSHMLOG_FENCE_4_SUFFIX,
		&atrshmlog_thread_fence_4,
		0,
		1
	      },
	      
	      {
		ATRSHMLOG_FENCE_5_SUFFIX,
		&atrshmlog_thread_fence_5,
		0,
		1
	      },
	      
	      {
		ATRSHMLOG_FENCE_6_SUFFIX,
		&atrshmlog_thread_fence_6,
		0,
		1
	      },
	      
	      {
		ATRSHMLOG_FENCE_7_SUFFIX,
		&atrshmlog_thread_fence_7,
		0,
		1
	      },
	      
	      {
		ATRSHMLOG_FENCE_8_SUFFIX,
		&atrshmlog_thread_fence_8,
		0,
		1
	      },
	      
	      {
		ATRSHMLOG_FENCE_9_SUFFIX,
		&atrshmlog_thread_fence_9,
		0,
		1
	      },
	      
	      {
		ATRSHMLOG_FENCE_10_SUFFIX,
		&atrshmlog_thread_fence_10,
		0,
		1
	      },
	      
	      {
		ATRSHMLOG_FENCE_11_SUFFIX,
		&atrshmlog_thread_fence_11,
		0,
		1
	      },
	      
	      {
		ATRSHMLOG_FENCE_12_SUFFIX,
		&atrshmlog_thread_fence_12,
		0,
		1
	      },
	      
	      {
		ATRSHMLOG_FENCE_13_SUFFIX,
		&atrshmlog_thread_fence_13,
		0,
		1
	      },

	      {
		ATRSHMLOG_CHECKSUM_SUFFIX,
		&atrshmlog_checksum,
		0,
		1
	      },

	      {
		ATRSHMLOG_TARGETBUFFERMAX_SUFFIX,
		&atrshmlog_targetbuffer_max,
		2,
		ATRSHMLOGTARGETBUFFERMAX
	      },

	      /*
	       * This is the exception env variable.
	       *
	       * We use it after the thing is running.
	       *
	       * We can switch logging off
	       * for the attach.
	       *
	       * Then its a question of the process to turn it on
	       * this can handle problems with self instantiation
	       * functions before you are ready to log.
	       *
	       * Set it off till you are sure you want to log.
	       *
	       * So this is a case we want to do it after 
	       * the events are ready.
	       *
	       * You can also set slaves to 0 in env or before in process
	       * and then have control over the process of creation of
	       * slaves, then swith logging on ...
	       */
	      {
		ATRSHMLOG_LOGGING_OFF_SUFFIX,
		&onoff,
		0,
		1,
	      }
			 
	    };

	  my_clock_id = 1;

	  for (int aindex = 0; aindex < (sizeof(atrshmlog_init_data) / sizeof(atrshmlog_init_data[0])); aindex++)
	    atrshmlog_init(atrshmlog_init_data[aindex].c,
			   atrshmlog_init_data[aindex].v,
			   atrshmlog_init_data[aindex].minimum,
			   atrshmlog_init_data[aindex].maximum,
			   in_env);
	  
	  atrshmlog_clock_id = my_clock_id;
	  
	  if (reattach == 0 && atrshmlog_init_buffers_in_advance)
	    atrshmlog_memset_prealloced();
	      
	  if (new_max >  ATRSHMLOGEVENTCOUNT
	      && new_max > atrshmlog_event_locks_max
	      && new_max <= ATRSHMLOGEVENTCOUNTMAXLIMIT)
	    {
	      int oldmax = atrshmlog_event_locks_max;
      
	      atrshmlog_event_t * p2 =  (atrshmlog_event_t*)malloc(new_max + 1);
		      
	      if (p2)
		{
		  memset(p2, 0 , new_max + 1);
		      
		  atrshmlog_event_locks = p2;
			  
		  atrshmlog_event_locks_max = new_max;

		  atrshmlog_putenv(ATRSHMLOGEVENTCOUNTSUFFIX, new_max);
		}
	    }
	    	      
	  atrshmlog_init_events(in_env ? 0 : 1); 
	      
	  /*
	   * Now we are done with the env settings.
	   * So we initialize now the events ( which uses some 
	   * envs by itself, but should be selfcontained
	   * be happy if this works )...
	   */
	  if (reattach == 0)
	    {
	      if (atexit (atrshmlog_exit_cleanup) != 0)
		result = atrshmlog_error_attach_3;
	  
	      // start buffer slave(s)
	      
	      int ret_slave = 0;
	      
	      for (int slaves = 0; ret_slave == 0
		     && slaves < atrshmlog_f_list_buffer_slave_count;
		   slaves++)
		{
		  ret_slave = atrshmlog_create_slave();
		}

	    }
	  
	  if (onoff == 0)
	    atrshmlog_set_logging_process_on();
	  else
	    atrshmlog_set_logging_process_off();
	}
      else
	{
	  // no luck. no connect
	  lshmid = 0;
	}
    }

 leave:
  ;
  
  atrshmlog_putenv(ATRSHMLOGENVSUFFIX, lshmid);

  /* 
   * Little but effectiv trick.
   *
   * We store the pid of the attach in a global read only var 
   * So a thread in a fork can detect the diffrence in pid
   * and restart  slave thread in its init.
   *
   * This should do the job at least for the shell fork
   * problem.
   */
      
  atrshmlog_attach_pid = getpid();

  ++atrshmlog_attach_once;

  // From now on all others see only the initialized module
  // But we still are in the spinlock....
  
  atomic_flag_clear(&atrshmlog_attach_once_flag);
  
  return result;
}


#define MMSET(_f,_x,_min,_max,_val) do { if ((_f)) if ((_min) <= (_val) && (_max) >= (_val) ) (_x) = (_val); } while (0);

/**
 * \n Main code:
 *
 * \brief The reattach to shm memory function
 *
 * We need a valid attach in advance - and then also
 * a detach.
 *
 * If we detect this we go on.
 *
 * We use a int array fro the parameters. 
 * The array deliver a flag and value pair, so first
 * is the flag, then the value.
 * If flag is 0 we don't use the value.
 * So you can start with new values where you 
 * want to.
 * The array has to be set to 0 for all values not used.
 * The array has so far 58 ints.
 *
 * We can set the values, but we do NOT reinit buffers.
 * We do NOT restart slaves.
 * We DO resize the event locks array.
 *
 *
 * \return
 * - Zero : ok
 * - Negativ error
 * - positive worked with minor error
 */
atrshmlog_ret_t atrshmlog_reattach(const atrshmlog_int32_t *const i_params)
{
  ATRSHMLOGSTAT(atrshmlog_counter_reattach);
  
  /* We attach the shm buffer. 
   * First we have to check for the env var and then for the
   * extern ptr  that holds the log base.
   * If the ptr is null we attach.
   * If the env is null we try the file atrshmlogshmid.txt
   * If we still have no hit we give up .
   * If env is ok, but if is 0 we give up.
   *
   * If the env is not set we set the ptr to null to prevent 
   * logging in case we have NOT been a top level process
   * and the log is already in a predecessor process open.
   *
   * Later on we can change this for getting subprocesses
   * to log too...
   */
  long int lshmid = atrshmlog_id;

  // we are a REattach, so an attach had to be there in the first place.
  if (lshmid == 0)
    {
      return atrshmlog_error_reattach_1;
    }
  
  // we lock. this is needed to prevent races...
  while (atomic_flag_test_and_set(&atrshmlog_attach_once_flag) )
	;

  // one enters this first - and only one
  int result = atrshmlog_error_reattach_2;

  /*
   * This is the first big catch.
   * We check for already attached.
   *
   * This is done simple by using the static here.
   *
   * Question: How are we already initialized ?
   *
   * Answer:
   * Hm this is not so simple.
   * We can be the second call or so ..
   * Or we are made but there was a fork before and the image 
   * of the process still has the attach memory.
   * So we make this from the value of attach once.
   * 
   */
  if (atrshmlog_attach_once != 0)
    {
      /*
       * We clear the lock.
       */
      atomic_flag_clear(&atrshmlog_attach_once_flag);

      return atrshmlog_error_reattach_3;
    }

  if (i_params == NULL)
    {
      /*
       * We clear the lock.
       */
      atomic_flag_clear(&atrshmlog_attach_once_flag);

      return atrshmlog_error_reattach_4;
    }

  // we reset the id.
  
  atrshmlog_id = 0;
    
  if (i_params[0] == 1)
    {
      lshmid = i_params[1];
    }

  /* Ok. We have it, and we think its ok if its not false info. 
   * a valid number not 0 is good enough for now
   */
  if (lshmid != 0)
    {
      /* This is looking good .
       * We try to attach now.
       */
      int shmid = (int)lshmid;
		
      void*shmaddr = (void*)0; /* We let decide the os */ 
		
# if ATRSHMLOG_PLATFORM_LINUX_X86_64_GCC == 1

      int shmflg = 0; 
		
      void *shmat_result = shmat(shmid, shmaddr, shmflg);

#endif
	  
# if ATRSHMLOG_PLATFORM_CYGWIN_X86_64_GCC == 1

      int shmflg = 0; 
		
      void *shmat_result = shmat(shmid, shmaddr, shmflg);

#endif

# if ATRSHMLOG_PLATFORM_MINGW_X86_64_GCC == 1


      int envsize = 0;


      if (i_params[2] == 1)
	envsize = i_params[3];
      else
	{
	  // then is MUST be set in the env
	  atrshmlog_init(ATRSHMLOG_BUFFER_COUNT_SUFFIX,
			 &envsize,
			 0,
			 4096,
			 1);

	}
      
      if (envsize < 1)
	{
	  envsize = 8;
	}
	  
      const int save = 256 ;
  
      size_t wantedsize = sizeof (atrshmlog_area_t) + (envsize - ATRSHMLOGBUFFER_MINCOUNT) * sizeof( atrshmlog_buffer_t) + (envsize * ATRSHMLOGBUFFER_INFOSIZE + save);

      int size = wantedsize;
	    
      void *shmat_result = atrshmlog_attach_mapped_file(shmid,size);

#endif

# if ATRSHMLOG_PLATFORM_BSD_AMD64_CLANG == 1

      int shmflg = 0; 
		
      void *shmat_result = shmat(shmid, shmaddr, shmflg);

#endif

# if ATRSHMLOG_PLATFORM_BSD_AMD64_GCC == 1

      int shmflg = 0; 
		
      void *shmat_result = shmat(shmid, shmaddr, shmflg);

#endif

# if ATRSHMLOG_PLATFORM_SOLARIS_X86_64_GCC == 1

      int shmflg = 0; 
		
      void *shmat_result = shmat(shmid, shmaddr, shmflg);

#endif
	  
      if (shmat_result != (void*)-1)
	{
	  atrshmlog_base_ptr = shmat_result;

	  atrshmlog_id = shmid;

	  result = atrshmlog_error_ok;
	      
	  atrshmlog_putenv(ATRSHMLOGENVSUFFIX, lshmid);

	  int onoff = 0;
	      
	  int new_max = atrshmlog_event_locks_max;

	  int my_clock_id = atrshmlog_clock_id;

	  MMSET(i_params[4], atrshmlog_init_buffers_in_advance, 0, 1, i_params[5]);
	  MMSET(i_params[6], atrshmlog_buffer_strategy, atrshmlog_strategy_first, atrshmlog_strategy_last, i_params[7]);
	  MMSET(i_params[8],atrshmlog_strategy_wait_wait_time, 0, 999999999, i_params[9]);
	  MMSET(i_params[10], atrshmlog_delimiter, 0, 255, i_params[11]);
	  MMSET(i_params[12], new_max, 0, ATRSHMLOGEVENTCOUNTMAXLIMIT, i_params[13]);
	  MMSET(i_params[14], atrshmlog_buffer_infosize, ATRSHMLOGBUFFER_INFOSIZE_MIN, ATRSHMLOGBUFFER_INFOSIZE, i_params[15]);
	  MMSET(i_params[16], atrshmlog_prealloc_buffer_count, ATRSHMLOG_INIT_PREALLOC_COUNT_MIN, ATRSHMLOG_MAX_PREALLOC_COUNT, i_params[17]);
	  MMSET(i_params[18], atrshmlog_f_list_buffer_slave_wait, ATRSHMLOG_INIT_BUFFER_SLAVE_WAIT_MIN,	ATRSHMLOG_INIT_BUFFER_SLAVE_WAIT_MAX, i_params[19]);
	  MMSET(i_params[20], atrshmlog_f_list_buffer_slave_count, ATRSHMLOG_SLAVE_COUNT_MIN, ATRSHMLOG_SLAVE_COUNT_MAX, i_params[21]);
	  MMSET(i_params[22], atrshmlog_wait_for_slaves, 0, 1, i_params[23]);
	  MMSET(i_params[24], my_clock_id, ATRSHMLOG_CLOCK_ID_MIN, ATRSHMLOG_CLOCK_ID_MAX, i_params[25]);
	  MMSET(i_params[26], atrshmlog_thread_fence_1, 0, 1, i_params[27]);
	  MMSET(i_params[28], atrshmlog_thread_fence_2, 0, 1, i_params[29]);
	  MMSET(i_params[30], atrshmlog_thread_fence_3, 0, 1, i_params[31]);
	  MMSET(i_params[32], atrshmlog_thread_fence_4, 0, 1, i_params[33]);
	  MMSET(i_params[34], atrshmlog_thread_fence_5, 0, 1, i_params[35]);
	  MMSET(i_params[36], atrshmlog_thread_fence_6, 0, 1, i_params[37]);
	  MMSET(i_params[38], atrshmlog_thread_fence_7, 0, 1, i_params[39]);
	  MMSET(i_params[40], atrshmlog_thread_fence_8, 0, 1, i_params[41]);
	  MMSET(i_params[42], atrshmlog_thread_fence_9, 0, 1, i_params[43]);
	  MMSET(i_params[44], atrshmlog_thread_fence_10, 0, 1, i_params[45]);
	  MMSET(i_params[46], atrshmlog_thread_fence_11, 0, 1, i_params[47]);
	  MMSET(i_params[48], atrshmlog_thread_fence_12, 0, 1, i_params[49]);
	  MMSET(i_params[50], atrshmlog_thread_fence_13, 0, 1, i_params[51]);
	  MMSET(i_params[52], atrshmlog_checksum, 0, 1, i_params[53]);
	  
	  MMSET(i_params[54], onoff, 0, 1, i_params[55]);
	  MMSET(i_params[56], atrshmlog_targetbuffer_max, 2, ATRSHMLOGTARGETBUFFERMAX, i_params[57]);
	  
	  atrshmlog_clock_id = my_clock_id;
	  
	  if (new_max >  ATRSHMLOGEVENTCOUNT
	      && new_max > atrshmlog_event_locks_max
	      && new_max <= ATRSHMLOGEVENTCOUNTMAXLIMIT)
	    {
	      int oldmax = atrshmlog_event_locks_max;
      
	      atrshmlog_event_t * p2 =  (atrshmlog_event_t*)malloc(new_max + 1);
		      
	      if (p2)
		{
		  memset(p2, 0 , new_max + 1);
		      
		  atrshmlog_event_locks = p2;
			  
		  atrshmlog_event_locks_max = new_max;

		  atrshmlog_putenv(ATRSHMLOGEVENTCOUNTSUFFIX, new_max);

		  // we have to reinit the thing
		  // and we use env for this
		  atrshmlog_init_events(0); 
		}
	    }

	  if (onoff == 0)
	    atrshmlog_set_logging_process_on();
	  else
	    atrshmlog_set_logging_process_off();
	}
      else
	{
	  // no luck. no connect
	  lshmid = 0;
	}
    }

 leave:
  ;
  
  atrshmlog_putenv(ATRSHMLOGENVSUFFIX, lshmid);

  ++atrshmlog_attach_once;

  // From now on all others see only the initialized module
  // But we still are in the spinlock....
  
  atomic_flag_clear(&atrshmlog_attach_once_flag);
  
  return result;
}

