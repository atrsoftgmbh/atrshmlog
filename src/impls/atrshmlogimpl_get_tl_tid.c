
#include "../atrshmlog_internal.h"

/***************************************************************/

/**
 * \file atrshmlogimpl_get_tl_tid.c
 */

/** 
 * \n Main code:
 *
 * \brief We get the tid of a thread local
 *
 * test t_get_tid.c
 */
atrshmlog_tid_t atrshmlog_get_thread_local_tid (volatile const void *i_local)
{
  atrshmlog_g_tl_t *g = (atrshmlog_g_tl_t*) i_local;

  if (g == NULL)
    return 0;

  return g->atrshmlog_thread_tid;
}

atrshmlog_pid_t atrshmlog_get_thread_local_pid (volatile const void *i_local)
{
  atrshmlog_g_tl_t *g = (atrshmlog_g_tl_t*) i_local;

  if (g == NULL)
    return 0;

  return g->atrshmlog_thread_pid;
}

atrshmlog_ret_t atrshmlog_get_thread_local_index (volatile const void *i_local)
{
  atrshmlog_g_tl_t *g = (atrshmlog_g_tl_t*) i_local;

  if (g == NULL)
    return 0;

  return g->atrshmlog_targetbuffer_index;
}

volatile const void *atrshmlog_get_thread_local_buffer (volatile const void *i_local, atrshmlog_int32_t i_index)
{
  atrshmlog_g_tl_t *g = (atrshmlog_g_tl_t*) i_local;

  if (g == NULL || i_index >= ATRSHMLOGTARGETBUFFERMAX || i_index < 0)
    return 0;

  return g->atrshmlog_targetbuffer_arr[i_index];
}

volatile const void *atrshmlog_get_thread_buffer_next_cleanup (volatile const void *i_buffer)
{
  atrshmlog_tbuff_t* b = ( atrshmlog_tbuff_t* ) i_buffer;
  if (b == NULL)
    return 0;

  return b->next_cleanup;
}

volatile const void *atrshmlog_get_thread_buffer_next_full (volatile const void *i_buffer)
{
  atrshmlog_tbuff_t* b = ( atrshmlog_tbuff_t* ) i_buffer;
  if (b == NULL)
    return 0;

  return b->next_full;
}

volatile const void *atrshmlog_get_thread_buffer_next_append (volatile const void *i_buffer)
{
  atrshmlog_tbuff_t* b = ( atrshmlog_tbuff_t* ) i_buffer;
  if (b == NULL)
    return 0;

  return b->next_append;
}

atrshmlog_ret_t atrshmlog_get_thread_buffer_safeguard (volatile const void *i_buffer)
{
  atrshmlog_tbuff_t* b = ( atrshmlog_tbuff_t* ) i_buffer;
  if (b == NULL)
    return 0;

  return b->safeguardfront;
}

atrshmlog_pid_t atrshmlog_get_thread_buffer_pid (volatile const void *i_buffer)
{
  atrshmlog_tbuff_t* b = ( atrshmlog_tbuff_t* ) i_buffer;
  if (b == NULL)
    return 0;

  return b->pid;
}

atrshmlog_tid_t atrshmlog_get_thread_buffer_tid (volatile const void *i_buffer)
{
  atrshmlog_tbuff_t* b = ( atrshmlog_tbuff_t* ) i_buffer;
  if (b == NULL)
    return 0;

  return b->tid;
}

atrshmlog_time_t atrshmlog_get_thread_buffer_acquiretime (volatile const void *i_buffer)
{
  atrshmlog_tbuff_t* b = ( atrshmlog_tbuff_t* ) i_buffer;
  if (b == NULL)
    return 0;

  return b->acquiretime;
}

atrshmlog_ret_t atrshmlog_get_thread_buffer_id (volatile const void *i_buffer)
{
  atrshmlog_tbuff_t* b = ( atrshmlog_tbuff_t* ) i_buffer;
  if (b == NULL)
    return 0;

  return b->id;
}

atrshmlog_ret_t atrshmlog_get_thread_buffer_chksum (volatile const void *i_buffer)
{
  atrshmlog_tbuff_t* b = ( atrshmlog_tbuff_t* ) i_buffer;
  if (b == NULL)
    return 0;

  return b->chksum;
}

atrshmlog_ret_t atrshmlog_get_thread_buffer_size (volatile const void *i_buffer)
{
  atrshmlog_tbuff_t* b = ( atrshmlog_tbuff_t* ) i_buffer;
  if (b == NULL)
    return 0;

  return b->size;
}

atrshmlog_ret_t atrshmlog_get_thread_buffer_maxsize (volatile const void *i_buffer)
{
  atrshmlog_tbuff_t* b = ( atrshmlog_tbuff_t* ) i_buffer;
  if (b == NULL)
    return 0;

  return b->maxsize;
}

atrshmlog_ret_t atrshmlog_get_thread_buffer_dispose (volatile const void *i_buffer)
{
  atrshmlog_tbuff_t* b = ( atrshmlog_tbuff_t* ) i_buffer;
  if (b == NULL)
    return 0;

  return b->dispose;
}

atrshmlog_ret_t atrshmlog_get_thread_buffer_dispatched (volatile const void *i_buffer)
{
  atrshmlog_tbuff_t* b = ( atrshmlog_tbuff_t* ) i_buffer;
  if (b == NULL)
    return 0;

  return atomic_load(&b->dispatched);
}

volatile const void *atrshmlog_get_thread_buffer_payload (volatile const void *i_buffer)
{
  atrshmlog_tbuff_t* b = ( atrshmlog_tbuff_t* ) i_buffer;
  if (b == NULL)
    return 0;

  return b->b;
}

