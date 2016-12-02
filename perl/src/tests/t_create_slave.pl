#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

$result = Atrshmlog::attach();

Atrshmlog::sleep_nanos(1000000000);

$akcount = 0;

$slave = undef;

$slave = Atrshmlog::get_next_slave_local($slave);

while ($slave) {
    ++$akcount;

    $slave = Atrshmlog::get_next_slave_local($slave);
}

print "create slave : $akcount : \n";

$s = Atrshmlog::create_slave();

Atrshmlog::sleep_nanos(1000000000);

print "create slave : $s : \n";

$akcount = 0;

$slave = undef;

$slave = Atrshmlog::get_next_slave_local($slave);

while ($slave) {
    ++$akcount;

    $slave = Atrshmlog::get_next_slave_local($slave);
}

print "create slave : $akcount : \n";


print " \n";

exit (0);

# end of main
