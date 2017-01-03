#!/usr/bin/ruby
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

require 'atrshmlog'

result = Atrshmlog::attach()

id = Atrshmlog::get_clock_id()

print "clock id : #{id} : \n"

oldid = Atrshmlog::set_clock_id(2)

print "clock id : #{oldid} : \n"

id = Atrshmlog::get_clock_id()

print "clock id : #{id} : \n"

oldid = Atrshmlog::set_clock_id(1)

print "clock id : #{oldid} : \n"

id = Atrshmlog::get_clock_id()

print "clock id : #{id} : \n"


print " \n"

exit (0)

# end of main
