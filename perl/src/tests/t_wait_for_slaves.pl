#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

$result = Atrshmlog::attach();

$flag = Atrshmlog::get_wait_for_slaves();

print "wait for slaves  : $flag : \n";

$old = Atrshmlog::set_wait_for_slaves_on();

print "wait for slaves  : $old : \n";

$flag = Atrshmlog::get_wait_for_slaves();

print "wait for slaves  : $flag : \n";

$old = Atrshmlog::set_wait_for_slaves_off();

print "wait for slaves  : $old : \n";

$flag = Atrshmlog::get_wait_for_slaves();

print "wait for slaves  : $flag : \n";

$old = Atrshmlog::set_wait_for_slaves_on();

print "wait for slaves  : $old : \n";

$flag = Atrshmlog::get_wait_for_slaves();

print "wait for slaves  : $flag : \n";


print " \n";

exit (0);

# end of main
