The ATRSHMLOG create shared memory buffer tool.  {#files_atrshmlogcreate_c}
====================================

This program helps you to create the shared memory for the log.

Its name is

     atrshmlogcreate
     

It uses the parameters for the key and count of buffers.

First is the key for the memory.

Its a 32 bit number on most systems.
For the mingw its the index in the names array for the ipc mapped file.

Next is the number of buffers you want to use in shared memory.

From the number is calculated the size that the shared memory
should have at least.

Third can be a filename for the dot file.

For the tool the output is the important thing for you.
Its the value for connecting to the shared memory - the id.

Its given in a text form that can be used for cut and paste into
the shells you will need.

If the environment variables cannot be used you have to save the id
and make a file with it.

See the documentation for the use of
flag files instead of environment variables for this.

The program creates a dot file for the ease of use. If you dont use a
third parameter its dot.atrshmlog.

For the mingw its a atrshmlog.cmd file.
So you can run the program and after you source the dot file ( or execute
the cmd for the fenster;plural systems).




