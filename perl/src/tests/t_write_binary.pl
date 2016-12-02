#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

$result = Atrshmlog::attach();

$ret = Atrshmlog::write_binary(1, $Atrshmlog::EVENT_POINT_IN_TIME_C, 42, 0, 0, "hallo welt", 5);

print "write binary : $ret : \n";

print " \n";

exit (0);

# end of main
