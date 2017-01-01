/** 
 * \file Atrshmlog.i
 *
 * \brief The atrshmlog swig file for perl.
 * 
 * We define a SWIG bridge here to use the module with perl.
 * 
 * See www.SWIG.org
 *
 */

/**
 * the module definition
 */
%module Atrshmlog

 /**
  * The stuff for the generated wrapper code file.
  */
%{

  /**
   * We include all
   */
#include "atrshmlog_internal.h"  

  /**
   * Prototypes for the helper functions
   */


extern atrshmlog_time_t atrshmlog_gettime(atrshmlog_int32_t event);

extern atrshmlog_ret_t atrshmlog_write_binary(const atrshmlog_int32_t i_eventnumber,
     const atrshmlog_int32_t i_eventflag,
     const atrshmlog_int32_t i_userflag,
     atrshmlog_time_t i_starttime,
     atrshmlog_time_t i_endtime,
     const char* i_local,
     const atrshmlog_int32_t i_size);
 
extern atrshmlog_ret_t atrshmlog_write(const atrshmlog_int32_t i_eventnumber,
     const atrshmlog_int32_t i_eventflag,
     const atrshmlog_int32_t i_userflag,
     atrshmlog_time_t i_starttime,
     atrshmlog_time_t i_endtime,
     const char* i_local);

extern void atratrshmlog_get_inittime(long* o_seconds, long* o_nanos);
 
extern void atratrshmlog_get_realtime(long* o_seconds, long* o_nanos);

extern void atratrshmlog_get_statistics(
					 int* o_s00,
					 int* o_s01,
					 int* o_s02,
					 int* o_s03,
					 int* o_s04,
					 int* o_s05,
					 int* o_s06,
					 int* o_s07,
					 int* o_s08,
					 int* o_s09,
					 int* o_s10,
					 int* o_s11,
					 int* o_s12,
					 int* o_s13,
					 int* o_s14,
					 int* o_s15,
					 int* o_s16,
					 int* o_s17,
					 int* o_s18,
					 int* o_s19,
					 int* o_s20,
					 int* o_s21,
					 int* o_s22,
					 int* o_s23,
					 int* o_s24,
					 int* o_s25,
					 int* o_s26,
					 int* o_s27,
					 int* o_s28,
					 int* o_s29,
					 int* o_s30,
					 int* o_s31,
					 int* o_s32,
					 int* o_s33,
					 int* o_s34,
					 int* o_s35,
					 int* o_s36,
					 int* o_s37,
					 int* o_s38,
					 int* o_s39,
					 int* o_s40,
					 int* o_s41,
					 int* o_s42,
					 int* o_s43,
					 int* o_s44,
					 int* o_s45,
					 int* o_s46,
					 int* o_s47,
					 int* o_s48,
					 int* o_s49,
					 int* o_s50,
					 int* o_s51,
					 int* o_s52,
					 int* o_s53,
					 int* o_s54,
					 int* o_s55,
					 int* o_s56,
					 int* o_s57,
					 int* o_s58,
					 int* o_s59,
					 int* o_s60,
					 int* o_s61,
					 int* o_s62,
					 int* o_s63,
					 int* o_s64,
					 int* o_s65,
					 int* o_s66,
					 int* o_s67,
					 int* o_s68,
					 int* o_s69,
					 int* o_s70,
					 int* o_s71,
					 int* o_s72,
					 int* o_s73,
					 int* o_s74,
					 int* o_s75,
					 int* o_s76,
					 int* o_s77,
					 int* o_s78,
					 int* o_s79,
					 int* o_s80,
					 int* o_s81,
					 int* o_s82,
					 int* o_s83,
					 int* o_s84,
					 int* o_s85,
					 int* o_s86,
					 int* o_s87,
					 int* o_s88,
					 int* o_s89,
					 int* o_s90,
					 int* o_s91,
					 int* o_s92,
					 int* o_s93,
					 int* o_s94,
					 int* o_s95,
					 int* o_s96,
					 int* o_s97,
					 int* o_s98,
					 int* o_s99);
 
 extern void atratrshmlog_read (volatile const void* area, 
				int index,
				int *ret,
				char *read_buffer, 
				int *read_length,		      
				long *pid,		     
				long *tid ,			
				long* inittime_seconds,	
				int* inittime_nanos,		   
				long* inittimetsc_before,	   
				long* inittimetsc_after,	   
				long* lasttime_seconds,		
				int* lasttime_nanos,			
				long* lasttimetsc_before,		
				long* lasttimetsc_after,		
				long* difftimetransfer,		
				long* starttransfer,			
				long* acquiretime,			
				int* id,				
				int* number_dispatched,		
				int* counter_write0,			
				int* counter_write0_discard,		
				int* counter_write0_wait,		
				int* counter_write0_adaptive,		
				int* counter_write0_adaptive_fast,	
				int* counter_write0_adaptive_very_fast, 
				int* counter_write1,			
				int* counter_write1_discard,		
				int* counter_write1_wait,		
				int* counter_write1_adaptive,		
				int* counter_write1_adaptive_fast,	
				int* counter_write1_adaptive_very_fast, 
				int* counter_write2,			
				int* counter_write2_discard,		
				int* counter_write2_wait,		
				int* counter_write2_adaptive,	       
				int* counter_write2_adaptive_fast,	
				int* counter_write2_adaptive_very_fast);


extern void atratrshmlog_read_fetch(volatile const void* area, 
				    int* ret,
				    int *index, 
				    char *read_buffer, 
				    int *read_length,			  
				    long *pid,	    
				    long *tid ,		       
				    long* inittime_seconds,	       
				    int* inittime_nanos,		 
				    long* inittimetsc_before,	  
				    long* inittimetsc_after,		  
				    long* lasttime_seconds,		  
				    int* lasttime_nanos,		  
				    long* lasttimetsc_before,	  
				    long* lasttimetsc_after,		  
				    long* difftimetransfer,		  
				    long* starttransfer,		  
				    long* acquiretime,		  
				    int* id,				  
				    int* number_dispatched,		  
				    int* counter_write0,		    
				    int* counter_write0_discard,	    
				    int* counter_write0_wait,	     
				    int* counter_write0_adaptive,		
				    int* counter_write0_adaptive_fast,	
				    int* counter_write0_adaptive_very_fast,	
				    int* counter_write1,			
				    int* counter_write1_discard,		
				    int* counter_write1_wait,		
				    int* counter_write1_adaptive,		
				    int* counter_write1_adaptive_fast,	
				    int* counter_write1_adaptive_very_fast,	
				    int* counter_write2,			
				    int* counter_write2_discard,		
				    int* counter_write2_wait,		
				    int* counter_write2_adaptive,		
				    int* counter_write2_adaptive_fast,	
				    int* counter_write2_adaptive_very_fast);

extern atrshmlog_ret_t atrshmlog_poke(volatile const void* area, int index, int value);

extern atrshmlog_ret_t atrshmlog_peek(volatile const void* area, int index);

extern int atratrshmlog_create(int key, int count);

extern atrshmlog_ret_t atratrshmlog_reattach(int flag0,
		       int id1,  
		       int flag2,  
		       int count3,  
		       int flag4,  
		       int iniinadvc5,  
		       int flag6,  
		       int bufferstrat7,  
		       int flag8,  
		       int wwtime9,  
		       int flag10,  
		       int delim11,  
		       int flag12,  
		       int evlocks13,  
		       int flag14,  
		       int buffinfosz15,  
		       int flag16,  
		       int pac17,  
		       int flag18,  
		       int slw19,  
		       int flag20,  
		       int slc21,  
		       int flag22,  
		       int wfs23,  
		       int flag24,  
		       int cid25,  
		       int flag26,  
		       int f1_27,  
		       int flag28,  
		       int f2_29,  
		       int flag30,  
		       int f3_31,  
		       int flag32,  
		       int f4_33,  
		       int flag34,  
		       int f5_35,  
		       int flag36,  
		       int f6_37,  
		       int flag38,  
		       int f7_39,  
		       int flag40,  
		       int f8_41,  
		       int flag42,  
		       int f9_43,  
		       int flag44,  
		       int f10_45,  
		       int flag46,  
		       int f11_47,  
		       int flag48,  
		       int f12_49,  
		       int flag50,  
		       int f13_51,  
		       int flag52,  
		       int chk53,  
		       int flag54,  
		       int loff55 
					     );
 
%}

