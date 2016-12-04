#!/usr/bin/python3
#
# Id:
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

import sys
import atrshmlog

result = atrshmlog.attach()

tl = atrshmlog.get_thread_locals_adress()

print('thread local : ' + str(tl) + ' : ')

tid = atrshmlog.get_tid()

print('thread local : ' + str(tid) + ' : ')

tltid = atrshmlog.get_thread_local_tid(tl)

print('thread local : ' + str(tltid) + ' : ')

atrshmlog.write(1, atrshmlog.EVENT_POINT_IN_TIME_C, 42, 0, 0, "hallo")

tltid = atrshmlog.get_thread_local_tid(tl)

print('thread local : ' + str(tltid) + ' : ')


atrshmlog.turn_logging_off(tl)

print('thread local : ')


print (' ')

exit(0)

# end of test

