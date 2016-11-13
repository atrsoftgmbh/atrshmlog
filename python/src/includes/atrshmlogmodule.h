#ifndef PY_ATRSHMLOGMODULE_H
#define PY_ATRSHMLOGMODULE_H 1

#ifdef __cplusplus
extern "C" { 
#endif

  
/*
 * the python module for the log
 */

/** \file atrshmlogmodule.h
 * 
 */


/**
 * we have a linux, 64 bit 
 */
#if ATRSHMLOG_PYTHON_PLATFORM == linux
  typedef unsigned long long pyatrshmlog_ull_t;
  typedef unsigned long pyatrshmlog_uint64_t;
  typedef int pyatrshmlog_int32_t;
#endif

  /**
   * We hav a cygwin 64 bit
   */
#if ATRSHMLOG_PYTHON_PLATFORM == cygwin
typedef unsigned long long pyatrshmlog_ull_t;
typedef unsigned long pyatrshmlog_uint64_t;
typedef int pyatrshmlog_int32_t;
#endif

  /**
   * We have a mingw 64 bit 
   */
#if ATRSHMLOG_PYTHON_PLATFORM == mingw
typedef unsigned long long pyatrshmlog_ull_t;
typedef unsigned long pyatrshmlog_uint64_t;
typedef int pyatrshmlog_int32_t;
#endif

  /**
   * For later 
   */
#if ATRSHMLOG_PYTHON_PLATFORM == win32
typedef unsigned __int64_t pyatrshmlog_ull_t;
typedef unsigned __int64_t pyatrshmlog_uint64_t;
typedef long pyatrshmlog_int32_t;
#endif

  /**
   * additional typedefs here 
   */
typedef const volatile void* pyatrshmlog_area_t;
typedef const volatile void* pyatrshmlog_threadlocal_t;

typedef void (*pyatrshmlog_vfctv_t)(void);
  

#define PyAtrshmlog_attach_NUM 0
#define PyAtrshmlog_attach_RETURN long
#define PyAtrshmlog_attach_PROTO (void)

#define PyAtrshmlog_gettime_NUM 1
#define PyAtrshmlog_gettime_RETURN pyatrshmlog_uint64_t
#define PyAtrshmlog_gettime_PROTO (void)

#define PyAtrshmlog_write0_NUM 2
#define PyAtrshmlog_write0_RETURN long
#define PyAtrshmlog_write0_PROTO (pyatrshmlog_int32_t eventnumber, \
				  pyatrshmlog_int32_t eventflag, \
				  pyatrshmlog_int32_t userflag, \
				  pyatrshmlog_uint64_t starttime, \
				  pyatrshmlog_uint64_t endtime)
  
#define PyAtrshmlog_write_NUM 3
#define PyAtrshmlog_write_RETURN long
#define PyAtrshmlog_write_PROTO (pyatrshmlog_int32_t eventnumber,	\
				 pyatrshmlog_int32_t eventflag, \
				 pyatrshmlog_int32_t userflag, \
				 pyatrshmlog_uint64_t starttime, \
				 pyatrshmlog_uint64_t endtime, \
				 void* payload, \
				 long size)

#define PyAtrshmlog_sleep_nanos_NUM 4
#define PyAtrshmlog_sleep_nanos_RETURN void
#define PyAtrshmlog_sleep_nanos_PROTO (pyatrshmlog_int32_t nanos)
  
#define PyAtrshmlog_get_statistics_max_index_NUM 5
#define PyAtrshmlog_get_statistics_max_index_RETURN long 
#define PyAtrshmlog_get_statistics_max_index_PROTO (void)
  
#define PyAtrshmlog_get_statistics_NUM 6
#define PyAtrshmlog_get_statistics_RETURN void
#define PyAtrshmlog_get_statistics_PROTO (pyatrshmlog_int32_t* statistics)
  
#define PyAtrshmlog_set_env_prefix_NUM 7
#define PyAtrshmlog_set_env_prefix_RETURN void
#define PyAtrshmlog_set_env_prefix_PROTO (const char* prefix)
  
#define PyAtrshmlog_get_env_prefix_NUM 8
#define PyAtrshmlog_get_env_prefix_RETURN const char *
#define PyAtrshmlog_get_env_prefix_PROTO (void)
  
#define PyAtrshmlog_get_env_NUM 9
#define PyAtrshmlog_get_env_RETURN const char *
#define PyAtrshmlog_get_env_PROTO (const char* suffix)
  
#define PyAtrshmlog_get_env_shmid_NUM 10
#define PyAtrshmlog_get_env_shmid_RETURN const char *
#define PyAtrshmlog_get_env_shmid_PROTO (void)
  
#define PyAtrshmlog_get_env_id_suffix_NUM 11
#define PyAtrshmlog_get_env_id_suffix_RETURN const char *
#define PyAtrshmlog_get_env_id_suffix_PROTO (void)
  
#define PyAtrshmlog_get_version_NUM 12
#define PyAtrshmlog_get_version_RETURN long
#define PyAtrshmlog_get_version_PROTO (void)
  
#define PyAtrshmlog_get_minor_version_NUM 13
#define PyAtrshmlog_get_minor_version_RETURN long
#define PyAtrshmlog_get_minor_version_PROTO (void)
  
#define PyAtrshmlog_get_patch_version_NUM 14
#define PyAtrshmlog_get_patch_version_RETURN long
#define PyAtrshmlog_get_patch_version_PROTO (void)
  
#define PyAtrshmlog_get_event_locks_max_NUM 15
#define PyAtrshmlog_get_event_locks_max_RETURN long
#define PyAtrshmlog_get_event_locks_max_PROTO (void)
  
#define PyAtrshmlog_set_event_locks_max_NUM 16
#define PyAtrshmlog_set_event_locks_max_RETURN long
#define PyAtrshmlog_set_event_locks_max_PROTO (pyatrshmlog_int32_t newmax)
  
#define PyAtrshmlog_get_event_NUM 17
#define PyAtrshmlog_get_event_RETURN long
#define PyAtrshmlog_get_event_PROTO (pyatrshmlog_int32_t event)
  
#define PyAtrshmlog_set_event_on_NUM 18
#define PyAtrshmlog_set_event_on_RETURN long
#define PyAtrshmlog_set_event_on_PROTO (pyatrshmlog_int32_t event)
  
#define PyAtrshmlog_set_event_off_NUM 19
#define PyAtrshmlog_set_event_off_RETURN long
#define PyAtrshmlog_set_event_off_PROTO (pyatrshmlog_int32_t event)
  
#define PyAtrshmlog_get_logging_NUM 20
#define PyAtrshmlog_get_logging_RETURN long 
#define PyAtrshmlog_get_logging_PROTO (void)
  
#define PyAtrshmlog_set_logging_process_on_NUM 21
#define PyAtrshmlog_set_logging_process_on_RETURN long
#define PyAtrshmlog_set_logging_process_on_PROTO (void)
  
#define PyAtrshmlog_set_logging_process_off_NUM 22
#define PyAtrshmlog_set_logging_process_off_RETURN long
#define PyAtrshmlog_set_logging_process_off_PROTO (void)
  
#define PyAtrshmlog_set_logging_process_off_final_NUM 23
#define PyAtrshmlog_set_logging_process_off_final_RETURN long
#define PyAtrshmlog_set_logging_process_off_final_PROTO (void)
  
#define PyAtrshmlog_get_shmid_NUM 24
#define PyAtrshmlog_get_shmid_RETURN long
#define PyAtrshmlog_get_shmid_PROTO (void)
  
#define PyAtrshmlog_get_area_NUM 25
#define PyAtrshmlog_get_area_RETURN pyatrshmlog_area_t
#define PyAtrshmlog_get_area_PROTO (void)
  
#define PyAtrshmlog_get_area_ich_habe_fertig_NUM 26
#define PyAtrshmlog_get_area_ich_habe_fertig_RETURN  long
#define PyAtrshmlog_get_area_ich_habe_fertig_PROTO (pyatrshmlog_area_t area)
   
#define PyAtrshmlog_set_area_ich_habe_fertig_NUM 27
#define PyAtrshmlog_set_area_ich_habe_fertig_RETURN long
#define PyAtrshmlog_set_area_ich_habe_fertig_PROTO (pyatrshmlog_area_t area, pyatrshmlog_int32_t flag)
  
#define PyAtrshmlog_get_area_count_NUM 28
#define PyAtrshmlog_get_area_count_RETURN long
#define PyAtrshmlog_get_area_count_PROTO (pyatrshmlog_area_t area)
  
#define PyAtrshmlog_get_area_version_NUM 29
#define PyAtrshmlog_get_area_version_RETURN long
#define PyAtrshmlog_get_area_version_PROTO (pyatrshmlog_area_t area)
  
#define PyAtrshmlog_get_buffer_max_size_NUM 30
#define PyAtrshmlog_get_buffer_max_size_RETURN long 
#define PyAtrshmlog_get_buffer_max_size_PROTO (void)
  
#define PyAtrshmlog_get_buffer_size_NUM 31 
#define PyAtrshmlog_get_buffer_size_RETURN long
#define PyAtrshmlog_get_buffer_size_PROTO (void)
  
#define PyAtrshmlog_set_buffer_size_NUM 32
#define PyAtrshmlog_set_buffer_size_RETURN long
#define PyAtrshmlog_set_buffer_size_PROTO (pyatrshmlog_int32_t size)
  
