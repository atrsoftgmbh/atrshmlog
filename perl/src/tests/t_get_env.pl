#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

$result = Atrshmlog::attach();

$s = shift @ARGV;

$val = Atrshmlog::get_env($s);

print "get env : $val : \n";

$val = Atrshmlog::get_env_shmid();

print "get env : $val : \n";

$val = Atrshmlog::get_env_id_suffix();

print "get env : $val : \n";


print " \n";

exit (0);

# end of main