/**
 * we need output here at least.
 */
%include typemaps.i 

 /**
  * we need the help for the return of binary via buffer
  */
%include cstring.i

 /**
  * Some things have to be repaired.
  */
%begin %{
     // we repair some stupid errors in our swig here ...
#define off64_t __off64_t
#define SWIG_From_wchar_t SWIG_From_int
%}


/* 
 * We ignore the following functions for now.
 * In case we need them we have a wrapper verion in place
 */

%ignore atrshmlog_write1; 
%ignore atrshmlog_write2; 
%ignore atrshmlog_get_statistics;
%ignore atrshmlog_get_inittime; 
%ignore atrshmlog_get_realtime; 
%ignore atrshmlog_read;         
%ignore atrshmlog_read_fetch;   
%ignore atrshmlog_reattach;   

// a dirty ignore for stdint included
%ignore _STDINT_H;

%ignore _FEATURES_H;

%ignore _DEFAULT_SOURCE;

%ignore __USE_ISOC11;

%ignore __USE_ISOC99;

%ignore __USE_ISOC95;

%ignore __USE_POSIX_IMPLICITLY;

%ignore _POSIX_SOURCE;

%ignore _POSIX_C_SOURCE;

%ignore __USE_POSIX;

%ignore __USE_POSIX2;

