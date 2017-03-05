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

/** \file t_write0.c
 * \brief We are a test program.
 */

#include "tests.h"

/** All the rest of unix */
# include <unistd.h>


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


  
  int r = ATRSHMLOG_WRITE0(1, 'P', 42, 0, 0);

  sleep(1);

  PN(atrshmlog_f_list_buffer_slave_count);

  atrshmlog_area_t *a = (atrshmlog_area_t *)ATRSHMLOG_GET_AREA();

  PP(a);
  
  a->writerflag = 0;
  
  a->writerpid = atrshmlog_attach_pid;

  a->writerflag = ATRSHMLOG_WRITER_SLAVE | 0;

  PN(a->writerpid);

  PN(a->writerflag);

  r = ATRSHMLOG_WRITE0(1, 'P', 42, 0, 0);

  sleep(1);

  PN(atrshmlog_f_list_buffer_slave_count);

  PN(a->writerpid);

  PN(a->writerflag);

  a->writerflag = 0;
  
  a->writerpid = atrshmlog_attach_pid;

  a->writerflag = ATRSHMLOG_WRITER_SLAVE | 8;

  r = ATRSHMLOG_WRITE0(1, 'P', 42, 0, 0);

  sleep(1);

  PN(atrshmlog_f_list_buffer_slave_count);

  printf("\n");

  return 0;
}

/* end of file */
