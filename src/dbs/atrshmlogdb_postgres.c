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

/** \file atrshmlogdb_postgres.c
 * \brief We deliver the database functions for postgres db
 *
 */


#include "../atrshmlog_internal.h"


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
atrshmlog_dbsession_t* atrshmlog_create_db(void);

/**
 * db specific function : cleanup
 */
void atrshmlog_cleanup_db(atrshmlog_dbsession_t* i_db);

/**
 * db specific function : begin
 */
int atrshmlog_begin_db(atrshmlog_dbsession_t* i_db);

/**
 * db specific function : commit
 */
int atrshmlog_commit_db(atrshmlog_dbsession_t* i_db);

/**
 * db specific function : rollback
 */
int atrshmlog_rollback_db(atrshmlog_dbsession_t* i_db);

/**
 * db specific function : get head seq
 */
int atrshmlog_get_head_seq_db(atrshmlog_dbsession_t* i_db,
			   uint64_t *o_head_id);

/**
 * db specific function : insert head
 */
int atrshmlog_insert_db_head(atrshmlog_dbsession_t* i_db,
			     uint64_t *io_head_id,
			  atrshmlog_io_head_t *i_head);

/**
 * db specific function : insert cstring
 */
int atrshmlog_insert_db_cstring(atrshmlog_dbsession_t* i_db,
			     uint64_t i_head_id,
			     atrshmlog_io_head_t *i_head,
			     atrshmlog_chunk_head_t* i_chunk);

/**
 * db specific function : insert ucs2 string
 */
int atrshmlog_insert_db_ucs2string(atrshmlog_dbsession_t* i_db,
				uint64_t i_head_id,
				atrshmlog_io_head_t *i_head,
				atrshmlog_chunk_head_t* i_chunk,
				int len);



/**
 * helper : deallocate prepared statements in pq
 */
int atrshmlog_free_prepared_db(atrshmlog_dbsession_t *i_db);

/*
 *--------------------------------------------------------
 */



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

static char *val[9] = { 0 } ; 

static char appname[63] = { 0 } ;

/**
 * db specific funtion : create
 */
atrshmlog_dbsession_t* atrshmlog_create_db(void)
{
  atrshmlog_dbsession_t* r =   (atrshmlog_dbsession_t*)calloc(1, sizeof(  atrshmlog_dbsession_t));

  if (r == NULL)
    return r;

  //  printf("create_db alloc\n");
  
  // we have ram ...

  r->status  = 0; // no init so far.

  r->keywords = (const char* const *)kw;

  if (val[0] == NULL || val[0][0] == 0)
    {
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

      if (val[0][0] == 0)
	return NULL;
    }
  
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
      atrshmlog_cleanup_db(r);
      return NULL;
    }

      
  ExecStatusType es = PQresultStatus(r->res);

  if (es != PGRES_COMMAND_OK)
    {
      printf("create_db commit bad %s\n", PQerrorMessage(r->conn));
      atrshmlog_cleanup_db(r);
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
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  es = PQresultStatus(r->res);

  if (es != PGRES_COMMAND_OK)
    {
      printf("create_db rollback bad %s\n", PQerrorMessage(r->conn));
      atrshmlog_cleanup_db(r);
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
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  es = PQresultStatus(r->res);

  if (es != PGRES_COMMAND_OK)
    {
      printf("create_db begin bad %s\n", PQerrorMessage(r->conn));
      atrshmlog_cleanup_db(r);
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
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  es = PQresultStatus(r->res);

  if (es != PGRES_COMMAND_OK)
    {
      printf("create_db get_head_seq bad %s\n", PQerrorMessage(r->conn));
      atrshmlog_cleanup_db(r);
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
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  es = PQresultStatus(r->res);

  if (es != PGRES_COMMAND_OK)
    {
      printf("create_db insert_db_head bad %s\n", PQerrorMessage(r->conn));
      atrshmlog_cleanup_db(r);
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
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  es = PQresultStatus(r->res);

  if (es != PGRES_COMMAND_OK)
    {
      printf("create_db insert_db_cstring bad %s\n", PQerrorMessage(r->conn));
      atrshmlog_cleanup_db(r);
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
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  es = PQresultStatus(r->res);

  if (es != PGRES_COMMAND_OK)
    {
      printf("create_db insert_db_ucs2string bad %s\n", PQerrorMessage(r->conn));
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  PQclear(r->res);

  
  // we have prepared statements
  r->status = 2;
  
  return r;
}

/**
 * db specific funtion : cleanup
 */
void atrshmlog_cleanup_db(atrshmlog_dbsession_t* i_db)
{
  if (i_db == NULL)
    return;

  if (i_db->status == 0)
    return;

  if (i_db->status == 2)
    {
      atrshmlog_free_prepared_db(i_db);
    }
  
  if (i_db->status == 3)
    {
      atrshmlog_rollback_db(i_db);

      atrshmlog_free_prepared_db(i_db);
    }
  
  PQfinish(i_db->conn);

  i_db->status = 0;

  free(i_db);
}

/**
 * db specific funtion : begin
 */
int atrshmlog_begin_db(atrshmlog_dbsession_t* i_db)
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
int atrshmlog_commit_db(atrshmlog_dbsession_t* i_db)
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
int atrshmlog_rollback_db(atrshmlog_dbsession_t* i_db)
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
int atrshmlog_insert_db_head(atrshmlog_dbsession_t* i_db,
			     uint64_t *io_head_id,
			     atrshmlog_io_head_t *i_head)
{
  if (i_db  == NULL)
    return -1;

  if (i_db->status != 3)
    return -2;

  char*pv[39];

  pv[0] = (char*)io_head_id,
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
int atrshmlog_insert_db_cstring(atrshmlog_dbsession_t* i_db,
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
int atrshmlog_insert_db_ucs2string(atrshmlog_dbsession_t* i_db,
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
int atrshmlog_get_head_seq_db(atrshmlog_dbsession_t* i_db,
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

/**
 * helper: free our session cached prepared statements
 */
int atrshmlog_free_prepared_db(atrshmlog_dbsession_t *i_db)
{
  i_db->res = PQexec(i_db->conn, "DEALLOCATE " "commit_db");
  PQclear(i_db->res);
  i_db->res = PQexec(i_db->conn, "DEALLOCATE " "rollback_db");
  PQclear(i_db->res);
  i_db->res = PQexec(i_db->conn, "DEALLOCATE " "begin_db");
  PQclear(i_db->res);
  i_db->res = PQexec(i_db->conn, "DEALLOCATE " "get_head_seq_db");
  PQclear(i_db->res);
  i_db->res = PQexec(i_db->conn, "DEALLOCATE " "insert_db_head");
  PQclear(i_db->res);
  i_db->res = PQexec(i_db->conn, "DEALLOCATE " "insert_db_cstring");
  PQclear(i_db->res);
  i_db->res = PQexec(i_db->conn, "DEALLOCATE " "insert_db_ucs2string");
  PQclear(i_db->res);

  return 0;
}

/* end of file */
