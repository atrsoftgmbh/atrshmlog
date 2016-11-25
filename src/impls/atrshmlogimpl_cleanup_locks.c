
#include "../atrshmlog_internal.h"


/*******************************************************************/

/**
 * \file atrshmlogimpl_cleanup_locks.c
 */

/** 
 * \n Main code:
 *
 * \brief We have to clean up the mutexes after stopping 
 * in the pthread version.
 *
 * This is an optional step in shuting down the log.
 * It is itself not mt safe.
 * So use a simple program for that
 * see the atrshmlogfinish.c
 *
 * test t_cleanup_locks.c
 */
void atrshmlog_cleanup_locks(volatile const void *i_area)
{
  ATRSHMLOGSTAT(atrshmlog_counter_cleanup_locks);

  if (! ATRSHMLOG_LOGGING )
    return;

  /* Next. We check for initialized */
  atrshmlog_area_t *a = (atrshmlog_area_t *)i_area;

  if (a->shmid != atrshmlog_get_shmid())
    return;

  if (a->shmsafeguard != ATRSHMLOGSAFEGUARDVALUE)
    return;

  if (a->shmversion != ATRSHMLOGVERSION)
    return;
  
  atrshmlog_buffer_t* b = &(a->logbuffers[0]);

  /* From now on the buffer is useless. So we switch to not logging */
  atomic_store(&a->ich_habe_fertig,  1);

  sleep(5); /* We give 5 seconds time for the reader to finish */
  
  for (int i = 0; i < a->shmcount; i++)
    {
      atomic_store(&b->state, atrshmlog_illegal); /* no leagal state, no op any more */

      b++;
    }

  b = &(a->logbuffers[0]);
  
  for (int i = 0; i < a->shmcount; i++)
    {
      b->safeguard = 0L;
      
      b++;
    }

  a->shmid = 0;

  if (atrshmlog_thread_fence_11)
    atomic_thread_fence(memory_order_release); 
  
  a->shmsafeguard = 0L;

  a->shmcount = 0;

  a->shmversion = 0;
}

