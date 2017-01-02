
/*
 * the ruby module for the log
 */

/** \file atrshmlogruby.c
 * \brief We implement the ruby to c bridge for the module
 *
 * We do this for an actual 2.2. version on fedora 23 here
 */


/********************************************/

// we are the ruby module
#include <ruby.h>

// we have our local config
#include "extconf.h"

// we use the loging module
#include "atrshmlog_internal.h"

/*********************************************/

/* a helper to convert safer */
union u_s
{
  const volatile void *p;
  unsigned long l;
} ;

typedef union u_s u_t;

/*********************************************/

// the module for ruby interpreter
static VALUE m_atrshmlog;

/*********************************************/

/*
 * the ruby bridge functions.
 * We use normal value semantic for all functions
 * with no more than 16 parameters - in a article 17 was 
 * named as a max for ruby, but i think i can live
 * with having the big 4 using arrays here.
 */

/* the functions */

static VALUE atrshmlogruby_attach(VALUE obj);

static VALUE atrshmlogruby_gettime(VALUE obj);

static VALUE atrshmlogruby_write0(VALUE obj,
			 VALUE eventnumber,
			 VALUE eventflag,
			 VALUE userflag,
			 VALUE starttime,
			 VALUE endtime);

static VALUE atrshmlogruby_write(VALUE obj,
			VALUE eventnumber,
			VALUE eventflag,
			VALUE userflag,
			VALUE starttime,
			VALUE endtime,
			VALUE payload);

static VALUE atrshmlogruby_sleep_nanos(VALUE obj,
			      VALUE nanos);

static VALUE atrshmlogruby_get_statistics_max_index(VALUE obj);

static VALUE atrshmlogruby_get_statistics(VALUE obj);

static VALUE atrshmlogruby_set_env_prefix(VALUE obj,
					  VALUE prefix);

static VALUE atrshmlogruby_get_env_prefix(VALUE obj);

static VALUE atrshmlogruby_get_env(VALUE obj,
				   VALUE suffix);

static VALUE atrshmlogruby_get_env_shmid(VALUE obj);

static VALUE atrshmlogruby_get_env_id_suffix(VALUE obj);

static VALUE atrshmlogruby_get_version(VALUE obj);

static VALUE atrshmlogruby_get_minor_version(VALUE obj);

static VALUE atrshmlogruby_get_patch_version(VALUE obj);
				
static VALUE atrshmlogruby_get_event_locks_max (VALUE obj);

static VALUE atrshmlogruby_set_event_locks_max (VALUE obj,
						VALUE newmax);

static VALUE atrshmlogruby_get_event (VALUE obj,
				      VALUE event);

static VALUE atrshmlogruby_set_event_on (VALUE obj,
					 VALUE event);

static VALUE atrshmlogruby_set_event_off (VALUE obj,
					  VALUE event);

static VALUE atrshmlogruby_get_logging (VALUE obj);
				
static VALUE atrshmlogruby_set_logging_process_on (VALUE obj);

static VALUE atrshmlogruby_set_logging_process_off (VALUE obj);

static VALUE atrshmlogruby_set_logging_process_off_final (VALUE obj);

static VALUE atrshmlogruby_get_shmid (VALUE obj);

static VALUE atrshmlogruby_get_area (VALUE obj);

static VALUE atrshmlogruby_get_area_ich_habe_fertig (VALUE obj,
						     VALUE area);

static VALUE atrshmlogruby_set_area_ich_habe_fertig (VALUE obj,
						     VALUE area,
						     VALUE flag);

static VALUE atrshmlogruby_get_area_count (VALUE obj,
					   VALUE area);

static VALUE atrshmlogruby_get_area_version (VALUE obj,
					     VALUE area);

static VALUE atrshmlogruby_get_buffer_max_size (VALUE obj);

static VALUE atrshmlogruby_get_buffer_size (VALUE obj);

static VALUE atrshmlogruby_set_buffer_size (VALUE obj,
					    VALUE size);

static VALUE atrshmlogruby_get_f_list_buffer_slave_count (VALUE obj);

static VALUE atrshmlogruby_set_f_list_buffer_slave_count (VALUE obj,
							  VALUE count);
				
static VALUE atrshmlogruby_get_clock_id (VALUE obj);

static VALUE atrshmlogruby_set_clock_id (VALUE obj,
					 VALUE id);

static VALUE atrshmlogruby_set_f_list_buffer_slave_run_off (VALUE obj);
				
static VALUE atrshmlogruby_set_wait_for_slaves_on (VALUE obj);

static VALUE atrshmlogruby_set_wait_for_slaves_off (VALUE obj);

static VALUE atrshmlogruby_get_wait_for_slaves (VALUE obj);

static VALUE atrshmlogruby_get_f_list_buffer_slave_wait (VALUE obj);

static VALUE atrshmlogruby_set_f_list_buffer_slave_wait (VALUE obj,
							 VALUE nanos);

static VALUE atrshmlogruby_get_acquire_count (VALUE obj);

static VALUE atrshmlogruby_get_prealloc_buffer_count (VALUE obj);

static VALUE atrshmlogruby_set_prealloc_buffer_count (VALUE obj,
						      VALUE count);

static VALUE atrshmlogruby_get_inittime (VALUE obj);

static VALUE atrshmlogruby_get_inittime_tsc_before (VALUE obj);

static VALUE atrshmlogruby_get_inittime_tsc_after (VALUE obj);
				
static VALUE atrshmlogruby_get_buffer_id (VALUE obj);

static VALUE atrshmlogruby_stop (VALUE obj);

static VALUE atrshmlogruby_flush (VALUE obj);

				
static VALUE atrshmlogruby_set_strategy (VALUE obj,
					 VALUE strategy);
				
static VALUE atrshmlogruby_get_strategy (VALUE obj);
				
static VALUE atrshmlogruby_set_strategy_process (VALUE obj,
						 VALUE strategy);
				
static VALUE atrshmlogruby_get_strategy_process (VALUE obj);
				
static VALUE atrshmlogruby_create_slave (VALUE obj);
				
static VALUE atrshmlogruby_decrement_slave_count (VALUE obj);
				
static VALUE atrshmlogruby_get_clicktime (VALUE obj);
				

static VALUE atrshmlogruby_set_thread_fence_1 (VALUE obj,
					       VALUE flag);
				
static VALUE atrshmlogruby_set_thread_fence_2 (VALUE obj,
					       VALUE flag);
				
static VALUE atrshmlogruby_set_thread_fence_3 (VALUE obj,
					       VALUE flag);
				
static VALUE atrshmlogruby_set_thread_fence_4 (VALUE obj,
					       VALUE flag);
				
static VALUE atrshmlogruby_set_thread_fence_5 (VALUE obj,
					       VALUE flag);
				
static VALUE atrshmlogruby_set_thread_fence_6 (VALUE obj,
					       VALUE flag);
				
static VALUE atrshmlogruby_set_thread_fence_7 (VALUE obj,
					       VALUE flag);
				
static VALUE atrshmlogruby_set_thread_fence_8 (VALUE obj,
					       VALUE flag);
				
static VALUE atrshmlogruby_set_thread_fence_9 (VALUE obj,
					       VALUE flag);
				
static VALUE atrshmlogruby_set_thread_fence_10 (VALUE obj,
						VALUE flag);
				
static VALUE atrshmlogruby_set_thread_fence_11 (VALUE obj,
						VALUE flag);
				
static VALUE atrshmlogruby_set_thread_fence_12 (VALUE obj,
						VALUE flag);
				
static VALUE atrshmlogruby_set_thread_fence_13 (VALUE obj,
						VALUE flag);
				
static VALUE atrshmlogruby_get_thread_fence_1 (VALUE obj);
				
static VALUE atrshmlogruby_get_thread_fence_2 (VALUE obj);
				
static VALUE atrshmlogruby_get_thread_fence_3 (VALUE obj);
				
static VALUE atrshmlogruby_get_thread_fence_4 (VALUE obj);
				
static VALUE atrshmlogruby_get_thread_fence_5 (VALUE obj);
				
static VALUE atrshmlogruby_get_thread_fence_6 (VALUE obj);
				
static VALUE atrshmlogruby_get_thread_fence_7 (VALUE obj);
				
static VALUE atrshmlogruby_get_thread_fence_8 (VALUE obj);
				
static VALUE atrshmlogruby_get_thread_fence_9 (VALUE obj);
				
static VALUE atrshmlogruby_get_thread_fence_10 (VALUE obj);
				
static VALUE atrshmlogruby_get_thread_fence_11 (VALUE obj);
				
static VALUE atrshmlogruby_get_thread_fence_12 (VALUE obj);
				
static VALUE atrshmlogruby_get_thread_fence_13 (VALUE obj);
				
				
static VALUE atrshmlogruby_get_realtime (VALUE obj);

static VALUE atrshmlogruby_get_thread_locals_adress (VALUE obj);
				
static VALUE atrshmlogruby_get_tid (VALUE obj);
				
static VALUE atrshmlogruby_turn_logging_off (VALUE obj,
					     VALUE tl);

static VALUE atrshmlogruby_set_init_buffers_in_advance_on (VALUE obj);
				
static VALUE atrshmlogruby_set_init_buffers_in_advance_off (VALUE obj);
				
static VALUE atrshmlogruby_get_init_buffers_in_advance (VALUE obj);
				

static VALUE atrshmlogruby_get_next_slave_local (VALUE obj,
						 VALUE sl);
				
static VALUE atrshmlogruby_get_thread_local_tid (VALUE obj,
						 VALUE tl);

static VALUE atrshmlogruby_remove_slave_via_local (VALUE obj,
						   VALUE sl);
				
static VALUE atrshmlogruby_reuse_thread_buffers (VALUE obj,
						 VALUE tid);
				
static VALUE atrshmlogruby_read (VALUE obj,
				 VALUE area,
				 VALUE index);
				
static VALUE atrshmlogruby_read_fetch (VALUE obj,
				       VALUE area);
				
static VALUE atrshmlogruby_verify (VALUE obj);
				
static VALUE atrshmlogruby_create (VALUE obj,
				   VALUE ipckey,
				   VALUE count);
				
static VALUE atrshmlogruby_delete (VALUE obj,
				   VALUE id);
				
static VALUE atrshmlogruby_cleanup_locks (VALUE obj,
					  VALUE area);
				
static VALUE atrshmlogruby_init_shm_log (VALUE obj,
					 VALUE area,
					 VALUE count);
				

static VALUE atrshmlogruby_poke (VALUE obj,
				 VALUE area,
				 VALUE index,
				 VALUE val);

static VALUE atrshmlogruby_peek (VALUE obj,
				 VALUE area,
				 VALUE index);
				
