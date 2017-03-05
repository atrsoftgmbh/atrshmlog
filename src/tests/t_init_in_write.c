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

/** \file t_init_in_write.c
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

  const volatile atrshmlog_g_tl_t *g = atrshmlog_get_thread_locals_adress();

  if (!g)
    exit(2);
  
  int rettl = atrshmlog_init_in_write((atrshmlog_g_tl_t *)g);

  if (rettl != 0)
    exit(3);
  
  PN(atrshmlog_get_thread_local_tid (g));

  PN(g->atrshmlog_thread_pid);

  PN(g->atrshmlog_targetbuffer_index);

  for (int i = 0; i < ATRSHMLOGTARGETBUFFERMAX; i++)
    {
      PN(i);
      PP(g->atrshmlog_targetbuffer_arr[i]);
    }
  
  printf("\n");

  return 0;
}

/* end of file */
