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

/** \file atrshmlogdb_mariadb.c
 * \brief We deliver the database functions for maria db
 *
 */


#include "../atrshmlog_internal.h"


/*
 *--------------------------------------------------------
 * 
 * db specific part : incudes and definitions in dbsession
 */


// we are the maria db  
#include <mysql/mysql.h>

/*
 *--------------------------------------------------------
 */

struct atrshmlog_dbsession_s {
  MYSQL *conn;
  // we have a small state machine here
  // 0 : not connected
  // 1 : connect ready
  // 2 : statements prepared
  // 3 : in transaction
  int status;
  char**values;

  MYSQL_STMT *s[8]; // begin, commit,r ollback, get seq, head, cstring, ucs2, last insert id
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
  // this is 0 for mariadb
  return 0;
}

/*
 *--------------------------------------------------------
 */


/*********************************************************************/

/* 
 * maria db code is here 
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

  //  printf("create_db alloc\n");
  
  // we have ram ...

  r->status  = 0; // no init so far.

  if (val[0] == NULL || val[0][0] == 0)
    {
      val[0] = (char*)ATRSHMLOG_GET_ENV("_MARIAHOST");
      val[1] = (char*)ATRSHMLOG_GET_ENV("_MARIAPORT");
      val[2] = (char*)ATRSHMLOG_GET_ENV("_MARIADBNAME");
      val[3] = (char*)ATRSHMLOG_GET_ENV("_MARIAUSER");
      val[4] = (char*)ATRSHMLOG_GET_ENV("_MARIAPWD");
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
  r->conn = mysql_init(NULL);

  if (r->conn == NULL)
    {
      free(r);
      return NULL;
    }

  int retdb = mysql_thread_init();

  if (retdb != 0)
    {
      mysql_close(r->conn);
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
  
  unsigned int port = (unsigned int)strtol(r->values[1], NULL, 10);
  
  MYSQL *m = mysql_real_connect(r->conn,
				r->values[0],
				r->values[3],
				r->values[4],
				r->values[2],
				port,
				NULL,
				0L);
  

  if (m == NULL)
    {
      mysql_close(r->conn);
      
      free(r);
      printf("create_db no connect\n");
      return NULL;
    }

  // we are connected. from now on we need a close
  r->status = 1;

  const int is_autocommit_off = 0;
  
  retdb = mysql_autocommit(r->conn, is_autocommit_off);
  
  // prepare the statements

  // begin exists ... but we dont need it. we set autocommit off....

  // commit as function

  // rollback as function

  // sequence does not exist, so we use a insert with autoincrement id

  r->s[4] = mysql_stmt_init(r->conn);

  if (r->s[4] == NULL)
    {
      printf("create_db result null.\n");
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  const char * insert_db_head = "insert into atrshmlog_head("
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
    "?)";

  retdb = mysql_stmt_prepare(r->s[4],  insert_db_head, strlen(insert_db_head));

  if (retdb != 0)
    {
      printf("create_db insert_db_head bad : %s :\n", mysql_stmt_error(r->s[4]));
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  r->s[5] = mysql_stmt_init(r->conn);

  if (r->s[5] == NULL)
    {
      printf("create_db result null.\n");
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  const char * insert_db_cstring ="insert into atrshmlog_log(head_id,"
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
    "?)";
    
  retdb = mysql_stmt_prepare(r->s[5],  insert_db_cstring, strlen(insert_db_cstring));

  if (retdb != 0)
    {
      printf("create_db insert_db_cstring bad : %s :\n", mysql_stmt_error(r->s[5]));
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  r->s[6] = mysql_stmt_init(r->conn);

  if (r->s[6] == NULL)
    {
      printf("create_db result null.\n");
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  const char * insert_db_ucs2string = "insert into atrshmlog_log_ucs2(head_id,"
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
    "?)";

  retdb = mysql_stmt_prepare(r->s[6],  insert_db_ucs2string, strlen(insert_db_ucs2string));

  if (retdb != 0)
    {
      printf("create_db insert_db_ucs2string bad : %s :\n", mysql_stmt_error(r->s[6]));
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  r->s[7] = mysql_stmt_init(r->conn);

  if (r->s[7] == NULL)
    {
      printf("create_db result null.\n");
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  const char* select_last_id = "SELECT LAST_INSERT_ID()";
  
  retdb = mysql_stmt_prepare(r->s[7],  select_last_id, strlen(select_last_id));

  if (retdb != 0)
    {
      printf("create_db select last id bad : %s :\n", mysql_stmt_error(r->s[7]));
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

  mysql_close(i_db->conn);
  
  mysql_thread_end();
  
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


  // dummy on mariadb in prepared statements.
  // we have also shut off autocommit in connect...
  
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

  int retdb = mysql_commit(i_db->conn);

  if (retdb != 0)
    {
      printf("commit_db bad %s\n", mysql_error(i_db->conn));
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

  int retdb = mysql_rollback(i_db->conn);

  if (retdb != 0)
    {
      printf("rollback_db bad %s\n", mysql_error(i_db->conn));
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

  unsigned long l;
  my_bool err;
  my_bool is_null;

  MYSQL_BIND pv[39];

  memset(pv, 0 , sizeof(pv));

  //  pv[0].buffer = NULL;
  // pv[0].buffer_length = 0;
  //pv[0].buffer_type = MYSQL_TYPE_LONGLONG;
  //pv[0].is_unsigned = 1;

  pv[1].buffer = &i_head->version;
  pv[1].buffer_length = 4;
  pv[1].buffer_type = MYSQL_TYPE_LONG;
  pv[1].is_unsigned = 0;

  pv[2].buffer = &i_head->tlen;
  pv[2].buffer_length = 4;
  pv[2].buffer_type = MYSQL_TYPE_LONG;
  pv[2].is_unsigned = 0;

  pv[3].buffer = &i_head->pid;
  pv[3].buffer_length = 8;
  pv[3].buffer_type = MYSQL_TYPE_LONGLONG;
  pv[3].is_unsigned = 1;

  pv[4].buffer = &i_head->tid;
  pv[4].buffer_length = 8;
  pv[4].buffer_type = MYSQL_TYPE_LONGLONG;
  pv[4].is_unsigned = 1;

  pv[5].buffer = &i_head->buffernumber;
  pv[5].buffer_length = 4;
  pv[5].buffer_type = MYSQL_TYPE_LONG;
  pv[5].is_unsigned = 0;

  pv[6].buffer = &i_head->filenumber;
  pv[6].buffer_length = 4;
  pv[6].buffer_type = MYSQL_TYPE_LONG;
  pv[6].is_unsigned = 0;

  pv[7].buffer = &i_head->inittime.tv_sec;
  pv[7].buffer_length = 8;
  pv[7].buffer_type = MYSQL_TYPE_LONGLONG;
  pv[7].is_unsigned = 1;

  pv[8].buffer = &i_head->inittime.tv_nsec;
  pv[8].buffer_length = 4;
  pv[8].buffer_type = MYSQL_TYPE_LONG;
  pv[8].is_unsigned = 0;

  pv[9].buffer = &i_head->inittsc_before;
  pv[9].buffer_length = 8;
  pv[9].buffer_type = MYSQL_TYPE_LONGLONG;
  pv[9].is_unsigned = 1;

  pv[10].buffer = &i_head->inittsc_after;
  pv[10].buffer_length = 8;
  pv[10].buffer_type = MYSQL_TYPE_LONGLONG;
  pv[10].is_unsigned = 1;

  pv[11].buffer = &i_head->lasttime.tv_sec;
  pv[11].buffer_length = 8;
  pv[11].buffer_type = MYSQL_TYPE_LONGLONG;
  pv[11].is_unsigned = 1;

  pv[12].buffer = &i_head->lasttime.tv_nsec;
  pv[12].buffer_length = 4;
  pv[12].buffer_type = MYSQL_TYPE_LONG;
  pv[12].is_unsigned = 0;

  pv[13].buffer = &i_head->lasttsc_before;
   pv[13].buffer_length = 8;
  pv[13].buffer_type = MYSQL_TYPE_LONGLONG;
  pv[13].is_unsigned = 1;

  pv[14].buffer = &i_head->lasttsc_after;
  pv[14].buffer_length = 8;
  pv[14].buffer_type = MYSQL_TYPE_LONGLONG;
  pv[14].is_unsigned = 1;

  pv[15].buffer = &i_head->difftimetransfer;
  pv[15].buffer_length = 8;
  pv[15].buffer_type = MYSQL_TYPE_LONGLONG;
  pv[15].is_unsigned = 1;

  pv[16].buffer = &i_head->starttransfer;
  pv[16].buffer_length = 8;
  pv[16].buffer_type = MYSQL_TYPE_LONGLONG;
  pv[16].is_unsigned = 1;

  pv[17].buffer = &i_head->acquiretime;
  pv[17].buffer_length = 8;
  pv[17].buffer_type = MYSQL_TYPE_LONGLONG;
  pv[17].is_unsigned = 1;

  pv[18].buffer = &i_head->id;
  pv[18].buffer_length = 4;
  pv[18].buffer_type = MYSQL_TYPE_LONG;
  pv[18].is_unsigned = 0;

  pv[19].buffer = &i_head->number_dispatched;
  pv[19].buffer_length = 4;
  pv[19].buffer_type = MYSQL_TYPE_LONG;
  pv[19].is_unsigned = 0;

  pv[20].buffer = &i_head->counter_write0;
  pv[20].buffer_length = 4;
  pv[20].buffer_type = MYSQL_TYPE_LONG;
  pv[20].is_unsigned = 0;

  pv[21].buffer = &i_head->counter_write0_discard;
  pv[21].buffer_length = 4;
  pv[21].buffer_type = MYSQL_TYPE_LONG;
  pv[21].is_unsigned = 0;

  pv[22].buffer = &i_head->counter_write0_wait;
  pv[22].buffer_length = 4;
  pv[22].buffer_type = MYSQL_TYPE_LONG;
  pv[22].is_unsigned = 0;

  pv[23].buffer = &i_head->counter_write0_adaptive;
  pv[23].buffer_length = 4;
  pv[23].buffer_type = MYSQL_TYPE_LONG;
  pv[23].is_unsigned = 0;

  pv[24].buffer = &i_head->counter_write0_adaptive_fast;
  pv[24].buffer_length = 4;
  pv[24].buffer_type = MYSQL_TYPE_LONG;
  pv[24].is_unsigned = 0;

  pv[25].buffer = &i_head->counter_write0_adaptive_very_fast;
  pv[25].buffer_length = 4;
  pv[25].buffer_type = MYSQL_TYPE_LONG;
  pv[25].is_unsigned = 0;


  pv[26].buffer = &i_head->counter_write1;
  pv[26].buffer_length = 4;
  pv[26].buffer_type = MYSQL_TYPE_LONG;
  pv[26].is_unsigned = 0;

  pv[27].buffer = &i_head->counter_write1_discard;
  pv[27].buffer_length = 4;
  pv[27].buffer_type = MYSQL_TYPE_LONG;
  pv[27].is_unsigned = 0;

  pv[28].buffer = &i_head->counter_write1_wait;
  pv[28].buffer_length = 4;
  pv[28].buffer_type = MYSQL_TYPE_LONG;
  pv[28].is_unsigned = 0;

  pv[29].buffer = &i_head->counter_write1_adaptive;
  pv[29].buffer_length = 4;
  pv[29].buffer_type = MYSQL_TYPE_LONG;
  pv[29].is_unsigned = 0;

  pv[30].buffer = &i_head->counter_write1_adaptive_fast;
  pv[30].buffer_length = 4;
  pv[30].buffer_type = MYSQL_TYPE_LONG;
  pv[30].is_unsigned = 0;

  pv[31].buffer = &i_head->counter_write1_adaptive_very_fast;
  pv[31].buffer_length = 4;
  pv[31].buffer_type = MYSQL_TYPE_LONG;
  pv[31].is_unsigned = 0;


  pv[32].buffer = &i_head->counter_write2;
  pv[32].buffer_length = 4;
  pv[32].buffer_type = MYSQL_TYPE_LONG;
  pv[32].is_unsigned = 0;

  pv[33].buffer = &i_head->counter_write2_discard;
  pv[33].buffer_length = 4;
  pv[33].buffer_type = MYSQL_TYPE_LONG;
  pv[33].is_unsigned = 0;

  pv[34].buffer = &i_head->counter_write2_wait;
  pv[34].buffer_length = 4;
  pv[34].buffer_type = MYSQL_TYPE_LONG;
  pv[34].is_unsigned = 0;

  pv[35].buffer = &i_head->counter_write2_adaptive;
  pv[35].buffer_length = 4;
  pv[35].buffer_type = MYSQL_TYPE_LONG;
  pv[35].is_unsigned = 0;

  pv[36].buffer = &i_head->counter_write2_adaptive_fast;
  pv[36].buffer_length = 4;
  pv[36].buffer_type = MYSQL_TYPE_LONG;
  pv[36].is_unsigned = 0;

  pv[37].buffer = &i_head->counter_write2_adaptive_very_fast;
  pv[37].buffer_length = 4;
  pv[37].buffer_type = MYSQL_TYPE_LONG;
  pv[37].is_unsigned = 0;

  int retdb = mysql_stmt_bind_param(i_db->s[4], &pv[1]);

  if (retdb != 0)
    {
      printf("insert_db_head bind bad %s\n", mysql_error(i_db->conn));
      return -3;
    }

  retdb = mysql_stmt_execute(i_db->s[4]);
  
  if (retdb != 0)
    {
      printf("insert_db_head exec bad %s\n", mysql_error(i_db->conn));
      return -3;
    }

  retdb = mysql_stmt_reset(i_db->s[4]);
  
  if (retdb != 0)
    {
      printf("insert_db_head reset bad %s\n", mysql_error(i_db->conn));
      return -3;
    }

  // so far... now we have to get the last id

  MYSQL_BIND result[1];

  memset(result, 0, sizeof(result));

  
  result[0].buffer = io_head_id;
  result[0].buffer_length = 8;
  result[0].buffer_type = MYSQL_TYPE_LONGLONG;
  result[0].is_unsigned = 1;
  result[0].length = &l;
  result[0].error = &err;
  result[0].is_null = &is_null;

  retdb = mysql_stmt_bind_result(i_db->s[7], result);
  
  if (retdb != 0)
    {
      printf("insert_db_head bind 2 bad %s\n", mysql_error(i_db->conn));
      return -3;
    }

  retdb = mysql_stmt_execute(i_db->s[7]);
  
  if (retdb != 0)
    {
      printf("insert_db_head exec 2 bad %s\n", mysql_error(i_db->conn));
      return -3;
    }

  retdb = mysql_stmt_store_result(i_db->s[7]);
  
  if (retdb != 0)
    {
      printf("insert_db_head store 2 bad %s\n", mysql_error(i_db->conn));
      return -3;
    }

  retdb = mysql_stmt_fetch(i_db->s[7]);

  if (retdb != 0)
    {
      printf("insert_db_head fetch 2 bad %s\n", mysql_error(i_db->conn));
      return -3;
    }

  retdb = mysql_stmt_free_result(i_db->s[7]);
  
  if (retdb != 0)
    {
      printf("insert_db_head free 2 bad %s\n", mysql_error(i_db->conn));
      return -3;
    }

  retdb = mysql_stmt_reset(i_db->s[7]);

  if (retdb != 0)
    {
      printf("insert_db_head reset 2 bad %s\n", mysql_error(i_db->conn));
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

  MYSQL_BIND pv[15];

  memset(pv, 0 , sizeof(pv));

  pv[0].buffer = &i_head_id;
  pv[0].buffer_length = 8;
  pv[0].buffer_type = MYSQL_TYPE_LONGLONG;
  pv[0].is_unsigned = 1;

  pv[1].buffer = &i_head->pid;
  pv[1].buffer_length = 8;
  pv[1].buffer_type = MYSQL_TYPE_LONGLONG;
  pv[1].is_unsigned = 1;

  pv[2].buffer = &i_head->tid;
  pv[2].buffer_length = 8;
  pv[2].buffer_type = MYSQL_TYPE_LONGLONG;
  pv[2].is_unsigned = 1;

  pv[3].buffer = &i_chunk->starttime;
  pv[3].buffer_length = 8;
  pv[3].buffer_type = MYSQL_TYPE_LONGLONG;
  pv[3].is_unsigned = 1;

  pv[4].buffer = &i_chunk->endtime;
  pv[4].buffer_length = 8;
  pv[4].buffer_type = MYSQL_TYPE_LONGLONG;
  pv[4].is_unsigned = 1;

  pv[5].buffer = &i_chunk->deltatime;
  pv[5].buffer_length = 8;
  pv[5].buffer_type = MYSQL_TYPE_LONGLONG;
  pv[5].is_unsigned = 1;

  pv[6].buffer = &i_chunk->startreal;
  pv[6].buffer_length = 8;
  pv[6].buffer_type = MYSQL_TYPE_LONGLONG;
  pv[6].is_unsigned = 1;

  pv[7].buffer = &i_chunk->endreal;
  pv[7].buffer_length = 8;
  pv[7].buffer_type = MYSQL_TYPE_LONGLONG;
  pv[7].is_unsigned = 1;

  pv[8].buffer = &i_chunk->deltareal;
  pv[8].buffer_length = 8;
  pv[8].buffer_type = MYSQL_TYPE_LONGLONG;
  pv[8].is_unsigned = 1;

  pv[9].buffer = &i_chunk->eventnumber;
  pv[9].buffer_length = 4;
  pv[9].buffer_type = MYSQL_TYPE_LONG;
  pv[9].is_unsigned = 0;

  pv[10].buffer = &i_chunk->eventflag;
  pv[10].buffer_length = 1;
  pv[10].buffer_type = MYSQL_TYPE_VARCHAR;
  pv[10].is_unsigned = 0;

  pv[11].buffer = &i_chunk->userflag;
  pv[11].buffer_length = 4;
  pv[11].buffer_type = MYSQL_TYPE_LONG;
  pv[11].is_unsigned = 0;

  pv[12].buffer = i_chunk->payload;
  pv[12].buffer_length = strlen(i_chunk->payload);
  pv[12].buffer_type = MYSQL_TYPE_STRING;
  pv[12].is_unsigned = 0;

  int retdb = mysql_stmt_bind_param(i_db->s[5], pv);

  if (retdb != 0)
    {
      printf("insert_db_cstring bind bad %s\n", mysql_error(i_db->conn));
      return -3;
    }

  retdb = mysql_stmt_execute(i_db->s[5]);
  
  if (retdb != 0)
    {
      printf("insert_db_cstring exec bad %s\n", mysql_error(i_db->conn));
      return -3;
    }

  retdb = mysql_stmt_reset(i_db->s[5]);
  
  if (retdb != 0)
    {
      printf("insert_db_cstring reset bad %s\n", mysql_error(i_db->conn));
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

  MYSQL_BIND pv[15];

  memset(pv, 0 , sizeof(pv));

  pv[0].buffer = &i_head_id;
  pv[0].buffer_length = 8;
  pv[0].buffer_type = MYSQL_TYPE_LONGLONG;
  pv[0].is_unsigned = 1;

  pv[1].buffer = &i_head->pid;
  pv[1].buffer_length = 8;
  pv[1].buffer_type = MYSQL_TYPE_LONGLONG;
  pv[1].is_unsigned = 1;

  pv[2].buffer = &i_head->tid;
  pv[2].buffer_length = 8;
  pv[2].buffer_type = MYSQL_TYPE_LONGLONG;
  pv[2].is_unsigned = 1;

  pv[3].buffer = &i_chunk->starttime;
  pv[3].buffer_length = 8;
  pv[3].buffer_type = MYSQL_TYPE_LONGLONG;
  pv[3].is_unsigned = 1;

  pv[4].buffer = &i_chunk->endtime;
  pv[4].buffer_length = 8;
  pv[4].buffer_type = MYSQL_TYPE_LONGLONG;
  pv[4].is_unsigned = 1;

  pv[5].buffer = &i_chunk->deltatime;
  pv[5].buffer_length = 8;
  pv[5].buffer_type = MYSQL_TYPE_LONGLONG;
  pv[5].is_unsigned = 1;

  pv[6].buffer = &i_chunk->startreal;
  pv[6].buffer_length = 8;
  pv[6].buffer_type = MYSQL_TYPE_LONGLONG;
  pv[6].is_unsigned = 1;

  pv[7].buffer = &i_chunk->endreal;
  pv[7].buffer_length = 8;
  pv[7].buffer_type = MYSQL_TYPE_LONGLONG;
  pv[7].is_unsigned = 1;

  pv[8].buffer = &i_chunk->deltareal;
  pv[8].buffer_length = 8;
  pv[8].buffer_type = MYSQL_TYPE_LONGLONG;
  pv[8].is_unsigned = 1;

  pv[9].buffer = &i_chunk->eventnumber;
  pv[9].buffer_length = 4;
  pv[9].buffer_type = MYSQL_TYPE_LONG;
  pv[9].is_unsigned = 0;

  pv[10].buffer = &i_chunk->eventflag;
  pv[10].buffer_length = 1;
  pv[10].buffer_type = MYSQL_TYPE_VARCHAR;
  pv[10].is_unsigned = 0;

  pv[11].buffer = &i_chunk->userflag;
  pv[11].buffer_length = 4;
  pv[11].buffer_type = MYSQL_TYPE_LONG;
  pv[11].is_unsigned = 0;

  pv[12].buffer = &i_chunk->payload;
  pv[12].buffer_length = len;
  pv[12].buffer_type = MYSQL_TYPE_MEDIUM_BLOB;
  pv[12].is_unsigned = 0;

  int retdb = mysql_stmt_bind_param(i_db->s[6], pv);

  if (retdb != 0)
    {
      printf("insert_db_ucs2string bind bad %s\n", mysql_error(i_db->conn));
      return -3;
    }

  retdb = mysql_stmt_execute(i_db->s[6]);
  
  if (retdb != 0)
    {
      printf("insert_db_ucs2string exec bad %s\n", mysql_error(i_db->conn));
      return -3;
    }

  retdb = mysql_stmt_reset(i_db->s[6]);
  
  if (retdb != 0)
    {
      printf("insert_db_ucs2string reset bad %s\n", mysql_error(i_db->conn));
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

  // we are a dummy fro mysql and mariadb
  
  return 0;
}

/**
 * helper: free our session cached prepared statements
 */
int atrshmlog_free_prepared_db(atrshmlog_dbsession_t *i_db)
{
  for (int i = 0; i < 8; i++)
    {
      if (i_db->s[i] != 0)
	mysql_stmt_close(i_db->s[i]);
    }
  
  return 0;
}

/* end of file */
