#!/usr/bin/ruby
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

require 'atrshmlog'

result = Atrshmlog::attach()

f = Atrshmlog::get_init_buffers_in_advance()

print "init in advance : #{f} : \n"

old = Atrshmlog::set_init_buffers_in_advance_on()

print "init in advance : #{old} : \n"

f = Atrshmlog::get_init_buffers_in_advance()

print "init in advance : #{f} : \n"

old = Atrshmlog::set_init_buffers_in_advance_off()

print "init in advance : #{old} : \n"

f = Atrshmlog::get_init_buffers_in_advance()

print "init in advance : #{f} : \n"

old = Atrshmlog::set_init_buffers_in_advance_on()

print "init in advance : #{old} : \n"

f = Atrshmlog::get_init_buffers_in_advance()

print "init in advance : #{f} : \n"


print " \n"

exit (0)

# end of main
