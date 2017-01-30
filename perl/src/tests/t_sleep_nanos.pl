#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

$result = Atrshmlog::attach();

print "sleep nanos  : $result : \n";

for ($n = 1000000; $n ; $n >>= 1 ) {
    $t1 = Atrshmlog::gettime();
    Atrshmlog::sleep_nanos($n);
    $t2 = Atrshmlog::gettime();

    print "sleep nanos  : $n : " . ($t2 - $t1 ) . " : \n";

}


print " \n";

exit (0);

# end of main
