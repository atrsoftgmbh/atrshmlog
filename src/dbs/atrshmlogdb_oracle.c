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

/** \file atrshmlogdb_oracle.c
 * \brief We deliver the database functions for oracle db
 *
 */


#include "../atrshmlog_internal.h"


/*
 *--------------------------------------------------------
 * 
 * db specific part : incudes and definitions in dbsession
 */


// we are the oracle
#include <oci.h>
#include <ocidfn.h>

// oracle dynamic bind helper call back functions
// we need those in the returning clause callbacks.
// see cdemodr2.c in rdbms/demo of your oralce for how
// to use them in a simple setup
// we have it even simpler....

// input is no data 
sb4 cbf_input_data( dvoid *ctxp,
		    OCIBind *bindp,
		    ub4 iter,
		    ub4 index,
		    dvoid **buffpp,
		    ub4 *alenpp,
		    ub1 *piecep,
		    dvoid **indpp);

//output can be data in theory ... but they are here empty lobs
sb4 cbf_output_data( dvoid *ctxp,
		     OCIBind *bindp,
		     ub4 iter,
		     ub4 index,
		     dvoid **buffpp,
		     ub4 **alenpp,
		     ub1 *piecep,
		     dvoid **indpp,
		     ub2 **rcodepp);
/*
 *--------------------------------------------------------
 */

#define BIND0START 0 // begin
#define BIND1START 0 // commit
#define BIND2START 0 // rollback
#define BIND3START 0 // get head seq
#define BIND4START 0 // insert head - the first statement with a real bind
#define BIND5START (38) // insert cstring - the second statement
#define BIND6START (38+15) // insert ucs2 string 
#define BIND7START (38+15+14) // last id

// we use for every session a full connection in this version
// you should check evtl the pooling instead.
// and dont forget things like direct path inserts...
struct atrshmlog_dbsession_s {
  OCIEnv     *envhp;
  OCIError   *errhp;
  OCIServer  *srvhp;
  OCISvcCtx  *svchp;
  OCISession *seshp;
  
  // we have a small state machine here
  // 0 : not connected
  // 1 : connect ready
  // 2 : statements prepared
  // 3 : in transaction
  int status;

  const char * const *values;

  OCIStmt   *stmthp[8]; // begin commit rollback get seq head cstring ucs2string last id 

  OCIBind   *bindhp[BIND7START + 0] ;

  uint64_t head_id;

  sb4 len;
  
  atrshmlog_io_head_t h;

  atrshmlog_chunk_head_t c;

  OCIDefine *defnhp[2] ; // get seq, last id
  
  uint64_t next_seq;
  uint64_t last_seq;

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
  // this is 0 for oracle
  return 0;
}

/*
 *--------------------------------------------------------
 */


/*********************************************************************/

static int once_db = -1;

/* 
 * oracle code is here 
 */

static char *val[9] = { 0 } ; 

static char appname[63] = { 0 } ;

// memory handling via callbacks. but we dont need them ...
typedef void* (*FCTmalloc_t)(void* , size_t);
typedef void* (*FCTrealloc_t)(void* , void*, size_t);
typedef void (*FCTfree_t)(void* , void*);

/**
 * db specific funtion : create
 */
