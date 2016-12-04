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

area = atrshmlog.get_area()

r = atrshmlog.read_fetch(area)

print('read fetch : ' + str(r[0]) + ' : ')

if r[0] == 0 :
    print('read fetch : ' + str(r[1]) + ' : ' + str(r[3]) + ' : ')
    

print (' ')

exit(0)

# end of test

