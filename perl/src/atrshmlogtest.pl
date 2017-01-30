#!/usr/bin/perl
#
# $Id:$
#
# We make a short testrun with perl

use Atrshmlog;

$ret = Atrshmlog::attach();

print "Attach was $ret \n";

$t1 = Atrshmlog::gettime();

print "time 1 is $t1 \n";

$t2 = Atrshmlog::gettime();

print "time 2 is $t2 \n";

$mode = $Atrshmlog::EVENT_INTERVAL_IN_TIME_C ;

$ret = Atrshmlog::write0(1,$mode,42,$t1,$t2);

print "logging write0 was $ret  \n";

$ret = Atrshmlog::write_binary(1,$mode,43,$t1,$t2, "hello, world", length ("hello, world"));

print "logging write_binary was $ret  \n";

$ret = Atrshmlog::write(1,$mode,44,$t1,$t2, "hello, anton world");

print "logging write was $ret  \n";

$ret = Atrshmlog::get_env("_ID");

print "getenv $ret \n";

$area = Atrshmlog::get_area();

print $area . "\n";

($seconds, $nanos) = Atrshmlog::get_realtime();

print "Seconds $seconds Nanos $nanos \n";

my @s =  Atrshmlog::get_statistics();

$index = 0;
foreach my $st (@s) {
    print "atrshmlogstat $index $st \n";
    ++$index;
}
exit(0);

# end of file
