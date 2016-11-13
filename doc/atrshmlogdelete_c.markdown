The ATRSHMLOG delete the shared memory tool.  {#files_atrshmlogdelete_c}
====================================

This program deletes the shared memory.

Its name is

     atrshmlogdelete

It is helpful in automated tests where every test is a cycle
of create, init, test, convert, calc, finish  and delete.

This cycle is easy to use in a framework.

So the delete is here to help you to automate your tests.

Its also needed to cleanup after you have decided no longer to use a
buffer, so the system can reuse that memory.

For the mingw the program is a dummy. The fenster;plural system uses a
reference counter to check if it should delete the mapped file. So if
you disconnect all programs from it, it is deleted directly. No need
for a program. The bad side is that the creator program has to run at
least till another program connects to the mapped file or its gone...

