The ATRSHMLOG makeall script.  {#files_makeall_script}
====================================

This script builds the module and the documentation.

Its name is

     makeall.sh


If you do need different to do then  make a copy and change it.

The script starts first the doxygen for
generating the documentation.

If you dont have doxygen make a dummy or change to a copy without
doxygen call. You then need to get the documentation from the
web at <http://www.atrsoft.de>

Next the compile for the files in the source directory
with filenames ending in .c is done.

Then the library is created.

The dbs and tests.

Then the lists shmbinfiles and shmbininternalfiles are used.
The programs in there are linked.

Then the tests are made.

Last is to compile the files with filename ending in .C and linking
then to the program.

The makeall.sh has now some optional switches to circumvent doing things.

If you don't need the first compile a +c blocks. If you dont need the
doxygen a +d blocks, for the lib a+l and so on. Check the code to know
what you can suppress.

You find the used scripts g99.sh, ell.sh, g++14w.sh in the bin directory.

Last is to activate the database parts with a -d - yes, its - and
not + this time - and you get the converter and reader with support
for the upload into database.





