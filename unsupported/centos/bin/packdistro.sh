#!/bin/bash
#!/usr/bin/ksh
#
# $Id:$
#
# pack the distro tarball

if [ -z $1 ]
then
    echo "usage: packdistro tarname"
    exit 1
fi

tar zcf $1 bin doc dot.platform.sh dot.platform.sh.* java mksh perl python readme.txt src SWIG unsupported

ls -l $1

# end of file

