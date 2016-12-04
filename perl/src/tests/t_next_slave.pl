#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

$target = shift @ARGV;

$result = Atrshmlog::attach();

Atrshmlog::sleep_nanos(1000000000);

$akcount = 0;

$slave = undef;

$slave = Atrshmlog::get_next_slave_local($slave);

while ($slave) {
    ++ $akcount;
    $slave = Atrshmlog::get_next_slave_local($slave);
}

print "next slave : $akcount : \n";

$s = 0;

for ($i = 0; $i < 8 ; ++$i) {
    $s = Atrshmlog::create_slave();
}

print "next slave : $s : \n";

Atrshmlog::sleep_nanos(1000000000);

$targetslave = undef;

$akcount = 0;

$slave = undef;

$slave = Atrshmlog::get_next_slave_local($slave);

while ($slave) {
    ++ $akcount;

    print "next slave : $akcount : \n";

    print "next slave : $$slave : \n";

    $tid = Atrshmlog::get_slave_tid($slave);
    
    print "next slave : $tid : \n";

    if ($akcount == $target) {
	$targetslave = $slave;
    }
    
    $slave = Atrshmlog::get_next_slave_local($slave);
}

print "next slave : $akcount : \n";

if ($targetslave) {
    Atrshmlog::turn_slave_off($targetslave);

    Atrshmlog::sleep_nanos(1000000000);

    $akcount = 0;

    $slave = undef;

    $slave = Atrshmlog::get_next_slave_local($slave);

    while ($slave) {
	++ $akcount;

	print "next slave : $akcount : \n";

	print "next slave : $$slave : \n";

	$tid = Atrshmlog::get_slave_tid($slave);
    
	print "next slave : $tid : \n";

	$slave = Atrshmlog::get_next_slave_local($slave);
    }
    
}     
     
print " \n";

exit (0);

# end of main
