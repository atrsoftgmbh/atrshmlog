Language layer : Java jni   {#language_layer_java_jni}
==========================

The java jni layer makes the use of the module possible for the java
development.

The java layer consists of two classes in the package\n
de.atrsoft.successorofoak.utilities.logging.atrshmlog\n
which are ATRSHMLOG and ATRSHMLOGTest.

ATRSHMLOG
---------
The class ATRSHMLOG contains all stuff of the module that can be useful
in a client application.

###Loading the library

First of all it is responsible for the loading of the library that
contains the jni layer and the module code.

So it is doing the loading on class load and this is one of the
popular ways today.

Its simple to change the class - you have the source code after all.

Its also simple to change the loading then - you comment it out
and
you do it in your application elsewhere before the first jni
is used.

###Basic functions

The basic functions are implemented as methods for a vanilla
object of the class.

So you can do things with inheritance.

Its up to you how the interface is used.

####attach()

Its the attaching of the module. Same functionality like the C function
atrshmlog_attach(). Simply use it and check its returncode.

####gettime()

Its the same functionality like the C function, but it uses the
value in its return code. We assume its a 64 bit click time, so
the return value is a long. As the C version it uses the event
number to check if the call has to be made at all. Still
there is the overhead of the jni call itself, so be prepared to have
a somewhat slower speed, more like 150 clicks against 40 clicks in C.
But its faster than most other timing functions. So we use it here for the
time meature.

####sgettime()

Its the same functionality like the C function, but it uses the
value in its return code. We assume its a 64 bit click time, so
the return value is a long. As the C version it uses the event
number to check if the call has to be made at all. Still
there is the overhead of the jni call itself, so be prepared to have
a somewhat slower speed, more like 150 clicks against 40 clicks in C.
But its faster than most other timing functions. so we use it.
This is a staic test version. If we get feedback thath this is faster
than the method of the object we will support at least the write methods
also as a static. So check this one time and send me a post card if its
faster.

####write()

You have several write methods.

You can give only the basic technical info, or you can give a String,
or a byte array.

For the String version there are two methods.

The basic write with
String transfers the String as ucs2 char array to the module write.
The write and the reader are moving it then binary. The converter
can make use of the lower bytes of the string in this version.
The String must be written with an eventflag
EVENT_POINT_IN_TIME_UCS or EVENT_INTERVAL_IN_TIME_UCS.

If you use the eventflags EVENT_POINT_IN_TIME_C
or EVENT_INTERVAL_IN_TIME_C
the String will be converted by the jni UTF conversion function.
In this case you get rough an UTF String as output with the converter.

The next write with String uses a limitation for the length - so you can
write a String thats far to big or even with unknown length by supporting
a limit. If the string is shorter the limit will be ignored, if the
String is bigger it will be cut of at the limit.

This has the advantage that you have not to make new Strings because of
their length - you use the String as it is. The String is here transfered
as Ucs2 and so the eventflags can be used as you decide, but only the Ucs
versions will be delivered to the converter. The C versions are replaced
by the Ucs versions.
 
The last write uses a byte array. So you can transfer what you want
in form of a byte string - only the size has to fit for logging with the
module. Interpretation is of course than a problem.
So try to use a self made converter in this case.

####getStatisticsMaxIndex()

This is the highest index the statistics counter use. The complete
array has then this plus one entries. You have to support
the getStatistics method the array, so you first get the highest index,
add one and reserve an empty array this way. The getStatistics
then fills the array.

####getStatistics()

This gets the statistics counter array and fills the values in the
given o_target array. Be sure to allocate it with the correct size.

####sleepNanos()

This makes use of the module function atrshmlog_sleep_nanos().
So you can use this as a sleep in your code, too.
Be prepared that this will not work on a platform that
does not support a real nano sleep. Namely that's the case
for the fenster;plural platform.

####setEnvPrefix()

This sets the environment variable prefix. This has to be used
before you make an attach call. So you can use your very own prefix
for the environment variables and flag files.

See the atrshmlog_set_env_prefix() function for details.

####getEnvPrefix()

This delivers the actual prefix for environment variables and flagfiles.

####getEnv()

This uses the module atrshmlog_get_env() function  to read an environment
variable with the given suffix and the module prefix.

####getEnvShmid()

This uses the module function to deliver the environment variable for
the shared memory id - its value to be precise.

####getEnvIdSuffix()

This delivers the environment variable for shared memory id suffix.

####getVersion()

This deliver the version for the module.

####getMinorVersion()

This deliver the minor version of the module.

####getPatchVersion()

This delivers the patch version of the module.

####getEventLocksMax()

This delivers the max index for events that can be used.
The event is represented by its number, the offset in an C char array,
and the value of that char. 


####setEventLocksMax()

This uses the module C function atrshmlog_set_event_locks_max()
to set the new max index in the events buffer. The number can be
smaller - the array will be in place, but the higher part is no
longer used. If it is bigger a new array is build via malloc and
the new events are set to 0.
So this function is best used before you attach, or you have to populate
the  event values by yourself.

####getEvent()

This deliver an event value from the buffer of events.

####setEventOn()

This switches the event on.

####setEventOff()

This switches the event off.

####getLogging()

This delivers the combination of the process logging flag, the shared memory
flag and the final flag.

The return code is
- 0 : logging is on
- non zero : code for the thing that switched logging off.

####setLoggingProcessOn()

This sets the process logging flag on (to 1).

####setLoggingProcessOff()

This sets the process logging flag to off ( to 0).

####setLoggingProcessOffFinal()

This sets the final loging flag to off.

There is no way to set it on again.

This is done to make all internal loops to end in the threads
for slave and for copy to shared memory loops.

This should be used when a process needs to stop and has to
switch of all logging activity for this, too.

####getShmid()

This deliver the shared memory id from the attach call.

####getArea()

This delivers the area address as a 64 bit long value for the use in some
area getter methods.

Please be sure not to change the value. This can crash your application.

####getAreaIchHabeFertig()

This delivers the value of the system wide flag in shared memory.
- 0 is for we are logging
- non zero is for we are no longer logging.

The flag can be set on and of, so you can stop logging for the system
and then switch it again on.

####setAreaIchHabeFertig()

This sets the system wide logging flag in the shared memory.
- 0 is for logging
- non zero is for no more logging

####getAreaCount()

This deliver the number of buffers in the shared memory .

####getBufferMaxSize()

This is the maximum size for log buffers in this version.
You can set the actual used size smaller. But for all
static allocated buffers this is the size, also for all
buffers in shared memory and in reader and converter.

####getBufferSize()

This deliver the actual size for log buffers. It can be set
smaller than the max size.

####setBufferSize()

This sets the acual buffer size to a new limit - it can be smaller
than the max size, but not bigger.

####getFListBufferSlaveCount()

This deliver the number of threads that are running as slave.
The number is set before attach and then used to start that many
slave threads.

You have to set it before the attach method call.

####setFListBufferSlaveCount()

This sets the number of slave threads that will be startet in attach.

You have to set it before you attach.

If you fork the process this is the number of threads that are
startet if you write to log. So you can stop transfer by setting it to 0
before you fork.

####getClockId()

This delivers the id of the used get click time function in the module.

####setClockId()

This sets the id of the used get click time function in the module.

####setFListBufferSlaveRunOff()

This set the flag that all slave threads make end. The slave still
runs till the flag is checked, so its not a kill. But after a slave
has made a wait or was active it is checked in the slave thread
and the threads then ends.

####setWaitForSlavesOn()

This sets the flag to wait for the end of slaves in the cleanup
function of the  module to on.
The cleanup will then first check if the threads are gone, then proceed.

You can set the flag to on, it is off by default.

####setWaitForSlavesOff()

This sets the flag to off, see setWaitForSlavesOn.

####getWaitForSlaves()

This deliver the flag for wait for slaves in the cleanup function.


####getFListBufferSlaveWait()

This deliver the nanoseconds a slave waits after he has found nothing
must be done til the next check.

####setFListBufferSlaveWait()

This sets the nanosecond a slave waits.

####getAcquireCount()

This deliver the number of buffers that are available for threads to log.


####getPreallocBufferCount()

This deliver the number of buffers that are allocated in case a dyn memory
alloc is used to get more log buffers.

####setPreallocBufferCount()

This sets the number of buffers that are allocated when a dyn allocate
is done.

####getInittime()

This deliver in a long array of at least two members the init real time
in nanoseconds for the element 0 and seconds in the element 1.

####getInittimeTscBefore()

This delivers the click time before the inittime was made.

####getInittimeTscAfter()

This delivers the click time after the inittime was made.

####getBufferId()

This delivers the highest used buffer id.

####stop()

This stops for the calling thread the logging.
There is no way a thread can log after it called stop.

####flush()

This flushes the used buffers of the calling thread, so they are then on the
full buffer list for slave processing.

Be sure you know the consequence : You have now to wait for
successful logging till the slaves could deliver the buffers to
the shared memory.

####setStrategy()

This set the strategy for the thread to handle an all buffer full situation.

Read the doc before you use this.

####getStrategy()

This gets the actual setting for the thread.

Read the doc before you use this.

####setStrategyProcess()

This sets the process wide strategy for handling a all buffer full in
logging situation. It is used in a new thread to initialize the threads value.

You cannot change the value for the thread with this, you have to do that the
method setStrategy in the thread itself.

####getStrategyProcess()

This gets the actual default in the process.

####createSlave()

This starts a new slave in the module for processing the slave function.

You can use this in case you encounter a high number of buffer full situations.

Read the doc before you try to use it.

####decrementSlaveCount()

This decrements the slave count for the module. If you interfere by killing
slave threads from outside the modules code you need to decrement the
counter too.

Be sure to know the consequences if you kill slaves. Read the doc.


####setThreadFence1() to setThreadFence13()

This sets the flag to honor the fence.

Se the doc for thread fence use.

####getThreadFence1() to getThreadFence13()

This delivers the flag for use of the fence.

####getRealtime()

This deliver in a long array with a count of at least 2 the actual real
time of the module.
The number at index 0 is the nanosecond part, the number index 1
the seconds part.

####getThreadLocalsAdress()

This delivers the adress for the threds own thread local struct.

This is needed in operations for managing the threads logging.
You need it for a forced stop from outside the thread.
So you shoud use it together with the tid and then deliver it to any
kind of controller thread system you have in your application.

####getTid()

This delivers the thread tid as it is used in the module.

Be sure you understand your thread identification before you start to
use it.

####turnLoggingOff()

This switches the logging off for the specified thread.
The thread is identified by its thread local struct adress.

So you need at least the tid and the thread local adress to identify and then
stop the logging for a thread from outside.

The stop is a soft one, it will work when the thread is active, but not kill
the thread. It simply stops its logging.

Also the thread buffers are put on the slave list, so any content will
be moved to the area and the readers. The buffers are set to the
available list after transfer of the content, so they will be recycled from
that list if needed. The thread itself cannot use them any more.

Be sure you know what you do if you switch logging off, there is no turn
on button.

If the thread local is of a slave thread you switch off the threads main
loop and the thread will finish. There are no buffers for a slave thread.
That's the prefered way to finish a slave.

####getNextSlaveLocal()

This is a kind of iterator for the slave list.

You start with 0 and get
the top of the slave list. The list contains the thread locals for the
slaves.

You end up when the last slave is used with 0 again - meaening you have done
a full walk.

The value is the thread local and can be used to identify a thread, to
turn it off, to get its tid and to remove it from the slave list.

####getThreadLocalTid ()

This delivers the thread id from the thread local structure of the thread.

You can use this to identify the thread with your OS or thread management
system.

####removeSlaveViaLocal()

This removes an entry from the slave list - you need this if you kill a thread
to maintain the list.

WARNING: To use it you have to call it BEFORE you kill.

####setInitBuffersInAdvanceOn()

This sets the init buffers in advance flag.

This is then used in an attach for the initialize of static buffers.

It is used also to initialize dynamic allocated buffers.

This is per default on for the fenster;plural systems.

On the posix systems its off.

#### setInitBuffersInAdvanceOff()

This clears the init buffers in advance flag.

This is then used in an attach for the initialize of static buffers.

It is used also to initialize dynamic allocated buffers.

This is per default on for the fenster;plural systems.

On the posix systems its off.

#### getInitBuffersInAdvance()

This delivers the flag value.

####reuseThreadBuffers()

This is for killed threads. If you have the tid the module uses you can get
the  buffers back on the available list for reuse.

####verify()

This uses the atrshmlog_verify() module function to check for problems in the
shared memory buffers.

####getClicktime()

This deliver the click time with the clock identified by clock id.

####read()

This fetches a buffer from the area into the process memory. Its the
now outdated reader backbone function.

You can use this to fetch buffers from the area, but its not recommended
to do it. Use the readFetch instead.

####readFetch()

This fetches a buffer from the area if any is used. If no buffer is used
it return with success and a length of 0 for the fetched buffer - no real
fetch is done, its only a test for the internal list and the returns.

####create()

This creates an area. Its the backbone function for the atrshmlogcreate program.

####delete()

This deletes an area. Its the backbone function of the atrshmlogdelete program.

On the mingw based systems its only a dummy, the OS uses a reference counting
and so deletes the buffer when no process is attached any more itself.

####cleanupLocks()

This is the backbone function of the atrshmlogfinish program. IT gives any
resources back to the system before you destroy an area with the delete call. 

#### initShmLog()

This is the initialization of an area. Its the backbone function for the
atrshmloginit program.

It acquires the needed resources that are used inside the area (whatever
thah is on the platform).

####peek()

We can check a byte in the area with this.


####poke()

We can set a byte in the area with this.

ATRSHMLOGTest
--------------

This is a small test driver for the jni layer.


How to use it
-------------

Import the ATRSHMLOG class and see the ATRSHMLOGTest fro the use of
attach, gettime and write.

You will need an initialized buffer.

To get the log you will need the reader.

To convert it to a text file you will need the convert.

So we make a small test run here to se how its done.

###create the buffer

You use the atrshmlogcreate for this.

     $ atrshmlogcreate 4711 8   # use 1 and 8 instead on mingw

Thats the create for a buffer with key 4711 / 1 and size of 8 log buffers.

You can check with your ipc tool

     $ ipcs -m

You get the values needed for the environment variable as an output.

Cut and paste it in the shell window . Or use the dot file.

The cut and paste in the window of your java test.

###initialize the buffer

You have to initialize the created buffer. To do this you use
the atrshmloginit program.

     $ atrshmloginit 8

It uses the environment variable for the shared memory id of the buffer.
So execute in a shell that you have set the variavles from the output
of atrshmlogcreate.
It uses the parameter for the count of buffers. You can make it  so smaller
for the log. Making it bigger is an error. So use normally the count
as given to the atrshmlogcreate.

###start the test in ATRSHMLOGTest

You can now start the test. ITs simple. Execute main.

To help you there is a shell script to do it.

     $   start_package_log.sh

On mingw

     CMD> start_package_log.cmd
     

It sets the needed values like the library path and calls the test class
main function.

###get the log from the buffer

You have now the log in the buffer, so you have to transfer it from there
to the file system. This is done by the reader.

You start the reader normally to a active buffer, so it blocks and does the
transfer in the background. In this case you dont need a block, so you
switch the buffer to inactive. The reader then will transfer at start and
then stop itself.

To set the buffer inactive you use the atrshmlogsignalreader program.

     $ atrshmlogstopreader

Now you can start the reader

     $ atrshmlogreaderd d1


This will transfer the logbuffer into a file. The file is located in
a directory tree. The tree starts with d1. The file is normally in the
first folder in the tree after d1. The folder is  0.

### convert the log to human readable text

Now you have a log in a file, but its still binary data. To access it you have
to convert to text.

This is done by the converter program.

     $ atrshmlogconv d1
     
This is actually a helper, not the converter itself, but it should do the job.

You get a converted file with suffix .txt from the .bin file.

Open that file in a text editor and check against the main method in
the ATRSHMLOGTest class.

How to build it
---------------

Copy the  c library and the two headers to the src directory.

     $ cd BASEDIR/java ; bin/getfrommain.sh

Switch to your java location.

We use in this version as reference the jdk 1.8.0.66 on linux x86_64.

If you use a different jdk you have to adjust the dot.java.sh file .
And you should make a proper copy into a tree that matches your
java system.

     $  cd BASEDIR/java/oracle/jdk1.8.0_66/src

Set the needed java home with the

     $ . dot.java.sh


If you use a different java version you have at least to adjust
the content of this file.

Compile the jni library.

     $  create_jni_lib.sh

Summary
-------


This is a fast path intro, so most stuff that you do not need
in the first place is not here.  See the doc for it.

