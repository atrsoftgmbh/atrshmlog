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

/* atr shmlog.c */

#include "atrshmlog.h"


#include "atrshmlog_internal.h"

/* 
 * this is the base address of the used shm buffer 
 * this is read to determine if the logging is attached
 * to a buffer. if not it is NULL 
 * so this is a read only for all users 
 * only reason to make it available it to speed up the macros
 * ok, its posible to use inline functions today, but they are
 * problematic if you try to hide such a thing as the pointer - 
 * you come from hiding it to the fact that the  impl of your
 * inline has to be known anyway. so its not a real solution ..
 * simply use it as its declared : volatile , never cache it
 * never change it - use it like its const ... 
 * ro
 */
volatile void* atrshmlog_base_ptr = 0;

/* 
 * we save the shm id here for checks 
 * we get it from the attach op.
 * ro
 */
static volatile int atrshmlog_id = 0;

/* 
 * we have an per program switch for logon or log off 
 * this can be used by the program to stop logging for a special 
 * path by using the procedures to switch on and off
 * this is a read only for the user, use the procedures if
 * you need it
 */
volatile const int atrshmlog_logging_process = 0;

/*
 * our delimiter character fro the argv string parts
 */
static char atrshmlog_delimiter = ' ';

/*
 * we can use a chached pid. thats nice if the os and the compiler
 * guys dont have it fast ( even an intrinsic cost time )
 * if you have getenv support you can switch this on.
 * but be sure what you do incase a process 
 *
 * forks
 * spawns
 * does some new fancy way of bearing a subprocess 
 *
 * then the problem of changing that long 
 * cached pid is up to you ...
 */
static int cached_pid = 0; /* flag */

static atrshmlog_pid_t atrshmlog_pid; /* the cached pid */

/* we hold here the prefix for all of our environment variables
 * and files so we can simply switch from one to another by 
 * changing the major prefix name
 * this is done by setting the ATRSHMLOG variable before 
 * attaching / getting the eenv value ...
 * dont change this 
 * size is big, so its ok for now
 * ro
 */

static char prefix_name_buffer[129];

/* 
 * the event flag buffer 
 * we use this to indivuell switch loggings on and off.
 * this is done at attach and is then set for the rest of
 * the runtime - at least in normalcases
 * you can switch on off and get with three procedures
 * the individual values in your rogam
 * so dont touch this if not needed.
 * rw
 */
static char atrshmlog_event_lock_buffer[ATRSHMLOGEVENTCOUNT] = { 0 };

/* 
 * global vars are here 
 * because the buffer is static its normally not reachabel. 
 * we use this pointer for checking the flags.
 * please use the procedures to change if needed
 * ro
 */
volatile const char* const atrshmlog_event_locks = atrshmlog_event_lock_buffer;


/*******************************************************************/


/* 
 * helper : init event lockbuffer
 */
