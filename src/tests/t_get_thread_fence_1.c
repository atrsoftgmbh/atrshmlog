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

/** \file t_get_thread_fence_1.c
 * \brief We are a test program.
 */

#include "tests.h"

int main (int argc, char *argv[])
{
  printf("%s\n", argv[0]);

  for (int __i = 1; __i < argc; __i++)
    printf("arg %d : %s : \n", __i, argv[__i]);
  
  if (argc != 2)
    exit(1);
  
  // we start without slaves ...
  atrshmlog_set_f_list_buffer_slave_count(0);
  
  atrshmlog_ret_t ret = atrshmlog_attach();

  if (ret != 0)
    {
      printf("%d\n", ret);
      exit(1);
    }
  
  PN(atrshmlog_thread_fence_1);

  PN(atrshmlog_get_thread_fence_1());

  atrshmlog_thread_fence_1 = strtol(argv[1], NULL, 10);

  PN(atrshmlog_thread_fence_1);

  PN(atrshmlog_get_thread_fence_1());
  
  printf("\n");

  return 0;
}

/* end of file */
