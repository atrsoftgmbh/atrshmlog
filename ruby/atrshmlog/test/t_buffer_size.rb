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

print "buffer size  : #{size} : \n"

aksize = Atrshmlog::get_buffer_size()

print "buffer size  : #{aksize} : \n"

oldsize = Atrshmlog::set_buffer_size(size * 2)

print "buffer size  : #{oldsize} : \n"

aksize = Atrshmlog::get_buffer_size()

print "buffer size  : #{aksize} : \n"

oldsize = Atrshmlog::set_buffer_size(aksize / 2)

print "buffer size  : #{oldsize} : \n"

aksize = Atrshmlog::get_buffer_size()

print "buffer size  : #{aksize} : \n"

oldsize = Atrshmlog::set_buffer_size(size)

print "buffer size  : #{oldsize} : \n"

aksize = Atrshmlog::get_buffer_size()

print "buffer size  : #{aksize} : \n"


print " \n"

exit (0)

# end of main
