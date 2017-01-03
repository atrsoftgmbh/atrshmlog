#!/usr/bin/ruby
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

require 'atrshmlog'

result = Atrshmlog::attach()

(s,n) = Atrshmlog::get_realtime()

print "get realtime : #{s} : #{n} : \n"

print " \n"

exit (0)

# end of main
