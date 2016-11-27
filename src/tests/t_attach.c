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

/** \file t_attach.c
 * \brief We are a test program.
 */

#include "../atrshmlog.h"

#include "../atrshmlog_internal.h"

/*
 * ------------------------------------------------------------
 */

#include <stdio.h>


#define PN(_x) printf( #_x " : %ld :\n", (long) _x)

#define PS(_x) printf( #_x " : %s :\n", _x);

#define PP(_x) printf( #_x " : %p :\n", _x);

int main (int argc, char *argv[])
{

  printf("%s\n", argv[0]);

  for (int __i = 1; __i < argc; __i++)
    printf("arg %d : %s : \n", __i, argv[__i]);
  
  atrshmlog_ret_t ret = atrshmlog_attach();

  if (ret != 0)
    {
      printf("%d\n", ret);
      exit(1);
    }

  PN(atrshmlog_id);
  PN(atrshmlog_attach_pid);
  PN(atrshmlog_acquire_count);
  PN(atrshmlog_buffer_id);
  PN(atrshmlog_init_buffers_in_advance);
  PN(atrshmlog_buffer_infosize);
  PN(atrshmlog_f_list_buffer_slave);
  PN(atrshmlog_prealloc_buffer_count);
  PN(atrshmlog_f_list_buffer_slave_run);
  PN(atrshmlog_f_list_buffer_slave_count);
  PN(atrshmlog_f_list_buffer_slave_wait);
  PN(atrshmlog_slave_to_shm_wait);
  PN(atrshmlog_strategy_wait_wait_time);
  PN(atrshmlog_wait_for_slaves);
  PN(atrshmlog_buffer_strategy);
  PN(atrshmlog_autoflush);
  PN(atrshmlog_checksum);
  PN(atrshmlog_thread_fence_1);
  PN(atrshmlog_thread_fence_2);
  PN(atrshmlog_thread_fence_3);
  PN(atrshmlog_thread_fence_4);
  PN(atrshmlog_thread_fence_5);
  PN(atrshmlog_thread_fence_6);
  PN(atrshmlog_thread_fence_7);
  PN(atrshmlog_thread_fence_8);
  PN(atrshmlog_thread_fence_9);
  PN(atrshmlog_thread_fence_10);
  PN(atrshmlog_thread_fence_11);
  PN(atrshmlog_thread_fence_12);
  PN(atrshmlog_thread_fence_13);
  PN(atrshmlog_transfer_to_shm_time);
  PN(atrshmlog_logging_process_off_final);
  PN(atrshmlog_inittimetsc_before);
  PN(atrshmlog_inittimetsc_after);
  PN(atrshmlog_inittime.tv_sec);
  PN(atrshmlog_inittime.tv_nsec);
  PN(atrshmlog_delimiter);
  PN(atrshmlog_event_locks_max);
  PN(atrshmlog_attach_once);
  PN(atomic_load(&atrshmlog_last_mem_to_shm));
  PN(atrshmlog_logging_process);
  PN(atrshmlog_clock_id);
  
  PP(atrshmlog_base_ptr);
  PP(atomic_load(&atrshmlog_tps));
  PP(atomic_load(&atrshmlog_tpa));
  PP(atomic_load(&atrshmlog_tpf));
  PP(atrshmlog_event_locks);
  
  
  PS(atrshmlog_prefix_name_buffer);
    
  PN(ATRSHMLOGVERSION);
  PN(ATRSHMLOGMINORVERSION);
  PN(ATRSHMLOGPATCHVERSION);
  PN(ATRSHMLOGBUFFER_INFOSIZE);
  PN(ATRSHMLOGBUFFER_PREALLOCED_COUNT);
  PN(ATRSHMLOGBUFFER_MINCOUNT);
  PN(ATRSHMLOGBUFFER_MAXCOUNT);
  PN(ATRSHMLOG_ACCESS);
  PN(ATRSHMLOG_INIT_BUFFER_SLAVE_WAIT);
  PN(ATRSHMLOG_INIT_SLAVE_TO_SHM_WAIT);
  PN(ATRSHMLOG_STRATEGY_WAIT_WAIT_TIME_DEFAULT);
  PN(ATRSHMLOG_TRANSFER_TO_SHM_START_DEFAULT);
  PN(ATRSHMLOG_INIT_PREALLOC_COUNT);
  PN(ATRSHMLOG_MAX_PREALLOC_COUNT);
  PN(ATRSHMLOGARGVBUFFERLEN);
  PN(ATRSHMLOGEVENTCOUNTMAXLIMIT);
  PN(ATRSHMLOGBUFFER_INFOSIZE_MIN);
  PN(ATRSHMLOG_INIT_PREALLOC_COUNT_MIN);
  PN(ATRSHMLOG_INIT_BUFFER_SLAVE_WAIT_MIN);
  PN(ATRSHMLOG_INIT_BUFFER_SLAVE_WAIT_MAX);
  PN(ATRSHMLOG_SLAVE_COUNT_MIN);
  PN(ATRSHMLOG_SLAVE_COUNT_MAX);
  PN(ATRSHMLOG_CLOCK_ID_MIN);
  PN(ATRSHMLOG_CLOCK_ID_MAX);
  PN(ATRSHMLOGEVENTCOUNT);
  PN(ATRSHMLOGTARGETBUFFERMAX);
  PN(ATRSHMLOGINITINADVANCEDEFAULT);
  PN(ATRSHMLOGCONTROLDATASIZE);
  PN(ATRSHMLOG_PUTENV_IN_ATTACH);

  PN(ATRSHMLOG_PLATFORM_LINUX_X86_64_GCC);
  PN(ATRSHMLOG_PLATFORM_CYGWIN_X86_64_GCC);
  PN(ATRSHMLOG_PLATFORM_MINGW_X86_64_GCC);
  PN(ATRSHMLOG_PLATFORM_BSD_AMD64_CLANG);
  PN(ATRSHMLOG_PLATFORM_BSD_AMD64_GCC);
  PN(ATRSHMLOG_PLATFORM_SOLARIS_X86_64_GCC);
  PN(ATRSHMLOG_FLAVOUR);
  
  PN(ATRSHMLOG_INLINE_TSC_CODE);
  PN(ATRSHMLOG_INLINE_GETTIME);

  PN(ATRSHMLOG_USE_PTHREAD);
  PN(ATRSHMLOG_USE_WINTHREAD);
  PN(ATRSHMLOG_USE_C11_THREAD);
  PN(ATRSHMLOG_SYSCALL);
  PN(ATRSHMLOG_USE_CLOCK_GETTIME);
  PN(ATRSHMLOG_USE_WIN_FILETIME);
  PN(ATRSHMLOG_USE_GETTIMEOFDAY);
  PN(ATRSHMLOG_USE_NANOSLEEP);
  PN(ATRSHMLOG_USE_SYSCALL_TID);
  PN(ATRSHMLOG_USE_PTHREAD_TID);
  PN(ATRSHMLOG_USE_WINTHREAD_TID);
  PN(ATRSHMLOG_USE_THR_SELF_TID);
  PN(ATRSHMLOG_USE_GETTHRID);
  PN(ATRSHMLOG_USE_LWP_SELF);
  PN(ATRSHMLOG_USE_SOLARIS_THR_SELF);
  PN(ATRSHMLOG_THREAD_LOCAL);
  PN(ATRSHMLOG_USE_SAFER_COPY);

  sleep(1);

  PN(atomic_load(&atrshmlog_f_list_active_slaves));
  PP(atomic_load(&atrshmlog_tpslave));

  printf("\n");

  return 0;
}

/* end of file */