void atrshmlog_init_events(int i_use_file)
{
  /* 
   * switch on or off determination
   * if this is null we switch off individual
   * that means the event flags aer on by default
   * if we do set this it means we invert. the flags are off by default
   * we have to switch them on individual
   */
  char *evn = NULL;

  /*
   * this is the list of on and off switchers.
   * the main logic is determined by event null - see above
   * this is a list of events to be shut off in default mode
   * is evn set - the thing is turned on instead.
   * so we can default the buffer to one of the 
   * two logical states and then switch individual off or on
   */
  char *onoff = NULL;

  /* helperbuffer for names with prefixes
   * ok, this is dirty
   * i know
   * but i dont want to use dyn memory in here.
   * so i use a prefix_name_buffer that is 128 bytes and 
   * this local buffers... 
   */
  char buff[255];

  /* if we are a login shell, there are no env variables.
   * so in this case we use files
   * this is done here in case the flag is given
   * any thing else needs only the env vars 
   * see for the code in the case the env vars are used, then check the code
   * here. there is one diffrence, the use of theenv for onoff
   * and the file is diffent - the file does not use a : as a 
   * delimiter.
   * beside this it works the same - or should at least..
   */
  if (i_use_file)
    {
      strcat(strcpy(buff,prefix_name_buffer), ATRSHMLOGENVFILESUFFIX);
      
      FILE *f = fopen(buff, "r");

      if (f)
    {
      fclose(f);
      evn = "on";
    }

      int flag = (evn == NULL) ? 1 : 0; /* on or off ... */

      memset(atrshmlog_event_lock_buffer, flag, sizeof(atrshmlog_event_lock_buffer));

      strcat(strcpy(buff, prefix_name_buffer), ATRSHMLOGEVENTONOFFFILESUFFIX);

      f = fopen(buff, "r");

      if (f)
    {
      long lindex;
      
      flag = flag ? 0 : 1;

      while (!feof(f) && !ferror(f)) {

        if (1 == fscanf(f, "%ld", &lindex))
          {
            long ev = lindex;

        if (ev < 0 || ev >= sizeof(atrshmlog_event_lock_buffer))
          ;
        else
          atrshmlog_event_lock_buffer[(int)ev] = flag;
          }
      }
      
      fclose(f);
    }

      return;
    }

  /* ok. we do not use a file, so we are not a login shell - or
   * we simply do not log at all. in that case we nether reach this
   * in the case we dont use a flag file we try to check the env
   */

  strcat(strcpy(buff, prefix_name_buffer), ATRSHMLOGEVENTNULLSUFFIX);
  evn = getenv(buff);

  strcat(strcpy(buff, prefix_name_buffer), ATRSHMLOGEVENTONOFFSUFFIX);
  onoff = getenv(buff);

  int flag = (evn == NULL) ? 1 : 0; /* on or off ... */
  
  memset(atrshmlog_event_lock_buffer, flag, sizeof(atrshmlog_event_lock_buffer));

  /* buffer initialized . we can now switch on or off */
  /* we use a : separated string. every event number is in the string
   * delimited by : or begin or end of the string
   * so we simply iterate from : to : to end and then have the number
   * if number is not in range we simply ignore it 
   *
   * example ATRSHMLOG_EVENT_ONOF=1:2:3:13:25:4711
   *
   * this will try to switch of the flags for event 1, 2 , 3, 13, 25 and 4711
   * (ok, we have a buffer of 10 k so this should be possible ... 
   * please dont abuse  - you only hurt yourself )
   *
   * if the env is set the thing turns upside down.
   * then the buffer flag are all 0 and we turN these events on... 
   */
  if (onoff)
    {
      int limit = strlen(onoff);
      int j;
      int ende = 0;
      long ev;
      char* ptr;

      char buff[255];

      /* invert the flag value meaning */
      flag = flag ? 0 : 1;
      
      for (j = 0; j < limit; j = ende + 1)
    {
      for (ende = j;
           onoff[ende] != '\0'
         && onoff[ende] != ':'; 
           ende++)
        ;

      if (onoff[ende] == '\0')
        break;

      if ((ende - j) == 0)
        continue;
      
      memcpy(buff, onoff + j, ende - j);

      buff[ende - j + 1] = '\0';
      
      ev = strtol(buff, &ptr, 10);

      if ((ptr - buff) != (ende - j))
        {
          ;  /* we ignore non numbers */
        }
      else
        {
          if (ev < 0 || ev >= sizeof(atrshmlog_event_lock_buffer))
        ;
          else
        atrshmlog_event_lock_buffer[(int)ev] = flag;
        }
    }

      if ((ende - j) == 0)
    return;

      if (onoff[j] != '\0')
    {
      ev = strtol(onoff + j, &ptr, 10);

      if ((ptr - (onoff + j)) != (ende - j))
        {
          ;  /* we ignore non numbers */
        }
      else
        {
          if (ev < 0 || ev >= sizeof(atrshmlog_event_lock_buffer))
        ;
          else
        atrshmlog_event_lock_buffer[(int)ev] = flag;
        }
    }
    }
}

