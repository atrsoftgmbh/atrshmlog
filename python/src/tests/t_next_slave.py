#!/usr/bin/python3
#
# Id:
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

import sys
import atrshmlog

target = int(sys.argv[1])

result = atrshmlog.attach()

atrshmlog.sleep_nanos(1000000000)

akcount = 0

slave = atrshmlog.get_next_slave_local(0)

while slave != 0:
    akcount = akcount + 1
    slave = atrshmlog.get_next_slave_local(slave)

print('next slave : ' + str(akcount) + ' : ')

s = 0

for i in range (0, 8) :
    s = atrshmlog.create_slave()


print('next slave : ' + str(s) + ' : ')

atrshmlog.sleep_nanos(1000000000)

akcount = 0

slave = atrshmlog.get_next_slave_local(0)

while  slave != 0 :
    akcount = akcount + 1
    print('next slave : ' + str(akcount) + ' : ')
    print('next slave : ' + str(slave) + ' : ')
    
    tid = atrshmlog.get_slave_tid(slave)
    
    print('next slave : ' + str(tid) + ' : ')
    
    if akcount == target :
        targetslave = slave
        
    slave = atrshmlog.get_next_slave_local(slave)


print('next slave : ' + str(akcount) + ' : ')

if targetslave != 0 :
    atrshmlog.turn_slave_off(targetslave)

    atrshmlog.sleep_nanos(1000000000)

akcount = 0

slave = atrshmlog.get_next_slave_local(0)

while  slave != 0 :
    akcount = akcount + 1
    print('next slave : ' + str(akcount) + ' : ')
    print('next slave : ' + str(slave) + ' : ')
    
    tid = atrshmlog.get_slave_tid(slave)
    
    print('next slave : ' + str(tid) + ' : ')
    
    slave = atrshmlog.get_next_slave_local(slave)



print (' ')

exit(0)

# end of test

