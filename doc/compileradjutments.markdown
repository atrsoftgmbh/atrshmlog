PROCESS : Adjust for compiler.   {#process_adjustforcompiler}
==========================

For the compiler you need adjustments in these areas:
- Header defines for platform, tick counter inlines
- Source code for non inline platform code, tick couner functions
- Compiler scripts for g99, g++14w and ell

So here are some details for the things.

Compiler adjustments for headers
--------------------------------

Most of them are in the adjustment page.

This is a shortcut.

See ATRSHMLOG_PLATFORM_xxx defines ....

See ATRSHMLOG_INLINE_TSC_CODE define and the spots for the inline
functions.

See ATRSHMLOG_USE_C11_THREAD define and the compiler library.
The threads.h header is optional - so your compiler can - but not
must - support it.

The mostly used gnu compiler for me didnt ( Version 5.3.1, fedora 23,
september 2016).

So if yours do it - give it a try. And sent me a postcard if
it really works. Could not test that code ...

The stdint.h has to support at least a 32 bit int, a 64 bit uint, and
should have a signed char support.

So far for the headers....

STOP:\n
I forgot: the assembler code. You need this way to get the tick time.
See the <en.wikipedia.org> and then time stamp counter. Check
for your compiler if it can do the things in cycle.h (if its not on wikipedia,
check the outdated copy i have in doc) ....

Compiler adjustments for source
--------------------------------

Hm, thats nearly the same list, only in the atrshmlog.c file
and the impls directory.

Simply check the usages of the defines....

Compiler scripts g99 ell g++14w
------------------------------
This is the hard stuff.

The build uses a simple approch of a shell script makeall.

The script itself uses the list files to target the things
that must be done.

And for every entry there is one of the so called driver scripts.

For the C compile stuff its the g99. For the linking of C programs
its the ell. For the compile and link of the C++ programs its
the g++14w.

Beside this strange names its quite similar to have a makefile
and to put some rules together and then make a list of
targets.

Because i believe in todays compiler speed i didnt use that makefile
stuff.

The script approch is as good as a makefile if you only have to compile
one time.

Ok, in development i am compiling all the time, but ....
... i have made very bad experiance with makefiles and such things
like pre compiled headers or inline assembler codes.

So i have decided to simply trade in time for simplicity.

The script then simply uses a loop for all entries in the listfiles.

The tough stuff is in the driver scripts.

###The c compiler g99

This is a script that gets a c source file and delivers the object file.
Per convention the object file has the basename of the source, and the
end of the name is replaced to .o instead of whatever the source uses.

Here is the simplyfied gnu version for my linux box.

    #!/bin/bash
    
    
    gcc -std=gnu11 -fPIC -I. -O3 $* -c
    # gcc -std=gnu11 -fPIC -I. -g $* -c


...pretty simple.

Today i relay on the platform defines, perhaps i have to add flavour later on.

You need to check for a compiler for c11. Its at least for the
atomics stuff a c11 - eventualy a c99 will work with add ons.
If your compiler does not work - try a
different  and check for the linkage of the library to the normal
used compiler.

For the gnu gcc  i had to support the language standard - thats c11 - and
for the  extensions (at least i suspect i have to ) the gnu11 flag.

Ok. Thats simple enough.

The next thing was to play some time round and found that most
software today is so called position independent.

Makes the binary a bit slower, a bit more fat, but you can use
it in every szenario, even in the language layers for java
and that stuff.

So we need - NOT - -fPIC but it would slam doors closed for some very
interesting things.

Last, but not least, the inline code thing.

It only worked for
me with a -O switch. Thats not what i expected, but after some
funny session over linkage errors and some testing i found it worked
this way. So a -O is it, and i then switched to -O3. Do your
best my little friend...

So you need to check:
- do i have a c compiler ?
- does it support c11 ? At least c99 ?
- does it support atomics - they are optional (!) even in c11...
- do i have linkage with the libraries on my system ?
- do i have inline code ?
- do i have the assembler inlined for the tick timer thing ?

and then you are here.


###The c linker ell
This script is for linking the module to the program object.

So in my case it was in the early development more than only
one object.

It uses the library to link.

So here is my simplified version:\n

    #!/bin/bash
    
    P=${1%%.o}
    
    shift
    
    gcc -L. -pthread ${P}.o $* -latrshmlog -o $P


... again, pretty simple.


The script takes the first name and extracts any object file
extension. Thats the P line.

Then i start the compiler again with the needed flags, the
object for the program, the rest of the arguments - these were
the former needed additional objects - and the module.
The result is the program, this time without any extension.

So you have to check if your compiler can act as a linker.
If thats the case you have to check for some hidden parameters
in case you try to link c11 - or here gnu11 - code.
That was the -pthread parameter in case of my gnu gcc.

The rest is relative universal, give it all objects and
tell it the right names. And of course the library.

###C++ compiler and link g++14w
Ok, this was not really needed, but for some tests i did it too.

The script is getting a C++ source file - or only a program name.
It produces a program with the basename of the source file.

Here is my simplified version:\n

    #!/bin/bash
    
    
    INFILE=${1%%.cpp}
    INFILE=${INFILE%%.C}
    INFILE=${INFILE%%.c++}
    INFILE=${INFILE}.C
    
    OUTFILE=${INFILE%%.C}
    
    g++ -std=c++14 \
     -Wall \
      -Weffc++ \
     -fdump-tree-original \
     -pthread \
     -L. \
      -O3 \
     "${INFILE}" -latrshmlog -o "$OUTFILE"
    
...again simple, but there are some new details here.

The Input is ripped off any of those extensions that are
in use. Sorry, but in C++ land its .C or .cpp or .cxx or ....

I gave up. Used in my code the .C and thats it for the first lines.

After the outfile is clear we have the gnu g++ in action.

First its again the language standard, here c++14. The 11 will do it too.

Then the checkings for warnings - i love this. A program with
an warning in c++ can easyly end up as a full crash on the wall.
So all in.

Then a special thing to check if the generated inline code
is indeed inline. I took that from a former project, didnt
check it for now .. no time ...

Then the -pthreaed thing again. 

Ok, i wanted c11 threads, but they simply were not there. So pthread
it is...

You have to tell the compiler where to find libraries
if you link in directly with the compiler. So the -L. here.

And of course the .....

WAIT:\n
This is a thing you have to realize. We use in the C header
inline C code for the tick timer thing. And its assembler code.

And now we use this here in the C++ compiler and get the same
need for -O ...

Ever tried that with DIFFERENT compiler vendors ?

I believe this will NOT work. But on my box it works, and on my
big boxes it work too ( small is a P9600 core 2 duo from 2009 with 8 GB
in my notebook,
big is dual processor amd with 24 or 32 cores - have both, watercooled
systems with no noise, and plenty GB of ram for my
Oracle database stuff....). So I do it.

If you recognize problems: switch to the functions instead of
the inline code. Try again.

The rest is easy. I have to tell the compiler which is my source,
then which is my library, and which is my target for output.

Summery
-------

I think you can do this with much more cpu power.
Use an IDE. Use a high sophisticated  IDE to script bridge
system and then go into deep details for proprietary make file
stuff. You will end up on fenster;plural s Micro.... C.... Studio ....
and find no support
for the last C standards. HAHA.

So for now i will use the script things and when it really
comes to a port on "fenster;plural" i will try to create linkable
objects with a one line cmd. period. Or use cygwin.
Or Mingw, Or UXwin. When everything fails: switch to a
hybrid that creates the objects and libraries.

There will be no support for a platform that does not want it.

( This said i suggest you check the ..... statement to the use
of tick counters on the fenster;plural systems).