/*******************************************************************/

/* helper : we use env vars */ 
static void atrshmlog_init_events_no_file()
{
  atrshmlog_init_events(0); 
}

/*******************************************************************/

/* helper : we use flag files */
static void atrshmlog_init_events_use_file()
{
  atrshmlog_init_events(1); 
}

/*******************************************************************/

/* attach the buffer if logging is wanted and the buffer is not
 * attached
 * set the global prefix_ame_buffer if non nullptr and non null str
 * supported by user
 */
int atrshmlog_attach()
{
  /* we attach the shm buffer. 
   * first we have to check for the env var and then for the
   * extern ptr  that holds the log base
   * if the ptr is null we attach.
   * if the env is null we try the file atrshmlogshmid.txt
   * if we still have no hit we give up 
   * if the env is not set we set the ptr to null to prevent 
   * logging in case we have NOT been a top level process
   * and the log is already in  a predecessor process open
   * later on we can change this for getting subprocesses
   * to log too...
   */

  int result = -1;
  
  volatile static int once = 0;
  
  if ((once == 0) && ! ATRSHMLOGLOGGING )
    {
      char *shmid_str;

      ++once;

      /* double init ? */
      if (once > 1)
    return result;

      /* 
       * this does two things : first it initializes the prefix_name_buffer
       * with the value of default and if the ATRSHMLOG is set then this 
       * string value.
       * then it uses the prefix with the suffix to get the env
       * value for this
       * then the thing is used to make the shmid to identify 
       * the attaching buffer....
       * see the code for details
       */
      shmid_str = atrshmlog_getenv();
      
      if (shmid_str != 0)
    {
      int base = 10; 
      char *endptr;
      
      long int lshmid = strtol(shmid_str, &endptr, base);

      /* ok. we have it, and we think its ok if its not false info. 
       * so we check for endptr points to a zero byte and lshmid is not 0
       */
      if (lshmid != 0 && *endptr == '\0')
        {
          /* this is looking good 
           * we try to attach
           */
          int shmid = (int)lshmid; /* this should be the same ... */

          void*shmaddr = (void*)0; /* we let decide the os */ 

          int shmflg = 0; 
          
          void *shmat_result = shmat(shmid, shmaddr, shmflg);

          if (shmat_result != (void*)-1)
        {
          atrshmlog_base_ptr = shmat_result;

          atrshmlog_id = shmid;

          atrshmlog_pid = getpid();

          char buff[255];
          strcat(strcpy(buff,prefix_name_buffer), ATRSHMLOGCACHEPIDSUFFIX);

          cached_pid = getenv(buff) != NULL;
          
          strcat(strcpy(buff,prefix_name_buffer), ATRSHMLOGDELIMITERSUFFIX);
          char *p = getenv(buff);

          if (p && *p)
            {
              atrshmlog_delimiter = (char)strtol(p, NULL, 10);
            }

          atrshmlog_init_events_no_file(); 

          atrshmlog_logging_on();

          result = 0;
        }
        }

      return result;
    }
      
      /* we are through the env. check for a file as a last resort */
      {
    char buff[255];

    strcat(strcpy(buff, prefix_name_buffer), ATRSHMLOGENVFILESUFFIX);
    
    FILE *f = fopen(buff, "r");

    if (f)
      {
        long int lshmid;
        if (1 != fscanf(f,"%ld", &lshmid))
          {
        fclose(f);

        strcat(strcat(strcpy(buff, prefix_name_buffer), ATRSHMLOGENVSUFFIX), "=");
        
        putenv(buff);
        --result;
        return result;
          }

        /* we have a number. try to connect now */
        fclose(f);

        /* ok. we have it, and we think its ok if its not false info. 
         * so we check for endptr points to a zero byte and lshmid is not 0
         */
        if (lshmid != 0)
          {
        /* this is looking good 
         * we try to attach
         */
        int shmid = (int)lshmid; /* this should be the same ... */
        
        void*shmaddr = (void*)0; /* we let decide the os */ 

        int shmflg = 0; 
          
        void *shmat_result = shmat(shmid, shmaddr, shmflg);

        if (shmat_result != (void*)-1)
          {
            sprintf(buff, "%s%s=%ld", prefix_name_buffer, ATRSHMLOGENVSUFFIX, lshmid);
            
            atrshmlog_base_ptr = shmat_result;
            
            atrshmlog_id = shmid;

            putenv(buff);

            atrshmlog_pid = getpid();

            char buff[255];
            strcat(strcpy(buff,prefix_name_buffer), ATRSHMLOGCACHEPIDSUFFIX);

            cached_pid = getenv(buff) != NULL;
          
            strcat(strcpy(buff,prefix_name_buffer), ATRSHMLOGDELIMITERSUFFIX);
            char *p = getenv(buff);

            if (p && *p)
            {
              atrshmlog_delimiter = (char)strtol(p, NULL, 10);
            }


            atrshmlog_init_events_use_file();

            atrshmlog_logging_on();

            result = 0;
          }
        else
          {
            strcat(strcat(strcpy(buff, prefix_name_buffer), ATRSHMLOGENVSUFFIX), "=");
        
        
            putenv(buff);
          }         
          }
        else
          {
        strcat(strcat(strcpy(buff, prefix_name_buffer), ATRSHMLOGENVSUFFIX), "=");
        
        putenv(buff);
          }         

        return result;
            
      }
      }
      
      return result; /* ok. we did it */
    }

  /* to come so far the ptr is not null. */

  if (once == 0)
    {
      char *shmid_str;

      ++once;

      if (once > 1)
    return result;
      
      shmid_str = atrshmlog_getenv();
      
      if (shmid_str == NULL)
    {
      /* well. we dont log any more. */
      atrshmlog_base_ptr = (void*)0;
      
      atrshmlog_id = 0;

      atrshmlog_logging_off();
    }
    }

  return result;
}