#define PyAtrshmlog_get_f_list_buffer_slave_count_NUM 33
#define PyAtrshmlog_get_f_list_buffer_slave_count_RETURN long
#define PyAtrshmlog_get_f_list_buffer_slave_count_PROTO (void)
  
#define PyAtrshmlog_set_f_list_buffer_slave_count_NUM 34
#define PyAtrshmlog_set_f_list_buffer_slave_count_RETURN long
#define PyAtrshmlog_set_f_list_buffer_slave_count_PROTO (pyatrshmlog_int32_t count)
  
#define PyAtrshmlog_get_clock_id_NUM 35
#define PyAtrshmlog_get_clock_id_RETURN long
#define PyAtrshmlog_get_clock_id_PROTO (void)
  
#define PyAtrshmlog_set_clock_id_NUM 36
#define PyAtrshmlog_set_clock_id_RETURN long
#define PyAtrshmlog_set_clock_id_PROTO (pyatrshmlog_int32_t id)
  
#define PyAtrshmlog_set_f_list_buffer_slave_run_off_NUM 37
#define PyAtrshmlog_set_f_list_buffer_slave_run_off_RETURN void
#define PyAtrshmlog_set_f_list_buffer_slave_run_off_PROTO (void)
  
#define PyAtrshmlog_set_wait_for_slaves_on_NUM 38
#define PyAtrshmlog_set_wait_for_slaves_on_RETURN long
#define PyAtrshmlog_set_wait_for_slaves_on_PROTO (void)
  
#define PyAtrshmlog_set_wait_for_slaves_off_NUM 39
#define PyAtrshmlog_set_wait_for_slaves_off_RETURN long
#define PyAtrshmlog_set_wait_for_slaves_off_PROTO (void)
  
#define PyAtrshmlog_get_wait_for_slaves_NUM 40
#define PyAtrshmlog_get_wait_for_slaves_RETURN long
#define PyAtrshmlog_get_wait_for_slaves_PROTO (void)
  
#define PyAtrshmlog_get_f_list_buffer_slave_wait_NUM 41
#define PyAtrshmlog_get_f_list_buffer_slave_wait_RETURN long
#define PyAtrshmlog_get_f_list_buffer_slave_wait_PROTO (void) 
  
#define PyAtrshmlog_set_f_list_buffer_slave_wait_NUM 42
#define PyAtrshmlog_set_f_list_buffer_slave_wait_RETURN long
#define PyAtrshmlog_set_f_list_buffer_slave_wait_PROTO (pyatrshmlog_int32_t nanos)
  
#define PyAtrshmlog_get_acquire_count_NUM 43
#define PyAtrshmlog_get_acquire_count_RETURN long
#define PyAtrshmlog_get_acquire_count_PROTO (void)
  
#define PyAtrshmlog_get_prealloc_buffer_count_NUM 44
#define PyAtrshmlog_get_prealloc_buffer_count_RETURN long
#define PyAtrshmlog_get_prealloc_buffer_count_PROTO (void)
  
#define PyAtrshmlog_set_prealloc_buffer_count_NUM 45
#define PyAtrshmlog_set_prealloc_buffer_count_RETURN long
#define PyAtrshmlog_set_prealloc_buffer_count_PROTO (pyatrshmlog_int32_t count)
  
#define PyAtrshmlog_get_inittime_NUM 46 
#define PyAtrshmlog_get_inittime_RETURN void 
#define PyAtrshmlog_get_inittime_PROTO (pyatrshmlog_uint64_t* seconds, pyatrshmlog_int32_t* nanos)
  
#define PyAtrshmlog_get_inittime_tsc_before_NUM 47
#define PyAtrshmlog_get_inittime_tsc_before_RETURN pyatrshmlog_uint64_t
#define PyAtrshmlog_get_inittime_tsc_before_PROTO (void)
  
#define PyAtrshmlog_get_inittime_tsc_after_NUM 48
#define PyAtrshmlog_get_inittime_tsc_after_RETURN  pyatrshmlog_uint64_t
#define PyAtrshmlog_get_inittime_tsc_after_PROTO (void)
  
#define PyAtrshmlog_get_buffer_id_NUM 49
#define PyAtrshmlog_get_buffer_id_RETURN long
#define PyAtrshmlog_get_buffer_id_PROTO (void)
  
#define PyAtrshmlog_stop_NUM 50
#define PyAtrshmlog_stop_RETURN void
#define PyAtrshmlog_stop_PROTO (void)
  
#define PyAtrshmlog_flush_NUM 51
#define PyAtrshmlog_flush_RETURN void
#define PyAtrshmlog_flush_PROTO (void)
  
#define PyAtrshmlog_set_strategy_NUM 52
#define PyAtrshmlog_set_strategy_RETURN long
#define PyAtrshmlog_set_strategy_PROTO (pyatrshmlog_int32_t strategy)
  
#define PyAtrshmlog_get_strategy_NUM 53
#define PyAtrshmlog_get_strategy_RETURN long
#define PyAtrshmlog_get_strategy_PROTO (void)
  
#define PyAtrshmlog_set_strategy_process_NUM 54
#define PyAtrshmlog_set_strategy_process_RETURN long
#define PyAtrshmlog_set_strategy_process_PROTO (pyatrshmlog_int32_t strategy)
  
#define PyAtrshmlog_get_strategy_process_NUM 55
#define PyAtrshmlog_get_strategy_process_RETURN long
#define PyAtrshmlog_get_strategy_process_PROTO (void)
  
#define PyAtrshmlog_create_slave_NUM 56
#define PyAtrshmlog_create_slave_RETURN int
#define PyAtrshmlog_create_slave_PROTO (void)
  
#define PyAtrshmlog_decrement_slave_count_NUM 57
#define PyAtrshmlog_decrement_slave_count_RETURN long
#define PyAtrshmlog_decrement_slave_count_PROTO (void)
  
#define PyAtrshmlog_get_clicktime_NUM 58
#define PyAtrshmlog_get_clicktime_RETURN pyatrshmlog_uint64_t
#define PyAtrshmlog_get_clicktime_PROTO (void)
  
#define PyAtrshmlog_set_thread_fence_1_NUM 59
#define PyAtrshmlog_set_thread_fence_1_RETURN long
#define PyAtrshmlog_set_thread_fence_1_PROTO (pyatrshmlog_int32_t flag)
  
#define PyAtrshmlog_set_thread_fence_2_NUM 60
#define PyAtrshmlog_set_thread_fence_2_RETURN long 
#define PyAtrshmlog_set_thread_fence_2_PROTO (pyatrshmlog_int32_t flag)
  
#define PyAtrshmlog_set_thread_fence_3_NUM 61
#define PyAtrshmlog_set_thread_fence_3_RETURN long
#define PyAtrshmlog_set_thread_fence_3_PROTO (pyatrshmlog_int32_t flag)
  
#define PyAtrshmlog_set_thread_fence_4_NUM 62
#define PyAtrshmlog_set_thread_fence_4_RETURN long
#define PyAtrshmlog_set_thread_fence_4_PROTO (pyatrshmlog_int32_t flag) 
  
#define PyAtrshmlog_set_thread_fence_5_NUM 63
#define PyAtrshmlog_set_thread_fence_5_RETURN long
#define PyAtrshmlog_set_thread_fence_5_PROTO (pyatrshmlog_int32_t flag) 
  
#define PyAtrshmlog_set_thread_fence_6_NUM 64
#define PyAtrshmlog_set_thread_fence_6_RETURN long
#define PyAtrshmlog_set_thread_fence_6_PROTO (pyatrshmlog_int32_t flag) 
  
#define PyAtrshmlog_set_thread_fence_7_NUM 65
#define PyAtrshmlog_set_thread_fence_7_RETURN long
#define PyAtrshmlog_set_thread_fence_7_PROTO (pyatrshmlog_int32_t flag) 
  
#define PyAtrshmlog_set_thread_fence_8_NUM 66
#define PyAtrshmlog_set_thread_fence_8_RETURN long
#define PyAtrshmlog_set_thread_fence_8_PROTO (pyatrshmlog_int32_t flag) 
  
#define PyAtrshmlog_set_thread_fence_9_NUM 67
#define PyAtrshmlog_set_thread_fence_9_RETURN long
#define PyAtrshmlog_set_thread_fence_9_PROTO (pyatrshmlog_int32_t flag) 
  
#define PyAtrshmlog_set_thread_fence_10_NUM 68
#define PyAtrshmlog_set_thread_fence_10_RETURN long 
#define PyAtrshmlog_set_thread_fence_10_PROTO (pyatrshmlog_int32_t flag) 
  
#define PyAtrshmlog_set_thread_fence_11_NUM 69
#define PyAtrshmlog_set_thread_fence_11_RETURN long 
#define PyAtrshmlog_set_thread_fence_11_PROTO (pyatrshmlog_int32_t flag) 
  
#define PyAtrshmlog_set_thread_fence_12_NUM 70
#define PyAtrshmlog_set_thread_fence_12_RETURN long 
#define PyAtrshmlog_set_thread_fence_12_PROTO (pyatrshmlog_int32_t flag) 
  
#define PyAtrshmlog_set_thread_fence_13_NUM 71
#define PyAtrshmlog_set_thread_fence_13_RETURN long 
#define PyAtrshmlog_set_thread_fence_13_PROTO (pyatrshmlog_int32_t flag)
  
