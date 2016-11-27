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

/** \file atrshmlogtest00.c
 * \brief We are a minimum test program.
 */

#include "../atrshmlog.h"

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
  const char hello[] = "hello, world.";
  
  printf("shm log attach %d and log : %s.\n", ATRSHMLOG_ATTACH(), hello);

  return ATRSHMLOG_WRITE(1, 'P', argc, 0, 0, hello, sizeof(hello) - 1);
}

/* end of file */
