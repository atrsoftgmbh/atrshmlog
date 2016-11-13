/*
 * the python module for the log
 */

/** \file atrshmlogmodule.c
 * \brief We implement the python bridge for the atrshmlog module
 * 
 * We can do this in python 2 and python 3.
 *
 * Set the proper environment with the dot.python.sh script.
 *
 * If you try to use the module with a wrong python version
 * the module will throw invalid methods for the byte or string functions.
 *
 * We check at compiletime the patchlevel versions from the 
 * python against the version given in the environment.
 *
 */

/******************************************************/

/**
 * if we dont have it from environment via define we assume a version 3
 */
#ifndef ATRSHMLOG_PYTHON_VERSION
#define ATRSHMLOG_PYTHON_VERSION 3
#endif

/**
 * We check the version is supported. 
 */
#if ATRSHMLOG_PYTHON_VERSION == 2
#else
# if ATRSHMLOG_PYTHON_VERSION == 3
# else
bumm bumm bumm bumm ups no known version from environment
# endif
#endif


// the build check version defines
#include <patchlevel.h>

#if ATRSHMLOG_PYTHON_VERSION == PY_MAJOR_VERSION
// we use the clean size type here
# define PY_SSIZE_T_CLEAN

// we are a python module. YES, WE ARE. 
# include <Python.h>

// we deliver this for the use via captule
#define ATRSHMLOG_MODULE
#include "atrshmlogmodule.h"

// we use the logging module
# include "atrshmlog_internal.h"

#else
bumm bumm bumm bumm ups build version and environment differ
#endif


/******************************************************/


/**
 * \brief A little helper for the area things. 
 *
 * We convert this way buffer adresses to pointers and back
 */

union u_s {
  const volatile void * a;
  unsigned long long p;
};

typedef union u_s u_t;

/******************************************************/

/**
 * We use an error object for the module.
 *
 * Create an reference and hold it in init.
 */
static PyObject *AtrshmlogError;

/******************************************************/

/**
 * A helper from the vast and infinite net ...
 */
static PyObject *
makelist(pyatrshmlog_int32_t *array, size_t size)
{
    PyObject *l = PyList_New(size);

    if (l == NULL)
      return NULL;
    
    for (size_t i = 0; i != size; ++i) {
      PyObject * item = PyLong_FromLong(array[i]);

      if (item == NULL)
	return NULL;
      
      PyList_SET_ITEM(l, i, item);
    }
    
    return l;
}

/***********************************************************************/

/*
 * The core functions.
 *
 * The static implementation of the funcionality.
 *
 * This is a thin layer around the C module functios.
 * Only some small adjustments. They are here mainly
 * for the use in other modules via C API capsule.
 * And to change some internal types to the types used in the 
 * python bridge.
 */

static PyAtrshmlog_attach_RETURN PyAtrshmlog_attach PyAtrshmlog_attach_PROTO
{
  return ATRSHMLOG_ATTACH();
}

static PyAtrshmlog_gettime_RETURN PyAtrshmlog_gettime PyAtrshmlog_gettime_PROTO
{
  return ATRSHMLOG_GET_TSC_CALL();
}

static PyAtrshmlog_write0_RETURN PyAtrshmlog_write0 PyAtrshmlog_write0_PROTO
{
   return ATRSHMLOG_WRITE0(eventnumber, eventflag, userflag, starttime, endtime);
}

static PyAtrshmlog_write_RETURN PyAtrshmlog_write PyAtrshmlog_write_PROTO
{
  return ATRSHMLOG_WRITE(eventnumber, eventflag, userflag, starttime, endtime, payload, size);
}

static PyAtrshmlog_sleep_nanos_RETURN PyAtrshmlog_sleep_nanos PyAtrshmlog_sleep_nanos_PROTO
{
    ATRSHMLOG_SLEEP_NANOS(nanos);
}

static PyAtrshmlog_get_statistics_max_index_RETURN PyAtrshmlog_get_statistics_max_index PyAtrshmlog_get_statistics_max_index_PROTO
{
  return ATRSHMLOG_GET_STATISTICS_MAX_INDEX();
}

static PyAtrshmlog_get_statistics_RETURN PyAtrshmlog_get_statistics PyAtrshmlog_get_statistics_PROTO
{
  ATRSHMLOG_GET_STATISTICS(statistics);
}

static PyAtrshmlog_set_env_prefix_RETURN PyAtrshmlog_set_env_prefix PyAtrshmlog_set_env_prefix_PROTO
{
  ATRSHMLOG_SET_ENV_PREFIX(prefix);
}

static PyAtrshmlog_get_env_prefix_RETURN PyAtrshmlog_get_env_prefix PyAtrshmlog_get_env_prefix_PROTO
{
  return ATRSHMLOG_GET_ENV_PREFIX();
}

static PyAtrshmlog_get_env_RETURN PyAtrshmlog_get_env PyAtrshmlog_get_env_PROTO
{
  return ATRSHMLOG_GET_ENV(suffix);
}

static PyAtrshmlog_get_env_shmid_RETURN PyAtrshmlog_get_env_shmid PyAtrshmlog_get_env_shmid_PROTO
{
  return ATRSHMLOG_GET_ENV_SHMID();
}

static PyAtrshmlog_get_env_id_suffix_RETURN PyAtrshmlog_get_env_id_suffix PyAtrshmlog_get_env_id_suffix_PROTO
{
  return ATRSHMLOG_GET_ENV_ID_SUFFIX();
}

static PyAtrshmlog_get_version_RETURN PyAtrshmlog_get_version PyAtrshmlog_get_version_PROTO
{
  return ATRSHMLOG_GET_VERSION();
}

static PyAtrshmlog_get_minor_version_RETURN PyAtrshmlog_get_minor_version PyAtrshmlog_get_minor_version_PROTO
{
  return ATRSHMLOG_GET_MINOR_VERSION();
}

static PyAtrshmlog_get_patch_version_RETURN PyAtrshmlog_get_patch_version PyAtrshmlog_get_patch_version_PROTO
{
  return ATRSHMLOG_GET_PATCH_VERSION();
}

static PyAtrshmlog_get_event_locks_max_RETURN PyAtrshmlog_get_event_locks_max PyAtrshmlog_get_event_locks_max_PROTO
{
  return ATRSHMLOG_GET_EVENT_LOCKS_MAX();
}

static PyAtrshmlog_set_event_locks_max_RETURN PyAtrshmlog_set_event_locks_max PyAtrshmlog_set_event_locks_max_PROTO
{
  return ATRSHMLOG_SET_EVENT_LOCKS_MAX(newmax);
}

static PyAtrshmlog_get_event_RETURN PyAtrshmlog_get_event PyAtrshmlog_get_event_PROTO
{
  return ATRSHMLOG_GET_EVENT(event);
}

static PyAtrshmlog_set_event_on_RETURN PyAtrshmlog_set_event_on PyAtrshmlog_set_event_on_PROTO
{
  return ATRSHMLOG_SET_EVENT_ON(event);
}

static PyAtrshmlog_set_event_off_RETURN PyAtrshmlog_set_event_off PyAtrshmlog_set_event_off_PROTO
{
  return ATRSHMLOG_SET_EVENT_OFF(event);
}

static PyAtrshmlog_get_logging_RETURN PyAtrshmlog_get_logging PyAtrshmlog_get_logging_PROTO
{
  return ATRSHMLOG_GET_LOGGING();
}

static PyAtrshmlog_set_logging_process_on_RETURN PyAtrshmlog_set_logging_process_on PyAtrshmlog_set_logging_process_on_PROTO
{
  return ATRSHMLOG_SET_LOGGING_PROCESS_ON();
}

static PyAtrshmlog_set_logging_process_off_RETURN PyAtrshmlog_set_logging_process_off PyAtrshmlog_set_logging_process_off_PROTO
{
  return ATRSHMLOG_SET_LOGGING_PROCESS_OFF();
}

static PyAtrshmlog_set_logging_process_off_final_RETURN PyAtrshmlog_set_logging_process_off_final PyAtrshmlog_set_logging_process_off_final_PROTO
{
  return ATRSHMLOG_SET_LOGGING_PROCESS_OFF_FINAL();
}

static PyAtrshmlog_get_shmid_RETURN PyAtrshmlog_get_shmid PyAtrshmlog_get_shmid_PROTO
{
  return ATRSHMLOG_GET_SHMID();
}

static PyAtrshmlog_get_area_RETURN PyAtrshmlog_get_area PyAtrshmlog_get_area_PROTO
{
  return ATRSHMLOG_GET_AREA();
}

static PyAtrshmlog_get_area_ich_habe_fertig_RETURN PyAtrshmlog_get_area_ich_habe_fertig PyAtrshmlog_get_area_ich_habe_fertig_PROTO
{
  return ATRSHMLOG_GET_AREA_ICH_HABE_FERTIG(area);
}

static PyAtrshmlog_set_area_ich_habe_fertig_RETURN PyAtrshmlog_set_area_ich_habe_fertig PyAtrshmlog_set_area_ich_habe_fertig_PROTO
{
  return ATRSHMLOG_SET_AREA_ICH_HABE_FERTIG(area, flag);
}

static PyAtrshmlog_get_area_count_RETURN PyAtrshmlog_get_area_count PyAtrshmlog_get_area_count_PROTO
{
  return ATRSHMLOG_GET_AREA_COUNT(area);
}

static PyAtrshmlog_get_area_version_RETURN PyAtrshmlog_get_area_version PyAtrshmlog_get_area_version_PROTO
{
  return ATRSHMLOG_GET_AREA_VERSION(area);
}

static PyAtrshmlog_get_buffer_max_size_RETURN PyAtrshmlog_get_buffer_max_size PyAtrshmlog_get_buffer_max_size_PROTO
{
  return ATRSHMLOG_GET_BUFFER_MAX_SIZE();
}

static PyAtrshmlog_get_buffer_size_RETURN PyAtrshmlog_get_buffer_size PyAtrshmlog_get_buffer_size_PROTO
{
  return ATRSHMLOG_GET_BUFFER_SIZE();
}

static PyAtrshmlog_get_f_list_buffer_slave_count_RETURN PyAtrshmlog_get_f_list_buffer_slave_count PyAtrshmlog_get_f_list_buffer_slave_count_PROTO
{
  return ATRSHMLOG_GET_F_LIST_BUFFER_SLAVE_COUNT();
}

static PyAtrshmlog_set_buffer_size_RETURN PyAtrshmlog_set_buffer_size PyAtrshmlog_set_buffer_size_PROTO
{
  return ATRSHMLOG_SET_BUFFER_SIZE(size);
}

static PyAtrshmlog_set_f_list_buffer_slave_count_RETURN PyAtrshmlog_set_f_list_buffer_slave_count PyAtrshmlog_set_f_list_buffer_slave_count_PROTO
{
  return ATRSHMLOG_SET_F_LIST_BUFFER_SLAVE_COUNT(count);
}

static PyAtrshmlog_get_clock_id_RETURN PyAtrshmlog_get_clock_id PyAtrshmlog_get_clock_id_PROTO
{
  return ATRSHMLOG_GET_CLOCK_ID();
}

static PyAtrshmlog_set_clock_id_RETURN PyAtrshmlog_set_clock_id PyAtrshmlog_set_clock_id_PROTO
{
  return ATRSHMLOG_SET_CLOCK_ID(id);
}

static PyAtrshmlog_set_f_list_buffer_slave_run_off_RETURN PyAtrshmlog_set_f_list_buffer_slave_run_off PyAtrshmlog_set_f_list_buffer_slave_run_off_PROTO
{
  ATRSHMLOG_SET_F_LIST_BUFFER_SLAVE_RUN_OFF();
}

static PyAtrshmlog_set_wait_for_slaves_on_RETURN PyAtrshmlog_set_wait_for_slaves_on PyAtrshmlog_set_wait_for_slaves_on_PROTO
{
  return ATRSHMLOG_SET_WAIT_FOR_SLAVES_ON();
}

static PyAtrshmlog_set_wait_for_slaves_off_RETURN PyAtrshmlog_set_wait_for_slaves_off PyAtrshmlog_set_wait_for_slaves_off_PROTO
{
  return ATRSHMLOG_SET_WAIT_FOR_SLAVES_OFF();
}

static PyAtrshmlog_get_wait_for_slaves_RETURN PyAtrshmlog_get_wait_for_slaves PyAtrshmlog_get_wait_for_slaves_PROTO
{
  return ATRSHMLOG_GET_WAIT_FOR_SLAVES();
}
  
static PyAtrshmlog_get_f_list_buffer_slave_wait_RETURN PyAtrshmlog_get_f_list_buffer_slave_wait PyAtrshmlog_get_f_list_buffer_slave_wait_PROTO
{
  return ATRSHMLOG_GET_F_LIST_BUFFER_SLAVE_WAIT();
}

static PyAtrshmlog_set_f_list_buffer_slave_wait_RETURN PyAtrshmlog_set_f_list_buffer_slave_wait PyAtrshmlog_set_f_list_buffer_slave_wait_PROTO
{
  return ATRSHMLOG_SET_F_LIST_BUFFER_SLAVE_WAIT(nanos);
}

static PyAtrshmlog_get_acquire_count_RETURN PyAtrshmlog_get_acquire_count PyAtrshmlog_get_acquire_count_PROTO
{
  return ATRSHMLOG_GET_ACQUIRE_COUNT();
}

static PyAtrshmlog_get_prealloc_buffer_count_RETURN PyAtrshmlog_get_prealloc_buffer_count PyAtrshmlog_get_prealloc_buffer_count_PROTO
{
  return ATRSHMLOG_GET_PREALLOC_BUFFER_COUNT();
}

static PyAtrshmlog_set_prealloc_buffer_count_RETURN PyAtrshmlog_set_prealloc_buffer_count PyAtrshmlog_set_prealloc_buffer_count_PROTO
{
  return ATRSHMLOG_SET_PREALLOC_BUFFER_COUNT(count);
}

static PyAtrshmlog_get_inittime_RETURN PyAtrshmlog_get_inittime PyAtrshmlog_get_inittime_PROTO
{
  atrshmlog_internal_time_t result;

  result = ATRSHMLOG_GET_INITTIME();

  *seconds = result.tv_sec;
  *nanos = result.tv_nsec;
}

static PyAtrshmlog_get_inittime_tsc_before_RETURN PyAtrshmlog_get_inittime_tsc_before PyAtrshmlog_get_inittime_tsc_before_PROTO
{
  return ATRSHMLOG_GET_INITTIME_TSC_BEFORE();
}

static PyAtrshmlog_get_inittime_tsc_after_RETURN PyAtrshmlog_get_inittime_tsc_after PyAtrshmlog_get_inittime_tsc_after_PROTO
{
  return ATRSHMLOG_GET_INITTIME_TSC_AFTER();
}

static PyAtrshmlog_get_buffer_id_RETURN PyAtrshmlog_get_buffer_id PyAtrshmlog_get_buffer_id_PROTO
{
  return ATRSHMLOG_GET_BUFFER_ID();
}

static PyAtrshmlog_stop_RETURN PyAtrshmlog_stop PyAtrshmlog_stop_PROTO
{
  ATRSHMLOG_STOP();
}

static PyAtrshmlog_flush_RETURN PyAtrshmlog_flush PyAtrshmlog_flush_PROTO
{
  ATRSHMLOG_FLUSH();
}

static PyAtrshmlog_set_strategy_RETURN PyAtrshmlog_set_strategy PyAtrshmlog_set_strategy_PROTO
{
  return ATRSHMLOG_SET_STRATEGY(strategy);
}

static PyAtrshmlog_get_strategy_RETURN PyAtrshmlog_get_strategy PyAtrshmlog_get_strategy_PROTO
{
  return ATRSHMLOG_GET_STRATEGY();
}

static PyAtrshmlog_set_strategy_process_RETURN PyAtrshmlog_set_strategy_process PyAtrshmlog_set_strategy_process_PROTO
{
  return ATRSHMLOG_SET_STRATEGY_PROCESS(strategy);
}

static PyAtrshmlog_get_strategy_process_RETURN PyAtrshmlog_get_strategy_process PyAtrshmlog_get_strategy_process_PROTO
{
  return ATRSHMLOG_GET_STRATEGY_PROCESS();
}

static PyAtrshmlog_create_slave_RETURN PyAtrshmlog_create_slave PyAtrshmlog_create_slave_PROTO
{
  return ATRSHMLOG_CREATE_SLAVE();
}

static PyAtrshmlog_decrement_slave_count_RETURN PyAtrshmlog_decrement_slave_count PyAtrshmlog_decrement_slave_count_PROTO
{
  return ATRSHMLOG_DECREMENT_SLAVE_COUNT();
}