#define PyAtrshmlog_get_thread_fence_1_NUM 72
#define PyAtrshmlog_get_thread_fence_1_RETURN long
#define PyAtrshmlog_get_thread_fence_1_PROTO (void)
  
#define PyAtrshmlog_get_thread_fence_2_NUM 73
#define PyAtrshmlog_get_thread_fence_2_RETURN long
#define PyAtrshmlog_get_thread_fence_2_PROTO (void)
  
#define PyAtrshmlog_get_thread_fence_3_NUM 74
#define PyAtrshmlog_get_thread_fence_3_RETURN long
#define PyAtrshmlog_get_thread_fence_3_PROTO (void)
  
#define PyAtrshmlog_get_thread_fence_4_NUM 75
#define PyAtrshmlog_get_thread_fence_4_RETURN long
#define PyAtrshmlog_get_thread_fence_4_PROTO (void)
  
#define PyAtrshmlog_get_thread_fence_5_NUM 76
#define PyAtrshmlog_get_thread_fence_5_RETURN long
#define PyAtrshmlog_get_thread_fence_5_PROTO (void) 
  
#define PyAtrshmlog_get_thread_fence_6_NUM 77
#define PyAtrshmlog_get_thread_fence_6_RETURN long 
#define PyAtrshmlog_get_thread_fence_6_PROTO (void)
  
#define PyAtrshmlog_get_thread_fence_7_NUM 78
#define PyAtrshmlog_get_thread_fence_7_RETURN long
#define PyAtrshmlog_get_thread_fence_7_PROTO (void)
  
#define PyAtrshmlog_get_thread_fence_8_NUM 79
#define PyAtrshmlog_get_thread_fence_8_RETURN long 
#define PyAtrshmlog_get_thread_fence_8_PROTO (void)
  
#define PyAtrshmlog_get_thread_fence_9_NUM 80
#define PyAtrshmlog_get_thread_fence_9_RETURN long
#define PyAtrshmlog_get_thread_fence_9_PROTO (void)
  
#define PyAtrshmlog_get_thread_fence_10_NUM 81
#define PyAtrshmlog_get_thread_fence_10_RETURN long
#define PyAtrshmlog_get_thread_fence_10_PROTO (void)
  
#define PyAtrshmlog_get_thread_fence_11_NUM 82
#define PyAtrshmlog_get_thread_fence_11_RETURN long
#define PyAtrshmlog_get_thread_fence_11_PROTO (void)
  
#define PyAtrshmlog_get_thread_fence_12_NUM 83
#define PyAtrshmlog_get_thread_fence_12_RETURN long
#define PyAtrshmlog_get_thread_fence_12_PROTO (void)
  
#define PyAtrshmlog_get_thread_fence_13_NUM 84
#define PyAtrshmlog_get_thread_fence_13_RETURN long
#define PyAtrshmlog_get_thread_fence_13_PROTO (void)
  
#define PyAtrshmlog_get_realtime_NUM 85
#define PyAtrshmlog_get_realtime_RETURN void
#define PyAtrshmlog_get_realtime_PROTO (pyatrshmlog_uint64_t* seconds, pyatrshmlog_int32_t* nanos)
  
#define PyAtrshmlog_get_thread_locals_adress_NUM 86
#define PyAtrshmlog_get_thread_locals_adress_RETURN pyatrshmlog_threadlocal_t
#define PyAtrshmlog_get_thread_locals_adress_PROTO (void)
  
#define PyAtrshmlog_get_tid_NUM 87
#define PyAtrshmlog_get_tid_RETURN pyatrshmlog_uint64_t
#define PyAtrshmlog_get_tid_PROTO (void)
  
#define PyAtrshmlog_turn_logging_off_NUM 88
#define PyAtrshmlog_turn_logging_off_RETURN void
#define PyAtrshmlog_turn_logging_off_PROTO  (pyatrshmlog_threadlocal_t tl)
  
#define PyAtrshmlog_set_init_buffers_in_advance_on_NUM 89
#define PyAtrshmlog_set_init_buffers_in_advance_on_RETURN long
#define PyAtrshmlog_set_init_buffers_in_advance_on_PROTO (void)
  
#define PyAtrshmlog_set_init_buffers_in_advance_off_NUM 90
#define PyAtrshmlog_set_init_buffers_in_advance_off_RETURN long
#define PyAtrshmlog_set_init_buffers_in_advance_off_PROTO (void)
  
#define PyAtrshmlog_get_init_buffers_in_advance_NUM 91
#define PyAtrshmlog_get_init_buffers_in_advance_RETURN long 
#define PyAtrshmlog_get_init_buffers_in_advance_PROTO (void)
  
#define PyAtrshmlog_get_next_slave_local_NUM 92
#define PyAtrshmlog_get_next_slave_local_RETURN  pyatrshmlog_threadlocal_t
#define PyAtrshmlog_get_next_slave_local_PROTO  (pyatrshmlog_threadlocal_t tl)
  
#define PyAtrshmlog_get_thread_local_tid_NUM 93
#define PyAtrshmlog_get_thread_local_tid_RETURN pyatrshmlog_uint64_t
#define PyAtrshmlog_get_thread_local_tid_PROTO  (pyatrshmlog_threadlocal_t tl)
  
#define PyAtrshmlog_remove_slave_via_local_NUM 94
#define PyAtrshmlog_remove_slave_via_local_RETURN long
#define PyAtrshmlog_remove_slave_via_local_PROTO  (pyatrshmlog_threadlocal_t tl)

#define PyAtrshmlog_reuse_thread_buffers_NUM 95
#define PyAtrshmlog_reuse_thread_buffers_RETURN int 
#define PyAtrshmlog_reuse_thread_buffers_PROTO  (pyatrshmlog_uint64_t tid)
  
#define PyAtrshmlog_read_NUM 96
#define PyAtrshmlog_read_RETURN long
#define PyAtrshmlog_read_PROTO (pyatrshmlog_area_t area, \
				pyatrshmlog_int32_t index, \
				void *buff,		      \
				pyatrshmlog_int32_t *length,		      \
				pyatrshmlog_uint64_t *pid,		      \
				pyatrshmlog_uint64_t *tid ,			\
				pyatrshmlog_uint64_t* inittime_seconds,	\
				pyatrshmlog_int32_t* inittime_nanos,		   \
				pyatrshmlog_uint64_t* inittimetsc_before,	   \
				pyatrshmlog_uint64_t* inittimetsc_after,	   \
				pyatrshmlog_uint64_t* lasttime_seconds,		\
				pyatrshmlog_int32_t* lasttime_nanos,			\
				pyatrshmlog_uint64_t* lasttimetsc_before,		\
				pyatrshmlog_uint64_t* lasttimetsc_after,		\
				pyatrshmlog_uint64_t* difftimetransfer,		\
				pyatrshmlog_uint64_t* starttransfer,			\
				pyatrshmlog_uint64_t* acquiretime,			\
				pyatrshmlog_int32_t* id,				\
				pyatrshmlog_int32_t* number_dispatched,		\
				pyatrshmlog_int32_t* counter_write0,			\
				pyatrshmlog_int32_t* counter_write0_discard,		\
				pyatrshmlog_int32_t* counter_write0_wait,		\
				pyatrshmlog_int32_t* counter_write0_adaptive,		\
				pyatrshmlog_int32_t* counter_write0_adaptive_fast,	\
				pyatrshmlog_int32_t* counter_write0_adaptive_very_fast, \
				pyatrshmlog_int32_t* counter_write1,			\
				pyatrshmlog_int32_t* counter_write1_discard,		\
				pyatrshmlog_int32_t* counter_write1_wait,		\
				pyatrshmlog_int32_t* counter_write1_adaptive,		\
				pyatrshmlog_int32_t* counter_write1_adaptive_fast,	\
				pyatrshmlog_int32_t* counter_write1_adaptive_very_fast, \
				pyatrshmlog_int32_t* counter_write2,			\
				pyatrshmlog_int32_t* counter_write2_discard,		\
				pyatrshmlog_int32_t* counter_write2_wait,		\
				pyatrshmlog_int32_t* counter_write2_adaptive,		\
				pyatrshmlog_int32_t* counter_write2_adaptive_fast,	\
				pyatrshmlog_int32_t* counter_write2_adaptive_very_fast)

