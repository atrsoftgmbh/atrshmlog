#!/usr/bin/ruby
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

require 'atrshmlog'

newcount = ARGV[0].to_i

print "slave run off : #{newcount} : \n"

oldcount = Atrshmlog::set_f_list_buffer_slave_count(newcount)

result = Atrshmlog::attach()

Atrshmlog::sleep_nanos(1000000000)

akcount = 0

slave = 0

slave = Atrshmlog::get_next_slave_local(slave)

while slave != 0
    akcount = akcount + 1
    slave = Atrshmlog::get_next_slave_local(slave)
end

print "slave run off : #{akcount} : \n"

Atrshmlog::set_f_list_buffer_slave_run_off()

Atrshmlog::sleep_nanos(1000000000)

akcount = 0

slave = 0

slave = Atrshmlog::get_next_slave_local(slave)

while slave != 0
    akcount = akcount + 1
    slave = Atrshmlog::get_next_slave_local(slave)
end

print "slave run off : #{akcount} : \n"


print " \n"

exit (0)

# end of main
