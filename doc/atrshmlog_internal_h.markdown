The ATRSHMLOG details interface header file.  {#files_atrshmlog_internal_h}
====================================


This is the header for internal details of the module.

Its name is

     atrshmlog_internal.h



Its normally a fact that you dont need it in the
production code.

But if you plan to make a use of the shared memory or of the
change of defines its the place you will have to work.

The first parts are the usual defines that should not be changed
by a normal user. Version numbers and that stuff.

Then begins a bunch of defines that can be changed - at least if you
plan to customize the module.

See the page on the adjustment process for that.

This group ends with the OS specifics.

This is for now a small number of includes and defines, but it will grow
with every target system.

For now we start with x86 in 64 bit linux and the gnu compiler...

This ends with systemcall interfaces.

Now you enter a group of defines that are - normally  - of no
interest. The suffixes for environment variable names and
flagfiles.

The last defines are for debugging - which means writing stuff out
via printfs....

OK. That can help if you seem to have problems in the adaptation to a new
platform, but normally you will not need it.

There is then a group of typedefs and structs that describe the
shared memory buffer.

And the pointer that is holding the adress.

The internal structs of the module follow.

Then the internal used functions.

Some level dependend functions close the thing....

If you need something like a new converter or even a new reader you will
check for the header.

There is not much to say about - with the exception of
the adjustments.

But for this see that in the documentation ( and I
mean the big one in the odt, not the page).
