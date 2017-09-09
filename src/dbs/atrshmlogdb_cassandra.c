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

/** \file atrshmlogdb_cassandra.c
 * \brief We deliver the database functions for apache cassandra
 *
 */


#include "../atrshmlog_internal.h"


/*
 *--------------------------------------------------------
 * 
 * db specific part : incudes and definitions in dbsession
 */


// we are the cassandra db
#include <cassandra.h>

#include <stdlib.h>

#include <stdio.h>

// ... and its a lie. We are the datastax driver, no part
// of cassandra at all. They use simply the cassandra like it were
// a build in part - its a lie, they only make the driver...

/*
 *--------------------------------------------------------
 */

struct atrshmlog_dbsession_s {
  CassCluster* cluster;
  CassSession* session;
  CassFuture* connect_future;

  // we have a small state machine here
  // 0 : not connected
  // 1 : connect ready
  // 2 : statements prepared
  // 3 : in transaction
  int status;
  char**values;

  const CassPrepared* s[9]; // begin, commit, rollback, get seq, head, cstring, ucs2, last theSeq id , update theSeq
  uint64_t head_id;
  uint64_t rowid;
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


/**
 * helper : we deliver the flag : need data in network order is 1
 */
int atrshmlog_need_data_in_network_order_db(void)
{
  // this is 0 for cassandra
  return 0;
}

/*
 *--------------------------------------------------------
 */


/*********************************************************************/

/* 
 * cassandra db code is here 
 */


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

  // we have ram ...

  r->status  = 0; // no init so far.

  r->head_id = 0;

  r->rowid = 0;
  
  if (val[0] == NULL || val[0][0] == 0)
    {
      val[0] = (char*)ATRSHMLOG_GET_ENV("_CASSCONTACTPOINTS");
      val[1] = (char*)ATRSHMLOG_GET_ENV("_CASSPORT");
      val[2] = (char*)ATRSHMLOG_GET_ENV("_CASSKEYSPACE");
      val[3] = (char*)ATRSHMLOG_GET_ENV("_CASSUSER");
      val[4] = (char*)ATRSHMLOG_GET_ENV("_CASSPWD");
      val[5] = 0;


      if (val[0][0] == 0)
	{
	  return NULL;
	}
    }
  
  //   printf("create_db alloc %s\n", val[0]);
  // printf("create_db alloc %s\n", val[1]);
  // printf("create_db alloc %s\n", val[2]);
  // printf("create_db alloc %s\n", val[3]);
  // printf("create_db alloc %s\n", val[4]);
  // printf("create_db alloc %s\n", val[5]);
  // printf("create_db alloc %s\n", val[6]);
  // printf("create_db alloc %s\n", val[7]);
  
  // we get the low level handle
  r->cluster = cass_cluster_new();

  if (r->cluster == NULL)
    {
      free(r);
      return NULL;
    }

  r->session = cass_session_new();

  if (r->session == NULL)
    {
      cass_cluster_free(r->cluster);

      free(r);
      return NULL;
    }

  r->values = (char**)val;

  r->s[0] = 0;
  r->s[1] = 0;
  r->s[2] = 0;
  r->s[3] = 0;
  r->s[4] = 0;
  r->s[5] = 0;
  r->s[6] = 0;
  r->s[7] = 0;
  r->s[8] = 0;
  

  CassError rc = cass_cluster_set_contact_points(r->cluster, r->values[0]);

  if (rc != 0)
    {
      cass_session_free(r->session);
      cass_cluster_free(r->cluster);

      free(r);
      return NULL;
    }
  
  unsigned int port = (unsigned int)strtol(r->values[1], NULL, 10);

  rc = cass_cluster_set_port(r->cluster, port);


  if (rc != 0)
    {
      cass_session_free(r->session);
      cass_cluster_free(r->cluster);

      free(r);
      return NULL;
    }
  
  // we use palin communication inhouse ...
  cass_cluster_set_credentials(r->cluster, r->values[3], r->values[4]);

  // now we connect and switch to the keyspace 
  r->connect_future = cass_session_connect_keyspace(r->session, r->cluster, r->values[2]);


  rc = cass_future_error_code(r->connect_future);
  
  if (rc != 0)
    {
      printf("create_db connect %d.\n", (int)rc);
      cass_session_free(r->session);
      cass_cluster_free(r->cluster);

      free(r);
      return NULL;
    }
  
  // we are connected. from now on we need a close
  r->status = 1;

  // prepare the statements

