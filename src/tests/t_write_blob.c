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

/** \file t_write_blob.c
 * \brief We are a test program.
 */

#include "../atrshmlog.h"

#include "../atrshmlog_internal.h"

#include <string.h>

/*
 * ------------------------------------------------------------
 */

#include <stdio.h>

#include <stdlib.h>


#define PN(_x) printf( #_x " : %ld :\n", (long) _x)

#define PS(_x) printf( #_x " : %s :\n", _x);

#define PP(_x) printf( #_x " : %p :\n", (void*)_x);


int main (int argc, char *argv[])
{
  printf("%s\n", argv[0]);

  for (int __i = 1; __i < argc; __i++)
    printf("arg %d : %s : \n", __i, argv[__i]);
  
  // we start without slaves ...
  atrshmlog_set_f_list_buffer_slave_count(0);
  
  atrshmlog_ret_t ret = atrshmlog_attach();

  if (ret != 0)
    {
      printf("%d\n", ret);
      exit(1);
    }

  PP(atrshmlog_base_ptr);

  PN(atrshmlog_event_locks_max);
  PN(atrshmlog_thread_fence_2);

  char buffer[32767];

  memset(buffer, 'A', sizeof(buffer));

  
  int r = ATRSHMLOG_WRITE(1, 'p', 42, 0, 0, buffer, sizeof( buffer));

  PN(r);
 
  const volatile atrshmlog_g_tl_t* g = ATRSHMLOG_GET_THREAD_LOCALS_ADRESS();

  if (g == NULL)
    {
      exit(1);
    }

  if (g->atrshmlog_idnotok)
    exit(2);

  PP(g);

  PN(g->atrshmlog_targetbuffer_index);
  
  atrshmlog_tbuff_t* t = g->atrshmlog_targetbuffer_arr[g->atrshmlog_targetbuffer_index];

  PP(t);

  PP(t->next_cleanup);
  
  PP(t->next_full);

  PP(t->next_append);

  PN(t->safeguardfront);

  PN(t->pid);

  PN(t->tid);

  PN(t->acquiretime);

  PN(t->id);

  PN(t->chksum);

  PN(t->size);

  PN(t->maxsize);

  PN(t->dispose);

  PN(atomic_load(&t->dispatched));

  PN(t->number_dispatched);

  PN(t->counter_write0);

  PN(t->counter_write0_discard);

  PN(t->counter_write0_wait);

  PN(t->counter_write0_adaptive);

  PN(t->counter_write0_adaptive_fast);

  PN(t->counter_write0_adaptive_very_fast);
  
  PN(t->counter_write1);

  PN(t->counter_write1_discard);

  PN(t->counter_write1_wait);

  PN(t->counter_write1_adaptive);

  PN(t->counter_write1_adaptive_fast);

  PN(t->counter_write1_adaptive_very_fast);
  
  PN(t->counter_write2);

  PN(t->counter_write2_discard);

  PN(t->counter_write2_wait);

  PN(t->counter_write2_adaptive);

  PN(t->counter_write2_adaptive_fast);

  PN(t->counter_write2_adaptive_very_fast);
  
  printf("\n");

  return 0;
}

/* end of file */
