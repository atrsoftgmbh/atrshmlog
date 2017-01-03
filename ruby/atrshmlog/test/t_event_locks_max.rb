#!/usr/bin/ruby
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

require 'atrshmlog'

maxindex = Atrshmlog::get_event_locks_max()

print "event locks max : #{maxindex} : \n"

newindex  = ARGV[0].to_i

print "event locks max : #{newindex} : \n"

oldindex = Atrshmlog::set_event_locks_max(newindex)

print "event locks max : #{oldindex} : \n"

maxindex = Atrshmlog::get_event_locks_max()

print "event locks max : #{maxindex} :  \n"

result = Atrshmlog::attach()

oldindex = Atrshmlog::set_event_locks_max(newindex + 100)

print "event locks max : #{oldindex} : \n"

maxindex = Atrshmlog::get_event_locks_max()

print "event locks max : #{maxindex} : \n"


print " \n"

exit (0)

# end of main
