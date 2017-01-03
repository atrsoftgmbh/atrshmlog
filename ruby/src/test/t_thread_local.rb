#!/usr/bin/ruby
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

require 'atrshmlog'

result = Atrshmlog::attach()

tl = Atrshmlog::get_thread_locals_adress()

print "thread local : #{tl} : \n"

tid = Atrshmlog::get_tid()

print "thread local : #{tid} : \n"

tltid = Atrshmlog::get_thread_local_tid(tl)

print "thread local : #{tltid} : \n"

Atrshmlog::write(1, Atrshmlog::EVENT_POINT_IN_TIME_C, 42, 0, 0, "hallo")

tltid = Atrshmlog::get_thread_local_tid(tl)

print "thread local : #{tltid} : \n"

Atrshmlog::turn_logging_off(tl);

print "thread local : \n";

print " \n";

exit (0);

# end of main
