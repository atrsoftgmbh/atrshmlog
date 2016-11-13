The ATRSHMLOG convert prototype.  {#files_atrshmlogconvert_c}
====================================

This is the prototype converter.

Its name is

     atrshmlogconvert


It takes  the binary file from a reader and converts it to a text file.
Optional you can get the threads statistics in a separate file.

The prototype uses a simple approch.

The given files are the parameters of the program.

It reads in the informations about the data that is independent
from a single log entry first.

Then it reads a filler. The core data and filler add up to 512 bytes.

This is depending on the version. If you add info, remove info, change sizes
you have to adjust here in the reader and in the converter
or you run out of sync.

After the core data is read in, the data buffer is read in.

For the buffer the first thing is the size - it can be the maximum
of ATRSHMLOGBUFFER_INFOSIZE . Bigger buffers are forbidden.

After the buffer is in a helper it transforms its content.

The helper takes all info that is from the core and then
converts entry by entry of the buffer.

The core info is used to make the output readable for the calc and sort,
so there is many  redundant info here.

One important thing is the calculation of the real time estimations.

The module knows only about clicks and the process has one time when it
attaches that is stored then. This time comes with a click
before and after. So the process has a rough knowledge about
the point in time and the related clicks.
When the buffer is full it is moved to shared memory and then also
there is a point in time made together with a click before and after.

The rest is a linear approch - but it has a subtile problem.
The numbers lead to an overflow in case you try to use only 64 bit
integers for the calculation. So i switched to the somewhat strange
approch of using long double here - at least a 128 bit type is needed
to do things right, and this was my best shoot on my box.

All other things are easy to understand - take the entry from the buffer
till its consumed, calc the values that are simple and then
write the line.

In case of the java interface there is a test for the event flag, so
the convert can handle at least the ucs2 strings from the
java environment in a rough way.

