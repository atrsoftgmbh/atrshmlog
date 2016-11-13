#!/bin/bash
#!/usr/bin/ksh
# $Id:$
#
# we compile in step 1 the java code to a class file 
# we have to be at the base dir for the java
# thats BASEDIR/java/VENDOR/JDK/src
# with vendor something like oracle, ibm, gnu ...
# and jdk something like jdk1.8.0_66 or jdk_1_8_0 or ...
# we have to set JAVA_HOME and the path for the things
# with a bunch of SET or a dot or a cmd file ....
date


if [ -z $JAVA_JNI_PLATFORM ]
then
    echo "please set the environment with dot.java.sh "
    exit 1
fi


SOURCES="de/atrsoft/successorofoak/utilities/logging/atrshmlog/ATRSHMLOG.java 
       de/atrsoft/successorofoak/utilities/logging/atrshmlog/ATRSHMLOGTest.java"

for J in $SOURCES
do
    echo "compile $J"
    javac $J
done

date

# end of file
