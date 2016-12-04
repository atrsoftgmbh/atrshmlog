#!/usr/bin/perl
#
# $Id:$
#
# perl test driver

use Atrshmlog;

$result = Atrshmlog::attach();

$f = Atrshmlog::get_thread_fence_1();

print "fence : $f : \n";

$old = Atrshmlog::set_thread_fence_1(1);

print "fence : $old : \n";

$f = Atrshmlog::get_thread_fence_1();

print "fence : $f : \n";

$old = Atrshmlog::set_thread_fence_1(0);

print "fence : $old : \n";

$f = Atrshmlog::get_thread_fence_1();

print "fence : $f : \n";

$f = Atrshmlog::get_thread_fence_2();

print "fence : $f : \n";

$old = Atrshmlog::set_thread_fence_2(1);

print "fence : $old : \n";

$f = Atrshmlog::get_thread_fence_2();

print "fence : $f : \n";

$old = Atrshmlog::set_thread_fence_2(0);

print "fence : $old : \n";

$f = Atrshmlog::get_thread_fence_2();

print "fence : $f : \n";

$f = Atrshmlog::get_thread_fence_3();

print "fence : $f : \n";

$old = Atrshmlog::set_thread_fence_3(1);

print "fence : $old : \n";

$f = Atrshmlog::get_thread_fence_3();

print "fence : $f : \n";

$old = Atrshmlog::set_thread_fence_3(0);

print "fence : $old : \n";

$f = Atrshmlog::get_thread_fence_3();

print "fence : $f : \n";

$f = Atrshmlog::get_thread_fence_4();

print "fence : $f : \n";

$old = Atrshmlog::set_thread_fence_4(1);

print "fence : $old : \n";

$f = Atrshmlog::get_thread_fence_4();

print "fence : $f : \n";

$old = Atrshmlog::set_thread_fence_4(0);

print "fence : $old : \n";

$f = Atrshmlog::get_thread_fence_4();

print "fence : $f : \n";

$f = Atrshmlog::get_thread_fence_5();

print "fence : $f : \n";

$old = Atrshmlog::set_thread_fence_5(1);

print "fence : $old : \n";

$f = Atrshmlog::get_thread_fence_5();

print "fence : $f : \n";

$old = Atrshmlog::set_thread_fence_5(0);

print "fence : $old : \n";

$f = Atrshmlog::get_thread_fence_5();

print "fence : $f : \n";

$f = Atrshmlog::get_thread_fence_6();

print "fence : $f : \n";

$old = Atrshmlog::set_thread_fence_6(1);

print "fence : $old : \n";

$f = Atrshmlog::get_thread_fence_6();

print "fence : $f : \n";

$old = Atrshmlog::set_thread_fence_6(0);

print "fence : $old : \n";

$f = Atrshmlog::get_thread_fence_6();

print "fence : $f : \n";

$f = Atrshmlog::get_thread_fence_7();

print "fence : $f : \n";

$old = Atrshmlog::set_thread_fence_7(1);

print "fence : $old : \n";

$f = Atrshmlog::get_thread_fence_7();

print "fence : $f : \n";

$old = Atrshmlog::set_thread_fence_7(0);

print "fence : $old : \n";

$f = Atrshmlog::get_thread_fence_7();

print "fence : $f : \n";

$f = Atrshmlog::get_thread_fence_8();

print "fence : $f : \n";

$old = Atrshmlog::set_thread_fence_8(1);

print "fence : $old : \n";

$f = Atrshmlog::get_thread_fence_8();

print "fence : $f : \n";

$old = Atrshmlog::set_thread_fence_8(0);

print "fence : $old : \n";

$f = Atrshmlog::get_thread_fence_8();

print "fence : $f : \n";

$f = Atrshmlog::get_thread_fence_9();

print "fence : $f : \n";

$old = Atrshmlog::set_thread_fence_9(1);

print "fence : $old : \n";

$f = Atrshmlog::get_thread_fence_9();

print "fence : $f : \n";

$old = Atrshmlog::set_thread_fence_9(0);

print "fence : $old : \n";

$f = Atrshmlog::get_thread_fence_9();

print "fence : $f : \n";

$f = Atrshmlog::get_thread_fence_10();

print "fence : $f : \n";

$old = Atrshmlog::set_thread_fence_10(1);

print "fence : $old : \n";

$f = Atrshmlog::get_thread_fence_10();

print "fence : $f : \n";

$old = Atrshmlog::set_thread_fence_10(0);

print "fence : $old : \n";

$f = Atrshmlog::get_thread_fence_10();

print "fence : $f : \n";

$f = Atrshmlog::get_thread_fence_11();

print "fence : $f : \n";

$old = Atrshmlog::set_thread_fence_11(1);

print "fence : $old : \n";

$f = Atrshmlog::get_thread_fence_11();

print "fence : $f : \n";

$old = Atrshmlog::set_thread_fence_11(0);

print "fence : $old : \n";

$f = Atrshmlog::get_thread_fence_11();

print "fence : $f : \n";

$f = Atrshmlog::get_thread_fence_12();

print "fence : $f : \n";

$old = Atrshmlog::set_thread_fence_12(1);

print "fence : $old : \n";

$f = Atrshmlog::get_thread_fence_12();

print "fence : $f : \n";

$old = Atrshmlog::set_thread_fence_12(0);

print "fence : $old : \n";

$f = Atrshmlog::get_thread_fence_12();

print "fence : $f : \n";

$f = Atrshmlog::get_thread_fence_13();

print "fence : $f : \n";

$old = Atrshmlog::set_thread_fence_13(1);

print "fence : $old : \n";

$f = Atrshmlog::get_thread_fence_13();

print "fence : $f : \n";

$old = Atrshmlog::set_thread_fence_13(0);

print "fence : $old : \n";

$f = Atrshmlog::get_thread_fence_13();

print "fence : $f : \n";



print " \n";

exit (0);

# end of main
