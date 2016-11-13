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

#include <stdio.h>


/*
 *--------------------------------------------------------
 */

/**
 *  helper to generate lines from a chunk 
 */
static int operate(FILE* i_fout,
		   char * const i_buffer,
		   int i_buffer_end,
		   int read_len,
		   atrshmlog_pid_t pid,
		   atrshmlog_tid_t tid,
		   atrshmlog_int32_t buffernumber,
		   atrshmlog_int32_t filenumber,
		   atrshmlog_internal_time_t inittime,
		   atrshmlog_time_t inittsc_before,
		   atrshmlog_time_t inittsc_after,
		   atrshmlog_internal_time_t lasttime,
		   atrshmlog_time_t lasttsc_before,
		   atrshmlog_time_t lasttsc_after,
		   atrshmlog_time_t difftimetransfer,
		   atrshmlog_time_t starttransfer,
		   atrshmlog_time_t acquiretime,
		   atrshmlog_int32_t id);

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
  
  // int fin;
  
  int buffer_index;

  char *end;

  int total_len = 0;

  int payloadlen = 0;

  int ret = 0;
  
  if (argc != 3)
    {
      printf("usage: %s infile outfile\n", argv[0]);
      printf("parameter count wrong.\n");
      printf("logsystem version is %d.\n", ATRSHMLOGVERSION);
      exit(1);
    }

  printf("shm log converter from file '%s' to file '%s'.\n", argv[1], argv[2]);

  
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

  total_len = 0;
  
  {
    size_t read_len;
    size_t result_len;
    
    
    atrshmlog_int32_t version;
    
    atrshmlog_int32_t buffernumber;

    atrshmlog_int32_t tlen;

    atrshmlog_pid_t pid;
    atrshmlog_tid_t tid; 

    atrshmlog_internal_time_t inittime;
    
    atrshmlog_time_t inittsc_before;
    
    atrshmlog_time_t inittsc_after;
	      
    atrshmlog_internal_time_t lasttime;
    
    atrshmlog_time_t lasttsc_before;
	      
    atrshmlog_time_t lasttsc_after;
    
    atrshmlog_time_t difftimetransfer;
  
    atrshmlog_time_t starttransfer;
  
    atrshmlog_time_t acquiretime;

    atrshmlog_int32_t id;

    atrshmlog_int32_t filenumber;

    char order[2];
  
    char* pos = buffer;

    int r = fread(&order, 1, 2, fin);

    if (r != 2)
      {

	printf("order is not ok.\n");
	exit(1);
      }

    r = fread(&version, 1, sizeof(atrshmlog_int32_t), fin);

    if (r != sizeof(atrshmlog_int32_t) || version != ATRSHMLOGVERSION)
      {
	printf("version is %d, not as needed %d.\n", version, ATRSHMLOGVERSION);
	exit(1);
      }
    
    r = fread(&tlen, 1, sizeof(atrshmlog_int32_t), fin);

    if (r != sizeof(atrshmlog_int32_t))
      {
	printf("tlen is not ok.\n");
	exit(1);
      }
    
    r = fread(&pid, 1, sizeof(atrshmlog_pid_t), fin);

    if (r != sizeof(atrshmlog_pid_t))
      {
	printf("pid is not ok.\n");
	exit(1);
      }
    
    r = fread(&tid, 1, sizeof(atrshmlog_tid_t), fin);

    if (r != sizeof(atrshmlog_tid_t))
      {
	printf("tid is not ok.\n");
	exit(1);
      }

    r = fread(&buffernumber, 1, sizeof(atrshmlog_int32_t), fin);

    if (r != sizeof(atrshmlog_int32_t))
      {
	printf("buffernumber is not ok.\n");
	exit(1);
      }
    r = fread(&filenumber, 1, sizeof(atrshmlog_int32_t), fin);

    if (r != sizeof(atrshmlog_int32_t))
      {
	printf("filenumber is not ok.\n");
	exit(1);
      }

    r = fread(&inittime.tv_sec, 1, sizeof(atrshmlog_time_seconds_t), fin);

    if (r != sizeof(atrshmlog_time_seconds_t))
      {
	printf("inittime is not ok.\n");
	exit(1);
      }

    r = fread(&inittime.tv_nsec, 1, sizeof(atrshmlog_time_nanoseconds_t), fin);

    if (r != sizeof(atrshmlog_time_nanoseconds_t))
      {
	printf("inittime is not ok.\n");
	exit(1);
      }
#if 	  ATRSHMLOGDEBUG ==  1
  printf("%ld %ld\n", (long)inittime.tv_sec, (long)inittime.tv_nsec);
#endif

  r = fread(&inittsc_before, 1, sizeof(atrshmlog_time_t), fin);

    if (r != sizeof(atrshmlog_time_t))
      {
	printf("inittsc_before is not ok.\n");
	exit(1);
      }

    r = fread(&inittsc_after, 1, sizeof(atrshmlog_time_t), fin);

    if (r != sizeof(atrshmlog_time_t))
      {
	printf("inittsc_after is not ok.\n");
	exit(1);
      }


    r = fread(&lasttime.tv_sec, 1, sizeof(atrshmlog_time_seconds_t), fin);

    if (r != sizeof(atrshmlog_time_seconds_t))
      {
	printf("lasttime is not ok.\n");
	exit(1);
      }

    r = fread(&lasttime.tv_nsec, 1, sizeof(atrshmlog_time_nanoseconds_t), fin);

    if (r != sizeof(atrshmlog_time_nanoseconds_t))
      {
	printf("lasttime is not ok.\n");
	exit(1);
      }

    r = fread(&lasttsc_before, 1, sizeof(atrshmlog_time_t), fin);

    if (r != sizeof(atrshmlog_time_t))
      {
	printf("lasttsc_before is not ok.\n");
	exit(1);
      }

    r = fread(&lasttsc_after, 1, sizeof(atrshmlog_time_t), fin);

    if (r != sizeof(atrshmlog_time_t))
      {
	printf("lasttsc_after is not ok.\n");
	exit(1);
      }

    r = fread(&difftimetransfer, 1, sizeof(atrshmlog_time_t), fin);

    if (r != sizeof(atrshmlog_time_t))
      {
	printf("difftimetransfer is not ok.\n");
	exit(1);
      }

    r = fread(&starttransfer, 1, sizeof(atrshmlog_time_t), fin);

    if (r != sizeof(atrshmlog_time_t))
      {
	printf("starttransfer is not ok.\n");
	exit(1);
      }

    r = fread(&acquiretime, 1, sizeof(atrshmlog_time_t), fin);

    if (r != sizeof(atrshmlog_time_t))
      {
	printf("acquiretime is not ok.\n");
	exit(1);
      }

    r = fread(&id, 1, sizeof(atrshmlog_int32_t), fin);

    if (r != sizeof(atrshmlog_int32_t))
      {
	printf("id is not ok.\n");
	exit(1);
      }

    // this is a well known size. see the reader for it
    // do not mess this up.
    // we need to read the head, and the filler
    // is used to make the next memory adress a good one
    // for memcpy

    char filler[394];
    
    r = fread(&filler, 1, sizeof(filler), fin);

    if (r != sizeof(filler))
      {
	printf("filler is not ok.\n");
	exit(1);
      }
    
    
    if (tlen <  0 || tlen > ATRSHMLOGBUFFER_INFOSIZE)
      {
	printf("tlen size wrong, is %d, not max %d\n", tlen, ATRSHMLOGBUFFER_INFOSIZE);
	exit(1);
      }
    
    read_len = tlen;
    do
      {
      again:
	result_len = fread(pos, 1, read_len - total_len + 1, fin);
    
	if (result_len < 0)
	  {
	    if (errno != EINTR)
	      {
		fprintf(stderr, "error in read %d.\n", errno);
		ret = 3;
		goto ende;
	      }
	    goto again;
	  }
	else
	  {
	    if (result_len > 0)
	      {
		pos += result_len;
		total_len += result_len;
	      }
	  }
      } while ( result_len > 0);
    
    fclose(fin);
    
    if (operate(fout, buffer, total_len, tlen,
		pid, tid, buffernumber, filenumber,
		inittime, inittsc_before, inittsc_after, lasttime,
		lasttsc_before, lasttsc_after, difftimetransfer, starttransfer,
		acquiretime, id) != 0)
      {
	fprintf(stderr, "error in operate, stop.\n");
	ret = 2;
	goto ende;
      }
  }
  
 ende:

  if (fout != NULL)
    {
      fflush(fout);
      fclose(fout);
    }

  
  return ret;
}

