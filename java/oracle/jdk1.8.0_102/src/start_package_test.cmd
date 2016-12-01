rem we test the java layer
rem
rem we need 
rem if [ $# -ne 2 ]
rem then
rem    exit 1
rem fi

SET PKG=de.atrsoft.successorofoak.utilities.logging.atrshmlog.tests
SET LIBDIR=./de/atrsoft/successorofoak/utilities/logging/atrshmlog/lib

rem we use this time the package version. so its with the classspath / package
rem the shared lib is also moved to the package lib dir here
rem on mingw we need also the runtime dll of the compiler stuff,
rem and in case its apthread version the libpthread
rem see the doc for more on mingw

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestAttach 

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestGettime

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestSgettime

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestStatistics

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestSleepNanos

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestEnvPrefix

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestGetEnv _ID

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestVersion

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestEventLocksMax 10042

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestEvent

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestLogging

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestShmid

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestArea

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestBufferMaxSize

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestBufferSize

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestSlaveCount 8

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestClockId

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestSlaveRunOff 8

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestWaitForSlaves

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestBufferSlaveWait

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestAcquireCount

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestPreallocCount

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestGetInitTime

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestGetRealtime

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestStop

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestFlush

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestStrategy

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestCreateSlave

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestDecrementSlaveCount

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestFence

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestInitInAdvance

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestNextSlave 3

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestChecksum

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestAutoflush

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestVerify

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestClicktime

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestThreadLocal

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestReadFetch

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestReadFetch

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestReuseBuffer

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestReadFetch

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestReadFetch

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestWrite

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestReadFetch

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestReadFetch

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestWriteString

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestReadFetch

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestReadFetch

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestWriteString2

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestReadFetch

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestReadFetch

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestWriteString3

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestReadFetch

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestReadFetch

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestWriteBytes

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestReadFetch

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestReadFetch

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestWrite

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestRead 0

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestReadFetch

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestReadFetch

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestCreate 2 8

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestCleanupLocks

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestInitShmLog 8

java -Djava.library.path=%LIBDIR% %PKG%.ATRSHMLOGTestDelete 2


rem end of file
