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

print "decrement slave count : $akcount : \n";

$s = Atrshmlog::create_slave();

Atrshmlog::sleep_nanos(1000000000);

print "decrement slave count : $s : \n";

$akcount = 0;

$slave = undef;

$slave = Atrshmlog::get_next_slave_local($slave);

while ($slave) {
    ++$akcount;

    $slave = Atrshmlog::get_next_slave_local($slave);
}

print "decrement slave count : $akcount : \n";

$r = Atrshmlog::decrement_slave_count();

print "decrement slave count : $r : \n";

while ($r >  0) {
    $r = Atrshmlog::decrement_slave_count();
    print "decrement slave count : $r : \n";
}

$r = Atrshmlog::decrement_slave_count();

print "decrement slave count : $r : \n";

print " \n";

exit (0);

# end of main
