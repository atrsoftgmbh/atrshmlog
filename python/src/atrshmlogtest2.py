#!/usr/bin/python
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

import sys
import atrshmlog

r = atrshmlog.attach()

if r == 0:
    t1 = atrshmlog.gettime()

    greating = sys.argv[1]

    t2 = atrshmlog.gettime()

    r = atrshmlog.write(1,atrshmlog.EVENT_INTERVAL_IN_TIME_C, 42, t1, t2, greating)
    atrshmlog.stop()

exit();

# end of test