/*******************************************************************/

/* we disconnect if needed
 */
void atrshmlog_detach()
{
  if (! ATRSHMLOGLOGGING )
      return;

  shmdt((const void*)atrshmlog_base_ptr);

  atrshmlog_base_ptr = (volatile void*)0;

  atrshmlog_id = 0;
}



/*******************************************************************/


/* we write to the log 
 * this is the major writer. 
 */
int atrshmlog_write(const int i_eventnumber,
             const int i_eventflag,
             const int i_userflag,
             const atrshmlog_time_t* i_starttime,
             const atrshmlog_time_t* i_endtime,
             const void* i_local,
             const int i_size,
             const char* i_argv[],
             const int i_argc_hint)
{
  if (! ATRSHMLOGLOGGING )
    goto exit1;

  /* no logging , wrong parameter ... */
  if (i_size <= 0)
    goto exit2;
  
  /* no logging , wrong parameter ... */
  if (i_local == NULL)
    goto exit3;
  
    /* temporary logging stop by program 
     * - can be on and of and on again ...
     */
  if (atrshmlog_logging_process == 0)
    goto exit4;
  

    /* next. we check for initialized */
  atr_shm_log_area_t * a = ATRSHMLOGGETAREA;

  /* bad thing. wrong id */
  if (a->shmid != atrshmlog_id)
    goto exit5;
  
  /* bad thing. safeguard invalid */
  if (a->safeguard != SAFEGUARDVALUE)
    goto exit6;
  
  /* bad thing : version is wrong */
  if (a->version != ATRSHMLOGVERSION)
    goto exit7;

  /* can be happen : end of logging anounced by user via flag in shm */
  if (a->ich_habe_fertig != 0) 
    goto exit8;
  

    /*
  int count_uses = 0;
  
  int count_memcpy = 0;

  int count_state_ok_first_check = 0;

  int count_state_not_ok = 0;
  
  int count_state_not_ok_second = 0;

  int count_mutex_lock = 0;

  int count_safeguard_problem = 0;

  int count_state_check_ok_second_check = 0;

  int count_buffer_filled = 0;

  int count_slot_checks = 0;

  int count_slots_full = 0;
  */
  /* we start with the next buffer that is suspected to be unused
   * thats the lastused +1 or 0 if we hit the count of buffer
   * this CAN be wrong in concurrent situatiuns
   * but the time window is small compared to the reality time
   * all other things need.
   * so we give it a try.
   * it also gives a all use buffer equal quality
   * so we get all buffers more or less equal full ...
   * when we get a full buffer we skip, when we get a buffer in use 
   * we have to wait
   */

  const int limit = a->count;

  atrshmlog_header_t h;

  if (cached_pid)
    goto dopid;
  
  h.pid = getpid(); /* or not... */

 donepid:
  
  /*
   * fill header buffer 
   * this was first done inside to the target.
   * but with some time the prefill before the operation is 
   * better in a multi thread env
   * and a high number of processes
   */
  

  /*    h.totallen = i_size; */
  h.starttime = *(atrshmlog_internal_time_t*)i_starttime;
  h.endtime = *(atrshmlog_internal_time_t*)i_endtime;
  h.tid = ATRSHMLOG_GETTHREADID;
  h.eventnumber = i_eventnumber;
  h.eventflag = i_eventflag;
  h.userevent = i_userflag;

  char argvbuffer[ATRSHMLOGARGVBUFFERLEN];

  /* we collect at max the length of the argv.
   * having more data in the argv is simply ignored, the max is the buffer
   * len. no more 
   * so its possible to use arbitrary number of argv, but only 
   * the first of them go into the buffer till the length limit is hit.
   */
    
  int argvbufferlen = 0;
    
  if (i_argv)
    goto doargv;
    
 afterargv:
  /* we have the target size now */
  h.totallen = i_size + sizeof(h) + argvbufferlen;
    
  /* we loop till we hit a free buffer.
   */
    
  int i = ATRSHMLOGCUTOFFLOWBYTE(a->lastused) + 1;

  // int i = (unsigned long)random() % limit;
    
  /* wrap around */
    
  if (i >= limit)
    i = 0;

  /* hint for all others, this is now in use ... */
  a->lastused = i; 

  while (1)
      {
	atr_shm_log_buffer_t* b = &(a->logbuffers[i]);

	char *write_start_adress;

	volatile long *my_slot;

	volatile int state = ATRSHMLOGCUTOFFLOWBYTE(b->state);

	/* count_uses++; */
	
	if (state & ((int)atr_shm_log_writeable | (int)atr_shm_log_write_halffull))
	  {
	    /*count_state_ok_first_check++; */

	    int mutex_lock_result;
	    int result_write = -1;
	    int mutex_unlock_result;
    
	    /* do the mutex thing now */
	    pthread_mutex_t* const m = &(b->lock_mutex);
      
	    do {
	      /* count_mutex_lock++; */
	      mutex_lock_result = pthread_mutex_lock(m);
	    } while (mutex_lock_result != 0);

	    /* any strange behaviour found for the buffer ? */
	    if (b->safeguard != SAFEGUARDVALUE)
	      goto dosafeguard;

	  donesafeguard:

        
	    /* double check for the state */
	    state = b->state;
    
	    if (state  & ((int)atr_shm_log_writeable|(int)atr_shm_log_write_halffull))
	      {
		/*count_state_check_ok_second_check++; */
		
		/* check for the size */
		if ((b->size + h.totallen) > ATRSHMLOGBUFFER_INFOSIZE)
		  goto size_hitted;

		/* 
		 * we check for a free slot.
		 * the slots are needed for the reader to see 
		 * a thread is doing transfers and has not finished
		 * so we need a free slot here and we do this in the mutex
		 * lock
		 * if we dont find a free we pass this buffer
		 * 
		 * if we find a free slot (the normal case )
		 * we set the slot.
		 * we make the slot non 0 and so the 
		 * reader can see we are not finished in 
		 * transfering our stuff even when we have 
		 * given free the mutex
		 */
        
		/* helper */
		volatile long* slotlimit = b->slots + ATRSHMLOGSLOTCOUNT;
		/* *slotlimit = 0;*/ /* set the guard */

		/* check for a free slot */
		for (my_slot = b->slots; *my_slot; my_slot++)
		  /*count_slot_checks++*/ ;

		/* no free slot so far, only the guard */
		if (my_slot == slotlimit)
		  {
		    /*		    count_slots_full++; */
		    goto again;
		  }

		/* ok. we have a free slot. 
		 * now we signal the reader we are not done 
		 */
		*my_slot = 1;

		/* we remember the adress we have to start writing */
		write_start_adress = &(b->info[b->size]);

		/* we reserve our space now */
		int res_len = (b->size += h.totallen);

		/* we set the state if this is needed */
		if (res_len >= ATRSHMLOGBUFFER_INFOSIZE)
		  goto size_limit_reached;

		if (res_len >= ATRSHMLOGBUFFER_INFOSIZE_HALF)
		  goto half_limit_reached;

		/*
		b->count_writes++;
		
		b->count_uses += count_uses;
	    
		b->count_memcpy += count_memcpy;

		b->count_state_ok_first_check += count_state_ok_first_check;

		b->count_mutex_lock += count_mutex_lock;

		b->count_safeguard_problem += count_safeguard_problem;

		b->count_state_check_ok_second_check += count_state_check_ok_second_check;

		b->count_buffer_filled += count_buffer_filled;

		b->count_slot_checks += count_slot_checks;

		b->count_slots_full += count_slots_full;

		b->count_state_not_ok += count_state_not_ok;

		b->count_state_not_ok_second += count_state_not_ok_second;
		*/
	      size_limit_hitted:
		do {
		  mutex_unlock_result = pthread_mutex_unlock(m);
		} while (mutex_unlock_result != 0);
        
		/* from here on we do the time consuming stuff 
		 *
		 * for the records:
		 * our memory is reserved in the mutex locked region
		 * and so we simply can now do the transfers.
		 * we hold a slot in the buffer, so the reader
		 * can act and reserve it for transfer, but as long as 
		 * we dont set the slot to 0 the reader has to wait 
		 * even if he has the mutex.
		 * so it is safe to transfer the buffers now.
		 * after that we can clear the slot
		 * then the reader can get the control of the buffer.
		 * the same time no other thread of writer can do anything
		 * to this memory we have reserved and the slot 
		 * only errounos writers can make this fail.  
		 */

		memcpy(write_start_adress, &h, sizeof(h));

		write_start_adress += sizeof(h);
        
		memcpy(write_start_adress, i_local, i_size);

		if (! argvbufferlen)
		  {
		/* we free at last the slot. now the reader
		 * can get control and move the buffer to
		 * processing and we have no problem then 
		 * to do the exit here 
		 * and now other writer threads can get this slot
		 * and write to this buffer too
		 * all in all this is a lock free construct
		 * that works because we get the slot indsde the 
		 * mutex locked region, and noone try to 
		 * compete with the slot for the other writers
		 */
		*my_slot = 0;

		return 0;
		  }
		
	      copy_argvbuffer:		
		  {
		    write_start_adress += i_size;

		    memcpy(write_start_adress, argvbuffer, argvbufferlen);
		    *my_slot = 0;

		    return 0;
		  }
		  
	      half_limit_reached:
		  {
		    b->state = atr_shm_log_write_halffull;
		  }
		goto size_limit_hitted;

	      size_limit_reached:
		{
		    b->state = atr_shm_log_full;
		  }
		goto size_limit_hitted;
		
	      size_hitted:
		{
		    /* count_buffer_filled++; */
		    
		    /* this is a bit harsh, but its normally ok
		     * we have a relative small write payload.
		     * if this is not fitting the buffer is 
		     * relatively full... 
		     */
		    b->state = atr_shm_log_full;
            
		    goto again;
		  }

 dosafeguard:
	      {
		/*count_safeguard_problem ++; */
		
		/* this a safeguard for the buffer, and if
		 * it is broken the best we can do is to ignore
		 * that buffer from now on
		 *          b->state = atr_shm_log_illegal;
		 * this is ok, but ...
		 * ... we can run out of buffers then easyly ...
		 * .. so we do instead a reinit
		 */ 
		b->size = 0;

		b->safeguard = SAFEGUARDVALUE;

		/* well, this wasnt that hard...
		 * but now we do a thing that can corrupt 
		 * things - we reset the slots
		 * this must be use - the slots can be corrupted
		 * but we dont try to be too paranoid
		 */
		for (int i = 0; i  <= ATRSHMLOGSLOTCOUNT; i++)
		  b->slots[i] = 0;

		/* ok. we set the state as now needed */
		b->state = atr_shm_log_writeable;
	      }
	      goto donesafeguard;
		
	}
	    /*	    else
	      {
		count_state_not_ok_second++;
	      }
	    */
	  again:
	    do {
	      mutex_unlock_result = pthread_mutex_unlock(m);
	    } while (mutex_unlock_result != 0);

	  }
	/*	else
	  {
	    count_state_not_ok++;
	  }
	*/
	/* i = (unsigned long)random() % limit; */
		i++; 
    
	if (i >= limit)
	  i = 0; 
	
	/* hint for all others, this is now in use ... */
	a->lastused = i; 

      }
  
  /* NOT REACHED */

 doargv:
  {
	int index = 0;
    
	char* dest = argvbuffer;

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
            
		*dest++ = atrshmlog_delimiter;
        
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
	    argvbufferlen = dest - argvbuffer;
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
            
		*dest++ = atrshmlog_delimiter;
            
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
	    argvbufferlen = dest - argvbuffer;
	  }
      }
      goto afterargv;

 exit1:
          return -1;
 exit2:
	  return -5;

 exit3:
	  
    return -6;

 exit4:
    
    return 8;

 exit5:  
    return -2;

 exit6:
    
    return -3;

 exit7:
        return -4;

 exit8:
	
    return 7;

 dopid:

    h.pid = atrshmlog_pid;

    goto donepid;


}




