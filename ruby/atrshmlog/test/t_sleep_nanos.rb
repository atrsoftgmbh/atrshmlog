#!/usr/bin/ruby
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

require 'atrshmlog'

result = Atrshmlog::attach()

print "sleep nanos  : #{result} : \n"


n = 1000000
while n != 0
    t1 = Atrshmlog::gettime()
    Atrshmlog::sleep_nanos(n)
    t2 = Atrshmlog::gettime()

    print "sleep nanos  : #{n} : #{ t2 - t1 } : \n"

    n = n / 2
end


print " \n"

exit (0)

# end of main
