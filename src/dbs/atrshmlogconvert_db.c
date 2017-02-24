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

/** \file atrshmlogconvert_db.c
 * \brief We convert binary files in this version format.
 *
 * We use the same as convert, but then we do not insert in file.
 * Instead we have a database connect to make and insert there.
 * The thing tries to be independent to the db.
 * So we have a db dependent part - includes, struct - in a separate file
 * and an independent rest here
 * we link this with a very thin main layer to a program with the
 * target name.
 */


#include "../atrshmlog.h"

#include "../atrshmlog_internal.h"

#include <stdio.h>


#include <stdlib.h>

#include <errno.h>

#include <string.h>

#include <ctype.h>
/*
 *--------------------------------------------------------
 * 
 * db specific part : incudes and definitions in dbsession
 *
 * we use a vanilla pointer of a struct and a procedural 
 * interface as abstraction
 */

/*
 *--------------------------------------------------------
 */

struct atrshmlog_dbsession_s ;

typedef struct atrshmlog_dbsession_s atrshmlog_dbsession_t;

/**
 * db specific function : create
 */
extern atrshmlog_dbsession_t* atrshmlog_create_db(void);

/**
 * db specific function : cleanup
 */
extern void atrshmlog_cleanup_db(atrshmlog_dbsession_t* i_db);

/**
 * db specific function : begin
 */
extern int atrshmlog_begin_db(atrshmlog_dbsession_t* i_db);

/**
 * db specific function : commit
 */
extern int atrshmlog_commit_db(atrshmlog_dbsession_t* i_db);

/**
 * db specific function : rollback
 */
extern int atrshmlog_rollback_db(atrshmlog_dbsession_t* i_db);

/**
 * db specific function : get head seq
 */
extern int atrshmlog_get_head_seq_db(atrshmlog_dbsession_t* i_db,
				     uint64_t *o_head_id);

/**
 * db specific function : insert head
 */
extern int atrshmlog_insert_db_head(atrshmlog_dbsession_t* i_db,
				    uint64_t *io_head_id,
				    atrshmlog_io_head_t *i_head);

/**
 * db specific function : insert cstring
 */
extern int atrshmlog_insert_db_cstring(atrshmlog_dbsession_t* i_db,
				       uint64_t i_head_id,
				       atrshmlog_io_head_t *i_head,
				       atrshmlog_chunk_head_t* i_chunk);

/**
 * db specific function : insert ucs2 string
 */
extern int atrshmlog_insert_db_ucs2string(atrshmlog_dbsession_t* i_db,
					  uint64_t i_head_id,
					  atrshmlog_io_head_t *i_head,
					  atrshmlog_chunk_head_t* i_chunk,
					  int len);



/**
 * helper : deallocate prepared statements in pq
 */
extern int atrshmlog_free_prepared_db(atrshmlog_dbsession_t *i_db);

/**
 * helper : we deliver the flag : need data in network order is 1
 */
extern int atrshmlog_need_data_in_network_order_db(void);

/*
 *--------------------------------------------------------
 */

/**
 *  helper to generate lines from a chunk 
 */
static int operate(atrshmlog_dbsession_t *i_db,
		   char * const i_buffer,
		   atrshmlog_io_head_t head,
		   int sbyteorder,
		   int *io_insert_head,
		   int *io_insert_ucs2,
		   int *io_insert_cstring);


/*
 *--------------------------------------------------------
 */

