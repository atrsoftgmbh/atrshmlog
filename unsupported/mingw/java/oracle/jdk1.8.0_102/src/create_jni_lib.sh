#!/bin/bash
#!/usr/bin/ksh
# $Id:$
#
# we create the jni lib.
# we make the class files, the create the header,
# and then compile the lib and copy it
# this is done simple, so it can give some errors..


date


if [ -z $JAVA_JNI_PLATFORM ]
then
    echo "please set the environment with dot.java.sh "
    exit 1
fi

case $JAVA_JNI_PLATFORM in
    linux)
	# linux x86_64 gnu
	LIB=libatrshmlogjni.so
	SOURCES="atrshmlogjnipackage.c"
	LIBS=-latrshmlog
	;;

    cygwin)
	# cygwin x86_64 gnu
	LIB=atrshmlogjni.dll
	SOURCES="atrshmlogjnipackage.c"
	LIBS=-latrshmlog
	;;

    mingw)
	# mingw x86_64 gnu via cygwin
	LIB=atrshmlogjni.dll
	SOURCES="atrshmlogjnipackage.c"
	LIBS=-latrshmlog
	;;

    posix)
	# linux x86_64 gnu
	LIB=libatrshmlogjni.so
	SOURCES="atrshmlogjnipackage.c"
	LIBS=-latrshmlog
	;;
	
    *)
	echo "dont know your platform here"
	exit 1
	;;
esac


####################################

./compile_to_class_package_version.sh

./create_header_package_version.sh

./compile_jni_stub.sh $LIB $SOURCES $LIBS
 
./copyjnilib.sh $LIB

./compile_to_class_package_tests.sh

date

# end of file


