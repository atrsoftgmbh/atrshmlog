#!/bin/bash
#
# $Id:$
#
# python tests

if [ $# -ne 2 ]
then
    exit 1
fi

tests/t_statistics.py

tests/t_sleep_nanos.py

tests/t_env_prefix.py

tests/t_get_env.py _ID

tests/t_version.py

tests/t_event_locks_max.py 10042

tests/t_event.py

tests/t_logging.py

tests/t_shmid.py

tests/t_area.py

tests/t_buffer_max_size.py

tests/t_buffer_size.py

tests/t_slave_count.py 8

tests/t_clock_id.py

tests/t_slave_run_off.py 8

tests/t_wait_for_slaves.py

tests/t_buffer_slave_wait.py

tests/t_acquire_count.py

tests/t_prealloc_count.py

tests/t_get_inittime.py

tests/t_get_realtime.py

tests/t_stop.py

tests/t_flush.py

tests/t_strategy.py

tests/t_create_slave.py

tests/t_decrement_slave_count.py

tests/t_fence.py

tests/t_init_in_advance.py

tests/t_next_slave.py 3

tests/t_checksum.py

tests/t_autoflush.py

tests/t_verify.py

tests/t_clicktime.py

tests/t_thread_local.py

tests/t_read_fetch.py

tests/t_read_fetch.py

tests/t_reuse_buffer.py

tests/t_read_fetch.py

tests/t_read_fetch.py

tests/t_write.py

tests/t_read_fetch.py

tests/t_read_fetch.py

tests/t_write0.py

tests/t_read_fetch.py

tests/t_read_fetch.py

tests/t_write.py

tests/t_read.py 0

tests/t_read_fetch.py

tests/t_read_fetch.py

tests/t_create.py $1 $2

. dot.atrshmlog

tests/t_init_shm_log.py $2

tests/t_cleanup_locks.py

tests/t_delete.py $ATRSHMLOG_ID


