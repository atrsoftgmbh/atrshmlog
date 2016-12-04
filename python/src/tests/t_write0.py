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

ret = atrshmlog.write0(1, atrshmlog.EVENT_POINT_IN_TIME_C, 42, 0, 0)

print('write0 : ' + str(ret) + ' : ')

print (' ')

exit(0)

# end of test

