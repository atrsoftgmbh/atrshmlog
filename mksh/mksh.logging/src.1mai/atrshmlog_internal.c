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

#include "atrshmlog.h"

#include "atrshmlog_internal.h"

/*******************************************************************/

/* helper : we init one buffer */
static int atrshmlog_init_one_buffer(atr_shm_log_area_t *i_shm,
                     const int i_index_buffer);


/*******************************************************************/

/* we create the shm 
 * on error we deliver a -x, else a valid positive shmid
 * this uses the key from external.
 * key is a 32 bit value
 */
int atrshmlog_create(const key_t i_key,
          const int i_count)
{
  int result = -1;

  if (i_key == 0 || i_key  == IPC_PRIVATE)
    return result;

  --result;
  
  /* at least ATRSHMLOGBUFFER_MINCOUNT .. */
  if (i_count < ATRSHMLOGBUFFER_MINCOUNT)
    return result;

  --result;
  
  /* max ATRSHMLOGBUFFER_MAXCOUNT buffers */
  if (i_count > ATRSHMLOGBUFFER_MAXCOUNT)
    return result;

  /* we calc the buffer size.
   * its a fixed part for our infos and a variabel count of
   * buffers.
   * some systems dont have much memory for a shm buffer, 
   * some have...
   * so simply try to check for the size that is possible with the
   * double number check ...
   */
  
  size_t wantedsize = sizeof (atr_shm_log_area_t) + (i_count - ATRSHMLOGBUFFER_MINCOUNT) * sizeof( atr_shm_log_buffer_t);

  int shmflg = IPC_CREAT | IPC_EXCL;

  shmflg |= ATRSHMLOG_ACCESS; 
    
  int result_shmget = shmget(i_key, wantedsize, shmflg);

  --result;
    
  if (result_shmget == -1)
    return result;

  return result_shmget;
}


/*******************************************************************/

/* we destroy the shm */
int atrshmlog_delete(const int i_shmid)
{

  int shmctl_result = shmctl(i_shmid, IPC_RMID, (struct shmid_ds *)0);

  return shmctl_result;
}

/*******************************************************************/

/* we verify the buffer is ok.
 * we check the shmid and we check the states 
 * this can not be used in mt case, so use it only for simple checker 
 * program s
 * a basic version is alredy in the atrshmlogverify.c
 */
int atrshmlog_verify()
{
  int result = -1 ;

  if ( ! ATRSHMLOGLOGGING )
    return result;

  atr_shm_log_area_t * a = ATRSHMLOGGETAREA;
    
  int i = 0;

  int limit = 0;
    
  --result;
    
  if (a->shmid != atrshmlog_shmid())
    return result;

  --result;

  if (a->safeguard != SAFEGUARDVALUE)
    return result;
    
  --result;

  limit = ATRSHMLOGCUTOFFLOWBYTE(a->count);
    
  if (ATRSHMLOGCUTOFFLOWBYTE(a->lastused) >= limit)
    return result;

  --result;
    
  if (a->version != ATRSHMLOGVERSION)
    return result;
    
  for (i = 0; i < limit ; i++)
    {
      int state = ATRSHMLOGCUTOFFLOWBYTE(a->logbuffers[i].state);

      --result;
    
      if (!(state == atr_shm_log_writeable
        || state == atr_shm_log_write_halffull
        || state == atr_shm_log_full ))
    return result;
      
      --result;

      if (a->logbuffers[i].safeguard != SAFEGUARDVALUE)
    return result;

      for (int i = 0; i  < ATRSHMLOGSLOTCOUNT; i++)
    ;
      
      /* TODO: 
       * check the list of info data too 
       * perhaps we will do this in the future.
       * for now we try to be not paranoid and think
       * the content is ok if all the numbers and safguards
       * are ok...
       */
    }

  return 0;
}

/*******************************************************************/

/* we initialize the buffer after it is attached 
 * this is not mt safe
 * so try to use a initialisation program that is
 * simple for it.
 * see the atrshmloginit.c for this
 */
