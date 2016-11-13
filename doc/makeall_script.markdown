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

Then the lists shmbinfiles and shmbininternalfiles are used.
The programs in there are linked.


Last is to compile the files with filename ending in .C and linking
then to the program.

You find the used scripts g99.sh, ell.sh, g++14w.sh in the bin directory.





