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

print "decrement slave count : #{akcount} : \n"

s = Atrshmlog::create_slave()

Atrshmlog::sleep_nanos(1000000000)

print "decrement slave count : #{s} : \n"

akcount = 0

slave = 0

slave = Atrshmlog::get_next_slave_local(slave)

while slave != 0
    akcount = akcount + 1

    slave = Atrshmlog::get_next_slave_local(slave)
end

print "decrement slave count : #{akcount} : \n"

r = Atrshmlog::decrement_slave_count()

print "decrement slave count : #{r} : \n"

while r >  0
    r = Atrshmlog::decrement_slave_count()
    print "decrement slave count : #{r} : \n"
end

r = Atrshmlog::decrement_slave_count()

print "decrement slave count : #{r} : \n"

print " \n"

exit (0)

# end of main
