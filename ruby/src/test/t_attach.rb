#!/usr/bin/ruby
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

require 'atrshmlog'

result = Atrshmlog::attach();

print "attach : result : \n";

print "shmid " , Atrshmlog::get_shmid() , " : \n";

print "acquire count " , Atrshmlog::get_acquire_count() , " : \n";

print "buffer id " , Atrshmlog::get_buffer_id() , " : \n";

print "init buffers in advance " , Atrshmlog::get_init_buffers_in_advance() , " : \n";

print "buffer size " , Atrshmlog::get_buffer_size() , " : \n";

print "prealloc buffer count " , Atrshmlog::get_prealloc_buffer_count() , " : \n";

print "f list buffer slave count " , Atrshmlog::get_f_list_buffer_slave_count() , " : \n";

print "f list buffer slave wait " , Atrshmlog::get_f_list_buffer_slave_wait() , " : \n";

print "wait for slaves " , Atrshmlog::get_wait_for_slaves() , " : \n";

print "strategy process " , Atrshmlog::get_strategy_process() , " : \n";

print "autoflush process " , Atrshmlog::get_autoflush_process() , " : \n";

print "checksum " , Atrshmlog::get_checksum() , " : \n";

print "thread fence 1 " , Atrshmlog::get_thread_fence_1() , " : \n";

print "thread fence 2 " , Atrshmlog::get_thread_fence_2() , " : \n";

print "thread fence 3 " , Atrshmlog::get_thread_fence_3() , " : \n";

print "thread fence 4 " , Atrshmlog::get_thread_fence_4() , " : \n";

print "thread fence 5 " , Atrshmlog::get_thread_fence_5() , " : \n";

print "thread fence 6 " , Atrshmlog::get_thread_fence_6() , " : \n";

print "thread fence 7 " , Atrshmlog::get_thread_fence_7() , " : \n";

print "thread fence 8 " , Atrshmlog::get_thread_fence_8() , " : \n";

print "thread fence 9 " , Atrshmlog::get_thread_fence_9() , " : \n";

print "thread fence 10 " , Atrshmlog::get_thread_fence_10() , " : \n";

print "thread fence 11 " , Atrshmlog::get_thread_fence_11() , " : \n";

print "thread fence 12 " , Atrshmlog::get_thread_fence_12() , " : \n";

print "thread fence 13 " , Atrshmlog::get_thread_fence_13() , " : \n";

print "inittime tsc before " , Atrshmlog::get_inittime_tsc_before() , " : \n";

print "inittime tsc after " , Atrshmlog::get_inittime_tsc_after() , " : \n";

(s, n) = Atrshmlog::get_inittime();

print "inittime : #{s} : #{n} : \n";

print "event locks max " , Atrshmlog::get_event_locks_max() , " : \n";

print "clock id " , Atrshmlog::get_clock_id() , " : \n";

print "version " , Atrshmlog::get_version() , " : \n";

print "minor version " , Atrshmlog::get_minor_version() , " : \n";

print "patch version " , Atrshmlog::get_patch_version() , " : \n";

print "buffer max size " , Atrshmlog::get_buffer_max_size() , " : \n";




print " \n";

exit (0);

# end of main
