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

#include "tests.h"



int main (int argc, char *argv[])
{
  printf("%s\n", argv[0]);

  for (int __i = 1; __i < argc; __i++)
    printf("arg %d : %s : \n", __i, argv[__i]);
  
#if ATRSHMLOG_PLATFORM_MINGW_X86_64_GCC == 1
  
  if(argc != 3)
    exit(1);
  
  int index = strtol(argv[1], NULL, 10);

  int size =  strtol(argv[2], NULL, 10);

  int already;
  
  int ret = atrshmlog_create_mapped_file(index, size, &already);
  
  printf("%d\n", ret);

#endif
  
  printf("\n");

  return 0;
}

/* end of file */
