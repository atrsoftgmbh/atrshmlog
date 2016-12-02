#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

$result = Atrshmlog::attach();

$count = Atrshmlog::get_prealloc_buffer_count();

print "prealloc count : $count : \n";

$oldid = Atrshmlog::set_prealloc_buffer_count(42);

print "prealloc count : $oldid : \n";

$count = Atrshmlog::get_prealloc_buffer_count();

print "prealloc count : $count : \n";

$oldid = Atrshmlog::set_prealloc_buffer_count(128);

print "prealloc count : $oldid : \n";

$count = Atrshmlog::get_prealloc_buffer_count();

print "prealloc count : $count : \n";


print " \n";

exit (0);

# end of main
