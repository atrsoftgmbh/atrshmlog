#!/bin/bash
#!/usr/bin/ksh
# $Id:$
#
# the hello jni header create. we use the class file 
# the class file is in the subdirectory identified by package path.
# so we have to stay at the root or have to use -classpath
# we have to write to the includes folder
# we need the java path for the package too
#
# use the SET or the dot file or the cmd ....

date

if [ -z $JAVA_JNI_PLATFORM ]
then
    echo "please set the environment with dot.java.sh "
    exit 1
fi

echo "create atrshmlogjni.h"
javah -classpath "." -o atrshmlogjni.h.$$ de.atrsoft.successorofoak.utilities.logging.atrshmlog.ATRSHMLOG

if [ $? -eq 0 ]
then
    mv atrshmlogjni.h.$$ includes/atrshmlogjni.h
    echo "create done"
fi

date

# end of file


