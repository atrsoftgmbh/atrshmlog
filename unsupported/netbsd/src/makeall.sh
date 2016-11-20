#!/bin/ksh
#!/bin/bash
#!/usr/local/bin/bash
#!/usr/bin/ksh
#!/bin/ksh
# $Id:$
#
# build it
#
# check to have the scripts in bin in the path first.
#
# first we try the documentation via doxygen. if not there
# install it. or change the buddydoc to another thing.
#
# then we compile the .c files.
#
# next is the library
#
# then we link the programs
#
# last are the c++ programs
# if you dont have a multithread capable compiler you have to spare test03
# edit the listfile for that
#

if [ -z $ATRSHMLOG_PLATFORM ]
then
    echo "please set the environment via dot.platform.sh"
    exit 1
fi

buddydoc.sh

RM=/bin/rm

#############################

for i in $( cat shmcfiles | egrep -v '^#' )
do
    $RM -f ${i%%.c}.o
    
    echo $i compile
    g99.sh $i
done

#############################

cd impls

for i in atrshmlogimpl*.c
do
    $RM -f ${i%%.c}.o
    
    echo $i compile
    g99.sh $i
done

cd ..

buddylib.sh

#############################

for i in $( cat shmbinfiles | egrep -v '^#' )
do
    $RM -f $i
    echo $i link
    ell.sh $i
done

#############################

for i in $( cat shmbininternalfiles  | egrep -v '^#' )
do
    $RM -f $i
    echo $i link
    ell.sh $i 
done

#############################

for i in $( cat shmCPPfiles | egrep -v '^#' )
do
    $RM -f ${i%%.C}
    echo ${i%%.C} build
    g++14w.sh $i
done

# end of file
