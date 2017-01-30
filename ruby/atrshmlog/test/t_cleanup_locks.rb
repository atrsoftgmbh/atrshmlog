#!/usr/bin/ruby
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

require 'atrshmlog'


result = Atrshmlog::attach()

print "cleanup locks : #{result} : \n"

area = Atrshmlog::get_area()

print "cleanup locks : #{area} : \n"

Atrshmlog::cleanup_locks(area)

print "cleanup locks \n"

print " \n"

exit (0)

# end of main
