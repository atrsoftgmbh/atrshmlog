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

count = atrshmlog.get_prealloc_buffer_count()

print('prealloc count : ' + str(count) + ' : ')

oldid = atrshmlog.set_prealloc_buffer_count(42)

print('prealloc count : ' + str(oldid) + ' : ')

count = atrshmlog.get_prealloc_buffer_count()

print('prealloc count : ' + str(count) + ' : ')

oldid = atrshmlog.set_prealloc_buffer_count(128)

print('prealloc count : ' + str(oldid) + ' : ')

count = atrshmlog.get_prealloc_buffer_count()

print('prealloc count : ' + str(count) + ' : ')


print (' ')

exit(0)

# end of test

