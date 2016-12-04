#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

$result = Atrshmlog::attach();

$count = shift @ARGV;

print "init shm log : $count : \n";

$area = Atrshmlog::get_area();

$iret = Atrshmlog::init_shm_log($area, $count);

print "init shm log : $iret : \n";


print " \n";

exit (0);

# end of main
