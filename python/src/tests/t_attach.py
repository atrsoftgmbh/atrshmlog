#!/usr/bin/python3
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

import sys
import atrshmlog

r = atrshmlog.attach()

print('attach : ' + str(r) + ' : ')

print('shmid ' + str(atrshmlog.get_shmid()) + ' : ')

print('acquire count ' + str(atrshmlog.get_acquire_count()) + ' : ')

print('buffer id ' + str(atrshmlog.get_buffer_id()) + ' : ')

print('init buffers in advance ' + str(atrshmlog.get_init_buffers_in_advance()) + ' : ')

print('buffer size ' + str(atrshmlog.get_buffer_size()) + ' : ')

print('prealloc buffer count ' + str(atrshmlog.get_prealloc_buffer_count()) + ' : ')

print('f list buffer slave count ' + str(atrshmlog.get_f_list_buffer_slave_count()) + ' : ')

print('f list buffer slave wait ' + str(atrshmlog.get_f_list_buffer_slave_wait()) + ' : ')

print('wait for slaves ' + str(atrshmlog.get_wait_for_slaves()) + ' : ')

print('strategy process ' + str(atrshmlog.get_strategy_process()) + ' : ')

print('autoflush process ' + str(atrshmlog.get_autoflush_process()) + ' : ')

print('checksum ' + str(atrshmlog.get_checksum()) + ' : ')

print('thread fence 1 ' + str(atrshmlog.get_thread_fence_1()) + ' : ')

print('thread fence 2 ' + str(atrshmlog.get_thread_fence_2()) + ' : ')

print('thread fence 3 ' + str(atrshmlog.get_thread_fence_3()) + ' : ')

print('thread fence 4 ' + str(atrshmlog.get_thread_fence_4()) + ' : ')

print('thread fence 5 ' + str(atrshmlog.get_thread_fence_5()) + ' : ')

print('thread fence 6 ' + str(atrshmlog.get_thread_fence_6()) + ' : ')

print('thread fence 7 ' + str(atrshmlog.get_thread_fence_7()) + ' : ')

print('thread fence 8 ' + str(atrshmlog.get_thread_fence_8()) + ' : ')

print('thread fence 9 ' + str(atrshmlog.get_thread_fence_9()) + ' : ')

print('thread fence 10 ' + str(atrshmlog.get_thread_fence_10()) + ' : ')

print('thread fence 11 ' + str(atrshmlog.get_thread_fence_11()) + ' : ')

print('thread fence 12 ' + str(atrshmlog.get_thread_fence_12()) + ' : ')

print('thread fence 13 ' + str(atrshmlog.get_thread_fence_13()) + ' : ')

print('inittime tsc before ' + str(atrshmlog.get_inittime_tsc_before()) + ' : ')

print('inittime tsc after ' + str(atrshmlog.get_inittime_tsc_after()) + ' : ')

(s, n) = atrshmlog.get_inittime()

print('inittime : ' + str(s) + ' : ' + str(n) + ' : ')

print('event locks max ' + str(atrshmlog.get_event_locks_max()) + ' : ')

print('clock id ' + str(atrshmlog.get_clock_id()) + ' : ')

print('version ' + str(atrshmlog.get_version()) + ' : ')

print('minor version ' + str(atrshmlog.get_minor_version()) + ' : ')

print('patch version ' + str(atrshmlog.get_patch_version()) + ' : ')

print('buffer max size ' + str(atrshmlog.get_buffer_max_size()) + ' : ')



print (' ')

exit(0);

# end of test

