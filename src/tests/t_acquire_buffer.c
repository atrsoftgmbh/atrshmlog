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

/** \file t_acquire_buffer.c
 * \brief We are a  test program.
 */

#include "../atrshmlog.h"

#include "../atrshmlog_internal.h"

/*
 * ------------------------------------------------------------
 */

#include <stdio.h>



/**
 * attach and make some logging
 *
 * this is a testing programm to check for write errors...
 * 
 * simply see it and then try it...
 *
 */
int main (int argc, char *argv[])
{
  printf("%s\n", argv[0]);
  
  for (int __i = 1; __i < argc; __i++)
    printf("arg %d : %s : \n", __i, argv[__i]);
  
  atrshmlog_g_tl_t * g =  (atrshmlog_g_tl_t *)atrshmlog_get_thread_locals_adress();

  if (g == NULL)
    {
      printf("%s : thread local null \n", argv[0]);
    }

  int limit = 1000;

  if (argc > 1)
    limit = strtol(argv[1], NULL, 10);
  
  for (int i = 0; i < limit; i++)
    {
      atrshmlog_tbuff_t* t = atrshmlog_acquire_buffer(g);

      printf("%p %ld %ld\n", t, (long)t->id, (long)t->acquiretime);
    }

  printf("\n");

  return 0;
}

/* end of file */
