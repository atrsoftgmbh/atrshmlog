#!/usr/bin/python3
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

import sys
import atrshmlog

atrshmlog.set_env_prefix('OTTO')

prefix = atrshmlog.get_env_prefix()

print('env prefix : ' + str(prefix) + ' : ')

atrshmlog.set_env_prefix('HUGO')

prefix = atrshmlog.get_env_prefix()

print('env prefix : ' + str(prefix) + ' : ')

print (' ')

exit(0);

# end of test

