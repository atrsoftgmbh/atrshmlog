#!/usr/bin/ruby
#
# $Id:$
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

require 'atrshmlog'

result = Atrshmlog::attach()

id = Atrshmlog::get_strategy()

print "strategy : #{id} : \n"

oldid = Atrshmlog::set_strategy(1)

print "strategy : #{oldid} : \n"

id = Atrshmlog::get_strategy()

print "strategy : #{id} : \n"

oldid = Atrshmlog::set_strategy(2)

print "strategy : #{oldid} : \n"

id = Atrshmlog::get_strategy()

print "strategy : #{id} : \n"

id = Atrshmlog::get_strategy_process()

print "strategy : #{id} : \n"

oldid = Atrshmlog::set_strategy_process(4)

print "strategy : #{oldid} : \n"

id = Atrshmlog::get_strategy_process()

print "strategy : #{id} : \n"

oldid = Atrshmlog::set_strategy_process(0)

print "strategy : #{oldid} : \n"

id = Atrshmlog::get_strategy_process()

print "strategy : #{id} : \n"


print " \n"

exit (0)

# end of main
