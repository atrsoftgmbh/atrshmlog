
#include "../atrshmlog_internal.h"


/**
 * \file atrshmlogimpl_connect_buffers_list.c
 */

/**
 * \n Main code:
 *
 * \brief  We conect desriptors.
 *
 * Create the list of 
 * next_availabe buffers and then put it on the a list.
 *
 * We are an in lock function, so we dont lock ourself.
 * We are now a super push for evtl. multiple buffers.
 *
 * \param raw
 * The adress of the memory.
 *
 * \param i_buffer_count
 * The count of buffers.
 *
 * \param i_chunkbuffer
 * The start of the memory for the chunks.
 *
 * \param i_buffer_size
 * The size for a chunk.
 *
 * \return 
 * \li 0 : ok
 * \li non zero : error
 */
int atrshmlog_il_connect_buffers_list(atrshmlog_tbuff_t* const raw,
					     const int i_buffer_count,
					     char* i_chunkbuffer,
					     const int i_buffer_size)
{
  atrshmlog_tbuff_t* n = raw;

  if (n == 0)
    return atrshmlog_error_connect_1;
  
  if (i_buffer_count < 1 )
    return atrshmlog_error_connect_2;

  atrshmlog_tbuff_t* t = n;

  atrshmlog_tbuff_t* tnext = t + 1;

  // We connect the buffers and attach if needed the chunks
  for (int i = 0; i < i_buffer_count - 1; t++, tnext++ , i++)
    {
      t->next_append = tnext;
      t->next_cleanup = tnext;
      
      if (i_chunkbuffer)
	{
	  t->b = i_chunkbuffer;
	  t->id = ++atrshmlog_buffer_id;
	  t->maxsize = i_buffer_size;
	  t->size = 0;

	  atomic_init(&t->dispatched, 0);

	  i_chunkbuffer += i_buffer_size;
	}
    }

  // Now do it for the last buffer
  if (i_chunkbuffer)
    {
      t->b = i_chunkbuffer;
      t->id = ++atrshmlog_buffer_id;
      t->maxsize = i_buffer_size;
      t->size = 0;

      atomic_init(&t->dispatched, 0);
	  
      i_chunkbuffer += i_buffer_size;
    }
  
  // We push all members in one move on the s list for cleanup
  t->next_cleanup = (atrshmlog_tbuff_t*)atomic_load(&atrshmlog_tps);

  while (!atomic_compare_exchange_weak(&atrshmlog_tps, &t->next_cleanup, n))
    ;

  // We push all members in one move on the a list for use
  t->next_append = (atrshmlog_tbuff_t*)atomic_load(&atrshmlog_tpa);

  while (!atomic_compare_exchange_weak(&atrshmlog_tpa, &t->next_append, n))
    ;

  return atrshmlog_error_ok;
}