#define PyAtrshmlog_read_fetch_NUM 97
#define PyAtrshmlog_read_fetch_RETURN long
#define PyAtrshmlog_read_fetch_PROTO  (pyatrshmlog_area_t area, \
				       pyatrshmlog_int32_t *index, \
				       void *buff, \
				       pyatrshmlog_int32_t *length,  \
				       pyatrshmlog_uint64_t *pid, \
				       pyatrshmlog_uint64_t *tid ,  \
				       pyatrshmlog_uint64_t* inittime_seconds, \
				       pyatrshmlog_int32_t* inittime_nanos, \
				       pyatrshmlog_uint64_t* inittimetsc_before,  \
				       pyatrshmlog_uint64_t* inittimetsc_after, \
				       pyatrshmlog_uint64_t* lasttime_seconds, \
				       pyatrshmlog_int32_t* lasttime_nanos,	\
				       pyatrshmlog_uint64_t* lasttimetsc_before, \
				       pyatrshmlog_uint64_t* lasttimetsc_after,	\
				       pyatrshmlog_uint64_t* difftimetransfer, \
				       pyatrshmlog_uint64_t* starttransfer, \
				       pyatrshmlog_uint64_t* acquiretime, \
				       pyatrshmlog_int32_t* id, \
				       pyatrshmlog_int32_t* number_dispatched, \
				       pyatrshmlog_int32_t* counter_write0, \
				       pyatrshmlog_int32_t* counter_write0_discard, \
				       pyatrshmlog_int32_t* counter_write0_wait, \
				       pyatrshmlog_int32_t* counter_write0_adaptive, \
				       pyatrshmlog_int32_t* counter_write0_adaptive_fast, \
				       pyatrshmlog_int32_t* counter_write0_adaptive_very_fast, \
				       pyatrshmlog_int32_t* counter_write1, \
				       pyatrshmlog_int32_t* counter_write1_discard, \
				       pyatrshmlog_int32_t* counter_write1_wait, \
				       pyatrshmlog_int32_t* counter_write1_adaptive, \
				       pyatrshmlog_int32_t* counter_write1_adaptive_fast, \
				       pyatrshmlog_int32_t* counter_write1_adaptive_very_fast, \
				       pyatrshmlog_int32_t* counter_write2, \
				       pyatrshmlog_int32_t* counter_write2_discard, \
				       pyatrshmlog_int32_t* counter_write2_wait, \
				       pyatrshmlog_int32_t* counter_write2_adaptive, \
				       pyatrshmlog_int32_t* counter_write2_adaptive_fast, \
				       pyatrshmlog_int32_t* counter_write2_adaptive_very_fast)

  
  
#define PyAtrshmlog_verify_NUM 98
#define PyAtrshmlog_verify_RETURN long
#define PyAtrshmlog_verify_PROTO (void)
  
#define PyAtrshmlog_create_NUM 99
#define PyAtrshmlog_create_RETURN long
#define PyAtrshmlog_create_PROTO (pyatrshmlog_int32_t ipckey, pyatrshmlog_int32_t count)
  
#define PyAtrshmlog_delete_NUM 100
#define PyAtrshmlog_delete_RETURN long
#define PyAtrshmlog_delete_PROTO (pyatrshmlog_int32_t id)
  
#define PyAtrshmlog_cleanup_locks_NUM 101
#define PyAtrshmlog_cleanup_locks_RETURN void
#define PyAtrshmlog_cleanup_locks_PROTO  (pyatrshmlog_area_t area)
  
#define PyAtrshmlog_init_shm_log_NUM 102
#define PyAtrshmlog_init_shm_log_RETURN long
#define PyAtrshmlog_init_shm_log_PROTO  (pyatrshmlog_area_t area, pyatrshmlog_int32_t count)
  
#define PyAtrshmlog_poke_NUM 103
#define PyAtrshmlog_poke_RETURN long
#define PyAtrshmlog_poke_PROTO  (pyatrshmlog_area_t area, pyatrshmlog_int32_t index, pyatrshmlog_int32_t value)
  
#define PyAtrshmlog_peek_NUM 104
#define PyAtrshmlog_peek_RETURN long
#define PyAtrshmlog_peek_PROTO  (pyatrshmlog_area_t area, pyatrshmlog_int32_t index)
  

/* Total number of C API pointers */

#define PyAtrshmlog_API_pointers 105



  /* we have now the static prototypes or for the capsule use the
   * function call sequences 
   */
  
