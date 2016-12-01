#!/bin/bash
#!/usr/bin/ksh
# $Id:$
#
# start the ATRSHMLOG test main method

if [ -z $JAVA_JNI_PLATFORM ]
then
    echo "please set the environment with dot.java.sh "
    exit 1
fi

if [ $# -ne 2 ]
then
    exit 1
fi

PKG=de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests
LIBDIR=./de/atrsoft/successorofoak/utilities/logging/atrshmlog/lib

# we use this time the package version. so its with the classspath / package
# the shared lib is also moved to the package lib dir here
# on mingw we need also the runtime dll of the compiler stuff,
# and in case its apthread version the libpthread
# see the doc for more on mingw
#
java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestAttach 

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestGettime

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestSgettime

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestStatistics

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestSleepNanos

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestEnvPrefix

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestGetEnv _ID

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestVersion

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestEventLocksMax 10042

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestEvent

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestLogging

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestShmid

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestArea

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestBufferMaxSize

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestBufferSize

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestSlaveCount 8

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestClockId

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestSlaveRunOff 8

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestWaitForSlaves

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestBufferSlaveWait

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestAcquireCount

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestPreallocCount

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestGetInitTime

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestGetRealtime

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestStop

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestFlush

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestStrategy

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestCreateSlave

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestDecrementSlaveCount

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestFence

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestInitInAdvance

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestNextSlave 3

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestChecksum

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestAutoflush

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestVerify

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestClicktime

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestThreadLocal

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestReadFetch

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestReadFetch

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestReuseBuffer

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestReadFetch

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestReadFetch

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestWrite

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestReadFetch

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestReadFetch

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestWriteString

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestReadFetch

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestReadFetch

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestWriteString2

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestReadFetch

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestReadFetch

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestWriteString3

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestReadFetch

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestReadFetch

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestWriteBytes

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestReadFetch

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestReadFetch

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestWrite

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestRead 0

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestReadFetch

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestReadFetch

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestCreate $1 $2

. dot.atrshmlog

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestInitShmLog $2

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestCleanupLocks

java -Djava.library.path=$LIBDIR $PKG.ATRSHMLOGTestDelete $ATRSHMLOG_ID


# end of file
