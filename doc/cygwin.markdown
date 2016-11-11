PLATFORM: The cygwin approch.   {#system_cygwin_x86_64_gnu}
===================

The fenster;plural system does not support the posix way
as the unixes. There is a somehow minimal support, for
example the file io interface for read write create etc.
is given ( with a little syntactic sugar).

But when it comes to C11 they are off (at least my knowledge in 10.2016).

So there is at least one way you can try.

Download cygwin and then start to use posix on your fenster;plural box.

Download
--------

You have to download not the full distro.
First check the cygwin master at http://www.cygwin.com for
the latest download binary.

Load it on the box.

Then start it and give the permissions the thing needs.

After this you have to select some directorys and places and then the
checking for download packages starts.

What to download
----------------
There is some stuff that is needed, but its only a small fraction.

- BASE\n
The base package of course. All in.
- Admin\n
The cygrunsrv. This is vital because you wont get shared memory
if you are not having it up.
This was formerly the cygserver and in BASE. Dont know why but now
the script to configure is in  BASE and its cygserver-config, but
the main binary has been moved ( at least in the version i got in 10.2016).
- Devel\n
cygwin-devel, binutils, doxygen, gcc-core, gcc-g++, gdb, optional mingw
- doc\n
cygwin-doc
- editors\n
emacs, vim
- interpreters\n
perl
- libs\n
all in.
- shells\n
bash
- utils\n
dos2unix
- publishing\n
texinfo
doxygen

That should do the job.

Cygwinserver
------------
That was a tough one.

I found only outdated info via google, so here is my version (10.2016).
You have to download the ADMIN or at least the cygrunsrv package.
You then start a terminal AS ADMINISTRATOR ( meaning you act like it) and
do it by right click on the terminal icon and then as administrator thing.
(By the way: i use a win 7 64 bit, so i HAVE a mouse and can do the
right button thing. And if you have another version i will not try to figure
out how you have to do that. Side note: My notebooks CPU is a core 2 duo,
and so my win 7 refused the win10 upgrade - the guys at winzigweich did the
download of the win10 upgrade starter and when i clicked it it did the
download thing. But when it came to the implementation the thing told me
that my cpu was not supported. Funny. Can make linux and oracle and big
java appservers, but for the simple best in all time win10 system its not
enough power ... what a big resource eater that win10 thing must be
if it beats even an oracle 12 db ....)

Oh, right. We were in the started administrator rights terminal.

Now we have to call the cygserver-config.

It asks about a config file, so do it the first time with a fresh one.
Then it asks to implement a service. This is the major point.
You need a service - aka backgroundserver - to do the ipc things.
The calls at posix level simply result in an error without it.
So you need not only the cygwin dll file - thats trivial after all - but also
an up and running service of the cygserver - so far the outdated docu ...

I got an error that a program cygrunsrv were missing, and after locating it
i downloaded that too. This made then in the second try the service.

Now i startet the server on the command line as given in the --help output.

After that my ipc functions work as usual - its funny to think about it
for the shared memory, but for now i simply use it and it works.

Building the module
-------------------

Ok. this was the second platform that i had to support.
So i made the new platform - you can read this
in [new platform](\ref process_adjustthemodule).

All in all it was simply copy the linux, replace the linux then
with cygwin and
at last a rerun of makeall.

All other things were there out of the box, so i got the make done, the
documentation was made by doxygen and i had a very easy start with the
first testdrive.

Building the java jni
---------------------

Ok. First things first. Download the jdk.
The module uses as a reference the jdk 1.8.0 build 66.
The jdk on this time ( 10.2016) was the 64 bit version of jdk 1.8.0 build 102.

So first create a new java subdirectory, this is oracle for the supplier.
Ok, that said its already there.
Change into it and then create a jdk1.8.0_102 directory (gag, its already there).
Change into the 1..8.0_66 directory.
Copy the stuff recursive into the new target for the jdk (gag, its already done).

I use here a tar cf - . | ( cd ../jdk1.8.0_102 && tar xf - ) line.

Then switch to the BASEDIR/java directory.

Execute the bin/getfrommain.sh to copy actual headers and library.

Now the files from the main directory are in place in all java directories.

Switch to the jdk 102 . ( This is a test if you can think in deductive ways -
i only give the jdk build and you swith to that ... hopefully).

Ok, for the others: switch to oracle, then jdk1.8.0_102.

Then switch to src, make an ls -l to check the timestamps and see for
the lib atrshmlog file.
Check then the includes with ls -l  includes and see for the atrshmlog.h
and the atrshmlog_internal.h.

When everything is in  place we can now change the dot.java.sh file.
Its still the path to the thing. So we have to do this her by making a copy
to dot.java102.sh.
Then edit the file.

For me i ended up in this :

     JAVA_HOME="/cygdrive/c/Program Files/Java/jdk1.8.0_102"

and

     JAVA_JNI_OS=win32

The rest is simple. Source the dot file. Then execute the create_jni_lib.sh.

Last thing is a test - but i have to say it wont work for me.

So i assume the cygwin is only a good self contained thing, not good
for corporation with others.

For the module working on fenster;plural switch to mingw.


