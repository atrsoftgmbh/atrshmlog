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

size = atrshmlog.get_buffer_max_size()

print('buffer size : ' + str(size) + ' : ')

aksize = atrshmlog.get_buffer_size()

print('buffer size  : ' + str(aksize) + ' : ')

oldsize = atrshmlog.set_buffer_size(size * 2)

print('buffer size  : ' + str(oldsize) + ' : ')

aksize = atrshmlog.get_buffer_size()

print('buffer size  : ' + str(aksize) + ' : ')

aksize2 = int(aksize / 2)

oldsize = atrshmlog.set_buffer_size(aksize2)

print('buffer size  : ' + str(oldsize) + ' : ')

aksize = atrshmlog.get_buffer_size()

print('buffer size  : ' + str(aksize) + ' : ')

oldsize = atrshmlog.set_buffer_size(size)

print('buffer size  : ' + str(oldsize) + ' : ')

aksize = atrshmlog.get_buffer_size()

print('buffer size  : ' + str(aksize) + ' : ')


print (' ')

exit(0);

# end of test

