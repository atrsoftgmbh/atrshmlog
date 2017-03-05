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

/** \file t_transfer_mem_to_shm.c
 * \brief We are a test program.
 */

#include "../atrshmlog.h"

#include "../atrshmlog_internal.h"

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

  atrshmlog_area_t * a = ATRSHMLOG_GETAREA;

  
  PN(atrshmlog_f_list_buffer_slave_count);

  PN(atomic_load(&a->shma));


  int r = ATRSHMLOG_WRITE0(1, 'P', 42, 0, 0);

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

  atrshmlog_tbuff_t* t = g->atrshmlog_targetbuffer_arr[0];

  PN(t->id);
  PN(t->size);
  
  PN(atomic_load(&a->shma));

  atrshmlog_dispatch_buffer(t);

  PN(atomic_load(&a->shma));

  PN(t->chksum);

  {
    int available = atomic_load(&a->shma);

    PN(atomic_load(&a->shma));
    PN(available);

    while (available != -1)
      {
	available = a->logbuffers[available].next_append;
	PN(available);
      }
  }
  
  PN(atomic_load(&a->shma));

  int rettm = atrshmlog_transfer_mem_to_shm(t, g);

  PN(rettm);

  PN(atomic_load(&a->shma));

  PN(atomic_load(&atrshmlog_counter[atrshmlog_counter_mem_to_shm]));

  if (rettm >= 0)
    {
      atrshmlog_buffer_t* b = &(a->logbuffers[rettm - 1000]);

      PN(b->id);
      PN(b->chksum);
  
    }
  
  {
    int available = atomic_load(&a->shma);

    PN(available);

    while (available != -1)
      {
	available = a->logbuffers[available].next_append;
	PN(available);
      }
  }

  printf("\n");

  return 0;
}

/* end of file */
