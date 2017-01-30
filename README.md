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
- BSD
- cygwin
- mingw via cygwin

Actual supported languages:

- java via jni
- python
- perl via swig
- ruby
- SWIG

The module is licenced under Apache 2.0 which schould make it available
for every interested user.

This module is a high performance logging solution.

It uses the tick counter of the CPU, so taking a timestamp is done in
nanosecs compared to the usual things of your OS that takes several
hundrets of them...

The log is done in alternating or ring buffer usage way, so you
are not slowed down if a buffer is full.

Normal payloads given (10 to 80 bytes ) you get a log in less
than 70 nanosecs done on a notebook....

The transfer is done by shared memory and a slave process that can
write to the filesystem. Alternatives are processes that write
to a database.

Supported databases are Mysql/MariaDB, PostgreSQL, Oracle, Cassandra.

You can also use a filestore first and then transfer to database,
without the need of an intermediate text file.

The logging itself needs a supporting thread at least - for very small
payloads you can circumvent the use of a thread at all and you can
use alternative a process end cleanup transfer without a thread at all.

The log also supports a direct to shared memory logging for
post mortem logging.

The log is highly configurable for the strategies if the buffers
are exhausted, and it is highly adjustable to different machine
architecture needs for synchronization of threads via memory barriers.

The logging itself is done on per thread base, so no contention between
logging threads.

The log uses nolock structures based on atomics only.

There are layers to make use of the log from perl, python, ruby, SWIG
capable languages and via JNI from java.

The log is documented by HTML and by a full fledge text documentation (440 pages in english).

have fun.
