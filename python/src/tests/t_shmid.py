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

id = atrshmlog.get_shmid()

print('shmid : ' + str(id) + ' : ')

print (' ')

exit(0)

# end of test

