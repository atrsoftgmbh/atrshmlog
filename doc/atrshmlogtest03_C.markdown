The ATRSHMLOG test program 03.  {#files_atrshmlogtest03_c}
====================================

This program is a test program and can be used
in development and tests.

Its name is

      atrshmlogtest03

Its a C++ program, so you can see how integration with C++
works.

It is a multithreaded test program.

You have to give it three parameters:
- first the number of threads in the parallel section
- second the number of loops for the parallel section
- third the number of internal loop


The program starts up and then enters the parallel section.

It creates the many threads as you gave it.

Every thread executes a loop with that many iterations as the second parameter.
The internal loop inside one iteration adds to an int counter.
You can give the number of iterations for the int counter as third parameter.

So you can simulate a small system of parallel running independent threads
that do something all the time - get time, internal loop, get time
and log - and set the relative runtime by setting the third parameter.

On my box the internal loop is at 16 near equal to the cost of logging.

The program then enters the sequental section.

In here it calculates the number of total loops by multiply of
number of threads with number of outer loops.

Then it starts one thread doing that number loops.

After the program ends you get some timing numbers.

You can use them to see how good the program is in its multithread
section - the result shows the sum of time for the threads against the
time the sequential thread needs and the quotient of the time of the
slowest thread in multithread against the sequential time.

The former shows how good the system is in scaling up and the second
how much time you get from making the loops parallel for the
total time you will need.

At last you get the statistics counters.
See also \ref atrshmlog_counter for this.

