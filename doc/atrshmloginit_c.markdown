The ATRSHMLOG init program.  {#files_atrshmloginit_c}
====================================

This program initializes the shared memory buffer.

Its name is

     atrshmloginit
     

It must run before the first write or read use of the buffer.

It is separated from the create to handle the fact
that both have different responsibilities.

The init itself is done by a function, so the program
only knows about the count of buffers via parameter.

If you want to use a fraction of memory for a diffrent
thing you can give is a smaller number than the create.

If the environment variable for COUNT is set you can run it
without a parameter. See the dot file from atrshmlogcreate .
