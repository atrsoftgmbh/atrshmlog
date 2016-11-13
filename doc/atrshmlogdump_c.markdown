The ATRSHMLOG dump the shared memory tool.  {#files_atrshmlogdump_c}
====================================

This is a helper for development.

Its name is

     atrshmlogdump


There is seldom the need to check down to every bit the content of the
shared memory, but in that case the dump tool can help.

It writes down the buffer in a file buffer.bin.

So you can then start to use your hex dump tool
and the offsets from the defect tool to
interpret the content.

To make things simple the dump does not try to check anything.

So you have to do your checks after the dump.

Please have in mind that its a good thing to NOT log in the time you
dump the memory - this would lead to races and you cannot know what you
have in the dump.


