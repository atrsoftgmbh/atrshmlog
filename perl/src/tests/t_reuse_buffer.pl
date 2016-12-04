#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

$result = Atrshmlog::attach();

Atrshmlog::write(1, $Atrshmlog::EVENT_POINT_IN_TIME_C, 42, 0, 0, "hallo anton");

$tid = Atrshmlog::get_tid();

print "reuse buffer : $tid : \n";

$count = Atrshmlog::reuse_thread_buffers($tid);

print "reuse buffer : $count : \n";

 
print " \n";

exit (0);

# end of main
