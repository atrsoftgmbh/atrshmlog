The ATRSHMLOG log off program.  {#files_atrshmlogoff_c}
====================================

This program switches the in the shared memory area residing switch for
system wide logging to off.

Its name is

     atrshmlogoff


So every older reader disconnects and stops reading from shared memory.

Every write ignores the logging and simply goes on after discarding.

You can switch on with the log on program.

Then you can again connect with a reader and writers will
again write log to the shared memory.

The readerd now ignores the switch. You can stop it with the signalreader
and the magic keys 9999 and 9999.

Or use the atrshmlogstopreader script.

So for the off its now only the logging itself.
