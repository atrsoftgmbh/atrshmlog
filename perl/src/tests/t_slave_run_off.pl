#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

$newcount = shift @ARGV;

print "slave run off : $newcount : \n";

$oldcount = Atrshmlog::set_f_list_buffer_slave_count($newcount);

$result = Atrshmlog::attach();

Atrshmlog::sleep_nanos(1000000000);

$akcount = 0;

$slave = undef;

$slave = Atrshmlog::get_next_slave_local($slave);

while ($slave) {
    ++ $akcount;
    $slave = Atrshmlog::get_next_slave_local($slave);
}

print "slave run off : $akcount : \n";

Atrshmlog::set_f_list_buffer_slave_run_off();

Atrshmlog::sleep_nanos(1000000000);

$akcount = 0;

$slave = undef;

$slave = Atrshmlog::get_next_slave_local($slave);

while ($slave) {
    ++ $akcount;
    $slave = Atrshmlog::get_next_slave_local($slave);
}

print "slave run off : $akcount : \n";


print " \n";

exit (0);

# end of main
