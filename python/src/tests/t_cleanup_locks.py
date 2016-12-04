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

print('cleanup locks : ' + str(r) + ' : ')

area = atrshmlog.get_area()

print('cleanup locks : ' + str(area) + ' : ')

atrshmlog.cleanup_locks(area)

print('cleanup locks ')



print (' ')

exit(0);

# end of test

