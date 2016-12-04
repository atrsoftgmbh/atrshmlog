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

maxindex = atrshmlog.get_statistics_max_index()

print('statistics : ' + str(maxindex) + ' : ')

stats = atrshmlog.get_statistics()

for i in range(0, maxindex) :
    print('statistics : ' + str(i) + ' : ' + str(stats[i]) + ' : ')

print (' ')

exit(0)

# end of test

