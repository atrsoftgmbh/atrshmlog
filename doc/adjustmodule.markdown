
PROCESS : Adjust the module.   {#process_adjustthemodule}
==========================

After you have made it so far you should have the module in the directories.

You should also have a clear info about

- the architecture
- the subarchitecture
- the operating system
- the compiler

If you have no clue about this, you can run

     $ atrshmlogchecksystem

which will give you info about the system, the architecture,
the compilers and the flavour of your system.

For my box it looks like this
     
     
     Hello to atrshmlog version 1, 0, 0 .
     
     This is the system and architecure check program.
     You get first the hits it gets in the analyse.
     Then you get possible defines for supported platforms
     if any match is there.
     
     found a linux system.
     found a x86 64 architecture.
     found a flavor fedora
     found a gcc compiler.
     found a naked cc compiler.
     
     Found hint for gcc linux_x86_64_fedora_gcc 
       define you can use is ATRSHMLOG_PLATFORM_LINUX_X86_64_GCC
       the ipc model is posix
     
     
     Found hint for cc linux_x86_64_fedora_gcc 
       define you can use is ATRSHMLOG_PLATFORM_LINUX_X86_64_GCC
       the ipc model is posix


You then get a list
of supported combinations with their define in the c source code.

See for the defines in the headers.

So you can now start to adjust the module.


The header atrshmlog.h
----------------------

Go to the src directory.

After the check you have a hint for the needed dot file for the
platform. Or you have no hint at all.

If not fire up your favorite text editor.

Open the atrshmlog.h header file.

Read in till you arrive at the defines.

###ATRSHMLOG_PLATFORM__os__architecture__subarchitecure__compiler

One of these is yours.

Use the proper dot.platform file to set this to 1.

GOOD.

###ATRSHMLOG_PLATFORM__os__architecture__subarchitecure__compiler not there ?

BAD.

Create a new one.

Example:

We got a nextstep os, m68k for the architecture, 32 bit and a gcc.

(Well, if it is not a gcc - can you tell me  if it supports c11 standard ?
If not, you have to get a gcc - or similar - to do the job.)

Ok. Then switch to the place where the define for a given
platform is used - best is

    ATRSHMLOG_PLATFORM_LINUX_X86_64_GCC

and start to copy the part of the platform.

Change the define.

Make a copy of the dot.platform file and rename it to the new platform.

If you need add a new flavour.

Good.

We have now a for example

      #if ATRSHMLOG_PLATFORM_NEXTSTEP_M68K_32_GNU == 1 

      /**
      * We use the modern cpu version of reading the click counter
      * register tsc.
      *
      * \return
       * The 64 bit number with the tick count.
        */
      inline uint64_t  atrshmlog_get_tsc_par_x86_64_gnu(void)
      {
        uint32_t hi, lo;
    
        __asm volatile
          ("rdtscp" : "=a" (lo), "=d" (hi));
    
        return  ((uint64_t)hi << 32) | lo;
      }
    
    
      /**
       * We use the old cpu version of reading the click counter
       * register tsc. We use a lfence for multicore here.
       *
       * \return
       * The 64 bit number with the tick count.
       */
      inline uint64_t  atrshmlog_get_tsc_fence_x86_64_gnu(void)
      {
        uint32_t hi, lo;
        
        __asm volatile
          ("lfence");
        
        __asm volatile
          ("rdtsc" : "=a" (lo), "=d" (hi));
    
        return  ((uint64_t)hi << 32) | lo;
      }

      /**
       * We use the old cpu version of reading the click counter
       * register tsc. No syncronization here.
       *
       * \return
       * The 64 bit number with the tick count.
       */
      inline  uint64_t atrshmlog_get_tsc_x86_64_gnu(void)
      {
        uint32_t hi, lo;
    
        __asm volatile
          ("rdtsc" : "=a" (lo), "=d" (hi));
    
        return  ((uint64_t)hi << 32) | lo;
      }

      /**
       * The well know dummy 
       * \return 
       * Null.
       */
      inline  uint64_t  atrshmlog_get_tsc_null_x86_64_gnu(void)
      {
        return 0;
      }
    #endif


Now adjust the function names - x86_64 moves to m68k_32 - and then the body
of the function.

This is the time to check if your cpu supports tick counters.

See <https://en.wikipedia.org> and then tsc time stamp counter.

Check for the demo file cycle.h and for your cpu.
Its also in the doc directory for reference.

If your compiler/platform is not there check
the manufacture info and the compiler assembler section.

If all fails use a get time os function and calc the things.

Next is the get clicktime. Adjust the names.

Last is to adjust the ATRSHMLOG_GET_TSC_CALL .

###ATRSHMLOG_INLINE

If you need adjust the inline macro. For now we dont use it,
But it can be in the future ...

###ATRSHMLOG_INLINE_TSC_CODE

If you have the tsc inline option from your compiler set it to 1.

For debugging or if you dont have inline to 0.

###ATRSHMLOG_INLINE_GETTIME

If you have the inline option set it to 1.

If not to 0.

###ATRSHMLOG_USE_PTHREAD

If you dont have c11 threads, you have to set this to 1 if you have pthreads.


###ATRSHMLOG_USE_WINTHREAD

If you have a fenster;plural system and you dont use any portability
layer stuff its the thing you need. This will work at least with mingw
for now.


###ATRSHMLOG_USE_C11_THREAD

If you have c11 threads for your compiler set it to 1.

If you have not c11 threads set it to 0.



###ATRSHMLOG_SYSCALL

If you have the syscall interface for your os set it to 1.

###ATRSHMLOG_USE_CLOCK_GETTIME

If you have the clock_gettime function set it to 1.

###ATRSHMLOG_USE_WIN_FILETIME

If you have a native fenster;plural system this is the best
thing to get a real time for now. Its in 100 nanos resolution.


###ATRSHMLOG_USE_GETTIMEOFDAY

If you have not another timing function set it to 1.

###ATRSHMLOG_USE_NANOSLEEP

If you have a nano sleep you can set this. But be sure.
On the fenster;plural system the thing is normally a lie.
There is no OS support for better than millisecond sleep.
Any other things are - normally - simple tricks to make the
time fit in a millisecond. Bad.

But worse is to have a busy wait sleep. Check that this is NOT the case
before you use it.

###ATRSHMLOG_USE_SYSCALL_TID

If you have the syscall and there is the tid call set it to 1.

###ATRSHMLOG_USE_PTHREAD_TID

If you have not the tid via syscall set  it to 1.

###ATRSHMLOG_USE_WINTHREAD_TID

If you have a fenster;plural system and no compatibility layer you
should use this tid.

###ATRSHMLOGPOINTINTIMEP
Normally you dont need to change this.
If you use an excotic OS with non ASCII then this should be
the Character with ASCII code 'P' number.

###ATRSHMLOGPOINTINTIMEp
Normally you dont need to change this.
the Character with ASCII code 'p' number.

###ATRSHMLOGPOINTINTIMEI
Normally you dont need to change this.
If you use an excotic OS with non ASCII then this should be
the Character with ASCII code 'I' number.

###ATRSHMLOGPOINTINTIMEi
Normally you dont need to change this.
the Character with ASCII code 'i' number.


And next the atrshmlog_internal.h
---------------------------------

Open the header.

Go to the first define, ATRSHMLOGVERSION

###ATRSHMLOGVERSION
If you made a new version - set the  ATRSHMLOGVERSION to the next number.

###ATRSHMLOGMINORVERSION
If you have added functionality, set the ATRSHMLOGMINORVERSION to the next number.

###ATRSHMLOGPATCHVERSION

IF you made a patch set the ATRSHMLOGPATCHVERSION to the next number.

###ATRSHMLOGBUFFER_INFOSIZE

If you think you are happy with bigger buffers set this to a higher value.

If you encounter memory shortage for the running system reduce it.

If you cange is adjust ATRSHMLOGVERSION, too.

See the doc for this.

### ATRSHMLOGBUFFER_PREALLOCED_COUNT

If you need more buffers than the default for your system you
should set this to a higher value.

Then adjust also the buffers.

If you have no use for this many buffers you can reduce it.

To adjust means also to change the code in impls for the static allocated
buffers.

To do this you use the script\n
atrshmloggenbuffers\n
which generates a proper proposal file for your change.
You have to copy the proposal to the real file if you think its ok.


###ATRSHMLOGBUFFER_MINCOUNT

Normally you dont need to change this.

###ATRSHMLOGBUFFER_MAXCOUNT

Normally you dont need to change this.


###ATRSHMLOG_ACCESS

If you have security concerns in development - well, then
change it to 660 or 600.

###ATRSHMLOG_INIT_BUFFER_SLAVE_WAIT

If you need only one slave and encounter still to high cpu
usage you can make it bigger.

###ATRSHMLOG_INIT_SLAVE_TO_SHM_WAIT

If you are in a high load szenario and have many wait events for
the shm transfer - you can set this higher.

###ATRSHMLOG_STRATEGY_WAIT_WAIT_TIME_DEFAULT

If you have a slow reaction in the wait strategy, you can
then set this smaller.

###ATRSHMLOG_TRANSFER_TO_SHM_START_DEFAULT

If you have a very wrong time for the transfer in adaptive
strategy you can set this different.

###ATRSHMLOG_INIT_PREALLOC_COUNT

If you have a very different load szenario and so you
cannot set the static buffer count good enough, you can
adjust the preallocation number for dyn memory alloc.
See the doc for it.

###ATRSHMLOG_MAX_PREALLOC_COUNT


Normally you dont need to change this.

###ATRSHMLOGARGVBUFFERLEN


Normally you dont need to change this.

###ATRSHMLOGEVENTCOUNTMAXLIMIT


Normally you dont need to change this.

###ATRSHMLOGBUFFER_INFOSIZE_MIN

Normally you dont need to change this.

###ATRSHMLOG_INIT_PREALLOC_COUNT_MIN

Normally you dont need to change this.


###ATRSHMLOG_INIT_BUFFER_SLAVE_WAIT_MIN
Normally you dont need to change this.

###ATRSHMLOG_INIT_BUFFER_SLAVE_WAIT_MAX
Normally you dont need to change this.

###ATRSHMLOG_SLAVE_COUNT_MIN
Normally you dont need to change this.


###ATRSHMLOG_SLAVE_COUNT_MAX
Normally you dont need to change this.


###ATRSHMLOG_CLOCK_ID_MIN
This has to be adjusted to your implementation of get_clicktime
if you have to change it.

###ATRSHMLOG_CLOCK_ID_MAX
This has to be adjusted to your implementation of get_clicktime
if you have to change it.


###ATRSHMLOGEVENTCOUNT
This can be changed if you know what you use the module for.
In case of an instrumentalization with a tool it can be
needed to make it bigger, for manual approch its ok.

The maximum number is vital in all macros that use an
event, so be sure to have it big enough for the thing you do.
Event numbers are strictly indexes into an signed char array,
so making it big also consumes memory.
Making it small is a good thing if you can do that - it reduces
cache trashing and memory use.
If you can use the environment or the helpers and after that
adjust the define here.

###ATRSHMLOGTARGETBUFFERMAX
This can be used to make memory use more smooth if you only
have a few threads but transfertimes in shared memory and reader is
too high. Alternative to making the buffer infosize bigger.
You should check the doc before you start playing with it.


###ATRSHMLOGCONTROLDATASIZE
Normally you dont need to change this.

If you insit to use different types for the transferdata you should
redefine the typedefs, rather than changing this.
If you change it you have to check for the write functions,
the convert program and the reader programs.

###ATRSHMLOG_ENV_PREFIX
If you cannot use the prefix ATRSHMLOG here you can redefine this
and then use it instead as the prefix for the environment variables
and the flag files.

###ATRSHMLOGDEBUG
If you need debugging then this should give you most of the needed
output if you switch to 1.
Normal use should be 0.

And next the atrshmlogimpl*.c
---------------------------------

We have to adjust several things in the implementation too.

Open the files in your text editor.

###atrshmlog_buffers_prealloced
If you change the define ATRSHMLOGBUFFER_PREALLOCED_COUNT you
have to change the initalization too.
And it has to match.
So see for the thing of initilization here:\n
We start with 0 and reach the ATRSHMLOGBUFFER_PREALLOCED_COUNT - 2 with
the macro ATRSHMLOGBUFFERS_PRE().
For the last element the number is ATRSHMLOGBUFFER_PREALLOCED_COUNT - 1
and the macro is ATRSHMLOGBUFFERS_PRE_LAST().
This is a must - not a nice - or you will have funny nights with debugging
strange behavior.
To make things easy there is now a helper to make a proposal file
form the actual setting in the internal header.
If you are ok with the proposal you simply copy it to the original file.
The script is atrshmloggenbuffers.

###atrshmlog_buffer_strategy
Be sure you know what you do. You can set another strategy here.


###atrshmlog_thread_fence_X
If your system is not so nice as the x86_64 platform it can be
necessary to swith the fences on.
So if you are indeed on - lets say  for good , power 64 , you will
eventually need the fences.
Then check that they work and switch the on.
See the doc before you do it.

###atrshmlog_clock_id
If you know that you need a other clock id then you should
redefine this.

###atrshmlog_delimiter
If you are on a funny OS you should consider to set this to the
ASCII value for SPACE.

###atrshmlog_sleep_nanos()
If your platform has a worse handling in nanosleep than mine
its possible you need to change the loop increment time timeframe here.

###atrshmlog_create_slave()
This can need a new branch if you dont have the existing
ones.

###atrshmlog_f_list_buffer_slave_proc()
This can need a new branch if you do another thread model.
Thats for the typedef AND for the used union in the code.

###atrshmlog_get_statistics()
We assume at least 32 bit ints here. So we use a bruite force
hack to split the 64 bit click time.
If your int is bigger than 64 bit you can try to change this.

###ATRSHMLOG_INLINE_TSC_CODE
From here on you have to add new code if your platform
differs from the already implemented.

The rest.
---------

Hm, thats hard to say. Convert, reader, what else ?

For the convert it depends mostly on the typedefs and
defines, so it should self adjust.

For the reader it should also self adjust.

But i have not tried everything yet.

Check compiler warnings - errors should not exist.

Check the bin files for the platform and flavour things.

Thats so far the thing i know.