#ifdef ATRSHMLOG_MODULE

  static PyAtrshmlog_attach_RETURN PyAtrshmlog_attach PyAtrshmlog_attach_PROTO;
  static PyAtrshmlog_gettime_RETURN PyAtrshmlog_gettime PyAtrshmlog_gettime_PROTO;

  static PyAtrshmlog_write0_RETURN PyAtrshmlog_write0 PyAtrshmlog_write0_PROTO;
  static PyAtrshmlog_write_RETURN PyAtrshmlog_write PyAtrshmlog_write_PROTO;
  static PyAtrshmlog_sleep_nanos_RETURN PyAtrshmlog_sleep_nanos PyAtrshmlog_sleep_nanos_PROTO;
  static PyAtrshmlog_get_statistics_max_index_RETURN PyAtrshmlog_get_statistics_max_index PyAtrshmlog_get_statistics_max_index_PROTO;
  static PyAtrshmlog_get_statistics_RETURN PyAtrshmlog_get_statistics PyAtrshmlog_get_statistics_PROTO;
  static PyAtrshmlog_set_env_prefix_RETURN PyAtrshmlog_set_env_prefix PyAtrshmlog_set_env_prefix_PROTO;
  static PyAtrshmlog_get_env_prefix_RETURN PyAtrshmlog_get_env_prefix PyAtrshmlog_get_env_prefix_PROTO;
  static PyAtrshmlog_get_env_RETURN PyAtrshmlog_get_env PyAtrshmlog_get_env_PROTO;
  static PyAtrshmlog_get_env_shmid_RETURN PyAtrshmlog_get_env_shmid PyAtrshmlog_get_env_shmid_PROTO;
  static PyAtrshmlog_get_env_id_suffix_RETURN PyAtrshmlog_get_env_id_suffix PyAtrshmlog_get_env_id_suffix_PROTO;
  static PyAtrshmlog_get_version_RETURN PyAtrshmlog_get_version PyAtrshmlog_get_version_PROTO;
  static PyAtrshmlog_get_minor_version_RETURN PyAtrshmlog_get_minor_version PyAtrshmlog_get_minor_version_PROTO;
  static PyAtrshmlog_get_patch_version_RETURN PyAtrshmlog_get_patch_version PyAtrshmlog_get_patch_version_PROTO;
  static PyAtrshmlog_get_event_locks_max_RETURN PyAtrshmlog_get_event_locks_max PyAtrshmlog_get_event_locks_max_PROTO;
  static PyAtrshmlog_set_event_locks_max_RETURN PyAtrshmlog_set_event_locks_max PyAtrshmlog_set_event_locks_max_PROTO;
  static PyAtrshmlog_get_event_RETURN PyAtrshmlog_get_event PyAtrshmlog_get_event_PROTO;
  static PyAtrshmlog_set_event_on_RETURN PyAtrshmlog_set_event_on PyAtrshmlog_set_event_on_PROTO;
  static PyAtrshmlog_set_event_off_RETURN PyAtrshmlog_set_event_off PyAtrshmlog_set_event_off_PROTO;
  static PyAtrshmlog_get_logging_RETURN PyAtrshmlog_get_logging PyAtrshmlog_get_logging_PROTO;
  static PyAtrshmlog_set_logging_process_on_RETURN PyAtrshmlog_set_logging_process_on PyAtrshmlog_set_logging_process_on_PROTO;
  static PyAtrshmlog_set_logging_process_off_RETURN PyAtrshmlog_set_logging_process_off PyAtrshmlog_set_logging_process_off_PROTO;
  static PyAtrshmlog_set_logging_process_off_final_RETURN PyAtrshmlog_set_logging_process_off_final PyAtrshmlog_set_logging_process_off_final_PROTO;
  static PyAtrshmlog_get_shmid_RETURN PyAtrshmlog_get_shmid PyAtrshmlog_get_shmid_PROTO;
  static PyAtrshmlog_get_area_RETURN PyAtrshmlog_get_area PyAtrshmlog_get_area_PROTO;
  static PyAtrshmlog_get_area_ich_habe_fertig_RETURN PyAtrshmlog_get_area_ich_habe_fertig PyAtrshmlog_get_area_ich_habe_fertig_PROTO;
  static PyAtrshmlog_set_area_ich_habe_fertig_RETURN PyAtrshmlog_set_area_ich_habe_fertig PyAtrshmlog_set_area_ich_habe_fertig_PROTO;
  static PyAtrshmlog_get_area_count_RETURN PyAtrshmlog_get_area_count PyAtrshmlog_get_area_count_PROTO;
  static PyAtrshmlog_get_area_version_RETURN PyAtrshmlog_get_area_version PyAtrshmlog_get_area_version_PROTO;
  static PyAtrshmlog_get_buffer_max_size_RETURN PyAtrshmlog_get_buffer_max_size PyAtrshmlog_get_buffer_max_size_PROTO;
  static PyAtrshmlog_get_buffer_size_RETURN PyAtrshmlog_get_buffer_size PyAtrshmlog_get_buffer_size_PROTO;
  static PyAtrshmlog_get_f_list_buffer_slave_count_RETURN PyAtrshmlog_get_f_list_buffer_slave_count PyAtrshmlog_get_f_list_buffer_slave_count_PROTO;
  static PyAtrshmlog_set_buffer_size_RETURN PyAtrshmlog_set_buffer_size PyAtrshmlog_set_buffer_size_PROTO;
  static PyAtrshmlog_set_f_list_buffer_slave_count_RETURN PyAtrshmlog_set_f_list_buffer_slave_count PyAtrshmlog_set_f_list_buffer_slave_count_PROTO;
  static PyAtrshmlog_get_clock_id_RETURN PyAtrshmlog_get_clock_id PyAtrshmlog_get_clock_id_PROTO;
  static PyAtrshmlog_set_clock_id_RETURN PyAtrshmlog_set_clock_id PyAtrshmlog_set_clock_id_PROTO;
  static PyAtrshmlog_set_f_list_buffer_slave_run_off_RETURN PyAtrshmlog_set_f_list_buffer_slave_run_off PyAtrshmlog_set_f_list_buffer_slave_run_off_PROTO;
  static PyAtrshmlog_set_wait_for_slaves_on_RETURN PyAtrshmlog_set_wait_for_slaves_on PyAtrshmlog_set_wait_for_slaves_on_PROTO;
  static PyAtrshmlog_set_wait_for_slaves_off_RETURN PyAtrshmlog_set_wait_for_slaves_off PyAtrshmlog_set_wait_for_slaves_off_PROTO;
  static PyAtrshmlog_get_wait_for_slaves_RETURN PyAtrshmlog_get_wait_for_slaves PyAtrshmlog_get_wait_for_slaves_PROTO;
  
  static PyAtrshmlog_get_f_list_buffer_slave_wait_RETURN PyAtrshmlog_get_f_list_buffer_slave_wait PyAtrshmlog_get_f_list_buffer_slave_wait_PROTO;
  static PyAtrshmlog_set_f_list_buffer_slave_wait_RETURN PyAtrshmlog_set_f_list_buffer_slave_wait PyAtrshmlog_set_f_list_buffer_slave_wait_PROTO;
  static PyAtrshmlog_get_acquire_count_RETURN PyAtrshmlog_get_acquire_count PyAtrshmlog_get_acquire_count_PROTO;
  static PyAtrshmlog_get_prealloc_buffer_count_RETURN PyAtrshmlog_get_prealloc_buffer_count PyAtrshmlog_get_prealloc_buffer_count_PROTO;
  static PyAtrshmlog_set_prealloc_buffer_count_RETURN PyAtrshmlog_set_prealloc_buffer_count PyAtrshmlog_set_prealloc_buffer_count_PROTO;
  static PyAtrshmlog_get_inittime_RETURN PyAtrshmlog_get_inittime PyAtrshmlog_get_inittime_PROTO;
  static PyAtrshmlog_get_inittime_tsc_before_RETURN PyAtrshmlog_get_inittime_tsc_before PyAtrshmlog_get_inittime_tsc_before_PROTO;
  static PyAtrshmlog_get_inittime_tsc_after_RETURN PyAtrshmlog_get_inittime_tsc_after PyAtrshmlog_get_inittime_tsc_after_PROTO;
  static PyAtrshmlog_get_buffer_id_RETURN PyAtrshmlog_get_buffer_id PyAtrshmlog_get_buffer_id_PROTO;
  static PyAtrshmlog_stop_RETURN PyAtrshmlog_stop PyAtrshmlog_stop_PROTO;
  static PyAtrshmlog_flush_RETURN PyAtrshmlog_flush PyAtrshmlog_flush_PROTO;
  static PyAtrshmlog_set_strategy_RETURN PyAtrshmlog_set_strategy PyAtrshmlog_set_strategy_PROTO;
  static PyAtrshmlog_get_strategy_RETURN PyAtrshmlog_get_strategy PyAtrshmlog_get_strategy_PROTO;
  static PyAtrshmlog_set_strategy_process_RETURN PyAtrshmlog_set_strategy_process PyAtrshmlog_set_strategy_process_PROTO;
  static PyAtrshmlog_get_strategy_process_RETURN PyAtrshmlog_get_strategy_process PyAtrshmlog_get_strategy_process_PROTO;
  static PyAtrshmlog_create_slave_RETURN PyAtrshmlog_create_slave PyAtrshmlog_create_slave_PROTO;
  static PyAtrshmlog_decrement_slave_count_RETURN PyAtrshmlog_decrement_slave_count PyAtrshmlog_decrement_slave_count_PROTO;
  static PyAtrshmlog_get_clicktime_RETURN PyAtrshmlog_get_clicktime PyAtrshmlog_get_clicktime_PROTO;
  static PyAtrshmlog_set_thread_fence_1_RETURN PyAtrshmlog_set_thread_fence_1 PyAtrshmlog_set_thread_fence_1_PROTO;
  static PyAtrshmlog_set_thread_fence_2_RETURN PyAtrshmlog_set_thread_fence_2 PyAtrshmlog_set_thread_fence_2_PROTO;
  static PyAtrshmlog_set_thread_fence_3_RETURN PyAtrshmlog_set_thread_fence_3 PyAtrshmlog_set_thread_fence_3_PROTO;
  static PyAtrshmlog_set_thread_fence_4_RETURN PyAtrshmlog_set_thread_fence_4 PyAtrshmlog_set_thread_fence_4_PROTO;
  static PyAtrshmlog_set_thread_fence_5_RETURN PyAtrshmlog_set_thread_fence_5 PyAtrshmlog_set_thread_fence_5_PROTO;
  static PyAtrshmlog_set_thread_fence_6_RETURN PyAtrshmlog_set_thread_fence_6 PyAtrshmlog_set_thread_fence_6_PROTO;
  static PyAtrshmlog_set_thread_fence_7_RETURN PyAtrshmlog_set_thread_fence_7 PyAtrshmlog_set_thread_fence_7_PROTO;
  static PyAtrshmlog_set_thread_fence_8_RETURN PyAtrshmlog_set_thread_fence_8 PyAtrshmlog_set_thread_fence_8_PROTO;
  static PyAtrshmlog_set_thread_fence_9_RETURN PyAtrshmlog_set_thread_fence_9 PyAtrshmlog_set_thread_fence_9_PROTO;
  static PyAtrshmlog_set_thread_fence_10_RETURN PyAtrshmlog_set_thread_fence_10 PyAtrshmlog_set_thread_fence_10_PROTO;
  static PyAtrshmlog_set_thread_fence_11_RETURN PyAtrshmlog_set_thread_fence_11 PyAtrshmlog_set_thread_fence_11_PROTO;
  static PyAtrshmlog_set_thread_fence_12_RETURN PyAtrshmlog_set_thread_fence_12 PyAtrshmlog_set_thread_fence_12_PROTO;
  static PyAtrshmlog_set_thread_fence_13_RETURN PyAtrshmlog_set_thread_fence_13 PyAtrshmlog_set_thread_fence_13_PROTO;
  static PyAtrshmlog_get_thread_fence_1_RETURN PyAtrshmlog_get_thread_fence_1 PyAtrshmlog_get_thread_fence_1_PROTO;
  static PyAtrshmlog_get_thread_fence_2_RETURN PyAtrshmlog_get_thread_fence_2 PyAtrshmlog_get_thread_fence_2_PROTO;
  static PyAtrshmlog_get_thread_fence_3_RETURN PyAtrshmlog_get_thread_fence_3 PyAtrshmlog_get_thread_fence_3_PROTO;
  static PyAtrshmlog_get_thread_fence_4_RETURN PyAtrshmlog_get_thread_fence_4 PyAtrshmlog_get_thread_fence_4_PROTO;
  static PyAtrshmlog_get_thread_fence_5_RETURN PyAtrshmlog_get_thread_fence_5 PyAtrshmlog_get_thread_fence_5_PROTO;
  static PyAtrshmlog_get_thread_fence_6_RETURN PyAtrshmlog_get_thread_fence_6 PyAtrshmlog_get_thread_fence_6_PROTO;
  static PyAtrshmlog_get_thread_fence_7_RETURN PyAtrshmlog_get_thread_fence_7 PyAtrshmlog_get_thread_fence_7_PROTO;
  static PyAtrshmlog_get_thread_fence_8_RETURN PyAtrshmlog_get_thread_fence_8 PyAtrshmlog_get_thread_fence_8_PROTO;
  static PyAtrshmlog_get_thread_fence_9_RETURN PyAtrshmlog_get_thread_fence_9 PyAtrshmlog_get_thread_fence_9_PROTO;
  static PyAtrshmlog_get_thread_fence_10_RETURN PyAtrshmlog_get_thread_fence_10 PyAtrshmlog_get_thread_fence_10_PROTO;
  static PyAtrshmlog_get_thread_fence_11_RETURN PyAtrshmlog_get_thread_fence_11 PyAtrshmlog_get_thread_fence_11_PROTO;
  static PyAtrshmlog_get_thread_fence_12_RETURN PyAtrshmlog_get_thread_fence_12 PyAtrshmlog_get_thread_fence_12_PROTO;
  static PyAtrshmlog_get_thread_fence_13_RETURN PyAtrshmlog_get_thread_fence_13 PyAtrshmlog_get_thread_fence_13_PROTO;
  static PyAtrshmlog_get_realtime_RETURN PyAtrshmlog_get_realtime PyAtrshmlog_get_realtime_PROTO;
  static PyAtrshmlog_get_thread_locals_adress_RETURN PyAtrshmlog_get_thread_locals_adress PyAtrshmlog_get_thread_locals_adress_PROTO;
  static PyAtrshmlog_get_tid_RETURN PyAtrshmlog_get_tid PyAtrshmlog_get_tid_PROTO;
  static PyAtrshmlog_turn_logging_off_RETURN PyAtrshmlog_turn_logging_off PyAtrshmlog_turn_logging_off_PROTO;
  static PyAtrshmlog_set_init_buffers_in_advance_on_RETURN PyAtrshmlog_set_init_buffers_in_advance_on PyAtrshmlog_set_init_buffers_in_advance_on_PROTO;
  static PyAtrshmlog_set_init_buffers_in_advance_off_RETURN PyAtrshmlog_set_init_buffers_in_advance_off PyAtrshmlog_set_init_buffers_in_advance_off_PROTO;
  static PyAtrshmlog_get_init_buffers_in_advance_RETURN PyAtrshmlog_get_init_buffers_in_advance PyAtrshmlog_get_init_buffers_in_advance_PROTO;
  static PyAtrshmlog_get_next_slave_local_RETURN PyAtrshmlog_get_next_slave_local PyAtrshmlog_get_next_slave_local_PROTO;
  static PyAtrshmlog_get_thread_local_tid_RETURN PyAtrshmlog_get_thread_local_tid PyAtrshmlog_get_thread_local_tid_PROTO;
  static PyAtrshmlog_remove_slave_via_local_RETURN PyAtrshmlog_remove_slave_via_local PyAtrshmlog_remove_slave_via_local_PROTO;
  static PyAtrshmlog_reuse_thread_buffers_RETURN PyAtrshmlog_reuse_thread_buffers PyAtrshmlog_reuse_thread_buffers_PROTO;
  static PyAtrshmlog_read_RETURN PyAtrshmlog_read PyAtrshmlog_read_PROTO;
  static PyAtrshmlog_read_fetch_RETURN PyAtrshmlog_read_fetch PyAtrshmlog_read_fetch_PROTO;
  static PyAtrshmlog_verify_RETURN PyAtrshmlog_verify PyAtrshmlog_verify_PROTO;
  static PyAtrshmlog_create_RETURN PyAtrshmlog_create PyAtrshmlog_create_PROTO;
  static PyAtrshmlog_delete_RETURN PyAtrshmlog_delete PyAtrshmlog_delete_PROTO;
  static PyAtrshmlog_cleanup_locks_RETURN PyAtrshmlog_cleanup_locks PyAtrshmlog_cleanup_locks_PROTO;
  static PyAtrshmlog_init_shm_log_RETURN PyAtrshmlog_init_shm_log PyAtrshmlog_init_shm_log_PROTO;
  static PyAtrshmlog_poke_RETURN PyAtrshmlog_poke PyAtrshmlog_poke_PROTO;
  static PyAtrshmlog_peek_RETURN PyAtrshmlog_peek PyAtrshmlog_peek_PROTO;
  
