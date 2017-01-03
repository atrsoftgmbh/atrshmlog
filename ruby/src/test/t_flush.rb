#!/usr/bin/ruby
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

require 'atrshmlog'

result = Atrshmlog::attach()

Atrshmlog::flush()

maxindex = Atrshmlog::get_statistics_max_index()

stats = Atrshmlog::get_statistics()

print "flush : #{stats[80]} : \n"

print " \n"

exit (0)

# end of main
