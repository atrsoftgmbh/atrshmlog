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

id = atrshmlog.get_strategy()

print('strategy : ' + str(id) + ' : ')

oldid = atrshmlog.set_strategy(1)

print('strategy : ' + str(oldid) + ' : ')

id = atrshmlog.get_strategy()

print('strategy : ' + str(id) + ' : ')

oldid = atrshmlog.set_strategy(2)

print('strategy : ' + str(oldid) + ' : ')

id = atrshmlog.get_strategy()

print('strategy : ' + str(id) + ' : ')

id = atrshmlog.get_strategy_process()

print('strategy : ' + str(id) + ' : ')

oldid = atrshmlog.set_strategy_process(4)

print('strategy : ' + str(oldid) + ' : ')

id = atrshmlog.get_strategy_process()

print('strategy : ' + str(id) + ' : ')

oldid = atrshmlog.set_strategy_process(0)

print('strategy : ' + str(oldid) + ' : ')

id = atrshmlog.get_strategy_process()

print('strategy : ' + str(id) + ' : ')

print (' ')

exit(0)

# end of test

