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

print('attach : ' + str(r) + ' : ')

id = atrshmlog.get_autoflush()

print('autoflush : ' + str(id) + ' : ')

oldid = atrshmlog.set_autoflush(1)

print('autoflush : ' + str(oldid) + ' : ')

id = atrshmlog.get_autoflush()

print('autoflush : ' + str(id) + ' : ')

oldid = atrshmlog.set_autoflush(0)

print('autoflush : ' + str(oldid) + ' : ')

id = atrshmlog.get_autoflush()

print('autoflush : ' + str(id) + ' : ')

id = atrshmlog.get_autoflush_process()

print('autoflush : ' + str(id) + ' : ')

oldid = atrshmlog.set_autoflush_process(1)

print('autoflush : ' + str(oldid) + ' : ')

id = atrshmlog.get_autoflush_process()

print('autoflush : ' + str(id) + ' : ')

oldid = atrshmlog.set_autoflush_process(0)

print('autoflush : ' + str(oldid) + ' : ')

id = atrshmlog.get_autoflush_process()

print('autoflush : ' + str(id) + ' : ')



print (' ')

exit(0);

# end of test