atrshmlog_dbsession_t* atrshmlog_create_db(void)
{
  atrshmlog_dbsession_t* r =   (atrshmlog_dbsession_t*)calloc(1, sizeof(  atrshmlog_dbsession_t));

  if (r == NULL)
    return r;

  if (once_db == -1)
    once_db = 0;

  once_db++; // from now on we are a counter. so if we make it to 0 the
  // cleanup can do its terminate job
  
  // we have ram ...

  r->status  = 0; // no init so far.

  r->envhp = 0;
  r->errhp = 0;
  r->svchp = 0;
  r->srvhp = 0;

  for (int i = 0; i < 8; i++)
    r->stmthp[i] = 0;

  if (val[0] == NULL || val[0][0] == 0)
    {
      val[0] = (char*)ATRSHMLOG_GET_ENV("_ORADBNAME");
      val[4] = (char*)ATRSHMLOG_GET_ENV("_ORAUSER");
      val[5] = (char*)ATRSHMLOG_GET_ENV("_ORAPWD");
      snprintf(appname, 63, "ATRSHMLOG_ORA%ld", (long)getpid());
    
      val[7] = appname;

      val[8] = 0;

      if (val[0] == NULL || val[0][0] == 0)
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

  // we first create per thread an env 
  ub4 typ = OCI_DEFAULT;
  void *ctxp = 0;
  FCTmalloc_t malocfp = 0;
  FCTrealloc_t ralocfp = 0;
  FCTfree_t mfreefp = 0;
  size_t xtramemsz = 0;
  //  void *usrmempp ; // STRANGE : oracle dont like a real adress even with size 0
  // so we do not use it . we use 0 instead directly
  ub2 charset = 0;
  ub2 ncharset = 0;
  
  int retdb  = OCIEnvNlsCreate (&r->envhp,
				typ,
				ctxp,
				malocfp,
				ralocfp,
				mfreefp,
				xtramemsz,
				0 , // funny. but if i give it a valid adress it fails ....  even with size 0 
				charset,
				ncharset
				);
  
  if (retdb != OCI_SUCCESS)
    {
      free(r);
      printf("create_db no oci env create %d\n", retdb);
      return NULL;
    }

  r->status = 0;

  // we get the error handle for this session  
  typ = OCI_HTYPE_ERROR;

  retdb = OCIHandleAlloc( r->envhp,
			  (void**)&r->errhp,
			  typ,
			  xtramemsz,
			  0);

  if(retdb != OCI_SUCCESS)
    {
      free(r);
      printf("create_db no err env create\n");
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  // we prepare the server handle 
  typ = OCI_HTYPE_SERVER;

  retdb = OCIHandleAlloc ( r->envhp,
			   (void**)&r->srvhp,
			   typ,
			   xtramemsz,
			   0);

  if(retdb != OCI_SUCCESS)
    {
      free(r);
      printf("create_db no srv env create\n");
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  // we connect now to the server
  text* dbname = (text *)r->values[0];
  sb4 len = strlen(r->values[0]);
  ub4 mode = OCI_DEFAULT;

  printf("create_db attach %s \n", r->values[0]);

  retdb = OCIServerAttach ( r->srvhp,
			    r->errhp,
			    dbname,
			    len,
			    mode);

  if(retdb != OCI_SUCCESS)
    {
      free(r);
      printf("create_db no attach env create\n");
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  // we are connected. from now on we need a cleanup
  r->status = 1;

  // the service handle
  typ = OCI_HTYPE_SVCCTX;
  
  retdb = OCIHandleAlloc ( r->envhp,
			   (void**)&r->svchp,
			   typ,
			   xtramemsz,
			   0);

  if(retdb != OCI_SUCCESS)
    {
      free(r);
      printf("create_db no svc ctx env create\n");
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  // we can now set the server for the service 
  ub4 ulen = 0;

  typ = OCI_HTYPE_SVCCTX;

  ub4 attrtyp = OCI_ATTR_SERVER;
  
  retdb =  OCIAttrSet( r->svchp,
		       typ,
		       r->srvhp,
		       ulen,
		       attrtyp,
		       r->errhp);

  if(retdb != OCI_SUCCESS)
    {
      free(r);
      printf("create_db no set srv env create\n");
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  // we create the session on top of the server connect
  // we use a 1 to 1 relation here, no pools
  typ = OCI_HTYPE_SESSION;
  
  retdb = OCIHandleAlloc(r->envhp,
			 (void**)&r->seshp,
			 typ,
			 xtramemsz,
			 0);
  
  if(retdb != OCI_SUCCESS)
    {
      free(r);
      printf("create_db no ses env create\n");
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  // we take user and password now
  
  text* username = (text*)r->values[4];

  ulen = strlen(r->values[4]);

  typ = OCI_HTYPE_SESSION;
  
  attrtyp = OCI_ATTR_USERNAME;
  
  retdb = OCIAttrSet(r->seshp,
		     typ,
		     username,
		     ulen,
		     attrtyp,
		     r->errhp);
  
  if(retdb != OCI_SUCCESS)
    {
      free(r);
      printf("create_db no set user env create\n");
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  text* password = (text*)r->values[5];

  ulen = strlen(r->values[5]);

  typ = OCI_HTYPE_SESSION;
  
  attrtyp = OCI_ATTR_PASSWORD;
  
  retdb = OCIAttrSet(r->seshp,
		     typ,
		     password,
		     ulen,
		     attrtyp,
		     r->errhp);

  if(retdb != OCI_SUCCESS)
    {
      free(r);
      printf("create_db no set psw env create\n");
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  // we start the session
  ub4 credits = OCI_CRED_RDBMS;

  mode = OCI_DEFAULT; // later we think a bit about stmt cache ...
  
  retdb = OCISessionBegin (r->svchp,
			   r->errhp,
			   r->seshp,
			   credits,
			   mode);
  
  if (retdb != OCI_SUCCESS)
    {
      free(r);
      printf("create_db no ses begin env create\n");
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  // we set the session in the service
  typ = OCI_HTYPE_SVCCTX;
  ulen = 0;
  attrtyp = OCI_ATTR_SESSION;

  retdb = OCIAttrSet(r->svchp,
		     typ,
		     r->seshp,
		     ulen,
		     attrtyp,
		     r->errhp);
  
  if (retdb != OCI_SUCCESS)
    {
      free(r);
      printf("create_db no set ses in svc env create\n");
      atrshmlog_cleanup_db(r);
      return NULL;
    }
   
  // prepare the statements

  // begin, commit and rollback not needed in oracle as p s

  // get head seq
  typ = OCI_HTYPE_STMT;
  
  retdb = OCIHandleAlloc( r->envhp,
			  (void**)&r->stmthp[3],
			  typ,
			  xtramemsz,
			  0);
   
  
  if (retdb != OCI_SUCCESS)
    {
      printf("create_db alloc 3 result %d.\n", retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }
   
  retdb = OCIHandleAlloc( r->envhp,
			  (void**)&r->stmthp[4],
			  typ,
			  xtramemsz,
			  0);
   
  
  if (retdb != OCI_SUCCESS)
    {
      printf("create_db alloc 4 result %d.\n", retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }
  
  
  retdb = OCIHandleAlloc( r->envhp,
			  (void**)&r->stmthp[5],
			  typ,
			  xtramemsz,
			  0);
   
  
  if (retdb != OCI_SUCCESS)
    {
      printf("create_db alloc 5 result %d.\n", retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  retdb = OCIHandleAlloc( r->envhp,
			  (void**)&r->stmthp[6],
			  typ,
			  xtramemsz,
			  0);
  
  
  if (retdb != OCI_SUCCESS)
    {
      printf("create_db alloc 6 result %d.\n", retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  // this is all. we dont need 7 or 8 here

  // we prepare now the statemnts
  // ----------------------------------------------------------------------
  
  text* stmt = "SELECT atrshmlog_head_seq.nextval from DUAL";

  ulen = strlen(stmt);

  typ = OCI_NTV_SYNTAX;

  mode = OCI_DEFAULT;
   
  retdb = OCIStmtPrepare(r->stmthp[3],
			 r->errhp,
			 stmt,
			 ulen,
			 typ,
			 mode);
  
  if (retdb != OCI_SUCCESS)
    {
      printf("create_db prep 3 result %d.\n", retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }
  
  // we define the output for the seq fetch
  mode = OCI_DEFAULT;
   
  ub4 pos = 1; // first field is 1 in oracle 

  sb4 siz = 8;

  ub2 styp = SQLT_INT;

  void* indicat_p = 0;

  ub2 *rlenp = 0;
  ub2 *rcodep = 0;
   
  retdb = OCIDefineByPos(r->stmthp[3],
			 &r->defnhp[0],
			 r->errhp,
			 pos, 
			 &r->next_seq,
			 siz,
			 styp,
			 indicat_p,
			 rlenp,
			 rcodep,
			 mode);
   
  if (retdb != OCI_SUCCESS)
    {
      printf("create_db prep 3 result %d.\n", retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }
  
  // ----------------------------------------------------------------------
  // we prepare the insert of head
   
  stmt = "INSERT INTO atrshmlog_head(head_id,"
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
    "counter_write0_adaptive_very_f," // oracle limit touched - only 30 chars ...
    "counter_write1,"
    "counter_write1_discard,"
    "counter_write1_wait,"
    "counter_write1_adaptive,"
    "counter_write1_adaptive_fast,"
    "counter_write1_adaptive_very_f,"
    "counter_write2,"
    "counter_write2_discard,"
    "counter_write2_wait,"
    "counter_write2_adaptive,"
    "counter_write2_adaptive_fast,"
    "counter_write2_adaptive_very_f)"
    " VALUES ("
    ":b_head_id,"
    ":b_version,"
    ":b_totallen,"
    ":b_pid,"
    ":b_tid,"
    ":b_bnr,"
    ":b_fnr,"
    ":b_inittime_sec,"
    ":b_inittime_nsec,"
    ":b_inittime_before,"
    ":b_inittime_after,"
    ":b_lasttime_sec,"
    ":b_lasttime_nsec,"
    ":b_lasttime_before,"
    ":b_lasttime_after,"
    ":b_difftimetransfer,"
    ":b_starttransfer,"
    ":b_acquiretime,"
    ":b_bid,"
    ":b_number_dispatched,"
    ":b_counter_write0,"
    ":b_counter_write0_discard,"
    ":b_counter_write0_wait,"
    ":b_counter_write0_adaptive,"
    ":b_counter_write0_adaptive_fast,"
    ":b_counter_write0_adaptive_very,"
    ":b_counter_write1,"
    ":b_counter_write1_discard,"
    ":b_counter_write1_wait,"
    ":b_counter_write1_adaptive,"
    ":b_counter_write1_adaptive_fast,"
    ":b_counter_write1_adaptive_very,"
    ":b_counter_write2,"
    ":b_counter_write2_discard,"
    ":b_counter_write2_wait,"
    ":b_counter_write2_adaptive,"
    ":b_counter_write2_adaptive_fast,"
    ":b_counter_write2_adaptive_very)";

  ulen = strlen(stmt);
   
  typ = OCI_NTV_SYNTAX;
   
  mode = OCI_DEFAULT;
   
  retdb = OCIStmtPrepare(r->stmthp[4],
			 r->errhp,
			 stmt,
			 ulen,
			 typ,
			 mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db prep 4 result %d.\n", retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  // we bind as fast as possible. so we have a dummy head here in place
  // we do not use the array interface - you can do this as a speed up.
  // we do not use direct path load here - you can do this as a speed up.


  int bindindex = BIND4START;
     
  mode = OCI_DEFAULT;

  text* placeholder = ":b_head_id";

  sb4 placeholder_len = strlen(placeholder);

  void *valuep = &r->head_id;

  sb4 value_sz = 8;

  ub2 dty = SQLT_INT;

  void *indp = 0 ;

  ub2 *alenp = 0 ; // we dont use it, no string

  rcodep = 0; // no return values

  ub4 maxarr_len = 0; // no pl/sql array operation

  ub4 *curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_version";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.version;

  value_sz = 4;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }

   
  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_totallen";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.tlen;

  value_sz = 4;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_pid";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.pid;

  value_sz = 8;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_tid";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.tid;

  value_sz = 8;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_bnr";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.buffernumber;

  value_sz = 4;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_fnr";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.filenumber;

  value_sz = 4;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_inittime_sec";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.inittime.tv_sec;

  value_sz = 8;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_inittime_nsec";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.inittime.tv_nsec;

  value_sz = 4;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_inittime_before";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.inittsc_before;

  value_sz = 8;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }

      

  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_inittime_after";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.inittsc_after;

  value_sz = 8;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_lasttime_sec";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.lasttime.tv_sec;

  value_sz = 8;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_lasttime_nsec";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.lasttime.tv_nsec;

  value_sz = 4;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_lasttime_before";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.lasttsc_before;

  value_sz = 8;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }

   

  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_lasttime_after";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.lasttsc_after;

  value_sz = 8;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_difftimetransfer";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.difftimetransfer;

  value_sz = 8;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_starttransfer";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.starttransfer;

  value_sz = 8;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_acquiretime";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.acquiretime;

  value_sz = 8;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_bid";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.id;

  value_sz = 4;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_number_dispatched",

    placeholder_len = strlen(placeholder);

  valuep = &r->h.number_dispatched;

  value_sz = 4;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_counter_write0";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.counter_write0;

  value_sz = 4;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_counter_write0_discard";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.counter_write0_discard;

  value_sz = 4;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_counter_write0_wait";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.counter_write0_wait;

  value_sz = 4;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_counter_write0_adaptive";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.counter_write0_adaptive;

  value_sz = 4;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_counter_write0_adaptive_fast";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.counter_write0_adaptive_fast;

  value_sz = 4;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_counter_write0_adaptive_very";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.counter_write0_adaptive_very_fast;

  value_sz = 4;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_counter_write1";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.counter_write1;

  value_sz = 4;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_counter_write1_discard";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.counter_write1_discard;

  value_sz = 4;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_counter_write1_wait";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.counter_write1_wait;

  value_sz = 4;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_counter_write1_adaptive";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.counter_write1_adaptive;

  value_sz = 4;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_counter_write1_adaptive_fast";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.counter_write1_adaptive_fast;

  value_sz = 4;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_counter_write1_adaptive_very";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.counter_write1_adaptive_very_fast;

  value_sz = 4;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }



  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_counter_write2";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.counter_write2;

  value_sz = 4;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_counter_write2_discard";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.counter_write2_discard;

  value_sz = 4;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_counter_write2_wait";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.counter_write2_wait;

  value_sz = 4;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_counter_write2_adaptive";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.counter_write2_adaptive;

  value_sz = 4;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_counter_write2_adaptive_fast";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.counter_write2_adaptive_fast;

  value_sz = 4;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_counter_write2_adaptive_very";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.counter_write2_adaptive_very_fast;

  value_sz = 4;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[4],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 4, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  // ----------------------------------------------------------------------
   

  // we prepare the insert of cstring 
  stmt = "INSERT INTO atrshmlog_log(head_id,"
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
    "len,"
    "payload,"
    "payload_big) "
    " VALUES ("
    ":b_head_id,"
    ":b_pid,"
    ":b_tid,"
    ":b_startclick,"
    ":b_endclick,"
    ":b_deltaclick,"
    ":b_startreal,"
    ":b_endreal,"
    ":b_deltareal,"
    ":b_logevent,"
    ":b_logflag,"
    ":b_userflag,"
    ":b_len,"
    ":b_payload,"
    "EMPTY_CLOB()) "
    "RETURNING payload_big INTO :o_payload_big" ;

  ulen = strlen(stmt);
   
  typ = OCI_NTV_SYNTAX;
   
  mode = OCI_DEFAULT;
   
  retdb = OCIStmtPrepare(r->stmthp[5],
			 r->errhp,
			 stmt,
			 ulen,
			 typ,
			 mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db prep 5 result %d.\n", retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }

   
  bindindex = BIND5START;
     
  mode = OCI_DEFAULT;

  placeholder = ":b_head_id";

  placeholder_len = strlen(placeholder);

  valuep = &r->head_id;

  value_sz = 8;

  dty = SQLT_INT;

  indp = 0 ; // not null column

  alenp = 0 ; // we dont use it, no string

  rcodep = 0; // no return values

  maxarr_len = 0; // no pl/sql array operation

  curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[5],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 5, %d %s result %d.\n", bindindex, placeholder, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_pid";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.pid;

  value_sz = 8;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[5],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 5, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_tid";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.tid;

  value_sz = 8;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[5],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 5, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_startclick";

  placeholder_len = strlen(placeholder);

  valuep = &r->c.starttime;

  value_sz = 8;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[5],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 5, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_endclick";

  placeholder_len = strlen(placeholder);

  valuep = &r->c.endtime;

  value_sz = 8;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[5],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 5, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_deltaclick";

  placeholder_len = strlen(placeholder);

  valuep = &r->c.deltatime;

  value_sz = 8;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[5],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 5, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_startreal";

  placeholder_len = strlen(placeholder);

  valuep = &r->c.startreal;

  value_sz = 8;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[5],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 5, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_endreal";

  placeholder_len = strlen(placeholder);

  valuep = &r->c.endreal;

  value_sz = 8;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[5],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 5, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_deltareal";

  placeholder_len = strlen(placeholder);

  valuep = &r->c.deltareal;

  value_sz = 8;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[5],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 5, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_logevent";

  placeholder_len = strlen(placeholder);

  valuep = &r->c.eventnumber;

  value_sz = 4;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[5],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 5, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_logflag";

  placeholder_len = strlen(placeholder);

  valuep = &r->c.eventflag;

  value_sz = 1;

  dty = SQLT_AFC;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[5],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 5, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_userflag";

  placeholder_len = strlen(placeholder);

  valuep = &r->c.userflag;

  value_sz = 4;

  dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[5],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 5, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }

                                 
  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_len";

  placeholder_len = strlen(placeholder);

  valuep = &r->len;

  value_sz = 4;

  dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[5],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 5, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }

                                 
  // we bind payload and clob on a per insert base

  // ----------------------------------------------------------------------

  // we prepare the ucs2 insert - first only the insert
  stmt = "INSERT INTO atrshmlog_log_ucs2(head_id,"
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
    "len,"
    "payload_big) "
    " VALUES ("
    ":b_head_id,"
    ":b_pid,"
    ":b_tid,"
    ":b_startclick,"
    ":b_endclick,"
    ":b_deltaclick,"
    ":b_startreal,"
    ":b_endreal,"
    ":b_deltareal,"
    ":b_logevent,"
    ":b_logflag,"
    ":b_userflag,"
    ":b_len,"
    "EMPTY_BLOB()) "
    "RETURNING payload_big INTO :o_payload_big";
   
  ulen = strlen(stmt);
   
  typ = OCI_NTV_SYNTAX;
   
  mode = OCI_DEFAULT;
   
  retdb = OCIStmtPrepare(r->stmthp[6],
			 r->errhp,
			 stmt,
			 ulen,
			 typ,
			 mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db prep 6 result %d.\n", retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }

   
  bindindex = BIND6START;
     
  mode = OCI_DEFAULT;

  placeholder = ":b_head_id";

  placeholder_len = strlen(placeholder);

  valuep = &r->head_id;

  value_sz = 8;

  dty = SQLT_INT;

  indp = 0 ; // not null column

  alenp = 0 ; // we dont use it, no string

  rcodep = 0; // no return values

  maxarr_len = 0; // no pl/sql array operation

  curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[6],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 6, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_pid";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.pid;

  value_sz = 8;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[6],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 6, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_tid";

  placeholder_len = strlen(placeholder);

  valuep = &r->h.tid;

  value_sz = 8;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[6],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 6, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_startclick";

  placeholder_len = strlen(placeholder);

  valuep = &r->c.starttime;

  value_sz = 8;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[6],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 6, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_endclick";

  placeholder_len = strlen(placeholder);

  valuep = &r->c.endtime;

  value_sz = 8;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[6],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 6, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_deltaclick";

  placeholder_len = strlen(placeholder);

  valuep = &r->c.deltatime;

  value_sz = 8;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[6],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 6, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_startreal";

  placeholder_len = strlen(placeholder);

  valuep = &r->c.startreal;

  value_sz = 8;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[6],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 6, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_endreal";

  placeholder_len = strlen(placeholder);

  valuep = &r->c.endreal;

  value_sz = 8;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[6],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 6, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_deltareal";

  placeholder_len = strlen(placeholder);

  valuep = &r->c.deltareal;

  value_sz = 8;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[6],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 6, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_logevent";

  placeholder_len = strlen(placeholder);

  valuep = &r->c.eventnumber;

  value_sz = 4;

  // dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[6],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 6, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_logflag";

  placeholder_len = strlen(placeholder);

  valuep = &r->c.eventflag;

  value_sz = 1;

  dty = SQLT_AFC;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[6],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 6, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }


  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_userflag";

  placeholder_len = strlen(placeholder);

  valuep = &r->c.userflag;

  value_sz = 4;

  dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[6],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 6, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  ++bindindex;
   
  // mode = OCI_DEFAULT;

  placeholder = ":b_len";

  placeholder_len = strlen(placeholder);

  valuep = &r->len;

  value_sz = 4;

  dty = SQLT_INT;

  // indp = 0 ; // not null column

  // alenp = 0 ; // we dont use it, no string

  // rcodep = 0; // no return values

  // maxarr_len = 0; // no pl/sql array operation

  // curelep = 0; // no pl/sql array operation

  retdb = OCIBindByName(r->stmthp[6],
			&r->bindhp[bindindex],
			r->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 6, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(r);
      return NULL;
    }

  // the blob comes in insert

  // no 7 and 8 statements 
                                 
   
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

  int retdb;
  
  if (i_db->svchp)
    {
      ub4 mode = OCI_DEFAULT;

      // make a final roundtrip to free server side resources
      OCIPing(i_db->svchp,
	      i_db->errhp,
	      mode);

      if (i_db->seshp)
	{
	  // we end the user session
	  mode = OCI_DEFAULT;

	  retdb = OCISessionEnd(i_db->svchp,
				i_db->errhp,
				i_db->seshp,
				mode);
      
	  if (retdb != OCI_SUCCESS)
	    {
	      printf("ses end error\n");
	    }
	}

      
      if (i_db->srvhp)
	{
	  // we disconnect from server
	  retdb = OCIServerDetach(i_db->srvhp,
				  i_db->errhp,
				  mode);
      
	  if (retdb != OCI_SUCCESS)
	    {
	      printf("srv end error\n");
	    }
	}
      
      // now we free the service ctx
      ub4 typ = OCI_HTYPE_SVCCTX;
      
      retdb = OCIHandleFree(i_db->svchp, typ);

      if (retdb != OCI_SUCCESS)
	{
	  printf("svc handle free error\n");
	}

      i_db->svchp = 0;
    }

  if (i_db->errhp)
    {
      ub4 typ = OCI_HTYPE_ERROR;
      
      retdb = OCIHandleFree(i_db->errhp, typ);

      if (retdb != OCI_SUCCESS)
	{
	  printf("err handle free error\n");
	}

      i_db->errhp = 0;

    }

  // only one per application ... uhm. seems we have a atexit here in need ?
  // i try it first with a poor man singleton

  once_db --;
  
  if (once_db == 0)
    {
      ub4 mode = OCI_DEFAULT;
      
      retdb = OCITerminate(mode);
    }

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


  // this is a dummy call for oracle.
  // it does not need a begin for the active global tx
  // and we dont attach or detach here....
  // so no use of multiple tx on one session.
  
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


  ub4 mode = OCI_DEFAULT;
  
  int retdb = OCITransCommit(i_db->svchp,
			     i_db->errhp,
			     mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("commit problem\n");
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

  ub4 mode = OCI_DEFAULT;
  
  int retdb = OCITransRollback(i_db->svchp,
			     i_db->errhp,
			     mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("rollback problem\n");
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


  i_db->head_id = *io_head_id;
  i_db->h = *i_head;

  ub4 iters = 1;
  ub4 rowoff = 0;
  const OCISnapshot   *snap_in = 0;
  OCISnapshot         *snap_out = 0;
  ub4 mode = OCI_DEFAULT;
  
  int retdb = OCIStmtExecute(i_db->svchp,
			     i_db->stmthp[4],
			     i_db->errhp,
			     iters,
			     rowoff,
			     snap_in,
			     snap_out,
			     mode);
  if(retdb != OCI_SUCCESS)
    {
      printf("execute head  err %d\n", retdb);
      return -3;
    }

  return 0;
}


// input context needs at least a indicator
struct input_s {
  sb2 indicator[2];
};
typedef struct input_s input_t;

// output context , no dummy receive the lob locator
struct output_s {
  ub4 rows;
  ub4 return_length[2];
  OCILobLocator* return_locator[2];
  sb2 indicator[2];
  ub2 return_code[2];
};
typedef struct output_s output_t;

// type of the callback in pice wise write for write2, but we dont use it
typedef sb4 (*OCICallbackLobWrite2_t)(dvoid *, dvoid *, oraub8 *, ub1 *, void **, oraub8 *); 

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


  i_db->head_id = i_head_id;
  i_db->h = *i_head;
  i_db->c = *i_chunk;
  
  int bindindex = BIND5START + 13;
  
  text* placeholder = ":b_payload";

  sb4 placeholder_len = strlen(placeholder);

  void *valuep = i_db->c.payload;

  i_db->len = i_db->c.totallen - ATRSHMLOGCONTROLDATASIZE;

  int needlob = 0;
  
  sb4 value_sz = i_db->len;

  // hack - we use later on a switch to clob here.....
  // for now we are happy with the first 3950 char ...
  if (value_sz > 3950)
    {
      needlob = 1;
      
      value_sz = 3950;
    }

  ub2 dty = SQLT_CHR;
    
  sb2 indval = 0 ; // thats not null for now 

  void *indp = &indval ; 

  ub2 *alenp = 0 ; // we dont use it, no string

  ub2 *rcodep = 0; // no return values

  ub4 maxarr_len = 0; // no pl/sql array operation

  ub4 *curelep = 0; // no pl/sql array operation

  ub4 mode = OCI_DEFAULT;
  
  int retdb = OCIBindByName(i_db->stmthp[5],
			&i_db->bindhp[bindindex],
			i_db->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 5, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(i_db);
      return NULL;
    }

  bindindex++;
  
  placeholder = ":o_payload_big";

  placeholder_len = strlen(placeholder);

  valuep = 0;

  value_sz = -1;

  dty = SQLT_CLOB;
    
  indp = 0; // ist empty from insert, not null

  alenp = 0 ; // we dont use it, no string

  rcodep = 0; // no return values

  maxarr_len = 0; // no pl/sql array operation

  curelep = 0; // no pl/sql array operation

  mode = OCI_DATA_AT_EXEC;
  
  retdb = OCIBindByName(i_db->stmthp[5],
			&i_db->bindhp[bindindex],
			i_db->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 5, %d %s result %d.\n", bindindex, placeholder, retdb);
      atrshmlog_cleanup_db(i_db);
      return NULL;
    }

  
  // input context dummy 
  input_t inputctx;

  // output context with locators - one active, one dummy
  
  output_t outputctx;

  // we have to set the locator in use.

  ub4 htyp =  OCI_DTYPE_LOB;
  size_t xtramem_sz = 0;
  
  retdb = OCIDescriptorAlloc(i_db->envhp,
			     (void**)&outputctx.return_locator[0],
			     htyp,
			     xtramem_sz,
			     0);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db locator 5, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(i_db);
      return NULL;
    }

  outputctx.return_length[0] = 86; // oracle deliver this in hard code form.

  outputctx.indicator[0] = 0 ; // not null
  outputctx.return_code[0] = 0; // the return code
			
  retdb = OCIBindDynamic(i_db->bindhp[bindindex],
			 i_db->errhp,
			 (dvoid *)&inputctx,
			 cbf_input_data,
			 (dvoid *)&outputctx,
			 cbf_output_data);
    
  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind dynamic 5, %d %s result %d.\n", bindindex, placeholder, retdb);
      atrshmlog_cleanup_db(i_db);
      return NULL;
    }

  ub4 iters = 1;
  ub4 rowoff = 0;
  const OCISnapshot   *snap_in = 0;
  OCISnapshot         *snap_out = 0;
  mode = OCI_DEFAULT;
  
  retdb = OCIStmtExecute(i_db->svchp,
			     i_db->stmthp[5],
			     i_db->errhp,
			     iters,
			     rowoff,
			     snap_in,
			     snap_out,
			     mode);

  if(retdb != OCI_SUCCESS)
    {
      printf("execute cstring err %d\n", retdb);
      return -3;
    }

  if (needlob)
    {
      oraub8 byte_amt = i_db->len;
      oraub8 char_amt = 0; // we use byte amount this time
      oraub8 offset = 1; // oracle lobs are starting with 1 
      void * buffp = i_db->c.payload;
      oraub8 buflen = byte_amt;  // same for first piece
      void* ctxp = 0; // we dont use a context here, only first write
      ub1 piece = OCI_ONE_PIECE; // we write one piece 
      ub2 csid = 0 ; // we take from env variables as normal
      ub1 csfrm = SQLCS_IMPLICIT; // default char form set
      
      retdb = OCILobWrite2(i_db->svchp,
			   i_db->errhp,
			   outputctx.return_locator[0],
			   &byte_amt,
			   &char_amt,
			   offset,
			   buffp,
			   buflen,
			   piece,
			   ctxp, 
			   (OCICallbackLobWrite2_t)0,
			   csid,
			   csfrm);
      
      if(retdb != OCI_SUCCESS)
	{
	  printf("execute cstring lob write %d\n", retdb);
	  return -3;
	}
      
    }

  retdb = OCIDescriptorFree(outputctx.return_locator[0],
			    htyp);
  
  if(retdb != OCI_SUCCESS)
    {
      printf("execute cstring desc free %d\n", retdb);
      return -3;
    }

  return 0;
}


/**
 * db specific funtion : insert_db_ucs2string
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

  i_db->head_id = i_head_id;
  i_db->h = *i_head;
  i_db->c = *i_chunk;

  i_db->len = len;

  int bindindex = BIND6START + 13;
  
  text* placeholder = ":o_payload_big";

  sb4 placeholder_len = strlen(placeholder);

  void *valuep = 0;

  sb4 value_sz = -1;

  ub2 dty = SQLT_BLOB;
    
  void *indp = 0; // ist empty from insert, not null

  ub2 *alenp = 0 ; // we dont use it, no string

  ub2 *rcodep = 0; // no return values

  ub4 maxarr_len = 0; // no pl/sql array operation

  ub4 *curelep = 0; // no pl/sql array operation

  ub4 mode = OCI_DATA_AT_EXEC;
  
  int retdb = OCIBindByName(i_db->stmthp[6],
			&i_db->bindhp[bindindex],
			i_db->errhp,
			placeholder,
			placeholder_len,
			valuep,
			value_sz,
			dty,
			indp,
			alenp,
			rcodep,
			maxarr_len,
			curelep,
			mode);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind 6, %d %s result %d.\n", bindindex, placeholder, retdb);
      atrshmlog_cleanup_db(i_db);
      return NULL;
    }

  
  // input context dummy 
  input_t inputctx;

  // output context with locators - one active, one dummy
  
  output_t outputctx;

  // we have to set the locator in use.

  ub4 htyp =  OCI_DTYPE_LOB;
  size_t xtramem_sz = 0;
  
  retdb = OCIDescriptorAlloc(i_db->envhp,
			     (void**)&outputctx.return_locator[0],
			     htyp,
			     xtramem_sz,
			     0);

  if (retdb != OCI_SUCCESS)
    {
      printf("create_db locator 6, %d result %d.\n", bindindex, retdb);
      atrshmlog_cleanup_db(i_db);
      return NULL;
    }

  outputctx.return_length[0] = 86; // oracle deliver this in hard code form.

  outputctx.indicator[0] = 0 ; // not null
  outputctx.return_code[0] = 0; // the return code
			
  retdb = OCIBindDynamic(i_db->bindhp[bindindex],
			 i_db->errhp,
			 (dvoid *)&inputctx,
			 cbf_input_data,
			 (dvoid *)&outputctx,
			 cbf_output_data);
    
  if (retdb != OCI_SUCCESS)
    {
      printf("create_db bind dynamic 6, %d %s result %d.\n", bindindex, placeholder, retdb);
      atrshmlog_cleanup_db(i_db);
      return NULL;
    }

  
  ub4 iters = 1;
  ub4 rowoff = 0;
  const OCISnapshot   *snap_in = 0;
  OCISnapshot         *snap_out = 0;
  mode = OCI_DEFAULT;
  
  retdb = OCIStmtExecute(i_db->svchp,
			     i_db->stmthp[6],
			     i_db->errhp,
			     iters,
			     rowoff,
			     snap_in,
			     snap_out,
			     mode);
  if(retdb != OCI_SUCCESS)
    {
      printf("execute cstring err %d\n", retdb);
      return -3;
    }


  oraub8 byte_amt = i_db->len;
  oraub8 char_amt = 0; // we use byte amount 
  oraub8 offset = 1; // oracle lobs are starting with 1 
  void * buffp = i_db->c.payload;
  oraub8 buflen = byte_amt;  // same for first piece
  void* ctxp = 0; // we dont use a context here, only first write
  ub1 piece = OCI_ONE_PIECE; // we write one piece 
  ub2 csid = 0 ; // we take from env variables as normal
  ub1 csfrm = SQLCS_IMPLICIT; // default char form set
  
  retdb = OCILobWrite2(i_db->svchp,
		       i_db->errhp,
		       outputctx.return_locator[0],
		       &byte_amt,
		       &char_amt,
		       offset,
		       buffp,
		       buflen,
		       piece,
		       ctxp, 
		       (OCICallbackLobWrite2_t)0,
		       csid,
		       csfrm);
      
  if(retdb != OCI_SUCCESS)
    {
      printf("execute ucs string lob write %d\n", retdb);
      return -3;
    }
      
  retdb = OCIDescriptorFree(outputctx.return_locator[0],
			    htyp);
  
  if(retdb != OCI_SUCCESS)
    {
      printf("execute ucs string desc free %d\n", retdb);
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

  ub4 iters = 0; // we do not use a multi row fetch
  ub4 rowoff = 0;
  const OCISnapshot   *snap_in = 0;
  OCISnapshot         *snap_out = 0;
  ub4 mode = OCI_DEFAULT;
  
  int retdb = OCIStmtExecute(i_db->svchp,
			     i_db->stmthp[3],
			     i_db->errhp,
			     iters,
			     rowoff,
			     snap_in, // no snapshot
			     snap_out, // no snapshot
			     mode);
			     
  int i;
  
  if (retdb != OCI_SUCCESS)
    {
      printf("get_head_seq_db bad %d \n", retdb);

      return -1;
    }

  // we get the new head_id in the define variable 
  *o_head_id = i_db->next_seq;
  
  return 0;
}

/**
 * helper: free our session cached prepared statements
 */
int atrshmlog_free_prepared_db(atrshmlog_dbsession_t *i_db)
{
  if (i_db->stmthp[0])
    {
      ub4 typ = OCI_HTYPE_STMT;
      
      int retdb = OCIHandleFree(i_db->stmthp[0], typ);

      if (retdb != OCI_SUCCESS)
	{
	  printf("free prepared problem begin\n");  
	}
      i_db->stmthp[0] = 0;
    }
  
  if (i_db->stmthp[1])
    {
      ub4 typ = OCI_HTYPE_STMT;
      
      int retdb = OCIHandleFree(i_db->stmthp[1], typ);

      if (retdb != OCI_SUCCESS)
	{
	  printf("free prepared problem commit\n");  
	}
      i_db->stmthp[1] = 0;
    }
  
  if (i_db->stmthp[2])
    {
      ub4 typ = OCI_HTYPE_STMT;
      
      int retdb = OCIHandleFree(i_db->stmthp[2], typ);

      if (retdb != OCI_SUCCESS)
	{
	  printf("free prepared problem rollback\n");  
	}
      i_db->stmthp[2] = 0;
    }
  
  if (i_db->stmthp[3])
    {
      ub4 typ = OCI_HTYPE_STMT;
      
      int retdb = OCIHandleFree(i_db->stmthp[3], typ);

      if (retdb != OCI_SUCCESS)
	{
	  printf("free prepared problem get seq\n");  
	}
      i_db->stmthp[3] = 0;
    }
  
  if (i_db->stmthp[4])
    {
      ub4 typ = OCI_HTYPE_STMT;
      
      int retdb = OCIHandleFree(i_db->stmthp[4], typ);

      if (retdb != OCI_SUCCESS)
	{
	  printf("free prepared problem head\n");  
	}
      i_db->stmthp[4] = 0;
    }
  
  if (i_db->stmthp[5])
    {
      ub4 typ = OCI_HTYPE_STMT;
      
      int retdb = OCIHandleFree(i_db->stmthp[5], typ);

      if (retdb != OCI_SUCCESS)
	{
	  printf("free prepared problem cstring\n");  
	}
      i_db->stmthp[5] = 0;
    }
  
  if (i_db->stmthp[6])
    {
      ub4 typ = OCI_HTYPE_STMT;
      
      int retdb = OCIHandleFree(i_db->stmthp[6], typ);

      if (retdb != OCI_SUCCESS)
	{
	  printf("free prepared problem ucs\n");  
	}
      i_db->stmthp[6] = 0;
    }
  
  if (i_db->stmthp[7])
    {
      ub4 typ = OCI_HTYPE_STMT;
      
      int retdb = OCIHandleFree(i_db->stmthp[7], typ);

      if (retdb != OCI_SUCCESS)
	{
	  printf("free prepared problem last\n");  
	}
      i_db->stmthp[7] = 0;
    }
  

  return 0;
}

/* ----------------------------------------------------------------- */
/* Inbind callback                                                   */
/* ----------------------------------------------------------------- */
sb4 cbf_input_data( dvoid *ctxp,
		    OCIBind *bindp,
		    ub4 iter,
		    ub4 index,
		    dvoid **bufpp,
		    ub4 *alenpp,
		    ub1 *piecep,
		    dvoid **indpp)
{
  input_t *inputctx = (input_t*)ctxp;
  
  *bufpp = (dvoid *)0;
  
  //  printf("input %p\n", ctxp);
  *alenpp = 0;

  // printf("input %p\n", ctxp);
  inputctx->indicator[0] = -1; // null value 
   
  //printf("input %p\n", ctxp);
  *indpp = (dvoid *)&inputctx->indicator[0];
  
  //printf("input %p\n", ctxp);
  *piecep = OCI_ONE_PIECE;

  return OCI_CONTINUE;
}

/* ----------------------------------------------------------------- */
/* Outbind callback                                                  */
/* ----------------------------------------------------------------- */
sb4 cbf_output_data( dvoid *ctxp,
		     OCIBind *bindp,
		     ub4 iter,
		     ub4 index,
		     dvoid **bufpp,
		     ub4 **alenp,
		     ub1 *piecep,
		     dvoid **indpp,
		     ub2 **rcodepp)
{
  ub4  bufl = 0;
  
  output_t* outputctx = (output_t *)ctxp;

  // we have only one iter and one index in our access,
  // if you have multiple you need loops here
  if (iter == 0 && index == 0)
    {
      outputctx->rows = 1;

      // we do not ask, its fix for this statement
      #if 0
      ub4 htyp = OCI_HTYPE_BIND;
      ub4 size_out = 4;

      ub4 attrtyp = OCI_ATTR_ROWS_RETURNED;
    
      (void) OCIAttrGet((CONST dvoid *) bindp,
			htyp,
			(dvoid *)&outputctx->rows,
			(ub4 *)&size_out,
			attrtyp,
			errhp);
      #endif
      
      *bufpp = (dvoid *) outputctx->return_locator[0];
      *indpp = (dvoid *) &outputctx->indicator[0];
      *rcodepp = (ub2*) &outputctx->return_code[0];
      *alenp = (ub4*)&outputctx->return_length[0];
    }
  
  *piecep = OCI_ONE_PIECE;

  return OCI_CONTINUE;
}

/* end of file */
