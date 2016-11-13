
perl -n -e 'BEGIN { $total = 0 ; } @F = split(/[\s]+/); $total += $F[0] * $F[1]; END { print $total . "\n" }' $1

