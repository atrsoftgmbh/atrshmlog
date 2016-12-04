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

id = atrshmlog.get_clock_id()

print('clock id : ' + str(id) + ' : ')

oldid = atrshmlog.set_clock_id(2)

print('clock id : ' + str(oldid) + ' : ')

id = atrshmlog.get_clock_id()

print('clock id : ' + str(id) + ' : ')

oldid = atrshmlog.set_clock_id(1)

print('clock id : ' + str(oldid) + ' : ')

id = atrshmlog.get_clock_id()

print('clock id : ' + str(id) + ' : ')



print (' ')

exit(0);

# end of test

