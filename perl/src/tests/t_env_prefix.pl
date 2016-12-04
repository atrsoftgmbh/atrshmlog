#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

Atrshmlog::set_env_prefix("OTTO");

$prefix = Atrshmlog::get_env_prefix();

print "env prefix : $prefix : \n";

Atrshmlog::set_env_prefix("HUGO");

$prefix = Atrshmlog::get_env_prefix();

print "env prefix : $prefix : \n";



print " \n";

exit (0);

# end of main
