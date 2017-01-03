#!/usr/bin/ruby
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

require 'atrshmlog'

result = Atrshmlog::attach()

Atrshmlog::sleep_nanos(1000000000)

akcount = 0

slave = 0

slave = Atrshmlog::get_next_slave_local(slave)

while slave != 0
    akcount = akcount + 1

    slave = Atrshmlog::get_next_slave_local(slave)
end

print "create slave : #{akcount} : \n"

s = Atrshmlog::create_slave()

Atrshmlog::sleep_nanos(1000000000)

print "create slave : #{s} : \n"

akcount = 0

slave = 0

slave = Atrshmlog::get_next_slave_local(slave)

while slave != 0
    akcount = akcount + 1

    slave = Atrshmlog::get_next_slave_local(slave)
end

print "create slave : #{akcount} : \n"


print " \n"

exit (0)

# end of main
