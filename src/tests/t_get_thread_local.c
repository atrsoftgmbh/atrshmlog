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

/** \file t_get_thread_local.c
 * \brief We are a test program.
 */

#include "tests.h"


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

  atrshmlog_write0(1,'P',42,0,0);


  
  const volatile atrshmlog_g_tl_t *g = atrshmlog_get_thread_locals_adress();

  PP(g);

  if (!g)
    exit(2);
  
  PN(atrshmlog_get_thread_local_tid (g));

  PN(atrshmlog_get_thread_local_pid (g));
  
  PN((long)atrshmlog_get_thread_local_index (g));

  PN(atrshmlog_get_thread_local_count (g));

  atrshmlog_write0(1,'P',42,0,0);

  PN(atrshmlog_get_thread_local_index (g));
  PN(g->atrshmlog_targetbuffer_index);

  PN(atrshmlog_get_thread_local_count (g));

  printf("\n");

  return 0;
}

/* end of file */
