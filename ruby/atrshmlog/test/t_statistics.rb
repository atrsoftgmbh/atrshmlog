#!/usr/bin/ruby
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

require 'atrshmlog'

result = Atrshmlog::attach()

maxindex = Atrshmlog::get_statistics_max_index()

print "statistics : #{maxindex} : \n"

stats = Atrshmlog::get_statistics()

print "statistics : #{stats.length} : \n"

index = 0

if stats.length > 0
    stats.each do |s|
	print "statistics : #{index} : #{s} : \n"
	index = index + 1
    end
end

print " \n"

exit (0)

# end of main
