#!/usr/bin/python3
#
# Id:
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

import sys
import atrshmlog

maxindex = atrshmlog.get_event_locks_max()

print('event locks max : ' + str(maxindex) + ' : ')

newindex  = int(sys.argv[1])

print('event locks max : ' + str(newindex) + ' : ')

oldindex = atrshmlog.set_event_locks_max(newindex)

print('event locks max : ' + str(oldindex) + ' : ')

maxindex = atrshmlog.get_event_locks_max()

print('event locks max : ' + str(maxindex) + ' : ')

result = atrshmlog.attach()

oldindex = atrshmlog.set_event_locks_max(newindex + 100)

print('event locks max : ' + str(oldindex) + ' : ')

maxindex = atrshmlog.get_event_locks_max()

print('event locks max : ' + str(maxindex) + ' : ')

print (' ')

exit(0)

# end of test

