#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

$result = Atrshmlog::attach();

$maxindex = Atrshmlog::get_statistics_max_index();

print "statistics : $maxindex : \n";

@stats = Atrshmlog::get_statistics();

print "statistics : $#stats : \n";

$index = 0;
if ($#stats > 0) {
    foreach $s (@stats) {
	print "statistics : $index : $s : \n";
	++$index;
    }
}

print " \n";

exit (0);

# end of main
