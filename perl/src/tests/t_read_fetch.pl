#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

$result = Atrshmlog::attach();

$area = Atrshmlog::get_area();

@r = Atrshmlog::read_fetch($area);

print "read fetch : $r[0] : \n";

if ($r[0] == 0) {
    print "read fetch : $r[1] : $r[3] : \n";
}
 
print " \n";

exit (0);

# end of main
