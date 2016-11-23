
# $Id:$
#
# we set some basic environment vatiables.
# they are used in the scripts for the module build
# so be honest here or you could end up with module that does not work
#
# The variables are only needed at build time.
# for runtime you have to check for the platform if it needs
# stuff in so or dll form and make the path an LD_LIBRARY_PATH
# working. this is for every platform different, so consult a local
# expert in that field.

date

echo "set basedir "
# we set the basedir. so this must be used inplace in the correct dir
ATRSHMLOG_BASEDIR="$(pwd)"

export ATRSHMLOG_BASEDIR

echo "set platform"
# we set the platform for build

ATRSHMLOG_PLATFORM=linux

# ATRSHMLOG_PLATFORM=cygwin

# ATRSHMLOG_PLATFORM=mingw

# ATRSHMLOG_PLATFORM=bsd

# ATRSHMLOG_PLATFORM=solaris

export ATRSHMLOG_PLATFORM

# default is 1

# linux fedora 1
# linux centos 2
# linux ubuntu 7
# linux opensuse 8
# linux debian 9
# linux sles 10

# bsd freebsd 3
# bsd openbsd 4
# bsd netbsd 5

# solaris opensolaris11 6

ATRSHMLOG_FLAVOUR=1

export ATRSHMLOG_FLAVOUR

echo "set path"
# we need a working path. so we assume its the basedir relative in bin
# what makes the thing

PATH="$PATH:$ATRSHMLOG_BASEDIR/bin:."

export PATH

date 
# end of file

