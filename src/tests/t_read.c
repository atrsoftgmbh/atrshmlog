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

/** \file t_read.c
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
  
  // we start without slaves ...
  atrshmlog_set_f_list_buffer_slave_count(0);
  
  atrshmlog_ret_t ret = atrshmlog_attach();

  if (ret != 0)
    {
      printf("%d\n", ret);
      exit(1);
    }

  if(argc != 2)
    exit(1);

  int count = strtol(argv[1], NULL, 10);

  const volatile atrshmlog_area_t *a =  atrshmlog_get_area();

  if (a == 0)
    exit(2);
  
  {
    atrshmlog_ret_t ret;
    atrshmlog_int32_t length;			  
    atrshmlog_pid_t pid;	    
    atrshmlog_tid_t tid;
    atrshmlog_internal_time_t inittime;
    atrshmlog_time_t inittimetsc_before;
    atrshmlog_time_t inittimetsc_after;
    atrshmlog_internal_time_t lasttime;
    atrshmlog_time_t lasttimetsc_before;
    atrshmlog_time_t lasttimetsc_after;
    atrshmlog_time_t difftimetransfer;
    atrshmlog_time_t starttransfer;
    atrshmlog_time_t acquiretime;		  
    atrshmlog_int32_t id;
    atrshmlog_int32_t number_dispatched;
    atrshmlog_int32_t counter_write0;
    atrshmlog_int32_t counter_write0_discard;
    atrshmlog_int32_t counter_write0_wait;
    atrshmlog_int32_t counter_write0_adaptive;
    atrshmlog_int32_t counter_write0_adaptive_fast;
    atrshmlog_int32_t counter_write0_adaptive_very_fast;
    atrshmlog_int32_t counter_write1;
    atrshmlog_int32_t counter_write1_discard;
    atrshmlog_int32_t counter_write1_wait;
    atrshmlog_int32_t counter_write1_adaptive;
    atrshmlog_int32_t counter_write1_adaptive_fast;
    atrshmlog_int32_t counter_write1_adaptive_very_fast;
    atrshmlog_int32_t counter_write2;			
    atrshmlog_int32_t counter_write2_discard;
    atrshmlog_int32_t counter_write2_wait;
    atrshmlog_int32_t counter_write2_adaptive;
    atrshmlog_int32_t counter_write2_adaptive_fast;
    atrshmlog_int32_t counter_write2_adaptive_very_fast;

    char*  read_buffer = malloc(1000000);
  
    ret = ATRSHMLOG_READ(a,
			 count,
			 read_buffer,
			 &length,
			 &pid,
			 &tid,
			 &inittime,
			 &inittimetsc_before,
			 &inittimetsc_after,
			 &lasttime,
			 &lasttimetsc_before,
			 &lasttimetsc_after,
			 &difftimetransfer,
			 &starttransfer,
			 &acquiretime,
			 &id,
			 &number_dispatched,
			 &counter_write0,
			 &counter_write0_discard,
			 &counter_write0_wait,
			 &counter_write0_adaptive,
			 &counter_write0_adaptive_fast,
			 &counter_write0_adaptive_very_fast,
			 &counter_write1,
			 &counter_write1_discard,
			 &counter_write1_wait,
			 &counter_write1_adaptive,
			 &counter_write1_adaptive_fast,
			 &counter_write1_adaptive_very_fast,
			 &counter_write2,
			 &counter_write2_discard,
			 &counter_write2_wait,
			 &counter_write2_adaptive,
			 &counter_write2_adaptive_fast,
			 &counter_write2_adaptive_very_fast);
  
    PN(ret);
    PN(length);			  
    PN(pid);	    
    PN(tid);
    PN(inittime.tv_sec);
    PN(inittime.tv_nsec);
    PN(inittimetsc_before);
    PN(inittimetsc_after);
    PN(lasttime.tv_sec);
    PN(lasttime.tv_nsec);
    
    PN(lasttimetsc_before);
    PN(lasttimetsc_after);
    PN(difftimetransfer);
    PN(starttransfer);
    PN(acquiretime);
    PN(id);
    PN(number_dispatched);
    PN(counter_write0);
    PN(counter_write0_discard);
    PN(counter_write0_wait);
    PN(counter_write0_adaptive);
    PN(counter_write0_adaptive_fast);
    PN(counter_write0_adaptive_very_fast);
    PN(counter_write1);
    PN(counter_write1_discard);
    PN(counter_write1_wait);
    PN(counter_write1_adaptive);
    PN(counter_write1_adaptive_fast);
    PN(counter_write1_adaptive_very_fast);
    PN(counter_write2);
    PN(counter_write2_discard);
    PN(counter_write2_wait);
    PN(counter_write2_adaptive);
    PN(counter_write2_adaptive_fast);
    PN(counter_write2_adaptive_very_fast);
  }
  
  printf("\n");

  return 0;
}

/* end of file */
