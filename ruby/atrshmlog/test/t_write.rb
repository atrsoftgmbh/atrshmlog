#!/usr/bin/ruby
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

require 'atrshmlog'

result = Atrshmlog::attach()

ret = Atrshmlog::write(1, Atrshmlog::EVENT_POINT_IN_TIME_C, 42, 0, 0, "hallo")

print "write : #{ret} : \n"

print " \n"

exit (0)

# end of main
