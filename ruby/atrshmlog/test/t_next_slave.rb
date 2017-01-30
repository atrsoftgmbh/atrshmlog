#!/usr/bin/ruby
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

require 'atrshmlog'

target = ARGV[0].to_i

result = Atrshmlog::attach()

Atrshmlog::sleep_nanos(1000000000)

akcount = 0

slave = 0

slave = Atrshmlog::get_next_slave_local(slave)

while slave != 0
    akcount = akcount + 1
    slave = Atrshmlog::get_next_slave_local(slave)
end

print "next slave : #{akcount} : \n"

s = 0

8.times {      
    s = Atrshmlog::create_slave()
}

print "next slave : #{s} : \n"

Atrshmlog::sleep_nanos(1000000000)

targetslave = -1

akcount = 0

slave = 0

slave = Atrshmlog::get_next_slave_local(slave)

while slave != 0
    akcount = akcount + 1

    print "next slave : #{akcount} : \n"

    print "next slave : #{slave} : \n"

    tid = Atrshmlog::get_slave_tid(slave)
    
    print "next slave : #{tid} : \n"

    if akcount == target
	targetslave = slave
    end
    
    slave = Atrshmlog::get_next_slave_local(slave)
end

print "next slave : #{akcount} : \n"

if targetslave != -1
    Atrshmlog::turn_slave_off(targetslave)

    Atrshmlog::sleep_nanos(1000000000)

    akcount = 0

    slave = 0

    slave = Atrshmlog::get_next_slave_local(slave)

    while slave != 0
	akcount = akcount + 1

	print "next slave : #{akcount} : \n"

	print "next slave : #{slave} : \n"

	tid = Atrshmlog::get_slave_tid(slave)
    
	print "next slave : #{tid} : \n"

	slave = Atrshmlog::get_next_slave_local(slave)
    end
    
end   
     
print " \n"

exit (0)

# end of main
