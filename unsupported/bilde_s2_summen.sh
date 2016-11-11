
perl -n -e 'BEGIN { %s =(); $o5 = 0; } @F = split(/[\s]+/);  $s{$F[4] - $o5} += 1; $o5 = $F[5]; END { foreach my $k (sort { $a <=> $b } keys %s) { print $k . " " . $s{$k} . "\n"; } } ' $1

