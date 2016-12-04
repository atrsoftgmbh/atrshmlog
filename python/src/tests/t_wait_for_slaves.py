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

flag = atrshmlog.get_wait_for_slaves()

print('wait for slaves  : ' + str(flag) + ' : ')

old = atrshmlog.set_wait_for_slaves_on()

print('wait for slaves  : ' + str(old) + ' : ')

flag = atrshmlog.get_wait_for_slaves()

print('wait for slaves  : ' + str(flag) + ' : ')

old = atrshmlog.set_wait_for_slaves_off()

print('wait for slaves  : ' + str(old) + ' : ')

flag = atrshmlog.get_wait_for_slaves()

print('wait for slaves  : ' + str(flag) + ' : ')

old = atrshmlog.set_wait_for_slaves_on()

print('wait for slaves  : ' + str(old) + ' : ')

flag = atrshmlog.get_wait_for_slaves()

print('wait for slaves  : ' + str(flag) + ' : ')



print (' ')

exit(0)

# end of test