void atrshmlog_fill_chunk_head (volatile const void *i_chunk, atrshmlog_chunk_head_t* c)
{
  char *h = (char*)i_chunk;
  
  memcpy(&c->starttime, h + 0, sizeof(atrshmlog_time_t));
  memcpy(&c->endtime, h + sizeof(atrshmlog_time_t) , sizeof(atrshmlog_time_t));
  

  memcpy(&c->totallen, h + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_time_t) , sizeof(atrshmlog_int32_t));
      
  memcpy(&c->eventnumber, h + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_int32_t), sizeof(atrshmlog_int32_t));
    
  memcpy(&c->userflag, h + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_int32_t) + sizeof(atrshmlog_int32_t), sizeof(atrshmlog_int32_t));

  c->eventflag = *((char*)h + ATRSHMLOGCONTROLDATASIZE - 1);

}


atrshmlog_int32_t atrshmlog_int32_change_order(atrshmlog_int32_t v)
{
  atrshmlog_int32_t r;

  unsigned char* t = (unsigned char*)&r;
  unsigned char* s = (unsigned char*)&v;

  t[0] = s[3];
  t[1] = s[2];
  t[2] = s[1];
  t[3] = s[0];

  
  return r;
}

uint64_t atrshmlog_int64_change_order(uint64_t v)
{
  uint64_t r;

  unsigned char* t = (unsigned char*)&r;
  unsigned char* s = (unsigned char*)&v;

  t[0] = s[7];
  t[1] = s[6];
  t[2] = s[5];
  t[3] = s[4];
  t[4] = s[3];
  t[5] = s[2];
  t[6] = s[1];
  t[7] = s[0];

  return r;
}

void atrshmlog_io_head_change_order(atrshmlog_io_head_t* h)
{
  unsigned char swaper1 = h->order[0];
  h->order[0] = h->order[1];
  h->order[1] = swaper1;

  h->version = atrshmlog_int32_change_order(h->version);
  h->tlen = atrshmlog_int32_change_order(h->tlen);
  h->pid = atrshmlog_int64_change_order(h->pid);
  h->tid = atrshmlog_int64_change_order(h->tid);
  h->buffernumber = atrshmlog_int32_change_order(h->buffernumber);
  h->filenumber = atrshmlog_int32_change_order(h->filenumber);
  h->inittime.tv_sec = atrshmlog_int64_change_order(h->inittime.tv_sec);
  h->inittime.tv_nsec = atrshmlog_int32_change_order(h->inittime.tv_nsec);
  h->inittsc_before = atrshmlog_int64_change_order(h->inittsc_before);
  h->inittsc_after = atrshmlog_int64_change_order(h->inittsc_after);
  h->lasttime.tv_sec = atrshmlog_int64_change_order(h->lasttime.tv_sec);
  h->lasttime.tv_nsec = atrshmlog_int32_change_order(h->lasttime.tv_nsec);
  h->lasttsc_before = atrshmlog_int64_change_order(h->lasttsc_before);
  h->lasttsc_after = atrshmlog_int64_change_order(h->lasttsc_after);
  h->difftimetransfer = atrshmlog_int64_change_order(h->difftimetransfer);
  h->starttransfer = atrshmlog_int64_change_order(h->starttransfer);
  h->acquiretime = atrshmlog_int64_change_order(h->acquiretime);
  h->id = atrshmlog_int32_change_order(h->id);
  h->number_dispatched = atrshmlog_int32_change_order(h->number_dispatched);
  h->counter_write0 = atrshmlog_int32_change_order(h->counter_write0);
  h->counter_write0_discard = atrshmlog_int32_change_order(h->counter_write0_discard);
  h->counter_write0_wait = atrshmlog_int32_change_order(h->counter_write0_wait);
  h->counter_write0_adaptive = atrshmlog_int32_change_order(h->counter_write0_adaptive);
  h->counter_write0_adaptive_fast = atrshmlog_int32_change_order(h->counter_write0_adaptive_fast);
  h->counter_write0_adaptive_very_fast = atrshmlog_int32_change_order(h->counter_write0_adaptive_very_fast);
  h->counter_write1 = atrshmlog_int32_change_order(h->counter_write1);
  h->counter_write1_discard = atrshmlog_int32_change_order(h->counter_write1_discard);
  h->counter_write1_wait = atrshmlog_int32_change_order(h->counter_write1_wait);
  h->counter_write1_adaptive = atrshmlog_int32_change_order(h->counter_write1_adaptive);
  h->counter_write1_adaptive_fast = atrshmlog_int32_change_order(h->counter_write1_adaptive_fast);
  h->counter_write1_adaptive_very_fast = atrshmlog_int32_change_order(h->counter_write1_adaptive_very_fast);
  h->counter_write2 = atrshmlog_int32_change_order(h->counter_write2);
  h->counter_write2_discard = atrshmlog_int32_change_order(h->counter_write2_discard);
  h->counter_write2_wait = atrshmlog_int32_change_order(h->counter_write2_wait);
  h->counter_write2_adaptive = atrshmlog_int32_change_order(h->counter_write2_adaptive);
  h->counter_write2_adaptive_fast = atrshmlog_int32_change_order(h->counter_write2_adaptive_fast);
  h->counter_write2_adaptive_very_fast = atrshmlog_int32_change_order(h->counter_write2_adaptive_very_fast);

}

void atrshmlog_chunk_head_change_order(atrshmlog_chunk_head_t* h)
{
  h->starttime = atrshmlog_int64_change_order(h->starttime);
  h->endtime = atrshmlog_int32_change_order(h->endtime);
  h->totallen = atrshmlog_int32_change_order(h->totallen);
  h->eventnumber = atrshmlog_int32_change_order(h->eventnumber);
  h->userflag = atrshmlog_int32_change_order(h->userflag);
  // eventflag is char
}
