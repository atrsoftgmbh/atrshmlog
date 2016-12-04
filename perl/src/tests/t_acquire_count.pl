#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

$result = Atrshmlog::attach();

$count = Atrshmlog::get_acquire_count();

print "aquire count : $count : \n";

$count = Atrshmlog::get_buffer_id();

print "aquire count : $count : \n";

print " \n";

exit (0);

# end of main
