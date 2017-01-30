#!/usr/bin/ruby
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

require 'atrshmlog'

result = Atrshmlog::attach()

count = ARGV[0].to_i

print "init shm log : #{count} : \n"

area = Atrshmlog::get_area()

iret = Atrshmlog::init_shm_log(area, count)

print "init shm log : #{iret} : \n"


print " \n"

exit (0)

# end of main
