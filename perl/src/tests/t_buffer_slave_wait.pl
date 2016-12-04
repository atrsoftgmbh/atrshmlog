#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

$result = Atrshmlog::attach();

$nanos = Atrshmlog::get_f_list_buffer_slave_wait();

print "buffer slave wait  : $nanos : \n";

$old = Atrshmlog::set_f_list_buffer_slave_wait(250000);

print "buffer slave wait  : $old : \n";

$nanos = Atrshmlog::get_f_list_buffer_slave_wait();

print "buffer slave wait  : $nanos : \n";

print " \n";

exit (0);

# end of main
