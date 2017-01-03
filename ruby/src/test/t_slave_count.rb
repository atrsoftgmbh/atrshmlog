#!/usr/bin/ruby
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

require 'atrshmlog'

count = Atrshmlog::get_f_list_buffer_slave_count()

print "slave count : #{count} : \n"

newcount = ARGV[0].to_i

print "slave count : #{newcount} : \n"

oldcount = Atrshmlog::set_f_list_buffer_slave_count(newcount)

print "slave count : #{oldcount} : \n"

count = Atrshmlog::get_f_list_buffer_slave_count()

print "slave count : #{count} : \n"

result = Atrshmlog::attach()

Atrshmlog::sleep_nanos(1000000000)

akcount = 0

slave = 0

slave = Atrshmlog::get_next_slave_local(slave)

while slave != 0
    akcount = akcount + 1
    slave = Atrshmlog::get_next_slave_local(slave)
end

print "slave count : #{akcount} : \n"

print " \n"

exit (0)

# end of main
