PROCESS : The check for completeness.    {#process_checkforcompleteness}
===============================

This is done by the script \n

     atrshmlogcheckcomplete.

You have to change to the BASEDIR/src directory.

There you need a shell.

Then start the script.

     $ atrshmlogcheckcomplete 

If you cannot start the script check for perl.

If you have perl start it with

    perl  atrshmlogcheckcomplete

The script writes a greatings message and then the list
of found files.

The last is the list of not found files - BAD .

If there is no list - GOOD.

At last it writes out if the test was ok or in error.

When you get a good you have at least the files in place.

When you get a error you have to check the download.

You can check against the github or the module on the web.