#else
  // the usage macros are here

static pyatrshmlog_vfctv_t *PyAtrshmlog_API;


#define PyAtrshmlog_attach \
  (*(PyAtrshmlog_attach_RETURN (*) PyAtrshmlog_attach_PROTO) PyAtrshmlog_API[PyAtrshmlog_attach_NUM])

#define PyAtrshmlog_gettime \
  (*(PyAtrshmlog_gettime_RETURN (*) PyAtrshmlog_gettime_PROTO) PyAtrshmlog_API[PyAtrshmlog_gettime_NUM])

#define PyAtrshmlog_write0 \
  (*(PyAtrshmlog_write0_RETURN (*) PyAtrshmlog_write0_PROTO) PYAtrshmlog_API[PyAtrshmlog_write0_NUM])

#define PyAtrshmlog_write \
  (*(PyAtrshmlog_write_RETURN (*) PyAtrshmlog_write_PROTO) PyAtrshmlog_API[PyAtrshmlog_write_NUM])
  

#define PyAtrshmlog_sleep_nanos \
  (*(PyAtrshmlog_sleep_nanos_RETURN (*) PyAtrshmlog_sleep_nanos_PROTO) PyAtrshmlog_API[PyAtrshmlog_sleep_nanos_NUM])
  
#define PyAtrshmlog_get_statistics_max_index \
  (*(PyAtrshmlog_get_statistics_max_index_RETURN (*) PyAtrshmlog_get_statistics_max_index_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_statistics_max_index_NUM])
  
#define PyAtrshmlog_get_statistics \
  (*(PyAtrshmlog_get_statistics_RETURN (*) PyAtrshmlog_get_statistics_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_statistics_NUM])
  
#define PyAtrshmlog_set_env_prefix \
  (*(PyAtrshmlog_set_env_prefix_RETURN (*) PyAtrshmlog_set_env_prefix_PROTO) PyAtrshmlog_API[PyAtrshmlog_set_env_prefix_NUM])
  
#define PyAtrshmlog_get_env_prefix \
  (*(PyAtrshmlog_get_env_prefix_RETURN (*) PyAtrshmlog_get_env_prefix_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_env_prefix_NUM])
  
#define PyAtrshmlog_get_env \
  (*(PyAtrshmlog_get_env_RETURN (*) PyAtrshmlog_get_env_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_env_NUM])
  
#define PyAtrshmlog_get_env_shmid \
  (*(PyAtrshmlog_get_env_shmid_RETURN (*) PyAtrshmlog_get_env_shmid_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_env_shmid_NUM])
  
#define PyAtrshmlog_get_env_id_suffix \
  (*(PyAtrshmlog_get_env_id_suffix_RETURN (*) PyAtrshmlog_get_env_id_suffix_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_env_id_suffix_NUM])
  
#define PyAtrshmlog_get_version \
  (*(PyAtrshmlog_get_version_RETURN (*) PyAtrshmlog_get_version_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_version_NUM])
  
#define PyAtrshmlog_get_minor_version \
  (*(PyAtrshmlog_get_minor_version_RETURN (*) PyAtrshmlog_get_minor_version_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_minor_version_NUM])
  
#define PyAtrshmlog_get_patch_version \
  (*(PyAtrshmlog_get_patch_version_RETURN (*) PyAtrshmlog_get_patch_version_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_patch_version_NUM])
  
#define PyAtrshmlog_get_event_locks_max \
  (*(PyAtrshmlog_get_event_locks_max_RETURN (*) PyAtrshmlog_get_event_locks_max_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_event_locks_max_NUM])
  
#define PyAtrshmlog_set_event_locks_max \
  (*(PyAtrshmlog_set_event_locks_max_RETURN (*) PyAtrshmlog_set_event_locks_max_PROTO) PyAtrshmlog_API[PyAtrshmlog_set_event_locks_max_NUM])
  
#define PyAtrshmlog_get_event \
  (*(PyAtrshmlog_get_event_RETURN (*) PyAtrshmlog_get_event_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_event_NUM])
  
#define PyAtrshmlog_set_event_on \
  (*(PyAtrshmlog_set_event_on_RETURN (*) PyAtrshmlog_set_event_on_PROTO) PyAtrshmlog_API[PyAtrshmlog_set_event_on_NUM])
  
#define PyAtrshmlog_set_event_off \
  (*(PyAtrshmlog_set_event_off_RETURN (*) PyAtrshmlog_set_event_off_PROTO) PyAtrshmlog_API[PyAtrshmlog_set_event_off_NUM])
  
#define PyAtrshmlog_get_logging \
  (*(PyAtrshmlog_get_logging_RETURN (*)PyAtrshmlog_get_logging_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_logging_NUM])
  
#define PyAtrshmlog_set_logging_process_on \
  (*(PyAtrshmlog_set_logging_process_on_RETURN (*)PyAtrshmlog_set_logging_process_on_PROTO) PyAtrshmlog_API[PyAtrshmlog_set_logging_process_on_NUM])
  
#define PyAtrshmlog_set_logging_process_off \
  (*(PyAtrshmlog_set_logging_process_off_RETURN (*)PyAtrshmlog_set_logging_process_off_PROTO) PyAtrshmlog_API[PyAtrshmlog_set_logging_process_off_NUM])
  
#define PyAtrshmlog_set_logging_process_off_final \
  (*(PyAtrshmlog_set_logging_process_off_final_RETURN (*)PyAtrshmlog_set_logging_process_off_final_PROTO) PyAtrshmlog_API[PyAtrshmlog_set_logging_process_off_final_NUM])
  
#define PyAtrshmlog_get_shmid \
  (*(PyAtrshmlog_get_shmid_RETURN (*)PyAtrshmlog_get_shmid_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_shmid_NUM])
  
#define PyAtrshmlog_get_area \
  (*(PyAtrshmlog_get_area_RETURN (*)PyAtrshmlog_get_area_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_area_NUM])
  
#define PyAtrshmlog_get_area_ich_habe_fertig \
  (*(PyAtrshmlog_get_area_ich_habe_fertig_RETURN (*)PyAtrshmlog_get_area_ich_habe_fertig_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_area_ich_habe_fertig_NUM])
  
#define PyAtrshmlog_set_area_ich_habe_fertig \
  (*(PyAtrshmlog_set_area_ich_habe_fertig_RETURN (*)PyAtrshmlog_set_area_ich_habe_fertig_PROTO) PyAtrshmlog_API[PyAtrshmlog_set_area_ich_habe_fertig_NUM])
  
#define PyAtrshmlog_get_area_count \
  (*(PyAtrshmlog_get_area_count_RETURN (*)PyAtrshmlog_get_area_count_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_area_count_NUM])
  
#define PyAtrshmlog_get_area_version \
  (*(PyAtrshmlog_get_area_version_RETURN (*)PyAtrshmlog_get_area_version_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_area_version_NUM])
  
#define PyAtrshmlog_get_buffer_max_size \
  (*(PyAtrshmlog_get_buffer_max_size_RETURN (*)PyAtrshmlog_get_buffer_max_size_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_buffer_max_size_NUM])
  
#define PyAtrshmlog_get_buffer_size \
  (*(PyAtrshmlog_get_buffer_size_RETURN (*)PyAtrshmlog_get_buffer_size_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_buffer_size_NUM])
  
#define PyAtrshmlog_set_buffer_size \
  (*(PyAtrshmlog_set_buffer_size_RETURN (*)PyAtrshmlog_set_buffer_size_PROTO) PyAtrshmlog_API[PyAtrshmlog_set_buffer_size_NUM])
  
#define PyAtrshmlog_get_f_list_buffer_slave_count \
  (*(PyAtrshmlog_get_f_list_buffer_slave_count_RETURN (*)PyAtrshmlog_get_f_list_buffer_slave_count_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_f_list_buffer_slave_count_NUM])
  