%ignore __USE_POSIX199309;

%ignore __USE_POSIX199506;

%ignore __USE_XOPEN2K;

%ignore __USE_XOPEN2K8;

%ignore _ATFILE_SOURCE;

%ignore __USE_MISC;

%ignore __USE_ATFILE;
   
%ignore __USE_FORTIFY_LEVEL;
   
%ignore _STDC_PREDEF_H;
   
%ignore __STDC_IEC_559__;
   
%ignore __STDC_IEC_559_COMPLEX__;
   
%ignore __STDC_ISO_10646__;
   
%ignore __STDC_NO_THREADS__;
   
%ignore __GNU_LIBRARY__;
   
%ignore __GLIBC__;
   
%ignore __GLIBC_MINOR__;
   
%ignore _SYS_CDEFS_H;
   
%ignore __WORDSIZE;
   
%ignore __WORDSIZE_TIME64_COMPAT32;
   
%ignore __SYSCALL_WORDSIZE;
   
%ignore _BITS_WCHAR_H;
   
%ignore __WCHAR_MAX;

%ignore __WCHAR_MIN;

%ignore INT8_MIN;
   
%ignore INT16_MIN;
   
%ignore INT32_MIN;
   
%ignore INT64_MIN;
   
%ignore INT8_MAX;
   
%ignore INT16_MAX;

%ignore INT32_MAX;
   
%ignore INT64_MAX;

%ignore UINT8_MAX;

%ignore UINT16_MAX;
   
%ignore UINT32_MAX;
   
%ignore UINT64_MAX;
   
%ignore INT_LEAST8_MIN;
   
%ignore INT_LEAST16_MIN;
   
%ignore INT_LEAST32_MIN;
   
%ignore INT_LEAST64_MIN;

%ignore INT_LEAST8_MAX;
   
%ignore INT_LEAST16_MAX;
   
%ignore INT_LEAST32_MAX;
   
%ignore INT_LEAST64_MAX;
   
%ignore UINT_LEAST8_MAX;
   
%ignore UINT_LEAST16_MAX;
   
%ignore UINT_LEAST32_MAX;
   
%ignore UINT_LEAST64_MAX;
   
%ignore INT_FAST8_MIN;
   
%ignore INT_FAST16_MIN;
   
%ignore INT_FAST32_MIN;
   
%ignore INT_FAST64_MIN;
   
%ignore INT_FAST8_MAX;
   
%ignore INT_FAST16_MAX;
   
%ignore INT_FAST32_MAX;
   
%ignore INT_FAST64_MAX;

%ignore UINT_FAST8_MAX;
   
%ignore UINT_FAST16_MAX;
   
%ignore UINT_FAST32_MAX;
   
%ignore UINT_FAST64_MAX;
   
%ignore INTPTR_MIN;
   
%ignore INTPTR_MAX;
   
%ignore UINTPTR_MAX;
   
%ignore INTMAX_MIN;
   
%ignore INTMAX_MAX;
   
%ignore UINTMAX_MAX;
   
%ignore PTRDIFF_MIN;
   
%ignore PTRDIFF_MAX;
   
%ignore SIG_ATOMIC_MIN;
   
%ignore SIG_ATOMIC_MAX;
   
%ignore SIZE_MAX;
   
%ignore WCHAR_MIN;
   
%ignore WCHAR_MAX;
   
%ignore WINT_MIN;
   
