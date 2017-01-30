#!/usr/bin/ruby
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

require 'atrshmlog'

id = ARGV[0].to_i

print "delete : #{id} : \n"

result = Atrshmlog::delete(id)

print "delete : #{result} : \n"


print " \n"

exit (0)

# end of main
