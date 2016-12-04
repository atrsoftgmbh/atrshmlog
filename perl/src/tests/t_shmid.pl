#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

$result = Atrshmlog::attach();

$id = Atrshmlog::get_shmid();

print "shmid : $id : \n";

 
print " \n";

exit (0);

# end of main
