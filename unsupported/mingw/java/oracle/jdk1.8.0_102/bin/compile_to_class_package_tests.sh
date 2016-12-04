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

PKG=de/atrsoft/successorofoak/utilities/logging/atrshmlog/tests

SOURCES="ATRSHMLOGTestAttach.java \
         ATRSHMLOGTestGettime.java \
         ATRSHMLOGTestSgettime.java \
         ATRSHMLOGTestCreate.java \
         ATRSHMLOGTestClicktime.java \
         ATRSHMLOGTestWrite.java \
         ATRSHMLOGTestWriteString.java \
         ATRSHMLOGTestWriteString2.java \
         ATRSHMLOGTestWriteString3.java \
         ATRSHMLOGTestWriteBytes.java \
         ATRSHMLOGTestStatistics.java \
         ATRSHMLOGTestSleepNanos.java \
         ATRSHMLOGTestEnvPrefix.java \
         ATRSHMLOGTestGetEnv.java \
         ATRSHMLOGTestVersion.java \
         ATRSHMLOGTestEventLocksMax.java \
         ATRSHMLOGTestEvent.java \
         ATRSHMLOGTestLogging.java \
         ATRSHMLOGTestShmid.java \
         ATRSHMLOGTestArea.java \
         ATRSHMLOGTestBufferMaxSize.java \
         ATRSHMLOGTestBufferSize.java \
         ATRSHMLOGTestSlaveCount.java \
         ATRSHMLOGTestClockId.java \
         ATRSHMLOGTestSlaveRunOff.java \
         ATRSHMLOGTestWaitForSlaves.java \
         ATRSHMLOGTestBufferSlaveWait.java \
         ATRSHMLOGTestAcquireCount.java \
         ATRSHMLOGTestPreallocCount.java \
         ATRSHMLOGTestGetTime.java \
         ATRSHMLOGTestGetRealtime.java \
         ATRSHMLOGTestStop.java \
         ATRSHMLOGTestFlush.java \
         ATRSHMLOGTestStrategy.java \
         ATRSHMLOGTestCreateSlave.java \
         ATRSHMLOGTestDecrementSlaveCount.java \
         ATRSHMLOGTestFence.java \
         ATRSHMLOGTestThreadLocal.java \
         ATRSHMLOGTestReuseBuffer.java \
         ATRSHMLOGTestInitInAdvance.java \
         ATRSHMLOGTestNextSlave.java \
         ATRSHMLOGTestChecksum.java \
         ATRSHMLOGTestAutoflush.java \
         ATRSHMLOGTestVerify.java \
         ATRSHMLOGTestRead.java \
         ATRSHMLOGTestReadFetch.java \
         ATRSHMLOGTestInitShmLog.java \
         ATRSHMLOGTestCleanupLocks.java \
         ATRSHMLOGTestDelete.java \
" 


for J in $SOURCES
do
    echo "compile $PKG/$J"
    javac $PKG/$J
done

date

# end of file
