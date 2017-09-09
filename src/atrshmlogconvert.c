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

/** \file atrshmlogconvert.c
 * \brief We convert binary files in this version format.
 *
 * So we read in one binary transfer file from a reader and convert it.
 * The result is a text file - at least for the transfer of P and I events.
 * If we use a p and a i event its a interpretation as a ucs2 (Java 16 bit) 
 * string part.
 * For all other binary data you have to adjust the converter.
 * See for details the documentation page about this.
 */


#include "atrshmlog.h"

#include "atrshmlog_internal.h"

#include <ctype.h>

#include <stdio.h>

#include <errno.h>

#include <string.h>

#include <stdlib.h>

/*
 *--------------------------------------------------------
 */

/**
 *  helper to generate lines from a chunk 
 */
static int operate(FILE* i_fout,
		   char * const i_buffer,
		   atrshmlog_io_head_t head,
		   int sbyteorder);

static void convertucs2(char* nextcontrol,
			int len,
			const char *prefix,
			FILE *i_fout);



/*
 *--------------------------------------------------------
 */

static char buffer [ ATRSHMLOGBUFFER_INFOSIZE ];
static char payloadbuffer [ ATRSHMLOGBUFFER_INFOSIZE + 1];

/*
 *--------------------------------------------------------
 */

/**
 * convert the input fragment file to a new text file
 * we convert first the header
 * then we take the info and gen per piece a line.
 * this is to handle nl chars in the info to get a still
 * well formed listing
 * non printable chars are set to  space. also tab and cr
 *
 * the format is then 
 * pid tid startime endtime event eventflag userevent info
 * pid : number type, 18 digits, left padded by 0, eg .  000000000000008945
 * tid : hex number type, 16 digits, left padded by 0 , eg. 00007f3cd616c700
 * startime : number type, 20 digits, left padded by 0, eg
 *    01459678038018992258, number of nanosecs since clock zero
 * endtime : number type, 20 digits, left padded by 0, eg 
 *    01459678038019072228, number of nanosecs since clock zero
 * event : number type, 18 digits, left padded by 0, eg 000000000000000000
 *    literally coded in the source to help locating the point of logging
 * eventflag : char, one char, should be one of I|P, I interval, P point,
 *    interval is a write with start and end metured separately,
 *    point is only starttime , delivered in start and end same value, 
 *    normally literally in the source
 * userevent : number type, 18 digits, left padded by 0, eg 
 *    000000000000000008, can vary from call to call, stands for example 
 *    for a line number in a interpreted script, or a loop count, 
 *    stuff like this
 * info : a piece from the final info. if the info contains no nl character
 *    only one line is generated. non printable and tab and cr
 *    is replaced by space.
 *    so you get as many lines from a info as there is nl included...
 *
 * for the number fields its possible to get diffrent if the architecture
 * delivers them so - for eg. in 32 bit env there would be only 10 digit 
 * numbers ...
 *
 * to handle the sorting 
 * cat *.txt | sort -s -n -k 3 
 * is a first approch, but sort is slow...
 *
 * atrshmlogconv uses the sort.
 * 
 * so if you can spare the sorted output its better to
 * use atrshmlogcalc directly without sorting before - it can handle that
 */
