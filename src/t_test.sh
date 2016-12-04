#!/bin/bash
#
# $Id:$
#
# test the c functions

if [ $# -ne 2 ]
then
    exit 1
fi

atrshmlogcreate $1 $2
. dot.atrshmlog
atrshmloginit

tests/t_acquire_buffer 1000
tests/t_alloc 1000
tests/t_attach
tests/t_create_mapped_file 1 42
tests/t_attach_mapped_file 1 42
tests/t_create_slave 4 "ps -elf H"
tests/t_decrement_slave_count 10 15

tests/t_get_acquire_count 42
tests/t_get_area
tests/t_get_area_count
tests/t_get_area_version
tests/t_get_autoflush
tests/t_get_autoflush_process 42
tests/t_get_buffer_id 42
tests/t_get_buffer_size 42
tests/t_get_checksum 42
tests/t_get_clock_id 42
tests/t_get_env _ID
tests/t_get_env_prefix 
tests/t_get_env_shmid
tests/t_get_event 15 42
tests/t_get_event_locks_max 42
tests/t_get_thread_fence_1 42
tests/t_get_thread_fence_2 42
tests/t_get_thread_fence_3 42
tests/t_get_thread_fence_4 42
tests/t_get_thread_fence_5 42
tests/t_get_thread_fence_6 42
tests/t_get_thread_fence_7 42
tests/t_get_thread_fence_8 42
tests/t_get_thread_fence_9 42
tests/t_get_thread_fence_10 42
tests/t_get_thread_fence_11 42
tests/t_get_thread_fence_12 42
tests/t_get_thread_fence_13 42
tests/t_get_f_list_buffer_slave_count 42
tests/t_get_area_ihf
tests/t_get_init_buffers_in_advance 42
tests/t_get_inittime 13 17 
tests/t_get_inittime_tsc_after 42
tests/t_get_inittime_tsc_before 42
tests/t_get_logging
tests/t_get_prealloc_count 42
tests/t_get_raw 100 42
tests/t_get_realtime 
tests/t_get_shmid 42
tests/t_get_f_list_buffer_slave_wait 42
tests/t_get_stats
tests/t_get_strategy
tests/t_get_strategy_process 42
tests/t_get_tid
tests/t_get_wait_for_slaves 42
tests/t_init_events 0 
tests/t_init_events 1
tests/t_init_in_write
tests/t_set_advance_off
tests/t_set_advance_on
tests/t_set_autoflush_process
tests/t_set_buffer_size 42
tests/t_set_checksum
tests/t_set_clock_id
tests/t_set_env_prefix
tests/t_set_event
tests/t_set_event_locks_max 42
tests/t_set_fence_1
tests/t_set_fence_2
tests/t_set_fence_3
tests/t_set_fence_4
tests/t_set_fence_5
tests/t_set_fence_6
tests/t_set_fence_7
tests/t_set_fence_8
tests/t_set_fence_9
tests/t_set_fence_10
tests/t_set_fence_11
tests/t_set_fence_12
tests/t_set_fence_13
tests/t_set_area_ihf
tests/t_set_logging_process
tests/t_set_prealloc_count
tests/t_set_slave_wait_nanos
tests/t_set_strategy
tests/t_set_strategy_process
tests/t_set_wait_for_slaves
tests/t_set_f_list_buffer_slave_run_off
tests/t_sleep_nanos

tests/t_turn_logging_off
tests/t_read_fetch

tests/t_stop
tests/t_read_fetch

tests/t_set_strategy
tests/t_read_fetch

tests/t_set_autoflush
tests/t_read_fetch

tests/t_reuse_thread_buffers
tests/t_read_fetch


tests/t_transfer_mem_to_shm
tests/t_read_fetch

tests/t_dispatch_buffer
tests/t_read_fetch

tests/t_exit_cleanup
tests/t_read_fetch

tests/t_free
tests/t_read_fetch

tests/t_flush
tests/t_read_fetch

tests/t_write0
tests/t_read_fetch

tests/t_write1
tests/t_read_fetch

tests/t_write2 hallo anton
tests/t_read_fetch

N=$(tests/t_create 4912 8 | grep id | cut -c4-)

tests/t_cleanup_locks

tests/t_delete $ATRSHMLOG_ID

atrshmlogcreate $1 $2
. dot.atrshmlog

tests/t_init_shm_log $2

ATRSHMLOG_SLAVE_COUNT=8
export ATRSHMLOG_SLAVE_COUNT

tests/t_turn_slave_off 4

tests/t_remove_slave_via_local 4

tests/t_buffer_slave_proc

unset ATRSHMLOG_SLAVE_COUNT

