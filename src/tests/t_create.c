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

/** \file t_create.c
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

  if(argc != 3)
    exit(1);
  
  int key = strtol(argv[1], NULL, 10);

  int count = strtol(argv[2], NULL, 10);
  
  atrshmlog_ret_t ret = atrshmlog_create(key,count);

  if (ret <= 0)
    {
      printf("%d\n", ret);
      exit(1);
    }


  printf("id %ld\n", (long)ret);

  return 0;
}

/* end of file */
