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

print "area : #{area} : \n"

ihf = Atrshmlog::get_area_ich_habe_fertig(area)

print "area : #{ihf} : \n"

oldihf = Atrshmlog::set_area_ich_habe_fertig(area, 1)

print "area : #{oldihf} : \n"

ihf = Atrshmlog::get_area_ich_habe_fertig(area)

print "area : #{ihf} : \n"

oldihf = Atrshmlog::set_area_ich_habe_fertig(area, 0)

print "area : #{oldihf} : \n"

ihf = Atrshmlog::get_area_ich_habe_fertig(area)

print "area : #{ihf} : \n"

count  = Atrshmlog::get_area_count(area)

print "area : #{count} : \n"

version = Atrshmlog::get_area_version(area)
 
print "area : #{version} : \n"

print " \n"

exit (0)

# end of main
