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

atrshmlog.write(1, atrshmlog.EVENT_POINT_IN_TIME_C, 42, 0, 0, 'hallo anton')

tid = atrshmlog.get_tid()

print('reuse buffer : ' + str(tid) + ' : ')

count = atrshmlog.reuse_thread_buffers(tid)

print('reuse buffer : ' + str(count) + ' : ')
    

print (' ')

exit(0)

# end of test

