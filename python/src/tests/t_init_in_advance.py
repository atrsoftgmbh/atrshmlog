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

f = atrshmlog.get_init_buffers_in_advance()

print('init in advance : ' + str(f) + ' : ')

old = atrshmlog.set_init_buffers_in_advance_on()

print('init in advance : ' + str(old) + ' : ')

f = atrshmlog.get_init_buffers_in_advance()

print('init in advance : ' + str(f) + ' : ')

old = atrshmlog.set_init_buffers_in_advance_off()

print('init in advance : ' + str(old) + ' : ')

f = atrshmlog.get_init_buffers_in_advance()

print('init in advance : ' + str(f) + ' : ')

old = atrshmlog.set_init_buffers_in_advance_on()

print('init in advance : ' + str(old) + ' : ')

f = atrshmlog.get_init_buffers_in_advance()

print('init in advance : ' + str(f) + ' : ')


print (' ')

exit(0)

# end of test

