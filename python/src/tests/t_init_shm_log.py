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

count = int(sys.argv[1])

print('init shm log : ' + str(count) + ' : ')

area = atrshmlog.get_area()

iret = atrshmlog.init_shm_log(area, count)

print('init shm log : ' + str(iret) + ' : ')

print (' ')

exit(0)

# end of test

