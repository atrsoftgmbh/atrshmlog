The ATRSHMLOG finish tool.  {#files_atrshmlogfinish_c}
====================================

This program is needed in cleanup the resources bound to a shared
memory buffer.

Its name is

     atrshmlogfinish


It tries to give free any locks, mutexes, condition variales etc.
that are connectet to that buffer.

So its use is the counterpart to the init tool.


