#!/usr/bin/ruby
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

require 'atrshmlog'

result = Atrshmlog::attach()

flag = Atrshmlog::get_wait_for_slaves()

print "wait for slaves  : #{flag} : \n"

old = Atrshmlog::set_wait_for_slaves_on()

print "wait for slaves  : #{old} : \n"

flag = Atrshmlog::get_wait_for_slaves()

print "wait for slaves  : #{flag} : \n"

old = Atrshmlog::set_wait_for_slaves_off()

print "wait for slaves  : #{old} : \n"

flag = Atrshmlog::get_wait_for_slaves()

print "wait for slaves  : #{flag} : \n"

old = Atrshmlog::set_wait_for_slaves_on()

print "wait for slaves  : #{old} : \n"

flag = Atrshmlog::get_wait_for_slaves()

print "wait for slaves  : #{flag} : \n"


print " \n"

exit (0)

# end of main
