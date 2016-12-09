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

/** \file t_reflexion.c
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
  
  atrshmlog_ret_t ret = atrshmlog_attach();

  if (ret != 0)
    {
      printf("%d\n", ret);
      exit(1);
    }


  ATRSHMLOG_WRITE0(1, 'P', 42, 0, 0);

  ATRSHMLOG_FLUSH();

  ATRSHMLOG_WRITE0(1, 'P', 42, 0, 0);

  ATRSHMLOG_WRITE0(1, 'P', 42, 0, 0);

  sleep(1);

  volatile const void* tl = atrshmlog_get_thread_locals_adress();
  
  PP(tl);

  PN(atrshmlog_get_thread_local_tid(tl));

  PN(atrshmlog_get_thread_local_pid(tl));

  int index = atrshmlog_get_thread_local_index(tl);

  PN(index);

  for (int i = 0; i < ATRSHMLOGTARGETBUFFERMAX; i++)
    {
      PN(i);
      volatile const void* b = atrshmlog_get_thread_local_buffer(tl, i);

      PP(b);

      if (b)
	{
	  PP(atrshmlog_get_thread_buffer_next_cleanup(b));
	  PP(atrshmlog_get_thread_buffer_next_full(b));
	  PP(atrshmlog_get_thread_buffer_next_append(b));

	  PN(atrshmlog_get_thread_buffer_safeguard(b));
	  PN(atrshmlog_get_thread_buffer_tid(b));
	  PN(atrshmlog_get_thread_buffer_pid(b));
	  PN(atrshmlog_get_thread_buffer_acquiretime(b));
	  PN(atrshmlog_get_thread_buffer_id(b));
	  PN(atrshmlog_get_thread_buffer_chksum(b));
	  PN(atrshmlog_get_thread_buffer_size(b));
	  PN(atrshmlog_get_thread_buffer_maxsize(b));
	  PN(atrshmlog_get_thread_buffer_dispose(b));
	  PN(atrshmlog_get_thread_buffer_dispatched(b));
	  PP(atrshmlog_get_thread_buffer_payload(b));
	}
    }
  
  printf("\n");

  return 0;
}

/* end of file */
