The ATRSHMLOG fork test program.  {#files_atrshmlogfork_c}
====================================

This program is for testing the buffer system and internals
in case a fork happens.

Its name is

      atrshmlogfork
      

The fork is a seldom thing, but in the development of
the module necessary szenario.

So this program helps to check for the module in a fork.

It shows also how to make communication via the shared memory from
one process to another.

For the fenster;plural system you can ignore it, there is no fork
in the mingw port.
