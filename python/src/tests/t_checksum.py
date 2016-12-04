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

id = atrshmlog.get_checksum()

print('checksum  : ' + str(id) + ' : ')

old = atrshmlog.set_checksum(1)

print('checksum  : ' + str(old) + ' : ')

id = atrshmlog.get_checksum()

print('checksum  : ' + str(id) + ' : ')

old = atrshmlog.set_checksum(0)

print('checksum  : ' + str(old) + ' : ')

id = atrshmlog.get_checksum()

print('checksum  : ' + str(id) + ' : ')


print (' ')

exit(0);

# end of test