static VALUE atrshmlogruby_get_slave_tid (VALUE obj,
					  VALUE sl);

				
static VALUE atrshmlogruby_turn_slave_off (VALUE obj,
					   VALUE sl);
				
static VALUE atrshmlogruby_set_autoflush_process (VALUE obj,
						  VALUE flag);

static VALUE atrshmlogruby_get_autoflush_process (VALUE obj);
				
static VALUE atrshmlogruby_set_autoflush (VALUE obj,
					  VALUE flag);

static VALUE atrshmlogruby_get_autoflush (VALUE obj);
				
static VALUE atrshmlogruby_set_checksum (VALUE obj,
					 VALUE flag);

static VALUE atrshmlogruby_get_checksum (VALUE obj);
				
static VALUE atrshmlogruby_detach (VALUE obj);

static VALUE atrshmlogruby_reattach (VALUE obj,
				     VALUE args);

static VALUE atrshmlogruby_get_strategy_wait_wait_time (VALUE obj);

static VALUE atrshmlogruby_set_strategy_wait_wait_time (VALUE obj,
							VALUE nanos);
				
static VALUE atrshmlogruby_get_thread_local_pid (VALUE obj,
						 VALUE tl);
				
static VALUE atrshmlogruby_get_thread_local_index (VALUE obj,
						   VALUE tl);

static VALUE atrshmlogruby_get_thread_local_buffer (VALUE obj,
						    VALUE tl,
						    VALUE index);
				
				
static VALUE atrshmlogruby_get_thread_buffer_next_cleanup (VALUE obj,
							   VALUE buf);
				
static VALUE atrshmlogruby_get_thread_buffer_next_full (VALUE obj,
							VALUE buf);

static VALUE atrshmlogruby_get_thread_buffer_next_append (VALUE obj,
							  VALUE buf);
				

static VALUE atrshmlogruby_get_thread_buffer_safeguard (VALUE obj,
							VALUE buf);
				
				
static VALUE atrshmlogruby_get_thread_buffer_pid (VALUE obj,
						  VALUE buf);

static VALUE atrshmlogruby_get_thread_buffer_tid (VALUE obj,
						  VALUE buf);
				
static VALUE atrshmlogruby_get_thread_buffer_acquiretime (VALUE obj,
							  VALUE buf);

static VALUE atrshmlogruby_get_thread_buffer_id (VALUE obj,
						 VALUE buf);
				
static VALUE atrshmlogruby_get_thread_buffer_chksum (VALUE obj,
						     VALUE buf);

static VALUE atrshmlogruby_get_thread_buffer_size (VALUE obj,
						   VALUE buf);
				
static VALUE atrshmlogruby_get_thread_buffer_maxsize (VALUE obj,
						      VALUE buf);
				
static VALUE atrshmlogruby_get_thread_buffer_dispose (VALUE obj,
						      VALUE buf);

static VALUE atrshmlogruby_get_thread_buffer_dispatched (VALUE obj,
							 VALUE buf);

static VALUE atrshmlogruby_get_thread_buffer_payload (VALUE obj,
						      VALUE buf);

static VALUE atrshmlogruby_get_slave_to_shm_wait (VALUE obj);
				
static VALUE atrshmlogruby_set_slave_to_shm_wait (VALUE obj,
						  VALUE nanos);
				
static VALUE atrshmlogruby_get_last_mem_to_shm (VALUE obj);
				
				
static VALUE atrshmlogruby_get_buffer_cleanup_anchor (VALUE obj);
				
static VALUE atrshmlogruby_get_buffer_full_anchor (VALUE obj);

static VALUE atrshmlogruby_get_buffer_append_anchor (VALUE obj);
				

/*******************************************************/

/*
 * we initialize the module 
 */
