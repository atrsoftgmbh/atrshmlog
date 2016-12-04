#!/usr/bin/python3
#
# Id:
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

import sys
import atrshmlog

count = atrshmlog.get_f_list_buffer_slave_count()

print('slave count : ' + str(count) + ' : ')

newcount = int(sys.argv[1])

print('slave count : ' + str(newcount) + ' : ')

oldcount = atrshmlog.set_f_list_buffer_slave_count(newcount)

print('slave count : ' + str(oldcount) + ' : ')

count = atrshmlog.get_f_list_buffer_slave_count()

print('slave count : ' + str(count) + ' : ')

result = atrshmlog.attach()

atrshmlog.sleep_nanos(1000000000)

akcount = 0

slave = atrshmlog.get_next_slave_local(0)

while slave != 0:
    akcount = akcount + 1
    slave = atrshmlog.get_next_slave_local(slave)

print('slave count : ' + str(akcount) + ' : ')



print (' ')

exit(0)

# end of test

