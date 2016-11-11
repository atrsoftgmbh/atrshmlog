
rem start the ATRSHMLOG demo


SET LIBDIR=./de/atrsoft/successorofoak/utilities/logging/atrshmlog/lib

rem  we use this time the package version. so its with the classspath / package

rem the shared lib is also moved to the package dir here

java -Djava.library.path=%LIBDIR% de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOGTest  hallo welt

rem end of file