void Init_atrshmlog(void)
{
  m_atrshmlog = rb_define_module("Atrshmlog");

  rb_define_singleton_method(m_atrshmlog, "attach", atrshmlogruby_attach, 0);

  rb_define_singleton_method(m_atrshmlog, "gettime", atrshmlogruby_gettime, 0);

  rb_define_singleton_method(m_atrshmlog, "write0", atrshmlogruby_write0, 5);

  rb_define_singleton_method(m_atrshmlog, "write", atrshmlogruby_write, 6);

  rb_define_singleton_method(m_atrshmlog, "sleep_nanos", atrshmlogruby_sleep_nanos, 1);

  rb_define_singleton_method(m_atrshmlog, "get_statistics_max_index", atrshmlogruby_get_statistics_max_index, 0);

  rb_define_singleton_method(m_atrshmlog, "get_statistics", atrshmlogruby_get_statistics, 0);

  rb_define_singleton_method(m_atrshmlog, "set_env_prefix", atrshmlogruby_set_env_prefix, 1);

  rb_define_singleton_method(m_atrshmlog, "get_env_prefix", atrshmlogruby_get_env_prefix, 0);

  rb_define_singleton_method(m_atrshmlog, "get_env", atrshmlogruby_get_env, 1);

  rb_define_singleton_method(m_atrshmlog, "get_env_shmid", atrshmlogruby_get_env_shmid, 0);

  rb_define_singleton_method(m_atrshmlog, "get_env_id_suffix", atrshmlogruby_get_env_id_suffix, 0);

  rb_define_singleton_method(m_atrshmlog, "get_version", atrshmlogruby_get_version, 0);

  rb_define_singleton_method(m_atrshmlog, "get_minor_version", atrshmlogruby_get_minor_version, 0);

  rb_define_singleton_method(m_atrshmlog, "get_patch_version", atrshmlogruby_get_patch_version, 0);

  rb_define_singleton_method(m_atrshmlog, "get_event_locks_max", atrshmlogruby_get_event_locks_max , 0);

  rb_define_singleton_method(m_atrshmlog, "set_event_locks_max", atrshmlogruby_set_event_locks_max , 1);

  rb_define_singleton_method(m_atrshmlog, "get_event", atrshmlogruby_get_event , 1);

  rb_define_singleton_method(m_atrshmlog, "set_event_on", atrshmlogruby_set_event_on , 1);

  rb_define_singleton_method(m_atrshmlog, "set_event_off", atrshmlogruby_set_event_off , 1);

  rb_define_singleton_method(m_atrshmlog, "get_logging", atrshmlogruby_get_logging , 0);
				
  rb_define_singleton_method(m_atrshmlog, "set_logging_process_on", atrshmlogruby_set_logging_process_on , 0);

  rb_define_singleton_method(m_atrshmlog, "set_logging_process_off", atrshmlogruby_set_logging_process_off , 0);

  rb_define_singleton_method(m_atrshmlog, "set_logging_process_off_final", atrshmlogruby_set_logging_process_off_final , 0);

  rb_define_singleton_method(m_atrshmlog, "get_shmid",  atrshmlogruby_get_shmid ,0);

  rb_define_singleton_method(m_atrshmlog, "get_area", atrshmlogruby_get_area , 0);

  rb_define_singleton_method(m_atrshmlog, "get_area_ich_habe_fertig", atrshmlogruby_get_area_ich_habe_fertig , 1);

  rb_define_singleton_method(m_atrshmlog, "set_area_ich_habe_fertig",  atrshmlogruby_set_area_ich_habe_fertig , 2);

  rb_define_singleton_method(m_atrshmlog, "get_area_count",  atrshmlogruby_get_area_count , 1);

  rb_define_singleton_method(m_atrshmlog, "get_area_version",  atrshmlogruby_get_area_version , 1);

  rb_define_singleton_method(m_atrshmlog, "get_buffer_max_size",  atrshmlogruby_get_buffer_max_size ,0);
  
  rb_define_singleton_method(m_atrshmlog, "get_buffer_size",  atrshmlogruby_get_buffer_size , 0);

  rb_define_singleton_method(m_atrshmlog, "set_buffer_size",  atrshmlogruby_set_buffer_size , 1);

  rb_define_singleton_method(m_atrshmlog, "get_f_list_buffer_slave_count",  atrshmlogruby_get_f_list_buffer_slave_count , 0);

  rb_define_singleton_method(m_atrshmlog, "set_f_list_buffer_slave_count",  atrshmlogruby_set_f_list_buffer_slave_count , 1);

  rb_define_singleton_method(m_atrshmlog, "get_clock_id",  atrshmlogruby_get_clock_id , 0);

  rb_define_singleton_method(m_atrshmlog, "set_clock_id",  atrshmlogruby_set_clock_id , 1);

  rb_define_singleton_method(m_atrshmlog, "set_f_list_buffer_slave_run_off",  atrshmlogruby_set_f_list_buffer_slave_run_off , 0);
				
  rb_define_singleton_method(m_atrshmlog, "set_wait_for_slaves_on",  atrshmlogruby_set_wait_for_slaves_on , 0);

  rb_define_singleton_method(m_atrshmlog, "set_wait_for_slaves_off",  atrshmlogruby_set_wait_for_slaves_off , 0);

  rb_define_singleton_method(m_atrshmlog, "get_wait_for_slaves",  atrshmlogruby_get_wait_for_slaves ,0);

  rb_define_singleton_method(m_atrshmlog, "get_f_list_buffer_slave_wait",  atrshmlogruby_get_f_list_buffer_slave_wait , 0);

  rb_define_singleton_method(m_atrshmlog, "set_f_list_buffer_slave_wait",  atrshmlogruby_set_f_list_buffer_slave_wait , 1);

  rb_define_singleton_method(m_atrshmlog, "get_acquire_count",  atrshmlogruby_get_acquire_count ,0);

  rb_define_singleton_method(m_atrshmlog, "get_prealloc_buffer_count",  atrshmlogruby_get_prealloc_buffer_count , 0);

  rb_define_singleton_method(m_atrshmlog, "set_prealloc_buffer_count", atrshmlogruby_set_prealloc_buffer_count, 1);

  rb_define_singleton_method(m_atrshmlog, "get_inittime",  atrshmlogruby_get_inittime , 0);

  rb_define_singleton_method(m_atrshmlog, "get_inittime_tsc_before", atrshmlogruby_get_inittime_tsc_before , 0);

  rb_define_singleton_method(m_atrshmlog, "get_inittime_tsc_after", atrshmlogruby_get_inittime_tsc_after , 0);
				
  rb_define_singleton_method(m_atrshmlog, "get_buffer_id", atrshmlogruby_get_buffer_id , 0);

  rb_define_singleton_method(m_atrshmlog, "stop", atrshmlogruby_stop , 0);

  rb_define_singleton_method(m_atrshmlog, "flush", atrshmlogruby_flush , 0);

  rb_define_singleton_method(m_atrshmlog, "set_strategy", atrshmlogruby_set_strategy , 1);

  rb_define_singleton_method(m_atrshmlog, "get_strategy", atrshmlogruby_get_strategy , 0);
				
  rb_define_singleton_method(m_atrshmlog, "set_strategy_process", atrshmlogruby_set_strategy_process , 1);
				
  rb_define_singleton_method(m_atrshmlog, "get_strategy_process", atrshmlogruby_get_strategy_process , 0);
				
  rb_define_singleton_method(m_atrshmlog, "create_slave", atrshmlogruby_create_slave , 0);
				
  rb_define_singleton_method(m_atrshmlog, "decrement_slave_count", atrshmlogruby_decrement_slave_count , 0);
				
  rb_define_singleton_method(m_atrshmlog, "get_clicktime", atrshmlogruby_get_clicktime , 0);
				
  rb_define_singleton_method(m_atrshmlog, "set_thread_fence_1", atrshmlogruby_set_thread_fence_1 , 1);
				
  rb_define_singleton_method(m_atrshmlog, "set_thread_fence_2", atrshmlogruby_set_thread_fence_2 , 1);

  rb_define_singleton_method(m_atrshmlog, "set_thread_fence_3", atrshmlogruby_set_thread_fence_3 , 1);

  rb_define_singleton_method(m_atrshmlog, "set_thread_fence_4", atrshmlogruby_set_thread_fence_4 , 1);

  rb_define_singleton_method(m_atrshmlog, "set_thread_fence_5", atrshmlogruby_set_thread_fence_5 , 1);

  rb_define_singleton_method(m_atrshmlog, "set_thread_fence_6", atrshmlogruby_set_thread_fence_6 , 1);

  rb_define_singleton_method(m_atrshmlog, "set_thread_fence_7", atrshmlogruby_set_thread_fence_7 , 1);

  rb_define_singleton_method(m_atrshmlog, "set_thread_fence_8", atrshmlogruby_set_thread_fence_8 , 1);

  rb_define_singleton_method(m_atrshmlog, "set_thread_fence_9", atrshmlogruby_set_thread_fence_9 , 1);

  rb_define_singleton_method(m_atrshmlog, "set_thread_fence_10", atrshmlogruby_set_thread_fence_10 , 1);

  rb_define_singleton_method(m_atrshmlog, "set_thread_fence_11", atrshmlogruby_set_thread_fence_11 , 1);

  rb_define_singleton_method(m_atrshmlog, "set_thread_fence_12", atrshmlogruby_set_thread_fence_12 , 1);

  rb_define_singleton_method(m_atrshmlog, "set_thread_fence_13", atrshmlogruby_set_thread_fence_13 , 1);

  rb_define_singleton_method(m_atrshmlog, "get_thread_fence_1", atrshmlogruby_get_thread_fence_1 , 0);
				
  rb_define_singleton_method(m_atrshmlog, "get_thread_fence_2", atrshmlogruby_get_thread_fence_2 , 0);
				
  rb_define_singleton_method(m_atrshmlog, "get_thread_fence_3",  atrshmlogruby_get_thread_fence_3 , 0);
				
  rb_define_singleton_method(m_atrshmlog, "get_thread_fence_4",  atrshmlogruby_get_thread_fence_4 , 0);
				
  rb_define_singleton_method(m_atrshmlog, "get_thread_fence_5", atrshmlogruby_get_thread_fence_5 , 0);
				
  rb_define_singleton_method(m_atrshmlog, "get_thread_fence_6", atrshmlogruby_get_thread_fence_6 , 0);
				
  rb_define_singleton_method(m_atrshmlog, "get_thread_fence_7", atrshmlogruby_get_thread_fence_7 , 0);
				
  rb_define_singleton_method(m_atrshmlog, "get_thread_fence_8", atrshmlogruby_get_thread_fence_8 , 0);
				
  rb_define_singleton_method(m_atrshmlog, "get_thread_fence_9", atrshmlogruby_get_thread_fence_9 , 0);
				
  rb_define_singleton_method(m_atrshmlog, "get_thread_fence_10", atrshmlogruby_get_thread_fence_10 , 0);
				
  rb_define_singleton_method(m_atrshmlog, "get_thread_fence_11", atrshmlogruby_get_thread_fence_11 , 0);
				
  rb_define_singleton_method(m_atrshmlog, "get_thread_fence_12", atrshmlogruby_get_thread_fence_12 , 0);
				
  rb_define_singleton_method(m_atrshmlog, "get_thread_fence_13", atrshmlogruby_get_thread_fence_13 , 0);
				
  rb_define_singleton_method(m_atrshmlog, "get_realtime", atrshmlogruby_get_realtime , 0);

  rb_define_singleton_method(m_atrshmlog, "get_thread_locals_adress", atrshmlogruby_get_thread_locals_adress , 0);
				
  rb_define_singleton_method(m_atrshmlog, "get_tid", atrshmlogruby_get_tid , 0);
				
  rb_define_singleton_method(m_atrshmlog, "turn_logging_off", atrshmlogruby_turn_logging_off , 1);

  rb_define_singleton_method(m_atrshmlog, "set_init_buffers_in_advance_on", atrshmlogruby_set_init_buffers_in_advance_on , 0);
				
  rb_define_singleton_method(m_atrshmlog, "set_init_buffers_in_advance_off", atrshmlogruby_set_init_buffers_in_advance_off , 0);
				
  rb_define_singleton_method(m_atrshmlog, "get_init_buffers_in_advance", atrshmlogruby_get_init_buffers_in_advance , 0);
				
  rb_define_singleton_method(m_atrshmlog, "get_next_slave_local", atrshmlogruby_get_next_slave_local , 1);

  rb_define_singleton_method(m_atrshmlog, "get_thread_local_tid", atrshmlogruby_get_thread_local_tid , 1);

  rb_define_singleton_method(m_atrshmlog, "remove_slave_via_local", atrshmlogruby_remove_slave_via_local , 1);
				
  rb_define_singleton_method(m_atrshmlog, "reuse_thread_buffers", atrshmlogruby_reuse_thread_buffers , 1);

  rb_define_singleton_method(m_atrshmlog, "read", atrshmlogruby_read, 2);
				
  rb_define_singleton_method(m_atrshmlog, "read_fetch",  atrshmlogruby_read_fetch , 1);

  rb_define_singleton_method(m_atrshmlog, "verify", atrshmlogruby_verify , 0);
				
  rb_define_singleton_method(m_atrshmlog, "create", atrshmlogruby_create , 2);
				
  rb_define_singleton_method(m_atrshmlog, "delete", atrshmlogruby_delete , 1);
				
  rb_define_singleton_method(m_atrshmlog, "cleanup_locks", atrshmlogruby_cleanup_locks , 1);
				
  rb_define_singleton_method(m_atrshmlog, "init_shm_log", atrshmlogruby_init_shm_log , 2);

  rb_define_singleton_method(m_atrshmlog, "poke", atrshmlogruby_poke , 3);

  rb_define_singleton_method(m_atrshmlog, "peek", atrshmlogruby_peek , 2);

  rb_define_singleton_method(m_atrshmlog, "get_slave_tid", atrshmlogruby_get_slave_tid , 1);

  rb_define_singleton_method(m_atrshmlog, "turn_slave_off",  atrshmlogruby_turn_slave_off , 1);
				
  rb_define_singleton_method(m_atrshmlog, "set_autoflush_process",  atrshmlogruby_set_autoflush_process , 1);

  rb_define_singleton_method(m_atrshmlog, "get_autoflush_process", atrshmlogruby_get_autoflush_process , 0);
				
  rb_define_singleton_method(m_atrshmlog, "set_autoflush", atrshmlogruby_set_autoflush , 1);

  rb_define_singleton_method(m_atrshmlog, "get_autoflush", atrshmlogruby_get_autoflush , 0);
				
  rb_define_singleton_method(m_atrshmlog, "set_checksum", atrshmlogruby_set_checksum , 1);

  rb_define_singleton_method(m_atrshmlog, "get_checksum", atrshmlogruby_get_checksum , 0);
				
  rb_define_singleton_method(m_atrshmlog, "detach", atrshmlogruby_detach , 0);

  rb_define_singleton_method(m_atrshmlog, "reattach", atrshmlogruby_reattach , -2);

  rb_define_singleton_method(m_atrshmlog, "get_strategy_wait_wait_time", atrshmlogruby_get_strategy_wait_wait_time , 0);

  rb_define_singleton_method(m_atrshmlog, "set_strategy_wait_wait_time", atrshmlogruby_set_strategy_wait_wait_time , 1);
				
  rb_define_singleton_method(m_atrshmlog, "get_thread_local_pid", atrshmlogruby_get_thread_local_pid , 1);
				
  rb_define_singleton_method(m_atrshmlog, "get_thread_local_index", atrshmlogruby_get_thread_local_index , 1);

  rb_define_singleton_method(m_atrshmlog, "get_thread_local_buffer", atrshmlogruby_get_thread_local_buffer , 2);
				
  rb_define_singleton_method(m_atrshmlog, "get_thread_buffer_next_cleanup", atrshmlogruby_get_thread_buffer_next_cleanup , 1);
				
  rb_define_singleton_method(m_atrshmlog, "get_thread_buffer_next_full", atrshmlogruby_get_thread_buffer_next_full , 1);

  rb_define_singleton_method(m_atrshmlog, "get_thread_buffer_next_append", atrshmlogruby_get_thread_buffer_next_append , 1);
				
  rb_define_singleton_method(m_atrshmlog, "get_thread_buffer_safeguard", atrshmlogruby_get_thread_buffer_safeguard , 1);
				
  rb_define_singleton_method(m_atrshmlog, "get_thread_buffer_pid", atrshmlogruby_get_thread_buffer_pid  , 1);

  rb_define_singleton_method(m_atrshmlog, "get_thread_buffer_tid", atrshmlogruby_get_thread_buffer_tid , 1);
				
  rb_define_singleton_method(m_atrshmlog, "get_thread_buffer_acquiretime", atrshmlogruby_get_thread_buffer_acquiretime , 1);

  rb_define_singleton_method(m_atrshmlog, "get_thread_buffer_id", atrshmlogruby_get_thread_buffer_id , 1);
				
  rb_define_singleton_method(m_atrshmlog, "get_thread_buffer_chksum", atrshmlogruby_get_thread_buffer_chksum , 1);

  rb_define_singleton_method(m_atrshmlog, "get_thread_buffer_size", atrshmlogruby_get_thread_buffer_size , 1);

  rb_define_singleton_method(m_atrshmlog, "get_thread_buffer_maxsize", atrshmlogruby_get_thread_buffer_maxsize , 1);

  rb_define_singleton_method(m_atrshmlog, "get_thread_buffer_dispose", atrshmlogruby_get_thread_buffer_dispose , 1);

  rb_define_singleton_method(m_atrshmlog, "get_thread_buffer_dispatched", atrshmlogruby_get_thread_buffer_dispatched , 1);

  rb_define_singleton_method(m_atrshmlog, "get_thread_buffer_payload", atrshmlogruby_get_thread_buffer_payload, 1);

  rb_define_singleton_method(m_atrshmlog, "get_slave_to_shm_wait", atrshmlogruby_get_slave_to_shm_wait , 0);
				
  rb_define_singleton_method(m_atrshmlog, "set_slave_to_shm_wait", atrshmlogruby_set_slave_to_shm_wait , 1);
				
  rb_define_singleton_method(m_atrshmlog, "get_last_mem_to_shm", atrshmlogruby_get_last_mem_to_shm , 0);
				
  rb_define_singleton_method(m_atrshmlog, "get_buffer_cleanup_anchor", atrshmlogruby_get_buffer_cleanup_anchor , 0);
				
  rb_define_singleton_method(m_atrshmlog, "get_buffer_full_anchor", atrshmlogruby_get_buffer_full_anchor , 0);

  rb_define_singleton_method(m_atrshmlog, "get_buffer_append_anchor", atrshmlogruby_get_buffer_append_anchor , 0);

  /********************/

  static const int ruby_EVENT_POINT_IN_TIME_UCS = ATRSHMLOGPOINTINTIMEp;
  static const int ruby_EVENT_POINT_IN_TIME_C = ATRSHMLOGPOINTINTIMEP;
  static const int ruby_EVENT_INTERVAL_IN_TIME_UCS = ATRSHMLOGPOINTINTIMEi;
  static const int ruby_EVENT_INTERVAL_IN_TIME_C = ATRSHMLOGPOINTINTIMEI;
  
  rb_define_const(m_atrshmlog, "EVENT_POINT_IN_TIME_UCS", INT2NUM(ruby_EVENT_POINT_IN_TIME_UCS));
  rb_define_const(m_atrshmlog, "EVENT_POINT_IN_TIME_C", INT2NUM(ruby_EVENT_POINT_IN_TIME_C));
  rb_define_const(m_atrshmlog, "EVENT_INTERVAL_IN_TIME_UCS", INT2NUM(ruby_EVENT_INTERVAL_IN_TIME_UCS));
  rb_define_const(m_atrshmlog, "EVENT_INTERVAL_IN_TIME_C", INT2NUM(ruby_EVENT_INTERVAL_IN_TIME_C));
}

