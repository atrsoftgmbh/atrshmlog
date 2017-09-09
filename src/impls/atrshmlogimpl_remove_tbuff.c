
#include "../atrshmlog_internal.h"

/*******************************************************************/

/**
 * \file atrshmlogimpl_remove_tbuff.c
 */

/** 
 * \n Main code:
 *
 * \brief We remove the buffer from the threads buffer list.
 *
 * If we detect a empty buffer list we also swith it to not ok logging.
 *
 * test t_turn_logging_off.c
 */
void atrshmlog_remove_tbuff(atrshmlog_g_tl_t* i_g, int i)
{
  if (i_g == NULL)
    return;

  if (i < 0)
    return;

  if (i_g->atrshmlog_targetbuffer_count <= i)
    return;

  // special case : we are the last buffer ....
  if (i_g->atrshmlog_targetbuffer_count == 1)
    {
      i_g->atrshmlog_targetbuffer_count = 0;

      i_g->atrshmlog_targetbuffer_index = 0;
      
      // we switch logging of for this thread
      i_g->atrshmlog_idnotok = 1;

      return;
    }

  // normal case : we have at least two buffers ...
  // so we copy the ptr from i + 1 to i and so on till end
  for (int k = i + 1; k < i_g->atrshmlog_targetbuffer_count; ++k)
    {
      i_g->atrshmlog_targetbuffer_arr[k-1] = i_g->atrshmlog_targetbuffer_arr[k];
    }

  // ok. now we have to correct the count
  --i_g->atrshmlog_targetbuffer_count;

  // and now we have to check for a valid index too
  if (i_g->atrshmlog_targetbuffer_count <= i_g->atrshmlog_targetbuffer_index)
    {
      i_g->atrshmlog_targetbuffer_index = 0;
    }
}

