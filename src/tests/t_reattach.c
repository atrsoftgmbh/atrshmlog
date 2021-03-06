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

/** \file t_reattach.c
 * \brief We are a test program.
 */

#include "tests.h"


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
  PN(atrshmlog_targetbuffer_max);
  
  PP(atrshmlog_base_ptr);
  PP(atomic_load(&atrshmlog_tps));
  PP(atomic_load(&atrshmlog_tpa));
  PP(atomic_load(&atrshmlog_tpf));
  PP(atrshmlog_event_locks);
  
  
  PS(atrshmlog_prefix_name_buffer);
  ATRSHMLOG_WRITE0(1, 'P', 42, 0, 0);
  
  ret = atrshmlog_detach();

  PN(ret);
  PN(atrshmlog_id);
  PN(atrshmlog_attach_once);
  
  ATRSHMLOG_WRITE0(1, 'P', 43, 0, 0);
  

  atrshmlog_int32_t p[100];

  for (int i = 0; i < 100; i++)
    p[i] = 0;

  for (int i = 1; i < argc && i < 101; i++)
    {
      int v = strtol(argv[i], NULL, 10);

      p[i - 1] = v;
    }

  ret = atrshmlog_reattach(p);

  PN(ret);

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
  PN(atrshmlog_targetbuffer_max);
  
  PP(atrshmlog_base_ptr);

  
  printf("\n");

  return 0;
}

/* end of file */