/*******************************************************************/
/*
 * this is os dependent. its the linux way here 
 */
void atrshmlog_gettime(atrshmlog_time_t* o_t)
{
  /* this is the best till now for linux systems */
     clock_gettime(CLOCK_REALTIME, (atrshmlog_internal_time_t*)o_t);
  //   clock_gettime(CLOCK_REALTIME_COARSE, (atrshmlog_internal_time_t*)o_t);
  //   clock_gettime(CLOCK_MONOTONIC_COARSE, (atrshmlog_internal_time_t*)o_t);
  //   clock_gettime(CLOCK_BOOTTIME, (atrshmlog_internal_time_t*)o_t);
  //   clock_gettime(CLOCK_PROCESS_CPUTIME_ID, (atrshmlog_internal_time_t*)o_t);
  // clock_gettime(CLOCK_THREAD_CPUTIME_ID, (atrshmlog_internal_time_t*)o_t);

}

/******************************************************************/

void atrshmlog_set_prefix (const char*i_prefix)
{
  /* set it if all is ok and if not already set */
  if (i_prefix && *i_prefix != '\0' && *prefix_name_buffer == '\0')
    {
      strncpy(prefix_name_buffer, i_prefix,128);
      prefix_name_buffer[128] = 0;
    }
}
/*******************************************************************/