int atrshmlog_init_shm_log (const int i_count_buffers)
{
  int result = -1;

  /* we check first, then we initialize.
   * its vital that we do NOT reinitialize the buffer ! 
   */
  
  if ( ! ATRSHMLOGLOGGING )
    return result;

  atr_shm_log_area_t *a = (atr_shm_log_area_t *)atrshmlog_base_ptr;

  --result;
    
  if (a->shmid == atrshmlog_shmid())
    return result;

  --result;

  if (a->safeguard == SAFEGUARDVALUE)
    return result;
    
  /* ok. we are in. the shm seems not to be initialized */
  
  for (int i = 0; i < i_count_buffers ; i++)
    {
      int initresult = atrshmlog_init_one_buffer(ATRSHMLOGGETAREA, i);
    
      --result;
      
      if (initresult < 0)
    return result;
    }
  

  /* ok. we can now set the final info in the buffer */
  a->version = ATRSHMLOGVERSION;
  a->shmid = atrshmlog_shmid();
  a->count = i_count_buffers;
  a->lastused = 0;
  a->ich_habe_fertig = 0;
  a->readerflag = 0;
  a->readerpid = 0;
  a->writerflag = 0;
  a->writerpid = 0;
  a->safeguard = SAFEGUARDVALUE;

  return 0;
}


/*******************************************************************/

/* we have to clean up the mutexes after stopping 
 * pthread version
 * this is an optional step in shuting down the log.
 * it is itself not mt safe.
 * so use a simple program for that
 * see the atrshmlogfinish.c
 */

void atrshmlog_cleanup_mutexes()
{
  if (! ATRSHMLOGLOGGING )
    return;

  /* next. we check for initialized */
  atr_shm_log_area_t *a = ATRSHMLOGGETAREA;

  if (a->shmid != atrshmlog_shmid())
    return;

  if (a->safeguard != SAFEGUARDVALUE)
    return;

  if (a->version != ATRSHMLOGVERSION)
    return;
  
  atr_shm_log_buffer_t* b = &(a->logbuffers[0]);

  /* from now on the buffer is useless. so we switch to not logging */
  a->ich_habe_fertig = 1;

  sleep(5); /* we give 5 seconds time for the reader to finish */
  
  for (int i = 0; i < ATRSHMLOGCUTOFFLOWBYTE(a->count); i++)
    {
      b->state = atr_shm_log_illegal; /* no leagal state, no op any more */

      b++;
    }

  b = &(a->logbuffers[0]);
  
  for (int i = 0; i < a->count; i++)
    {
      pthread_mutex_t * m = &(b->lock_mutex);

      pthread_mutex_destroy(m);

      b->safeguard = 0L;
      
      b++;
    }

  a->shmid = 0;
  a->safeguard = 0L;
}


/*******************************************************************/

/* helper: we initialize one buffer
   pthread version 
 */
int atrshmlog_init_one_buffer(atr_shm_log_area_t *i_shm,
                  int i_index_buffer)
{
  int result = -1;
  
  pthread_mutexattr_t pshared_mode;

  atr_shm_log_buffer_t* b = &(i_shm->logbuffers[i_index_buffer]);

  /* this is the only critical part : we have to initialize the mutex */

  /* we init first the attribute, shareable for processes */
  { 
    int init_attr_result = pthread_mutexattr_init(&pshared_mode);

    --result;
    
    if (init_attr_result != 0)
      return result;
  }
    
  /* ok. we have the attr. now we declaire its inter process shared */
  {
    int share_mode = PTHREAD_PROCESS_SHARED;
      
    int result_set_proc = pthread_mutexattr_setpshared(&pshared_mode, share_mode);

    --result;

    if (result_set_proc != 0)
      {
    pthread_mutexattr_destroy(&pshared_mode);
      
    return result;
      }
  }

  /* ok. we can now init the mutex itself */
  {
    pthread_mutex_t* m = &(b->lock_mutex);
    
    int result_mutex_init = pthread_mutex_init(m, &pshared_mode);

    --result;
      
    if (result_mutex_init != 0)
      {
    pthread_mutexattr_destroy(&pshared_mode);
      
    return result;
      }
  }
    
  /* ok, we have to clean up on our side */
  {
    int result_attr_destroy = pthread_mutexattr_destroy(&pshared_mode);

    --result;
      
    if (result_attr_destroy != 0)
      return result;
  }

  /* ok. we are through. we have to set the final infos and thats it */
  b->size = 0;
  
  b->safeguard = SAFEGUARDVALUE;

  for (int i = 0; i  <= ATRSHMLOGSLOTCOUNT; i++)
    b->slots[i] = 0;

  b->state = atr_shm_log_writeable;

  b->count_writes = 0;
  
  b->count_uses = 0;
  
  b->count_memcpy = 0;

  b->count_state_ok_first_check = 0;

  b->count_state_not_ok = 0;

  b->count_state_not_ok_second = 0;
 
  b->count_mutex_lock= 0 ;

  b->count_safeguard_problem= 0 ;

  b->count_state_check_ok_second_check= 0 ;

  b->count_buffer_filled= 0 ;

  b->count_slot_checks= 0 ;

  b->count_slots_full= 0 ;

  return 0;
}

