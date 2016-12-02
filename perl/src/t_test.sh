#!/bin/bash
#
# $Id:$
#
# perl tests

if [ $# -ne 2 ]
then
    exit 1
fi

tests/t_statistics.pl

tests/t_sleep_nanos.pl

tests/t_env_prefix.pl

tests/t_get_env.pl _ID

tests/t_version.pl

tests/t_event_locks_max.pl 10042

tests/t_event.pl

tests/t_logging.pl

tests/t_shmid.pl

tests/t_area.pl

tests/t_buffer_max_size.pl

tests/t_buffer_size.pl

tests/t_slave_count.pl 8

tests/t_clock_id.pl

tests/t_slave_run_off.pl 8

tests/t_wait_for_slaves.pl

tests/t_buffer_slave_wait.pl

tests/t_acquire_count.pl

tests/t_prealloc_count.pl

tests/t_get_inittime.pl

tests/t_get_realtime.pl

tests/t_stop.pl

tests/t_flush.pl

tests/t_strategy.pl

tests/t_create_slave.pl

tests/t_decrement_slave_count.pl

tests/t_fence.pl

tests/t_init_in_advance.pl

tests/t_next_slave.pl 3

tests/t_checksum.pl

tests/t_autoflush.pl

tests/t_verify.pl

tests/t_clicktime.pl

tests/t_thread_local.pl

tests/t_read_fetch.pl

tests/t_read_fetch.pl

tests/t_reuse_buffer.pl

tests/t_read_fetch.pl

tests/t_read_fetch.pl

tests/t_write.pl

tests/t_read_fetch.pl

tests/t_read_fetch.pl

tests/t_write_binary.pl

tests/t_read_fetch.pl

tests/t_read_fetch.pl

tests/t_write.pl

tests/t_read.pl 0

tests/t_read_fetch.pl

tests/t_read_fetch.pl

tests/t_create.pl $1 $2

. dot.atrshmlog

tests/t_init_shm_log.pl $2

tests/t_cleanup_locks.pl

tests/t_delete.pl $ATRSHMLOG_ID

