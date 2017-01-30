The ATRSHMLOG interface header file.  {#files_atrshmlog_h}
====================================

The header defines the interface to the module.

You use it as an include file where you need access to the
functions and variables.

In principle you can define the needed linkage declarations
without the header. But for more than a bunch of prototypes
I strongly recommend the header.

There is an important part of inline code defined at the end.

You can separate this if your compiler needs this into a separate
include.

The usage is as always simply to include it.

The adjustments that you can make are in the define section.

For details see the page for the adjustment process.

Here we focus on the use, so this is coupled with the compilers
way to handle the inline code.

For my platform i have to use not only the inline keyword,
but also the compiler needs a switch to do the optimization.

Here is the needed compiler line:

    gcc -std=gnu11 -fPIC -I. -O3 $* -c

The compiler is the gcc , version 5.3. I use a fedora 23 linux as
the os.

The compiler needs an -O3 for me to make me happy. The same compiler
fails to make the inline code if i switch to -g for debugging.

So this is coupled with the defines for
- ATRSHMLOG_PLATFORM
- ATRSHMLOG_FLAVOUR
- ATRSHMLOG_INLINE
- ATRSHMLOG_INLINE_TSC_CODE
- ATRSHMLOG_INLINE_GETTIME

See the dot.platform files in the BASEDIR for the settings of
environment variables for the bin files that are used in the header.

If you want to use the -g or an optimization level that the compiler
fails to inline you have to switch the last defines to 0.

Beside that the include holds the macros for use in an
production source code.

The use of macros is - normally - not a good thing, but it has some
usage if you plan for a stable interface in long terms.

So its possible to play games with some variables without having
them directly in sight.

A good example is the ATRSHMLOG_GETTIME() which was stable for
at least several months even when i switched from use
of pointers and real time to use of values and tick counter.

The thing can save the day, and if you dont overdue it you end
up with code in your target that has no problems if the internals
of the module change.

And change is always possible.

So be happy to have a layer that you control - you can do the
changes in the  macro and thats it.

Beside the macros there are of course the function prototypes.

So this is also a point where you have control - you have the source,
so you can change things.

Best is to sent changes back to me so i can make them in the module
too. If they make sense for the other users.

The rest is a bunch of typedefs to make the data portable between
platforms of different os and cpu systems.

So you normaly leave this to the C library.







