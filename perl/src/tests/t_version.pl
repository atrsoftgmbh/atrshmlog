#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

$result = Atrshmlog::attach();

$version = Atrshmlog::get_version();

$minorversion = Atrshmlog::get_minor_version();

$patchversion = Atrshmlog::get_patch_version();

print "version : $version : \n";
print "version : $minorversion : \n";
print "version : $patchversion : \n";

print " \n";

exit (0);

# end of main