static PyAtrshmlog_get_clicktime_RETURN PyAtrshmlog_get_clicktime PyAtrshmlog_get_clicktime_PROTO
{
  return ATRSHMLOG_GET_CLICKTIME();
}

static PyAtrshmlog_set_thread_fence_1_RETURN PyAtrshmlog_set_thread_fence_1 PyAtrshmlog_set_thread_fence_1_PROTO
{
  return ATRSHMLOG_SET_THREAD_FENCE_1(flag);
}

static PyAtrshmlog_set_thread_fence_2_RETURN PyAtrshmlog_set_thread_fence_2 PyAtrshmlog_set_thread_fence_2_PROTO
{
  return ATRSHMLOG_SET_THREAD_FENCE_2(flag);
}


static PyAtrshmlog_set_thread_fence_3_RETURN PyAtrshmlog_set_thread_fence_3 PyAtrshmlog_set_thread_fence_3_PROTO
{
  return ATRSHMLOG_SET_THREAD_FENCE_3(flag);
}


static PyAtrshmlog_set_thread_fence_4_RETURN PyAtrshmlog_set_thread_fence_4 PyAtrshmlog_set_thread_fence_4_PROTO
{
  return ATRSHMLOG_SET_THREAD_FENCE_4(flag);
}


static PyAtrshmlog_set_thread_fence_5_RETURN PyAtrshmlog_set_thread_fence_5 PyAtrshmlog_set_thread_fence_5_PROTO
{
  return ATRSHMLOG_SET_THREAD_FENCE_5(flag);
}


static PyAtrshmlog_set_thread_fence_6_RETURN PyAtrshmlog_set_thread_fence_6 PyAtrshmlog_set_thread_fence_6_PROTO
{
  return ATRSHMLOG_SET_THREAD_FENCE_6(flag);
}


static PyAtrshmlog_set_thread_fence_7_RETURN PyAtrshmlog_set_thread_fence_7 PyAtrshmlog_set_thread_fence_7_PROTO
{
  return ATRSHMLOG_SET_THREAD_FENCE_7(flag);
}


static PyAtrshmlog_set_thread_fence_8_RETURN PyAtrshmlog_set_thread_fence_8 PyAtrshmlog_set_thread_fence_8_PROTO
{
  return ATRSHMLOG_SET_THREAD_FENCE_8(flag);
}


static PyAtrshmlog_set_thread_fence_9_RETURN PyAtrshmlog_set_thread_fence_9 PyAtrshmlog_set_thread_fence_9_PROTO
{
  return ATRSHMLOG_SET_THREAD_FENCE_9(flag);
}


static PyAtrshmlog_set_thread_fence_10_RETURN PyAtrshmlog_set_thread_fence_10 PyAtrshmlog_set_thread_fence_10_PROTO
{
  return ATRSHMLOG_SET_THREAD_FENCE_10(flag);
}


static PyAtrshmlog_set_thread_fence_11_RETURN PyAtrshmlog_set_thread_fence_11 PyAtrshmlog_set_thread_fence_11_PROTO
{
  return ATRSHMLOG_SET_THREAD_FENCE_11(flag);
}


static PyAtrshmlog_set_thread_fence_12_RETURN PyAtrshmlog_set_thread_fence_12 PyAtrshmlog_set_thread_fence_12_PROTO
{
  return ATRSHMLOG_SET_THREAD_FENCE_12(flag);
}


static PyAtrshmlog_set_thread_fence_13_RETURN PyAtrshmlog_set_thread_fence_13 PyAtrshmlog_set_thread_fence_13_PROTO
{
  return ATRSHMLOG_SET_THREAD_FENCE_13(flag);
}


static PyAtrshmlog_get_thread_fence_1_RETURN PyAtrshmlog_get_thread_fence_1 PyAtrshmlog_get_thread_fence_1_PROTO
{
  return ATRSHMLOG_GET_THREAD_FENCE_1();
}

static PyAtrshmlog_get_thread_fence_2_RETURN PyAtrshmlog_get_thread_fence_2 PyAtrshmlog_get_thread_fence_2_PROTO
{
  return ATRSHMLOG_GET_THREAD_FENCE_2();
}


static PyAtrshmlog_get_thread_fence_3_RETURN PyAtrshmlog_get_thread_fence_3 PyAtrshmlog_get_thread_fence_3_PROTO
{
  return ATRSHMLOG_GET_THREAD_FENCE_3();
}


static PyAtrshmlog_get_thread_fence_4_RETURN PyAtrshmlog_get_thread_fence_4 PyAtrshmlog_get_thread_fence_4_PROTO
{
  return ATRSHMLOG_GET_THREAD_FENCE_4();
}


static PyAtrshmlog_get_thread_fence_5_RETURN PyAtrshmlog_get_thread_fence_5 PyAtrshmlog_get_thread_fence_5_PROTO
{
  return ATRSHMLOG_GET_THREAD_FENCE_5();
}


static PyAtrshmlog_get_thread_fence_6_RETURN PyAtrshmlog_get_thread_fence_6 PyAtrshmlog_get_thread_fence_6_PROTO
{
  return ATRSHMLOG_GET_THREAD_FENCE_6();
}


static PyAtrshmlog_get_thread_fence_7_RETURN PyAtrshmlog_get_thread_fence_7 PyAtrshmlog_get_thread_fence_7_PROTO
{
  return ATRSHMLOG_GET_THREAD_FENCE_7();
}


static PyAtrshmlog_get_thread_fence_8_RETURN PyAtrshmlog_get_thread_fence_8 PyAtrshmlog_get_thread_fence_8_PROTO
{
  return ATRSHMLOG_GET_THREAD_FENCE_8();
}


static PyAtrshmlog_get_thread_fence_9_RETURN PyAtrshmlog_get_thread_fence_9 PyAtrshmlog_get_thread_fence_9_PROTO
{
  return ATRSHMLOG_GET_THREAD_FENCE_9();
}


static PyAtrshmlog_get_thread_fence_10_RETURN PyAtrshmlog_get_thread_fence_10 PyAtrshmlog_get_thread_fence_10_PROTO
{
  return ATRSHMLOG_GET_THREAD_FENCE_10();
}


static PyAtrshmlog_get_thread_fence_11_RETURN PyAtrshmlog_get_thread_fence_11 PyAtrshmlog_get_thread_fence_11_PROTO
{
  return ATRSHMLOG_GET_THREAD_FENCE_11();
}


static PyAtrshmlog_get_thread_fence_12_RETURN PyAtrshmlog_get_thread_fence_12 PyAtrshmlog_get_thread_fence_12_PROTO
{
  return ATRSHMLOG_GET_THREAD_FENCE_12();
}


static PyAtrshmlog_get_thread_fence_13_RETURN PyAtrshmlog_get_thread_fence_13 PyAtrshmlog_get_thread_fence_13_PROTO
{
  return ATRSHMLOG_GET_THREAD_FENCE_13();
}


static PyAtrshmlog_get_realtime_RETURN PyAtrshmlog_get_realtime PyAtrshmlog_get_realtime_PROTO
{
  atrshmlog_internal_time_t result;

  result = ATRSHMLOG_GET_REALTIME();

  *seconds = result.tv_sec;
  *nanos = result.tv_nsec;
}

static PyAtrshmlog_get_thread_locals_adress_RETURN PyAtrshmlog_get_thread_locals_adress PyAtrshmlog_get_thread_locals_adress_PROTO
{
  return ATRSHMLOG_GET_THREAD_LOCALS_ADRESS();
}

static PyAtrshmlog_get_tid_RETURN PyAtrshmlog_get_tid PyAtrshmlog_get_tid_PROTO
{
  return ATRSHMLOG_GET_TID();
}

static PyAtrshmlog_turn_logging_off_RETURN PyAtrshmlog_turn_logging_off PyAtrshmlog_turn_logging_off_PROTO
{
  ATRSHMLOG_TURN_LOGGING_OFF(tl);
}

static PyAtrshmlog_set_init_buffers_in_advance_on_RETURN PyAtrshmlog_set_init_buffers_in_advance_on PyAtrshmlog_set_init_buffers_in_advance_on_PROTO
{
  return ATRSHMLOG_SET_INIT_BUFFERS_IN_ADVANCE_ON();
}

static PyAtrshmlog_set_init_buffers_in_advance_off_RETURN PyAtrshmlog_set_init_buffers_in_advance_off PyAtrshmlog_set_init_buffers_in_advance_off_PROTO
{
  return ATRSHMLOG_SET_INIT_BUFFERS_IN_ADVANCE_OFF();
}

static PyAtrshmlog_get_init_buffers_in_advance_RETURN PyAtrshmlog_get_init_buffers_in_advance PyAtrshmlog_get_init_buffers_in_advance_PROTO
{
  return ATRSHMLOG_GET_INIT_BUFFERS_IN_ADVANCE();
}

static PyAtrshmlog_get_next_slave_local_RETURN PyAtrshmlog_get_next_slave_local PyAtrshmlog_get_next_slave_local_PROTO
{
  return ATRSHMLOG_GET_NEXT_SLAVE_LOCAL(tl);
}

static PyAtrshmlog_get_thread_local_tid_RETURN PyAtrshmlog_get_thread_local_tid PyAtrshmlog_get_thread_local_tid_PROTO
{
  return ATRSHMLOG_GET_THREAD_LOCAL_TID(tl);
}

static PyAtrshmlog_remove_slave_via_local_RETURN PyAtrshmlog_remove_slave_via_local PyAtrshmlog_remove_slave_via_local_PROTO
{
  return ATRSHMLOG_REMOVE_SLAVE_VIA_LOCAL(tl);
}

static PyAtrshmlog_reuse_thread_buffers_RETURN PyAtrshmlog_reuse_thread_buffers PyAtrshmlog_reuse_thread_buffers_PROTO
{
  return ATRSHMLOG_REUSE_THREAD_BUFFERS(tid);
}

static PyAtrshmlog_read_RETURN PyAtrshmlog_read PyAtrshmlog_read_PROTO
{
  atrshmlog_internal_time_t inittime;
  
  atrshmlog_internal_time_t lasttime;

  PyAtrshmlog_read_RETURN result;
  
  result = ATRSHMLOG_READ(area,
			  index,
			  buff,
			  length,
			  pid,
			  tid,
			  &inittime,
			  inittimetsc_before,
			  inittimetsc_after,
			  &lasttime,
			  lasttimetsc_before,
			  lasttimetsc_after,
			  difftimetransfer,
			  starttransfer,
			  acquiretime,
			  id,
			  number_dispatched,
			  counter_write0,
			  counter_write0_discard,
			  counter_write0_wait,
			  counter_write0_adaptive,
			  counter_write0_adaptive_fast,
			  counter_write0_adaptive_very_fast,
			  counter_write1,
			  counter_write1_discard,
			  counter_write1_wait,
			  counter_write1_adaptive,
			  counter_write1_adaptive_fast,
			  counter_write1_adaptive_very_fast,
			  counter_write2,
			  counter_write2_discard,
			  counter_write2_wait,
			  counter_write2_adaptive,
			  counter_write2_adaptive_fast,
			  counter_write2_adaptive_very_fast);

  *inittime_seconds = inittime.tv_sec;
  *inittime_nanos = inittime.tv_nsec;
  
  *lasttime_seconds = lasttime.tv_sec;
  *lasttime_nanos = lasttime.tv_nsec;
  
  return result;
}

static PyAtrshmlog_read_fetch_RETURN PyAtrshmlog_read_fetch PyAtrshmlog_read_fetch_PROTO
{
  atrshmlog_internal_time_t inittime;
  
  atrshmlog_internal_time_t lasttime;
  
  PyAtrshmlog_read_RETURN result;

  result = ATRSHMLOG_READ_FETCH(area,
				index,
				buff,
				length,
				pid,
				tid,
				&inittime,
				inittimetsc_before,
				inittimetsc_after,
				&lasttime,
				lasttimetsc_before,
				lasttimetsc_after,
				difftimetransfer,
				starttransfer,
				acquiretime,
				id,
				number_dispatched,
				counter_write0,
				counter_write0_discard,
				counter_write0_wait,
				counter_write0_adaptive,
				counter_write0_adaptive_fast,
				counter_write0_adaptive_very_fast,
				counter_write1,
				counter_write1_discard,
				counter_write1_wait,
				counter_write1_adaptive,
				counter_write1_adaptive_fast,
				counter_write1_adaptive_very_fast,
				counter_write2,
				counter_write2_discard,
				counter_write2_wait,
				counter_write2_adaptive,
				counter_write2_adaptive_fast,
				counter_write2_adaptive_very_fast);

  *inittime_seconds = inittime.tv_sec;
  *inittime_nanos = inittime.tv_nsec;
  
  *lasttime_seconds = lasttime.tv_sec;
  *lasttime_nanos = lasttime.tv_nsec;
  
  return result;
}

static PyAtrshmlog_verify_RETURN PyAtrshmlog_verify PyAtrshmlog_verify_PROTO
{
  return ATRSHMLOG_VERIFY();
}

static PyAtrshmlog_create_RETURN PyAtrshmlog_create PyAtrshmlog_create_PROTO
{
  return ATRSHMLOG_CREATE(ipckey,count);
}

static PyAtrshmlog_delete_RETURN PyAtrshmlog_delete PyAtrshmlog_delete_PROTO
{
  return ATRSHMLOG_DELETE(id);
}

static PyAtrshmlog_cleanup_locks_RETURN PyAtrshmlog_cleanup_locks PyAtrshmlog_cleanup_locks_PROTO
{
  ATRSHMLOG_CLEANUP_LOCKS(area);
}

static PyAtrshmlog_init_shm_log_RETURN PyAtrshmlog_init_shm_log PyAtrshmlog_init_shm_log_PROTO
{
  return ATRSHMLOG_INIT_SHM_LOG(area, count);
}

static PyAtrshmlog_poke_RETURN PyAtrshmlog_poke PyAtrshmlog_poke_PROTO
{
  PyAtrshmlog_poke_RETURN result;
  
  unsigned char* p = (unsigned char*)area;

  p += index;
  
  result = *p;

  *p = (unsigned char)value;
  
  return result;
}

static PyAtrshmlog_peek_RETURN PyAtrshmlog_peek PyAtrshmlog_peek_PROTO
{
  PyAtrshmlog_peek_RETURN result;
  
  unsigned char* p = (unsigned char*)area;

  p += index;
  
  result = *p;

  return result;
}

/************************************************************/

/*
 * The interface to the python interpreter.
 *
 * We use the core functions here so we get teh same from
 * the interpreter and from a C API capsule call.
 */

/******************************************************/

/**
 * \brief We attach to the shared memory.
 */
