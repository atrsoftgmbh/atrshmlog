The ATRSHMLOG calculator file.  {#files_atrshmlogcalc_perl}
====================================

This perl script uses the convertet information after the
atrshmlogconvert  has done the work.

Its name is

     atrshmlogcalc
     

It calculates some histogram and statistics data.

Its a start. For real applications you will need a database.

The script is an example for the use with the mksh binary.

You should see this as an example, not as a prototpe.

First the script reads in the data.

After the  - simple - parsing it builds some in between info
and makes a little index system.

The major part is done after the index is there.

The analyse per pid gives a hint where the process spends most of its time.

The use of events is an important point here.

It has even a lookup to match them with the file, function and in some
cases even loops in the target.

So its important to plan for the events before you start to
put the log in the target.

For starters a good approch is to use a
fixed number scheme for the files involved (or classes, or packages ...)
and then a use of small numbers inside of these to descripe the points
of the logging.

There is nothing special about the script, only simple parse in,
make index, sort, and print out.
