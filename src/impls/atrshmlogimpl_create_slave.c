
#include "../atrshmlog_internal.h"


/********************************************************************/

/**
 * \file atrshmlogimpl_create_slave.c
 */

/** 
 * \n Main code:
 *
 * The last thread id of a slave  after creation.
 *
 * For now we only have the last thread id here.
 * In a future version i think its a good thing
 * to have a fourth list with the threads that are 
 * running as a slave. 
 *
 * But for now its only this last one.
 */
_Alignas(128) volatile atrshmlog_tid_t atrshmlog_f_list_buffer_slave;


/*******************************************************************/

/**
 * \n Main code:
 *
 * \brief The start of a slave thread.
 * 
 * You can use this if you have very diffrent needs for the logging
 * and you can judge from the program to do it.
 * 
 * Sometimes a program needs only few threads for logging and so few for
 * the slave activity.
 *
 * When then a hot spot of logging comes up you want to raise the number
 * of threads for the slaves too.
 *
 * So you call this one and a new slave is created.
 *
 * test t_create_slave.c
 */
int atrshmlog_create_slave(void)
{
  ATRSHMLOGSTAT(atrshmlog_counter_create_slave);

  atrshmlog_slave_t* i = (atrshmlog_slave_t*) calloc(1, sizeof(atrshmlog_slave_t));

  if (i == NULL)
    return -1;
  
#if  ATRSHMLOG_USE_PTHREAD  == 1
  pthread_t p;
  
  int ret = pthread_create(&p,
		 NULL,
		 atrshmlog_f_list_buffer_slave_proc,
		 i);

  atrshmlog_f_list_buffer_slave = 0;
  
  memcpy((void*)&atrshmlog_f_list_buffer_slave, &p, sizeof(atrshmlog_tid_t) < sizeof(p) ? sizeof(atrshmlog_tid_t) : sizeof(p));

  if (ret != 0)
    free(i);
  
#endif

#if  ATRSHMLOG_USE_C11_THREAD == 1
  thrd_t p;
  
  int ret = thrd_create( &p,
			 atrshmlog_f_list_buffer_slave_proc,
			 i);

  atrshmlog_f_list_buffer_slave = 0;

  memcpy((void*)&atrshmlog_f_list_buffer_slave, &p, sizeof(atrshmlog_tid_t) < sizeof(p) ? sizeof(atrshmlog_tid_t) : sizeof(p));

  if (ret != thrd_success)
    free(i);
  
#endif

#if ATRSHMLOG_USE_WINTHREAD == 1
  int ret = 0;
  
  uintptr_t rethandle = _beginthread (
				      atrshmlog_f_list_buffer_slave_proc,
				      0, // stacksize by system
				      i // arguments
				      );

  atrshmlog_f_list_buffer_slave = 0;

  if (rethandle == 0)
    {
      ret = -1;
      free(i);
    }

  if (rethandle == -1L)
    {
      ret = -1;
      free(i);
    }

#endif

#if ATRSHMLOGDEBUG == 1
  printf("start slave %d\n", ret);
#endif
  
  return ret;
}


