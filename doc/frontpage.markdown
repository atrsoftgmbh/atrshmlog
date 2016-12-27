The ATRSHMLOG module introduction.  {#mainpage}
==================================

This is the short documentation for the ATRSHMLOG module.

The module was created to make logging a bit faster than the usual
approch with doing file i/o and formating features.

What is the module ?
--------------------

The module consists of several source files.

There is
- the main include atrshmlog.h
- the internal details include atrshmlog_internal.h
- the module implementation file atrshmlog.c
- the files in the impls directory
- the files in the dbs directory
- the fiels in the tests directory
- the details dummy file atrshmlog_internal.c
- the buffer converter atrshmlogconvert.c
- the shared memory buffer creater atrshmlogcreate.c
- the test and manipulation tool atrshmlogdefect.c
- the shared memory deletion tool atrshmlogdelete.c
- the shared memory dumping tool atrshmlogdump.c
- the shared memory cleanup tool atrshmlogfinish.c
- the test for daughter process tool atrshmlogfork.c
- the test for daughter and threads tool atrshmlogforkwrite.c
- the shared memory initialization tool atrshmloginit.c
- the shared memory log off switch tool atrshmlogoff.c
- the shared memory log on  switch tool atrshmlogon.c
- the threaded list reader atrshmlogreaderd.c
- the reinitialize tool atrshmlogreset.c
- the communication tool atrshmlogsignalreader.c
- the communication tool  atrshmlogsignalwriter.c
- the demo atrshmlogtee.c
- the test program atrshmlogtest00.c
- the test program atrshmlogtest01.c
- the test program atrshmlogtest02.C for C++
- the test program atrshmlogtest03.C for C++
- the shared memory diagnose tool atrshmlogverify.c

and several files for the build and documentation process.

See the files.txt file for a detailed list.

There is also a page for most files in this documentation.


How do i use it ?
-----------------

This is a multiple step process.

At least you need to build, test and implement in your system.

But we do it step by step.

###Download and unpack

To use the module you need to unpack the download.

If you got it from the githup you do not need to
unpack anything, its already then in file form on your
system.

[See the download page](\ref process_download)

###Check for completeness
This needs an installed perl on your system.

The check is done with atrshmlogcheckcomplete - which is a
perl script - and the files.txt file.

If you dont have a perl you can do the check with files.txt manually.

See files.txt for this.

[See the check page](\ref process_checkforcompleteness)

###Check for your system
The module runs on different systems.

The atrshmlogchecksystem does the checking.

If you cannot run the thing - it is a perl script - you can
check the header atrshmlog.h and atrshmlog_internal.h

For the supported platform there are already adjusted copys
in the alreadydone directory. Simply copy yours into src.

[See the check page](\ref process_checkforyoursystem)

###Adjust the module
You have to set several defines in the headers to the values you need.

See for details the page on the adjustment process.

[See the adjustment page](\ref process_adjustthemodule)

###Adjust the compiler stub files
You have to adjust the compiler stub files in the bin directory.

See for details the page on compiler adjustments.

[See the adjustment page](\ref process_adjustforcompiler)

###Compile the module
You compile the module.

The result is a bunch of files for linking and at least on
library for linking. The names depend on the actual system you use.

There are also the tools and the test programs.

[See the compile page](\ref process_compilemodule)

###Test the thing
You should now test the module. See for details the page about testing.

[See the test page](\ref process_testmodule)

###Implement your logging in your system
You have now to implement the logging in your system.

See for details the page about how to implement.

There is a small example with the atrshmlogtee.c as an example,
and a big example with the MirOS BSD mksh.

And of course the universal hello world.

[See the implements page](\ref process_implementlogging)


###Implement your logging in different language
You get support for the following languages
- Java
  There is a Java jni solution to use the module.
  [See for details the java module page.](\ref language_layer_java_jni)
  
- Python
  There is a python module solution.
  [See for details the python module page.](\ref language_layer_python)
  
- Perl
  There is a perl module solution.
  [See for details the perl module page.](\ref language_layer_perl)

- SWIG
  There is a SWIG solution.
  The examples work with tcl.
  [See for details the SWIG module page.](\ref language_layer_swig)


###Implement your converter
If you need a binary logging you have to adjust the converter.

[See the converter adjustment page](\ref process_implementconverter)

What about version and patch stuff ?
------------------------------------

The module has a strict version scheme.

The binary compatible layouts for
the shared memory buffer and the resulting
transfered binary files from the readers is the
target of this version.

Any change in the binary layout means we need a new version.

If you have any nice functionality beside this its a minor version change.
This means not a bugfix - that is a patch version change.

So if you have the module with this version you can use any module
with the same version to convert the files from the reader.
This can even span different platforms (didnt do that for now, but
its already prepared).

If you have any problem with the version you should
check if there is a mixed up installation for different modules.

You can put all module stuff in one place , so its possible to
have multiple modules on a system with different versions.

Who is the maintainer ?
-----------------------

Thats me, Anton Rommerskirchen, member at ATRSoft GmbH.

You can contact me via email

mailto: atr@atrsoft.de

For the company you can visit at the web

<http://www.atrsoft.de>


Where is the documentation ?
----------------------------

The documentation is for the short path this web documentation.

For the long path see the doc folder and its files.

The long term documentation is written with libreoffice.

Start with the doc atrshmlog.odt or atrshmlog.pdf.





