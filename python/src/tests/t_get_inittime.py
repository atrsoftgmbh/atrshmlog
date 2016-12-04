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

t = atrshmlog.get_inittime_tsc_before()

print('get inittime : ' + str(t) + ' : ')

t = atrshmlog.get_inittime_tsc_after()

print('get inittime : ' + str(t) + ' : ')

(s,n) = atrshmlog.get_inittime()

print('get inittime : ' + str(s) + ' : ' + str(n) + ' : ')

print (' ')

exit(0)

# end of test

