Language layer : ruby   {#language_layer_ruby}
==========================

The ruby layer makes the use of the module possible for the ruby
development.

The layer consists of a bridge code file \n
atrshmlogruby.c\n


atrshmlogruby.c
---------
The bridge code contains all stuff of the module that can be useful
in a client application. For some functions it didn't worked, and so some
helper functions were needed. They are implemented in this file.

Note:\n
Because I didnt wanted to make objects explicit or classes I use
the singleton functions here. If you think this is wrong or should
be changed sent me a post card and i will do it - best with a small
example how to do it.

###Loading the library

You use it as always by

     require  'atrshmlog'
     ret = Atrshmlog::attach()
     
This is for the use from ruby.

###Basic functions

The basic functions are implemented as simple functions of the ruby module.
They make a thin wrapper around the functions of the C module.
So the most functions are using the C module directly. Only the gettime,
write, write_binary, get_inittime, get_realtime, read and read_fetch need
a helper to handle parameters right.

####attach()

Its the attaching of the module. Same functionality like the C function
atrshmlog_attach(). Simply use it and check its returncode.

####gettime()

Its the same functionality like the C function, but it uses the
value in its return code. We assume its a 64 bit click time, so
the return value is a FIX 63.

####write0()

This is the basic write for a simple technical log. You have simply
the event - that's the place you are in - the eventflag, which
means a I or P in ascii value to make clear you want a Point or an Interval,
and then the user flag for your own purpose.
Last are the time values.
In case of a I eventflag the first can be 0 and the call will substitute the
second time with the actual clicktime. the second time is always same as the
first here.

For the I eventflag its the second time that can be set to 0 and that
then is sustituted by the call with the actual click time.

There are the constants for the both - actual 4 total - eventflag values.
- EVENT_POINT_IN_TIME_C
  This is the logging eventflag for a point log 
- EVENT_INTERVAL_IN_TIME_C
  This is the logging eventflag for an interval log 
- EVENT_POINT_IN_TIME_UCS
  This is the logging eventflag for a point log with an ucs2 java string
- EVENT_INTERVAL_IN_TIME_UCS
  This is the logging eventflag for an interval log with an ucs2 java string

To make it simple: use only the C things here.

####write()

This is the write with a string as payload. See for the rest write0.

####get_statistics_max_index()

This is the highest index the statistics counter use. The complete
array has then this plus one entries. 

####get_statistics()

This gets the statistics counter array and fills the values in
an array. I used more than actual is in use, so check for the index too. 

####sleep_nanos()

This makes use of the module function atrshmlog_sleep_nanos().
So you can use this as a sleep in your code, too.
Be prepared that this will not work on a platform that
does not support a real nano sleep. Namely that's the case
for the fenster;plural platform.

####set_env_prefix()

This sets the environment variable prefix. This has to be used
before you make an attach call. So you can use your very own prefix
for the environment variables and flag files.

See the atrshmlog_set_env_prefix() function for details.

####get_env_prefix()

This delivers the actual prefix for environment variables and flagfiles.

####get_env()

This uses the module atrshmlog_get_env() function  to read an environment
variable with the given suffix and the module prefix.

####get_env_shmid()

This uses the module function to deliver the environment variable for
the shared memory id - its value to be precise.

####get_env_id_suffix()

This delivers the environment variable for shared memory id suffix.

####get_version()

This deliver the version for the module.

####get_minor_version()

This deliver the minor version of the module.

####get_patch_version()

This delivers the patch version of the module.

####get_event_locks_max()

This delivers the max index for events that can be used.
The event is represented by its number, the offset in an C char array,
and the value of that char. 

####set_event_locks_max()

This uses the module C function atrshmlog_set_event_locks_max()
to set the new max index in the events buffer. The number can be
smaller - the array will be in place, but the higher part is no
longer used. If it is bigger a new array is build via malloc and
the new events are set to 0.
So this function is best used before you attach, or you have to populate
the  event values by yourself.

####get_event()

This deliver an event value from the buffer of events.

####set_event_on()

This switches the event on.

####set_event_off()

This switches the event off.

####get_logging()

This delivers the combination of the process logging flag, the shared memory
flag and the final flag.

The return code is
- 0 : logging is on
- non zero : code for the thing that switched logging off.

####set_logging_process_on()

This sets the process logging flag on (to 1).

####set_logging_process_off()

This sets the process logging flag to off ( to 0).

####set_logging_process_off_final()

This sets the final loging flag to off.

There is no way to set it on again.

This is done to make all internal loops to end in the threads
for slave and for copy to shared memory loops.

This should be used when a process needs to stop and has to
switch of all logging activity for this, too.

####get_shmid()

This deliver the shared memory id from the attach call.

####get_area()

This delivers the area address as a reference value for the use in some
area getter methods.

Please be sure not to change the value. This can crash your application.

####get_area_ich_habe_fertig()

This delivers the value of the system wide flag in shared memory.
- 0 is for we are logging
- non zero is for we are no longer logging.

The flag can be set on and of, so you can stop logging for the system
and then switch it again on.

####set_area_ich_habe_fertig()

This sets the system wide logging flag in the shared memory.
- 0 is for logging
- non zero is for no more logging

####get_area_count()

This deliver the number of buffers in the shared memory .

####get_buffer_max_size()

This is the maximum size for log buffers in this version.
You can set the actual used size smaller. But for all
static allocated buffers this is the size, also for all
buffers in shared memory and in reader and converter.

####get_buffer_size()

This deliver the actual size for log buffers. It can be set
smaller than the max size.

####set_buffer_size()

This sets the acual buffer size to a new limit - it can be smaller
than the max size, but not bigger.

####get_f_list_buffer_slave_count()

This deliver the number of threads that are running as slave.
The number is set before attach and then used to start that many
slave threads.

You have to set it before the attach method call.

####set_f_list_buffer_slave_count()

This sets the number of slave threads that will be startet in attach.

You have to set it before you attach.

If you fork the process this is the number of threads that are
startet if you write to log. So you can stop transfer by setting it to 0
before you fork.

####get_clock_id()

This delivers the id of the used get click time function in the module.

####set_clock_id()

This sets the id of the used get click time function in the module.

####set_f_list_buffer_slave_run_off()

This set the flag that all slave threads make end. The slave still
runs till the flag is checked, so its not a kill. But after a slave
has made a wait or was active it is checked in the slave thread
and the threads then ends.

####set_wait_for_slaves_on()

This sets the flag to wait for the end of slaves in the cleanup
function of the  module to on.
The cleanup will then first check if the threads are gone, then proceed.

You can set the flag to on, it is off by default.

####set_wait_for_slaves_off()

This sets the flag to off, see set_wait_for_slaves_on.

####get_wait_for_slaves()

This deliver the flag for wait for slaves in the cleanup function.


####get_f_list_buffer_slave_wait()

This deliver the nanoseconds a slave waits after he has found nothing
must be done til the next check.

####set_f_list_buffer_slave_wait()

This sets the nanosecond a slave waits.

####get_acquire_count()

This deliver the number of buffers that are available for threads to log.


####get_prealloc_buffer_count()

This deliver the number of buffers that are allocated in case a dyn memory
alloc is used to get more log buffers.

####set_prealloc_buffer_count()

This sets the number of buffers that are allocated when a dyn allocate
is done.

####get_inittime()

This deliver the seconds and the nanoseconds of the inittime
back in a array of two values.

####get_inittime_tsc_before()

This delivers the click time before the inittime was made.

####get_inittime_tsc_after()

This delivers the click time after the inittime was made.

####get_buffer_id()

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

####set_strategy()

This set the strategy for the thread to handle an all buffer full situation.

Read the doc before you use this.

####get_strategy()

This gets the actual setting for the thread.

Read the doc before you use this.

####set_strategy_process()

This sets the process wide strategy for handling a all buffer full in
logging situation. It is used in a new thread to initialize the threads value.

You cannot change the value for the thread with this, you have to do that the
method set_strategy in the thread itself.

####get_strategy_process()

This gets the actual default in the process.

####create_slave()

This starts a new slave in the module for processing the slave function.

You can use this in case you encounter a high number of buffer full situations.

Read the doc before you try to use it.

####decrement_slave_count()

This decrements the slave count for the module. If you interfere by killing
slave threads from outside the modules code you need to decrement the
counter too.

Be sure to know the consequences if you kill slaves. Read the doc.


####set_thread_fence_1() to set_thread_fence_13()

This sets the flag to honor the fence.

Se the doc for thread fence use.

####get_thread_fence1() to get_thread_fence13()

This delivers the flag for use of the fence.

####get_realtime()

This deliver the seconds and nanos for the realtime clock
the module uses in an array of two values.

####get_thread_locals_adress()

This delivers the adress for the threds own thread local struct.

This is needed in operations for managing the threads logging.
You need it for a forced stop from outside the thread.
So you shoud use it together with the tid and then deliver it to any
kind of controller thread system you have in your application.

####get_tid()

This delivers the thread tid as it is used in the module.

Be sure you understand your thread identification before you start to
use it.

####turn_logging_off()

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

####get_next_slave_local()

This is a kind of iterator for the slave list.

You start with 0 and get
the top of the slave list. The list contains the thread locals for the
slaves.

You end up when the last slave is used with 0 again - meaening you have done
a full walk.

The value is the thread local and can be used to identify a thread, to
turn it off, to get its tid and to remove it from the slave list.

####get_thread_local_tid ()

This delivers the thread id from the thread local structure of the thread.

You can use this to identify the thread with your OS or thread management
system.

####remove_slave_via_local()

This removes an entry from the slave list - you need this if you kill a thread
to maintain the list.

WARNING: To use it you have to call it BEFORE you kill.

####set_init_buffers_in_advance_on()

This sets the init buffers in advance flag.

This is then used in an attach for the initialize of static buffers.

It is used also to initialize dynamic allocated buffers.

This is per default on for the fenster;plural systems.

On the posix systems its off.

#### set_init_buffers_in_advance_off()

This clears the init buffers in advance flag.

This is then used in an attach for the initialize of static buffers.

It is used also to initialize dynamic allocated buffers.

This is per default on for the fenster;plural systems.

On the posix systems its off.

#### get_init_buffers_in_advance()

This delivers the flag value.

####reuse_thread_buffers()

This is for killed threads. If you have the tid the module uses you can get
the  buffers back on the available list for reuse.

####verify()

This uses the atrshmlog_verify() module function to check for problems in the
shared memory buffers.

####get_clicktime()

This deliver the click time with the clock identified by clock id.

####read()

This fetches a buffer from the area into the process memory. Its the
now outdated reader backbone function.

You can use this to fetch buffers from the area, but its not recommended
to do it. Use the read_fetch instead.

####read_fetch()

This fetches a buffer from the area if any is used. If no buffer is used
it return with success and a length of 0 for the fetched buffer - no real
fetch is done, its only a test for the internal list and the returns.
Because the buffer is always get back you should alwys set it undefined
if you no longer need it. Its a one million bytes thing after all.

####create()

This creates an area. Its the backbone function for the atrshmlogcreate program.

####delete()

This deletes an area. Its the backbone function of the atrshmlogdelete program.

On the mingw based systems its only a dummy, the OS uses a reference counting
and so deletes the buffer when no process is attached any more itself.

####cleanup_locks()

This is the backbone function of the atrshmlogfinish program. It gives any
resources back to the system before you destroy an area with the delete call. 

#### init_shm_log()

This is the initialization of an area. Its the backbone function for the
atrshmloginit program.

It acquires the needed resources that are used inside the area (whatever
that is on the platform).

####peek()

We can check a byte in the area with this.


####poke()

We can set a byte in the area with this.

How to use it
-------------

Load the atrshmlog module.


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

###start the test in test/t_write.rb

You can now start the test. Its simple. Execute it..

To help you there is a shell script to do it.

     $   test/t_write.rb

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

Open that file in a text editor and check against the main in
the t_write.rb.

How to build it
---------------

Copy the  c library and the two headers to the src directory.

     $ cd BASEDIR/ruby ; bin/getfrommain.sh

Switch to your ruby location.

     $ cd atrshmlog # naming according to gem example not src
     
Set the needed ruby home with the

     $ . dot.ruby.sh


If you use a different ruby version you have at least to adjust
the content of this file.

Also the C compiler headers have to be in here - or use /usr/include

There is support for rake - see the Rakefile : \n

     $ rake build


If you dont want to use rake its manual this way.

Change to the ext and then atrshmlog. Run the extconf.rb for generating
the make file.

     $ cd ext/atrshmlog
     $ ruby extconf.rb
     
Compile the ruby library.

     $  make


Tests
-----

There is a module test in test.

Simply execute the test files.

They cover the way from ruby to the C function. The function itself is tested
in the C module, so no checks here.

If you need example code then this is for you the place.

Summary
-------


This is a fast path intro, so most stuff that you do not need
in the first place is not here.  See the doc for it.

