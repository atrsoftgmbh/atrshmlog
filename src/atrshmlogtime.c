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

/** \file atrshmlogtime.c
 * \brief We convert times
 *
 * We get a inittime, a lasttime and a clicktime and calc the realtime approx.
 */


#include "atrshmlog.h"

#include "atrshmlog_internal.h"

#include <stdio.h>

#include <stdlib.h>


/*
 *--------------------------------------------------------
 */
void calc(char* i_is,
	  char *i_in,
	  char* i_ib,
	  char* i_ia,
	  char *i_ls,
	  char *i_ln,
	  char *i_lb,
	  char *i_la,
	  char *i_v)
{
  long long is = strtoll(i_is, NULL, 10);
  long long in = strtoll(i_in, NULL, 10);
  long long ib = strtoll(i_ib, NULL, 10);
  long long ia = strtoll(i_ia, NULL, 10);
  long long ls = strtoll(i_ls, NULL, 10);
  long long ln = strtoll(i_ln, NULL, 10);
  long long lb = strtoll(i_lb, NULL, 10);
  long long la = strtoll(i_la, NULL, 10);

  long long v = strtoll(i_v, NULL, 10);

    /* we calc the starttime and endtime in real */

  long long tscstart = ia;

  tscstart += ib;

  tscstart /= 2;

  long long tscend = lb;

  tscend += la;

  tscend /= 2;

  long long tscdiff = tscend - tscstart;

  long double tscstart_d = tscstart;

  long double tscend_d =  tscend;

  long double tscdiff_d = tscdiff;
  
  /* we can now start to give the realtime a try
   * we assume the tsc is in the middle of the fetch of the real time
   * and so we give all tsc now a new realtime 
   */

  /* ok. we have now the tsc. we now need the real */

  long double realstart;

  realstart = is;
  
  realstart *=  1000000000.0;

  realstart += in;

  long double realend;

  realend = ls;
  
  realend *=  1000000000.0;

  realend += ln;

  long double realdiff = realend - realstart;
  
  /* we now calc the real time
   * (reals - realstart)/  (realend - realstart)  
   * eq (s - tstart) / (tend - tstart)  
   * so 
   * (reals - realstart) = (s - tstart) * (realend - realstart) / (tend - tstart)
   * so
   * reals = realstart + ((s - tstart) * (realend - realstart) / (tend - tstart))
   */
  long double reals_d =  realstart;

  long double quots_d = v;

  quots_d -= tscstart_d;
	
  quots_d *= realdiff;
        
  reals_d += quots_d / tscdiff_d;

  long long reals = reals_d;

  printf("%lld : %lld\n", v, reals);
}

/*
 *--------------------------------------------------------
 */

/**
 */
int main (int argc, char*argv[])
{
  if (argc < 10)
    {
      printf("usage: %s inittime_seconds \n    inittime_nanos \n    clicktime_before \n    clicktime_after \n     lasttime_seconds \n    lasttime_nanos \n    clicktime_before \n    clicktime_after \n     clicktime ...\n", argv[0]);
      printf("parameter count wrong.\n");
      printf("logsystem version is %d.\n", ATRSHMLOGVERSION);
      exit(1);
    }

  printf("shm log time:\n"
	 "    inittime seconds %s \n"
	 "    inittime nanos   %s \n"
	 "    inittime before  %s \n"
	 "    inittime after   %s \n"
	 "    lasttime seconds %s \n"
	 "    lasttime nanos   %s \n"
	 "    lasttime before  %s \n"
	 "    lasttime after   %s \n"
	 , argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], argv[8]);


  for (int i = 9; i < argc; i++)
    calc(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], argv[i]);
  
  return 0;
}


/* end of file */