%ignore WINT_MAX;
   
%ignore UINT_FAST8_MAX;
   
// end of dirty ignore
   
   
%rename(attach) atrshmlog_attach;
%rename(get_statistics_max_index) atrshmlog_get_statistics_max_index;
%rename(get_statistics) atratrshmlog_get_statistics;
%rename(write0) atrshmlog_write0;
%rename(sleep_nanos) atrshmlog_sleep_nanos;
%rename(set_env_prefix) atrshmlog_set_env_prefix;
%rename(get_env_prefix) atrshmlog_get_env_prefix;
%rename(get_env) atrshmlog_get_env;
%rename(get_env_shmid) atrshmlog_get_env_shmid;
%rename(get_env_id_suffix) atrshmlog_get_env_id_suffix;
%rename(get_version) atrshmlog_get_version;
%rename(get_minor_version) atrshmlog_get_minor_version;
%rename(get_patch_version) atrshmlog_get_patch_version;
%rename(get_event_locks_max) atrshmlog_get_event_locks_max;
%rename(set_event_locks_max) atrshmlog_set_event_locks_max;
%rename(get_event) atrshmlog_get_event;
%rename(set_event_on) atrshmlog_set_event_on;
%rename(set_event_off) atrshmlog_set_event_off;
%rename(get_logging) atrshmlog_get_logging;
%rename(set_logging_process_on) atrshmlog_set_logging_process_on;
%rename(set_logging_process_off) atrshmlog_set_logging_process_off;
%rename(set_logging_process_off_final) atrshmlog_set_logging_process_off_final;
%rename(get_shmid) atrshmlog_get_shmid;
%rename(get_area) atrshmlog_get_area;
%rename(get_area_ich_habe_fertig) atrshmlog_get_area_ich_habe_fertig;
%rename(set_area_ich_habe_fertig) atrshmlog_set_area_ich_habe_fertig;
%rename(get_area_count) atrshmlog_get_area_count;
%rename(get_area_version) atrshmlog_get_area_version;
%rename(get_buffer_max_size) atrshmlog_get_buffer_max_size;
%rename(get_buffer_size) atrshmlog_get_buffer_size;
%rename(set_buffer_size) atrshmlog_set_buffer_size;
%rename(get_f_list_buffer_slave_count) atrshmlog_get_f_list_buffer_slave_count;
%rename(set_f_list_buffer_slave_count) atrshmlog_set_f_list_buffer_slave_count;
%rename(get_clock_id) atrshmlog_get_clock_id;
%rename(set_clock_id) atrshmlog_set_clock_id;
%rename(set_f_list_buffer_slave_run_off) atrshmlog_set_f_list_buffer_slave_run_off;
%rename(set_wait_for_slaves_on) atrshmlog_set_wait_for_slaves_on;
%rename(set_wait_for_slaves_off) atrshmlog_set_wait_for_slaves_off;
%rename(get_wait_for_slaves) atrshmlog_get_wait_for_slaves;
%rename(get_f_list_buffer_slave_wait) atrshmlog_get_f_list_buffer_slave_wait;
%rename(set_f_list_buffer_slave_wait) atrshmlog_set_f_list_buffer_slave_wait;
%rename(get_acquire_count) atrshmlog_get_acquire_count;
%rename(get_prealloc_buffer_count) atrshmlog_get_prealloc_buffer_count;
%rename(set_prealloc_buffer_count) atrshmlog_set_prealloc_buffer_count;
%rename(get_inittime_tsc_before) atrshmlog_get_inittime_tsc_before;
%rename(get_inittime_tsc_after) atrshmlog_get_inittime_tsc_after;
%rename(get_buffer_id) atrshmlog_get_buffer_id;
%rename(stop) atrshmlog_stop;
%rename(flush) atrshmlog_flush;
%rename(set_strategy) atrshmlog_set_strategy;
%rename(get_strategy) atrshmlog_get_strategy;
%rename(set_strategy_process) atrshmlog_set_strategy_process;
%rename(get_strategy_process) atrshmlog_get_strategy_process;
%rename(create_slave) atrshmlog_create_slave;
%rename(decrement_slave_count) atrshmlog_decrement_slave_count;
%rename(set_thread_fence_1) atrshmlog_set_thread_fence_1;
%rename(set_thread_fence_2) atrshmlog_set_thread_fence_2;
%rename(set_thread_fence_3) atrshmlog_set_thread_fence_3;
%rename(set_thread_fence_4) atrshmlog_set_thread_fence_4;
%rename(set_thread_fence_5) atrshmlog_set_thread_fence_5;
%rename(set_thread_fence_6) atrshmlog_set_thread_fence_6;
%rename(set_thread_fence_7) atrshmlog_set_thread_fence_7;
%rename(set_thread_fence_8) atrshmlog_set_thread_fence_8;
%rename(set_thread_fence_9) atrshmlog_set_thread_fence_9;
%rename(set_thread_fence_10) atrshmlog_set_thread_fence_10;
%rename(set_thread_fence_11) atrshmlog_set_thread_fence_11;
%rename(set_thread_fence_12) atrshmlog_set_thread_fence_12;
%rename(set_thread_fence_13) atrshmlog_set_thread_fence_13;
%rename(get_thread_fence_1) atrshmlog_get_thread_fence_1;
%rename(get_thread_fence_2) atrshmlog_get_thread_fence_2;
%rename(get_thread_fence_3) atrshmlog_get_thread_fence_3;
%rename(get_thread_fence_4) atrshmlog_get_thread_fence_4;
%rename(get_thread_fence_5) atrshmlog_get_thread_fence_5;
%rename(get_thread_fence_6) atrshmlog_get_thread_fence_6;
%rename(get_thread_fence_7) atrshmlog_get_thread_fence_7;
%rename(get_thread_fence_8) atrshmlog_get_thread_fence_8;
%rename(get_thread_fence_9) atrshmlog_get_thread_fence_9;
%rename(get_thread_fence_10) atrshmlog_get_thread_fence_10;
%rename(get_thread_fence_11) atrshmlog_get_thread_fence_11;
%rename(get_thread_fence_12) atrshmlog_get_thread_fence_12;
%rename(get_thread_fence_13) atrshmlog_get_thread_fence_13;
%rename(get_thread_locals_adress) atrshmlog_get_thread_locals_adress;
%rename(get_tid) atrshmlog_get_tid;
%rename(turn_logging_off) atrshmlog_turn_logging_off;
%rename(set_init_buffers_in_advance_on) atrshmlog_set_init_buffers_in_advance_on;
%rename(set_init_buffers_in_advance_off) atrshmlog_set_init_buffers_in_advance_off;
%rename(get_init_buffers_in_advance) atrshmlog_get_init_buffers_in_advance;
%rename(get_next_slave_local) atrshmlog_get_next_slave_local;
%rename(get_thread_local_tid) atrshmlog_get_thread_local_tid;
%rename(remove_slave_via_local) atrshmlog_remove_slave_via_local;
%rename(reuse_thread_buffers) atrshmlog_reuse_thread_buffers;
%rename(verify) atrshmlog_verify;
%rename(get_clicktime) atrshmlog_get_clicktime;
%rename(create) atratrshmlog_create;
%rename(delete) atrshmlog_delete;
%rename(cleanup_locks) atrshmlog_cleanup_locks;
%rename(init_shm_log) atrshmlog_init_shm_log;
%rename(gettime) atrshmlog_gettime;
%rename(write_binary) atrshmlog_write_binary;
%rename(write) atrshmlog_write;
%rename(get_inittime) atratrshmlog_get_inittime;  
%rename(get_realtime) atratrshmlog_get_realtime;  
%rename(read) atratrshmlog_read;
%rename(read_fetch) atratrshmlog_read_fetch;  
%rename(peek) atrshmlog_peek;  
%rename(poke) atrshmlog_poke;  
%rename(get_slave_tid) atrshmlog_get_slave_tid;
%rename(turn_slave_off) atrshmlog_turn_slave_off;
%rename(get_checksum) atrshmlog_get_checksum;  
%rename(get_autoflush) atrshmlog_get_autoflush;  
%rename(get_autoflush_process) atrshmlog_get_autoflush_process;  
%rename(set_checksum) atrshmlog_set_checksum;  
%rename(set_autoflush) atrshmlog_set_autoflush;  
%rename(set_autoflush_process) atrshmlog_set_autoflush_process;  
%rename(detach) atrshmlog_detach;  
%rename(reattach) atratrshmlog_reattach;  
%rename(get_strategy_wait_wait_time) atrshmlog_get_strategy_wait_wait_time;  
%rename(set_strategy_wait_wait_time) atrshmlog_set_strategy_wait_wait_time;  
%rename(get_thread_local_pid) atrshmlog_get_thread_local_pid;
%rename(get_thread_local_index) atrshmlog_get_thread_local_index;
%rename(get_thread_local_buffer) atrshmlog_get_thread_local_buffer;
%rename(get_thread_buffer_next_cleanup) atrshmlog_get_thread_buffer_next_cleanup;
%rename(get_thread_buffer_next_full) atrshmlog_get_thread_buffer_next_full;
%rename(get_thread_buffer_next_append) atrshmlog_get_thread_buffer_next_append;
%rename(get_thread_buffer_safeguard) atrshmlog_get_thread_buffer_safeguard;
%rename(get_thread_buffer_pid) atrshmlog_get_thread_buffer_pid;
%rename(get_thread_buffer_tid) atrshmlog_get_thread_buffer_tid;
%rename(get_thread_buffer_acquiretime) atrshmlog_get_thread_buffer_acquiretime;
%rename(get_thread_buffer_id) atrshmlog_get_thread_buffer_id;
%rename(get_thread_buffer_chksum) atrshmlog_get_thread_buffer_chksum;
%rename(get_thread_buffer_size) atrshmlog_get_thread_buffer_size;
%rename(get_thread_buffer_maxsize) atrshmlog_get_thread_buffer_maxsize;
%rename(get_thread_buffer_dispose) atrshmlog_get_thread_buffer_dispose;
%rename(get_thread_buffer_dispatched) atrshmlog_get_thread_buffer_dispatched;
%rename(get_thread_buffer_payload) atrshmlog_get_thread_buffer_payload;
%rename(get_slave_to_shm_wait) atrshmlog_get_slave_to_shm_wait;
%rename(set_slave_to_shm_wait) atrshmlog_set_slave_to_shm_wait;
%rename(get_last_mem_to_shm) atrshmlog_get_last_mem_to_shm;
%rename(get_buffer_cleanup_anchor) atrshmlog_get_buffer_cleanup_anchor;
%rename(get_buffer_full_anchor) atrshmlog_get_buffer_full_anchor;
%rename(get_buffer_append_anchor) atrshmlog_get_buffer_append_anchor;