/*
 * get the name of the shm id envirionment variabel 
 * we can have multiple buffers to log to
 * so to make life simple we have a default buffer value,
 * this is ATRSHMLOG_ID in the environment
 * 
 * if you need more buffers and in concurrency you can do that 
 * by overwriting ATRSHMLOG_ID in it...
 * 
 * but sometimes its not that simple, 
 * then you can use the prefix part via ATRSHMLOG
 * this means you dont have to know the value, its already
 * set somewhere else and you only switch the prefix to
 * something diffrent. you can use 128 chars as the new prefix
 * that replace ATRSHMLOG in the lookups....
 * 
 * every program starts its logging with either 
 * a getenv or a attach (which itself starts with a getenv
 * so we use inside the getenv the following procedure
 * that constructs the name of the prefix dynamically
 * for the first time it is run. and after this it is never
 * changing the given value any more...
 *
 * ATRSHMLOG=hugo 
 *
 * is a perfectly working option....
 * this changes the prefix for all of the getenv and file lookups 
 * yes, the files too ... 
 * so you can set it somehow and then use it even in the case the thing is
 * a logging shell (to be onest, i could not achive this, i leave it as
 * an option for someone better in configuring the system ) 
 */
const char* atrshmlog_the_env()
{
  static char buff[255];

  static int once = 0;

  if(once == 0)
    {
      ++once;

      if (once == 1)

        {
      /* alredy set via program ? */
      if (*prefix_name_buffer == '\0')
        strcpy(prefix_name_buffer, ATRSHMLOG_ENV_PREFIX);
      
      if (getenv(ATRSHMLOG_ENV_PREFIX))
        {
          int len = strlen(getenv(ATRSHMLOG_ENV_PREFIX));

          if(len > 0)
        {
          memcpy(prefix_name_buffer, getenv(ATRSHMLOG_ENV_PREFIX), (len < 128) ? len : 128);
          prefix_name_buffer[(len < 128) ? len : 128 ] = '\0';
        }
        }

      strcat(strcpy(buff, prefix_name_buffer), ATRSHMLOGENVSUFFIX);
    }
    }
  
  return buff;
}

