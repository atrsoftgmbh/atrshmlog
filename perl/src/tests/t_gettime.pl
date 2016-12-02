#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

$result = Atrshmlog::attach();

$t = Atrshmlog::gettime(1);

print "gettime : $t : \n";

print " \n";

exit (0);

# end of main