%constant int EVENT_POINT_IN_TIME_C = 80;
%constant int EVENT_POINT_IN_TIME_UCS2 = 112;
%constant int EVENT_INTERVAL_IN_TIME_C = 73;
%constant int EVENT_INTERVAL_IN_TIME_UCS2 = 105;
   
#include "atrshmlog.h"  


/** 
 * \brief We destroy the shm 
 *
 * \param i_shmid
 * The shm id of the system
 *
 * \return
 * The error code of the delete
 */
extern atrshmlog_ret_t atrshmlog_delete(const int i_shmid);

/** 
 * \brief We have to clean up the locks after error condition 
 *
 * \param i_area
 * Points to the start of the area.
 *
 * \return
 * void
 */
extern void atrshmlog_cleanup_locks(volatile const void* i_area);

/** 
 * \brief We initialize the buffer after it is attached 
 *
 * \param i_area
 * Points to the start of the area.
 *
 * \param i_count_buffers
 * The number of buffers for the shm are.
 */
extern atrshmlog_ret_t atrshmlog_init_shm_log(volatile const void *i_area,
					      const atrshmlog_int32_t i_count_buffers);


extern atrshmlog_time_t atrshmlog_gettime(atrshmlog_int32_t event);

extern atrshmlog_ret_t atrshmlog_write_binary(const atrshmlog_int32_t i_eventnumber,
     const atrshmlog_int32_t i_eventflag,
     const atrshmlog_int32_t i_userflag,
     atrshmlog_time_t i_starttime,
     atrshmlog_time_t i_endtime,
     const char* i_local,
     const atrshmlog_int32_t i_size);
 
