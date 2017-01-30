#!/usr/bin/ruby
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

require 'atrshmlog'

result = Atrshmlog::attach()

count = Atrshmlog::get_prealloc_buffer_count()

print "prealloc count : #{count} : \n"

oldid = Atrshmlog::set_prealloc_buffer_count(42)

print "prealloc count : #{oldid} : \n"

count = Atrshmlog::get_prealloc_buffer_count()

print "prealloc count : #{count} : \n"

oldid = Atrshmlog::set_prealloc_buffer_count(128)

print "prealloc count : #{oldid} : \n"

count = Atrshmlog::get_prealloc_buffer_count()

print "prealloc count : #{count} : \n"


print " \n"

exit (0)

# end of main
