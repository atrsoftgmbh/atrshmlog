#!/usr/bin/ruby
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

require 'atrshmlog'


result = Atrshmlog::attach()

s = ARGV[0]

val = Atrshmlog::get_env(s)

print "get env : #{val} : \n"

val = Atrshmlog::get_env_shmid()

print "get env : #{val} : \n"

val = Atrshmlog::get_env_id_suffix()

print "get env : #{val} : \n"


print " \n"

exit (0)

# end of main