static PyObject*
python_atrshmlog_attach (PyObject *self, PyObject *args)
{
  PyAtrshmlog_attach_RETURN result ;

  result = PyAtrshmlog_attach();

  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief We get a clicktime.
 *
 * No use for the parameter here. 
 * We would spent more to check than we need to take.
 */
static PyObject*
python_atrshmlog_gettime(PyObject *self, PyObject *args)
{
  PyAtrshmlog_gettime_RETURN result;

  // We do not use an event here. this would be more time to
  // fetch from the args thatn we space at all.
  // so we simply ignore events for now and make life
  // better for having more time at all.
  result = PyAtrshmlog_gettime();

  return PyLong_FromUnsignedLongLong(result);
}

/******************************************************/

/**
 * \brief We log a basic info - barely the technical stuff.
 *
 * For the eventflag its per convention to use a Ascii I for interval
 * or a Ascii P for a point in time.
 * For now we have no constants in so you have to give a
 * - I 73 decimal, 49 hex
 * - P 80 decimal, 50 hex
 * or use the constants EVENT_POINT_IN_TIME_C and EVENT_INTERVAL_IN_TIME_C
 *
 * don't forget to check for the hidden mechanism in the doc
 */
static PyObject*
python_atrshmlog_write0(PyObject *self, PyObject *args)
{
  PyAtrshmlog_write0_RETURN result;
  unsigned long long starttime;
  unsigned long long endtime;
  int event;
  int eventflag;
  int userevent;
  
  
  if (!PyArg_ParseTuple(args, "iiiKK", &event, &eventflag, &userevent, &starttime, &endtime))
    {
      PyErr_SetString(AtrshmlogError, "write0 : fetch failed (event, eventflag, userevent, starttime, endtime)");
      return NULL;
    }

  result = PyAtrshmlog_write0(event, eventflag, userevent, starttime, endtime);

  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief We log a simple payload info with a string
 * 
 * For the eventflag its per convention to use a Ascii I for interval
 * or a Ascii P for a point in time.
 * For now we have no constants in so you have to give a
 * - I 73 decimal, 49 hex
 * - P 80 decimal, 50 hex
 * or use the constants EVENT_POINT_IN_TIME_C and EVENT_INTERVAL_IN_TIME_C
 *
 *
 * don't forget to check for the hidden mechanism in the doc
 *
 * We have to respect thath python not support binary in string.
 * So no embedded 0 here.
 * If you need binary logging sent me a post card for it.
 */
static PyObject*
python_atrshmlog_write(PyObject *self, PyObject *args)
{
  PyAtrshmlog_write_RETURN result;
  unsigned long long starttime;
  unsigned long long endtime;
  int event;
  int eventflag;
  int userevent;
  char *txt; 
  Py_ssize_t size;
  
  if (!PyArg_ParseTuple(args, "iiiKKs#", &event, &eventflag, &userevent, &starttime, &endtime, &txt, &size))
    {
      PyErr_SetString(AtrshmlogError, "write : fetch failed (event, eventflag, usrevent, starttime, endtime, string)");
      return NULL;
    }

  result = PyAtrshmlog_write(event, eventflag, userevent, starttime, endtime, txt, size);

  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief We wait nanoseconds.
 */
static PyObject*
python_atrshmlog_sleep_nanos(PyObject *self, PyObject *args)
{
  int nanos;
  
  if (!PyArg_ParseTuple(args, "i", &nanos))
    {
      PyErr_SetString(AtrshmlogError, "sleep_nanos : fetch failed (nanos)");
      return NULL;
    }

  PyAtrshmlog_sleep_nanos(nanos);
 
  return Py_None;
}


/******************************************************/

/**
 * \brief We deliver the max index of the statistics buffer
 */
static PyObject*
python_atrshmlog_get_statistics_max_index(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_statistics_max_index_RETURN result;

  result = PyAtrshmlog_get_statistics_max_index();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief  We deliver the statistics counter.
 */
static PyObject*
python_atrshmlog_get_statistics(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_statistics_max_index_RETURN result;

  // yes, its save for now - have an 85 for now as max index... 
  pyatrshmlog_int32_t h[100];

  result = PyAtrshmlog_get_statistics_max_index();

  PyAtrshmlog_get_statistics(h);
  
  PyObject * ret = makelist(h, result + 1);
 
  return ret;
}


/******************************************************/

/**
 * \brief We set the prefix for name lookup in the program.
 */
static PyObject*
python_atrshmlog_set_env_prefix(PyObject *self, PyObject *args)
{
  const char *prefix;
  
  if (!PyArg_ParseTuple(args, "s", &prefix))
    {
      PyErr_SetString(AtrshmlogError, "set_env_prefix : fetch failed (prefix)");
      return NULL;
    }

  PyAtrshmlog_set_env_prefix(prefix);
 
  return Py_None;
}

/******************************************************/

/**
 * \brief We get the actual name of the environment variable prefix.
 *
 */
static PyObject*
python_atrshmlog_get_env_prefix(PyObject *self, PyObject *args)
{
  const char* result;

  result = PyAtrshmlog_get_env_prefix();

  if (result == NULL)
    result = "";

#if ATRSHMLOG_PYTHON_VERSION == 3  
  return PyBytes_FromString(result);
#endif

#if ATRSHMLOG_PYTHON_VERSION == 2
  return PyString_FromString(result);
#endif

#if ATRSHMLOG_PYTHON_VERSION != 2
# if ATRSHMLOG_PYTHON_VERSION != 3
    bumm bumm bumm bumm bumm version problem
# endif
#endif
}


/******************************************************/

/**
 * \brief We get the env variable value of prefix and the supported suffix.
 */
static PyObject*
python_atrshmlog_get_env(PyObject *self, PyObject *args)
{
  const char* result;

  const char *suffix;
  
  if (!PyArg_ParseTuple(args, "s", &suffix))
    {
      PyErr_SetString(AtrshmlogError, "get_env : fetch failed (suffix)");
      return NULL;
    }

  result = PyAtrshmlog_get_env(suffix);

  if (result == NULL)
    result = "";
  
#if ATRSHMLOG_PYTHON_VERSION == 3  
  return PyBytes_FromString(result);
#endif

#if ATRSHMLOG_PYTHON_VERSION == 2
  return PyString_FromString(result);
#endif

#if ATRSHMLOG_PYTHON_VERSION != 2
# if ATRSHMLOG_PYTHON_VERSION != 3
    bumm bumm bumm bumm bumm version problem
# endif
#endif

}

/******************************************************/

/**
 * \brief We get the value of the environment variable holding the shmid in text.
 */
static PyObject*
python_atrshmlog_get_env_shmid(PyObject *self, PyObject *args)
{
  const char* result;

  result = PyAtrshmlog_get_env_shmid();

  if (result == NULL)
    result = "";
  
#if ATRSHMLOG_PYTHON_VERSION == 3  
  return PyBytes_FromString(result);
#endif

#if ATRSHMLOG_PYTHON_VERSION == 2
  return PyString_FromString(result);
#endif

#if ATRSHMLOG_PYTHON_VERSION != 2
# if ATRSHMLOG_PYTHON_VERSION != 3
    bumm bumm bumm bumm bumm version problem
# endif
#endif
}

/******************************************************/

/**
 * \brief We get the value of the environment variable holding the shmid in text.
 */
static PyObject*
python_atrshmlog_get_env_id_suffix(PyObject *self, PyObject *args)
{
  const char* result;

  result = PyAtrshmlog_get_env_id_suffix();

  if (result == NULL)
    result = "";
  
#if ATRSHMLOG_PYTHON_VERSION == 3  
  return PyBytes_FromString(result);
#endif

#if ATRSHMLOG_PYTHON_VERSION == 2
  return PyString_FromString(result);
#endif

#if ATRSHMLOG_PYTHON_VERSION != 2
# if ATRSHMLOG_PYTHON_VERSION != 3
    bumm bumm bumm bumm bumm version problem
# endif
#endif
}

/******************************************************/

/**
 * \brief Get the major version.
 */
static PyObject*
python_atrshmlog_get_version(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_version_RETURN result;

  result = PyAtrshmlog_get_version();
 
  return PyLong_FromLong(result);
}
  

/******************************************************/

/**
 * \brief Get the minor version.
 */
static PyObject*
python_atrshmlog_get_minor_version(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_minor_version_RETURN result;

  result = PyAtrshmlog_get_minor_version();
 
  return PyLong_FromLong(result);
}
  
/******************************************************/

/**
 * \brief Get the patch version.
 */
static PyObject*
python_atrshmlog_get_patch_version(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_patch_version_RETURN result;

  result = PyAtrshmlog_get_patch_version();
 
  return PyLong_FromLong(result);
}
  
/******************************************************/

/**
 * \brief Get the max of event locks - 1.
 */
static PyObject*
python_atrshmlog_get_event_locks_max(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_event_locks_max_RETURN result;

  result = PyAtrshmlog_get_event_locks_max();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Set the max of event locks.
 */
static PyObject*
python_atrshmlog_set_event_locks_max(PyObject *self, PyObject *args)
{
  PyAtrshmlog_set_event_locks_max_RETURN result;

  int newsize;
  
  if (!PyArg_ParseTuple(args, "i", &newsize))
    {
      PyErr_SetString(AtrshmlogError, "set_event_locks_max : fetch failed (index)");
      return NULL;
    }

  result = PyAtrshmlog_set_event_locks_max(newsize);
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Get an event value.
 */
static PyObject*
python_atrshmlog_get_event(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_event_RETURN result;

  int event;
  
  if (!PyArg_ParseTuple(args, "i", &event))
    {
      PyErr_SetString(AtrshmlogError, "get_event : fetch failed (event)");
      return NULL;
    }

  result = PyAtrshmlog_get_event(event);
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Set an event to 1.
 */
static PyObject*
python_atrshmlog_set_event_on(PyObject *self, PyObject *args)
{
  PyAtrshmlog_set_event_on_RETURN result;

  int event;
  
  if (!PyArg_ParseTuple(args, "i", &event))
    {
      PyErr_SetString(AtrshmlogError, "set_event_on : fetch failed (event)");
      return NULL;
    }

  result = PyAtrshmlog_set_event_on(event);
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Set an event to 0.
 */
static PyObject*
python_atrshmlog_set_event_off(PyObject *self, PyObject *args)
{
  PyAtrshmlog_set_event_off_RETURN result;

  int event;
  
  if (!PyArg_ParseTuple(args, "i", &event))
    {
      PyErr_SetString(AtrshmlogError, "set_event_off : fetch failed (event)");
      return NULL;
    }

  result = PyAtrshmlog_set_event_off(event);
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Get the logging status via function.
 */
static PyObject*
python_atrshmlog_get_logging(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_logging_RETURN result;

  result = PyAtrshmlog_get_logging();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Set the logging to on.
 */
static PyObject*
python_atrshmlog_set_logging_process_on(PyObject *self, PyObject *args)
{
  PyAtrshmlog_set_logging_process_on_RETURN result;

  result = PyAtrshmlog_set_logging_process_on();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Set the logging to off.
 */
static PyObject*
python_atrshmlog_set_logging_process_off(PyObject *self, PyObject *args)
{
  PyAtrshmlog_set_logging_process_off_RETURN result;

  result = PyAtrshmlog_set_logging_process_off();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Switch the logging for the process off finally.
 */
static PyObject*
python_atrshmlog_set_logging_process_off_final(PyObject *self, PyObject *args)
{
  PyAtrshmlog_set_logging_process_off_final_RETURN result;

  result = PyAtrshmlog_set_logging_process_off_final();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Get the shmid code.
 */
static PyObject*
python_atrshmlog_get_shmid(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_shmid_RETURN result;

  result = PyAtrshmlog_get_shmid();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Get the shm area start adress.
 *
 */
static PyObject*
python_atrshmlog_get_area(PyObject *self, PyObject *args)
{
  u_t result;

  result.a = PyAtrshmlog_get_area();

  return PyLong_FromUnsignedLongLong(result.p);
}

/******************************************************/

/**
 * \brief Get a shm area flag ich_habe_fertig.
 */
static PyObject*
python_atrshmlog_get_area_ich_habe_fertig(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_area_ich_habe_fertig_RETURN result;

  u_t u;
  
  if (!PyArg_ParseTuple(args, "K", &u.p))
    {
      PyErr_SetString(AtrshmlogError, "get_area_ich_habe_fertig : fetch failed (area)");
      return NULL;
    }

  result = PyAtrshmlog_get_area_ich_habe_fertig(u.a);
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Set the shm area flag ich_habe_fertig.
 */
static PyObject*
python_atrshmlog_set_area_ich_habe_fertig(PyObject *self, PyObject *args)
{
  PyAtrshmlog_set_area_ich_habe_fertig_RETURN result;

  int flag;
  
  u_t u;
  
  if (!PyArg_ParseTuple(args, "Ki", &u.p, &flag))
    {
      PyErr_SetString(AtrshmlogError, "set_area_ich_habe_fertig : fetch failed (area, flag)");
      return NULL;
    }

  result = PyAtrshmlog_set_area_ich_habe_fertig(u.a, flag);
 
  return PyLong_FromLong(result);
}


/******************************************************/

/**
 * \brief Get the shm count of buffers.
 */
static PyObject*
python_atrshmlog_get_area_count(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_area_count_RETURN result;

  u_t u;
  
  if (!PyArg_ParseTuple(args, "K", &u.p))
    {
      PyErr_SetString(AtrshmlogError, "get_area_count : fetch failed (area)");
      return NULL;
    }

  result = PyAtrshmlog_get_area_count(u.a);
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Get the shm version.
 */
static PyObject*
python_atrshmlog_get_area_version(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_area_version_RETURN result;

  u_t u;
  
  if (!PyArg_ParseTuple(args, "K", &u.p))
    {
      PyErr_SetString(AtrshmlogError, "get_area_version : fetch failed (area)");
      return NULL;
    }

  result = PyAtrshmlog_get_area_version(u.a);
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Get the maximum size of logbuffer.
 */
static PyObject*
python_atrshmlog_get_buffer_max_size(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_buffer_max_size_RETURN result;

  result = PyAtrshmlog_get_buffer_max_size();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Get the actual size of logbuffer in process.
 */
static PyObject*
python_atrshmlog_get_buffer_size(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_buffer_size_RETURN result;

  result = PyAtrshmlog_get_buffer_size();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Set the actual size for log buffer in process.
 */
static PyObject*
python_atrshmlog_set_buffer_size(PyObject *self, PyObject *args)
{
  PyAtrshmlog_set_buffer_size_RETURN result;

  int newsize;
  
  if (!PyArg_ParseTuple(args, "i", &newsize))
    {
      PyErr_SetString(AtrshmlogError, "set_buffer_size : fetch failed (size)");
      return NULL;
    }

  result = PyAtrshmlog_set_buffer_size(newsize);
 
  return PyLong_FromLong(result);
}


/******************************************************/

/**
 * \brief  Get the number of slave threads running.
 */
static PyObject*
python_atrshmlog_get_f_list_buffer_slave_count(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_f_list_buffer_slave_count_RETURN result;

  result = PyAtrshmlog_get_f_list_buffer_slave_count();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Set the number of slave threads startet.
 */
static PyObject*
python_atrshmlog_set_f_list_buffer_slave_count(PyObject *self, PyObject *args)
{
  PyAtrshmlog_set_f_list_buffer_slave_count_RETURN result;

  int newsize;
  
  if (!PyArg_ParseTuple(args, "i", &newsize))
    {
      PyErr_SetString(AtrshmlogError, "set_f_list_buffer_slave_count : fetch failed (count)");
      return NULL;
    }

  result = PyAtrshmlog_set_f_list_buffer_slave_count(newsize);
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief  Get the clock id used in get time.
 */
static PyObject*
python_atrshmlog_get_clock_id(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_clock_id_RETURN result;

  result = PyAtrshmlog_get_clock_id();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Set the clock id in get time.
 */
static PyObject*
python_atrshmlog_set_clock_id(PyObject *self, PyObject *args)
{
  PyAtrshmlog_set_clock_id_RETURN result;

  int newsize;
  
  if (!PyArg_ParseTuple(args, "i", &newsize))
    {
      PyErr_SetString(AtrshmlogError, "set_clock_id : fetch failed (id)");
      return NULL;
    }

  result = PyAtrshmlog_set_clock_id(newsize);
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief  Set the slave run flag to off.
 */
static PyObject*
python_atrshmlog_set_f_list_buffer_slave_run_off(PyObject *self, PyObject *args)
{
  PyAtrshmlog_set_f_list_buffer_slave_run_off();
 
  return Py_None;
}

/******************************************************/

/**
 * \brief  Set the wait for slaves flag to on.
 */
static PyObject*
python_atrshmlog_set_wait_for_slaves_on(PyObject *self, PyObject *args)
{
  PyAtrshmlog_set_wait_for_slaves_on_RETURN result;

  result = PyAtrshmlog_set_wait_for_slaves_on();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief  Set the wait for slaves flag to off.
 */
static PyObject*
python_atrshmlog_set_wait_for_slaves_off(PyObject *self, PyObject *args)
{
  PyAtrshmlog_set_wait_for_slaves_off_RETURN result;

  result = PyAtrshmlog_set_wait_for_slaves_off();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief  Get the wait for slaves flag.
 */
static PyObject*
python_atrshmlog_get_wait_for_slaves(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_wait_for_slaves_RETURN result;

  result = PyAtrshmlog_get_wait_for_slaves();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief  Get the nanos the slave waits after going to sleep.
 */
static PyObject*
python_atrshmlog_get_f_list_buffer_slave_wait(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_f_list_buffer_slave_wait_RETURN result;

  result = PyAtrshmlog_get_f_list_buffer_slave_wait();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Set the nanos the slave waits in sleep.
 */
static PyObject*
python_atrshmlog_set_f_list_buffer_slave_wait(PyObject *self, PyObject *args)
{
  PyAtrshmlog_set_f_list_buffer_slave_wait_RETURN result;

  int newsize;
  
  if (!PyArg_ParseTuple(args, "i", &newsize))
    {
      PyErr_SetString(AtrshmlogError, "set_f_list_buffer_slave_wait : fetch failed (nanos)");
      return NULL;
    }

  result = PyAtrshmlog_set_f_list_buffer_slave_wait(newsize);
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief  Get the number of buffers for the process.
 */
static PyObject*
python_atrshmlog_get_acquire_count(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_acquire_count_RETURN result;

  result = PyAtrshmlog_get_acquire_count();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief  Get the number of buffers fetched from dyn memory in one alloc.
 */
static PyObject*
python_atrshmlog_get_prealloc_buffer_count(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_prealloc_buffer_count_RETURN result;

  result = PyAtrshmlog_get_prealloc_buffer_count();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Set the number of buffers fetched with one alloc from dyn memory.
 */
static PyObject*
python_atrshmlog_set_prealloc_buffer_count(PyObject *self, PyObject *args)
{
  PyAtrshmlog_set_prealloc_buffer_count_RETURN result;

  int newsize;
  
  if (!PyArg_ParseTuple(args, "i", &newsize))
    {
      PyErr_SetString(AtrshmlogError, "set_prealloc_buffer_count : fetch failed (count)");
      return NULL;
    }

  result = PyAtrshmlog_set_prealloc_buffer_count(newsize);
 
  return PyLong_FromLong(result);
}


/******************************************************/

/**
 * \brief Get the inittime.
 */
static PyObject*
python_atrshmlog_get_inittime(PyObject *self, PyObject *args)
{
  pyatrshmlog_uint64_t s;
  pyatrshmlog_int32_t n;

  PyAtrshmlog_get_inittime(&s,&n);

  return Py_BuildValue("Ki", (unsigned long long)s, n);
}

/******************************************************/

/**
 * \brief Get the click time before inittime.
 */
static PyObject*
python_atrshmlog_get_inittime_tsc_before(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_inittime_tsc_before_RETURN result;

  result = PyAtrshmlog_get_inittime_tsc_before();
 
  return PyLong_FromUnsignedLongLong(result);
}

/******************************************************/

/**
 * \brief Get the click time after inittime.
 */
static PyObject*
python_atrshmlog_get_inittime_tsc_after(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_inittime_tsc_after_RETURN result;

  result = PyAtrshmlog_get_inittime_tsc_after();
 
  return PyLong_FromUnsignedLongLong(result);
}

/******************************************************/

/**
 * \brief Get the buffer id.
 */
static PyObject*
python_atrshmlog_get_buffer_id(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_buffer_id_RETURN result;

  result = PyAtrshmlog_get_buffer_id();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Stop logging for this thread.
 */
static PyObject*
python_atrshmlog_stop(PyObject *self, PyObject *args)
{
  PyAtrshmlog_stop();
 
  return Py_None;
}

/******************************************************/

/**
 * \brief Flush buffers for this thread.
 */
static PyObject*
python_atrshmlog_flush(PyObject *self, PyObject *args)
{
  PyAtrshmlog_flush();
 
  return Py_None;
}

/******************************************************/

/**
 * \brief Set the strategy for this thread.
 */
static PyObject*
python_atrshmlog_set_strategy(PyObject *self, PyObject *args)
{
  PyAtrshmlog_set_strategy_RETURN result;

  int newsize;
  
  if (!PyArg_ParseTuple(args, "i", &newsize))
    {
      PyErr_SetString(AtrshmlogError, "set_strategy : fetch failed (strategy)");
      return NULL;
    }

  result = PyAtrshmlog_set_strategy(newsize);
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief We get the strategy for this thread
 */
static PyObject*
python_atrshmlog_get_strategy(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_strategy_RETURN result;

  result = PyAtrshmlog_get_strategy();
 
  return PyLong_FromLong(result);
}


/******************************************************/

/**
 * \brief Set the strategy for the process
 */
static PyObject*
python_atrshmlog_set_strategy_process(PyObject *self, PyObject *args)
{
  PyAtrshmlog_set_strategy_process_RETURN result;

  int newsize;
  
  if (!PyArg_ParseTuple(args, "i", &newsize))
    {
      PyErr_SetString(AtrshmlogError, "set_strategy_process : fetch failed (strategy)");
      return NULL;
    }

  result = PyAtrshmlog_set_strategy_process(newsize);
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief We get the strategy for the process
 */
static PyObject*
python_atrshmlog_get_strategy_process(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_strategy_process_RETURN result;

  result = PyAtrshmlog_get_strategy_process();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Start a new slave thread.
 */
static PyObject*
python_atrshmlog_create_slave(PyObject *self, PyObject *args)
{
  PyAtrshmlog_create_slave_RETURN result;

  result = PyAtrshmlog_create_slave();
 
  return PyLong_FromLong((long)result);
}

/******************************************************/

/**
 * \brief We decrement the slave count.
 */
static PyObject*
python_atrshmlog_decrement_slave_count(PyObject *self, PyObject *args)
{
  PyAtrshmlog_decrement_slave_count_RETURN result;

  result = PyAtrshmlog_decrement_slave_count();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Get the real time with clicktime.
 *
 */
static PyObject*
python_atrshmlog_get_clicktime(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_clicktime_RETURN result;

  // We do not use an event here. this would be more time to
  // fetch from the args thatn we space at all.
  // so we simply ignore events for now and make life
  // better for having more time at all.
  result = PyAtrshmlog_get_clicktime();

  return PyLong_FromUnsignedLongLong(result);
}

/******************************************************/

/**
 * \brief Set the fence 1 flag on.
 */
static PyObject*
python_atrshmlog_set_thread_fence_1(PyObject *self, PyObject *args)
{
  PyAtrshmlog_set_thread_fence_1_RETURN result;

  int newsize;
  
  if (!PyArg_ParseTuple(args, "i", &newsize))
    {
      PyErr_SetString(AtrshmlogError, "set_thread_fence_1 : fetch failed (flag)");
      return NULL;
    }

  result = PyAtrshmlog_set_thread_fence_1(newsize);
 
  return PyLong_FromLong(result);
}


/******************************************************/

/**
 * \brief We get a fence on or off flag
 */
static PyObject*
python_atrshmlog_get_thread_fence_1(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_thread_fence_1_RETURN result;

  result = PyAtrshmlog_get_thread_fence_1();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Set the fence 2 flag on.
 */
static PyObject*
python_atrshmlog_set_thread_fence_2(PyObject *self, PyObject *args)
{
  PyAtrshmlog_set_thread_fence_2_RETURN result;

  int newsize;
  
  if (!PyArg_ParseTuple(args, "i", &newsize))
    {
      PyErr_SetString(AtrshmlogError, "set_thread_fence_2 : fetch failed (flag)");
      return NULL;
    }

  result = PyAtrshmlog_set_thread_fence_2(newsize);
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief We get a fence on or off flag
 */
static PyObject*
python_atrshmlog_get_thread_fence_2(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_thread_fence_2_RETURN result;

  result = PyAtrshmlog_get_thread_fence_2();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Set the fence 3 flag on.
 */
static PyObject*
python_atrshmlog_set_thread_fence_3(PyObject *self, PyObject *args)
{
  PyAtrshmlog_set_thread_fence_3_RETURN result;

  int newsize;
  
  if (!PyArg_ParseTuple(args, "i", &newsize))
    {
      PyErr_SetString(AtrshmlogError, "set_thread_fence_3 : fetch failed (flag)");
      return NULL;
    }

  result = PyAtrshmlog_set_thread_fence_3(newsize);
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief We get a fence on or off flag
 */
static PyObject*
python_atrshmlog_get_thread_fence_3(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_thread_fence_3_RETURN result;

  result = PyAtrshmlog_get_thread_fence_3();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Set the fence 4 flag on.
 */
static PyObject*
python_atrshmlog_set_thread_fence_4(PyObject *self, PyObject *args)
{
  PyAtrshmlog_set_thread_fence_4_RETURN result;

  int newsize;
  
  if (!PyArg_ParseTuple(args, "i", &newsize))
    {
      PyErr_SetString(AtrshmlogError, "set_thread_fence_4 : fetch failed (flag)");
      return NULL;
    }

  result = PyAtrshmlog_set_thread_fence_4(newsize);
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief We get a fence on or off flag
 */
static PyObject*
python_atrshmlog_get_thread_fence_4(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_thread_fence_4_RETURN result;

  result = PyAtrshmlog_get_thread_fence_4();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Set the fence 5 flag on.
 */
static PyObject*
python_atrshmlog_set_thread_fence_5(PyObject *self, PyObject *args)
{
  PyAtrshmlog_set_thread_fence_5_RETURN result;

  int newsize;
  
  if (!PyArg_ParseTuple(args, "i", &newsize))
    {
      PyErr_SetString(AtrshmlogError, "set_thread_fence_5 : fetch failed (flag)");
      return NULL;
    }

  result = PyAtrshmlog_set_thread_fence_5(newsize);
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief We get a fence on or off flag
 */
static PyObject*
python_atrshmlog_get_thread_fence_5(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_thread_fence_5_RETURN result;

  result = PyAtrshmlog_get_thread_fence_5();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Set the fence 6 flag on.
 */
static PyObject*
python_atrshmlog_set_thread_fence_6(PyObject *self, PyObject *args)
{
  PyAtrshmlog_set_thread_fence_6_RETURN result;

  int newsize;
  
  if (!PyArg_ParseTuple(args, "i", &newsize))
    {
      PyErr_SetString(AtrshmlogError, "set_thread_fence_6 : fetch failed (flag)");
      return NULL;
    }

  result = PyAtrshmlog_set_thread_fence_6(newsize);
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief We get a fence on or off flag
 */
static PyObject*
python_atrshmlog_get_thread_fence_6(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_thread_fence_6_RETURN result;

  result = PyAtrshmlog_get_thread_fence_6();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Set the fence 7 flag on.
 */
static PyObject*
python_atrshmlog_set_thread_fence_7(PyObject *self, PyObject *args)
{
  PyAtrshmlog_set_thread_fence_7_RETURN result;

  int newsize;
  
  if (!PyArg_ParseTuple(args, "i", &newsize))
    {
      PyErr_SetString(AtrshmlogError, "set_thread_fence_7 : fetch failed (flag)");
      return NULL;
    }

  result = PyAtrshmlog_set_thread_fence_7(newsize);
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief We get a fence on or off flag
 */
static PyObject*
python_atrshmlog_get_thread_fence_7(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_thread_fence_7_RETURN result;

  result = PyAtrshmlog_get_thread_fence_7();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Set the fence 8 flag on.
 */
static PyObject*
python_atrshmlog_set_thread_fence_8(PyObject *self, PyObject *args)
{
  PyAtrshmlog_set_thread_fence_8_RETURN result;

  int newsize;
  
  if (!PyArg_ParseTuple(args, "i", &newsize))
    {
      PyErr_SetString(AtrshmlogError, "set_thread_fence_8 : fetch failed (flag)");
      return NULL;
    }

  result = PyAtrshmlog_set_thread_fence_8(newsize);
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief We get a fence on or off flag
 */
static PyObject*
python_atrshmlog_get_thread_fence_8(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_thread_fence_8_RETURN result;

  result = PyAtrshmlog_get_thread_fence_8();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Set the fence 9 flag on.
 */
static PyObject*
python_atrshmlog_set_thread_fence_9(PyObject *self, PyObject *args)
{
  PyAtrshmlog_set_thread_fence_9_RETURN result;

  int newsize;
  
  if (!PyArg_ParseTuple(args, "i", &newsize))
    {
      PyErr_SetString(AtrshmlogError, "set_thread_fence_9 : fetch failed (flag)");
      return NULL;
    }

  result = PyAtrshmlog_set_thread_fence_9(newsize);
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief We get a fence on or off flag
 */
static PyObject*
python_atrshmlog_get_thread_fence_9(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_thread_fence_9_RETURN result;

  result = PyAtrshmlog_get_thread_fence_9();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Set the fence 10 flag on.
 */
static PyObject*
python_atrshmlog_set_thread_fence_10(PyObject *self, PyObject *args)
{
  PyAtrshmlog_set_thread_fence_10_RETURN result;

  int newsize;
  
  if (!PyArg_ParseTuple(args, "i", &newsize))
    {
      PyErr_SetString(AtrshmlogError, "set_thread_fence_10 : fetch failed (flag)");
      return NULL;
    }

  result = PyAtrshmlog_set_thread_fence_10(newsize);
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief We get a fence on or off flag
 */
static PyObject*
python_atrshmlog_get_thread_fence_10(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_thread_fence_10_RETURN result;

  result = PyAtrshmlog_get_thread_fence_10();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Set the fence 11 flag on.
 */
static PyObject*
python_atrshmlog_set_thread_fence_11(PyObject *self, PyObject *args)
{
  PyAtrshmlog_set_thread_fence_11_RETURN result;

  int newsize;
  
  if (!PyArg_ParseTuple(args, "i", &newsize))
    {
      PyErr_SetString(AtrshmlogError, "set_thread_fence_11 : fetch failed (flag)");
      return NULL;
    }

  result = PyAtrshmlog_set_thread_fence_11(newsize);
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief We get a fence on or off flag
 */
static PyObject*
python_atrshmlog_get_thread_fence_11(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_thread_fence_11_RETURN result;

  result = PyAtrshmlog_get_thread_fence_11();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Set the fence 12 flag on.
 */
static PyObject*
python_atrshmlog_set_thread_fence_12(PyObject *self, PyObject *args)
{
  PyAtrshmlog_set_thread_fence_12_RETURN result;

  int newsize;
  
  if (!PyArg_ParseTuple(args, "i", &newsize))
    {
      PyErr_SetString(AtrshmlogError, "set_thread_fence_12 : fetch failed (flag)");
      return NULL;
    }

  result = PyAtrshmlog_set_thread_fence_12(newsize);
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief We get a fence on or off flag
 */
static PyObject*
python_atrshmlog_get_thread_fence_12(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_thread_fence_12_RETURN result;

  result = PyAtrshmlog_get_thread_fence_12();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Set the fence 13 flag on.
 */
static PyObject*
python_atrshmlog_set_thread_fence_13(PyObject *self, PyObject *args)
{
  PyAtrshmlog_set_thread_fence_13_RETURN result;

  int newsize;
  
  if (!PyArg_ParseTuple(args, "i", &newsize))
    {
      PyErr_SetString(AtrshmlogError, "set_thread_fence_13 : fetch failed (flag)");
      return NULL;
    }

  result = PyAtrshmlog_set_thread_fence_13(newsize);
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief We get a fence on or off flag
 */
static PyObject*
python_atrshmlog_get_thread_fence_13(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_thread_fence_13_RETURN result;

  result = PyAtrshmlog_get_thread_fence_13();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief We get the real time.
 */
static PyObject*
python_atrshmlog_get_realtime(PyObject *self, PyObject *args)
{

  pyatrshmlog_uint64_t s;
  pyatrshmlog_int32_t n;

  PyAtrshmlog_get_realtime(&s, &n);

  return Py_BuildValue("Ki", (unsigned long long)s, n);
}

/******************************************************/

/**
 * \brief We get the thread locals adress of a thread
 *
 */
static PyObject*
python_atrshmlog_get_thread_locals_adress(PyObject *self, PyObject *args)
{
  u_t result;

  result.a = PyAtrshmlog_get_thread_locals_adress();

  return PyLong_FromUnsignedLongLong(result.p);
}

/******************************************************/

/**
 * \brief We get the thread tid
 */
static PyObject*
python_atrshmlog_get_tid(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_tid_RETURN result;

  result = PyAtrshmlog_get_tid();
 
  return PyLong_FromUnsignedLongLong(result);
}

/******************************************************/

/**
 * \brief We switch the thread off and dispatch its buffers
 */
static PyObject*
python_atrshmlog_turn_logging_off(PyObject *self, PyObject *args)
{
  u_t u;
  
  if (!PyArg_ParseTuple(args, "K", &u.p))
    {
      PyErr_SetString(AtrshmlogError, "turn_logging_off : fetch failed (threadlocal)");
      return NULL;
    }

  PyAtrshmlog_turn_logging_off(u.a);
 
  return Py_None;
}

/******************************************************/

/**
 * \brief We turn the init in advance on
 */
static PyObject*
python_atrshmlog_set_init_buffers_in_advance_on(PyObject *self, PyObject *args)
{
  PyAtrshmlog_set_init_buffers_in_advance_on_RETURN result;

  result = PyAtrshmlog_set_init_buffers_in_advance_on();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief We turn the init in advance off
 */
static PyObject*
python_atrshmlog_set_init_buffers_in_advance_off(PyObject *self, PyObject *args)
{
  PyAtrshmlog_set_init_buffers_in_advance_off_RETURN result;

  result = PyAtrshmlog_set_init_buffers_in_advance_off();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief We get the init in advance flag
 */
static PyObject*
python_atrshmlog_get_init_buffers_in_advance(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_init_buffers_in_advance_RETURN result;

  result = PyAtrshmlog_get_init_buffers_in_advance();
 
  return PyLong_FromLong(result);
}


/******************************************************/

/**
 *  \brief We get the next element on slave list
 */
static PyObject*
python_atrshmlog_get_next_slave_local(PyObject *self, PyObject *args)
{
  u_t u;
  
  if (!PyArg_ParseTuple(args, "K", &u.p))
    {
      PyErr_SetString(AtrshmlogError, "get_next_slave_local : fetch failed (threadlocal)");
      return NULL;
    }

  u.a = PyAtrshmlog_get_next_slave_local(u.a);
 
  return PyLong_FromUnsignedLongLong(u.p);
}

/******************************************************/

/**
 *  \brief We get the tid of a thread local
 */
static PyObject*
python_atrshmlog_get_thread_local_tid(PyObject *self, PyObject *args)
{
  PyAtrshmlog_get_thread_local_tid_RETURN result;

  u_t u;
  
  if (!PyArg_ParseTuple(args, "K", &u.p))
    {
      PyErr_SetString(AtrshmlogError, "get_thread_local_tid : fetch failed (threadlocal)");
      return NULL;
    }

  result = PyAtrshmlog_get_thread_local_tid(u.a);
 
  return PyLong_FromUnsignedLongLong(result);
}

/******************************************************/

/**
 * \brief  We remove the slave from the list of slaves
 */
static PyObject*
python_atrshmlog_remove_slave_via_local(PyObject *self, PyObject *args)
{
  PyAtrshmlog_remove_slave_via_local_RETURN result;

  u_t u;
  
  if (!PyArg_ParseTuple(args, "K", &u.p))
    {
      PyErr_SetString(AtrshmlogError, "remove_slave_via_local : fetch failed (threadlocal)");
      return NULL;
    }

  result = PyAtrshmlog_remove_slave_via_local(u.a);
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief We make reuse of buffers of a dead thread 
 */
static PyObject*
python_atrshmlog_reuse_thread_buffers(PyObject *self, PyObject *args)
{
  PyAtrshmlog_reuse_thread_buffers_RETURN result;

  unsigned long long tid;
  
  if (!PyArg_ParseTuple(args, "K", &tid))
    {
      PyErr_SetString(AtrshmlogError, "reuse_thread_buffers : fetch failed (tid)");
      return NULL;
    }

  result = PyAtrshmlog_reuse_thread_buffers(tid);
 
  return PyLong_FromLong((long)result);
}

/******************************************************/

_Thread_local static char * transferbuffer = 0;
  
/**
 * \brief Read a buffer from shm. Outdated version.
 *
 */
static PyObject*
python_atrshmlog_read(PyObject *self, PyObject *args)
{
  PyAtrshmlog_read_RETURN result;

  PyObject* r;

  PyObject* t;
  
  pyatrshmlog_int32_t index;
  
  u_t u;
  
  if (!PyArg_ParseTuple(args, "Ki", &u.p, &index))
    {
      PyErr_SetString(AtrshmlogError, "read : fetch failed (area, index)");
      return NULL;
    }

  if (transferbuffer == 0)
    {
      transferbuffer = malloc(ATRSHMLOG_GET_BUFFER_MAX_SIZE() + 1 );

      if (transferbuffer == 0)
	{
	  PyErr_SetString(AtrshmlogError, "read : malloc transferbuffer failed.");
	  return NULL;
  	}
    }
  
  pyatrshmlog_int32_t o_length;

  PyObject * py_length;
  
  pyatrshmlog_uint64_t o_pid;
  PyObject * py_pid;
  
  pyatrshmlog_uint64_t o_tid;
  PyObject * py_tid;


  pyatrshmlog_uint64_t inittime_seconds;
  pyatrshmlog_int32_t inittime_nanos;
  PyObject * py_inittime_s;
  PyObject * py_inittime_n;
  
  pyatrshmlog_uint64_t o_inittsc_before;
  PyObject * py_inittsc_before;
  
  pyatrshmlog_uint64_t o_inittsc_after;
  PyObject * py_inittsc_after;
	      
  pyatrshmlog_uint64_t lasttime_seconds;
  pyatrshmlog_int32_t lasttime_nanos;
  PyObject * py_lasttime_s;
  PyObject * py_lasttime_n;
  
  pyatrshmlog_uint64_t o_lasttsc_before;
  PyObject * py_lasttsc_before;
  
  pyatrshmlog_uint64_t o_lasttsc_after;
  PyObject * py_lasttsc_after;
  
  pyatrshmlog_uint64_t o_difftimetransfer;
  PyObject * py_difftimetransfer;
  
  pyatrshmlog_uint64_t o_starttransfer;
  PyObject * py_starttransfer;
  
  pyatrshmlog_uint64_t o_acquiretime;
  PyObject * py_acquiretime;
  
  pyatrshmlog_int32_t o_id;
  PyObject * py_id;
  
  pyatrshmlog_int32_t o_number_dispatched;
  PyObject * py_number_dispatched;
  
  /**
   * thread specific statistics are here.
   * so we can colect them later from the buffers in files.
   *
   * It was too bad for performance to have them in atomics global.
   * Too much interaction.
   * so I have them now local
   * We only set them in the beginning, never reset them.
   */

  pyatrshmlog_int32_t counter_write0;
  PyObject * py_counter_write0;
  pyatrshmlog_int32_t counter_write0_discard;
  PyObject * py_counter_write0_discard;
  pyatrshmlog_int32_t counter_write0_wait;
  PyObject * py_counter_write0_wait;
  pyatrshmlog_int32_t counter_write0_adaptive;
  PyObject * py_counter_write0_adaptive;
  pyatrshmlog_int32_t counter_write0_adaptive_fast;
  PyObject * py_counter_write0_adaptive_fast;
  pyatrshmlog_int32_t counter_write0_adaptive_very_fast;
  PyObject * py_counter_write0_adaptive_very_fast;
  pyatrshmlog_int32_t counter_write1;
  PyObject * py_counter_write1;
  pyatrshmlog_int32_t counter_write1_discard;
  PyObject * py_counter_write1_discard;
  pyatrshmlog_int32_t counter_write1_wait;
  PyObject * py_counter_write1_wait;
  pyatrshmlog_int32_t counter_write1_adaptive;
  PyObject * py_counter_write1_adaptive;
  pyatrshmlog_int32_t counter_write1_adaptive_fast;
  PyObject * py_counter_write1_adaptive_fast;
  pyatrshmlog_int32_t counter_write1_adaptive_very_fast;
  PyObject * py_counter_write1_adaptive_very_fast;
  pyatrshmlog_int32_t counter_write2;
  PyObject * py_counter_write2;
  pyatrshmlog_int32_t counter_write2_discard;
  PyObject * py_counter_write2_discard;
  pyatrshmlog_int32_t counter_write2_wait;
  PyObject * py_counter_write2_wait;
  pyatrshmlog_int32_t counter_write2_adaptive;
  PyObject * py_counter_write2_adaptive;
  pyatrshmlog_int32_t counter_write2_adaptive_fast;
  PyObject * py_counter_write2_adaptive_fast;
  pyatrshmlog_int32_t counter_write2_adaptive_very_fast;
  PyObject * py_counter_write2_adaptive_very_fast;
  
  result = PyAtrshmlog_read(u.a,
			  index,
			  &transferbuffer[0],
			  &o_length,
			  &o_pid,
			  &o_tid,
			  &inittime_seconds,
			  &inittime_nanos,
			  &o_inittsc_before,
			  &o_inittsc_after,
			  &lasttime_seconds,
			  &lasttime_nanos,
			  &o_lasttsc_before,
			  &o_lasttsc_after,
			  &o_difftimetransfer,
			  &o_starttransfer,
			  &o_acquiretime,
			  &o_id,
			  &o_number_dispatched,
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
			       
  if (result < 0)
    {
      // return -1; we construct a tuple and give it back.
      r = PyLong_FromLong(result);

      if (r == NULL)
	return NULL;
      
      t = PyTuple_New(1);

      if (t == NULL)
	return NULL;
      
      PyTuple_SET_ITEM(t,0,r);

      return t;
    }

  if (o_length == 0)
    {
      // return -1; we construct a tuple and give it back.
      r = PyLong_FromLong(result);
      if (r == NULL)
	return NULL;
      
      py_length = PyLong_FromLong(0);

      if(py_length  == NULL)
	return NULL;
      
      t = PyTuple_New(2);

      if (t == NULL)
	return NULL;
      
      PyTuple_SET_ITEM(t,0,r);
      PyTuple_SET_ITEM(t,1,py_length);

      return t;
    }

  r = PyLong_FromLong(0);

  PyObject * b = PyByteArray_FromStringAndSize(transferbuffer, o_length);

  if (b == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of buffer failed.");
      return NULL;
    }
  
  // ok. we have a len > 0 
  py_length = PyLong_FromLong(o_length);

  if (py_length == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of length failed.");
      return NULL;
    }
  
  py_pid = PyLong_FromUnsignedLongLong(o_pid);

  if (py_pid == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of pid failed.");
      return NULL;
    }
  
  py_tid = PyLong_FromUnsignedLongLong(o_tid);

  if (py_tid == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of tid failed.");
      return NULL;
    }
  
  py_inittime_s = PyLong_FromUnsignedLongLong(inittime_seconds);

  if (py_tid == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of tid failed.");
      return NULL;
    }
  
  py_inittime_n = PyLong_FromLong(inittime_nanos);

  if (py_inittime_n == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of inittime_n failed.");
      return NULL;
    }
  
  py_inittsc_before = PyLong_FromUnsignedLongLong(o_inittsc_before);

  if (py_inittsc_before == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of inittsc_before failed.");
      return NULL;
    }
  
  py_inittsc_after = PyLong_FromUnsignedLongLong(o_inittsc_after);

  if (py_inittsc_after == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of inittsc_after failed.");
      return NULL;
    }
  
  py_lasttime_s = PyLong_FromUnsignedLongLong(lasttime_seconds);

  if (py_lasttime_s == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of lasttime_s failed.");
      return NULL;
    }
  
  py_lasttime_n = PyLong_FromLong(lasttime_nanos);

  if (py_lasttime_n == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of lasttime_n failed.");
      return NULL;
    }
  
  py_lasttsc_before = PyLong_FromUnsignedLongLong(o_lasttsc_before);

  if (py_lasttsc_before == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of lasttsc_before failed.");
      return NULL;
    }
  
  py_lasttsc_after = PyLong_FromUnsignedLongLong(o_lasttsc_after);

  if (py_lasttsc_after == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of lasttsc_after failed.");
      return NULL;
    }
  
  py_difftimetransfer = PyLong_FromUnsignedLongLong(o_difftimetransfer);

  if (py_difftimetransfer == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of difftimetransfer failed.");
      return NULL;
    }
  
  py_starttransfer = PyLong_FromUnsignedLongLong(o_starttransfer);

  if (py_starttransfer == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of starttransfer failed.");
      return NULL;
    }
  
  py_acquiretime = PyLong_FromUnsignedLongLong(o_acquiretime);

  if (py_acquiretime == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of acquiretime failed.");
      return NULL;
    }
  
  py_id = PyLong_FromLong(o_id);

  if (py_id == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of id failed.");
      return NULL;
    }
  
  py_number_dispatched = PyLong_FromLong(o_number_dispatched);

  if (py_number_dispatched == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of number_dispatched failed.");
      return NULL;
    }
  
  py_counter_write0 = PyLong_FromLong(counter_write0);

  if (py_counter_write0 == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of counter_write0 failed.");
      return NULL;
    }
  
  py_counter_write0_discard = PyLong_FromLong(counter_write0_discard);

  if (py_counter_write0_discard == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of counter_write0_discard failed.");
      return NULL;
    }
  
  py_counter_write0_wait = PyLong_FromLong(counter_write0_wait);

  if (py_counter_write0_wait == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of counter_write0_wait failed.");
      return NULL;
    }
  
  py_counter_write0_adaptive = PyLong_FromLong(counter_write0_adaptive);

  if (py_counter_write0_adaptive == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of counter_write0_adaptive failed.");
      return NULL;
    }
  
  py_counter_write0_adaptive_fast = PyLong_FromLong(counter_write0_adaptive_fast);

  if (py_counter_write0_adaptive_fast == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of counter_write0_adaptive_fast failed.");
      return NULL;
    }
  
  py_counter_write0_adaptive_very_fast = PyLong_FromLong(counter_write0_adaptive_very_fast);

  if (py_counter_write0_adaptive_very_fast == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of counter_write0_adaptive_very_fast failed.");
      return NULL;
    }
  
  py_counter_write1 = PyLong_FromLong(counter_write1);

  if (py_counter_write1 == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of counter_write1 failed.");
      return NULL;
    }
  
  py_counter_write1_discard = PyLong_FromLong(counter_write1_discard);

  if (py_counter_write1_discard == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of counter_write1_discard failed.");
      return NULL;
    }
  
  py_counter_write1_wait = PyLong_FromLong(counter_write1_wait);

  if (py_counter_write1_wait == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of counter_write1_wait failed.");
      return NULL;
    }
  
  py_counter_write1_adaptive = PyLong_FromLong(counter_write1_adaptive);

  if (py_counter_write1_adaptive == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of counter_write1_adaptive failed.");
      return NULL;
    }
  
  py_counter_write1_adaptive_fast = PyLong_FromLong(counter_write1_adaptive_fast);

  if (py_counter_write1_adaptive_fast == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of counter_write1_adaptive_fast failed.");
      return NULL;
    }
  
  py_counter_write1_adaptive_very_fast = PyLong_FromLong(counter_write1_adaptive_very_fast);

  if (py_counter_write1_adaptive_very_fast == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of counter_write1_adaptive_very_fast failed.");
      return NULL;
    }
  
  py_counter_write2 = PyLong_FromLong(counter_write2);

  if (py_counter_write2 == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of counter_write2 failed.");
      return NULL;
    }
  
  py_counter_write2_discard = PyLong_FromLong(counter_write2_discard);

  if (py_counter_write2_discard == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of counter_write2_discard failed.");
      return NULL;
    }
  
  py_counter_write2_wait = PyLong_FromLong(counter_write2_wait);

  if (py_counter_write2_wait == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of counter_write2_wait failed.");
      return NULL;
    }
  
  py_counter_write2_adaptive = PyLong_FromLong(counter_write2_adaptive);

  if (py_counter_write2_adaptive == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of counter_write2_adaptive failed.");
      return NULL;
    }
  
  py_counter_write2_adaptive_fast = PyLong_FromLong(counter_write2_adaptive_fast);

  if (py_counter_write2_adaptive_fast == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of counter_write2_adaptive_fast failed.");
      return NULL;
    }
  
  py_counter_write2_adaptive_very_fast = PyLong_FromLong(counter_write2_adaptive_very_fast);

  if (py_counter_write2_adaptive_very_fast == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of counter_write2_adaptive_very_fast failed.");
      return NULL;
    }
  

  t = PyTuple_New(37);

  if (t == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of tuple failed.");
      return NULL;
    }
  
  PyTuple_SET_ITEM(t, 0, r);
  PyTuple_SET_ITEM(t, 1, py_length);
  PyTuple_SET_ITEM(t, 2, b);
  PyTuple_SET_ITEM(t, 3, py_pid);
  PyTuple_SET_ITEM(t, 4, py_tid);
  PyTuple_SET_ITEM(t, 5, py_inittime_s);
  PyTuple_SET_ITEM(t, 6, py_inittime_n);
  PyTuple_SET_ITEM(t, 7, py_inittsc_before);
  PyTuple_SET_ITEM(t, 8, py_inittsc_after);
  PyTuple_SET_ITEM(t, 9, py_lasttime_s);
  PyTuple_SET_ITEM(t, 10, py_lasttime_n);
  PyTuple_SET_ITEM(t, 11, py_lasttsc_before);
  PyTuple_SET_ITEM(t, 12, py_lasttsc_after);
  PyTuple_SET_ITEM(t, 13, py_difftimetransfer);
  PyTuple_SET_ITEM(t, 14, py_starttransfer);
  PyTuple_SET_ITEM(t, 15, py_acquiretime);
  PyTuple_SET_ITEM(t, 16, py_id);
  PyTuple_SET_ITEM(t, 17, py_number_dispatched);
  PyTuple_SET_ITEM(t, 18, py_counter_write0);
  PyTuple_SET_ITEM(t, 19, py_counter_write0_discard);
  PyTuple_SET_ITEM(t, 20, py_counter_write0_wait);
  PyTuple_SET_ITEM(t, 21, py_counter_write0_adaptive);
  PyTuple_SET_ITEM(t, 22, py_counter_write0_adaptive_fast);
  PyTuple_SET_ITEM(t, 23, py_counter_write0_adaptive_very_fast);
  PyTuple_SET_ITEM(t, 24, py_counter_write1);
  PyTuple_SET_ITEM(t, 25, py_counter_write1_discard);
  PyTuple_SET_ITEM(t, 26, py_counter_write1_wait);
  PyTuple_SET_ITEM(t, 27, py_counter_write1_adaptive);
  PyTuple_SET_ITEM(t, 28, py_counter_write1_adaptive_fast);
  PyTuple_SET_ITEM(t, 29, py_counter_write1_adaptive_very_fast);
  PyTuple_SET_ITEM(t, 30, py_counter_write2);
  PyTuple_SET_ITEM(t, 31, py_counter_write2_discard);
  PyTuple_SET_ITEM(t, 32, py_counter_write2_wait);
  PyTuple_SET_ITEM(t, 33, py_counter_write2_adaptive);
  PyTuple_SET_ITEM(t, 34, py_counter_write2_adaptive_fast);
  PyTuple_SET_ITEM(t, 35, py_counter_write2_adaptive_very_fast);
  PyTuple_SET_ITEM(t, 36, py_counter_write2_adaptive_very_fast);
  
  return t;
}


/******************************************************/

/**
 * \brief Read a buffer from shm. Actual version.
 *
 */
static PyObject*
python_atrshmlog_read_fetch(PyObject *self, PyObject *args)
{
  PyAtrshmlog_read_fetch_RETURN result;

  PyObject* r;

  PyObject* t;
  
  pyatrshmlog_int32_t index;

  PyObject* py_index;
  
  u_t u;
  
  if (!PyArg_ParseTuple(args, "K", &u.p))
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : fetch failed (area)");
      return NULL;
    }

  if (transferbuffer == 0)
    {
      transferbuffer = malloc(ATRSHMLOG_GET_BUFFER_MAX_SIZE() + 1 );

      if (transferbuffer == 0)
	{
	  PyErr_SetString(AtrshmlogError, "read_fetch : malloc transferbuffer failed.");
	  return NULL;
  	}
    }
  
  pyatrshmlog_int32_t o_length;

  PyObject * py_length;
  
  pyatrshmlog_uint64_t o_pid;
  PyObject * py_pid;
  

  pyatrshmlog_uint64_t o_tid;
  PyObject * py_tid;
  
  pyatrshmlog_uint64_t inittime_seconds;
  pyatrshmlog_int32_t inittime_nanos;
  
  PyObject * py_inittime_s;
  PyObject * py_inittime_n;
  
  pyatrshmlog_uint64_t o_inittsc_before;
  PyObject * py_inittsc_before;
  
  pyatrshmlog_uint64_t o_inittsc_after;
  PyObject * py_inittsc_after;
	      
  pyatrshmlog_uint64_t lasttime_seconds;
  pyatrshmlog_int32_t lasttime_nanos;

  PyObject * py_lasttime_s;
  PyObject * py_lasttime_n;
  
  pyatrshmlog_uint64_t o_lasttsc_before;
  PyObject * py_lasttsc_before;
  
  pyatrshmlog_uint64_t o_lasttsc_after;
  PyObject * py_lasttsc_after;
  
  pyatrshmlog_uint64_t o_difftimetransfer;
  PyObject * py_difftimetransfer;
  
  pyatrshmlog_uint64_t o_starttransfer;
  PyObject * py_starttransfer;
  
  pyatrshmlog_uint64_t o_acquiretime;
  PyObject * py_acquiretime;
  
  pyatrshmlog_int32_t o_id;
  PyObject * py_id;
  
  pyatrshmlog_int32_t o_number_dispatched;
  PyObject * py_number_dispatched;
  
  /**
   * thread specific statistics are here.
   * so we can colect them later from the buffers in files.
   *
   * It was too bad for performance to have them in atomics global.
   * Too much interaction.
   * so I have them now local
   * We only set them in the beginning, never reset them.
   */

  pyatrshmlog_int32_t counter_write0;
  PyObject * py_counter_write0;
  pyatrshmlog_int32_t counter_write0_discard;
  PyObject * py_counter_write0_discard;
  pyatrshmlog_int32_t counter_write0_wait;
  PyObject * py_counter_write0_wait;
  pyatrshmlog_int32_t counter_write0_adaptive;
  PyObject * py_counter_write0_adaptive;
  pyatrshmlog_int32_t counter_write0_adaptive_fast;
  PyObject * py_counter_write0_adaptive_fast;
  pyatrshmlog_int32_t counter_write0_adaptive_very_fast;
  PyObject * py_counter_write0_adaptive_very_fast;
  pyatrshmlog_int32_t counter_write1;
  PyObject * py_counter_write1;
  pyatrshmlog_int32_t counter_write1_discard;
  PyObject * py_counter_write1_discard;
  pyatrshmlog_int32_t counter_write1_wait;
  PyObject * py_counter_write1_wait;
  pyatrshmlog_int32_t counter_write1_adaptive;
  PyObject * py_counter_write1_adaptive;
  pyatrshmlog_int32_t counter_write1_adaptive_fast;
  PyObject * py_counter_write1_adaptive_fast;
  pyatrshmlog_int32_t counter_write1_adaptive_very_fast;
  PyObject * py_counter_write1_adaptive_very_fast;
  pyatrshmlog_int32_t counter_write2;
  PyObject * py_counter_write2;
  pyatrshmlog_int32_t counter_write2_discard;
  PyObject * py_counter_write2_discard;
  pyatrshmlog_int32_t counter_write2_wait;
  PyObject * py_counter_write2_wait;
  pyatrshmlog_int32_t counter_write2_adaptive;
  PyObject * py_counter_write2_adaptive;
  pyatrshmlog_int32_t counter_write2_adaptive_fast;
  PyObject * py_counter_write2_adaptive_fast;
  pyatrshmlog_int32_t counter_write2_adaptive_very_fast;
  PyObject * py_counter_write2_adaptive_very_fast;
  
  result = PyAtrshmlog_read_fetch(u.a,
				  &index,
				  &transferbuffer[0],
				  &o_length,
				  &o_pid,
				  &o_tid,
				  &inittime_seconds,
				  &inittime_nanos,
				  &o_inittsc_before,
				  &o_inittsc_after,
				  &lasttime_seconds,
				  &lasttime_nanos,
				  &o_lasttsc_before,
				  &o_lasttsc_after,
				  &o_difftimetransfer,
				  &o_starttransfer,
				  &o_acquiretime,
				  &o_id,
				  &o_number_dispatched,
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
			       
  if (result < 0)
    {
      // return -1; we construct a tuple and give it back.
      r = PyLong_FromLong(result);

      if (r == NULL)
	return NULL;
      
      t = PyTuple_New(1);

      if (t == NULL)
	return NULL;
      
      PyTuple_SET_ITEM(t,0,r);

      return t;
    }

  
  if (result > 0)
    {
      // return -1; we construct a tuple and give it back.
      r = PyLong_FromLong(1);

      if (r == NULL)
	return NULL;
      
      py_length = PyLong_FromLong(0);

      if (py_length == NULL)
	return NULL;

      t = PyTuple_New(2);

      if (t == NULL)
	return NULL;
      
      PyTuple_SET_ITEM(t,0,r);
      PyTuple_SET_ITEM(t,1,py_length);

      return t;
    }

  if (o_length == 0)
    {
      r = PyLong_FromLong(1);

      if (r == NULL)
	return NULL;
      
      py_length = PyLong_FromLong(0);

      if (py_length == NULL)
	return NULL;

      t = PyTuple_New(2);

      if (t == NULL)
	return NULL;
      
      PyTuple_SET_ITEM(t,0,r);
      PyTuple_SET_ITEM(t,1,py_length);

      return t;
    }

  r = PyLong_FromLong(0);

  PyObject * b = PyByteArray_FromStringAndSize(transferbuffer, o_length);

  if (b == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of buffer failed.");
      return NULL;
    }

  py_index = PyLong_FromLong(index);
  
  if (py_index == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of index failed.");
      return NULL;
    }
  
  // ok. we have a len > 0 
  py_length = PyLong_FromLong(o_length);

  if (py_length == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of length failed.");
      return NULL;
    }
  
  py_pid = PyLong_FromUnsignedLongLong(o_pid);

  if (py_pid == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of pid failed.");
      return NULL;
    }
  
  py_tid = PyLong_FromUnsignedLongLong(o_tid);

  if (py_tid == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of tid failed.");
      return NULL;
    }
  
  py_inittime_s = PyLong_FromUnsignedLongLong(inittime_seconds);

  if (py_tid == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of tid failed.");
      return NULL;
    }
  
  py_inittime_n = PyLong_FromLong(inittime_nanos);

  if (py_inittime_n == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of inittime_n failed.");
      return NULL;
    }
  
  py_inittsc_before = PyLong_FromUnsignedLongLong(o_inittsc_before);

  if (py_inittsc_before == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of inittsc_before failed.");
      return NULL;
    }
  
  py_inittsc_after = PyLong_FromUnsignedLongLong(o_inittsc_after);

  if (py_inittsc_after == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of inittsc_after failed.");
      return NULL;
    }
  
  py_lasttime_s = PyLong_FromUnsignedLongLong(lasttime_seconds);

  if (py_lasttime_s == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of lasttime_s failed.");
      return NULL;
    }
  
  py_lasttime_n = PyLong_FromLong(lasttime_nanos);

  if (py_lasttime_n == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of lasttime_n failed.");
      return NULL;
    }
  
  py_lasttsc_before = PyLong_FromUnsignedLongLong(o_lasttsc_before);

  if (py_lasttsc_before == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of lasttsc_before failed.");
      return NULL;
    }
  
  py_lasttsc_after = PyLong_FromUnsignedLongLong(o_lasttsc_after);

  if (py_lasttsc_after == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of lasttsc_after failed.");
      return NULL;
    }
  
  py_difftimetransfer = PyLong_FromUnsignedLongLong(o_difftimetransfer);

  if (py_difftimetransfer == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of difftimetransfer failed.");
      return NULL;
    }
  
  py_starttransfer = PyLong_FromUnsignedLongLong(o_starttransfer);

  if (py_starttransfer == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of starttransfer failed.");
      return NULL;
    }
  
  py_acquiretime = PyLong_FromUnsignedLongLong(o_acquiretime);

  if (py_acquiretime == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of acquiretime failed.");
      return NULL;
    }
  
  py_id = PyLong_FromLong(o_id);

  if (py_id == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of id failed.");
      return NULL;
    }
  
  py_number_dispatched = PyLong_FromLong(o_number_dispatched);

  if (py_number_dispatched == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of number_dispatched failed.");
      return NULL;
    }
  
  py_counter_write0 = PyLong_FromLong(counter_write0);

  if (py_counter_write0 == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of counter_write0 failed.");
      return NULL;
    }
  
  py_counter_write0_discard = PyLong_FromLong(counter_write0_discard);

  if (py_counter_write0_discard == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of counter_write0_discard failed.");
      return NULL;
    }
  
  py_counter_write0_wait = PyLong_FromLong(counter_write0_wait);

  if (py_counter_write0_wait == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of counter_write0_wait failed.");
      return NULL;
    }
  
  py_counter_write0_adaptive = PyLong_FromLong(counter_write0_adaptive);

  if (py_counter_write0_adaptive == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of counter_write0_adaptive failed.");
      return NULL;
    }
  
  py_counter_write0_adaptive_fast = PyLong_FromLong(counter_write0_adaptive_fast);

  if (py_counter_write0_adaptive_fast == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of counter_write0_adaptive_fast failed.");
      return NULL;
    }
  
  py_counter_write0_adaptive_very_fast = PyLong_FromLong(counter_write0_adaptive_very_fast);

  if (py_counter_write0_adaptive_very_fast == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of counter_write0_adaptive_very_fast failed.");
      return NULL;
    }
  
  py_counter_write1 = PyLong_FromLong(counter_write1);

  if (py_counter_write1 == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of counter_write1 failed.");
      return NULL;
    }
  
  py_counter_write1_discard = PyLong_FromLong(counter_write1_discard);

  if (py_counter_write1_discard == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of counter_write1_discard failed.");
      return NULL;
    }
  
  py_counter_write1_wait = PyLong_FromLong(counter_write1_wait);

  if (py_counter_write1_wait == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of counter_write1_wait failed.");
      return NULL;
    }
  
  py_counter_write1_adaptive = PyLong_FromLong(counter_write1_adaptive);

  if (py_counter_write1_adaptive == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of counter_write1_adaptive failed.");
      return NULL;
    }
  
  py_counter_write1_adaptive_fast = PyLong_FromLong(counter_write1_adaptive_fast);

  if (py_counter_write1_adaptive_fast == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of counter_write1_adaptive_fast failed.");
      return NULL;
    }
  
  py_counter_write1_adaptive_very_fast = PyLong_FromLong(counter_write1_adaptive_very_fast);

  if (py_counter_write1_adaptive_very_fast == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of counter_write1_adaptive_very_fast failed.");
      return NULL;
    }
  
  py_counter_write2 = PyLong_FromLong(counter_write2);

  if (py_counter_write2 == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of counter_write2 failed.");
      return NULL;
    }
  
  py_counter_write2_discard = PyLong_FromLong(counter_write2_discard);

  if (py_counter_write2_discard == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of counter_write2_discard failed.");
      return NULL;
    }
  
  py_counter_write2_wait = PyLong_FromLong(counter_write2_wait);

  if (py_counter_write2_wait == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of counter_write2_wait failed.");
      return NULL;
    }
  
  py_counter_write2_adaptive = PyLong_FromLong(counter_write2_adaptive);

  if (py_counter_write2_adaptive == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of counter_write2_adaptive failed.");
      return NULL;
    }
  
  py_counter_write2_adaptive_fast = PyLong_FromLong(counter_write2_adaptive_fast);

  if (py_counter_write2_adaptive_fast == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of counter_write2_adaptive_fast failed.");
      return NULL;
    }
  
  py_counter_write2_adaptive_very_fast = PyLong_FromLong(counter_write2_adaptive_very_fast);

  if (py_counter_write2_adaptive_very_fast == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read_fetch : create of counter_write2_adaptive_very_fast failed.");
      return NULL;
    }
  

  t = PyTuple_New(38);

  if (t == NULL)
    {
      PyErr_SetString(AtrshmlogError, "read : create of tuple failed.");
      return NULL;
    }
  
  PyTuple_SET_ITEM(t, 0, r);
  PyTuple_SET_ITEM(t, 1, py_index);
  PyTuple_SET_ITEM(t, 2, py_length);
  PyTuple_SET_ITEM(t, 3, b);
  PyTuple_SET_ITEM(t, 4, py_pid);
  PyTuple_SET_ITEM(t, 5, py_tid);
  PyTuple_SET_ITEM(t, 6, py_inittime_s);
  PyTuple_SET_ITEM(t, 7, py_inittime_n);
  PyTuple_SET_ITEM(t, 8, py_inittsc_before);
  PyTuple_SET_ITEM(t, 9, py_inittsc_after);
  PyTuple_SET_ITEM(t, 10, py_lasttime_s);
  PyTuple_SET_ITEM(t, 11, py_lasttime_n);
  PyTuple_SET_ITEM(t, 12, py_lasttsc_before);
  PyTuple_SET_ITEM(t, 13, py_lasttsc_after);
  PyTuple_SET_ITEM(t, 14, py_difftimetransfer);
  PyTuple_SET_ITEM(t, 15, py_starttransfer);
  PyTuple_SET_ITEM(t, 16, py_acquiretime);
  PyTuple_SET_ITEM(t, 17, py_id);
  PyTuple_SET_ITEM(t, 18, py_number_dispatched);
  PyTuple_SET_ITEM(t, 19, py_counter_write0);
  PyTuple_SET_ITEM(t, 20, py_counter_write0_discard);
  PyTuple_SET_ITEM(t, 21, py_counter_write0_wait);
  PyTuple_SET_ITEM(t, 22, py_counter_write0_adaptive);
  PyTuple_SET_ITEM(t, 23, py_counter_write0_adaptive_fast);
  PyTuple_SET_ITEM(t, 24, py_counter_write0_adaptive_very_fast);
  PyTuple_SET_ITEM(t, 25, py_counter_write1);
  PyTuple_SET_ITEM(t, 26, py_counter_write1_discard);
  PyTuple_SET_ITEM(t, 27, py_counter_write1_wait);
  PyTuple_SET_ITEM(t, 28, py_counter_write1_adaptive);
  PyTuple_SET_ITEM(t, 29, py_counter_write1_adaptive_fast);
  PyTuple_SET_ITEM(t, 30, py_counter_write1_adaptive_very_fast);
  PyTuple_SET_ITEM(t, 31, py_counter_write2);
  PyTuple_SET_ITEM(t, 32, py_counter_write2_discard);
  PyTuple_SET_ITEM(t, 33, py_counter_write2_wait);
  PyTuple_SET_ITEM(t, 34, py_counter_write2_adaptive);
  PyTuple_SET_ITEM(t, 35, py_counter_write2_adaptive_fast);
  PyTuple_SET_ITEM(t, 36, py_counter_write2_adaptive_very_fast);
  PyTuple_SET_ITEM(t, 37, py_counter_write2_adaptive_very_fast);
  
  return t;
}

/******************************************************/

/**
 * \brief Verify the shm struct is ok.
 */
static PyObject*
python_atrshmlog_verify(PyObject *self, PyObject *args)
{
  PyAtrshmlog_verify_RETURN result;

  result = PyAtrshmlog_verify();
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief Create the shm area.
 */
static PyObject*
python_atrshmlog_create(PyObject *self, PyObject *args)
{
  PyAtrshmlog_create_RETURN result;

  int ipckey;

  int count;
  
  if (!PyArg_ParseTuple(args, "ii", &ipckey, &count))
    {
      PyErr_SetString(AtrshmlogError, "create : fetch failed (ipckey, count)");
      return NULL;
    }

  result = PyAtrshmlog_create(ipckey,count);
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief We delete the shm area
 */
static PyObject*
python_atrshmlog_delete(PyObject *self, PyObject *args)
{
  PyAtrshmlog_delete_RETURN result;

  int shmid;

  if (!PyArg_ParseTuple(args, "i", &shmid))
    {
      PyErr_SetString(AtrshmlogError, "delete : fetch failed (shmid)");
      return NULL;
    }

  result = PyAtrshmlog_delete(shmid);
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief We cleanup any resources in the buffer.
 */
static PyObject*
python_atrshmlog_cleanup_locks(PyObject *self, PyObject *args)
{
  u_t u;
  
  if (!PyArg_ParseTuple(args, "K", &u.p))
    {
      PyErr_SetString(AtrshmlogError, "cleanup_locks : fetch failed (area)");
      return NULL;
    }

  PyAtrshmlog_cleanup_locks(u.a);
 
  return Py_None;
}

/******************************************************/

/**
 * \brief We initialize the buffer after it is attached 
 */
static PyObject*
python_atrshmlog_init_shm_log(PyObject *self, PyObject *args)
{
  PyAtrshmlog_init_shm_log_RETURN result;

  int count;
  
  u_t u;
  
  if (!PyArg_ParseTuple(args, "Ki", &u.p, &count))
    {
      PyErr_SetString(AtrshmlogError, "init_shm_log : fetch failed (area, count)");
      return NULL;
    }

  result = PyAtrshmlog_init_shm_log(u.a, count);
 
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief We set a new byte in the area
 */
static PyObject*
python_atrshmlog_poke(PyObject *self, PyObject *args)
{
  PyAtrshmlog_poke_RETURN result;

  int val;

  int index;
  
  u_t u;
  
  if (!PyArg_ParseTuple(args, "Kii", &u.p, &index, &val))
    {
      PyErr_SetString(AtrshmlogError, "poke : fetch failed (area, index, value)");
      return NULL;
    }

  result = PyAtrshmlog_poke(u.a, index, val);
  
  return PyLong_FromLong(result);
}

/******************************************************/

/**
 * \brief We get a byte from the area
 */
static PyObject*
python_atrshmlog_peek(PyObject *self, PyObject *args)
{
  PyAtrshmlog_peek_RETURN result;

  int index;
  
  u_t u;
  
  if (!PyArg_ParseTuple(args, "Ki", &u.p, &index))
    {
      PyErr_SetString(AtrshmlogError, "peek : fetch failed (area, index)");
      return NULL;
    }

  result = PyAtrshmlog_peek(u.a, index);

  return PyLong_FromLong(result);
}


/********************************************************************/

static PyMethodDef AtrshmlogMethods[] = {
    {"gettime",  python_atrshmlog_gettime, METH_VARARGS,
     "Get a clicktime for an event."},

    {"write",  python_atrshmlog_write, METH_VARARGS,
     "Write a short log."},
    
    {"write0",  python_atrshmlog_write0, METH_VARARGS,
     "Write a short log."},
    
    {"attach",  python_atrshmlog_attach, METH_VARARGS,
     "Attach to the area."},

    {"sleep_nanos",  python_atrshmlog_sleep_nanos, METH_VARARGS,
     "We wait nanoseconds."},

    {"get_statistics_max_index",  python_atrshmlog_get_statistics_max_index, METH_VARARGS,
     "We deliver the max index of the statistics buffer."},

    {"get_statistics",  python_atrshmlog_get_statistics, METH_VARARGS,
     "We deliver the statistics counter."},

    {"set_env_prefix",  python_atrshmlog_set_env_prefix, METH_VARARGS,
     "We set the prefix for name lookup in the program."},

    {"get_env_prefix",  python_atrshmlog_get_env_prefix, METH_VARARGS,
     "We get the actual name of the environment variable prefix."},

    {"get_env",  python_atrshmlog_get_env, METH_VARARGS,
     "We get the env variable value of prefix and the supported suffix."},

    {"get_env_shmid",  python_atrshmlog_get_env_shmid, METH_VARARGS,
     "We get the value of the environment variable holding the shmid in text."},

    {"get_env_id_suffix",  python_atrshmlog_get_env_id_suffix, METH_VARARGS,
     "We get the value of the environment variable holding the shmid in text."},

    {"get_version",  python_atrshmlog_get_version, METH_VARARGS,
     "Get the major version."},

    {"get_minor_version",  python_atrshmlog_get_minor_version, METH_VARARGS,
     "Get the minor version."},

    {"get_patch_version",  python_atrshmlog_get_patch_version, METH_VARARGS,
     "Get the patch version."},

    {"get_event_locks_max",  python_atrshmlog_get_event_locks_max, METH_VARARGS,
     "Get the max of event locks - 1."},

    {"set_event_locks_max",  python_atrshmlog_set_event_locks_max, METH_VARARGS,
     "Set the max of event locks."},

    {"get_event",  python_atrshmlog_get_event, METH_VARARGS,
     "Get an event value."},

    {"set_event_on",  python_atrshmlog_set_event_on, METH_VARARGS,
     "Set an event to 1."},

    {"set_event_off",  python_atrshmlog_set_event_off, METH_VARARGS,
     "Set an event to 0."},

    {"get_logging",  python_atrshmlog_get_logging, METH_VARARGS,
     "Get the logging status via function."},

    {"set_logging_process_on",  python_atrshmlog_set_logging_process_on, METH_VARARGS,
     "Set the logging to on."},

    {"set_logging_process_off",  python_atrshmlog_set_logging_process_off, METH_VARARGS,
     "Set the logging to off."},

    {"set_logging_process_off_final",  python_atrshmlog_set_logging_process_off_final, METH_VARARGS,
     "Switch the logging for the process off finally."},

    {"get_shmid",  python_atrshmlog_get_shmid, METH_VARARGS,
     "Get the shmid code."},

    {"get_area",  python_atrshmlog_get_area, METH_VARARGS,
     "Get the shm area start adress."},

    {"get_area_ich_habe_fertig",  python_atrshmlog_get_area_ich_habe_fertig, METH_VARARGS,
     "Get a shm area flag ich_habe_fertig."},

    {"set_area_ich_habe_fertig",  python_atrshmlog_set_area_ich_habe_fertig, METH_VARARGS,
     "Set the shm area flag ich_habe_fertig."},

    {"get_area_count",  python_atrshmlog_get_area_count, METH_VARARGS,
     "Get the shm count of buffers."},

    {"get_area_version",  python_atrshmlog_get_area_version, METH_VARARGS,
     "Get the shm version."},

    {"get_buffer_max_size",  python_atrshmlog_get_buffer_max_size, METH_VARARGS,
     "Get the maximum size of logbuffer."},

    {"get_buffer_size",  python_atrshmlog_get_buffer_size, METH_VARARGS,
     "Get the actual size of logbuffer in process."},

    {"set_buffer_size",  python_atrshmlog_set_buffer_size, METH_VARARGS,
     "Set the actual size for log buffer in process."},

    {"get_f_list_buffer_slave_count",  python_atrshmlog_get_f_list_buffer_slave_count, METH_VARARGS,
     "Get the number of slave threads running."},

    {"set_f_list_buffer_slave_count",  python_atrshmlog_set_f_list_buffer_slave_count, METH_VARARGS,
     "Set the number of slave threads startet."},

    {"get_clock_id",  python_atrshmlog_get_clock_id, METH_VARARGS,
     "Get the clock id used in get time."},

    {"set_clock_id",  python_atrshmlog_set_clock_id, METH_VARARGS,
     "Set the clock id in get time."},

    {"set_f_list_buffer_slave_run_off",  python_atrshmlog_set_f_list_buffer_slave_run_off, METH_VARARGS,
     "Set the slave run flag to off."},

    {"set_wait_for_slaves_on",  python_atrshmlog_set_wait_for_slaves_on, METH_VARARGS,
     "Set the wait for slaves flag to on."},

    {"set_wait_for_slaves_off",  python_atrshmlog_set_wait_for_slaves_off, METH_VARARGS,
     "Set the wait for slaves flag to off."},

    {"get_wait_for_slaves",  python_atrshmlog_get_wait_for_slaves, METH_VARARGS,
     "Get the wait for slaves flag."},

    {"get_f_list_buffer_slave_wait",  python_atrshmlog_get_f_list_buffer_slave_wait, METH_VARARGS,
     "Get the nanos the slave waits after going to sleep."},

    {"set_f_list_buffer_slave_wait",  python_atrshmlog_set_f_list_buffer_slave_wait, METH_VARARGS,
     "Set the nanos the slave waits in sleep."},

    {"get_acquire_count",  python_atrshmlog_get_acquire_count, METH_VARARGS,
     "Get the number of buffers for the process."},

    {"get_prealloc_buffer_count",  python_atrshmlog_get_prealloc_buffer_count, METH_VARARGS,
     "Get the number of buffers fetched from dyn memory in one alloc."},

    {"set_prealloc_buffer_count",  python_atrshmlog_set_prealloc_buffer_count, METH_VARARGS,
     "Set the number of buffers fetched with one alloc from dyn memory."},

    {"get_inittime",  python_atrshmlog_get_inittime, METH_VARARGS,
     "Get the inittime."},

    {"get_inittime_tsc_before",  python_atrshmlog_get_inittime_tsc_before, METH_VARARGS,
     "Get the click time before inittime."},

    {"get_inittime_tsc_after",  python_atrshmlog_get_inittime_tsc_after, METH_VARARGS,
     "Get the click time after inittime."},

    {"get_buffer_id",  python_atrshmlog_get_buffer_id, METH_VARARGS,
     "Get the buffer id."},

    {"stop",  python_atrshmlog_stop, METH_VARARGS,
     "Stop logging for this thread."},

    {"flush",  python_atrshmlog_flush, METH_VARARGS,
     "Flush buffers for this thread."},

    {"set_strategy",  python_atrshmlog_set_strategy, METH_VARARGS,
     "Set the strategy for this thread."},

    {"get_strategy",  python_atrshmlog_get_strategy, METH_VARARGS,
     "We get the strategy for this thread"},

    {"set_strategy_process",  python_atrshmlog_set_strategy_process, METH_VARARGS,
     "Set the strategy for the process"},

    {"get_strategy_process",  python_atrshmlog_get_strategy_process, METH_VARARGS,
     "We get the strategy for the process"},

    {"create_slave",  python_atrshmlog_create_slave, METH_VARARGS,
     "Start a new slave thread."},

    {"decrement_slave_count",  python_atrshmlog_decrement_slave_count, METH_VARARGS,
     "We decrement the slave count."},

    {"get_clicktime",  python_atrshmlog_get_clicktime, METH_VARARGS,
     "Get the real time with clicktime."},

    {"set_thread_fence_1",  python_atrshmlog_set_thread_fence_1, METH_VARARGS,
     "Set the fence 1 flag on."},

    {"set_thread_fence_2",  python_atrshmlog_set_thread_fence_2, METH_VARARGS,
     "Set the fence 2 flag on."},

    {"set_thread_fence_3",  python_atrshmlog_set_thread_fence_3, METH_VARARGS,
     "Set the fence 3 flag on."},

    {"set_thread_fence_4",  python_atrshmlog_set_thread_fence_4, METH_VARARGS,
     "Set the fence 4 flag on."},

    {"set_thread_fence_5",  python_atrshmlog_set_thread_fence_5, METH_VARARGS,
     "Set the fence 5 flag on."},

    {"set_thread_fence_6",  python_atrshmlog_set_thread_fence_6, METH_VARARGS,
     "Set the fence 6 flag on."},

    {"set_thread_fence_7",  python_atrshmlog_set_thread_fence_7, METH_VARARGS,
     "Set the fence 7 flag on."},

    {"set_thread_fence_8",  python_atrshmlog_set_thread_fence_8, METH_VARARGS,
     "Set the fence 8 flag on."},

    {"set_thread_fence_9",  python_atrshmlog_set_thread_fence_9, METH_VARARGS,
     "Set the fence 9 flag on."},

    {"set_thread_fence_10",  python_atrshmlog_set_thread_fence_10, METH_VARARGS,
     "Set the fence 10 flag on."},

    {"set_thread_fence_11",  python_atrshmlog_set_thread_fence_11, METH_VARARGS,
     "Set the fence 11 flag on."},

    {"set_thread_fence_12",  python_atrshmlog_set_thread_fence_12, METH_VARARGS,
     "Set the fence 12 flag on."},

    {"set_thread_fence_13",  python_atrshmlog_set_thread_fence_13, METH_VARARGS,
     "Set the fence 13 flag on."},

    {"get_thread_fence_1",  python_atrshmlog_get_thread_fence_1, METH_VARARGS,
     "We get a fence on or off flag."},

    {"get_thread_fence_2",  python_atrshmlog_get_thread_fence_2, METH_VARARGS,
     "We get a fence on or off flag."},

    {"get_thread_fence_3",  python_atrshmlog_get_thread_fence_3, METH_VARARGS,
     "We get a fence on or off flag."},

    {"get_thread_fence_4",  python_atrshmlog_get_thread_fence_4, METH_VARARGS,
     "We get a fence on or off flag."},

    {"get_thread_fence_5",  python_atrshmlog_get_thread_fence_5, METH_VARARGS,
     "We get a fence on or off flag."},

    {"get_thread_fence_6",  python_atrshmlog_get_thread_fence_6, METH_VARARGS,
     "We get a fence on or off flag."},

    {"get_thread_fence_7",  python_atrshmlog_get_thread_fence_7, METH_VARARGS,
     "We get a fence on or off flag."},

    {"get_thread_fence_8",  python_atrshmlog_get_thread_fence_8, METH_VARARGS,
     "We get a fence on or off flag."},

    {"get_thread_fence_9",  python_atrshmlog_get_thread_fence_9, METH_VARARGS,
     "We get a fence on or off flag."},

    {"get_thread_fence_10",  python_atrshmlog_get_thread_fence_10, METH_VARARGS,
     "We get a fence on or off flag."},

    {"get_thread_fence_11",  python_atrshmlog_get_thread_fence_11, METH_VARARGS,
     "We get a fence on or off flag."},

    {"get_thread_fence_12",  python_atrshmlog_get_thread_fence_12, METH_VARARGS,
     "We get a fence on or off flag."},

    {"get_thread_fence_13",  python_atrshmlog_get_thread_fence_13, METH_VARARGS,
     "We get a fence on or off flag."},

    {"get_realtime",  python_atrshmlog_get_realtime, METH_VARARGS,
     "We get the real time."},

    {"get_thread_locals_adress",  python_atrshmlog_get_thread_locals_adress, METH_VARARGS,
     "We get the thread locals adress of a thread."},

    {"get_tid",  python_atrshmlog_get_tid, METH_VARARGS,
     "We get the thread tid."},

    {"turn_logging_off",  python_atrshmlog_turn_logging_off, METH_VARARGS,
     "We switch the thread off and dispatch its buffers."},

    {"set_init_buffers_in_advance_on",  python_atrshmlog_set_init_buffers_in_advance_on, METH_VARARGS,
     "We turn the init in advance on."},

    {"set_init_buffers_in_advance_off",  python_atrshmlog_set_init_buffers_in_advance_off, METH_VARARGS,
     "We turn the init in advance off."},

    {"get_init_buffers_in_advance",  python_atrshmlog_get_init_buffers_in_advance, METH_VARARGS,
     "We get the init in advance flag."},

    {"get_next_slave_local",  python_atrshmlog_get_next_slave_local, METH_VARARGS,
     "We get the next element on slave list."},

    {"get_thread_local_tid",  python_atrshmlog_get_thread_local_tid, METH_VARARGS,
     "We get the tid of a thread local."},

    {"remove_slave_via_local",  python_atrshmlog_remove_slave_via_local, METH_VARARGS,
     "We remove the slave from the list of slaves."},

    {"reuse_thread_buffers",  python_atrshmlog_reuse_thread_buffers, METH_VARARGS,
     "We make reuse of buffers of a dead thread."},

    {"read",  python_atrshmlog_read, METH_VARARGS,
     "We read a buffer from shared memory. Outdated."},

    {"read_fetch",  python_atrshmlog_read_fetch, METH_VARARGS,
     "We read a buffer from shared memory. Actual."},

    {"verify",  python_atrshmlog_verify, METH_VARARGS,
     "Verify the shm struct is ok."},

    {"create",  python_atrshmlog_create, METH_VARARGS,
     "Create the shm area."},

    {"delete",  python_atrshmlog_delete, METH_VARARGS,
     "We delete the shm area."},

    {"cleanup_locks",  python_atrshmlog_cleanup_locks, METH_VARARGS,
     "We cleanup any resources in the buffer."},

    {"init_shm_log",  python_atrshmlog_init_shm_log, METH_VARARGS,
     "We initialize the buffer after it is attached."},

    {"poke",  python_atrshmlog_poke, METH_VARARGS,
     "We set a new byte in the area."},

    {"peek",  python_atrshmlog_peek, METH_VARARGS,
     "We get a byte from the area."},


    {NULL, NULL, 0, NULL}        /* Sentinel */
};

/**
 *  \brief We initialize for the use of the C api the core functions array
 */
static void init_core(pyatrshmlog_vfctv_t *PyAtrshmlog_API )
{
  PyAtrshmlog_API[PyAtrshmlog_attach_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_attach;
  PyAtrshmlog_API[PyAtrshmlog_gettime_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_gettime;
  PyAtrshmlog_API[PyAtrshmlog_write0_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_write0;
  PyAtrshmlog_API[PyAtrshmlog_write_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_write;
  PyAtrshmlog_API[PyAtrshmlog_sleep_nanos_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_sleep_nanos;
  PyAtrshmlog_API[PyAtrshmlog_get_statistics_max_index_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_statistics_max_index;
  PyAtrshmlog_API[PyAtrshmlog_get_statistics_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_statistics;
  PyAtrshmlog_API[PyAtrshmlog_set_env_prefix_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_set_env_prefix;
  PyAtrshmlog_API[PyAtrshmlog_get_env_prefix_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_env_prefix;
  PyAtrshmlog_API[PyAtrshmlog_get_env_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_env;
  PyAtrshmlog_API[PyAtrshmlog_get_env_shmid_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_env_shmid;
  PyAtrshmlog_API[PyAtrshmlog_get_env_id_suffix_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_env_id_suffix;
  PyAtrshmlog_API[PyAtrshmlog_get_version_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_version;
  PyAtrshmlog_API[PyAtrshmlog_get_minor_version_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_minor_version;
  PyAtrshmlog_API[PyAtrshmlog_get_patch_version_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_patch_version;
  PyAtrshmlog_API[PyAtrshmlog_get_event_locks_max_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_event_locks_max;
  PyAtrshmlog_API[PyAtrshmlog_set_event_locks_max_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_set_event_locks_max;
  PyAtrshmlog_API[PyAtrshmlog_get_event_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_event;
  PyAtrshmlog_API[PyAtrshmlog_set_event_on_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_set_event_on;
  PyAtrshmlog_API[PyAtrshmlog_set_event_off_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_set_event_off;
  PyAtrshmlog_API[PyAtrshmlog_get_logging_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_logging;
  PyAtrshmlog_API[PyAtrshmlog_set_logging_process_on_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_set_logging_process_on;
  PyAtrshmlog_API[PyAtrshmlog_set_logging_process_off_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_set_logging_process_off;
  PyAtrshmlog_API[PyAtrshmlog_set_logging_process_off_final_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_set_logging_process_off_final;
  PyAtrshmlog_API[PyAtrshmlog_get_shmid_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_shmid;
  PyAtrshmlog_API[PyAtrshmlog_get_area_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_area;
  PyAtrshmlog_API[PyAtrshmlog_get_area_ich_habe_fertig_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_area_ich_habe_fertig;
  PyAtrshmlog_API[PyAtrshmlog_set_area_ich_habe_fertig_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_set_area_ich_habe_fertig;
  PyAtrshmlog_API[PyAtrshmlog_get_area_count_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_area_count;
  PyAtrshmlog_API[PyAtrshmlog_get_area_version_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_area_version;
  PyAtrshmlog_API[PyAtrshmlog_get_buffer_max_size_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_buffer_max_size;
  PyAtrshmlog_API[PyAtrshmlog_get_buffer_size_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_buffer_size;
  PyAtrshmlog_API[PyAtrshmlog_set_buffer_size_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_set_buffer_size;
  PyAtrshmlog_API[PyAtrshmlog_get_f_list_buffer_slave_count_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_f_list_buffer_slave_count;
  PyAtrshmlog_API[PyAtrshmlog_set_f_list_buffer_slave_count_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_set_f_list_buffer_slave_count;
  PyAtrshmlog_API[PyAtrshmlog_get_clock_id_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_clock_id;
  PyAtrshmlog_API[PyAtrshmlog_set_clock_id_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_set_clock_id;
  PyAtrshmlog_API[PyAtrshmlog_set_f_list_buffer_slave_run_off_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_set_f_list_buffer_slave_run_off;
  PyAtrshmlog_API[PyAtrshmlog_set_wait_for_slaves_on_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_set_wait_for_slaves_on;
  PyAtrshmlog_API[PyAtrshmlog_set_wait_for_slaves_off_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_set_wait_for_slaves_off;
  PyAtrshmlog_API[PyAtrshmlog_get_wait_for_slaves_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_wait_for_slaves;
  PyAtrshmlog_API[PyAtrshmlog_get_f_list_buffer_slave_wait_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_f_list_buffer_slave_wait;
  PyAtrshmlog_API[PyAtrshmlog_set_f_list_buffer_slave_wait_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_set_f_list_buffer_slave_wait;
  PyAtrshmlog_API[PyAtrshmlog_get_acquire_count_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_acquire_count;
  PyAtrshmlog_API[PyAtrshmlog_get_prealloc_buffer_count_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_prealloc_buffer_count;
  PyAtrshmlog_API[PyAtrshmlog_set_prealloc_buffer_count_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_set_prealloc_buffer_count;
  PyAtrshmlog_API[PyAtrshmlog_get_inittime_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_inittime;
  PyAtrshmlog_API[PyAtrshmlog_get_inittime_tsc_before_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_inittime_tsc_before;
  PyAtrshmlog_API[PyAtrshmlog_get_inittime_tsc_after_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_inittime_tsc_after;
  PyAtrshmlog_API[PyAtrshmlog_get_buffer_id_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_buffer_id;
  PyAtrshmlog_API[PyAtrshmlog_stop_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_stop;
  PyAtrshmlog_API[PyAtrshmlog_flush_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_flush;
  PyAtrshmlog_API[PyAtrshmlog_set_strategy_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_set_strategy;
  PyAtrshmlog_API[PyAtrshmlog_get_strategy_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_strategy;
  PyAtrshmlog_API[PyAtrshmlog_set_strategy_process_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_set_strategy_process;
  PyAtrshmlog_API[PyAtrshmlog_get_strategy_process_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_strategy_process;
  PyAtrshmlog_API[PyAtrshmlog_create_slave_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_create_slave;
  PyAtrshmlog_API[PyAtrshmlog_decrement_slave_count_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_decrement_slave_count;
  PyAtrshmlog_API[PyAtrshmlog_get_clicktime_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_clicktime;
  PyAtrshmlog_API[PyAtrshmlog_set_thread_fence_1_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_set_thread_fence_1;
  PyAtrshmlog_API[PyAtrshmlog_set_thread_fence_2_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_set_thread_fence_2;
  PyAtrshmlog_API[PyAtrshmlog_set_thread_fence_3_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_set_thread_fence_3;
  PyAtrshmlog_API[PyAtrshmlog_set_thread_fence_4_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_set_thread_fence_4;
  PyAtrshmlog_API[PyAtrshmlog_set_thread_fence_5_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_set_thread_fence_5;
  PyAtrshmlog_API[PyAtrshmlog_set_thread_fence_6_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_set_thread_fence_6;
  PyAtrshmlog_API[PyAtrshmlog_set_thread_fence_7_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_set_thread_fence_7;
  PyAtrshmlog_API[PyAtrshmlog_set_thread_fence_8_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_set_thread_fence_8;
  PyAtrshmlog_API[PyAtrshmlog_set_thread_fence_9_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_set_thread_fence_9;
  PyAtrshmlog_API[PyAtrshmlog_set_thread_fence_10_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_set_thread_fence_10;
  PyAtrshmlog_API[PyAtrshmlog_set_thread_fence_11_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_set_thread_fence_11;
  PyAtrshmlog_API[PyAtrshmlog_set_thread_fence_12_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_set_thread_fence_12;
  PyAtrshmlog_API[PyAtrshmlog_set_thread_fence_13_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_set_thread_fence_13;
  PyAtrshmlog_API[PyAtrshmlog_get_thread_fence_1_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_thread_fence_1;
  PyAtrshmlog_API[PyAtrshmlog_get_thread_fence_2_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_thread_fence_2;
  PyAtrshmlog_API[PyAtrshmlog_get_thread_fence_3_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_thread_fence_3;
  PyAtrshmlog_API[PyAtrshmlog_get_thread_fence_4_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_thread_fence_4;
  PyAtrshmlog_API[PyAtrshmlog_get_thread_fence_5_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_thread_fence_5;
  PyAtrshmlog_API[PyAtrshmlog_get_thread_fence_6_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_thread_fence_6;
  PyAtrshmlog_API[PyAtrshmlog_get_thread_fence_7_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_thread_fence_7;
  PyAtrshmlog_API[PyAtrshmlog_get_thread_fence_8_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_thread_fence_8;
  PyAtrshmlog_API[PyAtrshmlog_get_thread_fence_9_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_thread_fence_9;
  PyAtrshmlog_API[PyAtrshmlog_get_thread_fence_10_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_thread_fence_10;
  PyAtrshmlog_API[PyAtrshmlog_get_thread_fence_11_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_thread_fence_11;
  PyAtrshmlog_API[PyAtrshmlog_get_thread_fence_12_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_thread_fence_12;
  PyAtrshmlog_API[PyAtrshmlog_get_thread_fence_13_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_thread_fence_13;
  PyAtrshmlog_API[PyAtrshmlog_get_realtime_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_realtime;
  PyAtrshmlog_API[PyAtrshmlog_get_thread_locals_adress_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_thread_locals_adress;
  PyAtrshmlog_API[PyAtrshmlog_get_tid_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_tid;
  PyAtrshmlog_API[PyAtrshmlog_turn_logging_off_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_turn_logging_off;
  PyAtrshmlog_API[PyAtrshmlog_set_init_buffers_in_advance_on_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_set_init_buffers_in_advance_on;
  PyAtrshmlog_API[PyAtrshmlog_set_init_buffers_in_advance_off_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_set_init_buffers_in_advance_off;
  PyAtrshmlog_API[PyAtrshmlog_get_init_buffers_in_advance_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_init_buffers_in_advance;
  PyAtrshmlog_API[PyAtrshmlog_get_next_slave_local_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_next_slave_local;
  PyAtrshmlog_API[PyAtrshmlog_get_thread_local_tid_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_get_thread_local_tid;
  PyAtrshmlog_API[PyAtrshmlog_remove_slave_via_local_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_remove_slave_via_local;
  PyAtrshmlog_API[PyAtrshmlog_reuse_thread_buffers_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_reuse_thread_buffers;
  PyAtrshmlog_API[PyAtrshmlog_read_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_read;
  PyAtrshmlog_API[PyAtrshmlog_read_fetch_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_read_fetch;
  PyAtrshmlog_API[PyAtrshmlog_verify_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_verify;
  PyAtrshmlog_API[PyAtrshmlog_create_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_create;
  PyAtrshmlog_API[PyAtrshmlog_delete_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_delete;
  PyAtrshmlog_API[PyAtrshmlog_cleanup_locks_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_cleanup_locks;
  PyAtrshmlog_API[PyAtrshmlog_init_shm_log_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_init_shm_log;
  PyAtrshmlog_API[PyAtrshmlog_poke_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_poke;
  PyAtrshmlog_API[PyAtrshmlog_peek_NUM] = (pyatrshmlog_vfctv_t)PyAtrshmlog_peek;
}

#if ATRSHMLOG_PYTHON_VERSION == 3

static struct PyModuleDef atrshmlogmodule = {
   PyModuleDef_HEAD_INIT,
   "atrshmlog",  
   "Logging with the atrshmlog c module via shared memory", 
   -1,  
   AtrshmlogMethods
};

#endif

#if ATRSHMLOG_PYTHON_VERSION == 3
#define ATRSHMLOG_INITNAME PyInit_atrshmlog
#define ATRSHMLOG_RETURN_INIT(__m) return (__m) 
#endif

#if ATRSHMLOG_PYTHON_VERSION == 2
#define ATRSHMLOG_INITNAME initatrshmlog
#define ATRSHMLOG_RETURN_INIT(__m) 
#endif

#if ATRSHMLOG_PYTHON_VERSION == 2
#else
# if ATRSHMLOG_PYTHON_VERSION == 3
# else
bumm bumm bumm bumm ups no known version from environment
# endif
#endif

PyMODINIT_FUNC
ATRSHMLOG_INITNAME(void)
{
  static pyatrshmlog_vfctv_t PyAtrshmlog_API[PyAtrshmlog_API_pointers];
  
  const char* v = Py_GetVersion();

  char mv[2];
  
  mv[0] = v[0];
  mv[1] = 0;

  int version = strtol(mv, NULL, 10);
  
  if (version != PY_MAJOR_VERSION)
    {
      ATRSHMLOG_RETURN_INIT(NULL);
    }

  // ok. we have at least the correct major version here
  PyObject *m;
  
# if ATRSHMLOG_PYTHON_VERSION == 3
  m = PyModule_Create(&atrshmlogmodule);
#endif

#if ATRSHMLOG_PYTHON_VERSION == 2
  m = Py_InitModule("atrshmlog", AtrshmlogMethods);
#endif
  
  if (m == NULL)
    ATRSHMLOG_RETURN_INIT(NULL);

  AtrshmlogError = PyErr_NewException("atrshmlog.error", NULL, NULL);
    
  Py_INCREF(AtrshmlogError);
    
  PyModule_AddObject(m, "error", AtrshmlogError);

  init_core(PyAtrshmlog_API);
  
  // we set the capsule now.
  PyObject *c_api_object;
  
  c_api_object = PyCapsule_New((void *)PyAtrshmlog_API, "atrshmlog._C_API", NULL);

  if (c_api_object != NULL)
    PyModule_AddObject(m, "_C_API", c_api_object);

  // we set the constants for the write and write0
  if (PyModule_AddIntConstant(m, "EVENT_POINT_IN_TIME_UCS", 112))
    {
      PyErr_SetString(AtrshmlogError, "EVENT_POINT_IN_TIME_UCS : build failed");
      ATRSHMLOG_RETURN_INIT(NULL);
    }
    
  if (PyModule_AddIntConstant(m, "EVENT_POINT_IN_TIME_C", 80))
    {
      PyErr_SetString(AtrshmlogError, "EVENT_POINT_IN_TIME_C : build failed");
      ATRSHMLOG_RETURN_INIT(NULL);
    }
    
  if (PyModule_AddIntConstant(m, "EVENT_INTERVAL_IN_TIME_UCS", 105))
    {
      PyErr_SetString(AtrshmlogError, "EVENT_INTERVAL_IN_TIME_UCS : build failed");
      ATRSHMLOG_RETURN_INIT(NULL);
    }
    
  if (PyModule_AddIntConstant(m, "EVENT_INTERVAL_IN_TIME_C", 73))
    {
      PyErr_SetString(AtrshmlogError, "EVENT_INTERVAL_IN_TIME_C : build failed");
      ATRSHMLOG_RETURN_INIT(NULL);
    }
    
  ATRSHMLOG_RETURN_INIT(m);
}

/* end of file */
