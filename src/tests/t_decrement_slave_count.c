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

/** \file t_decrement_slave_count.c
 * \brief We are a test program.
 */

#include "../atrshmlog.h"

#include "../atrshmlog_internal.h"

/*
 * ------------------------------------------------------------
 */

#include <stdio.h>


#define PN(_x) printf( #_x " : %ld :\n", (long) _x)

#define PS(_x) printf( #_x " : %s :\n", _x);

#define PP(_x) printf( #_x " : %p :\n", _x);

int main (int argc, char *argv[])
{
  printf("%s\n", argv[0]);

  for (int __i = 1; __i < argc; __i++)
    printf("arg %d : %s : \n", __i, argv[__i]);
  
  if(argc != 3)
    exit(1);
  
  int newcount = strtol(argv[1], NULL, 10);

  atrshmlog_f_list_active_slaves = newcount;

  int limit = strtol(argv[2], NULL, 10);
  
  for (int index = 0; index < limit; index++)
    {
      PN(index);
      PN(atrshmlog_f_list_active_slaves);
      int ret = atrshmlog_decrement_slave_count();
      PN(ret);
      PN(atrshmlog_f_list_active_slaves);
    }

  printf("\n");

  return 0;
}

/* end of file */
