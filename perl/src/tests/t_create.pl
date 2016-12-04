#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

$key = shift @ARGV;

print "create : $key : \n";

$count = shift @ARGV;

print "create : $count : \n";

$id = Atrshmlog::create(4812,$count);

print "create : $id : \n";

if ($id >  0) {
    open(OUT, ">dot.atrshmlog") or die "cannot open for write\n";

    print OUT "ATRSHMLOG_ID=$id\nexport ATRSHMLOG_ID\nATRSHMLOG_COUNT=$count\nexport ATRSHMLOG_COUNT\n";

    close OUT;
}

print " \n";

exit (0);

# end of main
