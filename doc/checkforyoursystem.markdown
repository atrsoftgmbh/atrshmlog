PROCESS : The check for your system.      {#process_checkforyoursystem}
=========================

The check for your system can be made by atrshmlogchecksystem.

You have to go to the src directory.

Get a shell. 

Start the script.

If it does not start check for perl.

Start it with perl

     $ perl atrshmlogchecksystem


The script writes out a greating message.

Then it writes out what kind of system it identifies.

You will get the architecture, the subarchitecture, the os and the compiler.

For the architecture it knows
- x86
- ia64
- sparc
- power
- arm

For the subarchitecture it knows
- 32 bit
- 64 bit

For the os it knows
- linux
- openbsd
- netbsd
- mirbsd
- solaris
- aix


For the compiler it knows
- gnu gcc
- clang
- xlc

You need this to do the adjustments in the header files.

Please check the header atrshmlog.h and atrshmlog_internal.h.

At last it writes out the define it finds for a supported
platform. so you can then use simply thath platform define in
the C code.

If it has no define its still worth to check for a system nearby.

So then check the found info and decide what supported system
is nearby.

Make a clone as in the adjustment process described.