  const char * insert_db_head = "insert into atrshmlog_head("
    "head_id,"
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
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?)";

  CassFuture* prepare_future
    = cass_session_prepare(r->session,
			   insert_db_head);

  /* Wait for the statement to prepare and get the result */
  rc = cass_future_error_code(prepare_future);

  if (rc != 0)
    {
      cass_future_free(prepare_future);
      printf("create_db prepare db head %d.\n", (int)rc);
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  r->s[4] = cass_future_get_prepared(prepare_future);

  cass_future_free(prepare_future);
  
  if (r->s[4] == NULL)
    {
      printf("create_db db head null.\n");
      atrshmlog_cleanup_db(r);
      return NULL;
    }
  

  const char * insert_db_cstring ="insert into atrshmlog_log(head_id,"
    "rowid,"
    "pid,"
    "tid,"
    "startclick,"
    "endclick,"
    "deltaclick,"
    "startreal,"
    "endreal,"
    "deltareal,"
    "logevent,"
    "logflag,"
    "userflag,"
    "payload) "
    " values ("
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?)";
    
  prepare_future
    = cass_session_prepare(r->session,
			   insert_db_cstring);

  /* Wait for the statement to prepare and get the result */
  rc = cass_future_error_code(prepare_future);

  if (rc != 0)
    {
      cass_future_free(prepare_future);
      printf("create_db prepare insert cstring %d.\n", (int)rc);
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  r->s[5] = cass_future_get_prepared(prepare_future);

  cass_future_free(prepare_future);
  
  if (r->s[5] == NULL)
    {
      printf("create_db inset cstring null.\n");
      atrshmlog_cleanup_db(r);
      return NULL;
    }
  
  const char * insert_db_ucs2string = "insert into atrshmlog_log_ucs2(head_id,"
    "rowid,"
    "pid,"
    "tid,"
    "startclick,"
    "endclick,"
    "deltaclick,"
    "startreal,"
    "endreal,"
    "deltareal,"
    "logevent,"
    "logflag,"
    "userflag,"
    "payload) "
    " values ("
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?,"
    "?)";

  prepare_future
    = cass_session_prepare(r->session,
			   insert_db_ucs2string);

  /* Wait for the statement to prepare and get the result */
  rc = cass_future_error_code(prepare_future);

  if (rc != 0)
    {
      cass_future_free(prepare_future);
      printf("create_db prepare insert ucs2 %d.\n", (int)rc);
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  r->s[6] = cass_future_get_prepared(prepare_future);

  cass_future_free(prepare_future);
  
  if (r->s[6] == NULL)
    {
      printf("create_db insert ucs2 null.\n");
      atrshmlog_cleanup_db(r);
      return NULL;
    }
  

  const char* select_last_id = "SELECT head_id FROM theSeq WHERE self = 1";
  
  prepare_future
    = cass_session_prepare(r->session,
			   select_last_id);

  /* Wait for the statement to prepare and get the result */
  rc = cass_future_error_code(prepare_future);

  if (rc != 0)
    {
      cass_future_free(prepare_future);
      printf("create_db prepare select last id %d.\n", (int)rc);
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  r->s[7] = cass_future_get_prepared(prepare_future);

  cass_future_free(prepare_future);
  
  if (r->s[7] == NULL)
    {
      printf("create_db select last id null.\n");
      atrshmlog_cleanup_db(r);
      return NULL;
    }
  

  const char* update_last_id = "UPDATE theSeq SET head_id = ? "
    "WHERE self = 1 IF head_id = ?";
  
  prepare_future
    = cass_session_prepare(r->session,
			   update_last_id);

  /* Wait for the statement to prepare and get the result */
  rc = cass_future_error_code(prepare_future);

  if (rc != 0)
    {
      cass_future_free(prepare_future);
      printf("create_db prepare update last id %d.\n", (int)rc);
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  r->s[8] = cass_future_get_prepared(prepare_future);

  cass_future_free(prepare_future);
  
  if (r->s[8] == NULL)
    {
      printf("create_db update last id null.\n");
      atrshmlog_cleanup_db(r);
      return NULL;
    }
  
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

  cass_future_free(i_db->connect_future);
  
  cass_session_free(i_db->session);
  
  cass_cluster_free(i_db->cluster);

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


  // dummy on cassandra in prepared statements.
  
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

  // dummy on cassandra

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

  // dummy on cassandra

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


  CassStatement* statement = cass_prepared_bind(i_db->s[4]);

  CassError rc = cass_statement_bind_int64(statement, 0, *io_head_id);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int32(statement, 1, i_head->version);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int32(statement, 2, i_head->tlen);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int64(statement, 3, i_head->pid);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int64(statement, 4, i_head->tid);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int32(statement, 5, i_head->buffernumber);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int32(statement, 6, i_head->filenumber);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int64(statement, 7, i_head->inittime.tv_sec);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int32(statement, 8, i_head->inittime.tv_nsec);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int64(statement, 9, i_head->inittsc_before);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int64(statement, 10, i_head->inittsc_after);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int64(statement, 11, i_head->lasttime.tv_sec);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int32(statement, 12, i_head->lasttime.tv_nsec);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int64(statement, 13, i_head->lasttsc_before);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int64(statement, 14, i_head->lasttsc_after);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int64(statement, 15, i_head->difftimetransfer);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int64(statement, 16, i_head->starttransfer);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int64(statement, 17, i_head->acquiretime);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int32(statement, 18, i_head->id);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int32(statement, 19, i_head->number_dispatched);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int32(statement, 20, i_head->counter_write0);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int32(statement, 21, i_head->counter_write0_discard);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int32(statement, 22, i_head->counter_write0_wait);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int32(statement, 23, i_head->counter_write0_adaptive);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int32(statement, 24, i_head->counter_write0_adaptive_fast);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int32(statement, 25, i_head->counter_write0_adaptive_very_fast);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int32(statement, 26, i_head->counter_write1);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int32(statement, 27, i_head->counter_write1_discard);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int32(statement, 28, i_head->counter_write1_wait);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int32(statement, 29, i_head->counter_write1_adaptive);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int32(statement, 30, i_head->counter_write1_adaptive_fast);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int32(statement, 31, i_head->counter_write1_adaptive_very_fast);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int32(statement, 32, i_head->counter_write2);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int32(statement, 33, i_head->counter_write2_discard);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int32(statement, 34, i_head->counter_write2_wait);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int32(statement, 35, i_head->counter_write2_adaptive);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int32(statement, 36, i_head->counter_write2_adaptive_fast);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int32(statement, 37, i_head->counter_write2_adaptive_very_fast);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  CassFuture* future = cass_session_execute(i_db->session, statement);

  cass_future_wait(future);

  rc = cass_future_error_code(future);
  
  if (rc != 0)
    {
      cass_future_free(future);

      cass_statement_free(statement);
      
      printf("insert_db_head exec bad %d\n", (int)rc);
      return -3;
    }

  cass_future_free(future);

  cass_statement_free(statement);
  
  // we reset th row id, it has to start now again.
  i_db->rowid = 0;
  
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


  CassStatement* statement = cass_prepared_bind(i_db->s[5]);

  CassError rc = cass_statement_bind_int64(statement, 0, i_head_id);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_cstring exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int64(statement, 1, i_db->rowid);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_cstring exec bad %d\n", (int)rc);
      return -3;
    }

  i_db->rowid++;
  
  rc = cass_statement_bind_int64(statement, 2, i_head->pid);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_cstring exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int64(statement, 3, i_head->tid);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_cstring exec bad %d\n", (int)rc);
      return -3;
    }


  rc = cass_statement_bind_int64(statement, 4, i_chunk->starttime);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_cstring exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int64(statement, 5, i_chunk->endtime);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_cstring exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int64(statement, 6, i_chunk->deltatime);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_cstring exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int64(statement, 7, i_chunk->startreal);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_cstring exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int64(statement, 8, i_chunk->endreal);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_cstring exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int64(statement, 9, i_chunk->deltareal);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_cstring exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int32(statement, 10, i_chunk->eventnumber);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_cstring exec bad %d\n", (int)rc);
      return -3;
    }


  char f[2] ;
  f[0] = i_chunk->eventflag;
  f[1] = 0;

  rc = cass_statement_bind_string(statement, 11, f);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_cstring exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int32(statement, 12, i_chunk->userflag);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_cstring exec bad %d\n", (int)rc);
      return -3;
    }

  
  rc = cass_statement_bind_string(statement, 13, i_chunk->payload);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_cstring exec bad %d\n", (int)rc);
      return -3;
    }

  CassFuture* future = cass_session_execute(i_db->session, statement);

  cass_future_wait(future);

  rc = cass_future_error_code(future);
  
  if (rc != 0)
    {
      cass_future_free(future);

      cass_statement_free(statement);
      
      printf("insert_db_cstring exec bad %d\n", (int)rc);
      return -3;
    }

  cass_future_free(future);

  cass_statement_free(statement);
  

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


  CassStatement* statement = cass_prepared_bind(i_db->s[6]);

  CassError rc = cass_statement_bind_int64(statement, 0, i_head_id);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_cstring exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int64(statement, 1, i_db->rowid);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_cstring exec bad %d\n", (int)rc);
      return -3;
    }

  i_db->rowid++;
  
  rc = cass_statement_bind_int64(statement, 2, i_head->pid);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_cstring exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int64(statement, 3, i_head->tid);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_cstring exec bad %d\n", (int)rc);
      return -3;
    }


  rc = cass_statement_bind_int64(statement, 4, i_chunk->starttime);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_cstring exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int64(statement, 5, i_chunk->endtime);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_cstring exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int64(statement, 6, i_chunk->deltatime);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_cstring exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int64(statement, 7, i_chunk->startreal);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_cstring exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int64(statement, 8, i_chunk->endreal);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_cstring exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int64(statement, 9, i_chunk->deltareal);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_cstring exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int32(statement, 10, i_chunk->eventnumber);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_cstring exec bad %d\n", (int)rc);
      return -3;
    }


  char f[2] ;
  f[0] = i_chunk->eventflag;
  f[1] = 0;

  rc = cass_statement_bind_string(statement, 11, f);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_cstring exec bad %d\n", (int)rc);
      return -3;
    }

  rc = cass_statement_bind_int32(statement, 12, i_chunk->userflag);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_cstring exec bad %d\n", (int)rc);
      return -3;
    }

  
  rc = cass_statement_bind_bytes(statement, 13, i_chunk->payload, len);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("insert_db_cstring exec bad %d\n", (int)rc);
      return -3;
    }

  CassFuture* future = cass_session_execute(i_db->session, statement);

  cass_future_wait(future);

  rc = cass_future_error_code(future);
  
  if (rc != 0)
    {
      cass_future_free(future);

      cass_statement_free(statement);
      
      printf("insert_db_cstring exec bad %d\n", (int)rc);
      return -3;
    }

  cass_future_free(future);

  cass_statement_free(statement);
  
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

  *o_head_id = 0;
  
  // all the dirty work. there is no concept of acid here, so we
  // have to use a helper - which i dont want to do
  // or resort to the lightweight transaction
  // this means i have to select the actual,
  // then update
  // then check if its applied
  // then do all again if not applied,
  // use the selected val if applied... uff.

 do_it_again_sam:
  ;
  
  int64_t n = 0;
  
  // ok first we read the actual value, it will be ours...
  CassStatement* statement = cass_prepared_bind(i_db->s[7]);

  CassFuture* future = cass_session_execute(i_db->session, statement);

  cass_future_wait(future);

  CassError rc = cass_future_error_code(future);
  
  if (rc != 0)
    {
      cass_future_free(future);

      cass_statement_free(statement);

      printf("get_head_seq_db exec bad %d\n", (int)rc);
      return -3;
    }

  const CassResult* result = cass_future_get_result(future);

  CassIterator* iterator = cass_iterator_from_result(result);

  if (cass_iterator_next(iterator)) {
    const CassRow* row = cass_iterator_get_row(iterator);

    
    cass_value_get_int64(cass_row_get_column(row, 0), &n);
  }

  cass_result_free(result);
  cass_iterator_free(iterator);
  cass_future_free(future);
  cass_statement_free(statement);

  // so far ... we have done a select, now we have to update it
  // but with an if clause and the condition its still the old value
  statement = cass_prepared_bind(i_db->s[8]);

  rc = cass_statement_bind_int64(statement, 0, n + 1);

  rc = cass_statement_bind_int64(statement, 1, n);
 
  if (rc != 0)
    {
      cass_statement_free(statement);
      printf("get_head_seq_db exec bad %d\n", (int)rc);
      return -3;
    }

  future = cass_session_execute(i_db->session, statement);

  cass_future_wait(future);

  rc = cass_future_error_code(future);
  
  if (rc != 0)
    {
      cass_future_free(future);

      cass_statement_free(statement);

      printf("get_head_seq_db exec bad %d\n", (int)rc);
      return -3;
    }

  result = cass_future_get_result(future);
  
  const CassValue* value = cass_row_get_column(cass_result_first_row(result), 0);

  cass_bool_t applied;

  cass_value_get_bool(value, &applied);

  if (applied != cass_true )
    {
      // hm. someone was faster than us ... so the value was already changed...
      goto do_it_again_sam;
    }
  
  *o_head_id = n;

  return 0;
}

/**
 * helper: free our session cached prepared statements
 */
int atrshmlog_free_prepared_db(atrshmlog_dbsession_t *i_db)
{
  for (int i = 0; i < 9; i++)
    {
      if (i_db->s[i] != 0)
	cass_prepared_free(i_db->s[i]);
    }
  
  return 0;
}

/* end of file */
