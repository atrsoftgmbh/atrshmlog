#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

$result = Atrshmlog::attach();

Atrshmlog::stop();

$maxindex = Atrshmlog::get_statistics_max_index();

@stats = Atrshmlog::get_statistics();

print "stop : $stats[11] : \n";

print " \n";

exit (0);

# end of main
