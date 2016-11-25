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

/** \file t_delete.c
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

#if ATRSHMLOG_PLATFORM_MINGW_X86_64_GCC == 1
  
  if(argc != 3)
    exit(1);
  
  int index = strtol(argv[1], NULL, 10);

  int size =  strtol(argv[2], NULL, 10);

  int already;
  
  int ret = atrshmlog_create_mapped_file(index, size, &already);
  
  printf("%d\n", ret);

#endif
  
  return 0;
}

/* end of file */
