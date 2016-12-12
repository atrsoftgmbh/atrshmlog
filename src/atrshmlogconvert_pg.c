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

/** \file atrshmlogconvert_pg.c
 * \brief We convert binary files in this version format.
 *
 * We use the same as convert, but then we do not insert in file.
 * instead we have a database connect to make and insert there.
 * the thing tries to be independent to the db.
 */


#include "atrshmlog.h"

#include "atrshmlog_internal.h"

#include <stdio.h>


/*
 *--------------------------------------------------------
 * 
 * db specific part : incudes and definitions in dbsession
 */


// we are the postgreSQL 
#include <libpq-fe.h>

/*
 *--------------------------------------------------------
 */

struct atrshmlog_dbsession_s {
  PGconn *conn;
  PGresult *res;
  // we have a small state machine here
  // 0 : not connected
  // 1 : connect ready
  // 2 : statements prepared
  // 3 : in transaction
  int status;
  const char * const *keywords;
  const char * const *values;
};

typedef struct atrshmlog_dbsession_s atrshmlog_dbsession_t;

/**
 * db specific function : create
 */
static atrshmlog_dbsession_t* create_db(void);

/**
 * db specific function : cleanup
 */
static void cleanup_db(atrshmlog_dbsession_t* i_db);

/**
 * db specific function : begin
 */
static int begin_db(atrshmlog_dbsession_t* i_db);

/**
 * db specific function : commit
 */
static int commit_db(atrshmlog_dbsession_t* i_db);

/**
 * db specific function : rollback
 */
static int rollback_db(atrshmlog_dbsession_t* i_db);

/**
 * db specific function : get head seq
 */
static int get_head_seq_db(atrshmlog_dbsession_t* i_db,
			   uint64_t *o_head_id);

/**
 * db specific function : insert head
 */
static int insert_db_head(atrshmlog_dbsession_t* i_db,
			     uint64_t i_head_id,
			  atrshmlog_io_head_t *i_head);

/**
 * db specific function : insert cstring
 */
static int insert_db_cstring(atrshmlog_dbsession_t* i_db,
			     uint64_t i_head_id,
			     atrshmlog_io_head_t *i_head,
			     atrshmlog_chunk_head_t* i_chunk);

/**
 * db specific function : insert ucs2 string
 */
static int insert_db_ucs2string(atrshmlog_dbsession_t* i_db,
				uint64_t i_head_id,
				atrshmlog_io_head_t *i_head,
				atrshmlog_chunk_head_t* i_chunk,
				int len);


/**
 *  helper to generate lines from a chunk 
 */
static int operate(atrshmlog_dbsession_t *i_db,
		   char * const i_buffer,
		   atrshmlog_io_head_t head,
		   int sbyteorder);

static void convertucs2(char* nextcontrol,
			int len,
			const char *prefix);



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
  FILE* fin = NULL;

  // int fin;
  
  int buffer_index;

  char *end;

  int payloadlen = 0;

  int ret = 0;
  
  if (argc < 2)
    {
      printf("usage: %s infile\n", argv[0]);
      printf("parameter count wrong.\n");
      printf("logsystem version is %d.\n", ATRSHMLOGVERSION);
      exit(1);
    }

  printf("shm log converter from file '%s' to pgdb.\n", argv[1]);


  fin = fopen(argv[1], "rb");

  if (fin == NULL )
    {
      fprintf(stderr, "error in open file %s.\n" , argv[1]);
      return 1;
    }

  atrshmlog_dbsession_t *dbsession = NULL;

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

    // we create the db session and the insert commands.

    dbsession = create_db();

    if (dbsession == NULL)
      goto ende;

    int dbret = begin_db(dbsession);
    
    if (operate(dbsession, buffer, head, sbyteorder) != 0)
      {
	fprintf(stderr, "error in operate, stop.\n");
	ret = 2;
	dbret = rollback_db(dbsession);
	
	goto ende;
      }

    dbret = commit_db(dbsession);

  }
  
 ende:

  cleanup_db(dbsession);
  
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
int operate(atrshmlog_dbsession_t *i_db,
	    char * const i_buffer,
	    atrshmlog_io_head_t head,
	    int sbyteorder)

