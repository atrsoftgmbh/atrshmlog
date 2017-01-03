#!/usr/bin/ruby
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

require 'atrshmlog'

result = Atrshmlog::attach()

t = Atrshmlog::get_inittime_tsc_before()

print "get inittime : #{t} : \n"

t = Atrshmlog::get_inittime_tsc_after()

print "get inittime : #{t} : \n"

(s,n) = Atrshmlog::get_inittime()

print "get inittime : #{s} : #{n} : \n";

print " \n";

exit (0);

# end of main
