#!/usr/bin/ruby
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

require 'atrshmlog'

index = ARGV[0].to_i

result = Atrshmlog::attach()

area = Atrshmlog::get_area()

r = Atrshmlog::read(area, index)

print "read : #{r[0]} : \n"

if r[0] == 0
    print "read : #{r[2]} : \n"
end
 
print " \n"

exit (0)

# end of main
