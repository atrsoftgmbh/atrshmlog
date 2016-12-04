#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

$result = Atrshmlog::attach();

$t = Atrshmlog::get_inittime_tsc_before();

print "get inittime : $t : \n";

$t = Atrshmlog::get_inittime_tsc_after();

print "get inittime : $t : \n";

($s,$n) = Atrshmlog::get_inittime();

print "get inittime : $s : $n : \n";

print " \n";

exit (0);

# end of main
