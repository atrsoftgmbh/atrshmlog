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

version = atrshmlog.get_version()

minorversion = atrshmlog.get_minor_version()

patchversion = atrshmlog.get_patch_version()

print('version : ' + str(version) + ' : ')
print('version : ' + str(minorversion) + ' : ')
print('version : ' + str(patchversion) + ' : ')


print (' ')

exit(0)

# end of test

