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

/** \file t_set_fence_8.c
 * \brief We are a test program.
 */

#include "tests.h"


int main (int argc, char *argv[])
{
  printf("%s\n", argv[0]);

  for (int __i = 1; __i < argc; __i++)
    printf("arg %d : %s : \n", __i, argv[__i]);
  
  atrshmlog_attach();
  
  PN(atrshmlog_get_thread_fence_8());

  PN(atrshmlog_set_thread_fence_8(1));
  
  PN(atrshmlog_get_thread_fence_8());

  PN(atrshmlog_set_thread_fence_8(0));
  
  PN(atrshmlog_get_thread_fence_8());
  
  printf("\n");

  return 0;
}

/* end of file */
