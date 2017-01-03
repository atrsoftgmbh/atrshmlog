#!/usr/bin/ruby
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

require 'atrshmlog'

Atrshmlog::set_env_prefix("OTTO")

prefix = Atrshmlog::get_env_prefix()

print "env prefix : #{prefix} : \n"

Atrshmlog::set_env_prefix("HUGO")

prefix = Atrshmlog::get_env_prefix()

print "env prefix : #{prefix} : \n"



print " \n"

exit (0)

# end of main
