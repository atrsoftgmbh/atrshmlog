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

logging = atrshmlog.get_logging()

print('logging : ' + str(logging) + ' : ')

oldlogging = atrshmlog.set_logging_process_off()

print('logging : ' + str(oldlogging) + ' : ')

logging = atrshmlog.get_logging()

print('logging : ' + str(logging) + ' : ')

oldlogging = atrshmlog.set_logging_process_on()

print('logging : ' + str(oldlogging) + ' : ')

logging = atrshmlog.get_logging()

print('logging : ' + str(logging) + ' : ')

area = atrshmlog.get_area()

old = atrshmlog.set_area_ich_habe_fertig(area, 1)

print('logging : ' + str(old) + ' : ')

logging = atrshmlog.get_logging()

print('logging : ' + str(logging) + ' : ')

old = atrshmlog.set_area_ich_habe_fertig(area, 0)

print('logging : ' + str(old) + ' : ')

logging = atrshmlog.get_logging()

print('logging : ' + str(logging) + ' : ')

oldlogging = atrshmlog.set_logging_process_off_final()

print('logging : ' + str(oldlogging) + ' : ')

logging = atrshmlog.get_logging()

print('logging : ' + str(logging) + ' : ')


print (' ')

exit(0)

# end of test

