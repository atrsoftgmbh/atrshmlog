The ATRSHMLOG files.txt config file.  {#files_files_txt}
====================================

This file contains the list of files that are in the distribution.

Its name is

     files.txt


If you have a running perl you can use atrshmlogcheckcomplete
to do the check.

Open a shell, switch to the src directory and execute it.

     $ atrshmlogcheckcomplete
     
If you have not a running perl you need to do the check by manual.

For every file there is a line.

On the line you find the filename and if its not in the default directory
the directory as a list of names for the path.

You have to check the entries starting after the line with the single
word

FILES

till the line with the single word

FILESEND

If there is a line starting with DIRECTORY this is
the simplyfied path from the BASEDIR up to the directory that
holds the following files. It sets the default then to this.

