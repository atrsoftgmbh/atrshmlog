#!/usr/bin/ruby
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

require 'atrshmlog'

result = Atrshmlog::attach()

area = Atrshmlog::get_area()

r = Atrshmlog::read_fetch(area)

print "read fetch : #{r[0]} : \n"

if r[0] == 0
    print "read fetch : #{r[1]} : #{r[3]} : \n"
end
 
print " \n"

exit (0)

# end of main
