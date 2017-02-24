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

/** \file t_reuse_thread_buffers.c
 * \brief We are a test program.
 */

#include "../atrshmlog.h"

#include "../atrshmlog_internal.h"

/*
 * ------------------------------------------------------------
 */

#include <stdio.h>

#include <stdlib.h>

/** All the rest of unix */
# include <unistd.h>


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

  ATRSHMLOG_WRITE0(0, 'P', 42, 0,0);
  
  const volatile atrshmlog_g_tl_t* g = ATRSHMLOG_GET_THREAD_LOCALS_ADRESS();

  if (g == NULL)
    {
      exit(1);
    }

  if (g->atrshmlog_idnotok)
    exit(2);

  for (int i = 0; i < ATRSHMLOGTARGETBUFFERMAX; i++)
    {
      atrshmlog_tbuff_t* t_ak = g->atrshmlog_targetbuffer_arr[i];
      PP(t_ak);
    }

  atrshmlog_tid_t tid = atrshmlog_get_tid();

  PN(tid);
  PN(getpid());

  int retre = atrshmlog_reuse_thread_buffers(tid);

  sleep(1);
  
  PN(retre);
  
  if (retre > 0)
    {
      for (int i = 0; i < ATRSHMLOGTARGETBUFFERMAX; i++)
	{
	  atrshmlog_tbuff_t* t_ak = g->atrshmlog_targetbuffer_arr[i];
	  PP(t_ak);
	}
      
      atrshmlog_tbuff_t* t = (atrshmlog_tbuff_t*)atomic_load(&atrshmlog_tpa);

      int i = 0;

      while (t)
	{
	  PN(i);
	  PP(t);

	  PN(t->pid);
	  PN(t->tid);
	  PN(t->id);
	  PN(t->size);
	  PN(t->chksum);
      
	  t = t->next_append;
	  i++;
	}
    }
  
  printf("\n");

  return 0;
}

/* end of file */
