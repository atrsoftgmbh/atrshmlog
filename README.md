# atrshmlog
The ATRSoft GmbH shared memory logging module.

This is a C module that makes possible logging in an shared memory area.

The logging itself is done in local buffers, so it is suitable for
use in multithread systems.

It also can be used from multiple processes parallel.

The module is available for users in C and C++ on varius platforms.

Ther are also language layers for use of the module from other
languages than C and C++.

Actual supported platforms:

- Linux
- cygwin
- mingw via cygwin

Actual supported languages:

- java via jni
- python
- perl via swig
- SWIG

The module is licenced under Apache 2.0 which schould make it available
for every interested user.

