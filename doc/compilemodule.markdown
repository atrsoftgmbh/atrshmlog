PROCESS : Compile the module.   {#process_compilemodule}
==========================

You have to compile the module.

This is the major build step.

Compile with makeall
--------------------

We normally compile with makeall.

The script is in the BASEDIR/src directory.

You have to set up the platform with the dot file in the BASEDIR
for your platform.

This is setting some environment variables and also sets some defines
for the compile this way.

Open a shell.

Change to BASEDIR (the directory that contains the whole thing).

Change to src and check the atrshmlogchecksystem script output.

Then use the proper dot file in BASEDIR like this (example fedora linux) :

     $ . dot.platform.sh.fedora_linux


So you switch then to the src back.

Check the makeall is there :

     $ ls makeall.sh


If its missing control the check for completeness.

Something must have gone wrong there.

Start the script.

     $ makeall.sh

You can redirect with the usual

     $ makeall.sh > 1.log 2>&1


It takes on my box (Intel P9600 at 2.6 GHz and 8 GB ram with fedora 23
and virtualbox with win 7 64 bit):
- about 40 seconds on linux x86_64
- about 8 minutes on cygwin win 7
- about 12 minutes on cygwin win 7 for mingw x86_64 target

You can go through the output, its not so much if everything is right.

The makeall first makes the documentation. It calls doxygen for this.

If you have not installed doxygen you can comment the step in the
buddydoc like this.

     # doxygen


The  mingw does not do this step.


After the doxygen has done its job we have the compile for the
C source files.

This is a simple loop for all files in the shmcfiles list.
Any residing object is deleted and then the g99 script is
used to compile the source file.

So you need to have the g99 script in your path. The dot files do that
by themselfs. 

If this is not the case extend your path like this:\n

     PATH=$PATH:$(pwd):$(pwd)/../bin

This is not the best. But it works for now.

Then the compile switches to the impls directory.

There it simply compiles every c file.

After doing the job in the impls it switches back to the src.

If you have activate the database option you get done the dbs next.

This step is normally off.

The database needs at least the thing and the development part
in place. And in its full version it needs postgreSQL, mysql or mariadb,
the oracle 11 or later, the cassandra from apache.

So I have made this optional because I think you will not have
these in place. If you plan to use the database you have to check for the
build, adjust it, and then make the proper environment settings.

For oracle there is the unsupported folder with some helper scripts.

Then the library is made.

After the loop for the compile comes the loop for linking.

First the shmbin files, then the internals.

This loop uses the ell script.

Then the tests .

Last in tests is the loop for the C++ sources.

Again the files in the directory are compiled and it is the
g++14w script that does the job.

If you have the database option active the linking of the database
parts is done.

Compile manual
--------------

If you have to do it manual - or want to do it this way -
you can make the object / binary with a single call to the
script with the name of the file.

In case of the C++ its the source files name.

     $ g++14w.sh file.C


In case of the C its also the source file name.

     $ g99.sh file.c
     

In case of the linkage of c programs its the program name.

     $ ell.sh progname

The object has to be there from a former g99.sh run.


Build documentation
-------------------

You can build the documentation by running doxygen in the src
directory. Switch to the src directory. Open a shell.

Then start doxygen :

     $ doxygen


thats all.


Install
-------

After you have the compile done you can install the resulting artifacts.

For the install there is a simple copy of the programs
to the place you want to have them.
And you have to copy the headers and the lib.

This sounds not to much work, so i have not a install script
for now. But this will be done in one of the next releases.


For the  programs its like this

     $ cp $(cat shmallfiles) /usr/local/bin

For the headers its

     $ cp atrshmlog.h atrshmlog_internal.h /usr/local/include

For the library it like this

     $ cp libatrshmlogc.a /usr/local/lib


For the documentation you have to copy the subdirectories.

     $ cd doc; tar cf - html | ( cd /usr/local/doc && tar xf -)

You should create a target directory for the doc first and use this.


Deinstall
---------

The process is to delete manual the files that were copied in the
install process.
