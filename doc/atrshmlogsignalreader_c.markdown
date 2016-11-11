The ATRSHMLOG signal reader program.  {#files_atrshmlogsignalreader_c}
====================================

This program uses the reader field to communicate
with a program that uses them.

Its name is

      atrshmlogsignalreader


This is a kind of morse code tool here.

Use it as a prototype how to communicate via the
shared memory buffer and see also the fork examples.

It is also a production program, you shut down the atrshmlogreaderd with it
with the magic pid 9999 and value 9999.