/**
 * \brief This is the main function called from the thin main layer
 *
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
int atrshmlog_db_main (int argc, char*argv[])
{
  FILE* fin = NULL;

  int bcount = 0;

  int hcount = 0;
  
  int ucs2count = 0;
  
  int ccount = 0;
  
  int ret = 0;
  
  if (argc < 2)
    {
      printf("usage: %s infile ...\n", argv[0]);
      printf("parameter count wrong.\n");
      printf("logsystem version is %d.\n", ATRSHMLOGVERSION);
      exit(1);
    }

  printf("shm log converter from file '%s' to db.\n", argv[1]);

  atrshmlog_time_t t1, t2;

  t1 = ATRSHMLOG_GET_TSC_CALL();

  atrshmlog_dbsession_t *dbsession = NULL;

  // we create the db session and the insert commands.

  dbsession = atrshmlog_create_db();

  // printf("after dbsession\n");

  if (dbsession == NULL)
    goto ende;

  for (int fileindex = 1; fileindex < argc; fileindex++)
    {
      atrshmlog_io_head_t head;

      memset(&head, 0, sizeof(head));

      printf("process file %s\n", argv[fileindex]);
      
      fin = fopen(argv[fileindex], "rb");

      if (fin == NULL )
	{
	  fprintf(stderr, "error in open file %s.\n" , argv[fileindex]);
	  continue;
	}

      int r = fread(&head.order, 1, 2, fin);

      if (r != 2)
	{

	  printf("order is not ok.\n");
	  fclose(fin);
	  continue;
	}

      r = fread(&head.version, 1, sizeof(atrshmlog_int32_t), fin);

      if (r != sizeof(atrshmlog_int32_t))
	{
	  printf("version is not ok.\n");
	  fclose(fin);
	  continue;
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
	  fclose(fin);
	  continue;
	}
    
      if (sbyteorder)
	{
	  head.version = atrshmlog_int32_change_order(head.version);
	}
    
      r = fread(&head.tlen, 1, sizeof(atrshmlog_int32_t), fin);

      if (r != sizeof(atrshmlog_int32_t))
	{
	  printf("tlen is not ok.\n");
	  fclose(fin);
	  continue;
	}
    
      r = fread(&head.pid, 1, sizeof(atrshmlog_pid_t), fin);

      if (r != sizeof(atrshmlog_pid_t))
	{
	  printf("pid is not ok.\n");
	  fclose(fin);
	  continue;
	}
    
      r = fread(&head.tid, 1, sizeof(atrshmlog_tid_t), fin);

      if (r != sizeof(atrshmlog_tid_t))
	{
	  printf("tid is not ok.\n");
	  fclose(fin);
	  continue;
	}

      r = fread(&head.buffernumber, 1, sizeof(atrshmlog_int32_t), fin);

      if (r != sizeof(atrshmlog_int32_t))
	{
	  printf("buffernumber is not ok.\n");
	  fclose(fin);
	  continue;
	}
    
      r = fread(&head.filenumber, 1, sizeof(atrshmlog_int32_t), fin);

      if (r != sizeof(atrshmlog_int32_t))
	{
	  printf("filenumber is not ok.\n");
	  fclose(fin);
	  continue;
	}

      r = fread(&head.inittime.tv_sec, 1, sizeof(atrshmlog_time_seconds_t), fin);

      if (r != sizeof(atrshmlog_time_seconds_t))
	{
	  printf("inittime is not ok.\n");
	  fclose(fin);
	  continue;
	}

      r = fread(&head.inittime.tv_nsec, 1, sizeof(atrshmlog_time_nanoseconds_t), fin);

      if (r != sizeof(atrshmlog_time_nanoseconds_t))
	{
	  printf("inittime is not ok.\n");
	  fclose(fin);
	  continue;
	}

      r = fread(&head.inittsc_before, 1, sizeof(atrshmlog_time_t), fin);

      if (r != sizeof(atrshmlog_time_t))
	{
	  printf("inittsc_before is not ok.\n");
	  fclose(fin);
	  continue;
	}

      r = fread(&head.inittsc_after, 1, sizeof(atrshmlog_time_t), fin);

      if (r != sizeof(atrshmlog_time_t))
	{
	  printf("inittsc_after is not ok.\n");
	  fclose(fin);
	  continue;
	}


      r = fread(&head.lasttime.tv_sec, 1, sizeof(atrshmlog_time_seconds_t), fin);

      if (r != sizeof(atrshmlog_time_seconds_t))
	{
	  printf("lasttime is not ok.\n");
	  fclose(fin);
	  continue;
	}

      r = fread(&head.lasttime.tv_nsec, 1, sizeof(atrshmlog_time_nanoseconds_t), fin);

      if (r != sizeof(atrshmlog_time_nanoseconds_t))
	{
	  printf("lasttime is not ok.\n");
	  fclose(fin);
	  continue;
	}

      r = fread(&head.lasttsc_before, 1, sizeof(atrshmlog_time_t), fin);

      if (r != sizeof(atrshmlog_time_t))
	{
	  printf("lasttsc_before is not ok.\n");
	  fclose(fin);
	  continue;
	}

      r = fread(&head.lasttsc_after, 1, sizeof(atrshmlog_time_t), fin);

      if (r != sizeof(atrshmlog_time_t))
	{
	  printf("lasttsc_after is not ok.\n");
	  fclose(fin);
	  continue;
	}

      r = fread(&head.difftimetransfer, 1, sizeof(atrshmlog_time_t), fin);

      if (r != sizeof(atrshmlog_time_t))
	{
	  printf("difftimetransfer is not ok.\n");
	  fclose(fin);
	  continue;
	}

      r = fread(&head.starttransfer, 1, sizeof(atrshmlog_time_t), fin);

      if (r != sizeof(atrshmlog_time_t))
	{
	  printf("starttransfer is not ok.\n");
	  fclose(fin);
	  continue;
	}

      r = fread(&head.acquiretime, 1, sizeof(atrshmlog_time_t), fin);

      if (r != sizeof(atrshmlog_time_t))
	{
	  printf("acquiretime is not ok.\n");
	  fclose(fin);
	  continue;
	}

      r = fread(&head.id, 1, sizeof(atrshmlog_int32_t), fin);

      if (r != sizeof(atrshmlog_int32_t))
	{
	  printf("id is not ok.\n");
	  fclose(fin);
	  continue;
	}

      r = fread(&head.number_dispatched, 1, sizeof(atrshmlog_int32_t), fin);

      if (r != sizeof(atrshmlog_int32_t))
	{
	  printf("number_dispatched is not ok.\n");
	  fclose(fin);
	  continue;
	}

      r = fread(&head.counter_write0, 1, sizeof(atrshmlog_int32_t), fin);

      if (r != sizeof(atrshmlog_int32_t))
	{
	  printf("counter_write0 is not ok.\n");
	  fclose(fin);
	  continue;
	}

      r = fread(&head.counter_write0_discard, 1, sizeof(atrshmlog_int32_t), fin);

      if (r != sizeof(atrshmlog_int32_t))
	{
	  printf("counter_write0_discard is not ok.\n");
	  fclose(fin);
	  continue;
	}

      r = fread(&head.counter_write0_wait, 1, sizeof(atrshmlog_int32_t), fin);

      if (r != sizeof(atrshmlog_int32_t))
	{
	  printf("counter_write0_wait is not ok.\n");
	  fclose(fin);
	  continue;
	}

      r = fread(&head.counter_write0_adaptive, 1, sizeof(atrshmlog_int32_t), fin);

      if (r != sizeof(atrshmlog_int32_t))
	{
	  printf("counter_write0_adaptive is not ok.\n");
	  fclose(fin);
	  continue;
	}

      r = fread(&head.counter_write0_adaptive_fast, 1, sizeof(atrshmlog_int32_t), fin);

      if (r != sizeof(atrshmlog_int32_t))
	{
	  printf("counter_write0_adaptive_fast is not ok.\n");
	  fclose(fin);
	  continue;
	}

      r = fread(&head.counter_write0_adaptive_very_fast, 1, sizeof(atrshmlog_int32_t), fin);

      if (r != sizeof(atrshmlog_int32_t))
	{
	  printf("counter_write0_adaptive_very_fast is not ok.\n");
	  fclose(fin);
	  continue;
	}

      r = fread(&head.counter_write1, 1, sizeof(atrshmlog_int32_t), fin);

      if (r != sizeof(atrshmlog_int32_t))
	{
	  printf("counter_write1 is not ok.\n");
	  fclose(fin);
	  continue;
	}

      r = fread(&head.counter_write1_discard, 1, sizeof(atrshmlog_int32_t), fin);

      if (r != sizeof(atrshmlog_int32_t))
	{
	  printf("counter_write1_discard is not ok.\n");
	  fclose(fin);
	  continue;
	}

      r = fread(&head.counter_write1_wait, 1, sizeof(atrshmlog_int32_t), fin);

      if (r != sizeof(atrshmlog_int32_t))
	{
	  printf("counter_write1_wait is not ok.\n");
	  fclose(fin);
	  continue;
	}

      r = fread(&head.counter_write1_adaptive, 1, sizeof(atrshmlog_int32_t), fin);

      if (r != sizeof(atrshmlog_int32_t))
	{
	  printf("counter_write1_adaptive is not ok.\n");
	  fclose(fin);
	  continue;
	}

      r = fread(&head.counter_write1_adaptive_fast, 1, sizeof(atrshmlog_int32_t), fin);

      if (r != sizeof(atrshmlog_int32_t))
	{
	  printf("counter_write1_adaptive_fast is not ok.\n");
	  fclose(fin);
	  continue;
	}

      r = fread(&head.counter_write1_adaptive_very_fast, 1, sizeof(atrshmlog_int32_t), fin);

      if (r != sizeof(atrshmlog_int32_t))
	{
	  printf("counter_write1_adaptive_very_fast is not ok.\n");
	  fclose(fin);
	  continue;
	}

      r = fread(&head.counter_write2, 1, sizeof(atrshmlog_int32_t), fin);

      if (r != sizeof(atrshmlog_int32_t))
	{
	  printf("counter_write2 is not ok.\n");
	  fclose(fin);
	  continue;
	}

      r = fread(&head.counter_write2_discard, 1, sizeof(atrshmlog_int32_t), fin);

      if (r != sizeof(atrshmlog_int32_t))
	{
	  printf("counter_write2_discard is not ok.\n");
	  fclose(fin);
	  continue;
	}

      r = fread(&head.counter_write2_wait, 1, sizeof(atrshmlog_int32_t), fin);

      if (r != sizeof(atrshmlog_int32_t))
	{
	  printf("counter_write2_wait is not ok.\n");
	  fclose(fin);
	  continue;
	}

      r = fread(&head.counter_write2_adaptive, 1, sizeof(atrshmlog_int32_t), fin);

      if (r != sizeof(atrshmlog_int32_t))
	{
	  printf("counter_write2_adaptive is not ok.\n");
	  fclose(fin);
	  continue;
	}

      r = fread(&head.counter_write2_adaptive_fast, 1, sizeof(atrshmlog_int32_t), fin);

      if (r != sizeof(atrshmlog_int32_t))
	{
	  printf("counter_write2_adaptive_fast is not ok.\n");
	  fclose(fin);
	  continue;
	}

      r = fread(&head.counter_write2_adaptive_very_fast, 1, sizeof(atrshmlog_int32_t), fin);

      if (r != sizeof(atrshmlog_int32_t))
	{
	  printf("counter_write2_adaptive_very_fast is not ok.\n");
	  fclose(fin);
	  continue;
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
	  fclose(fin);
	  continue;
	}
    
    
      if (head.tlen <  0 || head.tlen > ATRSHMLOGBUFFER_INFOSIZE)
	{
	  printf("tlen size wrong, is %d, not max %d\n", head.tlen, ATRSHMLOGBUFFER_INFOSIZE);
	  fclose(fin);
	  continue;
	}

      char buffer [ ATRSHMLOGBUFFER_INFOSIZE ];
    
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
	  fclose(fin);
	  continue;
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

      int dbret = atrshmlog_begin_db(dbsession);

      bcount++;
      
      if (operate(dbsession, buffer, head, sbyteorder, &hcount, &ucs2count, &ccount) != 0)
	{
	  fprintf(stderr, "error in operate, stop.\n");
	  ret = 2;
	  dbret = atrshmlog_rollback_db(dbsession);
	
	  goto ende;
	}

      dbret = atrshmlog_commit_db(dbsession);

    }
  
 ende:

  atrshmlog_cleanup_db(dbsession);

  t2 = ATRSHMLOG_GET_TSC_CALL();

  printf("clicktime needed %ld : %ld buffers : %d heads : %d ucs2string : %d cstring\n", (long)(t2 - t1), bcount, hcount, ucs2count, ccount); 
 
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
int operate(atrshmlog_dbsession_t *i_db,
	    char * const i_buffer,
	    atrshmlog_io_head_t head,
	    int sbyteorder,
	    int *io_insert_head,
	    int *io_insert_ucs2,
	    int *io_insert_cstring)

{
  int result = -1;

  const int horder_is_n_order = ATRSHMLOG_H_ORDER_IS_N_ORDER;

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
  
  if (!horder_is_n_order && atrshmlog_need_data_in_network_order_db())
    {
      atrshmlog_io_head_change_order(&head);
    }

  uint64_t head_id;

  int retdb = atrshmlog_get_head_seq_db(i_db, &head_id);

  if (retdb != 0)
    {
      return -1;
    }

  retdb = atrshmlog_insert_db_head(i_db, &head_id, &head);
	
  if (retdb != 0)
    {
      return -1;
    }

  ++*io_insert_head;
  
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

      char payloadbuffer [ ATRSHMLOGBUFFER_INFOSIZE + 1];
 
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
      
      
      if (ucs2string)
	{
	  if (!horder_is_n_order && atrshmlog_need_data_in_network_order_db())
	    {
	      atrshmlog_chunk_head_change_order(&c);
	    }
	  
	  c.payload = payloadbuffer;
	      
	  int retdb = atrshmlog_insert_db_ucs2string(i_db, head_id, &head, &c, (totallen - ATRSHMLOGCONTROLDATASIZE));

	  ++*io_insert_ucs2;
	  
	  if (retdb != 0)
	    {
	      return -1;
	    }
	}
      else
	{
	  char* lastpos = payloadbuffer;
	  char* nextcontrol = payloadbuffer;

	  if (!horder_is_n_order && atrshmlog_need_data_in_network_order_db())
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
	      
	      int retdb = atrshmlog_insert_db_cstring(i_db, head_id, &head, &c);

	      ++*io_insert_cstring;
	      
	      if (retdb != 0)
		{
		  return -1;
		}
	      
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



/* end of file */
