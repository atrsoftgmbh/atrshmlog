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

/** \file t_buffer_slave_proc.c
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

  atrshmlog_ret_t ret = atrshmlog_attach();

  if (ret != 0)
    {
      printf("%d\n", ret);
      exit(1);
    }

  PN(atrshmlog_f_list_buffer_slave_count);


  sleep(5);

  PN(atomic_load(&atrshmlog_f_list_active_slaves));
  PP(atomic_load(&atrshmlog_tpslave));

  system ("pstree");
  
  sleep(5);

  int i = 0;
  
  void* s = atrshmlog_get_next_slave_local(NULL);

  while (s )
    {
      atrshmlog_slave_t* slave = s;

      printf("%d : self %p tid %ld  next %p local %p\n", i, slave, (long)slave->tid, slave->next, slave->g);
      s = atrshmlog_get_next_slave_local(s);
    }
  
  return 0;
}

/* end of file */
