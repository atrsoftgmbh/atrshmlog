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

/** \file t_get_logging.c
 * \brief We are a test program.
 */

#include "../atrshmlog.h"

#include "../atrshmlog_internal.h"

/*
 * ------------------------------------------------------------
 */

#include <stdio.h>

#include <stdlib.h>


#define PN(_x) printf( #_x " : %ld :\n", (long) _x)

#define PS(_x) printf( #_x " : %s :\n", _x);

#define PP(_x) printf( #_x " : %p :\n", (void*)_x);


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

  PN(ATRSHMLOG_LOGGING);
  
  PN(atrshmlog_thread_fence_10);

  atrshmlog_area_t * a = ATRSHMLOG_GETAREA;

  PN(a->ich_habe_fertig);

  PN(atrshmlog_logging_process_off_final);

  PN(atrshmlog_logging_process);

  PN(atrshmlog_get_logging());
  
  atrshmlog_logging_process = 0;
  
  PN(atrshmlog_get_logging());

  atrshmlog_logging_process_off_final = 1;

  PN(atrshmlog_get_logging());

  a->ich_habe_fertig = 1;

  PN(atrshmlog_get_logging());

  atrshmlog_base_ptr = 0;

  PN(atrshmlog_get_logging());

  printf("\n");

  return 0;
}

/* end of file */
