#!/usr/bin/perl
#
# $Id:$
#
# We make a short testrun with perl

use Atrshmlog;

$ret = Atrshmlog::attach();

print "Attach was $ret \n";

my $area = Atrshmlog::get_area();

my @r = Atrshmlog::read_fetch($area);

if ($r[0] < 0) {
    print "error in read_fetch : " . $r[0]. " \n";
    exit (1);
}

if ($r[0] > 0) {
    print "read_fetch gave no buffer : " . $r[0] . "\n";
    exit(0);
}

# ok. we have a 0 in. now for the rest

foreach my $ret (@r) {
    print "   " . $ret . "\n";
}

exit(0);

# end of file
