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

/** \file t_exit_cleanup.c
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

  PN(atrshmlog_f_list_buffer_slave_count);

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
  
  atrshmlog_tbuff_t* t = g->atrshmlog_targetbuffer_arr[g->atrshmlog_targetbuffer_index];

  PP(t);

  PP(atrshmlog_base_ptr);
  
  PN(atomic_load(&atrshmlog_f_list_active_slaves));

  PN(atrshmlog_wait_for_slaves);

  PN(atrshmlog_thread_fence_10);
  
  atrshmlog_exit_cleanup ();

  PN(atomic_load(&atrshmlog_counter[atrshmlog_counter_exit_cleanup]));

  PN(atrshmlog_logging_process);

  PN(atrshmlog_f_list_buffer_slave_run);

  PN(atrshmlog_logging_process_off_final);
  
  printf("\n");

  return 0;
}

/* end of file */