/*****************************************************/

/*
 * We implement the bridge code
 */
VALUE atrshmlogruby_attach(VALUE obj)
{
  int result = ATRSHMLOG_ATTACH();
  
  return INT2NUM(result);
}

VALUE atrshmlogruby_gettime(VALUE obj)
{
  unsigned long result = ATRSHMLOG_GET_TSC_CALL();

  return LONG2FIX(result);
}

VALUE atrshmlogruby_write0(VALUE obj,
			   VALUE eventnumber,
			   VALUE eventflag,
			   VALUE userflag,
			   VALUE starttime,
			   VALUE endtime)
{
  Check_Type(eventnumber, T_FIXNUM);
  Check_Type(eventflag, T_FIXNUM);
  Check_Type(userflag, T_FIXNUM);
  Check_Type(starttime, T_FIXNUM);
  Check_Type(endtime, T_FIXNUM);
  
  atrshmlog_int32_t event = FIX2INT(eventnumber);
  atrshmlog_int32_t eflag = FIX2INT(eventflag);
  atrshmlog_int32_t uflag = FIX2INT(userflag);
  unsigned long start = FIX2ULONG(starttime);
  unsigned long end = FIX2ULONG(endtime);

  int result = ATRSHMLOG_WRITE0(event, eflag, uflag, start, end);

  return INT2FIX(result);
}

VALUE atrshmlogruby_write(VALUE obj,
			  VALUE eventnumber,
			  VALUE eventflag,
			  VALUE userflag,
			  VALUE starttime,
			  VALUE endtime,
			  VALUE payload)
{
  Check_Type(eventnumber, T_FIXNUM);
  Check_Type(eventflag, T_FIXNUM);
  Check_Type(userflag, T_FIXNUM);
  Check_Type(starttime, T_FIXNUM);
  Check_Type(endtime, T_FIXNUM);
  
  atrshmlog_int32_t event = FIX2INT(eventnumber);
  atrshmlog_int32_t eflag = FIX2INT(eventflag);
  atrshmlog_int32_t uflag = FIX2INT(userflag);
  unsigned long start = FIX2ULONG(starttime);
  unsigned long end = FIX2ULONG(endtime);

  const char *payl = RSTRING_PTR(payload);
  int siz = RSTRING_LEN(payload);
  
  int result = ATRSHMLOG_WRITE(event, eflag, uflag, start, end, payl, siz);

  return INT2FIX(result);
}


VALUE atrshmlogruby_sleep_nanos(VALUE obj,
				VALUE nanos)
{
  int n = NUM2INT(nanos);
  
  ATRSHMLOG_SLEEP_NANOS(n);
  
  int result = 0;

  return INT2NUM(result);
}


VALUE atrshmlogruby_get_statistics_max_index(VALUE obj)
{
  int result = ATRSHMLOG_GET_STATISTICS_MAX_INDEX();

  return INT2NUM(result);
}


