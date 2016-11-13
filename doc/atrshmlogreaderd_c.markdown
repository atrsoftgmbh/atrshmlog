The ATRSHMLOG reader d program.  {#files_atrshmlogreaderd_c}
====================================

This is the concurrent buffer reader with list.

Its name is

     atrshmlogreaderd
     

It reads from shared memory and then writes the
buffer down in the file system.

It uses a multi thread design.

This is the prototyp for your reader if you need a special one.

The program is the production system part, so dont change it.

Make a copy and start with that.

The reader d can be used in different ways.

The reader uses two environment variables for this.

The FETCH COUNT can be set to give the number of threads that
try to fetch from shared memory in parallel.

The WRITE COUNT can be set to give the number of threads to write
buffers down to the file system.

The reader starts with a high load szenario setting.

So its a wise thing in a low load szenario to reduce them.

The reader uses the number of buffers in shared memory to calc the
start values if you dont set the environment variables.

Try to understand first the adjustment process before you try
to give other numbers.

The reader stops when it encounters a reader signal for pid 9999 and
value 9999.

It shows the switches for the system wide logging flag in the area.

To stop it use the signalreader or the atrshmlogstopreader script.