#define PyAtrshmlog_set_f_list_buffer_slave_count \
  (*(PyAtrshmlog_set_f_list_buffer_slave_count_RETURN (*)PyAtrshmlog_set_f_list_buffer_slave_count_PROTO) PyAtrshmlog_API[PyAtrshmlog_set_f_list_buffer_slave_count_NUM])
  
#define PyAtrshmlog_get_clock_id \
  (*(PyAtrshmlog_get_clock_id_RETURN (*)PyAtrshmlog_get_clock_id_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_clock_id_NUM])
  
#define PyAtrshmlog_set_clock_id \
  (*(PyAtrshmlog_set_clock_id_RETURN (*)PyAtrshmlog_set_clock_id_PROTO) PyAtrshmlog_API[PyAtrshmlog_set_clock_id_NUM])
  
#define PyAtrshmlog_set_f_list_buffer_slave_run_off \
  (*(PyAtrshmlog_set_f_list_buffer_slave_run_off_RETURN (*)PyAtrshmlog_set_f_list_buffer_slave_run_off_PROTO) PyAtrshmlog_API[PyAtrshmlog_set_f_list_buffer_slave_run_off_NUM])
  
#define PyAtrshmlog_set_wait_for_slaves_on \
  (*(PyAtrshmlog_set_wait_for_slaves_on_RETURN (*)PyAtrshmlog_set_wait_for_slaves_on_PROTO) PyAtrshmlog_API[PyAtrshmlog_set_wait_for_slaves_on_NUM])
  
#define PyAtrshmlog_set_wait_for_slaves_off \
  (*(PyAtrshmlog_set_wait_for_slaves_off_RETURN (*)PyAtrshmlog_set_wait_for_slaves_off_PROTO) PyAtrshmlog_API[PyAtrshmlog_set_wait_for_slaves_off_NUM])
  
#define PyAtrshmlog_get_wait_for_slaves \
  (*(PyAtrshmlog_get_wait_for_slaves_RETURN (*)PyAtrshmlog_get_wait_for_slaves_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_wait_for_slaves_NUM])
  
#define PyAtrshmlog_get_f_list_buffer_slave_wait \
  (*(PyAtrshmlog_get_f_list_buffer_slave_wait_RETURN (*)PyAtrshmlog_get_f_list_buffer_slave_wait_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_f_list_buffer_slave_wait_NUM])
  
#define PyAtrshmlog_set_f_list_buffer_slave_wait \
  (*(PyAtrshmlog_set_f_list_buffer_slave_wait_RETURN (*)PyAtrshmlog_set_f_list_buffer_slave_wait_PROTO) PyAtrshmlog_API[PyAtrshmlog_set_f_list_buffer_slave_wait_NUM])
  
#define PyAtrshmlog_get_acquire_count \
  (*(PyAtrshmlog_get_acquire_count_RETURN (*)PyAtrshmlog_get_acquire_count_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_acquire_count_NUM])
  
#define PyAtrshmlog_get_prealloc_buffer_count \
  (*(PyAtrshmlog_get_prealloc_buffer_count_RETURN (*)PyAtrshmlog_get_prealloc_buffer_count_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_prealloc_buffer_count_NUM])
  
#define PyAtrshmlog_set_prealloc_buffer_count \
  (*(PyAtrshmlog_set_prealloc_buffer_count_RETURN (*)PyAtrshmlog_set_prealloc_buffer_count_PROTO) PyAtrshmlog_API[PyAtrshmlog_set_prealloc_buffer_count_NUM])
  
#define PyAtrshmlog_get_inittime \
  (*(PyAtrshmlog_get_inittime_RETURN (*)PyAtrshmlog_get_inittime_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_inittime_NUM])
  
#define PyAtrshmlog_get_inittime_tsc_before \
  (*(PyAtrshmlog_get_inittime_tsc_before_RETURN (*)PyAtrshmlog_get_inittime_tsc_before_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_inittime_tsc_before_NUM])
  
#define PyAtrshmlog_get_inittime_tsc_after \
  (*(PyAtrshmlog_get_inittime_tsc_after_RETURN (*)PyAtrshmlog_get_inittime_tsc_after_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_inittime_tsc_after_NUM])
  
#define PyAtrshmlog_get_buffer_id \
  (*(PyAtrshmlog_get_buffer_id_RETURN (*)PyAtrshmlog_get_buffer_id_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_buffer_id_NUM])
  
#define PyAtrshmlog_stop \
  (*(PyAtrshmlog_stop_RETURN (*)PyAtrshmlog_stop_PROTO) PyAtrshmlog_API[PyAtrshmlog_stop_NUM])
  
#define PyAtrshmlog_flush \
  (*(PyAtrshmlog_flush_RETURN (*)PyAtrshmlog_flush_PROTO) PyAtrshmlog_API[PyAtrshmlog_flush_NUM])
  
#define PyAtrshmlog_set_strategy \
  (*(PyAtrshmlog_set_strategy_RETURN (*)PyAtrshmlog_set_strategy_PROTO) PyAtrshmlog_API[PyAtrshmlog_set_strategy_NUM])
  
#define PyAtrshmlog_get_strategy \
  (*(PyAtrshmlog_get_strategy_RETURN (*)PyAtrshmlog_get_strategy_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_strategy_NUM])
  
#define PyAtrshmlog_set_strategy_process \
  (*(PyAtrshmlog_set_strategy_process_RETURN (*)PyAtrshmlog_set_strategy_process_PROTO) PyAtrshmlog_API[PyAtrshmlog_set_strategy_process_NUM])
  
#define PyAtrshmlog_get_strategy_process \
  (*(PyAtrshmlog_get_strategy_process_RETURN (*)PyAtrshmlog_get_strategy_process_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_strategy_process_NUM])
  
#define PyAtrshmlog_create_slave \
  (*(PyAtrshmlog_create_slave_RETURN (*)PyAtrshmlog_create_slave_PROTO) PyAtrshmlog_API[PyAtrshmlog_create_slave_NUM])
  
#define PyAtrshmlog_decrement_slave_count \
  (*(PyAtrshmlog_decrement_slave_count_RETURN (*)PyAtrshmlog_decrement_slave_count_PROTO) PyAtrshmlog_API[PyAtrshmlog_decrement_slave_count_NUM])
  
#define PyAtrshmlog_get_clicktime \
  (*(PyAtrshmlog_get_clicktime_RETURN (*)PyAtrshmlog_get_clicktime_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_clicktime_NUM])
  
#define PyAtrshmlog_set_thread_fence_1 \
  (*(PyAtrshmlog_set_thread_fence_1_RETURN (*)PyAtrshmlog_set_thread_fence_1_PROTO) PyAtrshmlog_API[PyAtrshmlog_set_thread_fence_1_NUM])
  
#define PyAtrshmlog_set_thread_fence_2 \
  (*(PyAtrshmlog_set_thread_fence_2_RETURN (*)PyAtrshmlog_set_thread_fence_2_PROTO) PyAtrshmlog_API[PyAtrshmlog_set_thread_fence_2_NUM])
  
#define PyAtrshmlog_set_thread_fence_3 \
  (*(PyAtrshmlog_set_thread_fence_3_RETURN (*)PyAtrshmlog_set_thread_fence_3_PROTO) PyAtrshmlog_API[PyAtrshmlog_set_thread_fence_3_NUM])
  
#define PyAtrshmlog_set_thread_fence_4 \
  (*(PyAtrshmlog_set_thread_fence_4_RETURN (*)PyAtrshmlog_set_thread_fence_4_PROTO) PyAtrshmlog_API[PyAtrshmlog_set_thread_fence_4_NUM])
  
#define PyAtrshmlog_set_thread_fence_5 \
  (*(PyAtrshmlog_set_thread_fence_5_RETURN (*)PyAtrshmlog_set_thread_fence_5_PROTO) PyAtrshmlog_API[PyAtrshmlog_set_thread_fence_5_NUM])
  
#define PyAtrshmlog_set_thread_fence_6 \
  (*(PyAtrshmlog_set_thread_fence_6_RETURN (*)PyAtrshmlog_set_thread_fence_6_PROTO) PyAtrshmlog_API[PyAtrshmlog_set_thread_fence_6_NUM])
  
#define PyAtrshmlog_set_thread_fence_7 \
  (*(PyAtrshmlog_set_thread_fence_7_RETURN (*)PyAtrshmlog_set_thread_fence_7_PROTO) PyAtrshmlog_API[PyAtrshmlog_set_thread_fence_7_NUM])
  
#define PyAtrshmlog_set_thread_fence_8 \
  (*(PyAtrshmlog_set_thread_fence_8_RETURN (*)PyAtrshmlog_set_thread_fence_8_PROTO) PyAtrshmlog_API[PyAtrshmlog_set_thread_fence_8_NUM])
  
#define PyAtrshmlog_set_thread_fence_9 \
  (*(PyAtrshmlog_set_thread_fence_9_RETURN (*)PyAtrshmlog_set_thread_fence_9_PROTO) PyAtrshmlog_API[PyAtrshmlog_set_thread_fence_9_NUM])
  
#define PyAtrshmlog_set_thread_fence_10 \
  (*(PyAtrshmlog_set_thread_fence_10_RETURN (*)PyAtrshmlog_set_thread_fence_10_PROTO) PyAtrshmlog_API[PyAtrshmlog_set_thread_fence_10_NUM])
  
