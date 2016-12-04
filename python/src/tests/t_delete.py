#!/usr/bin/python3
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

import sys
import atrshmlog

id = int(sys.argv[1])

print('delete : ' + str(id) + ' : ')

r = atrshmlog.delete(id)

print('delete : ' + str(r) + ' : ')

print (' ')

exit(0);

# end of test