int main (int argc, char*argv[])
{
  FILE* fout = NULL;

  FILE* fin = NULL;

  FILE* fstat = NULL;

  FILE* ftimes = NULL;
  
  // int fin;
  
  int buffer_index;

  char *end;

  int payloadlen = 0;

  int ret = 0;
  
  if (argc < 3)
    {
      printf("usage: %s infile outfile [statisticsfile] [timesfile]\n", argv[0]);
      printf("parameter count wrong.\n");
      printf("logsystem version is %d.\n", ATRSHMLOGVERSION);
      exit(1);
    }

  printf("shm log converter from file '%s' to file '%s'.\n", argv[1], argv[2]);


  if (argc > 3)
    {
      fstat = fopen(argv[3], "w");

      if (fstat == NULL)
	{
	  fprintf(stderr, "error in open file %s.\n" , argv[3]);
	  return 1;
	}
    }
  
  if (argc > 4)
    {
      ftimes = fopen(argv[4], "w");

      if (ftimes == NULL)
	{
	  fprintf(stderr, "error in open file %s.\n" , argv[4]);
	  return 1;
	}
    }
  
  fout = fopen(argv[2], "w");

  if (fout == NULL)
    {
      fprintf(stderr, "error in open file %s.\n" , argv[2]);
      return 1;
    }
  
  fin = fopen(argv[1], "rb");

  if (fin == NULL )
    {
      fprintf(stderr, "error in open file %s.\n" , argv[1]);
      return 1;
    }

  {
    atrshmlog_io_head_t head;

    memset(&head, 0, sizeof(head));
    
    int r = fread(&head.order, 1, 2, fin);

    if (r != 2)
      {

	printf("order is not ok.\n");
	exit(1);
      }

    r = fread(&head.version, 1, sizeof(atrshmlog_int32_t), fin);

    if (r != sizeof(atrshmlog_int32_t))
      {
	printf("version is not ok.\n");
	exit(1);
      }

    uint16_t order = 1;

    int sbyteorder = (head.order[0] != *(unsigned char*)&order);
    
    if (sbyteorder)
      {
	head.version = atrshmlog_int32_change_order(head.version);
      }
    
    if (head.version != ATRSHMLOGVERSION)
      {
	printf("version is %d, not as needed %d.\n", head.version, ATRSHMLOGVERSION);
	exit(1);
      }
    
    if (sbyteorder)
      {
	head.version = atrshmlog_int32_change_order(head.version);
      }
    
    r = fread(&head.tlen, 1, sizeof(atrshmlog_int32_t), fin);

    if (r != sizeof(atrshmlog_int32_t))
      {
	printf("tlen is not ok.\n");
	exit(1);
      }
    
    r = fread(&head.pid, 1, sizeof(atrshmlog_pid_t), fin);

    if (r != sizeof(atrshmlog_pid_t))
      {
	printf("pid is not ok.\n");
	exit(1);
      }
    
    r = fread(&head.tid, 1, sizeof(atrshmlog_tid_t), fin);

    if (r != sizeof(atrshmlog_tid_t))
      {
	printf("tid is not ok.\n");
	exit(1);
      }

    r = fread(&head.buffernumber, 1, sizeof(atrshmlog_int32_t), fin);

    if (r != sizeof(atrshmlog_int32_t))
      {
	printf("buffernumber is not ok.\n");
	exit(1);
      }
    
    r = fread(&head.filenumber, 1, sizeof(atrshmlog_int32_t), fin);

    if (r != sizeof(atrshmlog_int32_t))
      {
	printf("filenumber is not ok.\n");
	exit(1);
      }

    r = fread(&head.inittime.tv_sec, 1, sizeof(atrshmlog_time_seconds_t), fin);

    if (r != sizeof(atrshmlog_time_seconds_t))
      {
	printf("inittime is not ok.\n");
	exit(1);
      }

    r = fread(&head.inittime.tv_nsec, 1, sizeof(atrshmlog_time_nanoseconds_t), fin);

    if (r != sizeof(atrshmlog_time_nanoseconds_t))
      {
	printf("inittime is not ok.\n");
	exit(1);
      }

    r = fread(&head.inittsc_before, 1, sizeof(atrshmlog_time_t), fin);

    if (r != sizeof(atrshmlog_time_t))
      {
	printf("inittsc_before is not ok.\n");
	exit(1);
      }

    r = fread(&head.inittsc_after, 1, sizeof(atrshmlog_time_t), fin);

    if (r != sizeof(atrshmlog_time_t))
      {
	printf("inittsc_after is not ok.\n");
	exit(1);
      }


    r = fread(&head.lasttime.tv_sec, 1, sizeof(atrshmlog_time_seconds_t), fin);

    if (r != sizeof(atrshmlog_time_seconds_t))
      {
	printf("lasttime is not ok.\n");
	exit(1);
      }

    r = fread(&head.lasttime.tv_nsec, 1, sizeof(atrshmlog_time_nanoseconds_t), fin);

    if (r != sizeof(atrshmlog_time_nanoseconds_t))
      {
	printf("lasttime is not ok.\n");
	exit(1);
      }

    r = fread(&head.lasttsc_before, 1, sizeof(atrshmlog_time_t), fin);

    if (r != sizeof(atrshmlog_time_t))
      {
	printf("lasttsc_before is not ok.\n");
	exit(1);
      }

    r = fread(&head.lasttsc_after, 1, sizeof(atrshmlog_time_t), fin);

    if (r != sizeof(atrshmlog_time_t))
      {
	printf("lasttsc_after is not ok.\n");
	exit(1);
      }

    r = fread(&head.difftimetransfer, 1, sizeof(atrshmlog_time_t), fin);

    if (r != sizeof(atrshmlog_time_t))
      {
	printf("difftimetransfer is not ok.\n");
	exit(1);
      }

    r = fread(&head.starttransfer, 1, sizeof(atrshmlog_time_t), fin);

    if (r != sizeof(atrshmlog_time_t))
      {
	printf("starttransfer is not ok.\n");
	exit(1);
      }

    r = fread(&head.acquiretime, 1, sizeof(atrshmlog_time_t), fin);

    if (r != sizeof(atrshmlog_time_t))
      {
	printf("acquiretime is not ok.\n");
	exit(1);
      }

    r = fread(&head.id, 1, sizeof(atrshmlog_int32_t), fin);

    if (r != sizeof(atrshmlog_int32_t))
      {
	printf("id is not ok.\n");
	exit(1);
      }

    r = fread(&head.number_dispatched, 1, sizeof(atrshmlog_int32_t), fin);

    if (r != sizeof(atrshmlog_int32_t))
      {
	printf("number_dispatched is not ok.\n");
	exit(1);
      }

    r = fread(&head.counter_write0, 1, sizeof(atrshmlog_int32_t), fin);

    if (r != sizeof(atrshmlog_int32_t))
      {
	printf("counter_write0 is not ok.\n");
	exit(1);
      }

    r = fread(&head.counter_write0_discard, 1, sizeof(atrshmlog_int32_t), fin);

    if (r != sizeof(atrshmlog_int32_t))
      {
	printf("counter_write0_discard is not ok.\n");
	exit(1);
      }

    r = fread(&head.counter_write0_wait, 1, sizeof(atrshmlog_int32_t), fin);

    if (r != sizeof(atrshmlog_int32_t))
      {
	printf("counter_write0_wait is not ok.\n");
	exit(1);
      }

    r = fread(&head.counter_write0_adaptive, 1, sizeof(atrshmlog_int32_t), fin);

    if (r != sizeof(atrshmlog_int32_t))
      {
	printf("counter_write0_adaptive is not ok.\n");
	exit(1);
      }

    r = fread(&head.counter_write0_adaptive_fast, 1, sizeof(atrshmlog_int32_t), fin);

    if (r != sizeof(atrshmlog_int32_t))
      {
	printf("counter_write0_adaptive_fast is not ok.\n");
	exit(1);
      }

    r = fread(&head.counter_write0_adaptive_very_fast, 1, sizeof(atrshmlog_int32_t), fin);

    if (r != sizeof(atrshmlog_int32_t))
      {
	printf("counter_write0_adaptive_very_fast is not ok.\n");
	exit(1);
      }

    r = fread(&head.counter_write1, 1, sizeof(atrshmlog_int32_t), fin);

    if (r != sizeof(atrshmlog_int32_t))
      {
	printf("counter_write1 is not ok.\n");
	exit(1);
      }

    r = fread(&head.counter_write1_discard, 1, sizeof(atrshmlog_int32_t), fin);

    if (r != sizeof(atrshmlog_int32_t))
      {
	printf("counter_write1_discard is not ok.\n");
	exit(1);
      }

    r = fread(&head.counter_write1_wait, 1, sizeof(atrshmlog_int32_t), fin);

    if (r != sizeof(atrshmlog_int32_t))
      {
	printf("counter_write1_wait is not ok.\n");
	exit(1);
      }

    r = fread(&head.counter_write1_adaptive, 1, sizeof(atrshmlog_int32_t), fin);

    if (r != sizeof(atrshmlog_int32_t))
      {
	printf("counter_write1_adaptive is not ok.\n");
	exit(1);
      }

    r = fread(&head.counter_write1_adaptive_fast, 1, sizeof(atrshmlog_int32_t), fin);

    if (r != sizeof(atrshmlog_int32_t))
      {
	printf("counter_write1_adaptive_fast is not ok.\n");
	exit(1);
      }

    r = fread(&head.counter_write1_adaptive_very_fast, 1, sizeof(atrshmlog_int32_t), fin);

    if (r != sizeof(atrshmlog_int32_t))
      {
	printf("counter_write1_adaptive_very_fast is not ok.\n");
	exit(1);
      }

    r = fread(&head.counter_write2, 1, sizeof(atrshmlog_int32_t), fin);

    if (r != sizeof(atrshmlog_int32_t))
      {
	printf("counter_write2 is not ok.\n");
	exit(1);
      }

    r = fread(&head.counter_write2_discard, 1, sizeof(atrshmlog_int32_t), fin);

    if (r != sizeof(atrshmlog_int32_t))
      {
	printf("counter_write2_discard is not ok.\n");
	exit(1);
      }

    r = fread(&head.counter_write2_wait, 1, sizeof(atrshmlog_int32_t), fin);

    if (r != sizeof(atrshmlog_int32_t))
      {
	printf("counter_write2_wait is not ok.\n");
	exit(1);
      }

    r = fread(&head.counter_write2_adaptive, 1, sizeof(atrshmlog_int32_t), fin);

    if (r != sizeof(atrshmlog_int32_t))
      {
	printf("counter_write2_adaptive is not ok.\n");
	exit(1);
      }

    r = fread(&head.counter_write2_adaptive_fast, 1, sizeof(atrshmlog_int32_t), fin);

    if (r != sizeof(atrshmlog_int32_t))
      {
	printf("counter_write2_adaptive_fast is not ok.\n");
	exit(1);
      }

    r = fread(&head.counter_write2_adaptive_very_fast, 1, sizeof(atrshmlog_int32_t), fin);

    if (r != sizeof(atrshmlog_int32_t))
      {
	printf("counter_write2_adaptive_very_fast is not ok.\n");
	exit(1);
      }

    // from this on we work in order for the head
    if (sbyteorder)
      {
	atrshmlog_io_head_change_order(&head);
      }

    // this is a well known size. see the reader for it
    // do not mess this up.
    // we need to read the head, and the filler
    // is used to make the next memory adress a good one
    // for memcpy
    // we use a 512 byte head. then the buffer comes.

    char filler[318];
    
    r = fread(&filler, 1, sizeof(filler), fin);

    if (r != sizeof(filler))
      {
	printf("filler is not ok.\n");
	exit(1);
      }
    
    
    if (head.tlen <  0 || head.tlen > ATRSHMLOGBUFFER_INFOSIZE)
      {
	printf("tlen size wrong, is %d, not max %d\n", head.tlen, ATRSHMLOGBUFFER_INFOSIZE);
	exit(1);
      }
    
    char* pos = buffer;

    int total_len = 0;

    size_t read_len;
    size_t result_len;
    
    read_len = head.tlen;

    result_len = fread(pos, 1, read_len, fin);
    
    if (result_len != read_len)
      {
	fprintf(stderr, "error in read %d.\n", errno);
	ret = 3;
	goto ende;
      }

    pos += result_len;
    total_len += result_len;
    
    fclose(fin);
    
    printf("id %4ld acquiretime %8ld pid %6ld tid %8ld slavetime %9ld readertime %9ld payloadsize %7ld shmbuffer %3d filenumber %7d sequence %d\n",
	 (long)head.id,
	 (long)head.acquiretime,
	 (long)head.pid,
	 (long)head.tid,
	 (long)(head.lasttsc_before - head.starttransfer),
	 (long)head.difftimetransfer,
	 (long)head.tlen,
	 head.buffernumber,
	 head.filenumber,
	 (int)head.number_dispatched
	 );

    if (operate(fout, buffer, head, sbyteorder) != 0)
      {
	fprintf(stderr, "error in operate, stop.\n");
	ret = 2;
	goto ende;
      }

    // if we have statistics and we have a stat file
    if(fstat != NULL)
      {
	fprintf(fstat, "atrshmlogstat 12 %ld\n", (long)head.counter_write0);
	fprintf(fstat, "atrshmlogstat 17 %ld\n", (long)head.counter_write0_discard);
	fprintf(fstat, "atrshmlogstat 18 %ld\n", (long)head.counter_write0_wait);
	fprintf(fstat, "atrshmlogstat 19 %ld\n", (long)head.counter_write0_adaptive);
	fprintf(fstat, "atrshmlogstat 20 %ld\n", (long)head.counter_write0_adaptive_fast);
	fprintf(fstat, "atrshmlogstat 21 %ld\n", (long)head.counter_write0_adaptive_very_fast);
	fprintf(fstat, "atrshmlogstat 24 %ld\n", (long)head.counter_write1);
	fprintf(fstat, "atrshmlogstat 29 %ld\n", (long)head.counter_write1_discard);
	fprintf(fstat, "atrshmlogstat 30 %ld\n", (long)head.counter_write1_wait);
	fprintf(fstat, "atrshmlogstat 31 %ld\n", (long)head.counter_write1_adaptive);
	fprintf(fstat, "atrshmlogstat 32 %ld\n", (long)head.counter_write1_adaptive_fast);
	fprintf(fstat, "atrshmlogstat 33 %ld\n", (long)head.counter_write1_adaptive_very_fast);
	fprintf(fstat, "atrshmlogstat 37 %ld\n", (long)head.counter_write2);
	fprintf(fstat, "atrshmlogstat 42 %ld\n", (long)head.counter_write2_discard);
	fprintf(fstat, "atrshmlogstat 43 %ld\n", (long)head.counter_write2_wait);
	fprintf(fstat, "atrshmlogstat 44 %ld\n", (long)head.counter_write2_adaptive);
	fprintf(fstat, "atrshmlogstat 45 %ld\n", (long)head.counter_write2_adaptive_fast);
	fprintf(fstat, "atrshmlogstat 46 %ld\n", (long)head.counter_write2_adaptive_very_fast);
      }

    if(ftimes != NULL)
      {
	fprintf(ftimes,
		"inittime_seconds : %lld\n"
		"inittime_nanos   : %lld\n"
		"inittime_before  : %lld\n"
		"inittime_after   : %lld\n"
		"lasttime_seconds : %lld\n"
		"lasttime_nanos   : %lld\n"
		"lasttime_before  : %lld\n"
		"lasttime_after   : %lld\n",
		(long long)head.inittime.tv_sec,
		(long long)head.inittime.tv_nsec,
		(long long)head.inittsc_before,
		(long long)head.inittsc_after,
		(long long)head.lasttime.tv_sec,
		(long long)head.lasttime.tv_nsec,
		(long long)head.lasttsc_before,
		(long long)head.lasttsc_after);
		
      }
  }
  
 ende:

  if (fout != NULL)
    {
      fflush(fout);
      fclose(fout);
    }

  if (fstat != NULL)
    {
      fflush(fstat);
      fclose(fstat);
    }
  
  if (ftimes != NULL)
    {
      fflush(ftimes);
      fclose(ftimes);
    }
  
  return ret;
}

