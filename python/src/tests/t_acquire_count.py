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

count = atrshmlog.get_acquire_count()

print('acquire count : ' + str(count) + ' : ')

count = atrshmlog.get_buffer_id()

print('acquire count : ' + str(count) + ' : ')


exit(0);

# end of test

