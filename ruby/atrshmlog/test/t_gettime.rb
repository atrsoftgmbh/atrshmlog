#!/usr/bin/ruby
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

require 'atrshmlog'

result = Atrshmlog::attach()

t = Atrshmlog::gettime()

print "gettime : #{t} : \n"

print " \n"

exit (0)

# end of main
