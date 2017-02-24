
#include "../atrshmlog_internal.h"


/*******************************************************************/

/**
 * \file atrshmlogimpl_init_shm_log.c
 */

/** 
 * \n Main code:
 *
 * \brief helper: we initialize one buffer
 *
 * \param i_shm
 * The adress of the shared memory buffer.
 *
 * \param i_index_buffer
 * The index of the buffer.
 *
 * \param i_chunkbase
 * The index in the shared memory for the chunk buffer.
 *
 * \return
 * \li 0 : ok
 * \li non zero : error
 */
static int atrshmlog_init_one_buffer(volatile atrshmlog_area_t *i_shm,
				     const int i_index_buffer,
				     const int i_chunkbase)
{
  int result = -1;
  
  volatile atrshmlog_buffer_t* b = &(i_shm->logbuffers[i_index_buffer]);

  int lock_wanted = 0;


  int count_users = 0;

  /* ok. we are through. we have to set the final infos and thats it */
  b->safeguard = ATRSHMLOGSAFEGUARDVALUE;

  atomic_init(&(b->state), atrshmlog_writeable);

  b->info = i_chunkbase;

  b->next_append = i_index_buffer + 1;

#if ATRSHMLOGDEBUG == 1
  printf("next shm is %ld and append is %ld\n", i_index_buffer, i_index_buffer + 1);
#endif
  b->next_full = -1;
  
  b->shmsize = 0;

  return atrshmlog_error_ok;
}

/*******************************************************************/


/** 
 * \n Main code:
 *
 * \brief We initialize the buffer after it is attached .
 *
 * This is not mt safe.
 *
 * So try to use an initialisation program that is
 * simple for it.
 *
 * See the atrshmloginit.c for this
 *
 * test t_init_shm_log.c
 */
atrshmlog_ret_t atrshmlog_init_shm_log(volatile const void * const i_area,
				       const atrshmlog_int32_t i_count_buffers)
{
  ATRSHMLOGSTAT(atrshmlog_counter_init_shm);

  /* We check first, then we initialize.
   * Its vital that we do NOT reinitialize the buffer ! 
   */
  
  if ( i_area == 0 )
    return  atrshmlog_error_init_shm_1;

  volatile atrshmlog_area_t *a = (volatile atrshmlog_area_t *)i_area;

  if (atrshmlog_thread_fence_12)
    atomic_thread_fence(memory_order_acquire); 
  
  if (a->shmid == atrshmlog_get_shmid())
    return atrshmlog_error_init_shm_2;

  if (a->shmsafeguard == ATRSHMLOGSAFEGUARDVALUE)
    return atrshmlog_error_init_shm_2;

  
  /* Ok. We are in. The shm seems not to be initialized by us */

  // Chunks are after the desciptors
  unsigned int chunks = (char*)&a->logbuffers[i_count_buffers] - (char*)a;

  chunks = (chunks + 15) & ~15;

  int bufferlen = (ATRSHMLOGBUFFER_INFOSIZE + 15) & ~15;
  
  for (int i = 0; i < i_count_buffers ; i++, chunks += ATRSHMLOGBUFFER_INFOSIZE)
    {
      int initresult = atrshmlog_init_one_buffer(a, i, chunks);

#if ATRSHMLOGDEBUG == 1
      printf("%p %lx\n", a, (long)chunks);
#endif

      if (initresult < 0)
	return atrshmlog_error_init_shm_3;

    }
  
  /* We finish the list of available buffers with this one 
   */
  a->logbuffers[ i_count_buffers - 1].next_append = -1;
       

  /* Ok. We can now set the final info in the buffer */
  a->shmversion = ATRSHMLOGVERSION;
  
  a->shmcount = i_count_buffers;

  /* We put the buffers on the available list
   */
  atomic_init(&a->shma, 0);
  
  /* There is no buffer on the full list now */
  atomic_init(&a->shmf, -1);

  /* The flag that stops system wide logging */
  a->ich_habe_fertig = 0;
  
  a->readerflag = 0;
  a->readerpid = 0;
  a->writerflag = 0;
  a->writerpid = 0;
  a->shmsafeguard = ATRSHMLOGSAFEGUARDVALUE;

  a->shmid = atrshmlog_get_shmid();

  if (atrshmlog_thread_fence_12)
    atomic_thread_fence(memory_order_release); 
  
  return atrshmlog_error_ok;
}
