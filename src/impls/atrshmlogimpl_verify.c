
#include "../atrshmlog_internal.h"


/*******************************************************************/

/**
 * \file atrshmlogimpl_verify.c
 */

/** 
 * \n Main code:
 *
 * \brief We verify the buffer is ok.
 *
 * We check the shmid and we check the states.
 * 
 * This can not be used in mt case, so use it only for simple checker 
 * programs.
 *
 * A basic version is already in the atrshmlogverify.c
 */
atrshmlog_ret_t atrshmlog_verify(void)
{
  ATRSHMLOGSTAT(atrshmlog_counter_verify);

  int result = -1;
  
  if ( ! ATRSHMLOG_LOGGING )
    return atrshmlog_error_verify_1;

  atrshmlog_area_t * a = ATRSHMLOG_GETAREA;
    
  int i = 0;

  int limit = 0;
    
  --result;

  if (atrshmlog_thread_fence_9)
    atomic_thread_fence(memory_order_acquire); 

  if (a->shmid != atrshmlog_get_shmid())
    return atrshmlog_error_verify_2;

  --result;

  if (a->shmsafeguard != ATRSHMLOGSAFEGUARDVALUE)
    return atrshmlog_error_verify_3;
    
  --result;

  limit = a->shmcount;
    
  if (a->shmversion != ATRSHMLOGVERSION)
    {
#if ATRSHMLOGDEBUG == 1      
      printf("version error\n");
#endif
      return atrshmlog_error_verify_4;
    }
    
  for (i = 0; i < limit ; i++)
    {
      int state = atomic_load(&(a->logbuffers[i].state));

      --result;
    
      if (!(state == atrshmlog_writeable
        || state == atrshmlog_write_halffull
        || state == atrshmlog_full ))
	return atrshmlog_error_verify_5;
      
      --result;

      if (a->logbuffers[i].safeguard != ATRSHMLOGSAFEGUARDVALUE)
	return atrshmlog_error_verify_6;

      
      /* TODO: 
       * check the list of info data too 
       * perhaps we will do this in the future.
       * for now we try to be not paranoid and think
       * the content is ok if all the numbers and safguards
       * are ok...
       */
    }

  return atrshmlog_error_ok;
}