/*******************************************************************/

/* we read a buffer and write it to a local memory area
 * we can read half full, full and writeable buffers
 * the latter is needed for the cleanup after finish of the 
 * buffers so far not cleared and only partly full
 * pthread version
 *  this is done in a safe way. so we can do this in parallel
 * to running writers.
 * and we need to do it. if all buffers are full the writers have 
 * to loop forever. so we have to use the mt safe way here
 * thats a bit a blockade of the system, but if the number of
 * buffers is sufficient we dont disturb the writers much..
 * they check from outside of the mutex.
 * so we can simply lock, switch the flag to dont disturb me mode
 * and clenaup the buffer.
 * anything else is up to the writer
 * process. so try to handle the data then as fast as you can.
 * see for a simple reader the
 * atrshmlogreader.c
 */
int atrshmlog_read(const int i_index_buffer,
           void *o_target,
           int *o_target_length)
{
  int result = -1;

  if(!ATRSHMLOGLOGGING)
    return result;

  --result;
     
  if (i_index_buffer < 0)
    return result;

  --result;
  
  atr_shm_log_area_t *i_shm = ATRSHMLOGGETAREA;
  
  if (i_shm->shmid != atrshmlog_shmid())
    return result;

  --result;
  
  if (i_shm->safeguard != SAFEGUARDVALUE)
    return result;

  --result;
  
  if (ATRSHMLOGCUTOFFLOWBYTE(i_shm->count) <= i_index_buffer)
    return result;

  --result;

  if (i_shm->version != ATRSHMLOGVERSION)
    return result;
  
  /* ok. we can now do the transfer */

  atr_shm_log_buffer_t* b = &(i_shm->logbuffers[i_index_buffer]);

  int lock_mutex_result ;

  int unlock_mutex_result;
    
  pthread_mutex_t* m = &(b->lock_mutex);

  volatile int state;
    
  --result ;

  state = ATRSHMLOGCUTOFFLOWBYTE(b->state);
    
  if (!(state == atr_shm_log_writeable
    || state == atr_shm_log_write_halffull
    || state == atr_shm_log_full))
    return result;

  do {
    lock_mutex_result = pthread_mutex_lock(m);
  } while (lock_mutex_result != 0);

  /* from now on we have to do an unlock */
  state = b->state;

  /* we block all concurrent writers */
  b->state = atr_shm_log_illegal;
  
  int size = b->size;
    
  --result;

  if (size < 0)
    {
      goto ende;
    }

  --result;
    
  if (size > ATRSHMLOGBUFFER_INFOSIZE)
    {
      goto ende;
    }

  --result ;

  if (!(state == atr_shm_log_writeable
    || state == atr_shm_log_write_halffull
    || state == atr_shm_log_full))
    {
      /* restore state */
      b->state = state;
      goto ende;
    }

  /* we have to check the slots now  */


 slotcheck:
  for (int i = 0 ; i < ATRSHMLOGSLOTCOUNT; i++)
    if (b->slots[i] != 0)
      {
    usleep(1);
    goto slotcheck;
      }

  /* ok. we are clear to go to copy */

  /* is the length ok for copy */ 

  if (size > 0)
    {
      /* we have to do the copy now */

      (void)memcpy(o_target, &(b->info[0]), size);
    }

  *o_target_length = size;

  b->size = 0;
  
  b->state = atr_shm_log_writeable;
      
  result = 0;

 ende:
  do {
    unlock_mutex_result = pthread_mutex_unlock(m);
  } while (unlock_mutex_result != 0);

  return result;
}

/* end of file */


