
#include "../atrshmlog_internal.h"

/***************************************************************/

/**
 * \file atrshmlogimpl_change_order.c
 */

/** 
 * \n Main code:
 *
 * \brief We get the tid of a thread local
 *
 * test t_get_tid.c
 */
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
  h->endtime = atrshmlog_int64_change_order(h->endtime);
  h->deltatime = atrshmlog_int64_change_order(h->deltatime);
  h->startreal = atrshmlog_int64_change_order(h->startreal);
  h->endreal = atrshmlog_int64_change_order(h->endreal);
  h->deltareal = atrshmlog_int64_change_order(h->deltareal);
  h->totallen = atrshmlog_int32_change_order(h->totallen);
  h->eventnumber = atrshmlog_int32_change_order(h->eventnumber);
  h->userflag = atrshmlog_int32_change_order(h->userflag);
  // eventflag is char, paylaod ist char...
}
