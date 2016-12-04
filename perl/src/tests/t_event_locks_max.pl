#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

$maxindex = Atrshmlog::get_event_locks_max();

print "event locks max : $maxindex : \n";

$newindex  = shift @ARGV;

print "event locks max : $newindex : \n";

$oldindex = Atrshmlog::set_event_locks_max($newindex);

print "event locks max : $oldindex : \n";

$maxindex = Atrshmlog::get_event_locks_max();

print "event locks max : $maxindex : \n";

$result = Atrshmlog::attach();

$oldindex = Atrshmlog::set_event_locks_max($newindex + 100);

print "event locks max : $oldindex : \n";

$maxindex = Atrshmlog::get_event_locks_max();

print "event locks max : $maxindex : \n";


print " \n";

exit (0);

# end of main
