#!/usr/bin/ruby
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

require 'atrshmlog'

id = Atrshmlog::get_clock_id()

print "clicktime : #{id} : \n"

result = Atrshmlog::attach()

id = Atrshmlog::get_clock_id()

print "clicktime : #{id} : \n"

click = Atrshmlog::get_clicktime()

print "clicktime : #{click} : \n"


print " \n"

exit (0)

# end of main
