#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

$result = Atrshmlog::attach();

$logging = Atrshmlog::get_logging();

print "logging : $logging : \n";

$oldlogging = Atrshmlog::set_logging_process_off();

print "logging : $oldlogging : \n";

$logging = Atrshmlog::get_logging();

print "logging : $logging : \n";

$oldlogging = Atrshmlog::set_logging_process_on();

print "logging : $oldlogging : \n";

$logging = Atrshmlog::get_logging();

print "logging : $logging : \n";

$area = Atrshmlog::get_area();

$old = Atrshmlog::set_area_ich_habe_fertig($area, 1);

print "logging : $old : \n";

$logging = Atrshmlog::get_logging();

print "logging : $logging : \n";

$old = Atrshmlog::set_area_ich_habe_fertig($area, 0);

print "logging : $old : \n";

$logging = Atrshmlog::get_logging();

print "logging : $logging : \n";

$oldlogging = Atrshmlog::set_logging_process_off_final();

print "logging : $oldlogging : \n";

$logging = Atrshmlog::get_logging();

print "logging : $logging : \n";

print " \n";

exit (0);

# end of main
