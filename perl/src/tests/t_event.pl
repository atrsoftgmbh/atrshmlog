#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

$result = Atrshmlog::attach();

$event = Atrshmlog::get_event(42);

print "event : $event : \n";

$oldevent = Atrshmlog::set_event_off(42);

print "event : $oldevent : \n";

$event = Atrshmlog::get_event(42);

print "event : $event : \n";

$oldevent = Atrshmlog::set_event_on(42);

print "event : $oldevent : \n";

$event = Atrshmlog::get_event(42);

print "event : $event : \n";


print " \n";

exit (0);

# end of main
