#!/usr/bin/ruby
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

require 'atrshmlog'

key = ARGV[0].to_i

print "create : #{key} : \n"

count = ARGV[1].to_i

print "create : #{count} : \n"

id = Atrshmlog::create(key,count)

print "create : #{id} : \n"

if id >  0
  open("dot.atrshmlog", "w") do |out|
    out.print "ATRSHMLOG_ID=#{id}\nexport ATRSHMLOG_ID\nATRSHMLOG_COUNT=#{count}\nexport ATRSHMLOG_COUNT\n"
  end
end

print " \n"

exit (0)

# end of main
