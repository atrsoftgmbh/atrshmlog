#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

$result = Atrshmlog::attach();

$size = Atrshmlog::get_buffer_max_size();

print "buffer max size  : $size : \n";


print " \n";

exit (0);

# end of main
