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

/** \file atrshmlogreset.c
 * \brief We are the check and reset tool for the shared memory
 */
#include "atrshmlog.h"

#include "atrshmlog_internal.h"

/*
 * ------------------------------------------------------------------
 */

/**
 * attach and verify the struct parameters
 *
 * this is a testing program
 * it is used with tee, reader and the others to make checking for error
 * possible.
 * you can of course always dump and then check the content with something like
 * od or a smart hex fileeditor ...
 * but for a quick check this is ok.
 * see for overrites and hanging buffers with corrupted
 * states ...
 * of wrong versiones ...
 * or whatever you can imageine what went wrong ...
 * after using this its then time to fire up your debugger ... in case
 * something is wrong.
 *
 */
int main (int argc, char*argv[])
{
  const char *shmid_ptr = ATRSHMLOG_GET_ENV_SHMID();

  long lanz;
  
  if (shmid_ptr == (void*)0)
    {
      printf("usage: %s count_of_buffers\n", argv[0]);
      printf("ENVIRONMENT VARIABLE NOT SET !!!\n");
      printf("%s_ID\n", ATRSHMLOG_GET_ENV_PREFIX());
      printf("logsystem version is %d.\n", ATRSHMLOGVERSION);
      exit(1);
    }

  if (argc != 2)
    {
      printf("usage: %s count_of_buffers\n", argv[0]);
      printf("parameter count wrong.\n");
      printf("logsystem version is %d.\n", ATRSHMLOGVERSION);
      exit(1);
    }

  lanz = strtol(argv[1], 0, 10);
  
  printf("shm log attach and verify.\n");
  printf("logsystem version is %d.\n", ATRSHMLOGVERSION);
  
  ATRSHMLOG_ATTACH();

  if (!ATRSHMLOG_LOGGING)
    {
      printf ("attach not successful.\ncheck ENVIRONMENT VARIABLE %s_ID\n"
          "Value seems not to fit : '%s'\n", ATRSHMLOG_GET_ENV_PREFIX(),shmid_ptr);

      exit(1);
    }

  fflush(stdout);
  
  {
    int v_result = ATRSHMLOG_VERIFY();

    if (v_result != 0)
      {
	printf("verify error %d. errno %d\n",  v_result, errno);
      }
    
    if (v_result == atrshmlog_error_verify_5)
      exit(1);

    /*******************************************/
    {
      atrshmlog_area_t * a = (atrshmlog_area_t *)ATRSHMLOG_GET_AREA();
      
      int anzahl_buffer = (int)lanz;
      int anzahl_shm = a->shmcount;
      int i = 0;
      
      if (anzahl_buffer != anzahl_shm)
	printf("count of buffer wrong: expected was %d, in shm is %d.\n",
	       anzahl_buffer, anzahl_shm);

      if (a->shmsafeguard != ATRSHMLOGSAFEGUARDVALUE)
	{
	  printf("safeguard wrong : 0X%lx , expect 0X%lx\n",
		 a->shmsafeguard, ATRSHMLOGSAFEGUARDVALUE);

	  a->shmsafeguard = ATRSHMLOGSAFEGUARDVALUE;
	  a->shmcount = anzahl_buffer;
	  printf("safeguard and count corrected. count is now %d\n", anzahl_buffer);
	}
      
      printf("ich habe fertig is %d\n", a->ich_habe_fertig);
      
      for (i = 0; i < anzahl_shm ; i++)
	{
	  int state = atomic_load(&a->logbuffers[i].state);
	  long size;
          
	  if (!(state == atrshmlog_writeable
		|| state == atrshmlog_write_halffull
		|| state == atrshmlog_full))
	    {
	      printf("error in buffer %d : state is %d.\n"
		     "  allowed %d, %d, %d, %d, %d.\n",
		     i,
		     state,
		     atrshmlog_uninit,
		     atrshmlog_writeable,
		     atrshmlog_write_halffull,
		     atrshmlog_full,
		     atrshmlog_illegal);

	      atomic_store(&a->logbuffers[i].state, atrshmlog_writeable);
	      a->logbuffers[i].shmsize = 0;
	      
	      printf("state set to writeable, size to 0\n");
	    }

	  if (a->logbuffers[i].safeguard != ATRSHMLOGSAFEGUARDVALUE)
	    {
	      printf("error in buffer %d, safeguard is 0X%lx, expect 0X%lx.\n",
		     i,
		     a->logbuffers[i].safeguard,
		     ATRSHMLOGSAFEGUARDVALUE);
	      
	      a->logbuffers[i].safeguard = ATRSHMLOGSAFEGUARDVALUE;
	      a->logbuffers[i].shmsize = 0;

	      printf("safeguard set, size to 0\n");
	    }

	  size = a->logbuffers[i].shmsize;

	  printf("buffer %d : size is %ld, limit %ld, state is %s\n",
		 i,
		 size,
		 ATRSHMLOGBUFFER_INFOSIZE,
		 state == atrshmlog_uninit ? "uninit" :
		 state == atrshmlog_writeable ? "writeable" :
		 state == atrshmlog_write_halffull ? "write_halffull":
		 state == atrshmlog_full ? "full" :
		 state == atrshmlog_illegal ? "illegal" :
		 "corrupt");

	  if (size >= ATRSHMLOGBUFFER_INFOSIZE)
	    {
	      printf("size too big.\n");
	      atomic_store(&a->logbuffers[i].state, atrshmlog_writeable);
	      a->logbuffers[i].shmsize = 0;
	      printf("size set to 0 , state to writeable.\n");
	    }
	}
    }
  }


  
  return 0;
}

/* end of file */
