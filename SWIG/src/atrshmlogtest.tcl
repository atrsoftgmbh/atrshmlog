#!/usr/bin/tclsh
#
# $Id:$
#
# We make a short testrun with tcl

load ./Atrshmlog.so atrshmlog

set attach_return  [ atrshmlog::attach ]

puts "Attach was $attach_return "

set t1 [ atrshmlog::gettime ]

puts "time 1 is $t1 "

set t2 [ atrshmlog::gettime ]

puts "time 2 is $t2 "

set mode  $atrshmlog::EVENT_INTERVAL_IN_TIME_C 

set write_return [ atrshmlog::write0 1 $mode 42 $t1 $t2 ]

puts "logging write0 was $write_return "

set write_return [ atrshmlog::write_binary 1 $mode 43 $t1 $t2 "hello, world" 12 ]

puts "logging write_binary was $write_return "

set write_return [ atrshmlog::write 1 $mode 44 $t1 $t2 "hello, anton world" ]

puts "logging write was $write_return "

set shmid [ atrshmlog::get_env "_ID" ]

puts "getenv $shmid "

set area [ atrshmlog::get_area ]

puts " the area is $area "

set realtime [ atrshmlog::get_realtime ]

puts "Seconds [ lindex $realtime 0 ] Nanos [ lindex $realtime 1 ] "

set statistics [  atrshmlog::get_statistics ] 

set i 0

foreach val $statistics {
    puts "atrshmlogstat $i $val "
    incr i
}

exit 0

# end of file
