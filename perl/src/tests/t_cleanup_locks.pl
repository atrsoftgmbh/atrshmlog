#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

$result = Atrshmlog::attach();

print "cleanup locks : $result : \n";

$area = Atrshmlog::get_area();

print "cleanup locks : $area : \n";

Atrshmlog::cleanup_locks($area);

print "cleanup locks \n";

print " \n";

exit (0);

# end of main
