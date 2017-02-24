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

/** \file t_turn_slave_off.c
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

  if(argc != 2)
    exit(2);

  int target = strtol(argv[1], NULL, 10);
  
  sleep(1);

  PN(atomic_load(&atrshmlog_f_list_active_slaves));

  int i = 0;
  
  const volatile void* s = atrshmlog_get_next_slave_local(NULL);

  const volatile void* t = 0;
  
  while (s )
    {
      const volatile atrshmlog_slave_t* slave = s;

      printf("%d : self %p tid %ld  next %p local %p\n", i, slave, (long)slave->tid, slave->next, slave->g);

      PN(atrshmlog_get_slave_tid(slave));

      if (i == target)
	t = slave;
      
      s = atrshmlog_get_next_slave_local(s);
      
      i++;
    }

  if (t)
    {
      atrshmlog_turn_slave_off(t);

      const volatile atrshmlog_g_tl_t* g  =  ((atrshmlog_slave_t*)t)->g;

      PN(g->atrshmlog_idnotok);
      
      sleep(1);

      {
	int i = 0;
  
	const volatile void* s = atrshmlog_get_next_slave_local(NULL);

	while (s )
	  {
	    const volatile atrshmlog_slave_t* slave = s;

	    printf("%d : self %p tid %ld  next %p local %p\n", i, slave, (long)slave->tid, slave->next, slave->g);

	    PN(atrshmlog_get_slave_tid(slave));

	    s = atrshmlog_get_next_slave_local(s);
      
	    i++;
	  }
      }
    }
  
  printf("\n");

  return 0;
}

/* end of file */
