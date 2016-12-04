#!/usr/bin/python3
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

import sys
import atrshmlog

r = atrshmlog.attach()

atrshmlog.sleep_nanos(1000000000)

akcount = 0

slave = atrshmlog.get_next_slave_local(0)

while slave != 0:
    akcount = akcount + 1
    slave = atrshmlog.get_next_slave_local(slave)


print('create slave : ' + str(akcount) + ' : ')

s = atrshmlog.create_slave()

atrshmlog.sleep_nanos(1000000000)

print('create slave : ' + str(s) + ' : ')

akcount = 0

slave = atrshmlog.get_next_slave_local(0)

while slave != 0:
    akcount = akcount + 1
    slave = atrshmlog.get_next_slave_local(slave)


print('create slave : ' + str(akcount) + ' : ')


print (' ')

exit(0);

# end of test

