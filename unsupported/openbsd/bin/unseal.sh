#!/bin/ksh
#!/bin/bash
#!/usr/local/bin/bash
#!/usr/bin/ksh
#
# $Id:$
#
# we unseal up all mess
# if you need something unsealed up too add it and sent me a postcard with it


find . -type f -name '*.c' -print | xargs -n1 -ifile chmod 644 file

find . -type f -name '*.C' -print | xargs -n1 -ifile chmod 644 file

find . -type f -name '*.h' -print | xargs -n1 -ifile chmod 644 file

find . -type f -name '*.java' -print | xargs -n1 -ifile chmod 644 file

find . -type f -name '*.sh' -print | xargs -n1 -ifile chmod 755 file

find . -type f -name 'Doxyfile' -print | xargs -n1 -ifile chmod 644 file

find . -type f -name '*.markdown' -print | xargs -n1 -ifile chmod 644 file

find . -type f -name 'atrshmlog.h.*linux' -print | xargs -n1 -ifile chmod 644 file

find . -type f -name 'atrshmlog.h.cygwin' -print | xargs -n1 -ifile chmod 644 file

find . -type f -name 'atrshmlog.h.mingw' -print | xargs -n1 -ifile chmod 644 file

find . -type f -name 'atrshmlog.h.*bsd' -print | xargs -n1 -ifile chmod 644 file

find . -type f -name 'atrshmlog_internal.h.*linux' -print | xargs -n1 -ifile chmod 644 file

find . -type f -name 'atrshmlog_internal.h.cygwin' -print | xargs -n1 -ifile chmod 644 file

find . -type f -name 'atrshmlog_internal.h.mingw' -print | xargs -n1 -ifile chmod 644 file

find . -type f -name 'atrshmlog_internal.h.*bsd' -print | xargs -n1 -ifile chmod 644 file

find . -type f -name 'atrshmlogcalc' -print | xargs -n1 -ifile chmod 755 file
find . -type f -name 'atrshmlogcheckcomplete' -print | xargs -n1 -ifile chmod 755 file
find . -type f -name 'atrshmlogconv' -print | xargs -n1 -ifile chmod 755 file
find . -type f -name 'atrshmlogsort' -print | xargs -n1 -ifile chmod 755 file
find . -type f -name 'atrshmlogchecksystem.cmd' -print | xargs -n1 -ifile chmod 755 file
find . -type f -name 'atrshmlogchecksystem' -print | xargs -n1 -ifile chmod 755 file
find . -type f -name 'atrshmlogerror' -print | xargs -n1 -ifile chmod 755 file
find . -type f -name 'atrshmlogstat' -print | xargs -n1 -ifile chmod 755 file

find . -type f -name 'shmallfiles' -print | xargs -n1 -ifile chmod 644 file
find . -type f -name 'shmallbinfiles' -print | xargs -n1 -ifile chmod 644 file
find . -type f -name 'shmbinfiles' -print | xargs -n1 -ifile chmod 644 file
find . -type f -name 'shmbininternalfiles' -print | xargs -n1 -ifile chmod 644 file
find . -type f -name 'files.txt' -print | xargs -n1 -ifile chmod 644 file


# end of file

