#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

$result = Atrshmlog::attach();

$id = Atrshmlog::get_autoflush();

print "autoflush : $id : \n";

$oldid = Atrshmlog::set_autoflush(1);

print "autoflush : $oldid : \n";

$id = Atrshmlog::get_autoflush();

print "autoflush : $id : \n";

$oldid = Atrshmlog::set_autoflush(0);

print "autoflush : $oldid : \n";

$id = Atrshmlog::get_autoflush();

print "autoflush : $id : \n";

$id = Atrshmlog::get_autoflush_process();

print "autoflush : $id : \n";

$oldid = Atrshmlog::set_autoflush_process(1);

print "autoflush : $oldid : \n";

$id = Atrshmlog::get_autoflush_process();

print "autoflush : $id : \n";

$oldid = Atrshmlog::set_autoflush_process(0);

print "autoflush : $oldid : \n";

$id = Atrshmlog::get_autoflush_process();

print "autoflush : $id : \n";


print " \n";

exit (0);

# end of main
