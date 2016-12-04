#!/usr/bin/python3
#
# Id:
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

import sys
import atrshmlog

index = int(sys.argv[1])

result = atrshmlog.attach()

area = atrshmlog.get_area()

r = atrshmlog.read(area, index)

print('read : ' + str(r[0]) + ' : ')

if r[0] == 0 :
    print('read : ' + str(r[2]) + ' : ')
    

print (' ')

exit(0)

# end of test

