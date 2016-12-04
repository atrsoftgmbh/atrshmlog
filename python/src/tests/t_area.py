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

area = atrshmlog.get_area()

print('area : ' + str(area) + ' : ')

ihf = atrshmlog.get_area_ich_habe_fertig(area)

print('area : ' + str(ihf) + ' : ')

oldihf = atrshmlog.set_area_ich_habe_fertig(area, 1)

print('area : ' + str(oldihf) + ' : ')

ihf = atrshmlog.get_area_ich_habe_fertig(area)

print('area : ' + str(ihf) + ' : ')

oldihf = atrshmlog.set_area_ich_habe_fertig(area, 0)

print('area : ' + str(oldihf) + ' : ')

ihf = atrshmlog.get_area_ich_habe_fertig(area)

print('area : ' + str(ihf) + ' : ')

count = atrshmlog.get_area_count(area)

print('area : ' + str(count) + ' : ')

version = atrshmlog.get_area_version(area)

print('area : ' + str(version) + ' : ')

print (' ')

exit(0);

# end of test

