#!/usr/bin/ruby
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

require 'atrshmlog'

result = Atrshmlog::attach()

size = Atrshmlog::get_buffer_max_size()

print "buffer max size  : #{size} : \n"


print " \n"

exit (0)

# end of main
