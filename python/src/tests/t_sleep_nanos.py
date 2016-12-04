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

n = 1000000

while n > 0:
    t1 = atrshmlog.gettime()
    atrshmlog.sleep_nanos(n)
    t2 = atrshmlog.gettime()

    print('sleep nanos : ' + str(n) + ' : ' +  str(t2 - t1) + ' : ')
    n = int(n / 2)

print (' ')

exit(0)

# end of test

