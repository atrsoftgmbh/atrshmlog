#!/usr/bin/python3
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

import sys
import atrshmlog

id = atrshmlog.get_clock_id()

print('clicktime : ' + str(id) + ' : ')

r = atrshmlog.attach()

id = atrshmlog.get_clock_id()

print('clicktime : ' + str(id) + ' : ')

click = atrshmlog.get_clicktime()

print('clicktime : ' + str(click) + ' : ')




print (' ')

exit(0);

# end of test

