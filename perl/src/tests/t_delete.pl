#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

$id = shift @ARGV;

print "delete : $id : \n";

$result = Atrshmlog::delete($id);

print "delete : $result : \n";


print " \n";

exit (0);

# end of main
