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

#include "tests.h"



int main (int argc, char *argv[])
{
  printf("%s\n", argv[0]);

  for (int __i = 1; __i < argc; __i++)
    printf("arg %d : %s : \n", __i, argv[__i]);
  
  if(argc != 2)
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

  #if  ATRSHMLOG_PLATFORM_MINGW_X86_64_GCC == 0

  system("pstree");
  system(argv[2]); // ps -elf H on my box ...

  #endif
  
  printf("\n");

  return 0;
}

/* end of file */
