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

size = atrshmlog.get_buffer_max_size()

print('buffer max size : ' + str(size) + ' : ')


print (' ')

exit(0);

# end of test

