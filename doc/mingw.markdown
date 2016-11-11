PLATFORM: The cygwin mingw approch.   {#system_cygwin_x86_64_mingw_gnu}
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
cygwin-devel, binutils, doxygen, gcc-core, gcc-g++, gdb, the mingw stuff for x86_64 and i686.
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
We use native windows, so no need for that.


Building the module
-------------------

Ok. this was the third platform that i had to support.

So i made the new platform - you can read this
in [new platform](\ref process_adjustthemodule).

All in all it was simply copy the cygwin, replace the cygwin then
with mingw and
at last a rerun of makeall.

All other things were there out of the box, so i got the make done, the
documentation was cut off, its already there with a cygwin build.
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

After this I made the create, the init, the small test and then the

    CMD > start_package_log.cmd

test. All worked.

So you have to use in the CMD after that the path with setpath.cmd in
the bin and then the rest is up to the settings in start_package_log.cmd.

For now thats all for me to say about the java on fenster;plural thing.




