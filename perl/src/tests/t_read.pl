#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

$index = shift @ARGV;

$result = Atrshmlog::attach();

$area = Atrshmlog::get_area();

@r = Atrshmlog::read($area, $index);

print "read : $r[0] : \n";

if ($r[0] == 0) {
    print "read : $r[2] : \n";
}
 
print " \n";

exit (0);

# end of main
