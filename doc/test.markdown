PROCESS : Test the module.   {#process_testmodule}
==========================

The test is done manually.

You have to execute the programs that were created in the build.

So here is the procedure step by step.


atrshmlogcreate
---------------

You have to create a shared memory buffer.

Execute the atrshmlogcreate. First give no parameters.

     $ atrshmlogcreate

The output should be a error message like this:

     usage: atrshmlogcreate ipckey32bit count_of_buffers
     logsystem version is 1.

As you can see the output delivers a short usage text.

Now use the two parameters. For the count its easy, we start
with 8 buffers here. For the key you have in principle first
to check if it is in use with a tool like ipcs. We assume the key
4711 is not in use.

     $ atrshmlogcreate 4711 8   # use 1 and 8 for mingw

This should give you this output - the number for ID can change ...

     shm log create.
     logsystem version is 1.
     ipckey is 0X1267
     count of buffer is 8
     
     paste this into the user process environment startshell
     or into the profile or ENV file
     
     
     ATRSHMLOG_ID="1835018"
     export ATRSHMLOG_ID
     
     if you have to use a login shell you can use the files instead.
     see atrshmlog_attach for this


So now we should have a shared memory buffer. Check that with your
os tool, namely ipcs.

You get also a dot file - dot.atrshmlog in case you dont give
a third parameter as name. For mingw its a atrshmlog.cmd file.

Source this in.

     $ . dot.atrshmlog # or execute for mingw CMD > atrshmlog.cmd


The nice thing is to have the output for the setting of the environment
variable in place.

So you can cut & paste that. Or use a wrapper script and reroute it in
a dot file.... or use the undercover created dot file from the
program

     $ . dot.atrshmlog    # or atrshmlog.cmd on fenster;plural mingw port

We set the things old school in our shell:

     ATRSHMLOG_ID="1835018"
     export ATRSHMLOG_ID
     

Try to run the create again.

     shm log create.
     logsystem version is 1.
     ipckey is 0X1267
     count of buffer is 8
     error -144 , errno 17.

Ok. This gives an error. Check for the code in the error helper
atrshmlogerror. Should be a is already existing thing...

atrshmloginit
-------------

Next thing after the create of the shared memory is the init.

Execute the program

     $ atrshmloginit

This gives the output ( but only if ATRSHMLOG_COUNT is NOT set ... ) 

     usage: atrshmloginit countbuffer
     parameter count wrong.
     logsystem version is 1.

We can now execute with the count, in our case 8.

     $ atrshmloginit 8

This time we get the output

     shm log attach and init.
     logsystem version is 1.


Now run it again. This time the output is

     shm log attach and init.
     logsystem version is 1.
     init error -152. errno 0
     


We check the error code with the helper script for errors.

     $ atrshmlogerror -152

Which gives us

      code -152
      atrshmlog_error_init_shm_2
      You tried to initialize an already initialized area.

Ok, this seem to be sufficient.

atrshmlogverify
---------------

Now we can use the check program.

Execute it.

     $ atrshmlogverify

The output is

     usage: atrshmlogverify count_of_buffers
     parameter count wrong.
     logsystem version is 1.

Now again with the parameter count of buffers, in our case 8.

     $ atrshmlogverify 8

The output is

     shm log attach and verify.
     logsystem version is 1.
     ich habe fertig is 0 at 28
     buffer 0 : size is 0, limit 524288, state is writeable
     buffer 1 : size is 0, limit 524288, state is writeable
     buffer 2 : size is 0, limit 524288, state is writeable
     buffer 3 : size is 0, limit 524288, state is writeable
     buffer 4 : size is 0, limit 524288, state is writeable
     buffer 5 : size is 0, limit 524288, state is writeable
     buffer 6 : size is 0, limit 524288, state is writeable
     buffer 7 : size is 0, limit 524288, state is writeable
     

So we have a valid area.

atrshmlogdump
-------------

Now we can dump the area.

Execute the dump program

     $ atrshmlogdump

The output is

     usage: atrshmlogdump countbuffer
     argument count is wrong.
     logsystem version is 1.

Now we give it the number of buffers

     $ atrshmlogdump 8

The output is

     shm log attach and dump 8.
     logsystem version is 1.
     buffer written to file ./buffer.bin ...

and we have a dump file in the directory

     $ ls -l buffer.bin

which is

     -rw-rw-r--. 1 shmlog shmlog 4195712 30. Sep 11:51 buffer.bin

You can now use your favorite bin to text thing to see what you got.
For a start od -h or xxd or your favorite editors hexmode.

atrshmlogoff
------------

This program switches a system wide flag to no more logging.

This ends the readers - (no, not now. only the old onces ...).

The writers no longer log.

You execute the program

     $ atrshmlogoff

and get this output


     shm log attach and set ich_habe_fertig to 1.
     logsystem version is 1.
     ich_habe_fertig was before 0



atrshmlogon
-----------
This program switches a system wide flag to again do logging.

This lets writers again do the logging.

You execute the program

     $ atrshmlogon

and get this output

     shm log attach and set ich_habe_fertig to 0.
     logsystem version is 1.
     ich_habe_fertig was before 1



atrshmlogdefect
---------------
The program has two modes.

The modes depend on the number of parameters. In the first mode you have
any number but two parameters.

The program then writes out some structural info and exit.

You execute it for example like this

     $  atrshmlogdefect

And you get a lot output.

     usage: atrshmlogdefect index_buffer newbytevalue
     parameter count wrong.
     index_buffer in bytes from start.
     newbytevalue 0 .. 255
     logsystem version is 1.
     info
              
     sizeof atrshmlog_internal_time_t  16
     sizeof atrshmlog_time_t           8
     sizeof atrshmlog_pid_t            8
     sizeof atrshmlog_tid_t            8
     sizeof atrshmlog_time_t           8
     sizeof atrshmlog_int32_t          4
     sizeof atr_shm_log_buffer_t       136
     sizeof atr_shm_log_area_t         336
     sizeof time_t                     8
     sizeof atomic_int                 4 
     sizeof atomic_flag                1 
     sizeof atomic_intptr_t            8 
     sizeof time_t                     8 
     transferbufferheader size        29 
     argv buffer max len             : 65536
     
     offsets
     version         : 0
     shmid           : 4
     safeguard       : 8
     count           : 24
     ich_habe_fertig : 28
     reader flag     : 32
     reader pid      : 40
     writer flag     : 48
     writer pid      : 56
     
     info
     count buffer : 8
     
     buffer 0 offsets
       data       : 1152
       state      : 64
       size       : 68
       info       : 192
       safeguard  : 184
     
     buffer 1 offsets
       data       : 525440
       state      : 200
       size       : 204
       info       : 328
       safeguard  : 320
     
     buffer 2 offsets
       data       : 1049728
       state      : 336
       size       : 340
       info       : 464
       safeguard  : 456
     
     buffer 3 offsets
       data       : 1574016
       state      : 472
       size       : 476
       info       : 600
       safeguard  : 592
     
     buffer 4 offsets
       data       : 2098304
       state      : 608
       size       : 612
       info       : 736
       safeguard  : 728
     
     buffer 5 offsets
       data       : 2622592
       state      : 744
       size       : 748
       info       : 872
       safeguard  : 864
     
     buffer 6 offsets
       data       : 3146880
       state      : 880
       size       : 884
       info       : 1008
       safeguard  : 1000
     
     buffer 7 offsets
       data       : 3671168
       state      : 1016
       size       : 1020
       info       : 1144
       safeguard  : 1136
     
     totalsize (see dump file size in atrshmlogdump) : 4195456


Thats a lot info about sizes and offsets in the shared memory.

In the second form you can change a byte in shared memory. For this
you have to give the offset and the new value in decimal as parameters.

     $ atrshmlogdefect 0 42

You get

     logsystem version is 1.
     shm log defekt setting.
     before : 1 , now 42 

Use the verify to see for the version now.

     shm log attach and verify.
     logsystem version is 1.
     verify error -184. errno 0
     ich habe fertig is 0 at 28
     buffer 0 : size is 14459, limit 524288, state is full
     buffer 1 : size is 14459, limit 524288, state is full
     buffer 2 : size is 0, limit 524288, state is writeable
     buffer 3 : size is 0, limit 524288, state is writeable
     buffer 4 : size is 0, limit 524288, state is writeable
     buffer 5 : size is 0, limit 524288, state is writeable
     buffer 6 : size is 0, limit 524288, state is writeable
     buffer 7 : size is 0, limit 524288, state is writeable

You get a -184 which is atrshmlog_error_verify_4, wrong version number.

you can set this back with

     $ atrshmlogdefect 0 1


that gives

     logsystem version is 1.
     shm log defekt setting.
     before : 42 , now 1 



atrshmlogreset
--------------

You can repair a somehow corrupted shared memory buffer.

To do this you execute

     $ atrshmlogreset 8

If you dont know the correct size try the verify to get it first.

You will get info if something is wrong and what was done to fix it.



atrshmlogreaderd
----------------

You need to transfer the log after it has placed in the
shared memory into the filesystem. This is done by the so called readers.
The reader d is the production reader for now.
It uses a multithread approch. See the doc for the motivation
and the adjustments you should make.
The reader d uses two environment variables and up to three parameters.

You execute it this way

     $ atrshmlogreaderd d1

This will start the reader and give the following output:

     shm log attach and loop write file.
     logsystem version is 1.
     directory is d1
     files_per_dir is 10000
     verion is 42, not as needed 1.

Ups, there was a former test with a defect ...

Again :

     shm log attach and loop write file.
     logsystem version is 1.
     directory is d1
     files_per_dir is 10000
     count of initial used fetchers is 4
     count of initial used writes is 12
     enter transferloop ... 

So far, so good. The thing is now in a loop and does checking the
bufferlist in the shared memory.

If any buffer is there it is transfered into process memory and
later written down to filesystem.

The process uses itself a set of fetchers. They fetch a buffer from the
shared memory full list and transfer it to the internal list of buffers.
Then they give the buffer to the shared memory available list and go to sleep.

The buffers on the internal list are taken by the writers. They remove
the buffer from the internal list and then write it down to the file system.
After that the buffer is pushed on the available list in the reader.
Then the writer goes to sleep.

Dont confuse here. There are in total three lists in the process:
- the available list (empty buffers)
- the internal list (full list)
- the sequence list (all buffers)

The shared memory has two lists:
- the available list (empty buffers)
- the full list ( full buffers)

The writer has itself again three lists:
- the available list (empty buffers)
- the full list (full buffers)
- the sequence list (all buffers)

Try to realize this and then read again if you were confused.

As you can see this is a lot multithreading and its not complete, but
it works for now.

When you set the system wide flag to no more logging the reader stops.
This will be changed in a later version.

So for this version we stop the thing now.

     $ atrshmlogstopreader

and the reader writes out

     logging stopped by ich_habe_fertig ...
     logging done.
     writer data : time 4247660  count 2  per use 2123830

So you have now the time the process needed to write buffers, the
number of buffers and the time per buffer, all in clicks.

In the working directory you have now a new tree, starting
with _d1_ directory (the parameter for the reader that must be given).

The next is a layer of at least 5 directories, starting with _0_ .

In the directories there are the binary files from the transfer.

     $ ls -l d1/0

gives us

     -rw-------. 1 shmlog shmlog  16171 28. Sep 19:41 atrshmlog_p1232_t1232_f0.bin
     -rw-rw-r--. 1 shmlog shmlog  59180 29. Sep 16:33 atrshmlog_p1232_t1232_f0.txt
     -rw-------. 1 shmlog shmlog  14971 30. Sep 17:59 atrshmlog_p3217_t3217_f0.bin
     -rw-------. 1 shmlog shmlog  14971 30. Sep 17:59 atrshmlog_p4051_t4051_f1.bin
     -rw-------. 1 shmlog shmlog  57071 29. Sep 16:33 atrshmlog_p4091_t4091_f0.bin
     -rw-rw-r--. 1 shmlog shmlog 100080 29. Sep 16:33 atrshmlog_p4091_t4091_f0.txt

As you can see there is already some old stuff and the new of our
atrshmlogdefect tries...


atrshmlogtest01
---------------

This is the simple test program.

You execute it this way

     $ atrshmlogtest01

You can give additional parameters.

The program then makes some logging.

The output is

     shm log attach and set ich_habe_fertig to 1. parametercount 1
     done 32 write.
     after argv
     non logging
     logging
     ich_habe_fertig was before 0
     atrshmlogstat 0 -114452926
     atrshmlogstat 1 3445
     atrshmlogstat 2 1
     atrshmlogstat 3 0
     atrshmlogstat 4 0
     atrshmlogstat 5 2
     atrshmlogstat 6 0
     atrshmlogstat 7 0
     atrshmlogstat 8 0
     atrshmlogstat 9 0
     atrshmlogstat 10 1
     atrshmlogstat 11 0
     atrshmlogstat 12 0
     atrshmlogstat 13 0
     atrshmlogstat 14 0
     atrshmlogstat 15 0
     atrshmlogstat 16 0
     atrshmlogstat 17 0
     atrshmlogstat 18 0
     atrshmlogstat 19 0
     atrshmlogstat 20 0
     atrshmlogstat 21 0
     atrshmlogstat 22 0
     atrshmlogstat 23 0
     atrshmlogstat 24 38
     atrshmlogstat 25 0
     atrshmlogstat 26 0
     atrshmlogstat 27 0
     atrshmlogstat 28 0
     atrshmlogstat 29 0
     atrshmlogstat 30 0
     atrshmlogstat 31 0
     atrshmlogstat 32 0
     atrshmlogstat 33 0
     atrshmlogstat 34 0
     atrshmlogstat 35 0
     atrshmlogstat 36 0
     atrshmlogstat 37 3
     atrshmlogstat 38 0
     atrshmlogstat 39 0
     atrshmlogstat 40 0
     atrshmlogstat 41 0
     atrshmlogstat 42 0
     atrshmlogstat 43 0
     atrshmlogstat 44 0
     atrshmlogstat 45 0
     atrshmlogstat 46 0
     atrshmlogstat 47 0
     atrshmlogstat 48 0
     atrshmlogstat 49 0
     atrshmlogstat 50 0
     atrshmlogstat 51 0
     atrshmlogstat 52 0
     atrshmlogstat 53 0
     atrshmlogstat 54 0
     atrshmlogstat 55 0
     atrshmlogstat 56 0
     atrshmlogstat 57 0
     atrshmlogstat 58 0
     atrshmlogstat 59 0
     atrshmlogstat 60 0
     atrshmlogstat 61 0
     atrshmlogstat 62 0
     atrshmlogstat 63 0
     atrshmlogstat 64 0
     atrshmlogstat 65 0
     atrshmlogstat 66 0
     atrshmlogstat 67 0
     atrshmlogstat 68 0
     atrshmlogstat 69 0
     atrshmlogstat 70 0
     atrshmlogstat 71 0
     atrshmlogstat 72 0
     atrshmlogstat 73 2
     atrshmlogstat 74 1
     atrshmlogstat 75 0
     atrshmlogstat 76 0
     atrshmlogstat 77 0
     atrshmlogstat 78 0
     atrshmlogstat 79 0
     atrshmlogstat 80 0
     atrshmlogstat 81 0
     

Thats a statistic print out at the end. See atrshmlog_counter for the
meaning of the numbers.

atrshmlogtest02
---------------

This is a second test program. Its a simple test for C++.
You know already that we use a inline approch for the click counters.
So this was the test if this works even with the C++ compiler.

The execute is

     $ atrshmlogtest02

The output is

     shm log check for c++ code 
     check error 132

Ups, that was a atrshmlogoff before.

Again:

     shm log check for c++ code 
      end of test.


atrshmlogtest03
---------------

This is the development test program. It uses a C++ code and is doing
a multithread test.

You give three parameters. The first is for the count of threads.
I did one time 16384 on my big boxes for that.
On the small its more like 1 to 8.

The second parameter is the count of loops in a thread. The loop contains
a gettime, a internal loop of counting an int value, a second get time
and the log call.

The third parameter is the count for the internal loop that counts an int.

So you can make the thread doing some thing from  16 to a billion counts
between the two get time.

This is a simple simulation of a highly cpu oriented test szenario.

The program then makes one thread for the total of loops by parameter one and two and so it gives a reference for a single thread system.

You can start it like this

     $ atrshmlogtest03 2 1024000 16

the output is then

     
     shm log check for c++ code and threads
     start of threads.
     dispatcher has count 2 and looptime 1024000
     packages made 
     0 160652299
     1 217040527
     threads delivered futures 377692826 217040527
     main 231332200
     1.63269 1.06585
     end of test.
     atrshmlogstat 0 -1078897676
     atrshmlogstat 1 4085
     atrshmlogstat 2 1
     atrshmlogstat 3 0
     atrshmlogstat 4 0
     atrshmlogstat 5 6
     atrshmlogstat 6 312
     atrshmlogstat 7 312
     atrshmlogstat 8 312
     atrshmlogstat 9 0
     atrshmlogstat 10 1
     atrshmlogstat 11 0
     atrshmlogstat 12 0
     atrshmlogstat 13 0
     atrshmlogstat 14 0
     atrshmlogstat 15 0
     atrshmlogstat 16 0
     atrshmlogstat 17 0
     atrshmlogstat 18 0
     atrshmlogstat 19 0
     atrshmlogstat 20 0
     atrshmlogstat 21 0
     atrshmlogstat 22 0
     atrshmlogstat 23 0
     atrshmlogstat 24 4096064
     atrshmlogstat 25 0
     atrshmlogstat 26 0
     atrshmlogstat 27 0
     atrshmlogstat 28 0
     atrshmlogstat 29 0
     atrshmlogstat 30 60
     atrshmlogstat 31 0
     atrshmlogstat 32 0
     atrshmlogstat 33 0
     atrshmlogstat 34 0
     atrshmlogstat 35 0
     atrshmlogstat 36 0
     atrshmlogstat 37 0
     atrshmlogstat 38 0
     atrshmlogstat 39 0
     atrshmlogstat 40 0
     atrshmlogstat 41 0
     atrshmlogstat 42 0
     atrshmlogstat 43 0
     atrshmlogstat 44 0
     atrshmlogstat 45 0
     atrshmlogstat 46 0
     atrshmlogstat 47 0
     atrshmlogstat 48 0
     atrshmlogstat 49 0
     atrshmlogstat 50 0
     atrshmlogstat 51 0
     atrshmlogstat 52 0
     atrshmlogstat 53 0
     atrshmlogstat 54 0
     atrshmlogstat 55 0
     atrshmlogstat 56 0
     atrshmlogstat 57 0
     atrshmlogstat 58 0
     atrshmlogstat 59 0
     atrshmlogstat 60 0
     atrshmlogstat 61 0
     atrshmlogstat 62 0
     atrshmlogstat 63 0
     atrshmlogstat 64 0
     atrshmlogstat 65 0
     atrshmlogstat 66 0
     atrshmlogstat 67 0
     atrshmlogstat 68 0
     atrshmlogstat 69 0
     atrshmlogstat 70 0
     atrshmlogstat 71 0
     atrshmlogstat 72 0
     atrshmlogstat 73 1
     atrshmlogstat 74 0
     atrshmlogstat 75 0
     atrshmlogstat 76 0
     atrshmlogstat 77 0
     atrshmlogstat 78 0
     atrshmlogstat 79 0
     atrshmlogstat 80 0
     atrshmlogstat 81 0
     
You can see that my box is far from scaling.

The two threads have
- 0 160652299
- 1 217040527

time used (in nanoseconds), while the single thread used 231332200 nanoseconds

For the statistics there is the sum of the threads against the time of the
single - in this case  1.63269. And there is the ratio of the slowest
thread in multithread against the single, 1.06585.

Doing this on a scaling system looks a bit different.

     shm log check for c++ code and threads
     start of threads.
     dispatcher has count 4 and looptime 512000
     packages made 
     0 74101919
     1 74091216
     2 76197152
     3 70734911
     threads delivered futures 295125198 76197152
     main 271097998
     1.08863 3.55785
     end of test.

I spare us the statistcs.

So you can also check for the degree of speed up on your system.
In case of the big box it is a factor 3.55 faster to use 4 threads.
The total cpu cost is 1.08 of the single run, so we got it really
cheap here.

Two things must be said here.

The reader did only transfer to shm, no write till it ended.
The statistics were shut off - they are a problem in a high load
szenario.
Update: Statistics are no no longer a problem, so they are now on.

atrshmlogtee
------------

This program is a do it like demo for the tee program.

You use it like this

     $ atrshmlogtee < sourcefile > targetfile

The Log contains the sourcefile in pices of 40 byte lines.



atrshmlogfork
-------------

This program tests a clone working with the log. You get a clone when
you fork a process, but not overlay it then with exec.

The program is more or less a copy of verify. So it verify the buffer
structure two times, one in the parent , one in child.

It then uses the fields for communication in the shared memory
to play a small ping pong conversation via shared memory.

You execute the prgram like this

     $ atrshmlogfork 8

The output is

     shm log attach and verify.
     logsystem version is 1.
     child still alive 
     3833 buffer 0 : size is 0, limit 524288
     3833 buffer 1 : size is 0, limit 524288
     3833 buffer 2 : size is 0, limit 524288
     3833 buffer 3 : size is 0, limit 524288
     3833 buffer 4 : size is 0, limit 524288
     3833 buffer 5 : size is 0, limit 524288
     3833 buffer 6 : size is 0, limit 524288
     3833 buffer 7 : size is 0, limit 524288
     sended to client 3833.
     father still alive 
     0 buffer 0 : size is 0, limit 524288
     0 buffer 1 : size is 0, limit 524288
     0 buffer 2 : size is 0, limit 524288
     0 buffer 3 : size is 0, limit 524288
     0 buffer 4 : size is 0, limit 524288
     0 buffer 5 : size is 0, limit 524288
     0 buffer 6 : size is 0, limit 524288
     0 buffer 7 : size is 0, limit 524288
     father has pid 3831
     got from father 4711
     child still alive 
     father still alive 



atrshmlogforkwrite
-------------

This program  tests a clone working with the log. You get a clone when
you fork a process, but not overlay it then with exec.

The program is more or less a copy of verify. So it verify the buffer
structure two times, one in the parent , one in child.

It then uses the fields for communication in the shared memory
to play a small ping pong conversation via shared memory.

It also executes the write function in child and parent. So the
restart of slave threads can be tested ans the use of
the atexit function.

You execute the program like this

     $ atrshmlogforkwrite 8

The output is same as for atrshmlogfork, only a pstree is used
while the process is up to show the running threads of the box.

So its best to direct the output to a file and then check it.



atrshmlogfinish
---------------

This program clears all resources attached to the shared memory,
i.e. mutexes and condition variables.

So it it the counterpart to init.

You execute it like this

     $ atrshmlogfinish  8

The output is then

     shm log attach and finish mutexe.
     logsystem version is 1.

atrshmlogdelete
---------------

Teh program removes the shared memory buffer and gives it back to the system.

It is the counterpart to create.

You execute it like this

     $ atrshmlogdelete $ATRSHMLOG_ID



the output is


     shm log delete.
     shmid is 1572870
     logsystem version is 1.
     
     
     unset ATRSHMLOG_ID


So you can no longer use the buffer.

The ID is used, not the key, so dont mess things up in your
usage. The key is the parameter for the creatte, the ID for the delete.


atrshmlogconvert
----------------

This program is the converter for binary files from the reader to
a text file.

Optional you can get the thread statistic into a second file as parameter 3.

The convert uses a simple approch to write the payload out.
For now its scii 8 bit, with a 0 last char of line.

There is a hack for 16 bit ucs2 java strings, so you can use them with
the event flag p or i and the convert will try a convert to utf here.

Any other binary must be extracted by a customized convert.
See for this the doc.

The convert depends on the version of the log. So dont try a bin file from
a diffrent version.

The convert writes the data in a line oriented way and calculates some
additional values. So its a lot more data than the binary.

For the version 1 it writes the following columns:
- Process id pid
- Thread Id tid
- Buffer number
- File id number
- Start time in clicks
- End time in clicks
- Delta of start and end time
- Start time in realtime nanoseconds
- End time in  realtime nanoseconds
- Delta for start and end realtime
- Eventnumber
- Eventflag
- User event number
- Payload (C string or in case of event flag p or i converted ucs2)

You get from the convert an output that has some values in the output.

You execute the convert like this

     $ atrshmlogconvert d1/0/atrshmlog_p3944_t3944_f0.bin atrshmlog_p3944_t3944_f0.txt

You get the following output


     shm log converter from file 'd1/0/atrshmlog_p3944_t3944_f0.bin' to file 'atrshmlog_p3944_t3944_f0.txt'.
     id    1 acquiretime      190 pid   3944 tid     3944 slavetime     15530 readertime     21240 payloadsize      29 shmbuffer   0 filenumber       0 

The numbers in the second line are:
- id of the buffer in process
- acquire time for the buffer in clicks
- process id of the writer
- thread id of the writer
- transfertime from memory in writer to shared memory (slavetime)
- transfertime from shared memory to reader memory (readertime)
- size of the payload in bytes
- number of the used shared memory buffer
- number of the used file for the reader

Those numbers have all a meaning in the adjustment of the module for
your system, so be sure you understand them before you switch things
in the module. See the doc for this.

atrshmlogconv
-------------

This is a script that uses the convert for the given directory
tree starting with the parameter as directory.

So you dont have to call the convert manually - you call the conv
instead for the tree and the related txt files are made for all bin files.

Be sure not to have wrong bin files in the tree.

The naming convention of the reader is used to find the bin files,
so you have at least a bit safety.

The output is the output for the called convert for every file found  to
be converted.

You execute the script like this

     $ atrshmlogconv d1




atrshmlogsignalreader
---------------------

This program reads the shared memory reader fields and then sets them
with the parameters. You need the parameter pid and flag.

The program makes use of the two fields for communication to readers
attached to the shared memory.

This will be the mechanism to stop readers in the next release.

For now it simply sets the values.

You execute it like this

     $ atrshmlogsignalreader 1234 1

The program writes the output

     shm log attach and set reader flag and pid.
     logsystem version is 1.
     pid  before 0
     flag before 0

Its then up to a reader to act according to the numbers.

See the fork programs for a way that could be used.



atrshmlogsignalwriter
---------------------

This program reads the shared memory writer fields and then sets them
with the parameters. You need the parameter pid and flag.

The program makes use of the two fields for communication to writers
attached to the shared memory.

For now it simply sets the values.

You execute it like this

     $ atrshmlogsignalwriter 1234 1

The program writes the output

     shm log attach and set writer flag and pid.
     logsystem version is 1.
     pid  before 0
     flag before 0

Its then up to a writer to act according to the numbers.

See the fork programs for a way that could be used.



atrshmlogcalc
-------------

This is a helper script that calculates the statistics from the
converted text info files.

The script is written in perl.

You have to install a perl 5 to do it. No modules needed.

The script has special info fr the mksh version with logging.

Its a bit outdated, so you must make some adjustments to it
before you can use it again. In a later version it will be
updated.

The script reads in the files given as parameters. So you can
in principle give any number of files to it.
It builds an internal index so that you have not to use sorted input.
This means you have not to sort the files in advance, the script
does that by itself (which costs a lot of runtime ).

For the use see the page for the mksh use case.

atrshmlogsort
-------------

The script sorts for a directory tree starting with the directory as parameter
the contained text files after convert run.

The files are used per directory, so you get one resulting file with the
name prot.txt in the directory with all data of the text files.

The sort uses the sort program. see for the script and the sort man page.

you execute the script like this

     $ atrshmlogsort d1



atrshmlogerror
--------------

The script gets a list of error numbers and prints out the corresponding
info about the errors.

You need to have perl installed for it.

You execute it like this

     $ atrshmlogerror -184 17 -42


The output is

     error -184:
     enum is atrshmlog_error_verify_4 
     Version mismatch for area and module code in atrshmlog_verify. 
     no error found for 17 in map.
     error -42:
     enum is atrshmlog_error_attach_3 
     The cleanup could not sucessful made atexit in atrshmlog_attach. 


atrshmlogstatistic
------------------

The script gets a statistics data list as input as a file in parameter.
It then parses the file and extracts the statistic info.
It then writes out a more readable report for that statistics.

You execute it like this

     $ atrshmlogstatistic outputfile

The outputfile was created by a atrshmlogtest03 before.

The resulting output is commented.


atrshmlogreader
---------------

This program was the first reader for the module. It is now obsolet.
Its a demo for how to write a reader in the first place.

In a later version it will be rewritten to do the transfer and write again
on top of the used buffer list mechanism.

You can ignore it for now.

atrshmlogreaderb
---------------

This program was the second reader for the module. It is now obsolet.
Its a demo howto read from the shared memory, but uses only internal
buffering for the data fetched from shared memory. When it stops
it writes out the data.

In a later version it will be rewritten to do the transfer and write again
on top of the used buffer list mechanism.

You can ignore it for now.

atrshmlogreaderc
----------------

This program was the third reader for the module. It is now obsolet.
Its a demo howto read from the shared memory, but has been replaced with
the actual reader version, reader d.


In a later version it will be rewritten to do the transfer and write again
on top of the used buffer list mechanism.

You can ignore it for now.



Summery
-------

You make the module with your settings for the defines and defaults.

Then you test the module by use of the programs and scripts.

The workflow is a bit different from the usual test cycle.

You execute more than the necessary programs. So you should take the time
and get familiar with the use on the command line.

After that you can use them in your scriptings / testings framework.

So understand them before you use them.

If you get any problems with the testing its best to start with a clear
new buffer.

So making them is not really a problem.

The problem is to have all acting shells up to date for the
environment variables, namely the ATRSHMLOG_ID.

This is vital, so simply try not to use multiple buffers in the start.

If you get any strange behaviour you should switch the optimization
in the build to debugging mode, switch the inline of code off and
start to inspect if you need it with your best friend, the debugger.

Making a version with ATRSHMLOGDEBUG set t 1 is an alternative,
check then for the additional output.

