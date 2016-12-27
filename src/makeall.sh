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
# options :
# +d : ignore doc
# +c : ignore c and impls
# +l : ignore lib
# +b : ignore link binaries
# +t : ignore tests
# -a : do database

DODOC=1
DOC=1
DOL=1
DOB=1
DOTESTS=1
DOA=0

while [ $# -ne 0 ]
do
    case $1 in
	\+d)
	    DODOC=0
	    shift
	    ;;
	\+c)
	    DOC=0
	    shift
	    ;;
	\+l)
	    DOL=0
	    shift
	    ;;
	\+b)
	    DOB=0
	    shift
	    ;;
	\-a)
	    DOA=1
	    shift
	    ;;
	\+t)
	    DOTESTS=0
	    shift
	    ;;
	*)
	    shift
	    ;;
    esac
done

if [ -z $ATRSHMLOG_PLATFORM ]
then
    echo "please set the environment via dot.platform.sh"
    exit 1
fi

if [ $DODOC -eq 1 ]
then
    buddydoc.sh
fi

RM=/bin/rm

#############################

if [ $DOC -eq 1 ]
then

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

fi

if [ $DOA -eq 1 ]
then
    cd dbs

    for i in atrshmlog*.c
    do
	$RM -f ${i%%.c}.o
    
	echo $i compile
	g99.sh $i
    done

    cd ..
fi

if [ $DOL -eq 1 ]
then
    buddylib.sh
fi

#############################

if [ $DOB -eq 1 ]
then

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

fi

#############################

if [ $DOTESTS -eq 1 ]
then

    cd tests

    echo "in tests now ..."

    for i in $( cat ../shmtestcfiles | egrep -v '^#' )
    do
	$RM -f ${i%%.c}.o
    
	echo $i compile
	g99.sh $i
	ell.sh ${i%%.c}
    done

    for i in $( cat ../shmCPPfiles | egrep -v '^#' )
    do
	$RM -f ${i%%.C}
	echo ${i%%.C} build
	g++14w.sh $i
    done

    cd ..
fi

if [ $DOA -eq 1 ]
then

    #############################

    cd dbs
    
    cat ../shmbininternalfiles_postgres  | egrep -v '^#' | while read i rest
    do
	$RM -f $i
	echo $i link
	ell.sh $i $rest
    done

    cat ../shmbininternalfiles_mariadb  | egrep -v '^#' | while read i rest
    do
	$RM -f $i
	echo $i link
	ell.sh $i $rest
    done

    cat ../shmbininternalfiles_oracle  | egrep -v '^#' | while read i rest
    do
	$RM -f $i
	echo $i link
	ell.sh $i $rest
    done

    cd ..
fi

# end of file
