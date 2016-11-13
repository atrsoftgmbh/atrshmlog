
perl -n -e 'BEGIN { %s =(); } @F = split(/[\s]+/);  $s{$F[6]} += 1; END { foreach my $k (sort { $a <=> $b } keys %s) { print $k . " " . $s{$k} . "\n"; } } ' $1

