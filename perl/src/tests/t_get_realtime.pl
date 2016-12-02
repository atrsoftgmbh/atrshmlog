#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

$result = Atrshmlog::attach();

($s,$n) = Atrshmlog::get_realtime();

print "get realtime : $s : $n : \n";

print " \n";

exit (0);

# end of main
