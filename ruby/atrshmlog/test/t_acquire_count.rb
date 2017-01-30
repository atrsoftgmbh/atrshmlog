#!/usr/bin/ruby
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

require 'atrshmlog'

r = Atrshmlog::attach()

count = Atrshmlog::get_acquire_count()

print "acquire count : #{count} : \n"

count = Atrshmlog::get_buffer_id()

print "acquire count : #{count} : \n"


exit(0)

# end of test