/****************************************************/

#if ATRSHMLOG_USE_SAFER_COPY == 1 
#define MYCAT(_d,_s) 	strlcat((_d), (_s), 256)
#else
#define MYCAT(_d,_s) 	strcat((_d), (_s))
#endif

/** 
 * we convert the content of buffer into output
 * we start with the controldata
 * then we use the length and calc the rest
 * can be empty 
 * we write this one info chunk.
 * we add a line number and write the lines out (?)
 * then we advandce 
 * we check if the thing is still complete.
 * if not we memove the rest t the beginning of the buffer and 
 * adjust end and newend
 * if anything goes wrong we return with -1
 */
int operate(FILE* i_fout,
	    char * const i_buffer,
	    atrshmlog_io_head_t head,
	    int sbyteorder)

{
  int result = -1;

  /* we calc the starttime and endtime in real */

  long long tscstart = head.inittsc_after;

  tscstart += head.inittsc_before;

  tscstart /= 2;

  long long tscend = head.lasttsc_before;

  tscend += head.lasttsc_after;

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

  realstart = head.inittime.tv_sec;
  
  realstart *=  1000000000.0;

  realstart += head.inittime.tv_nsec;

  long double realend;

  realend = head.lasttime.tv_sec;
  
  realend *=  1000000000.0;

  realend += head.lasttime.tv_nsec;

  long double realdiff = realend - realstart;
  
  /* and now we can start with a realtime adder */
  

  int chunkindex = 0;

  atrshmlog_chunk_head_t c;
  
  for (int akindex = 0; akindex < head.tlen; akindex += c.totallen)
    {
      chunkindex++;

      atrshmlog_fill_chunk_head (i_buffer + akindex, &c);

      if (sbyteorder)
	{
	  atrshmlog_chunk_head_change_order(&c);
	}
      
      if (c.totallen < ATRSHMLOGCONTROLDATASIZE || c.totallen > ATRSHMLOGBUFFER_INFOSIZE)
	{
	  fprintf(stderr, "error c.totallen : size :  in %d : %d.\n", chunkindex, c.totallen);
	  return -1;
	}

      size_t lcount = c.totallen - ATRSHMLOGCONTROLDATASIZE;

      if (lcount > 0)
	{
	  memcpy(payloadbuffer, i_buffer + akindex + ATRSHMLOGCONTROLDATASIZE, lcount);
	}
    
      payloadbuffer[lcount] = '\0';

      
      int ucs2string = 0;
      
      if (c.eventflag == ATRSHMLOGPOINTINTIMEp || c.eventflag == ATRSHMLOGPOINTINTIMEi)
	ucs2string = 1;
	
      /* we now calc the real time
       * (reals - realstart)/  (realend - realstart)  
       * eq (s - tstart) / (tend - tstart)  
       * so 
       * (reals - realstart) = (s - tstart) * (realend - realstart) / (tend - tstart)
       * so
       * reals = realstart + ((s - tstart) * (realend - realstart) / (tend - tstart))
       */
      long double reals_d =  realstart;

      long double quots_d = c.starttime;

      quots_d -= tscstart_d;
	
      quots_d *= realdiff;
        
      reals_d += quots_d / tscdiff_d;

      long long reals = reals_d;
	
      long double reale_d =  realstart;

      long double quote_d = c.endtime;

      quote_d -= tscstart_d;
	
      quote_d *= realdiff;
        
      reale_d += quote_d / tscdiff_d;

      long long reale = reale_d;
	
      long long reald = reale - reals;


      // NOW WE ONLY PRINT AND WRITE OUT.
      char controlbuffer [ 256 ];
      char tidstr[40];
      char bnr[40];
      char fnr[40];
      char starttimestr[40];
      char endtimestr[40];
      char deltastr[40];
      char realstarttimestr[40];
      char realendtimestr[40];
      char realdeltastr[40];
      char eventstr[40];
      char eventflagstr[40];
      char userstr[40];
      

      snprintf(tidstr, 40, "%016ld", (long)head.tid);
	
      snprintf(bnr, 40, "%03d", (int)head.buffernumber);

      snprintf(fnr, 40, "%018d", (int)head.filenumber);
	
      snprintf(starttimestr, 40, "%018lld", (long long)c.starttime);
	
      snprintf(endtimestr, 40, "%018lld", (long long)c.endtime);

      snprintf(deltastr, 40, "%018lld", (long long)(c.endtime - c.starttime));


      snprintf(realstarttimestr, 40, "%018lld", reals);
	
      snprintf(realendtimestr, 40, "%018lld", reale);

      snprintf(realdeltastr, 40, "%018lld", reald);

	
      snprintf(controlbuffer, 256,
	       "%010ld ",
	       (long)head.pid);

      snprintf(eventstr, 40, "%010ld ", (long)c.eventnumber);

      snprintf(userstr, 40, "%010ld", (long)c.userflag);
    
      eventflagstr[0] = c.eventflag;
      eventflagstr[1] = ' ' ; 
      eventflagstr[2] = '\0';
	
      MYCAT(controlbuffer, tidstr);

      MYCAT(controlbuffer, " ");

      MYCAT(controlbuffer, bnr);

      MYCAT(controlbuffer, " ");

      MYCAT(controlbuffer, fnr);

      MYCAT(controlbuffer, " ");

      MYCAT(controlbuffer, starttimestr);

      MYCAT(controlbuffer , " ");

      MYCAT(controlbuffer, endtimestr);

      MYCAT(controlbuffer , " ");

      MYCAT(controlbuffer, deltastr);

      MYCAT(controlbuffer , " ");

      MYCAT(controlbuffer, realstarttimestr);

      MYCAT(controlbuffer , " ");

      MYCAT(controlbuffer, realendtimestr);

      MYCAT(controlbuffer , " ");

      MYCAT(controlbuffer, realdeltastr);

      MYCAT(controlbuffer , " ");

      MYCAT(controlbuffer, eventstr);

      MYCAT(controlbuffer, eventflagstr);

    
      MYCAT(controlbuffer, userstr);

	
      if (ucs2string)
	{
	  convertucs2(payloadbuffer, (c.totallen - ATRSHMLOGCONTROLDATASIZE), controlbuffer, i_fout);
	}
      else
	{
	  char* lastpos = payloadbuffer;
	  char* nextcontrol = payloadbuffer;

	  /* simple text convert, somehow dirty done */ 
	  while ( nextcontrol != NULL )
	    {
	      nextcontrol = strchr(lastpos, '\n');

	      if (nextcontrol)
		*nextcontrol = '\0';

		
	      for (char *k = lastpos; *k; k++)
		{
		  if (!isprint(*k))
		    *k = ' ';
		  
		  if (*k == '\t' || *k == '\r')
		    *k = ' ';
		    
		}
        
	      fprintf(i_fout, "%s %s\n",
		      controlbuffer,
		      lastpos
		      );
		
	      lastpos = nextcontrol + 1;
	    }
	}
      
    }
  
  /* ok. we are out normal 
  * we adjust end and newend and memove the bufer rest
  */
 leave1:

  return 0;
}



