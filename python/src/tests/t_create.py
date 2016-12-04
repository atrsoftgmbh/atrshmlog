#!/usr/bin/python3
#
# Id:
#
# We test a bit of the atrshmlog here.
#
# This is for the first starter, so only the basic things.

import sys
import atrshmlog

key = int(sys.argv[1]);

print('create : ' + str(key) + ' : ')

count = int(sys.argv[2]);

print('create : ' + str(count) + ' : ')

id = atrshmlog.create(key, count)

print('create : ' + str(id) + ' : ')

if id > 0 :
    f = open('dot.atrshmlog', 'w')
    
    c =  'ATRSHMLOG_ID=' + str(id) + '\nexport ATRSHMLOG_ID\nATRSHMLOG_COUNT=' + str(count) + '\nexport ATRSHMLOG_COUNT\n'
    
    f.write(c)
    
    f.close()


print (' ')

exit(0)

# end of test

