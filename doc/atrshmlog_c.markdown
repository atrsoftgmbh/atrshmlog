The ATRSHMLOG module file.  {#files_atrshmlog_c}
====================================

This is the implementation of the module core.

The rest of the module reside in the impls directory.

The file names follow a convention.

A _flag.c contains a flag variable, best a small number of them.

A _buffer.c contains a buffer definition.

A _list.c contains a list , mostly only the anchor of it.

The _set_ files contain setters.

The _get_ files contain getters.

The rest is special, simply grep for the function you need to access
and check the files you hit.

The module results in a lib that is then used from here on.

So you can transfer contents of files into others to reduce
compile time - at least for my linux box i dont need it and its
better to have a fine grained library. So i have abandoned the
one file only approch and made a hard liner split.

