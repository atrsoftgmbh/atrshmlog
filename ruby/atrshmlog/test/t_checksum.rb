#!/usr/bin/ruby
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

require 'atrshmlog'


result = Atrshmlog::attach()

id = Atrshmlog::get_checksum()

print "checksum  : #{id} : \n"

old = Atrshmlog::set_checksum(1)

print "checksum  : #{old} : \n"

id = Atrshmlog::get_checksum()

print "checksum  : #{id} : \n"

old = Atrshmlog::set_checksum(0)

print "checksum  : #{old} : \n"

id = Atrshmlog::get_checksum()

print "checksum  : #{id} : \n"

print " \n"

exit (0)

# end of main
