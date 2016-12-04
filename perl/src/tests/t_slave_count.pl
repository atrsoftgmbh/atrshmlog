#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

$count = Atrshmlog::get_f_list_buffer_slave_count();

print "slave count : $count : \n";

$newcount = shift @ARGV;

print "slave count : $newcount : \n";

$oldcount = Atrshmlog::set_f_list_buffer_slave_count($newcount);

print "slave count : $oldcount : \n";

$count = Atrshmlog::get_f_list_buffer_slave_count();

print "slave count : $count : \n";

$result = Atrshmlog::attach();

Atrshmlog::sleep_nanos(1000000000);

$akcount = 0;

$slave = undef;

$slave = Atrshmlog::get_next_slave_local($slave);

while ($slave) {
    ++ $akcount;
    $slave = Atrshmlog::get_next_slave_local($slave);
}

print "slave count : $akcount : \n";

print " \n";

exit (0);

# end of main
