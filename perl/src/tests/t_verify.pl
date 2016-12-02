#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

$result = Atrshmlog::attach();

$ret = Atrshmlog::verify();

print "verify : $ret : \n";

print " \n";

exit (0);

# end of main