#define PyAtrshmlog_set_thread_fence_11 \
  (*(PyAtrshmlog_set_thread_fence_11_RETURN (*)PyAtrshmlog_set_thread_fence_11_PROTO) PyAtrshmlog_API[PyAtrshmlog_set_thread_fence_11_NUM])
  
#define PyAtrshmlog_set_thread_fence_12 \
  (*(PyAtrshmlog_set_thread_fence_12_RETURN (*)PyAtrshmlog_set_thread_fence_12_PROTO) PyAtrshmlog_API[PyAtrshmlog_set_thread_fence_12_NUM])
  
#define PyAtrshmlog_set_thread_fence_13 \
  (*(PyAtrshmlog_set_thread_fence_13_RETURN (*)PyAtrshmlog_set_thread_fence_13_PROTO) PyAtrshmlog_API[PyAtrshmlog_set_thread_fence_13_NUM])
  
#define PyAtrshmlog_get_thread_fence_1 \
  (*(PyAtrshmlog_get_thread_fence_1_RETURN (*)PyAtrshmlog_get_thread_fence_1_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_thread_fence_1_NUM])
  
#define PyAtrshmlog_get_thread_fence_2 \
  (*(PyAtrshmlog_get_thread_fence_2_RETURN (*)PyAtrshmlog_get_thread_fence_2_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_thread_fence_2_NUM])
  
#define PyAtrshmlog_get_thread_fence_3 \
  (*(PyAtrshmlog_get_thread_fence_3_RETURN (*)PyAtrshmlog_get_thread_fence_3_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_thread_fence_3_NUM])
  
#define PyAtrshmlog_get_thread_fence_4 \
  (*(PyAtrshmlog_get_thread_fence_4_RETURN (*)PyAtrshmlog_get_thread_fence_4_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_thread_fence_4_NUM])
  
#define PyAtrshmlog_get_thread_fence_5 \
  (*(PyAtrshmlog_get_thread_fence_5_RETURN (*)PyAtrshmlog_get_thread_fence_5_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_thread_fence_5_NUM])
  
#define PyAtrshmlog_get_thread_fence_6 \
  (*(PyAtrshmlog_get_thread_fence_6_RETURN (*)PyAtrshmlog_get_thread_fence_6_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_thread_fence_6_NUM])
  
#define PyAtrshmlog_get_thread_fence_7 \
  (*(PyAtrshmlog_get_thread_fence_7_RETURN (*)PyAtrshmlog_get_thread_fence_7_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_thread_fence_7_NUM])
  
#define PyAtrshmlog_get_thread_fence_8 \
  (*(PyAtrshmlog_get_thread_fence_8_RETURN (*)PyAtrshmlog_get_thread_fence_8_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_thread_fence_8_NUM])
  
#define PyAtrshmlog_get_thread_fence_9 \
  (*(PyAtrshmlog_get_thread_fence_9_RETURN (*)PyAtrshmlog_get_thread_fence_9_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_thread_fence_9_NUM])
  
#define PyAtrshmlog_get_thread_fence_10 \
  (*(PyAtrshmlog_get_thread_fence_10_RETURN (*)PyAtrshmlog_get_thread_fence_10_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_thread_fence_10_NUM])
  
#define PyAtrshmlog_get_thread_fence_11 \
  (*(PyAtrshmlog_get_thread_fence_11_RETURN (*)PyAtrshmlog_get_thread_fence_11_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_thread_fence_11_NUM])
  
#define PyAtrshmlog_get_thread_fence_12 \
  (*(PyAtrshmlog_get_thread_fence_12_RETURN (*)PyAtrshmlog_get_thread_fence_12_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_thread_fence_12_NUM])
  
#define PyAtrshmlog_get_thread_fence_13 \
  (*(PyAtrshmlog_get_thread_fence_13_RETURN (*)PyAtrshmlog_get_thread_fence_13_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_thread_fence_13_NUM])
  
#define PyAtrshmlog_get_realtime \
  (*(PyAtrshmlog_get_realtime_RETURN (*)PyAtrshmlog_get_realtime_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_realtime_NUM])
  
#define PyAtrshmlog_get_thread_locals_adress \
  (*(PyAtrshmlog_get_thread_locals_adress_RETURN (*)PyAtrshmlog_get_thread_locals_adress_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_thread_locals_adress_NUM])
  
#define PyAtrshmlog_get_tid \
  (*(PyAtrshmlog_get_tid_RETURN (*)PyAtrshmlog_get_tid_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_tid_NUM])
  
#define PyAtrshmlog_turn_logging_off \
  (*(PyAtrshmlog_turn_logging_off_RETURN (*)PyAtrshmlog_turn_logging_off_PROTO) PyAtrshmlog_API[PyAtrshmlog_turn_logging_off_NUM])
  
#define PyAtrshmlog_set_init_buffers_in_advance_on \
  (*(PyAtrshmlog_set_init_buffers_in_advance_on_RETURN (*)PyAtrshmlog_set_init_buffers_in_advance_on_PROTO) PyAtrshmlog_API[PyAtrshmlog_set_init_buffers_in_advance_on_NUM])
  
#define PyAtrshmlog_set_init_buffers_in_advance_off \
  (*(PyAtrshmlog_set_init_buffers_in_advance_off_RETURN (*)PyAtrshmlog_set_init_buffers_in_advance_off_PROTO) PyAtrshmlog_API[PyAtrshmlog_set_init_buffers_in_advance_off_NUM])
  
#define PyAtrshmlog_get_init_buffers_in_advance \
  (*(PyAtrshmlog_get_init_buffers_in_advance_RETURN (*)PyAtrshmlog_get_init_buffers_in_advance_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_init_buffers_in_advance_NUM])
  
#define PyAtrshmlog_get_next_slave_local \
  (*(PyAtrshmlog_get_next_slave_local_RETURN (*)PyAtrshmlog_get_next_slave_local_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_next_slave_local_NUM])
  
#define PyAtrshmlog_get_thread_local_tid \
  (*(PyAtrshmlog_get_thread_local_tid_RETURN (*)PyAtrshmlog_get_thread_local_tid_PROTO) PyAtrshmlog_API[PyAtrshmlog_get_thread_local_tid_NUM])
  
#define PyAtrshmlog_remove_slave_via_local  \
  (*(PyAtrshmlog_remove_slave_via_local_RETURN (*)PyAtrshmlog_remove_slave_via_local_PROTO) PyAtrshmlog_API[PyAtrshmlog_remove_slave_via_local_NUM])
  
#define PyAtrshmlog_reuse_thread_buffers  \
  (*(PyAtrshmlog_reuse_thread_buffers_RETURN (*)PyAtrshmlog_reuse_thread_buffers_PROTO) PyAtrshmlog_API[PyAtrshmlog_reuse_thread_buffers_NUM])
  
#define PyAtrshmlog_read  \
  (*(PyAtrshmlog_read_RETURN (*)PyAtrshmlog_read_PROTO) PyAtrshmlog_API[PyAtrshmlog_read_NUM])
  
#define PyAtrshmlog_read_fetch  \
  (*(PyAtrshmlog_read_fetch_RETURN (*)PyAtrshmlog_read_fetch_PROTO) PyAtrshmlog_API[PyAtrshmlog_read_fetch_NUM])
  
#define PyAtrshmlog_verify						\
  (*(PyAtrshmlog_verify_RETURN (*)PyAtrshmlog_verify_PROTO) PyAtrshmlog_API[PyAtrshmlog_verify_NUM])
  
#define PyAtrshmlog_create \
  (*(PyAtrshmlog_create_RETURN (*)PyAtrshmlog_create_PROTO) PyAtrshmlog_API[PyAtrshmlog_create_NUM])
  
#define PyAtrshmlog_delete \
  (*(PyAtrshmlog_delete_RETURN (*)PyAtrshmlog_delete_PROTO) PyAtrshmlog_API[PyAtrshmlog_delete_NUM])
  
#define PyAtrshmlog_cleanup_locks \
  (*(PyAtrshmlog_cleanup_locks_RETURN (*)PyAtrshmlog_cleanup_locks_PROTO) PyAtrshmlog_API[PyAtrshmlog_cleanup_locks_NUM])
  
#define PyAtrshmlog_init_shm_log \
  (*(PyAtrshmlog_init_shm_log_RETURN (*)PyAtrshmlog_init_shm_log_PROTO) PyAtrshmlog_API[PyAtrshmlog_init_shm_log_NUM])
  
#define PyAtrshmlog_poke \
  (*(PyAtrshmlog_poke_RETURN (*)PyAtrshmlog_poke_PROTO) PyAtrshmlog_API[PyAtrshmlog_poke_NUM])
  
#define PyAtrshmlog_peek \
  (*(PyAtrshmlog_peek_RETURN (*)PyAtrshmlog_peek_PROTO) PyAtrshmlog_API[PyAtrshmlog_peek_NUM])
  



  /* Return -1 on error, 0 on success.
   * PyCapsule_Import will set an exception if there's an error.
   */
  static int
  import_atrshmlog(void)
  {
    PyAtrshmlog_API = (pyatrshmlog_vfctv_t*)PyCapsule_Import("atrshmlog._C_API", 0);
    return (PyAtrshmlog_API != (pyatrshmlog_vfctv_t)0) ? 0 : -1;
  }

#endif

    
  
#ifdef __cplusplus
}
#endif


#endif 
/* end of file */