extern atrshmlog_ret_t atrshmlog_write(const atrshmlog_int32_t i_eventnumber,
     const atrshmlog_int32_t i_eventflag,
     const atrshmlog_int32_t i_userflag,
     atrshmlog_time_t i_starttime,
     atrshmlog_time_t i_endtime,
     const char* i_local);

 
extern void atratrshmlog_get_inittime(long *OUTPUT, long* OUTPUT);
				  
extern void atratrshmlog_get_realtime(long *OUTPUT, long* OUTPUT);

extern void atratrshmlog_get_statistics(
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT,
					 int* OUTPUT);
					 
%cstring_chunk_output(char *read_buffer, 1000000)
				  
 extern void atratrshmlog_read (volatile const void* area, 
				int index,
				int *OUTPUT,
				char *read_buffer, 
				int *OUTPUT,		      
				long *OUTPUT,		     
				long *OUTPUT,			
				long *OUTPUT,	
				int* OUTPUT,
				long* OUTPUT,
				long* OUTPUT,
				long* OUTPUT,
				int* OUTPUT,			
				long* OUTPUT,
				long* OUTPUT,
				long* OUTPUT,
				long* OUTPUT,
				long* OUTPUT,
				int* OUTPUT,
				int* OUTPUT,
				int* OUTPUT,
				int* OUTPUT,
				int* OUTPUT,
				int* OUTPUT,
				int* OUTPUT,
				int* OUTPUT,
				int* OUTPUT,
				int* OUTPUT,
				int* OUTPUT,
				int* OUTPUT,
				int* OUTPUT,
				int* OUTPUT,
				int* OUTPUT,
				int* OUTPUT,
				int* OUTPUT,
				int* OUTPUT,
				int* OUTPUT,
				int* OUTPUT);


