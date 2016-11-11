

#include "../atrshmlog_internal.h"

/**
 * \file atrshmlogimpl_get_raw_buffers.c
 */

/**
 * \n Main code:
 *
 * \brief We get memory from the dyn memory area.
 *
 * We are a raw helper, so we have no lock because we are inside 
 * a lock.
 *
 * We get memory for buffers and descriptors and give it back.
 * No list stuff is done so far.
 *
 * \param i_buffer_count
 * The count of buffers we want
 *
 * \param i_buffer_size
 * The size of the buffer.
 *
 * \return
 * The pointer for the first buffer
 */
atrshmlog_tbuff_t* atrshmlog_il_get_raw_buffers(const int i_buffer_count,
					     const int i_buffer_size)
{
  const int save = 16;

  ATRSHMLOGSTAT(atrshmlog_counter_get_raw);
  
  atrshmlog_tbuff_t* n;

  if(atrshmlog_init_buffers_in_advance)
    n = calloc(1, i_buffer_count  * (sizeof(atrshmlog_tbuff_t) + i_buffer_size + save));
  else
    n = malloc(i_buffer_count  * (sizeof(atrshmlog_tbuff_t) + i_buffer_size + save));

  if ( n != 0)
     atrshmlog_acquire_count += i_buffer_count;
   
  return n;
}

