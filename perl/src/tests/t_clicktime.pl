#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

$id = Atrshmlog::get_clock_id();

print "clicktime : $id : \n";

$result = Atrshmlog::attach();

$id = Atrshmlog::get_clock_id();

print "clicktime : $id : \n";

$click = Atrshmlog::get_clicktime();

print "clicktime : $click : \n";


print " \n";

exit (0);

# end of main