{
  int result = -1;

  const uint16_t o = 1;

  const int horder_is_n_order = (*((unsigned char*)&o) == 0);

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

  int tlen = head.tlen;
  
  if (!horder_is_n_order)
    {
      atrshmlog_io_head_change_order(&head);
    }

  uint64_t head_id;

  int retdb = get_head_seq_db(i_db, &head_id);

  if (retdb != 0)
    {
      return -1;
    }

  retdb = insert_db_head(i_db, head_id, &head);
	
  if (retdb != 0)
    {
      return -1;
    }

  atrshmlog_chunk_head_t c;

  int totallen ;
  
  for (int akindex = 0; akindex < tlen; akindex += totallen)
    {
      chunkindex++;

      atrshmlog_fill_chunk_head (i_buffer + akindex, &c);

      if (sbyteorder)
	{
	  atrshmlog_chunk_head_change_order(&c);
	}

      totallen = c.totallen;
      
      if (totallen < ATRSHMLOGCONTROLDATASIZE || totallen > ATRSHMLOGBUFFER_INFOSIZE)
	{
	  fprintf(stderr, "error c.totallen : size :  in %d : %d.\n", chunkindex, totallen);
	  return -1;
	}

      size_t lcount = totallen - ATRSHMLOGCONTROLDATASIZE;

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

      c.startreal = reals_d;
	
      long double reale_d =  realstart;

      long double quote_d = c.endtime;

      quote_d -= tscstart_d;
	
      quote_d *= realdiff;
        
      reale_d += quote_d / tscdiff_d;

      c.endreal = reale_d;

      c.deltareal = c.endreal - c.startreal;

      c.deltatime = c.endtime - c.starttime;
      
      //      long long reald = reale - reals;

      // NOW WE ONLY PRINT AND WRITE OUT.
      char controlbuffer [ 256 ];

      controlbuffer[0] = 0;
      
#if 0
      char pidstr[40];
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
      

      snprintf(pidstr, 40, "%016ld", (long)head.pid);

      snprintf(tidstr, 40, "%016ld", (long)head.tid);
	
      snprintf(bnr, 40, "%03d", (int)head.buffernumber);

      snprintf(fnr, 40, "%018d", (int)head.filenumber);
	
      snprintf(starttimestr, 40, "%018lld", (long long)c.starttime);
	
      snprintf(endtimestr, 40, "%018lld", (long long)c.endtime);

      snprintf(deltastr, 40, "%018lld", (long long)(c.endtime - c.starttime));


      snprintf(realstarttimestr, 40, "%018lld", reals);
	
      snprintf(realendtimestr, 40, "%018lld", reale);

      snprintf(realdeltastr, 40, "%018lld", reald);

	
      snprintf(eventstr, 40, "%010ld ", (long)c.eventnumber);

      snprintf(userstr, 40, "%010ld", (long)c.userflag);
    
      eventflagstr[0] = c.eventflag;
      eventflagstr[1] = ' ' ; 
      eventflagstr[2] = '\0';

      MYCAT(controlbuffer, pidstr);

      MYCAT(controlbuffer, " ");

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

#endif
      
      if (ucs2string)
	{
	  if (!horder_is_n_order)
	    {
	      atrshmlog_chunk_head_change_order(&c);
	    }
	  
	  c.payload = payloadbuffer;
	      
	  int retdb = insert_db_ucs2string(i_db, head_id, &head, &c, (totallen - ATRSHMLOGCONTROLDATASIZE));

	  if (retdb != 0)
	    {
	      return -1;
	    }
	}
      else
	{
	  char* lastpos = payloadbuffer;
	  char* nextcontrol = payloadbuffer;

	  if (!horder_is_n_order)
	    {
	      atrshmlog_chunk_head_change_order(&c);
	    }
	  
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

	      c.payload = lastpos;
	      
	      int retdb = insert_db_cstring(i_db, head_id, &head, &c);

	      if (retdb != 0)
		{
		  return -1;
		}
	      
	      /*
	      fprintf(i_fout, "%s %s\n",
		      controlbuffer,
		      lastpos
		      );
	      */
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
			const char *prefix)
{
#if 0
  unsigned char *p = (unsigned char*)inputstringbuffer;

  char *target = inputstringbuffer; // dirty. we use the buffer itself for write

  int converted = 0;

  char *laststart = target;
  
  for (int i = 0 ; i <  (lentimes2 / 2); i++, p += 2)
    {
      int ch = (unsigned)((p[1] & 255) << 8) & ( p[0] & 255);

      if (ch == '\n')
	{
	  //	  fprintf(i_fout, "%s ", prefix);

	  if(converted > 0)
	    fwrite(laststart,
		   1,
		   converted,
		   i_fout);

	  //  fprintf(i_fout, "\n");
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
  #endif
}


/*********************************************************************/

/* 
 * postgreSQL code is here 
 */

static char *kw[9] =
  {
    "host",
    "hostaddr",
    "port",
    "dbname",
    "user",
    "password",
    "connect_timeout",
    "application_name",
    0
  };

static char *val[9]; 

static char appname[63];

/**
 * db specific funtion : create
 */
static atrshmlog_dbsession_t* create_db(void)
{
  atrshmlog_dbsession_t* r =   (atrshmlog_dbsession_t*)calloc(1, sizeof(  atrshmlog_dbsession_t));

  if (r == NULL)
    return r;

  //  printf("create_db alloc\n");
  
  // we have ram ...

  r->status  = 0; // no init so far.

  r->keywords = (const char* const *)kw;

  val[0] = (char*)ATRSHMLOG_GET_ENV("_PQHOST");
  val[1] = (char*)ATRSHMLOG_GET_ENV("_PQHOSTADDR");
  val[2] = (char*)ATRSHMLOG_GET_ENV("_PQPORT");
  val[3] = (char*)ATRSHMLOG_GET_ENV("_PQDBNAME");
  val[4] = (char*)ATRSHMLOG_GET_ENV("_PQUSER");
  val[5] = (char*)ATRSHMLOG_GET_ENV("_PQPWD");
  val[6] = "10"; // 10 seconds

  snprintf(appname, 63, "ATRSHMLOGCONVERT_PG%ld", (long)getpid());
  
  val[7] = appname;

  val[8] = 0;
  
  //   printf("create_db alloc %s\n", val[0]);
  // printf("create_db alloc %s\n", val[1]);
  // printf("create_db alloc %s\n", val[2]);
  // printf("create_db alloc %s\n", val[3]);
  // printf("create_db alloc %s\n", val[4]);
  // printf("create_db alloc %s\n", val[5]);
  // printf("create_db alloc %s\n", val[6]);
  // printf("create_db alloc %s\n", val[7]);
  
  r->values = (const char* const *)val;

  int expand_dbname = 0;
  
  r->conn = PQconnectdbParams(r->keywords,
			      r->values,
			      expand_dbname);

  if (r->conn == NULL)
    {
      free(r);
      printf("create_db no connect\n");
      return NULL;
    }

  ConnStatusType pqstat = PQstatus(r->conn);

  if (pqstat != CONNECTION_OK)
    {
      printf("create_db connect bad %s\n", PQerrorMessage(r->conn));
      free(r);
      return NULL;
    }

  // we are connected. from now on we need a PGfinish.
  r->status = 1;

  // prepare the statements
  r->res = PQprepare(r->conn,
		     "commit_db",
		     "COMMIT",
		     0,
		     NULL);

  if (r->res == NULL)
    {
      printf("create_db result null.\n");
      cleanup_db(r);
      return NULL;
    }

      
  ExecStatusType es = PQresultStatus(r->res);

  if (es != PGRES_COMMAND_OK)
    {
      printf("create_db commit bad %s\n", PQerrorMessage(r->conn));
      cleanup_db(r);
      return NULL;
    }

  PQclear(r->res);

  r->res = PQprepare(r->conn,
		     "rollback_db",
		     "ROLLBACK",
		     0,
		     NULL);

  if (r->res == NULL)
    {
      printf("create_db result null.\n");
      cleanup_db(r);
      return NULL;
    }

  es = PQresultStatus(r->res);

  if (es != PGRES_COMMAND_OK)
    {
      printf("create_db rollback bad %s\n", PQerrorMessage(r->conn));
      cleanup_db(r);
      return NULL;
    }

  PQclear(r->res);

  r->res = PQprepare(r->conn,
		     "begin_db",
		     "BEGIN",
		     0,
		     NULL);

  if (r->res == NULL)
    {
      printf("create_db result null.\n");
      cleanup_db(r);
      return NULL;
    }

  es = PQresultStatus(r->res);

  if (es != PGRES_COMMAND_OK)
    {
      printf("create_db begin bad %s\n", PQerrorMessage(r->conn));
      cleanup_db(r);
      return NULL;
    }

  PQclear(r->res);

  r->res = PQprepare(r->conn,
		     "get_head_seq_db",
		     "SELECT nextval('atrshmlog_head_seq')",
		     0,
		     NULL);

  if (r->res == NULL)
    {
      printf("create_db result null.\n");
      cleanup_db(r);
      return NULL;
    }

  es = PQresultStatus(r->res);

  if (es != PGRES_COMMAND_OK)
    {
      printf("create_db get_head_seq bad %s\n", PQerrorMessage(r->conn));
      cleanup_db(r);
      return NULL;
    }

  PQclear(r->res);

  r->res = PQprepare(r->conn,
		     "insert_db_head",
		     "insert into atrshmlog_head(head_id,"
		     "version,"
		     "totallen,"
		     "pid,"
		     "tid,"
		     "bnr,"
		     "fnr,"
		     "inittime_sec,"
		     "inittime_nsec,"
		     "inittime_before,"
		     "inittime_after,"
		     "lasttime_sec,"
		     "lasttime_nsec,"
		     "lasttime_before,"
		     "lasttime_after,"
		     "difftimetransfer,"
		     "starttransfer,"
		     "acquiretime,"
		     "bid,"
		     "number_dispatched,"
		     "counter_write0,"
		     "counter_write0_discard,"
		     "counter_write0_wait,"
		     "counter_write0_adaptive,"
		     "counter_write0_adaptive_fast,"
		     "counter_write0_adaptive_very_fast,"
		     "counter_write1,"
		     "counter_write1_discard,"
		     "counter_write1_wait,"
		     "counter_write1_adaptive,"
		     "counter_write1_adaptive_fast,"
		     "counter_write1_adaptive_very_fast,"
		     "counter_write2,"
		     "counter_write2_discard,"
		     "counter_write2_wait,"
		     "counter_write2_adaptive,"
		     "counter_write2_adaptive_fast,"
		     "counter_write2_adaptive_very_fast)"
		     " values ("
		     "$1::bigint,"
		     "$2::integer,"
		     "$3::integer,"
		     "$4::bigint,"
		     "$5::bigint,"
		     "$6::integer,"
		     "$7::integer,"
		     "$8::bigint,"
		     "$9::integer,"
		     "$10::bigint,"
		     "$11::bigint,"
		     "$12::bigint,"
		     "$13::integer,"
		     "$14::bigint,"
		     "$15::bigint,"
		     "$16::bigint,"
		     "$17::bigint,"
		     "$18::bigint,"
		     "$19::integer,"
		     "$20::integer,"
		     "$21::integer,"
		     "$22::integer,"
		     "$23::integer,"
		     "$24::integer,"
		     "$25::integer,"
		     "$26::integer,"
		     "$27::integer,"
		     "$28::integer,"
		     "$29::integer,"
		     "$30::integer,"
		     "$31::integer,"
		     "$32::integer,"
		     "$33::integer,"
		     "$34::integer,"
		     "$35::integer,"
		     "$36::integer,"
		     "$37::integer,"
		     "$38::integer)",
		     0,
		     NULL);

  if (r->res == NULL)
    {
      printf("create_db result null.\n");
      cleanup_db(r);
      return NULL;
    }

  es = PQresultStatus(r->res);

  if (es != PGRES_COMMAND_OK)
    {
      printf("create_db insert_db_head bad %s\n", PQerrorMessage(r->conn));
      cleanup_db(r);
      return NULL;
    }

  PQclear(r->res);

  r->res = PQprepare(r->conn,
		     "insert_db_cstring",
		     "insert into atrshmlog_log(head_id,pid,tid,startclick,endclick,deltaclick,startreal,endreal,deltareal,logevent,logflag,userflag,payload) "
		     " values ("
		     "$1::bigint,"
		     "$2::bigint,"
		     "$3::bigint,"
		     "$4::bigint,"
		     "$5::bigint,"
		     "$6::bigint,"
		     "$7::bigint,"
		     "$8::bigint,"
		     "$9::bigint,"
		     "$10::integer,"
		     "$11::char(1),"
		     "$12::integer,"
		     "$13)",
		     0,
		     NULL);

  if (r->res == NULL)
    {
      printf("create_db result null.\n");
      cleanup_db(r);
      return NULL;
    }

  es = PQresultStatus(r->res);

  if (es != PGRES_COMMAND_OK)
    {
      printf("create_db insert_db_cstring bad %s\n", PQerrorMessage(r->conn));
      cleanup_db(r);
      return NULL;
    }

  PQclear(r->res);

  r->res = PQprepare(r->conn,
		     "insert_db_ucs2string",
		     "insert into atrshmlog_log_ucs2(head_id,pid,tid,startclick,endclick,deltaclick,startreal,endreal,deltareal,logevent,logflag,userflag,payload) "
		     " values ("
		     "$1::bigint,"
		     "$2::bigint,"
		     "$3::bigint,"
		     "$4::bigint,"
		     "$5::bigint,"
		     "$6::bigint,"
		     "$7::bigint,"
		     "$8::bigint,"
		     "$9::bigint,"
		     "$10::integer,"
		     "$11::char(1),"
		     "$12::integer,"
		     "$13)",
		     0,
		     NULL);

  if (r->res == NULL)
    {
      printf("create_db result null.\n");
      cleanup_db(r);
      return NULL;
    }

  es = PQresultStatus(r->res);

  if (es != PGRES_COMMAND_OK)
    {
      printf("create_db insert_db_ucs2string bad %s\n", PQerrorMessage(r->conn));
      cleanup_db(r);
      return NULL;
    }

  PQclear(r->res);

#if 0  
  // we get the insert statement info

  r->res = PQdescribePrepared(r->conn, "insert_db_cstring");

  if (r->res == NULL)
    {
      printf("create_db result null.\n");
      cleanup_db(r);
      return NULL;
    }

  es = PQresultStatus(r->res);

  if (es != PGRES_COMMAND_OK)
    {
      printf("create_db insert_db_cstring describe bad %s\n", PQerrorMessage(r->conn));
      cleanup_db(r);
      return NULL;
    }

  // ok. info is here
  int nr = PQnparams(r->res);

  printf("filds %d\n", nr);

  for (int par = 0; par < nr; par++)
    {
      Oid o = PQparamtype(r->res, par);
      printf("par %d : %ld\n", par , (long)o);
    }
  
  PQclear(r->res);
#endif
  
  // we have prepared statements
  r->status = 2;
  
  return r;
}

/**
 * db specific funtion : cleanup
 */
static void cleanup_db(atrshmlog_dbsession_t* i_db)
{
  if (i_db == NULL)
    return;

  if (i_db->status == 0)
    return;

  if (i_db->status == 2)
    {
      // free the statements
    }
  
  if (i_db->status == 3)
    {
      rollback_db(i_db);

      // free the statements

    }
  
  PQfinish(i_db->conn);

  i_db->status = 0;

  free(i_db);
}

/**
 * db specific funtion : begin
 */
static int begin_db(atrshmlog_dbsession_t* i_db)
{
  if (i_db == NULL)
    return -1;

  if (i_db->status != 2)
    return -2;

  i_db->res = PQexecPrepared(i_db->conn,
			     "begin_db",
			     0, // no parameters
			     NULL, 
			     NULL,
			     NULL,
			     1);

  ExecStatusType es = PQresultStatus(i_db->res);

  if (es != PGRES_COMMAND_OK)
    {
      printf("begin_db bad %s\n", PQerrorMessage(i_db->conn));
    }
  
  PQclear(i_db->res);

  if (es != PGRES_COMMAND_OK)
    {
      return -3;
    }
  
  // we are now in a transaction
  i_db->status = 3;

  return 0;
}

/**
 * db specific funtion : commit
 */
static int commit_db(atrshmlog_dbsession_t* i_db)
{
  if (i_db == NULL)
    return -1;

  if (i_db->status != 3)
    return -2;

  i_db->res = PQexecPrepared(i_db->conn,
			     "commit_db",
			     0, // no parameters
			     NULL, 
			     NULL,
			     NULL,
			     1);
  
  ExecStatusType es = PQresultStatus(i_db->res);

  if (es != PGRES_COMMAND_OK)
    {
      printf("commit_db bad %s\n", PQerrorMessage(i_db->conn));
    }
  
  PQclear(i_db->res);

  if (es != PGRES_COMMAND_OK)
    {
      return -3;
    }

  // we have ended the transaction
  i_db->status = 2;

  return 0;
}

/**
 * db specific funtion : rollback
 */
static int rollback_db(atrshmlog_dbsession_t* i_db)
{
  if (i_db == NULL)
    return -1;

  if (i_db->status != 3)
    return -2 ;

  i_db->res = PQexecPrepared(i_db->conn,
			     "rollback_db",
			     0, // no parameters
			     NULL, 
			     NULL,
			     NULL,
			     1);
  ExecStatusType es = PQresultStatus(i_db->res);

  if (es != PGRES_COMMAND_OK)
    {
      printf("rollback_db bad %s\n", PQerrorMessage(i_db->conn));
    }
  
  PQclear(i_db->res);

  if (es != PGRES_COMMAND_OK)
    {
      return -3;
    }

  // we have ended the transaction
  i_db->status = 2;

  return 0;
}


/**
 * db specific funtion : insert_db_head
 */
static int insert_db_head(atrshmlog_dbsession_t* i_db,
			     uint64_t i_head_id,
			     atrshmlog_io_head_t *i_head)
{
  if (i_db  == NULL)
    return -1;

  if (i_db->status != 3)
    return -2;

  char*pv[39];

  pv[0] = (char*)&i_head_id,
  pv[1] = (char*)&i_head->version;
  pv[2] = (char*)&i_head->tlen;
  pv[3] = (char*)&i_head->pid;
  pv[4] = (char*)&i_head->tid;
  pv[5] = (char*)&i_head->buffernumber;
  pv[6] = (char*)&i_head->filenumber;
  pv[7] = (char*)&i_head->inittime.tv_sec;
  pv[8] = (char*)&i_head->inittime.tv_nsec;
  pv[9] = (char*)&i_head->inittsc_before;
  pv[10] = (char*)&i_head->inittsc_after;
  pv[11] = (char*)&i_head->lasttime.tv_sec;
  pv[12] = (char*)&i_head->lasttime.tv_nsec;
  pv[13] = (char*)&i_head->lasttsc_before;
  pv[14] = (char*)&i_head->lasttsc_after;
  pv[15] = (char*)&i_head->difftimetransfer;
  pv[16] = (char*)&i_head->starttransfer;
  pv[17] = (char*)&i_head->acquiretime;
  pv[18] = (char*)&i_head->id;
  pv[19] = (char*)&i_head->number_dispatched;
  pv[20] = (char*)&i_head->counter_write0;
  pv[21] = (char*)&i_head->counter_write0_discard;
  pv[22] = (char*)&i_head->counter_write0_wait;
  pv[23] = (char*)&i_head->counter_write0_adaptive;
  pv[24] = (char*)&i_head->counter_write0_adaptive_fast;
  pv[25] = (char*)&i_head->counter_write0_adaptive_very_fast;
  pv[26] = (char*)&i_head->counter_write1;
  pv[27] = (char*)&i_head->counter_write1_discard;
  pv[28] = (char*)&i_head->counter_write1_wait;
  pv[29] = (char*)&i_head->counter_write1_adaptive;
  pv[30] = (char*)&i_head->counter_write1_adaptive_fast;
  pv[31] = (char*)&i_head->counter_write1_adaptive_very_fast;
  pv[32] = (char*)&i_head->counter_write2;
  pv[33] = (char*)&i_head->counter_write2_discard;
  pv[34] = (char*)&i_head->counter_write2_wait;
  pv[35] = (char*)&i_head->counter_write2_adaptive;
  pv[36] = (char*)&i_head->counter_write2_adaptive_fast;
  pv[37] = (char*)&i_head->counter_write2_adaptive_very_fast;
  pv[38] = 0;

  int plen[39] ;

  plen[0] = 8;
  plen[1] = 4;
  plen[2] = 4;
  plen[3] = 8;
  plen[4] = 8;
  plen[5] = 4;
  plen[6] = 4;
  plen[7] = 8;
  plen[8] = 4;
  plen[9] = 8;
  plen[10] = 8;
  plen[11] = 8;
  plen[12] = 4;
  plen[13] = 8;
  plen[14] = 8;
  plen[15] = 8;
  plen[16] = 8;
  plen[17] = 8;
  plen[18] = 4;
  plen[19] = 4;
  plen[20] = 4;
  plen[21] = 4;
  plen[22] = 4;
  plen[23] = 4;
  plen[24] = 4;
  plen[25] = 4;
  plen[26] = 4;
  plen[27] = 4;
  plen[28] = 4;
  plen[29] = 4;
  plen[30] = 4;
  plen[31] = 4;
  plen[32] = 4;
  plen[33] = 4;
  plen[34] = 4;
  plen[35] = 4;
  plen[36] = 4;
  plen[37] = 4;
  plen[38] = 0;

  int pfor[39] ;

  pfor[0] = 1;
  pfor[1] = 1;
  pfor[2] = 1;
  pfor[3] = 1;
  pfor[4] = 1;
  pfor[5] = 1;
  pfor[6] = 1;
  pfor[7] = 1;
  pfor[8] = 1;
  pfor[9] = 1;
  pfor[10] = 1;
  pfor[11] = 1;
  pfor[12] = 1;
  pfor[13] = 1;
  pfor[14] = 1;
  pfor[15] = 1;
  pfor[16] = 1;
  pfor[17] = 1;
  pfor[18] = 1;
  pfor[19] = 1;
  pfor[20] = 1;
  pfor[21] = 1;
  pfor[22] = 1;
  pfor[23] = 1;
  pfor[24] = 1;
  pfor[25] = 1;
  pfor[26] = 1;
  pfor[27] = 1;
  pfor[28] = 1;
  pfor[29] = 1;
  pfor[30] = 1;
  pfor[31] = 1;
  pfor[32] = 1;
  pfor[33] = 1;
  pfor[34] = 1;
  pfor[35] = 1;
  pfor[36] = 1;
  pfor[37] = 1;
  pfor[38] = 1;
  
  i_db->res = PQexecPrepared(i_db->conn,
			     "insert_db_head",
			     38,
			     (const char * const *)pv,
			     (const int *)plen,
			     (const int *)pfor,
			     1); // we do this binary
  
  ExecStatusType es = PQresultStatus(i_db->res);

  if (es != PGRES_COMMAND_OK)
    {
      printf("insert_db_head bad %s\n", PQerrorMessage(i_db->conn));
    }
  
  PQclear(i_db->res);

  if (es != PGRES_COMMAND_OK)
    {
      return -3;
    }

  return 0;
}

/**
 * db specific funtion : insert_db_cstring
 */
static int insert_db_cstring(atrshmlog_dbsession_t* i_db,
			     uint64_t i_head_id,
			     atrshmlog_io_head_t *i_head,
			     atrshmlog_chunk_head_t* i_chunk)
{
  if (i_db  == NULL)
    return -1;

  if (i_db->status != 3)
    return -2;

  char*pv[15];

  pv[0] = (char*)&i_head_id,
  pv[1] = (char*)&i_head->pid;
  pv[2] = (char*)&i_head->tid;
  pv[3] = (char*)&i_chunk->starttime;
  pv[4] = (char*)&i_chunk->endtime;
  pv[5] = (char*)&i_chunk->deltatime;
  pv[6] = (char*)&i_chunk->startreal;
  pv[7] = (char*)&i_chunk->endreal;
  pv[8] = (char*)&i_chunk->deltareal;
  pv[9] = (char*)&i_chunk->eventnumber;
  pv[10] = (char*)&i_chunk->eventflag;
  pv[11] = (char*)&i_chunk->userflag;
  pv[12] = (char*)i_chunk->payload;
  pv[13] = 0;

  int plen[15] ;

  plen[0] = 8;
  plen[1] = 8;
  plen[2] = 8;
  plen[3] = 8;
  plen[4] = 8;
  plen[5] = 8;
  plen[6] = 8;
  plen[7] = 8;
  plen[8] = 8;
  plen[9] = 4;
  plen[10] = 1;
  plen[11] = 4;
  plen[12] = strlen(i_chunk->payload);
  plen[13] = 0;

  int pfor[15] ;

  pfor[0] = 1;
  pfor[1] = 1;
  pfor[2] = 1;
  pfor[3] = 1;
  pfor[4] = 1;
  pfor[5] = 1;
  pfor[6] = 1;
  pfor[7] = 1;
  pfor[8] = 1;
  pfor[9] = 1;
  pfor[10] = 1;
  pfor[11] = 1;
  pfor[12] = 0;
  pfor[13] = 1;
  
  i_db->res = PQexecPrepared(i_db->conn,
			     "insert_db_cstring",
			     13,
			     (const char * const *)pv,
			     (const int *)plen,
			     (const int *)pfor,
			     1); // we do this binary
  
  ExecStatusType es = PQresultStatus(i_db->res);

  if (es != PGRES_COMMAND_OK)
    {
      printf("insert_db_cstring bad %s\n", PQerrorMessage(i_db->conn));
    }
  
  PQclear(i_db->res);

  if (es != PGRES_COMMAND_OK)
    {
      return -3;
    }

  return 0;
}


/**
 * db specific funtion : insert_db_cstring
 */
static int insert_db_ucs2string(atrshmlog_dbsession_t* i_db,
				uint64_t i_head_id,
				atrshmlog_io_head_t *i_head,
				atrshmlog_chunk_head_t* i_chunk,
				int len)
{
  if (i_db  == NULL)
    return -1;

  if (i_db->status != 3)
    return -2;

  char*pv[15];

  pv[0] = (char*)&i_head_id,
  pv[1] = (char*)&i_head->pid;
  pv[2] = (char*)&i_head->tid;
  pv[3] = (char*)&i_chunk->starttime;
  pv[4] = (char*)&i_chunk->endtime;
  pv[5] = (char*)&i_chunk->deltatime;
  pv[6] = (char*)&i_chunk->startreal;
  pv[7] = (char*)&i_chunk->endreal;
  pv[8] = (char*)&i_chunk->deltareal;
  pv[9] = (char*)&i_chunk->eventnumber;
  pv[10] = (char*)&i_chunk->eventflag;
  pv[11] = (char*)&i_chunk->userflag;
  pv[12] = (char*)i_chunk->payload;
  pv[13] = 0;

  int plen[15] ;

  plen[0] = 8;
  plen[1] = 8;
  plen[2] = 8;
  plen[3] = 8;
  plen[4] = 8;
  plen[5] = 8;
  plen[6] = 8;
  plen[7] = 8;
  plen[8] = 8;
  plen[9] = 4;
  plen[10] = 1;
  plen[11] = 4;
  plen[12] = len;
  plen[13] = 0;

  int pfor[15] ;

  pfor[0] = 1;
  pfor[1] = 1;
  pfor[2] = 1;
  pfor[3] = 1;
  pfor[4] = 1;
  pfor[5] = 1;
  pfor[6] = 1;
  pfor[7] = 1;
  pfor[8] = 1;
  pfor[9] = 1;
  pfor[10] = 1;
  pfor[11] = 1;
  pfor[12] = 1;
  pfor[13] = 1;
  
  i_db->res = PQexecPrepared(i_db->conn,
			     "insert_db_ucs2string",
			     13,
			     (const char * const *)pv,
			     (const int *)plen,
			     (const int *)pfor,
			     1); // we do this binary
  
  ExecStatusType es = PQresultStatus(i_db->res);

  if (es != PGRES_COMMAND_OK)
    {
      printf("insert_db_cstring bad %s\n", PQerrorMessage(i_db->conn));
    }
  
  PQclear(i_db->res);

  if (es != PGRES_COMMAND_OK)
    {
      return -3;
    }

  return 0;
}

/**
 * db specific funtion : get_head_seq_db
 */
static int get_head_seq_db(atrshmlog_dbsession_t* i_db,
			   uint64_t *o_head_id)
{
  if (i_db  == NULL)
    return -1;

  if (i_db->status != 3)
    return -2;

  
  i_db->res = PQexecPrepared(i_db->conn,
			     "get_head_seq_db",
			     0,
			     (const char * const *)NULL,
			     (const int *)NULL,
			     (const int *)NULL,
			     1); // we do this binary
  
  
  ExecStatusType es = PQresultStatus(i_db->res);

  int i;
  
  if (es != PGRES_TUPLES_OK)
    {
      printf("get_head_seq_db bad %d : %s :\n", es, PQerrorMessage(i_db->conn));
    }
  else
    {
      // we get the new head_id

      int fnum = PQfnumber(i_db->res, "nextval");

      for (i = 0; i < PQntuples(i_db->res); i++)
	{
	  char *val = PQgetvalue(i_db->res, i, fnum);

	  // we get it in network order AND WE DONT CHANGE IT ...
	  *o_head_id = *(uint64_t*)val;
	}
    }

  
  PQclear(i_db->res);

  if (es != PGRES_TUPLES_OK)
    {
      return -3;
    }

  // empty result...
  if (i < 1)
    {
      return -4;
    }
  
  return 0;
}

/* end of file */
