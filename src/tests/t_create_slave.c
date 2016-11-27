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

/** \file t_create_slave.c
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
  
  int limit = strtol(argv[1], NULL, 10);

  int ret = atrshmlog_attach();

  if (ret != 0)
    {
      printf("%d\n", ret);
      exit(1);
    }

  for (int index = 0; index < limit; index++)
    {
      int sret =  atrshmlog_create_slave();

      printf("%d :%d \n", index, sret);
    }

  sleep(1);

  system("pstree");
  system(argv[2]); // ps -elf H on my box ...
  
  printf("\n");

  return 0;
}

/* end of file */
