#!/bin/ksh
#!/bin/bash
#!/usr/local/bin/bash
#!/usr/bin/ksh
#
# $Id:$
#
# we clean up all mess
# if you ned something cleand up too add it and sent me a postcard with it

find . -type f -name '*~' -print | xargs -n1 -ifile /bin/rm file

find . -type f -name '*.o' -print | xargs -n1 -ifile /bin/rm file

find . -type f -name '*.C.003t.original' -print | xargs -n1 -ifile /bin/rm file

find . -type f -name '*.a' -print | xargs -n1 -ifile /bin/rm file

find . -type f -name '*.so' -print | xargs -n1 -ifile /bin/rm file

find . -type f -name '*.dll' -print | xargs -n1 -ifile /bin/rm file

find . -type f -name '*.class' -print | xargs -n1 -ifile /bin/rm file

# end of file