/*******************************************************************/

/*
 * get the environment value for the shm id
 */
char* atrshmlog_getenv()
{
  return getenv(atrshmlog_the_env());
}

/*******************************************************************/
/*
 * the major version of the shmlog system
 */
int atrshmlog_version()
{
  return ATRSHMLOGVERSION;
}

/*******************************************************************/
/*
 * the minor version
 */
int atrshmlog_minor_version()
{
  return ATRSHMLOGMINORVERSION;
}

/*******************************************************************/
/*
 * the patch version
 */
int atrshmlog_patch_version()
{
  return ATRSHMLOGPATCHVERSION;
}

/*******************************************************************/
int atrshmlog_shmid()
{
  return atrshmlog_id;
}


/*******************************************************************/

/*
 * set the event flag to non zero. so from this on we use the event
 * this is to be used by the program 
 */
void atrshmlog_set_event(int index)
{
  if (index < 0 || index >= sizeof(atrshmlog_event_lock_buffer))
    return;
  
  ((char*)atrshmlog_event_locks)[index] = 1;
}

/*******************************************************************/
/*
 * set the event flag to zero. so the event is no longer used
 * this is to be used by the program
 */
void atrshmlog_clear_event(int index)
{
  if (index < 0 || index >= sizeof(atrshmlog_event_lock_buffer))
    return;
  
  ((char*)atrshmlog_event_locks)[index] = 0;
}

/*******************************************************************/
/*
 * get the event flag state
 */
int atrshmlog_get_event(int index)
{
  if (index < 0 || index >= sizeof(atrshmlog_event_lock_buffer))
    return 0;
  
  return ((char*)atrshmlog_event_locks)[index] != 0;
}

/*******************************************************************/
void atrshmlog_logging_on()
{
  *((int*)&atrshmlog_logging_process) = 1;
}

/*******************************************************************/
void atrshmlog_logging_off()
{
  *((int*)&atrshmlog_logging_process) = 0;
}


/*******************************************************************/
int atrshmlog_logging()
{
  if (!ATRSHMLOGLOGGING)
    return -1;

  if (atrshmlog_logging_process == 0)
    return -2;

  atr_shm_log_area_t * a = ATRSHMLOGGETAREA;

  if (a->ich_habe_fertig != 0)
    return -3;
  
  return 0;
}

/* end of file */
