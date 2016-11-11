
# $Id:$
#
# we set the needed variables
#
# this is java home for the access to the jdk
# the jni os the jdk uses for the header.
# this is also used to switch in scripts for
# names of libs and flags

JAVA_HOME=/usr/lib/jvm/java-1.8.0-openjdk-1.8.0.111-1.b16.fc23.x86_64
export JAVA_HOME
    
PATH="${JAVA_HOME}/bin:$PATH:."    

# we set the os for jni too
# this is the jni subdirectory for the jdk you use

JAVA_JNI_OS=linux

export JAVA_JNI_OS


# we set the platform too
# this is the platform for buildenvironment,: linux, cygwin, mingw, ...
JAVA_JNI_PLATFORM=linux

export JAVA_JNI_PLATFORM

# end of file
