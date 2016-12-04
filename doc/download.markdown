PROCESS : The Download of the module  {#process_download}
==========================

You need the files to do something.

So you can get them from github :

     $ git clone https://github.com/atrsoftgmbh/atrshmlog

Or you make a download of the atrsoftgmbh_shmlog_X_X_X.tar.gz 
from the website

<http://www.atrsoft.de>


After the thing is on your system you will have to unpack in
case you have downloaded the atrshmlog.tar.gz .

Choose an empty folder for the extract - you can use also
one with files inside if they dont have same names as the
files in the download.

This folder will be the BASEDIR in the rest of the documentation.

After unpacking with

    $ tar zxf atrshmlog.tar.gz

you should have a small directorytree.

It contains the bin directory, the doc directory and the src directory.

Inside these directorys are the files that you can check for with the
files.txt .

Thre is also a big example with the mksh shell code.

And of course the java, perl, python and SWIG layers.

Last thing you have to do is unseal it - meaning you have to make files writeable.

To do that execute bin/unseal.sh in the BASEDIR.

     $ bin/unseal.sh

If you think you will not change anything - there is at least stuff in the
java that is generated and now sealed. So change to the java directory
and unseal it.

    $ cd java
    $ ../bin/unseal.sh

Now you are ready to build.