extern void atratrshmlog_read_fetch(volatile const void* area, 
				    int* OUTPUT,
				    int *OUTPUT,
				    char *read_buffer, 
				    int *OUTPUT,
				    long *OUTPUT,
				    long *OUTPUT,
				    long* OUTPUT,
				    int* OUTPUT,
				    long* OUTPUT,
				    long* OUTPUT,
				    long* OUTPUT,
				    int* OUTPUT,
				    long* OUTPUT,
				    long* OUTPUT,
				    long* OUTPUT,
				    long* OUTPUT,
				    long* OUTPUT,
				    int* OUTPUT,
				    int* OUTPUT,
				    int* OUTPUT,
				    int* OUTPUT,
				    int* OUTPUT,
				    int* OUTPUT,
				    int* OUTPUT,
				    int* OUTPUT,
				    int* OUTPUT,
				    int* OUTPUT,
				    int* OUTPUT,
				    int* OUTPUT,
				    int* OUTPUT,
				    int* OUTPUT,
				    int* OUTPUT,
				    int* OUTPUT,
				    int* OUTPUT,
				    int* OUTPUT,
				    int* OUTPUT,
				    int* OUTPUT);
				     
				     
atrshmlog_ret_t atrshmlog_poke(volatile const void* area, int index, int value)
{
  atrshmlog_ret_t result;
  
  unsigned char* p = (unsigned char*)area;

  p += index;
  
  result = *p;

  *p = (unsigned char)value;
  
  return result;
}

atrshmlog_ret_t atrshmlog_peek(volatile const void* area, int index)
{
  atrshmlog_ret_t result;
  
  unsigned char* p = (unsigned char*)area;

  p += index;
  
  result = *p;

  return result;
}

extern int atratrshmlog_create(int key, int count);

extern atrshmlog_ret_t atratrshmlog_reattach(int flag0,
		       int id1,  
		       int flag2,  
		       int count3,  
		       int flag4,  
		       int iniinadvc5,  
		       int flag6,  
		       int bufferstrat7,  
		       int flag8,  
		       int wwtime9,  
		       int flag10,  
		       int delim11,  
		       int flag12,  
		       int evlocks13,  
		       int flag14,  
		       int buffinfosz15,  
		       int flag16,  
		       int pac17,  
		       int flag18,  
		       int slw19,  
		       int flag20,  
		       int slc21,  
		       int flag22,  
		       int wfs23,  
		       int flag24,  
		       int cid25,  
		       int flag26,  
		       int f1_27,  
		       int flag28,  
		       int f2_29,  
		       int flag30,  
		       int f3_31,  
		       int flag32,  
		       int f4_33,  
		       int flag34,  
		       int f5_35,  
		       int flag36,  
		       int f6_37,  
		       int flag38,  
		       int f7_39,  
		       int flag40,  
		       int f8_41,  
		       int flag42,  
		       int f9_43,  
		       int flag44,  
		       int f10_45,  
		       int flag46,  
		       int f11_47,  
		       int flag48,  
		       int f12_49,  
		       int flag50,  
		       int f13_51,  
		       int flag52,  
		       int chk53,  
		       int flag54,  
		       int loff55 
					     );
 

/* end of file */
