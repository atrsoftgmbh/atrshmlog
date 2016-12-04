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

atrshmlog.flush()

maxindex = atrshmlog.get_statistics_max_index()

stats = atrshmlog.get_statistics()

print('flush : ' + str(stats[80]) + ' : ')

print (' ')

exit(0)

# end of test