VALUE atrshmlogruby_get_statistics(VALUE obj)
{
  atrshmlog_int32_t s[100];

  ATRSHMLOG_GET_STATISTICS(s);

  int size = atrshmlog_counter_end + 1;
  
  VALUE ret = rb_ary_new2(size);

  for (int i = 0; i <= atrshmlog_counter_end; i++)
    rb_ary_store(ret, i , INT2NUM((int)s[i]));
  
  return ret;
}


 VALUE atrshmlogruby_set_env_prefix(VALUE obj,
				 VALUE prefix)
{
  char buff[1024];

  int len = RSTRING_LEN(prefix);
  
  memcpy(buff, RSTRING_PTR(prefix), len);

  buff[len] = 0;
  
  ATRSHMLOG_SET_ENV_PREFIX(buff);
  
  int result = 0;

  return INT2NUM(result);
}


 VALUE atrshmlogruby_get_env_prefix(VALUE obj)
{
  const char* result = ATRSHMLOG_GET_ENV_PREFIX();

  return rb_str_new2(result);
}


 VALUE atrshmlogruby_get_env(VALUE obj,
			  VALUE suffix)
{
  char buff[1024];

  int len = RSTRING_LEN(suffix);
  
  memcpy(buff, RSTRING_PTR(suffix), len);

  buff[len] = 0;
  
  const char* result =  ATRSHMLOG_GET_ENV(buff);

  return rb_str_new2(result);
}


 VALUE atrshmlogruby_get_env_shmid(VALUE obj)
{
  const char* result = ATRSHMLOG_GET_ENV_SHMID();

  return rb_str_new2(result);
}


 VALUE atrshmlogruby_get_env_id_suffix(VALUE obj)
{
  const char* result = ATRSHMLOG_GET_ENV_ID_SUFFIX();

  return rb_str_new2(result);
}


 VALUE atrshmlogruby_get_version(VALUE obj)
{
  int result = ATRSHMLOG_GET_VERSION();

  return INT2NUM(result);
}


 VALUE atrshmlogruby_get_minor_version(VALUE obj)
{
  int result = ATRSHMLOG_GET_MINOR_VERSION();

  return INT2NUM(result);
}


 VALUE atrshmlogruby_get_patch_version(VALUE obj)
{
  int result = ATRSHMLOG_GET_PATCH_VERSION();

  return INT2NUM(result);
}

				
 VALUE atrshmlogruby_get_event_locks_max (VALUE obj)
{
  int result = ATRSHMLOG_GET_EVENT_LOCKS_MAX();

  return INT2NUM(result);
}


 VALUE atrshmlogruby_set_event_locks_max (VALUE obj,
				       VALUE newmax)
{
  int n = NUM2INT(newmax);
  
  int result = ATRSHMLOG_SET_EVENT_LOCKS_MAX(n);

  return INT2NUM(result);
}


 VALUE atrshmlogruby_get_event (VALUE obj,
			     VALUE event)
{
  int e = NUM2INT(event);
  
  int result = ATRSHMLOG_GET_EVENT(e);

  return INT2NUM(result);
}


 VALUE atrshmlogruby_set_event_on (VALUE obj,
				VALUE event)
{
  int e = NUM2INT(event);
  
  int result = ATRSHMLOG_SET_EVENT_ON(e);

  return INT2NUM(result);
}


 VALUE atrshmlogruby_set_event_off (VALUE obj,
				 VALUE event)
{
  int e = NUM2INT(event);
  
  int result = ATRSHMLOG_SET_EVENT_OFF(e);

  return INT2NUM(result);
}


 VALUE atrshmlogruby_get_logging (VALUE obj)
{
  int result = ATRSHMLOG_GET_LOGGING();

  return INT2NUM(result);
}

				
 VALUE atrshmlogruby_set_logging_process_on (VALUE obj)
{
  int result = ATRSHMLOG_SET_LOGGING_PROCESS_ON();

  return INT2NUM(result);
}


 VALUE atrshmlogruby_set_logging_process_off (VALUE obj)
{
  int result = ATRSHMLOG_SET_LOGGING_PROCESS_OFF();

  return INT2NUM(result);
}


 VALUE atrshmlogruby_set_logging_process_off_final (VALUE obj)
{
  int result = ATRSHMLOG_SET_LOGGING_PROCESS_OFF_FINAL();

  return INT2NUM(result);
}


 VALUE atrshmlogruby_get_shmid (VALUE obj)
{
  int result = ATRSHMLOG_GET_SHMID();

  return INT2NUM(result);
}


 VALUE atrshmlogruby_get_area (VALUE obj)
{
  u_t u;

  u.p = ATRSHMLOG_GET_AREA();

  return ULONG2NUM(u.l);
}


 VALUE atrshmlogruby_get_area_ich_habe_fertig (VALUE obj,
					    VALUE area)
{
  u_t u;

  u.l = NUM2ULONG(area);

  int result = ATRSHMLOG_GET_AREA_ICH_HABE_FERTIG(u.p);

  return INT2NUM(result);
}


 VALUE atrshmlogruby_set_area_ich_habe_fertig (VALUE obj,
					    VALUE area,
					    VALUE flag)
{
  u_t u;

  u.l = NUM2ULONG(area);
  
  int f = NUM2INT(flag);
  
  int result = ATRSHMLOG_SET_AREA_ICH_HABE_FERTIG(u.p, f);

  return INT2NUM(result);
}


 VALUE atrshmlogruby_get_area_count (VALUE obj,
				  VALUE area)
{
  u_t u;

  u.l = NUM2ULONG(area);
  
  int result = ATRSHMLOG_GET_AREA_COUNT(u.p);

  return INT2NUM(result);
}


 VALUE atrshmlogruby_get_area_version (VALUE obj,
				    VALUE area)
{
  u_t u;

  u.l = NUM2ULONG(area);
  
  int result =ATRSHMLOG_GET_AREA_VERSION(u.p);

  return INT2NUM(result);
}


 VALUE atrshmlogruby_get_buffer_max_size (VALUE obj)
{
  int result = ATRSHMLOG_GET_BUFFER_MAX_SIZE();

  return INT2NUM(result);
}


 VALUE atrshmlogruby_get_buffer_size (VALUE obj)
{
  int result = ATRSHMLOG_GET_BUFFER_SIZE();

  return INT2NUM(result);
}


 VALUE atrshmlogruby_set_buffer_size (VALUE obj,
				   VALUE size)
{
  int s = NUM2INT(size);
  
  int result = ATRSHMLOG_SET_BUFFER_SIZE(s);

  return INT2NUM(result);
}


 VALUE atrshmlogruby_get_f_list_buffer_slave_count (VALUE obj)
{
  int result = ATRSHMLOG_GET_F_LIST_BUFFER_SLAVE_COUNT();

  return INT2NUM(result);
}


 VALUE atrshmlogruby_set_f_list_buffer_slave_count (VALUE obj,
						 VALUE count)
{
  int c = NUM2INT(count);
  
  int result = ATRSHMLOG_SET_F_LIST_BUFFER_SLAVE_COUNT(c);

  return INT2NUM(result);
}

				
 VALUE atrshmlogruby_get_clock_id (VALUE obj)
{
  int result = ATRSHMLOG_GET_CLOCK_ID();

  return INT2NUM(result);
}


 VALUE atrshmlogruby_set_clock_id (VALUE obj,
				VALUE id)
{
  int i = NUM2INT(id);
  
  int result = ATRSHMLOG_SET_CLOCK_ID(i);

  return INT2NUM(result);
}


 VALUE atrshmlogruby_set_f_list_buffer_slave_run_off (VALUE obj)
{
  ATRSHMLOG_SET_F_LIST_BUFFER_SLAVE_RUN_OFF();

  int result = 0;

  return INT2NUM(result);
}

				
 VALUE atrshmlogruby_set_wait_for_slaves_on (VALUE obj)
{
  int result = ATRSHMLOG_SET_WAIT_FOR_SLAVES_ON();

  return INT2NUM(result);
}


 VALUE atrshmlogruby_set_wait_for_slaves_off (VALUE obj)
{
  int result = ATRSHMLOG_SET_WAIT_FOR_SLAVES_OFF();

  return INT2NUM(result);
}


 VALUE atrshmlogruby_get_wait_for_slaves (VALUE obj)
{
  int result = ATRSHMLOG_GET_WAIT_FOR_SLAVES();

  return INT2NUM(result);
}


 VALUE atrshmlogruby_get_f_list_buffer_slave_wait (VALUE obj)
{
  int result = ATRSHMLOG_GET_F_LIST_BUFFER_SLAVE_WAIT();

  return INT2NUM(result);
}


 VALUE atrshmlogruby_set_f_list_buffer_slave_wait (VALUE obj,
						VALUE nanos)
{
  int n = NUM2INT(nanos);
  
  int result = ATRSHMLOG_SET_F_LIST_BUFFER_SLAVE_WAIT(n);

  return INT2NUM(result);
}


 VALUE atrshmlogruby_get_acquire_count (VALUE obj)
{
  int result = ATRSHMLOG_GET_ACQUIRE_COUNT();

  return INT2NUM(result);
}


 VALUE atrshmlogruby_get_prealloc_buffer_count (VALUE obj)
{
  int result = ATRSHMLOG_GET_PREALLOC_BUFFER_COUNT();

  return INT2NUM(result);
}


 VALUE atrshmlogruby_set_prealloc_buffer_count (VALUE obj,
					     VALUE count)
{
  int c = NUM2INT(count);
  
  int result = ATRSHMLOG_SET_PREALLOC_BUFFER_COUNT(c);

  return INT2NUM(result);
}


 VALUE atrshmlogruby_get_inittime (VALUE obj)
{
  atrshmlog_internal_time_t t = ATRSHMLOG_GET_INITTIME();

  int size = 2;
  
  VALUE ret = rb_ary_new2(size);

  rb_ary_store(ret, 0, LONG2NUM(t.tv_sec));
  rb_ary_store(ret, 1, INT2NUM(t.tv_nsec));
  
  return ret;
}


 VALUE atrshmlogruby_get_inittime_tsc_before (VALUE obj)
{
  unsigned long  result = ATRSHMLOG_GET_INITTIME_TSC_BEFORE();

  return ULONG2NUM(result);
}


 VALUE atrshmlogruby_get_inittime_tsc_after (VALUE obj)
{
  unsigned long  result = ATRSHMLOG_GET_INITTIME_TSC_AFTER();

  return ULONG2NUM(result);
}

				
 VALUE atrshmlogruby_get_buffer_id (VALUE obj)
{
  int result = ATRSHMLOG_GET_BUFFER_ID();

  return INT2NUM(result);
}


 VALUE atrshmlogruby_stop (VALUE obj)
{
  ATRSHMLOG_STOP();
  
  int result = 0;

  return INT2NUM(result);
}


 VALUE atrshmlogruby_flush (VALUE obj)
{
  ATRSHMLOG_FLUSH();
  
  int result = 0;

  return INT2NUM(result);
}


				
 VALUE atrshmlogruby_set_strategy (VALUE obj,
				VALUE strategy)
{
  int s = NUM2INT(strategy);
  
  int result = ATRSHMLOG_SET_STRATEGY(s);

  return INT2NUM(result);
}

				
 VALUE atrshmlogruby_get_strategy (VALUE obj)
{
  int result =  ATRSHMLOG_GET_STRATEGY();

  return INT2NUM(result);
}

				
 VALUE atrshmlogruby_set_strategy_process (VALUE obj,
					VALUE strategy)
{
  int s = NUM2INT(strategy);
  
  int result = ATRSHMLOG_SET_STRATEGY_PROCESS(s);

  return INT2NUM(result);
}

				
 VALUE atrshmlogruby_get_strategy_process (VALUE obj)
{
  int result = ATRSHMLOG_GET_STRATEGY_PROCESS();

  return INT2NUM(result);
}

				
 VALUE atrshmlogruby_create_slave (VALUE obj)
{
  int result = ATRSHMLOG_CREATE_SLAVE();

  return INT2NUM(result);
}

				
 VALUE atrshmlogruby_decrement_slave_count (VALUE obj)
{
  int result = ATRSHMLOG_DECREMENT_SLAVE_COUNT();

  return INT2NUM(result);
}

				
 VALUE atrshmlogruby_get_clicktime (VALUE obj)
{
  unsigned long result = ATRSHMLOG_GET_CLICKTIME();

  return ULONG2NUM(result);
}

				

 VALUE atrshmlogruby_set_thread_fence_1 (VALUE obj,
				      VALUE flag)
{
  int f = NUM2INT(flag);
  
  int result = ATRSHMLOG_SET_THREAD_FENCE_1(f);

  return INT2NUM(result);
}

 VALUE atrshmlogruby_set_thread_fence_2 (VALUE obj,
				      VALUE flag)
{
  int f = NUM2INT(flag);
  
  int result = ATRSHMLOG_SET_THREAD_FENCE_2(f);

  return INT2NUM(result);
}

 VALUE atrshmlogruby_set_thread_fence_3 (VALUE obj,
				      VALUE flag)
{
  int f = NUM2INT(flag);
  
  int result = ATRSHMLOG_SET_THREAD_FENCE_3(f);

  return INT2NUM(result);
}

 VALUE atrshmlogruby_set_thread_fence_4 (VALUE obj,
				      VALUE flag)
{
  int f = NUM2INT(flag);
  
  int result = ATRSHMLOG_SET_THREAD_FENCE_4(f);

  return INT2NUM(result);
}

 VALUE atrshmlogruby_set_thread_fence_5 (VALUE obj,
				      VALUE flag)
{
  int f = NUM2INT(flag);
  
  int result = ATRSHMLOG_SET_THREAD_FENCE_5(f);

  return INT2NUM(result);
}

 VALUE atrshmlogruby_set_thread_fence_6 (VALUE obj,
				      VALUE flag)
{
  int f = NUM2INT(flag);
  
  int result = ATRSHMLOG_SET_THREAD_FENCE_6(f);

  return INT2NUM(result);
}

 VALUE atrshmlogruby_set_thread_fence_7 (VALUE obj,
				      VALUE flag)
{
  int f = NUM2INT(flag);
  
  int result = ATRSHMLOG_SET_THREAD_FENCE_7(f);

  return INT2NUM(result);
}

 VALUE atrshmlogruby_set_thread_fence_8 (VALUE obj,
				      VALUE flag)
{
  int f = NUM2INT(flag);
  
  int result = ATRSHMLOG_SET_THREAD_FENCE_8(f);

  return INT2NUM(result);
}

 VALUE atrshmlogruby_set_thread_fence_9 (VALUE obj,
				      VALUE flag)
{
  int f = NUM2INT(flag);
  
  int result = ATRSHMLOG_SET_THREAD_FENCE_9(f);

  return INT2NUM(result);
}

 VALUE atrshmlogruby_set_thread_fence_10 (VALUE obj,
				      VALUE flag)
{
  int f = NUM2INT(flag);
  
  int result = ATRSHMLOG_SET_THREAD_FENCE_10(f);

  return INT2NUM(result);
}

 VALUE atrshmlogruby_set_thread_fence_11 (VALUE obj,
				      VALUE flag)
{
  int f = NUM2INT(flag);
  
  int result = ATRSHMLOG_SET_THREAD_FENCE_11(f);

  return INT2NUM(result);
}

 VALUE atrshmlogruby_set_thread_fence_12 (VALUE obj,
				      VALUE flag)
{
  int f = NUM2INT(flag);
  
  int result = ATRSHMLOG_SET_THREAD_FENCE_12(f);

  return INT2NUM(result);
}

 VALUE atrshmlogruby_set_thread_fence_13 (VALUE obj,
				      VALUE flag)
{
  int f = NUM2INT(flag);
  
  int result = ATRSHMLOG_SET_THREAD_FENCE_13(f);

  return INT2NUM(result);
}

				
 VALUE atrshmlogruby_get_thread_fence_1 (VALUE obj)
{
  int result  = ATRSHMLOG_GET_THREAD_FENCE_1();

  return INT2NUM(result);
}

				
 VALUE atrshmlogruby_get_thread_fence_2 (VALUE obj)
{
  int result  = ATRSHMLOG_GET_THREAD_FENCE_2();

  return INT2NUM(result);
}

 VALUE atrshmlogruby_get_thread_fence_3 (VALUE obj)
{
  int result  = ATRSHMLOG_GET_THREAD_FENCE_3();

  return INT2NUM(result);
}

 VALUE atrshmlogruby_get_thread_fence_4 (VALUE obj)
{
  int result  = ATRSHMLOG_GET_THREAD_FENCE_4();

  return INT2NUM(result);
}

 VALUE atrshmlogruby_get_thread_fence_5 (VALUE obj)
{
  int result  = ATRSHMLOG_GET_THREAD_FENCE_5();

  return INT2NUM(result);
}

 VALUE atrshmlogruby_get_thread_fence_6 (VALUE obj)
{
  int result  = ATRSHMLOG_GET_THREAD_FENCE_6();

  return INT2NUM(result);
}

 VALUE atrshmlogruby_get_thread_fence_7 (VALUE obj)
{
  int result  = ATRSHMLOG_GET_THREAD_FENCE_7();

  return INT2NUM(result);
}

 VALUE atrshmlogruby_get_thread_fence_8 (VALUE obj)
{
  int result  = ATRSHMLOG_GET_THREAD_FENCE_8();

  return INT2NUM(result);
}

 VALUE atrshmlogruby_get_thread_fence_9 (VALUE obj)
{
  int result  = ATRSHMLOG_GET_THREAD_FENCE_9();

  return INT2NUM(result);
}

 VALUE atrshmlogruby_get_thread_fence_10 (VALUE obj)
{
  int result  = ATRSHMLOG_GET_THREAD_FENCE_10();

  return INT2NUM(result);
}

 VALUE atrshmlogruby_get_thread_fence_11 (VALUE obj)
{
  int result  = ATRSHMLOG_GET_THREAD_FENCE_11();

  return INT2NUM(result);
}

 VALUE atrshmlogruby_get_thread_fence_12 (VALUE obj)
{
  int result  = ATRSHMLOG_GET_THREAD_FENCE_12();

  return INT2NUM(result);
}

 VALUE atrshmlogruby_get_thread_fence_13 (VALUE obj)
{
  int result  = ATRSHMLOG_GET_THREAD_FENCE_13();

  return INT2NUM(result);
}

				
 VALUE atrshmlogruby_get_realtime (VALUE obj)
{
  atrshmlog_internal_time_t t = ATRSHMLOG_GET_REALTIME();

  int size = 2;
  
  VALUE ret = rb_ary_new2(size);

  rb_ary_store(ret, 0, LONG2NUM(t.tv_sec));
  rb_ary_store(ret, 1, INT2NUM(t.tv_nsec));
  
  return ret;
}


 VALUE atrshmlogruby_get_thread_locals_adress (VALUE obj)
{
  u_t u;

  u.p = ATRSHMLOG_GET_THREAD_LOCALS_ADRESS();
  
  return ULONG2NUM(u.l);
}

				
 VALUE atrshmlogruby_get_tid (VALUE obj)
{
  unsigned long result = ATRSHMLOG_GET_TID();

  return ULONG2NUM(result);
}

				
 VALUE atrshmlogruby_turn_logging_off (VALUE obj,
				    VALUE tl)
{
  u_t u;

  u.l = NUM2ULONG(tl);
  
  ATRSHMLOG_TURN_LOGGING_OFF(u.p);

  int result = 0;

  return INT2NUM(result);
}


 VALUE atrshmlogruby_set_init_buffers_in_advance_on (VALUE obj)
{
  int result = ATRSHMLOG_SET_INIT_BUFFERS_IN_ADVANCE_ON();

  return INT2NUM(result);
}

				
 VALUE atrshmlogruby_set_init_buffers_in_advance_off (VALUE obj)
{
  int result = ATRSHMLOG_SET_INIT_BUFFERS_IN_ADVANCE_OFF();

  return INT2NUM(result);
}

				
 VALUE atrshmlogruby_get_init_buffers_in_advance (VALUE obj)
{
  int result = ATRSHMLOG_GET_INIT_BUFFERS_IN_ADVANCE();

  return INT2NUM(result);
}

				

 VALUE atrshmlogruby_get_next_slave_local (VALUE obj,
					VALUE sl)
{
  u_t u;

  u.l = NUM2ULONG(sl);
  
  u.p = ATRSHMLOG_GET_NEXT_SLAVE_LOCAL(u.p);

  return ULONG2NUM(u.l);
}

				
 VALUE atrshmlogruby_get_thread_local_tid (VALUE obj,
					VALUE tl)
{
  u_t u;

  u.l = NUM2ULONG(tl);
  
  unsigned long result = ATRSHMLOG_GET_THREAD_LOCAL_TID(u.p);

  return ULONG2NUM(result);
}


 VALUE atrshmlogruby_remove_slave_via_local (VALUE obj,
					  VALUE sl)
{
  u_t u;

  u.l = NUM2ULONG(sl);
  
  int result = ATRSHMLOG_REMOVE_SLAVE_VIA_LOCAL(u.p);

  return INT2NUM(result);
}

				
 VALUE atrshmlogruby_reuse_thread_buffers (VALUE obj,
					VALUE tid)
{
  unsigned long t = NUM2ULONG(tid);
  
  int result = ATRSHMLOG_REUSE_THREAD_BUFFERS(t);

  return INT2NUM(result);
}

