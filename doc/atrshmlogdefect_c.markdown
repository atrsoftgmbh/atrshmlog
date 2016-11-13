The ATRSHMLOG test and maniplation tool.  {#files_atrshmlogdefect_c}
====================================

The program is for test and manipulation.

Its name is

     atrshmlogdefect
     

In the first variant you use the program with no or more than 2 parameters.

It then shows some vital numbers and makes some log entries.

The entries deliver a rought estimation about the logging speed.

In the second form you give exact two parameters.

The first is the offset in the shared memory buffer,
the second the value for the byte that you want to set at this offset.
Both are decimal.

So you can change things in shared memory that way.

This can help a lot in development.

For the first thing its a good estimate for the speed.

An example.

    $ atrshmlogcreate 4711 8
    $ . dot.atrshmlog
    $  atrshmloginit 
    $  atrshmlogdefect 1 2 3 4 5 6
    $  atrshmlogstopreader
    $  atrshmlogreaderd d1
    $  atrshmlogconv d1


You get :

0000001232 0000000000001232 000 000000000000000000 000073650122379490 000073650122383030 000000000000003540 1475084483130126254 1475084483130127585 000000000000001331 0000000002 I 0000000001 hundret times gettime

0000001232 0000000000001232 000 000000000000000000 000073650122383030 000073650122383030 000000000000000000 1475084483130127585 1475084483130127585 000000000000000000 0000000006 P 0000000001 Point prepare

....

0000001232 0000000000001232 000 000000000000000000 000073650122440900 000073650122440900 000000000000000000 1475084483130149341 1475084483130149341 000000000000000000 0000000004 P 0000000001 Point in time logging

0000001232 0000000000001232 000 000000000000000000 000073650122440900 000073650122468370 000000000000027470 1475084483130149341 1475084483130159669 000000000000010328 0000000005 I 0000000001 hundret times write to buffer

0000001232 0000000000001232 000 000000000000000000 000073650122468600 000073650122468600 000000000000000000 1475084483130159755 1475084483130159755 000000000000000000 0000000008 P 0000000001 Point in time logging atrshmlogdefect 1 2 3 4 5 6

....

0000001232 0000000000001232 000 000000000000000000 000073650122468600 000073650122468600 000000000000000000 1475084483130159755 1475084483130159755 000000000000000000 0000000008 P 0000000001 Point in time logging atrshmlogdefect 1 2 3 4 5 6

0000001232 0000000000001232 000 000000000000000000 000073650122468600 000073650122528980 000000000000060380 1475084483130159755 1475084483130182455 000000000000022700 0000000009 I 0000000001 hundret times write to buffer argv

Ok. here you have it.

Line one is the timing for hundret gettime calls. In this case this is 3540 clicks - resulting in rough 13 nanosecs per gettime.
Line 5 is the time for 100 writes. In this case 27470 clicks. Thats rough 103 nanoseconds for a log entry.
Line 8 gives you the numbers for the argv thing. ITs 227 nanoseconds.

Compare this with your way to log before.


