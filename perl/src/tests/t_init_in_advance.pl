#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

$result = Atrshmlog::attach();

$f = Atrshmlog::get_init_buffers_in_advance();

print "init in advance : $f : \n";

$old = Atrshmlog::set_init_buffers_in_advance_on();

print "init in advance : $old : \n";

$f = Atrshmlog::get_init_buffers_in_advance();

print "init in advance : $f : \n";

$old = Atrshmlog::set_init_buffers_in_advance_off();

print "init in advance : $old : \n";

$f = Atrshmlog::get_init_buffers_in_advance();

print "init in advance : $f : \n";

$old = Atrshmlog::set_init_buffers_in_advance_on();

print "init in advance : $old : \n";

$f = Atrshmlog::get_init_buffers_in_advance();

print "init in advance : $f : \n";


print " \n";

exit (0);

# end of main