_Thread_local static char *read_buffer = 0; 

				
 VALUE atrshmlogruby_read (VALUE obj,
			VALUE area,
			VALUE index)
{
  atrshmlog_ret_t ret;
  atrshmlog_int32_t length;			  
  atrshmlog_pid_t pid;	    
  atrshmlog_tid_t tid;
  atrshmlog_internal_time_t inittime;
  atrshmlog_time_t inittimetsc_before;
  atrshmlog_time_t inittimetsc_after;
  atrshmlog_internal_time_t lasttime;
  atrshmlog_time_t lasttimetsc_before;
  atrshmlog_time_t lasttimetsc_after;
  atrshmlog_time_t difftimetransfer;
  atrshmlog_time_t starttransfer;
  atrshmlog_time_t acquiretime;		  
  atrshmlog_int32_t id;
  atrshmlog_int32_t number_dispatched;
  atrshmlog_int32_t counter_write0;
  atrshmlog_int32_t counter_write0_discard;
  atrshmlog_int32_t counter_write0_wait;
  atrshmlog_int32_t counter_write0_adaptive;
  atrshmlog_int32_t counter_write0_adaptive_fast;
  atrshmlog_int32_t counter_write0_adaptive_very_fast;
  atrshmlog_int32_t counter_write1;
  atrshmlog_int32_t counter_write1_discard;
  atrshmlog_int32_t counter_write1_wait;
  atrshmlog_int32_t counter_write1_adaptive;
  atrshmlog_int32_t counter_write1_adaptive_fast;
  atrshmlog_int32_t counter_write1_adaptive_very_fast;
  atrshmlog_int32_t counter_write2;			
  atrshmlog_int32_t counter_write2_discard;
  atrshmlog_int32_t counter_write2_wait;
  atrshmlog_int32_t counter_write2_adaptive;
  atrshmlog_int32_t counter_write2_adaptive_fast;
  atrshmlog_int32_t counter_write2_adaptive_very_fast;

  u_t u;

  u.l = NUM2ULONG(area);
  
  int in = NUM2INT(index);
  
  if (read_buffer == 0)
    read_buffer = malloc(1000000);
  
  ret = ATRSHMLOG_READ(u.p,
		       in,
		       read_buffer,
		       &length,
		       &pid,
		       &tid,
		       &inittime,
		       &inittimetsc_before,
		       &inittimetsc_after,
		       &lasttime,
		       &lasttimetsc_before,
		       &lasttimetsc_after,
		       &difftimetransfer,
		       &starttransfer,
		       &acquiretime,
		       &id,
		       &number_dispatched,
		       &counter_write0,
		       &counter_write0_discard,
		       &counter_write0_wait,
		       &counter_write0_adaptive,
		       &counter_write0_adaptive_fast,
		       &counter_write0_adaptive_very_fast,
		       &counter_write1,
		       &counter_write1_discard,
		       &counter_write1_wait,
		       &counter_write1_adaptive,
		       &counter_write1_adaptive_fast,
		       &counter_write1_adaptive_very_fast,
		       &counter_write2,
		       &counter_write2_discard,
		       &counter_write2_wait,
		       &counter_write2_adaptive,
		       &counter_write2_adaptive_fast,
		       &counter_write2_adaptive_very_fast);
  

  if (ret < 0)
    {
      VALUE retarr = rb_ary_new2(1);
      rb_ary_store(retarr, 0, INT2NUM(ret));

      return retarr;
    }

  if( ret > 0)
    {
      VALUE retarr = rb_ary_new2(1);
      rb_ary_store(retarr, 0, INT2NUM(ret));

      return retarr;
    }

  // ok. we have data
  VALUE retarr = rb_ary_new2( 36 );
  
  rb_ary_store(retarr, 0, INT2NUM(ret));

  VALUE s = rb_str_new(read_buffer, length);
  
  rb_ary_store(retarr, 1, s);
  rb_ary_store(retarr, 2, INT2NUM(length));
  rb_ary_store(retarr, 3, ULONG2NUM(pid));
  rb_ary_store(retarr, 4, ULONG2NUM(tid));
  rb_ary_store(retarr, 5, ULONG2NUM(inittime.tv_sec));
  rb_ary_store(retarr, 6, INT2NUM(inittime.tv_nsec));
  rb_ary_store(retarr, 7, ULONG2NUM(inittimetsc_before));
  rb_ary_store(retarr, 8, ULONG2NUM(inittimetsc_after));
  rb_ary_store(retarr, 9, ULONG2NUM(lasttime.tv_sec));
  rb_ary_store(retarr, 10, INT2NUM(lasttime.tv_nsec));
  rb_ary_store(retarr, 11, ULONG2NUM(lasttimetsc_before));
  rb_ary_store(retarr, 12, ULONG2NUM(lasttimetsc_after));
  rb_ary_store(retarr, 13, ULONG2NUM(difftimetransfer));
  rb_ary_store(retarr, 14, ULONG2NUM(starttransfer));
  rb_ary_store(retarr, 15, ULONG2NUM(acquiretime));
  rb_ary_store(retarr, 16, INT2NUM(id));
  rb_ary_store(retarr, 17, INT2NUM(number_dispatched));
  rb_ary_store(retarr, 18, INT2NUM(counter_write0));
  rb_ary_store(retarr, 19, INT2NUM(counter_write0_discard));
  rb_ary_store(retarr, 20, INT2NUM(counter_write0_wait));
  rb_ary_store(retarr, 21, INT2NUM(counter_write0_adaptive));
  rb_ary_store(retarr, 22, INT2NUM(counter_write0_adaptive_fast));
  rb_ary_store(retarr, 23, INT2NUM(counter_write0_adaptive_very_fast));
  rb_ary_store(retarr, 24, INT2NUM(counter_write1));
  rb_ary_store(retarr, 25, INT2NUM(counter_write1_discard));
  rb_ary_store(retarr, 26, INT2NUM(counter_write1_wait));
  rb_ary_store(retarr, 27, INT2NUM(counter_write1_adaptive));
  rb_ary_store(retarr, 28, INT2NUM(counter_write1_adaptive_fast));
  rb_ary_store(retarr, 29, INT2NUM(counter_write1_adaptive_very_fast));
  rb_ary_store(retarr, 30, INT2NUM(counter_write2));
  rb_ary_store(retarr, 31, INT2NUM(counter_write2_discard));
  rb_ary_store(retarr, 32, INT2NUM(counter_write2_wait));
  rb_ary_store(retarr, 33, INT2NUM(counter_write2_adaptive));
  rb_ary_store(retarr, 34, INT2NUM(counter_write2_adaptive_fast));
  rb_ary_store(retarr, 35, INT2NUM(counter_write2_adaptive_very_fast));
  

  return retarr;
}

				
 VALUE atrshmlogruby_read_fetch (VALUE obj,
			      VALUE area)
{
  atrshmlog_ret_t ret;
  atrshmlog_int32_t index;			  
  atrshmlog_int32_t length;			  
  atrshmlog_pid_t pid;	    
  atrshmlog_tid_t tid;
  atrshmlog_internal_time_t inittime;
  atrshmlog_time_t inittimetsc_before;
  atrshmlog_time_t inittimetsc_after;
  atrshmlog_internal_time_t lasttime;
  atrshmlog_time_t lasttimetsc_before;
  atrshmlog_time_t lasttimetsc_after;
  atrshmlog_time_t difftimetransfer;
  atrshmlog_time_t starttransfer;
  atrshmlog_time_t acquiretime;		  
  atrshmlog_int32_t id;
  atrshmlog_int32_t number_dispatched;
  atrshmlog_int32_t counter_write0;
  atrshmlog_int32_t counter_write0_discard;
  atrshmlog_int32_t counter_write0_wait;
  atrshmlog_int32_t counter_write0_adaptive;
  atrshmlog_int32_t counter_write0_adaptive_fast;
  atrshmlog_int32_t counter_write0_adaptive_very_fast;
  atrshmlog_int32_t counter_write1;
  atrshmlog_int32_t counter_write1_discard;
  atrshmlog_int32_t counter_write1_wait;
  atrshmlog_int32_t counter_write1_adaptive;
  atrshmlog_int32_t counter_write1_adaptive_fast;
  atrshmlog_int32_t counter_write1_adaptive_very_fast;
  atrshmlog_int32_t counter_write2;			
  atrshmlog_int32_t counter_write2_discard;
  atrshmlog_int32_t counter_write2_wait;
  atrshmlog_int32_t counter_write2_adaptive;
  atrshmlog_int32_t counter_write2_adaptive_fast;
  atrshmlog_int32_t counter_write2_adaptive_very_fast;

  u_t u;

  u.l = NUM2ULONG(area);
  
  if (read_buffer == 0)
    read_buffer = malloc(1000000);
  
  ret = ATRSHMLOG_READ_FETCH(u.p,
		       &index,
		       read_buffer,
		       &length,
		       &pid,
		       &tid,
		       &inittime,
		       &inittimetsc_before,
		       &inittimetsc_after,
		       &lasttime,
		       &lasttimetsc_before,
		       &lasttimetsc_after,
		       &difftimetransfer,
		       &starttransfer,
		       &acquiretime,
		       &id,
		       &number_dispatched,
		       &counter_write0,
		       &counter_write0_discard,
		       &counter_write0_wait,
		       &counter_write0_adaptive,
		       &counter_write0_adaptive_fast,
		       &counter_write0_adaptive_very_fast,
		       &counter_write1,
		       &counter_write1_discard,
		       &counter_write1_wait,
		       &counter_write1_adaptive,
		       &counter_write1_adaptive_fast,
		       &counter_write1_adaptive_very_fast,
		       &counter_write2,
		       &counter_write2_discard,
		       &counter_write2_wait,
		       &counter_write2_adaptive,
		       &counter_write2_adaptive_fast,
		       &counter_write2_adaptive_very_fast);
  

  if (ret < 0)
    {
      VALUE retarr = rb_ary_new2(1);
      rb_ary_store(retarr, 0, INT2NUM(ret));

      return retarr;
    }

  if( ret > 0)
    {
      VALUE retarr = rb_ary_new2(1);
      rb_ary_store(retarr, 0, INT2NUM(ret));

      return retarr;
    }

  if (length < 1)
    {
      VALUE retarr = rb_ary_new2(4);
      rb_ary_store(retarr, 0, INT2NUM(ret));
      rb_ary_store(retarr, 1, INT2NUM(index));
      rb_ary_store(retarr, 2, INT2NUM(0));
      rb_ary_store(retarr, 3, INT2NUM(length));
      return retarr;
    }
  
  // ok. we have data
  VALUE retarr = rb_ary_new2( 37 );
  
  rb_ary_store(retarr, 0, INT2NUM(ret));

  rb_ary_store(retarr, 1, INT2NUM(index));
  
  VALUE s = rb_str_new(read_buffer, length);
  
  rb_ary_store(retarr, 2, s);
  rb_ary_store(retarr, 3, INT2NUM(length));
  rb_ary_store(retarr, 4, ULONG2NUM(pid));
  rb_ary_store(retarr, 5, ULONG2NUM(tid));
  rb_ary_store(retarr, 6, ULONG2NUM(inittime.tv_sec));
  rb_ary_store(retarr, 7, INT2NUM(inittime.tv_nsec));
  rb_ary_store(retarr, 8, ULONG2NUM(inittimetsc_before));
  rb_ary_store(retarr, 9, ULONG2NUM(inittimetsc_after));
  rb_ary_store(retarr, 10, ULONG2NUM(lasttime.tv_sec));
  rb_ary_store(retarr, 11, INT2NUM(lasttime.tv_nsec));
  rb_ary_store(retarr, 12, ULONG2NUM(lasttimetsc_before));
  rb_ary_store(retarr, 13, ULONG2NUM(lasttimetsc_after));
  rb_ary_store(retarr, 14, ULONG2NUM(difftimetransfer));
  rb_ary_store(retarr, 15, ULONG2NUM(starttransfer));
  rb_ary_store(retarr, 16, ULONG2NUM(acquiretime));
  rb_ary_store(retarr, 17, INT2NUM(id));
  rb_ary_store(retarr, 18, INT2NUM(number_dispatched));
  rb_ary_store(retarr, 19, INT2NUM(counter_write0));
  rb_ary_store(retarr, 20, INT2NUM(counter_write0_discard));
  rb_ary_store(retarr, 21, INT2NUM(counter_write0_wait));
  rb_ary_store(retarr, 22, INT2NUM(counter_write0_adaptive));
  rb_ary_store(retarr, 23, INT2NUM(counter_write0_adaptive_fast));
  rb_ary_store(retarr, 24, INT2NUM(counter_write0_adaptive_very_fast));
  rb_ary_store(retarr, 25, INT2NUM(counter_write1));
  rb_ary_store(retarr, 26, INT2NUM(counter_write1_discard));
  rb_ary_store(retarr, 27, INT2NUM(counter_write1_wait));
  rb_ary_store(retarr, 28, INT2NUM(counter_write1_adaptive));
  rb_ary_store(retarr, 29, INT2NUM(counter_write1_adaptive_fast));
  rb_ary_store(retarr, 30, INT2NUM(counter_write1_adaptive_very_fast));
  rb_ary_store(retarr, 31, INT2NUM(counter_write2));
  rb_ary_store(retarr, 32, INT2NUM(counter_write2_discard));
  rb_ary_store(retarr, 33, INT2NUM(counter_write2_wait));
  rb_ary_store(retarr, 34, INT2NUM(counter_write2_adaptive));
  rb_ary_store(retarr, 35, INT2NUM(counter_write2_adaptive_fast));
  rb_ary_store(retarr, 36, INT2NUM(counter_write2_adaptive_very_fast));
  

  return retarr;
}

				
 VALUE atrshmlogruby_verify (VALUE obj)
{
  int result = ATRSHMLOG_VERIFY();

  return INT2NUM(result);
}

				
 VALUE atrshmlogruby_create (VALUE obj,
			  VALUE ipckey,
			  VALUE count)
{
  int i = NUM2INT(ipckey);
  int c = NUM2INT(count);
  
  int result = ATRSHMLOG_CREATE(i,c);

  return INT2NUM(result);
}

				
 VALUE atrshmlogruby_delete (VALUE obj,
			  VALUE id)
{
  int i = NUM2INT(id);
  
  int result = ATRSHMLOG_DELETE(i);

  return INT2NUM(result);
}

				
 VALUE atrshmlogruby_cleanup_locks (VALUE obj,
				 VALUE area)
{
  u_t u;

  u.l = NUM2ULONG(area);
  
  ATRSHMLOG_CLEANUP_LOCKS(u.p);

  int result = 0;

  return INT2NUM(result);
}

				
 VALUE atrshmlogruby_init_shm_log (VALUE obj,
				VALUE area,
				VALUE count)
{
  u_t u;

  u.l = NUM2ULONG(area);

  int c = NUM2INT(count);
  
  int result = ATRSHMLOG_INIT_SHM_LOG(u.p, c);

  return INT2NUM(result);
}

				

 VALUE atrshmlogruby_poke (VALUE obj,
			VALUE area,
			VALUE index,
			VALUE val)
{
  u_t u;

  u.l = NUM2ULONG(area);

  int in = NUM2INT(index);

  int v = NUM2INT(val);
  
  unsigned char* p = (unsigned char*)u.p;

  p += in;
  
  int result = *p;

  *p = (unsigned char)v;

  return INT2NUM(result);
}


 VALUE atrshmlogruby_peek (VALUE obj,
			VALUE area,
			VALUE index)
{
  u_t u;

  u.l = NUM2ULONG(area);

  int in = NUM2INT(index);
  
  unsigned char* p = (unsigned char*)u.p;

  p += in;
  
  int result = *p;

  return INT2NUM(result);
}

				
 VALUE atrshmlogruby_get_slave_tid (VALUE obj,
				 VALUE sl)
{
  u_t u;

  u.l = NUM2ULONG(sl);
  
  unsigned long result = ATRSHMLOG_GET_SLAVE_TID(u.p);

  return ULONG2NUM(result);
}


				
 VALUE atrshmlogruby_turn_slave_off (VALUE obj,
				  VALUE sl)
{
  u_t u;

  u.l = NUM2ULONG(sl);
  
  ATRSHMLOG_TURN_SLAVE_OFF(u.p);
  
  int result = 0;

  return INT2NUM(result);
}

				
 VALUE atrshmlogruby_set_autoflush_process (VALUE obj,
					 VALUE flag)
{
  int f = NUM2INT(flag);
  
  int result = ATRSHMLOG_SET_AUTOFLUSH_PROCESS(f);

  return INT2NUM(result);
}


 VALUE atrshmlogruby_get_autoflush_process (VALUE obj)
{
  int result = ATRSHMLOG_GET_AUTOFLUSH_PROCESS();

  return INT2NUM(result);
}

				
 VALUE atrshmlogruby_set_autoflush (VALUE obj,
				 VALUE flag)
{
  int f = NUM2INT(flag);
  
  int result = ATRSHMLOG_SET_AUTOFLUSH(f);

  return INT2NUM(result);
}


 VALUE atrshmlogruby_get_autoflush (VALUE obj)
{
  int result = ATRSHMLOG_GET_AUTOFLUSH();

  return INT2NUM(result);
}

				
 VALUE atrshmlogruby_set_checksum (VALUE obj,
				VALUE flag)
{
  int f = NUM2INT(flag);
  
  int result = ATRSHMLOG_SET_CHECKSUM(f);

  return INT2NUM(result);
}


 VALUE atrshmlogruby_get_checksum (VALUE obj)
{
  int result = ATRSHMLOG_GET_CHECKSUM();

  return INT2NUM(result);
}

				
 VALUE atrshmlogruby_detach (VALUE obj)
{
  int result = ATRSHMLOG_DETACH();

  return INT2NUM(result);
}


 VALUE atrshmlogruby_reattach (VALUE obj,
			       VALUE args)
{
  atrshmlog_int32_t pi[100];

  int len = RARRAY_LEN(args);

  int i;
  
  for (i = 0; i < len; i++)
    pi[i] = NUM2INT(rb_ary_entry(args, i));

  /* version 1.2.0 */
  for (; i < 56; i++)
    pi[i] = 0;
  
  int result =  ATRSHMLOG_REATTACH(pi);

  return INT2NUM(result);
}


 VALUE atrshmlogruby_get_strategy_wait_wait_time (VALUE obj)
{
  int result = ATRSHMLOG_GET_STRATEGY_WAIT_WAIT_TIME();

  return INT2NUM(result);
}


 VALUE atrshmlogruby_set_strategy_wait_wait_time (VALUE obj,
					       VALUE nanos)
{
  int n = NUM2INT(nanos);
  
  int result = ATRSHMLOG_SET_STRATEGY_WAIT_WAIT_TIME(n);

  return INT2NUM(result);
}

				
 VALUE atrshmlogruby_get_thread_local_pid (VALUE obj,
					VALUE tl)
{
  u_t u;
  
  u.l = NUM2ULONG(tl);
  
  int result = ATRSHMLOG_GET_THREAD_LOCAL_PID(u.p);

  return INT2NUM(result);
}

				
 VALUE atrshmlogruby_get_thread_local_index (VALUE obj,
					  VALUE tl)
{
  u_t u;
  
  u.l = NUM2ULONG(tl);
  
  int result = ATRSHMLOG_GET_THREAD_LOCAL_INDEX(u.p);

  return INT2NUM(result);
}


 VALUE atrshmlogruby_get_thread_local_buffer (VALUE obj,
					   VALUE tl,
					   VALUE index)
{
  u_t u;
  
  u.l = NUM2ULONG(tl);

  int in = NUM2INT(index);

  u.p = ATRSHMLOG_GET_THREAD_LOCAL_BUFFER(u.p, in);
  
  return ULONG2NUM(u.l);
}

				
				
 VALUE atrshmlogruby_get_thread_buffer_next_cleanup (VALUE obj,
						  VALUE buf)
{
  u_t u;
  
  u.l = NUM2ULONG(buf);
  
  u.p = ATRSHMLOG_GET_THREAD_BUFFER_NEXT_CLEANUP(u.p);

  return ULONG2NUM(u.l);
}

				
 VALUE atrshmlogruby_get_thread_buffer_next_full (VALUE obj,
						  VALUE buf)
{
  u_t u;
  
  u.l = NUM2ULONG(buf);
  
  u.p = ATRSHMLOG_GET_THREAD_BUFFER_NEXT_FULL(u.p);

  return ULONG2NUM(u.l);
}


 VALUE atrshmlogruby_get_thread_buffer_next_append (VALUE obj,
						  VALUE buf)
{
  u_t u;
  
  u.l = NUM2ULONG(buf);
  
  u.p = ATRSHMLOG_GET_THREAD_BUFFER_NEXT_APPEND(u.p);

  return ULONG2NUM(u.l);
}

				

 VALUE atrshmlogruby_get_thread_buffer_safeguard (VALUE obj,
					       VALUE buf)
{
  u_t u;
  
  u.l = NUM2ULONG(buf);
  
  int result = ATRSHMLOG_GET_THREAD_BUFFER_SAFEGUARD(u.p);

  return INT2NUM(result);
}

				
				
 VALUE atrshmlogruby_get_thread_buffer_pid (VALUE obj,
					 VALUE buf)
{
  u_t u;
  
  u.l = NUM2ULONG(buf);
  
  int result = ATRSHMLOG_GET_THREAD_BUFFER_PID(u.p);

  return INT2NUM(result);
}


 VALUE atrshmlogruby_get_thread_buffer_tid (VALUE obj,
					 VALUE buf)
{
  u_t u;
  
  u.l = NUM2ULONG(buf);
  
  int result = ATRSHMLOG_GET_THREAD_BUFFER_TID(u.p);

  return INT2NUM(result);
}

				
 VALUE atrshmlogruby_get_thread_buffer_acquiretime (VALUE obj,
						 VALUE buf)
{
  u_t u;
  
  u.l = NUM2ULONG(buf);
  
  int result = ATRSHMLOG_GET_THREAD_BUFFER_ACQUIRETIME(u.p);

  return INT2NUM(result);
}


 VALUE atrshmlogruby_get_thread_buffer_id (VALUE obj,
					VALUE buf)
{
  u_t u;
  
  u.l = NUM2ULONG(buf);
  
  int result = ATRSHMLOG_GET_THREAD_BUFFER_ID(u.p);

  return INT2NUM(result);
}

				
 VALUE atrshmlogruby_get_thread_buffer_chksum (VALUE obj,
					    VALUE buf)
{
  u_t u;
  
  u.l = NUM2ULONG(buf);
  
  int result = ATRSHMLOG_GET_THREAD_BUFFER_CHKSUM(u.p);

  return INT2NUM(result);
}


 VALUE atrshmlogruby_get_thread_buffer_size (VALUE obj,
					  VALUE buf)
{
  u_t u;
  
  u.l = NUM2ULONG(buf);
  
  int result = ATRSHMLOG_GET_THREAD_BUFFER_SIZE(u.p);

  return INT2NUM(result);
}

				
 VALUE atrshmlogruby_get_thread_buffer_maxsize (VALUE obj,
					     VALUE buf)
{
  u_t u;
  
  u.l = NUM2ULONG(buf);
  
  int result = ATRSHMLOG_GET_THREAD_BUFFER_MAXSIZE(u.p);

  return INT2NUM(result);
}

				
 VALUE atrshmlogruby_get_thread_buffer_dispose (VALUE obj,
					     VALUE buf)
{
  u_t u;
  
  u.l = NUM2ULONG(buf);
  
  int result = ATRSHMLOG_GET_THREAD_BUFFER_DISPOSE(u.p);

  return INT2NUM(result);
}


 VALUE atrshmlogruby_get_thread_buffer_dispatched (VALUE obj,
						VALUE buf)
{
  u_t u;
  
  u.l = NUM2ULONG(buf);
  
  int result = ATRSHMLOG_GET_THREAD_BUFFER_DISPATCHED(u.p);

  return INT2NUM(result);
}


 VALUE atrshmlogruby_get_thread_buffer_payload (VALUE obj,
					     VALUE buf)
{
  u_t u;
  
  u.l = NUM2ULONG(buf);
  
  u.p = ATRSHMLOG_GET_THREAD_BUFFER_PAYLOAD(u.p);

  return ULONG2NUM(u.l);
}


 VALUE atrshmlogruby_get_slave_to_shm_wait (VALUE obj)
{
  int result = ATRSHMLOG_GET_SLAVE_TO_SHM_WAIT();

  return INT2NUM(result);
}

				
 VALUE atrshmlogruby_set_slave_to_shm_wait (VALUE obj,
					 VALUE nanos)
{
  int n = NUM2INT(nanos);
  
  int result = ATRSHMLOG_SET_SLAVE_TO_SHM_WAIT(n);

  return INT2NUM(result);
}

				
 VALUE atrshmlogruby_get_last_mem_to_shm (VALUE obj)
				
{
  int result = ATRSHMLOG_GET_LAST_MEM_TO_SHM();

  return INT2NUM(result);
}

				
 VALUE atrshmlogruby_get_buffer_cleanup_anchor (VALUE obj)
{
  u_t u;
  
  u.p = ATRSHMLOG_GET_BUFFER_CLEANUP_ANCHOR();

  return ULONG2NUM(u.l);
}

				
 VALUE atrshmlogruby_get_buffer_full_anchor (VALUE obj)
{
  u_t u;
  
  u.p = ATRSHMLOG_GET_BUFFER_FULL_ANCHOR();

  return ULONG2NUM(u.l);
}


 VALUE atrshmlogruby_get_buffer_append_anchor (VALUE obj)
{
  u_t u;
  
  u.p = ATRSHMLOG_GET_BUFFER_APPEND_ANCHOR();

  return ULONG2NUM(u.l);
}

/* end of file */