/****************************************************/

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
	    int i_buffer_end,
	    int i_read_len,
	    atrshmlog_pid_t i_pid,
	    atrshmlog_tid_t i_tid,
	    atrshmlog_int32_t i_buffernumber,
	    atrshmlog_int32_t i_filenumber,
	    atrshmlog_internal_time_t i_inittime,
	    atrshmlog_time_t i_inittsc_before,
	    atrshmlog_time_t i_inittsc_after,
	    atrshmlog_internal_time_t i_lasttime,
	    atrshmlog_time_t i_lasttsc_before,
	    atrshmlog_time_t i_lasttsc_after,
	    atrshmlog_time_t i_difftimetransfer,
	    atrshmlog_time_t i_starttransfer,
	    atrshmlog_time_t i_acquiretime,
	    atrshmlog_int32_t i_id)
{
  int result = -1;

  int akindex = 0;

  int chunkindex = 0;

  long tid = 0;
  memcpy(&tid, &i_tid, sizeof( atrshmlog_tid_t));
  
  //  printf("payload is pid %ld tid %lx %d %d\n", (long)i_pid, tid, i_buffer_end, i_read_len);

  printf("id %4ld acquiretime %8ld pid %6ld tid %8ld slavetime %9ld readertime %9ld payloadsize %7ld shmbuffer %3d filenumber %7d \n",
	 (long)i_id,
	 (long)i_acquiretime,
	 (long)i_pid,
	 tid,
	 (long)(i_lasttsc_before - i_starttransfer),
	 (long)i_difftimetransfer,
	 (long)i_buffer_end,
	 i_buffernumber,
	 i_filenumber
	 );

  /* we calc the starttime and endtime in real */

  long long tscstart = i_inittsc_after;

  tscstart += i_inittsc_before;

  tscstart /= 2;

  #if 	  ATRSHMLOGDEBUG ==  1
  printf("start tsc %lld %lld %lld\n", (long long) i_inittsc_after, (long long)i_inittsc_before, tscstart);
  #endif
  long long tscend = i_lasttsc_before;

  tscend += i_lasttsc_after;

  tscend /= 2;

  #if 	  ATRSHMLOGDEBUG ==  1
  printf("last tsc %lld %lld %lld\n", (long long) i_lasttsc_after, (long long)i_lasttsc_before, tscend);
  #endif
  long long tscdiff = tscend - tscstart;

  #if 	  ATRSHMLOGDEBUG ==  1
  printf("diff tsc %lld \n",  tscdiff);
  #endif
  //  printf("Meantimes %lld %lld %lld\n", tscstart, tscend, tscdiff) ; 

  /* we can now start to give the realtime a try
   * we assume the tsc is in the middle of the fetch of the real time
   * and so we give all tsc now a new realtime 
   */

  /* ok. we have now the tsc. we now need the real */

  long long realstart;

  realstart = i_inittime.tv_sec;
  
  #if 	  ATRSHMLOGDEBUG ==  1
  printf("realstart %lld \n",  realstart);
  #endif

  realstart *=  (1000L * 1000L * 1000L);

  #if 	  ATRSHMLOGDEBUG ==  1
  printf("realstart %lld \n",  realstart);
  #endif
  realstart += i_inittime.tv_nsec;

  #if 	  ATRSHMLOGDEBUG ==  1
  printf("realstart %lld \n",  realstart);
  #endif
  long long realend;

  realend = i_lasttime.tv_sec;
  
  #if 	  ATRSHMLOGDEBUG ==  1
  printf("realend %lld \n",  realend);
  #endif

  realend *=  (1000L * 1000L * 1000L);

  #if 	  ATRSHMLOGDEBUG ==  1
  printf("realend %lld \n",  realend);
  #endif

  realend += i_lasttime.tv_nsec;

  #if 	  ATRSHMLOGDEBUG ==  1
  printf("realend %lld \n",  realend);
  #endif

  long long realdiff = realend - realstart;
  
  #if 	  ATRSHMLOGDEBUG ==  1
  printf("realdiff %lld \n",  realdiff);
  #endif
  // printf("Realtimes %lld %lld %lld\n", realstart, realend, realdiff);

  /* and now we can start with a realtime adder */
  
  while (akindex < i_read_len)
    {
      int chunklen;
      atrshmlog_time_t starttime;
      atrshmlog_time_t endtime;
      int event;
      char eventflag = 0;
      int ucs2string = 0;
      int userflag;

      char h[ATRSHMLOGCONTROLDATASIZE];
      
      char* dest;

      char* s;
      
      /* we move the vals from the actual chunk starting at akindex to
       * the helper vars 
       */

      chunkindex++;
      
      s = i_buffer + akindex;

      memcpy (h, s, ATRSHMLOGCONTROLDATASIZE);


      s += ATRSHMLOGCONTROLDATASIZE;
      
      /* check if this is possible */
      atrshmlog_int32_t totallen;

      memcpy (&totallen, h + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_time_t) , sizeof(atrshmlog_int32_t));
      
      if (totallen < ATRSHMLOGCONTROLDATASIZE)
	{
	  fprintf(stderr, "error h.totallen < control in %d : %d.\n", chunkindex, totallen);
	  return -1;
	}


      if (totallen > ATRSHMLOGBUFFER_INFOSIZE)
	{
	  fprintf(stderr, "error h.totallen > infosize in %d.\n", chunkindex);
	  return -1;
	}
      
      {
	size_t lcount = totallen - ATRSHMLOGCONTROLDATASIZE;
	void* src = s;
	void* dest = payloadbuffer;

	if (lcount > 0)
	  {
	    memcpy(dest, src, lcount);
	    /* printf("payload is %d\n", lcount); */
	  }
    
	payloadbuffer[lcount] = '\0';
	/* printf("pl %s\n", payloadbuffer); */
      }

      akindex += totallen;
      
      {
	/* calc the output values 
	 * and write out in file 
	 */
	char controlbuffer [ 256 ];
	char* lastpos;
	char* nextcontrol;
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
	
	sprintf(tidstr, "%016ld", tid);

        sprintf(bnr, "%03d", i_buffernumber);

	sprintf(fnr, "%018d", i_filenumber);
	
	atrshmlog_time_t stime;

	memcpy(&stime, h + 0, sizeof(atrshmlog_time_t));

	long long s;

#if 0
	s = stime.tv_sec;
	s *= (1000L * 1000L * 1000L);
	s += stime.tv_nsec;
#endif

	s = stime;

	/* we now calc the real time
	 * (reals - realstart)/  (realend - realstart)  
         * eq (s - tstart) / (tend - tstart)  
         * so 
         * (reals - realstart) = (s - tstart) * (realend - realstart) / (tend - tstart)
         * so
         * reals = realstart + ((s - tstart) * (realend - realstart) / (tend - tstart))
         */
	long double reals_d =  realstart;

	long double quots_d = s;

	quots_d -= tscstart;
	
	quots_d *= realdiff;
        
	reals_d += quots_d / tscdiff;

	long long reals = reals_d;
	
	atrshmlog_time_t etime;
	memcpy(&etime, h + sizeof(atrshmlog_time_t) , sizeof(atrshmlog_time_t));

	long long e;

#if 0
	e = etime.tv_sec ;
	e *= (1000L * 1000L * 1000L);
	e+= etime.tv_nsec;
#endif
	e = etime;

	long double reale_d =  realstart;

	long double quote_d = e;

	quote_d -= tscstart;
	
	quote_d *= realdiff;
        
	reale_d += quote_d / tscdiff;

	long long reale = reale_d;
	
	long long reald = reale - reals;

#if 0
	if (stime.tv_sec > etime.tv_sec )
	  printf("time error \n");

	if (stime.tv_nsec >= (1000L * 1000L * 1000L))
	  printf("nsec too big \n");
	
	if (etime.tv_nsec >= (1000L * 1000L * 1000L))
	  printf("nsec too big \n");
	#endif
#if 0
	sprintf(starttimestr, "%018ld %018ld", (long)stime.tv_sec, (long)stime.tv_nsec );
	
	
	sprintf(endtimestr, "%018ld %018ld", (long)etime.tv_sec, (long)etime.tv_nsec );
#else

	sprintf(starttimestr, "%018lld", s);
	
	sprintf(endtimestr, "%018lld", e);

	sprintf(deltastr, "%018lld", e - s);


	sprintf(realstarttimestr, "%018lld", reals);
	
	sprintf(realendtimestr, "%018lld", reale);

	sprintf(realdeltastr, "%018lld", reald);

	
#endif
	
	sprintf(controlbuffer,
		"%010ld ",
		(long)i_pid);

	strcat(controlbuffer, tidstr);

	strcat(controlbuffer, " ");

	strcat(controlbuffer, bnr);

	strcat(controlbuffer, " ");

	strcat(controlbuffer, fnr);

	strcat(controlbuffer, " ");

	strcat(controlbuffer, starttimestr);

	strcat(controlbuffer , " ");

	strcat(controlbuffer, endtimestr);

	strcat(controlbuffer , " ");

	strcat(controlbuffer, deltastr);

	strcat(controlbuffer , " ");

	strcat(controlbuffer, realstarttimestr);

	strcat(controlbuffer , " ");

	strcat(controlbuffer, realendtimestr);

	strcat(controlbuffer , " ");

	strcat(controlbuffer, realdeltastr);

	strcat(controlbuffer , " ");

	atrshmlog_int32_t eventnumber;

	memcpy(&eventnumber, h + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_int32_t), sizeof(atrshmlog_int32_t));
    
	sprintf(eventstr, "%010ld ", (long)eventnumber);

	strcat(controlbuffer, eventstr);
	eventflag = *(h + (ATRSHMLOGCONTROLDATASIZE - 1));
	eventflagstr[0] = eventflag;
	eventflagstr[1] = ' ' ; 
	eventflagstr[2] = '\0';
	
	strcat(controlbuffer, eventflagstr);

	if (eventflag == ATRSHMLOGPOINTINTIMEp || eventflag == ATRSHMLOGPOINTINTIMEi)
	  ucs2string = 1;
	
	atrshmlog_int32_t userevent;

	memcpy(&userevent, h + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_int32_t) + sizeof(atrshmlog_int32_t), sizeof(atrshmlog_int32_t));
    
	sprintf(userstr, "%010ld", (long)userevent);
    
	strcat(controlbuffer, userstr);

	nextcontrol = payloadbuffer, lastpos = payloadbuffer;

	/* printf(" payoad %s\n", payloadbuffer); */

	if (ucs2string)
	  {
	    convertucs2(nextcontrol, (totallen - ATRSHMLOGCONTROLDATASIZE), controlbuffer, i_fout);
	  }
	else
	  {
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
    }
  
  /* ok. we are out normal 
  * we adjust end and newend and memove the bufer rest
  */
 leave1:

  return 0;
}




static void convertucs2(char *inputstringbuffer,
			int lentimes2,
			const char *prefix,
			FILE *i_fout)
{
  unsigned char *p = inputstringbuffer;

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
