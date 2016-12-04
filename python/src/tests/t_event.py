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

event = atrshmlog.get_event(42)

print('event : ' + str(event) + ' : ')

oldevent = atrshmlog.set_event_off(42)

print('event : ' + str(oldevent) + ' : ')

event = atrshmlog.get_event(42)

print('event : ' + str(event) + ' : ')

oldevent = atrshmlog.set_event_on(42)

print('event : ' + str(oldevent) + ' : ')

event = atrshmlog.get_event(42)

print('event : ' + str(event) + ' : ')


print (' ')

exit(0)

# end of test