/**
 * poor man solution
 */
static void convertucs2(char *inputstringbuffer,
			int lentimes2,
			const char *prefix,
			FILE *i_fout)
{
  unsigned char *p = (unsigned char*)inputstringbuffer;

  char *target = inputstringbuffer; // dirty. we use the buffer itself for write

  int converted = 0;

  char *laststart = target;
  
  for (int i = 0 ; i <  (lentimes2 / 2); i++, p += 2)
    {
      int ch = (unsigned)((p[1] & 255) << 8) & ( p[0] & 255);

      if (ch == '\n')
	{
	  fprintf(i_fout, "%s ", prefix);

	  if(converted > 0)
	    fwrite(laststart,
		   1,
		   converted,
		   i_fout);

	  fprintf(i_fout, "\n");
	  converted = 0;
	  target = inputstringbuffer + (2 * i);
	  laststart = target;
	  continue;
	}

      switch (ch)
	{
	case '\r':
	  break;

	case '\t':
	  *target = ' ' ;
	  target++;
	  converted++;
	  break;

	default:
	  if (isprint(*(char*)p))
	    {
	      *target = *(char*)p;
	    }
	  else
	    {
	      *target = ' ' ;
	    }
	  
	  target++;
	  converted++;
	  break;
	}
    }

  fprintf(i_fout, "%s " , prefix);

  if(converted > 0)
    fwrite (laststart,
	    1,
	    converted,
	    i_fout);

  fprintf(i_fout, "\n");
}

/* end of file */
