: # work around NeXTstep bug
echo + Running genopt on 'rlimits.opt'...
(srcfile='./rlimits.opt'; BUILDSH_RUN_GENOPT=1; . './Build.sh')
echo + Running genopt on 'sh_flags.opt'...
(srcfile='./sh_flags.opt'; BUILDSH_RUN_GENOPT=1; . './Build.sh')
set -x
cc -std=c99 -pthread -Wno-deprecated-declarations -fno-asynchronous-unwind-tables -fno-strict-aliasing -fstack-protector-strong -Wall -fwrapv -I.  -DMKSH_BUILDSH -D_GNU_SOURCE -DSETUID_CAN_FAIL_WITH_EAGAIN -DHAVE_ATTRIBUTE_BOUNDED=0 -DHAVE_ATTRIBUTE_FORMAT=1 -DHAVE_ATTRIBUTE_NORETURN=1 -DHAVE_ATTRIBUTE_PURE=1 -DHAVE_ATTRIBUTE_UNUSED=1 -DHAVE_ATTRIBUTE_USED=1 -DHAVE_SYS_TIME_H=1 -DHAVE_TIME_H=1 -DHAVE_BOTH_TIME_H=1 -DHAVE_SYS_BSDTYPES_H=0 -DHAVE_SYS_FILE_H=1 -DHAVE_SYS_MKDEV_H=0 -DHAVE_SYS_MMAN_H=1 -DHAVE_SYS_PARAM_H=1 -DHAVE_SYS_RESOURCE_H=1 -DHAVE_SYS_SELECT_H=1 -DHAVE_SYS_SYSMACROS_H=1 -DHAVE_BSTRING_H=0 -DHAVE_GRP_H=1 -DHAVE_IO_H=0 -DHAVE_LIBGEN_H=1 -DHAVE_LIBUTIL_H=0 -DHAVE_PATHS_H=1 -DHAVE_STDINT_H=1 -DHAVE_STRINGS_H=1 -DHAVE_TERMIOS_H=1 -DHAVE_ULIMIT_H=1 -DHAVE_VALUES_H=1 -DHAVE_CAN_INTTYPES=1 -DHAVE_CAN_UCBINTS=1 -DHAVE_CAN_INT8TYPE=1 -DHAVE_CAN_UCBINT8=1 -DHAVE_RLIM_T=1 -DHAVE_SIG_T=1 -DHAVE_SYS_ERRLIST=1 -DHAVE_SYS_SIGNAME=0 -DHAVE_SYS_SIGLIST=1 -DHAVE_FLOCK=1 -DHAVE_LOCK_FCNTL=1 -DHAVE_GETRUSAGE=1 -DHAVE_GETSID=1 -DHAVE_GETTIMEOFDAY=1 -DHAVE_KILLPG=1 -DHAVE_MEMMOVE=1 -DHAVE_MKNOD=0 -DHAVE_MMAP=1 -DHAVE_NICE=1 -DHAVE_REVOKE=0 -DHAVE_SETLOCALE_CTYPE=1 -DHAVE_LANGINFO_CODESET=1 -DHAVE_SELECT=1 -DHAVE_SETRESUGID=1 -DHAVE_SETGROUPS=1 -DHAVE_STRERROR=0 -DHAVE_STRSIGNAL=0 -DHAVE_STRLCPY=0 -DHAVE_FLOCK_DECL=1 -DHAVE_REVOKE_DECL=1 -DHAVE_SYS_ERRLIST_DECL=1 -DHAVE_SYS_SIGLIST_DECL=1 -DHAVE_PERSISTENT_HISTORY=1 -DMKSH_BUILD_R=521 -c lalloc.c || exit 1
cc -std=c99 -pthread -Wno-deprecated-declarations -fno-asynchronous-unwind-tables -fno-strict-aliasing -fstack-protector-strong -Wall -fwrapv -I.  -DMKSH_BUILDSH -D_GNU_SOURCE -DSETUID_CAN_FAIL_WITH_EAGAIN -DHAVE_ATTRIBUTE_BOUNDED=0 -DHAVE_ATTRIBUTE_FORMAT=1 -DHAVE_ATTRIBUTE_NORETURN=1 -DHAVE_ATTRIBUTE_PURE=1 -DHAVE_ATTRIBUTE_UNUSED=1 -DHAVE_ATTRIBUTE_USED=1 -DHAVE_SYS_TIME_H=1 -DHAVE_TIME_H=1 -DHAVE_BOTH_TIME_H=1 -DHAVE_SYS_BSDTYPES_H=0 -DHAVE_SYS_FILE_H=1 -DHAVE_SYS_MKDEV_H=0 -DHAVE_SYS_MMAN_H=1 -DHAVE_SYS_PARAM_H=1 -DHAVE_SYS_RESOURCE_H=1 -DHAVE_SYS_SELECT_H=1 -DHAVE_SYS_SYSMACROS_H=1 -DHAVE_BSTRING_H=0 -DHAVE_GRP_H=1 -DHAVE_IO_H=0 -DHAVE_LIBGEN_H=1 -DHAVE_LIBUTIL_H=0 -DHAVE_PATHS_H=1 -DHAVE_STDINT_H=1 -DHAVE_STRINGS_H=1 -DHAVE_TERMIOS_H=1 -DHAVE_ULIMIT_H=1 -DHAVE_VALUES_H=1 -DHAVE_CAN_INTTYPES=1 -DHAVE_CAN_UCBINTS=1 -DHAVE_CAN_INT8TYPE=1 -DHAVE_CAN_UCBINT8=1 -DHAVE_RLIM_T=1 -DHAVE_SIG_T=1 -DHAVE_SYS_ERRLIST=1 -DHAVE_SYS_SIGNAME=0 -DHAVE_SYS_SIGLIST=1 -DHAVE_FLOCK=1 -DHAVE_LOCK_FCNTL=1 -DHAVE_GETRUSAGE=1 -DHAVE_GETSID=1 -DHAVE_GETTIMEOFDAY=1 -DHAVE_KILLPG=1 -DHAVE_MEMMOVE=1 -DHAVE_MKNOD=0 -DHAVE_MMAP=1 -DHAVE_NICE=1 -DHAVE_REVOKE=0 -DHAVE_SETLOCALE_CTYPE=1 -DHAVE_LANGINFO_CODESET=1 -DHAVE_SELECT=1 -DHAVE_SETRESUGID=1 -DHAVE_SETGROUPS=1 -DHAVE_STRERROR=0 -DHAVE_STRSIGNAL=0 -DHAVE_STRLCPY=0 -DHAVE_FLOCK_DECL=1 -DHAVE_REVOKE_DECL=1 -DHAVE_SYS_ERRLIST_DECL=1 -DHAVE_SYS_SIGLIST_DECL=1 -DHAVE_PERSISTENT_HISTORY=1 -DMKSH_BUILD_R=521 -c eval.c || exit 1
cc -std=c99 -pthread -Wno-deprecated-declarations -fno-asynchronous-unwind-tables -fno-strict-aliasing -fstack-protector-strong -Wall -fwrapv -I.  -DMKSH_BUILDSH -D_GNU_SOURCE -DSETUID_CAN_FAIL_WITH_EAGAIN -DHAVE_ATTRIBUTE_BOUNDED=0 -DHAVE_ATTRIBUTE_FORMAT=1 -DHAVE_ATTRIBUTE_NORETURN=1 -DHAVE_ATTRIBUTE_PURE=1 -DHAVE_ATTRIBUTE_UNUSED=1 -DHAVE_ATTRIBUTE_USED=1 -DHAVE_SYS_TIME_H=1 -DHAVE_TIME_H=1 -DHAVE_BOTH_TIME_H=1 -DHAVE_SYS_BSDTYPES_H=0 -DHAVE_SYS_FILE_H=1 -DHAVE_SYS_MKDEV_H=0 -DHAVE_SYS_MMAN_H=1 -DHAVE_SYS_PARAM_H=1 -DHAVE_SYS_RESOURCE_H=1 -DHAVE_SYS_SELECT_H=1 -DHAVE_SYS_SYSMACROS_H=1 -DHAVE_BSTRING_H=0 -DHAVE_GRP_H=1 -DHAVE_IO_H=0 -DHAVE_LIBGEN_H=1 -DHAVE_LIBUTIL_H=0 -DHAVE_PATHS_H=1 -DHAVE_STDINT_H=1 -DHAVE_STRINGS_H=1 -DHAVE_TERMIOS_H=1 -DHAVE_ULIMIT_H=1 -DHAVE_VALUES_H=1 -DHAVE_CAN_INTTYPES=1 -DHAVE_CAN_UCBINTS=1 -DHAVE_CAN_INT8TYPE=1 -DHAVE_CAN_UCBINT8=1 -DHAVE_RLIM_T=1 -DHAVE_SIG_T=1 -DHAVE_SYS_ERRLIST=1 -DHAVE_SYS_SIGNAME=0 -DHAVE_SYS_SIGLIST=1 -DHAVE_FLOCK=1 -DHAVE_LOCK_FCNTL=1 -DHAVE_GETRUSAGE=1 -DHAVE_GETSID=1 -DHAVE_GETTIMEOFDAY=1 -DHAVE_KILLPG=1 -DHAVE_MEMMOVE=1 -DHAVE_MKNOD=0 -DHAVE_MMAP=1 -DHAVE_NICE=1 -DHAVE_REVOKE=0 -DHAVE_SETLOCALE_CTYPE=1 -DHAVE_LANGINFO_CODESET=1 -DHAVE_SELECT=1 -DHAVE_SETRESUGID=1 -DHAVE_SETGROUPS=1 -DHAVE_STRERROR=0 -DHAVE_STRSIGNAL=0 -DHAVE_STRLCPY=0 -DHAVE_FLOCK_DECL=1 -DHAVE_REVOKE_DECL=1 -DHAVE_SYS_ERRLIST_DECL=1 -DHAVE_SYS_SIGLIST_DECL=1 -DHAVE_PERSISTENT_HISTORY=1 -DMKSH_BUILD_R=521 -c exec.c || exit 1
cc -std=c99 -pthread -Wno-deprecated-declarations -fno-asynchronous-unwind-tables -fno-strict-aliasing -fstack-protector-strong -Wall -fwrapv -I.  -DMKSH_BUILDSH -D_GNU_SOURCE -DSETUID_CAN_FAIL_WITH_EAGAIN -DHAVE_ATTRIBUTE_BOUNDED=0 -DHAVE_ATTRIBUTE_FORMAT=1 -DHAVE_ATTRIBUTE_NORETURN=1 -DHAVE_ATTRIBUTE_PURE=1 -DHAVE_ATTRIBUTE_UNUSED=1 -DHAVE_ATTRIBUTE_USED=1 -DHAVE_SYS_TIME_H=1 -DHAVE_TIME_H=1 -DHAVE_BOTH_TIME_H=1 -DHAVE_SYS_BSDTYPES_H=0 -DHAVE_SYS_FILE_H=1 -DHAVE_SYS_MKDEV_H=0 -DHAVE_SYS_MMAN_H=1 -DHAVE_SYS_PARAM_H=1 -DHAVE_SYS_RESOURCE_H=1 -DHAVE_SYS_SELECT_H=1 -DHAVE_SYS_SYSMACROS_H=1 -DHAVE_BSTRING_H=0 -DHAVE_GRP_H=1 -DHAVE_IO_H=0 -DHAVE_LIBGEN_H=1 -DHAVE_LIBUTIL_H=0 -DHAVE_PATHS_H=1 -DHAVE_STDINT_H=1 -DHAVE_STRINGS_H=1 -DHAVE_TERMIOS_H=1 -DHAVE_ULIMIT_H=1 -DHAVE_VALUES_H=1 -DHAVE_CAN_INTTYPES=1 -DHAVE_CAN_UCBINTS=1 -DHAVE_CAN_INT8TYPE=1 -DHAVE_CAN_UCBINT8=1 -DHAVE_RLIM_T=1 -DHAVE_SIG_T=1 -DHAVE_SYS_ERRLIST=1 -DHAVE_SYS_SIGNAME=0 -DHAVE_SYS_SIGLIST=1 -DHAVE_FLOCK=1 -DHAVE_LOCK_FCNTL=1 -DHAVE_GETRUSAGE=1 -DHAVE_GETSID=1 -DHAVE_GETTIMEOFDAY=1 -DHAVE_KILLPG=1 -DHAVE_MEMMOVE=1 -DHAVE_MKNOD=0 -DHAVE_MMAP=1 -DHAVE_NICE=1 -DHAVE_REVOKE=0 -DHAVE_SETLOCALE_CTYPE=1 -DHAVE_LANGINFO_CODESET=1 -DHAVE_SELECT=1 -DHAVE_SETRESUGID=1 -DHAVE_SETGROUPS=1 -DHAVE_STRERROR=0 -DHAVE_STRSIGNAL=0 -DHAVE_STRLCPY=0 -DHAVE_FLOCK_DECL=1 -DHAVE_REVOKE_DECL=1 -DHAVE_SYS_ERRLIST_DECL=1 -DHAVE_SYS_SIGLIST_DECL=1 -DHAVE_PERSISTENT_HISTORY=1 -DMKSH_BUILD_R=521 -c expr.c || exit 1
cc -std=c99 -pthread -Wno-deprecated-declarations -fno-asynchronous-unwind-tables -fno-strict-aliasing -fstack-protector-strong -Wall -fwrapv -I.  -DMKSH_BUILDSH -D_GNU_SOURCE -DSETUID_CAN_FAIL_WITH_EAGAIN -DHAVE_ATTRIBUTE_BOUNDED=0 -DHAVE_ATTRIBUTE_FORMAT=1 -DHAVE_ATTRIBUTE_NORETURN=1 -DHAVE_ATTRIBUTE_PURE=1 -DHAVE_ATTRIBUTE_UNUSED=1 -DHAVE_ATTRIBUTE_USED=1 -DHAVE_SYS_TIME_H=1 -DHAVE_TIME_H=1 -DHAVE_BOTH_TIME_H=1 -DHAVE_SYS_BSDTYPES_H=0 -DHAVE_SYS_FILE_H=1 -DHAVE_SYS_MKDEV_H=0 -DHAVE_SYS_MMAN_H=1 -DHAVE_SYS_PARAM_H=1 -DHAVE_SYS_RESOURCE_H=1 -DHAVE_SYS_SELECT_H=1 -DHAVE_SYS_SYSMACROS_H=1 -DHAVE_BSTRING_H=0 -DHAVE_GRP_H=1 -DHAVE_IO_H=0 -DHAVE_LIBGEN_H=1 -DHAVE_LIBUTIL_H=0 -DHAVE_PATHS_H=1 -DHAVE_STDINT_H=1 -DHAVE_STRINGS_H=1 -DHAVE_TERMIOS_H=1 -DHAVE_ULIMIT_H=1 -DHAVE_VALUES_H=1 -DHAVE_CAN_INTTYPES=1 -DHAVE_CAN_UCBINTS=1 -DHAVE_CAN_INT8TYPE=1 -DHAVE_CAN_UCBINT8=1 -DHAVE_RLIM_T=1 -DHAVE_SIG_T=1 -DHAVE_SYS_ERRLIST=1 -DHAVE_SYS_SIGNAME=0 -DHAVE_SYS_SIGLIST=1 -DHAVE_FLOCK=1 -DHAVE_LOCK_FCNTL=1 -DHAVE_GETRUSAGE=1 -DHAVE_GETSID=1 -DHAVE_GETTIMEOFDAY=1 -DHAVE_KILLPG=1 -DHAVE_MEMMOVE=1 -DHAVE_MKNOD=0 -DHAVE_MMAP=1 -DHAVE_NICE=1 -DHAVE_REVOKE=0 -DHAVE_SETLOCALE_CTYPE=1 -DHAVE_LANGINFO_CODESET=1 -DHAVE_SELECT=1 -DHAVE_SETRESUGID=1 -DHAVE_SETGROUPS=1 -DHAVE_STRERROR=0 -DHAVE_STRSIGNAL=0 -DHAVE_STRLCPY=0 -DHAVE_FLOCK_DECL=1 -DHAVE_REVOKE_DECL=1 -DHAVE_SYS_ERRLIST_DECL=1 -DHAVE_SYS_SIGLIST_DECL=1 -DHAVE_PERSISTENT_HISTORY=1 -DMKSH_BUILD_R=521 -c funcs.c || exit 1
cc -std=c99 -pthread -Wno-deprecated-declarations -fno-asynchronous-unwind-tables -fno-strict-aliasing -fstack-protector-strong -Wall -fwrapv -I.  -DMKSH_BUILDSH -D_GNU_SOURCE -DSETUID_CAN_FAIL_WITH_EAGAIN -DHAVE_ATTRIBUTE_BOUNDED=0 -DHAVE_ATTRIBUTE_FORMAT=1 -DHAVE_ATTRIBUTE_NORETURN=1 -DHAVE_ATTRIBUTE_PURE=1 -DHAVE_ATTRIBUTE_UNUSED=1 -DHAVE_ATTRIBUTE_USED=1 -DHAVE_SYS_TIME_H=1 -DHAVE_TIME_H=1 -DHAVE_BOTH_TIME_H=1 -DHAVE_SYS_BSDTYPES_H=0 -DHAVE_SYS_FILE_H=1 -DHAVE_SYS_MKDEV_H=0 -DHAVE_SYS_MMAN_H=1 -DHAVE_SYS_PARAM_H=1 -DHAVE_SYS_RESOURCE_H=1 -DHAVE_SYS_SELECT_H=1 -DHAVE_SYS_SYSMACROS_H=1 -DHAVE_BSTRING_H=0 -DHAVE_GRP_H=1 -DHAVE_IO_H=0 -DHAVE_LIBGEN_H=1 -DHAVE_LIBUTIL_H=0 -DHAVE_PATHS_H=1 -DHAVE_STDINT_H=1 -DHAVE_STRINGS_H=1 -DHAVE_TERMIOS_H=1 -DHAVE_ULIMIT_H=1 -DHAVE_VALUES_H=1 -DHAVE_CAN_INTTYPES=1 -DHAVE_CAN_UCBINTS=1 -DHAVE_CAN_INT8TYPE=1 -DHAVE_CAN_UCBINT8=1 -DHAVE_RLIM_T=1 -DHAVE_SIG_T=1 -DHAVE_SYS_ERRLIST=1 -DHAVE_SYS_SIGNAME=0 -DHAVE_SYS_SIGLIST=1 -DHAVE_FLOCK=1 -DHAVE_LOCK_FCNTL=1 -DHAVE_GETRUSAGE=1 -DHAVE_GETSID=1 -DHAVE_GETTIMEOFDAY=1 -DHAVE_KILLPG=1 -DHAVE_MEMMOVE=1 -DHAVE_MKNOD=0 -DHAVE_MMAP=1 -DHAVE_NICE=1 -DHAVE_REVOKE=0 -DHAVE_SETLOCALE_CTYPE=1 -DHAVE_LANGINFO_CODESET=1 -DHAVE_SELECT=1 -DHAVE_SETRESUGID=1 -DHAVE_SETGROUPS=1 -DHAVE_STRERROR=0 -DHAVE_STRSIGNAL=0 -DHAVE_STRLCPY=0 -DHAVE_FLOCK_DECL=1 -DHAVE_REVOKE_DECL=1 -DHAVE_SYS_ERRLIST_DECL=1 -DHAVE_SYS_SIGLIST_DECL=1 -DHAVE_PERSISTENT_HISTORY=1 -DMKSH_BUILD_R=521 -c histrap.c || exit 1
cc -std=c99 -pthread -Wno-deprecated-declarations -fno-asynchronous-unwind-tables -fno-strict-aliasing -fstack-protector-strong -Wall -fwrapv -I.  -DMKSH_BUILDSH -D_GNU_SOURCE -DSETUID_CAN_FAIL_WITH_EAGAIN -DHAVE_ATTRIBUTE_BOUNDED=0 -DHAVE_ATTRIBUTE_FORMAT=1 -DHAVE_ATTRIBUTE_NORETURN=1 -DHAVE_ATTRIBUTE_PURE=1 -DHAVE_ATTRIBUTE_UNUSED=1 -DHAVE_ATTRIBUTE_USED=1 -DHAVE_SYS_TIME_H=1 -DHAVE_TIME_H=1 -DHAVE_BOTH_TIME_H=1 -DHAVE_SYS_BSDTYPES_H=0 -DHAVE_SYS_FILE_H=1 -DHAVE_SYS_MKDEV_H=0 -DHAVE_SYS_MMAN_H=1 -DHAVE_SYS_PARAM_H=1 -DHAVE_SYS_RESOURCE_H=1 -DHAVE_SYS_SELECT_H=1 -DHAVE_SYS_SYSMACROS_H=1 -DHAVE_BSTRING_H=0 -DHAVE_GRP_H=1 -DHAVE_IO_H=0 -DHAVE_LIBGEN_H=1 -DHAVE_LIBUTIL_H=0 -DHAVE_PATHS_H=1 -DHAVE_STDINT_H=1 -DHAVE_STRINGS_H=1 -DHAVE_TERMIOS_H=1 -DHAVE_ULIMIT_H=1 -DHAVE_VALUES_H=1 -DHAVE_CAN_INTTYPES=1 -DHAVE_CAN_UCBINTS=1 -DHAVE_CAN_INT8TYPE=1 -DHAVE_CAN_UCBINT8=1 -DHAVE_RLIM_T=1 -DHAVE_SIG_T=1 -DHAVE_SYS_ERRLIST=1 -DHAVE_SYS_SIGNAME=0 -DHAVE_SYS_SIGLIST=1 -DHAVE_FLOCK=1 -DHAVE_LOCK_FCNTL=1 -DHAVE_GETRUSAGE=1 -DHAVE_GETSID=1 -DHAVE_GETTIMEOFDAY=1 -DHAVE_KILLPG=1 -DHAVE_MEMMOVE=1 -DHAVE_MKNOD=0 -DHAVE_MMAP=1 -DHAVE_NICE=1 -DHAVE_REVOKE=0 -DHAVE_SETLOCALE_CTYPE=1 -DHAVE_LANGINFO_CODESET=1 -DHAVE_SELECT=1 -DHAVE_SETRESUGID=1 -DHAVE_SETGROUPS=1 -DHAVE_STRERROR=0 -DHAVE_STRSIGNAL=0 -DHAVE_STRLCPY=0 -DHAVE_FLOCK_DECL=1 -DHAVE_REVOKE_DECL=1 -DHAVE_SYS_ERRLIST_DECL=1 -DHAVE_SYS_SIGLIST_DECL=1 -DHAVE_PERSISTENT_HISTORY=1 -DMKSH_BUILD_R=521 -c jobs.c || exit 1
cc -std=c99 -pthread -Wno-deprecated-declarations -fno-asynchronous-unwind-tables -fno-strict-aliasing -fstack-protector-strong -Wall -fwrapv -I.  -DMKSH_BUILDSH -D_GNU_SOURCE -DSETUID_CAN_FAIL_WITH_EAGAIN -DHAVE_ATTRIBUTE_BOUNDED=0 -DHAVE_ATTRIBUTE_FORMAT=1 -DHAVE_ATTRIBUTE_NORETURN=1 -DHAVE_ATTRIBUTE_PURE=1 -DHAVE_ATTRIBUTE_UNUSED=1 -DHAVE_ATTRIBUTE_USED=1 -DHAVE_SYS_TIME_H=1 -DHAVE_TIME_H=1 -DHAVE_BOTH_TIME_H=1 -DHAVE_SYS_BSDTYPES_H=0 -DHAVE_SYS_FILE_H=1 -DHAVE_SYS_MKDEV_H=0 -DHAVE_SYS_MMAN_H=1 -DHAVE_SYS_PARAM_H=1 -DHAVE_SYS_RESOURCE_H=1 -DHAVE_SYS_SELECT_H=1 -DHAVE_SYS_SYSMACROS_H=1 -DHAVE_BSTRING_H=0 -DHAVE_GRP_H=1 -DHAVE_IO_H=0 -DHAVE_LIBGEN_H=1 -DHAVE_LIBUTIL_H=0 -DHAVE_PATHS_H=1 -DHAVE_STDINT_H=1 -DHAVE_STRINGS_H=1 -DHAVE_TERMIOS_H=1 -DHAVE_ULIMIT_H=1 -DHAVE_VALUES_H=1 -DHAVE_CAN_INTTYPES=1 -DHAVE_CAN_UCBINTS=1 -DHAVE_CAN_INT8TYPE=1 -DHAVE_CAN_UCBINT8=1 -DHAVE_RLIM_T=1 -DHAVE_SIG_T=1 -DHAVE_SYS_ERRLIST=1 -DHAVE_SYS_SIGNAME=0 -DHAVE_SYS_SIGLIST=1 -DHAVE_FLOCK=1 -DHAVE_LOCK_FCNTL=1 -DHAVE_GETRUSAGE=1 -DHAVE_GETSID=1 -DHAVE_GETTIMEOFDAY=1 -DHAVE_KILLPG=1 -DHAVE_MEMMOVE=1 -DHAVE_MKNOD=0 -DHAVE_MMAP=1 -DHAVE_NICE=1 -DHAVE_REVOKE=0 -DHAVE_SETLOCALE_CTYPE=1 -DHAVE_LANGINFO_CODESET=1 -DHAVE_SELECT=1 -DHAVE_SETRESUGID=1 -DHAVE_SETGROUPS=1 -DHAVE_STRERROR=0 -DHAVE_STRSIGNAL=0 -DHAVE_STRLCPY=0 -DHAVE_FLOCK_DECL=1 -DHAVE_REVOKE_DECL=1 -DHAVE_SYS_ERRLIST_DECL=1 -DHAVE_SYS_SIGLIST_DECL=1 -DHAVE_PERSISTENT_HISTORY=1 -DMKSH_BUILD_R=521 -c lex.c || exit 1
cc -std=c99 -pthread -Wno-deprecated-declarations -fno-asynchronous-unwind-tables -fno-strict-aliasing -fstack-protector-strong -Wall -fwrapv -I.  -DMKSH_BUILDSH -D_GNU_SOURCE -DSETUID_CAN_FAIL_WITH_EAGAIN -DHAVE_ATTRIBUTE_BOUNDED=0 -DHAVE_ATTRIBUTE_FORMAT=1 -DHAVE_ATTRIBUTE_NORETURN=1 -DHAVE_ATTRIBUTE_PURE=1 -DHAVE_ATTRIBUTE_UNUSED=1 -DHAVE_ATTRIBUTE_USED=1 -DHAVE_SYS_TIME_H=1 -DHAVE_TIME_H=1 -DHAVE_BOTH_TIME_H=1 -DHAVE_SYS_BSDTYPES_H=0 -DHAVE_SYS_FILE_H=1 -DHAVE_SYS_MKDEV_H=0 -DHAVE_SYS_MMAN_H=1 -DHAVE_SYS_PARAM_H=1 -DHAVE_SYS_RESOURCE_H=1 -DHAVE_SYS_SELECT_H=1 -DHAVE_SYS_SYSMACROS_H=1 -DHAVE_BSTRING_H=0 -DHAVE_GRP_H=1 -DHAVE_IO_H=0 -DHAVE_LIBGEN_H=1 -DHAVE_LIBUTIL_H=0 -DHAVE_PATHS_H=1 -DHAVE_STDINT_H=1 -DHAVE_STRINGS_H=1 -DHAVE_TERMIOS_H=1 -DHAVE_ULIMIT_H=1 -DHAVE_VALUES_H=1 -DHAVE_CAN_INTTYPES=1 -DHAVE_CAN_UCBINTS=1 -DHAVE_CAN_INT8TYPE=1 -DHAVE_CAN_UCBINT8=1 -DHAVE_RLIM_T=1 -DHAVE_SIG_T=1 -DHAVE_SYS_ERRLIST=1 -DHAVE_SYS_SIGNAME=0 -DHAVE_SYS_SIGLIST=1 -DHAVE_FLOCK=1 -DHAVE_LOCK_FCNTL=1 -DHAVE_GETRUSAGE=1 -DHAVE_GETSID=1 -DHAVE_GETTIMEOFDAY=1 -DHAVE_KILLPG=1 -DHAVE_MEMMOVE=1 -DHAVE_MKNOD=0 -DHAVE_MMAP=1 -DHAVE_NICE=1 -DHAVE_REVOKE=0 -DHAVE_SETLOCALE_CTYPE=1 -DHAVE_LANGINFO_CODESET=1 -DHAVE_SELECT=1 -DHAVE_SETRESUGID=1 -DHAVE_SETGROUPS=1 -DHAVE_STRERROR=0 -DHAVE_STRSIGNAL=0 -DHAVE_STRLCPY=0 -DHAVE_FLOCK_DECL=1 -DHAVE_REVOKE_DECL=1 -DHAVE_SYS_ERRLIST_DECL=1 -DHAVE_SYS_SIGLIST_DECL=1 -DHAVE_PERSISTENT_HISTORY=1 -DMKSH_BUILD_R=521 -c main.c || exit 1
cc -std=c99 -pthread -Wno-deprecated-declarations -fno-asynchronous-unwind-tables -fno-strict-aliasing -fstack-protector-strong -Wall -fwrapv -I.  -DMKSH_BUILDSH -D_GNU_SOURCE -DSETUID_CAN_FAIL_WITH_EAGAIN -DHAVE_ATTRIBUTE_BOUNDED=0 -DHAVE_ATTRIBUTE_FORMAT=1 -DHAVE_ATTRIBUTE_NORETURN=1 -DHAVE_ATTRIBUTE_PURE=1 -DHAVE_ATTRIBUTE_UNUSED=1 -DHAVE_ATTRIBUTE_USED=1 -DHAVE_SYS_TIME_H=1 -DHAVE_TIME_H=1 -DHAVE_BOTH_TIME_H=1 -DHAVE_SYS_BSDTYPES_H=0 -DHAVE_SYS_FILE_H=1 -DHAVE_SYS_MKDEV_H=0 -DHAVE_SYS_MMAN_H=1 -DHAVE_SYS_PARAM_H=1 -DHAVE_SYS_RESOURCE_H=1 -DHAVE_SYS_SELECT_H=1 -DHAVE_SYS_SYSMACROS_H=1 -DHAVE_BSTRING_H=0 -DHAVE_GRP_H=1 -DHAVE_IO_H=0 -DHAVE_LIBGEN_H=1 -DHAVE_LIBUTIL_H=0 -DHAVE_PATHS_H=1 -DHAVE_STDINT_H=1 -DHAVE_STRINGS_H=1 -DHAVE_TERMIOS_H=1 -DHAVE_ULIMIT_H=1 -DHAVE_VALUES_H=1 -DHAVE_CAN_INTTYPES=1 -DHAVE_CAN_UCBINTS=1 -DHAVE_CAN_INT8TYPE=1 -DHAVE_CAN_UCBINT8=1 -DHAVE_RLIM_T=1 -DHAVE_SIG_T=1 -DHAVE_SYS_ERRLIST=1 -DHAVE_SYS_SIGNAME=0 -DHAVE_SYS_SIGLIST=1 -DHAVE_FLOCK=1 -DHAVE_LOCK_FCNTL=1 -DHAVE_GETRUSAGE=1 -DHAVE_GETSID=1 -DHAVE_GETTIMEOFDAY=1 -DHAVE_KILLPG=1 -DHAVE_MEMMOVE=1 -DHAVE_MKNOD=0 -DHAVE_MMAP=1 -DHAVE_NICE=1 -DHAVE_REVOKE=0 -DHAVE_SETLOCALE_CTYPE=1 -DHAVE_LANGINFO_CODESET=1 -DHAVE_SELECT=1 -DHAVE_SETRESUGID=1 -DHAVE_SETGROUPS=1 -DHAVE_STRERROR=0 -DHAVE_STRSIGNAL=0 -DHAVE_STRLCPY=0 -DHAVE_FLOCK_DECL=1 -DHAVE_REVOKE_DECL=1 -DHAVE_SYS_ERRLIST_DECL=1 -DHAVE_SYS_SIGLIST_DECL=1 -DHAVE_PERSISTENT_HISTORY=1 -DMKSH_BUILD_R=521 -c misc.c || exit 1
cc -std=c99 -pthread -Wno-deprecated-declarations -fno-asynchronous-unwind-tables -fno-strict-aliasing -fstack-protector-strong -Wall -fwrapv -I.  -DMKSH_BUILDSH -D_GNU_SOURCE -DSETUID_CAN_FAIL_WITH_EAGAIN -DHAVE_ATTRIBUTE_BOUNDED=0 -DHAVE_ATTRIBUTE_FORMAT=1 -DHAVE_ATTRIBUTE_NORETURN=1 -DHAVE_ATTRIBUTE_PURE=1 -DHAVE_ATTRIBUTE_UNUSED=1 -DHAVE_ATTRIBUTE_USED=1 -DHAVE_SYS_TIME_H=1 -DHAVE_TIME_H=1 -DHAVE_BOTH_TIME_H=1 -DHAVE_SYS_BSDTYPES_H=0 -DHAVE_SYS_FILE_H=1 -DHAVE_SYS_MKDEV_H=0 -DHAVE_SYS_MMAN_H=1 -DHAVE_SYS_PARAM_H=1 -DHAVE_SYS_RESOURCE_H=1 -DHAVE_SYS_SELECT_H=1 -DHAVE_SYS_SYSMACROS_H=1 -DHAVE_BSTRING_H=0 -DHAVE_GRP_H=1 -DHAVE_IO_H=0 -DHAVE_LIBGEN_H=1 -DHAVE_LIBUTIL_H=0 -DHAVE_PATHS_H=1 -DHAVE_STDINT_H=1 -DHAVE_STRINGS_H=1 -DHAVE_TERMIOS_H=1 -DHAVE_ULIMIT_H=1 -DHAVE_VALUES_H=1 -DHAVE_CAN_INTTYPES=1 -DHAVE_CAN_UCBINTS=1 -DHAVE_CAN_INT8TYPE=1 -DHAVE_CAN_UCBINT8=1 -DHAVE_RLIM_T=1 -DHAVE_SIG_T=1 -DHAVE_SYS_ERRLIST=1 -DHAVE_SYS_SIGNAME=0 -DHAVE_SYS_SIGLIST=1 -DHAVE_FLOCK=1 -DHAVE_LOCK_FCNTL=1 -DHAVE_GETRUSAGE=1 -DHAVE_GETSID=1 -DHAVE_GETTIMEOFDAY=1 -DHAVE_KILLPG=1 -DHAVE_MEMMOVE=1 -DHAVE_MKNOD=0 -DHAVE_MMAP=1 -DHAVE_NICE=1 -DHAVE_REVOKE=0 -DHAVE_SETLOCALE_CTYPE=1 -DHAVE_LANGINFO_CODESET=1 -DHAVE_SELECT=1 -DHAVE_SETRESUGID=1 -DHAVE_SETGROUPS=1 -DHAVE_STRERROR=0 -DHAVE_STRSIGNAL=0 -DHAVE_STRLCPY=0 -DHAVE_FLOCK_DECL=1 -DHAVE_REVOKE_DECL=1 -DHAVE_SYS_ERRLIST_DECL=1 -DHAVE_SYS_SIGLIST_DECL=1 -DHAVE_PERSISTENT_HISTORY=1 -DMKSH_BUILD_R=521 -c shf.c || exit 1
cc -std=c99 -pthread -Wno-deprecated-declarations -fno-asynchronous-unwind-tables -fno-strict-aliasing -fstack-protector-strong -Wall -fwrapv -I.  -DMKSH_BUILDSH -D_GNU_SOURCE -DSETUID_CAN_FAIL_WITH_EAGAIN -DHAVE_ATTRIBUTE_BOUNDED=0 -DHAVE_ATTRIBUTE_FORMAT=1 -DHAVE_ATTRIBUTE_NORETURN=1 -DHAVE_ATTRIBUTE_PURE=1 -DHAVE_ATTRIBUTE_UNUSED=1 -DHAVE_ATTRIBUTE_USED=1 -DHAVE_SYS_TIME_H=1 -DHAVE_TIME_H=1 -DHAVE_BOTH_TIME_H=1 -DHAVE_SYS_BSDTYPES_H=0 -DHAVE_SYS_FILE_H=1 -DHAVE_SYS_MKDEV_H=0 -DHAVE_SYS_MMAN_H=1 -DHAVE_SYS_PARAM_H=1 -DHAVE_SYS_RESOURCE_H=1 -DHAVE_SYS_SELECT_H=1 -DHAVE_SYS_SYSMACROS_H=1 -DHAVE_BSTRING_H=0 -DHAVE_GRP_H=1 -DHAVE_IO_H=0 -DHAVE_LIBGEN_H=1 -DHAVE_LIBUTIL_H=0 -DHAVE_PATHS_H=1 -DHAVE_STDINT_H=1 -DHAVE_STRINGS_H=1 -DHAVE_TERMIOS_H=1 -DHAVE_ULIMIT_H=1 -DHAVE_VALUES_H=1 -DHAVE_CAN_INTTYPES=1 -DHAVE_CAN_UCBINTS=1 -DHAVE_CAN_INT8TYPE=1 -DHAVE_CAN_UCBINT8=1 -DHAVE_RLIM_T=1 -DHAVE_SIG_T=1 -DHAVE_SYS_ERRLIST=1 -DHAVE_SYS_SIGNAME=0 -DHAVE_SYS_SIGLIST=1 -DHAVE_FLOCK=1 -DHAVE_LOCK_FCNTL=1 -DHAVE_GETRUSAGE=1 -DHAVE_GETSID=1 -DHAVE_GETTIMEOFDAY=1 -DHAVE_KILLPG=1 -DHAVE_MEMMOVE=1 -DHAVE_MKNOD=0 -DHAVE_MMAP=1 -DHAVE_NICE=1 -DHAVE_REVOKE=0 -DHAVE_SETLOCALE_CTYPE=1 -DHAVE_LANGINFO_CODESET=1 -DHAVE_SELECT=1 -DHAVE_SETRESUGID=1 -DHAVE_SETGROUPS=1 -DHAVE_STRERROR=0 -DHAVE_STRSIGNAL=0 -DHAVE_STRLCPY=0 -DHAVE_FLOCK_DECL=1 -DHAVE_REVOKE_DECL=1 -DHAVE_SYS_ERRLIST_DECL=1 -DHAVE_SYS_SIGLIST_DECL=1 -DHAVE_PERSISTENT_HISTORY=1 -DMKSH_BUILD_R=521 -c syn.c || exit 1
cc -std=c99 -pthread -Wno-deprecated-declarations -fno-asynchronous-unwind-tables -fno-strict-aliasing -fstack-protector-strong -Wall -fwrapv -I.  -DMKSH_BUILDSH -D_GNU_SOURCE -DSETUID_CAN_FAIL_WITH_EAGAIN -DHAVE_ATTRIBUTE_BOUNDED=0 -DHAVE_ATTRIBUTE_FORMAT=1 -DHAVE_ATTRIBUTE_NORETURN=1 -DHAVE_ATTRIBUTE_PURE=1 -DHAVE_ATTRIBUTE_UNUSED=1 -DHAVE_ATTRIBUTE_USED=1 -DHAVE_SYS_TIME_H=1 -DHAVE_TIME_H=1 -DHAVE_BOTH_TIME_H=1 -DHAVE_SYS_BSDTYPES_H=0 -DHAVE_SYS_FILE_H=1 -DHAVE_SYS_MKDEV_H=0 -DHAVE_SYS_MMAN_H=1 -DHAVE_SYS_PARAM_H=1 -DHAVE_SYS_RESOURCE_H=1 -DHAVE_SYS_SELECT_H=1 -DHAVE_SYS_SYSMACROS_H=1 -DHAVE_BSTRING_H=0 -DHAVE_GRP_H=1 -DHAVE_IO_H=0 -DHAVE_LIBGEN_H=1 -DHAVE_LIBUTIL_H=0 -DHAVE_PATHS_H=1 -DHAVE_STDINT_H=1 -DHAVE_STRINGS_H=1 -DHAVE_TERMIOS_H=1 -DHAVE_ULIMIT_H=1 -DHAVE_VALUES_H=1 -DHAVE_CAN_INTTYPES=1 -DHAVE_CAN_UCBINTS=1 -DHAVE_CAN_INT8TYPE=1 -DHAVE_CAN_UCBINT8=1 -DHAVE_RLIM_T=1 -DHAVE_SIG_T=1 -DHAVE_SYS_ERRLIST=1 -DHAVE_SYS_SIGNAME=0 -DHAVE_SYS_SIGLIST=1 -DHAVE_FLOCK=1 -DHAVE_LOCK_FCNTL=1 -DHAVE_GETRUSAGE=1 -DHAVE_GETSID=1 -DHAVE_GETTIMEOFDAY=1 -DHAVE_KILLPG=1 -DHAVE_MEMMOVE=1 -DHAVE_MKNOD=0 -DHAVE_MMAP=1 -DHAVE_NICE=1 -DHAVE_REVOKE=0 -DHAVE_SETLOCALE_CTYPE=1 -DHAVE_LANGINFO_CODESET=1 -DHAVE_SELECT=1 -DHAVE_SETRESUGID=1 -DHAVE_SETGROUPS=1 -DHAVE_STRERROR=0 -DHAVE_STRSIGNAL=0 -DHAVE_STRLCPY=0 -DHAVE_FLOCK_DECL=1 -DHAVE_REVOKE_DECL=1 -DHAVE_SYS_ERRLIST_DECL=1 -DHAVE_SYS_SIGLIST_DECL=1 -DHAVE_PERSISTENT_HISTORY=1 -DMKSH_BUILD_R=521 -c tree.c || exit 1
cc -std=c99 -pthread -Wno-deprecated-declarations -fno-asynchronous-unwind-tables -fno-strict-aliasing -fstack-protector-strong -Wall -fwrapv -I.  -DMKSH_BUILDSH -D_GNU_SOURCE -DSETUID_CAN_FAIL_WITH_EAGAIN -DHAVE_ATTRIBUTE_BOUNDED=0 -DHAVE_ATTRIBUTE_FORMAT=1 -DHAVE_ATTRIBUTE_NORETURN=1 -DHAVE_ATTRIBUTE_PURE=1 -DHAVE_ATTRIBUTE_UNUSED=1 -DHAVE_ATTRIBUTE_USED=1 -DHAVE_SYS_TIME_H=1 -DHAVE_TIME_H=1 -DHAVE_BOTH_TIME_H=1 -DHAVE_SYS_BSDTYPES_H=0 -DHAVE_SYS_FILE_H=1 -DHAVE_SYS_MKDEV_H=0 -DHAVE_SYS_MMAN_H=1 -DHAVE_SYS_PARAM_H=1 -DHAVE_SYS_RESOURCE_H=1 -DHAVE_SYS_SELECT_H=1 -DHAVE_SYS_SYSMACROS_H=1 -DHAVE_BSTRING_H=0 -DHAVE_GRP_H=1 -DHAVE_IO_H=0 -DHAVE_LIBGEN_H=1 -DHAVE_LIBUTIL_H=0 -DHAVE_PATHS_H=1 -DHAVE_STDINT_H=1 -DHAVE_STRINGS_H=1 -DHAVE_TERMIOS_H=1 -DHAVE_ULIMIT_H=1 -DHAVE_VALUES_H=1 -DHAVE_CAN_INTTYPES=1 -DHAVE_CAN_UCBINTS=1 -DHAVE_CAN_INT8TYPE=1 -DHAVE_CAN_UCBINT8=1 -DHAVE_RLIM_T=1 -DHAVE_SIG_T=1 -DHAVE_SYS_ERRLIST=1 -DHAVE_SYS_SIGNAME=0 -DHAVE_SYS_SIGLIST=1 -DHAVE_FLOCK=1 -DHAVE_LOCK_FCNTL=1 -DHAVE_GETRUSAGE=1 -DHAVE_GETSID=1 -DHAVE_GETTIMEOFDAY=1 -DHAVE_KILLPG=1 -DHAVE_MEMMOVE=1 -DHAVE_MKNOD=0 -DHAVE_MMAP=1 -DHAVE_NICE=1 -DHAVE_REVOKE=0 -DHAVE_SETLOCALE_CTYPE=1 -DHAVE_LANGINFO_CODESET=1 -DHAVE_SELECT=1 -DHAVE_SETRESUGID=1 -DHAVE_SETGROUPS=1 -DHAVE_STRERROR=0 -DHAVE_STRSIGNAL=0 -DHAVE_STRLCPY=0 -DHAVE_FLOCK_DECL=1 -DHAVE_REVOKE_DECL=1 -DHAVE_SYS_ERRLIST_DECL=1 -DHAVE_SYS_SIGLIST_DECL=1 -DHAVE_PERSISTENT_HISTORY=1 -DMKSH_BUILD_R=521 -c var.c || exit 1
cc -std=c99 -pthread -Wno-deprecated-declarations -fno-asynchronous-unwind-tables -fno-strict-aliasing -fstack-protector-strong -Wall -fwrapv -I.  -DMKSH_BUILDSH -D_GNU_SOURCE -DSETUID_CAN_FAIL_WITH_EAGAIN -DHAVE_ATTRIBUTE_BOUNDED=0 -DHAVE_ATTRIBUTE_FORMAT=1 -DHAVE_ATTRIBUTE_NORETURN=1 -DHAVE_ATTRIBUTE_PURE=1 -DHAVE_ATTRIBUTE_UNUSED=1 -DHAVE_ATTRIBUTE_USED=1 -DHAVE_SYS_TIME_H=1 -DHAVE_TIME_H=1 -DHAVE_BOTH_TIME_H=1 -DHAVE_SYS_BSDTYPES_H=0 -DHAVE_SYS_FILE_H=1 -DHAVE_SYS_MKDEV_H=0 -DHAVE_SYS_MMAN_H=1 -DHAVE_SYS_PARAM_H=1 -DHAVE_SYS_RESOURCE_H=1 -DHAVE_SYS_SELECT_H=1 -DHAVE_SYS_SYSMACROS_H=1 -DHAVE_BSTRING_H=0 -DHAVE_GRP_H=1 -DHAVE_IO_H=0 -DHAVE_LIBGEN_H=1 -DHAVE_LIBUTIL_H=0 -DHAVE_PATHS_H=1 -DHAVE_STDINT_H=1 -DHAVE_STRINGS_H=1 -DHAVE_TERMIOS_H=1 -DHAVE_ULIMIT_H=1 -DHAVE_VALUES_H=1 -DHAVE_CAN_INTTYPES=1 -DHAVE_CAN_UCBINTS=1 -DHAVE_CAN_INT8TYPE=1 -DHAVE_CAN_UCBINT8=1 -DHAVE_RLIM_T=1 -DHAVE_SIG_T=1 -DHAVE_SYS_ERRLIST=1 -DHAVE_SYS_SIGNAME=0 -DHAVE_SYS_SIGLIST=1 -DHAVE_FLOCK=1 -DHAVE_LOCK_FCNTL=1 -DHAVE_GETRUSAGE=1 -DHAVE_GETSID=1 -DHAVE_GETTIMEOFDAY=1 -DHAVE_KILLPG=1 -DHAVE_MEMMOVE=1 -DHAVE_MKNOD=0 -DHAVE_MMAP=1 -DHAVE_NICE=1 -DHAVE_REVOKE=0 -DHAVE_SETLOCALE_CTYPE=1 -DHAVE_LANGINFO_CODESET=1 -DHAVE_SELECT=1 -DHAVE_SETRESUGID=1 -DHAVE_SETGROUPS=1 -DHAVE_STRERROR=0 -DHAVE_STRSIGNAL=0 -DHAVE_STRLCPY=0 -DHAVE_FLOCK_DECL=1 -DHAVE_REVOKE_DECL=1 -DHAVE_SYS_ERRLIST_DECL=1 -DHAVE_SYS_SIGLIST_DECL=1 -DHAVE_PERSISTENT_HISTORY=1 -DMKSH_BUILD_R=521 -c edit.c || exit 1
cc -std=c99 -pthread -Wno-deprecated-declarations -fno-asynchronous-unwind-tables -fno-strict-aliasing -fstack-protector-strong -Wall -fwrapv -I.  -DMKSH_BUILDSH -D_GNU_SOURCE -DSETUID_CAN_FAIL_WITH_EAGAIN -DHAVE_ATTRIBUTE_BOUNDED=0 -DHAVE_ATTRIBUTE_FORMAT=1 -DHAVE_ATTRIBUTE_NORETURN=1 -DHAVE_ATTRIBUTE_PURE=1 -DHAVE_ATTRIBUTE_UNUSED=1 -DHAVE_ATTRIBUTE_USED=1 -DHAVE_SYS_TIME_H=1 -DHAVE_TIME_H=1 -DHAVE_BOTH_TIME_H=1 -DHAVE_SYS_BSDTYPES_H=0 -DHAVE_SYS_FILE_H=1 -DHAVE_SYS_MKDEV_H=0 -DHAVE_SYS_MMAN_H=1 -DHAVE_SYS_PARAM_H=1 -DHAVE_SYS_RESOURCE_H=1 -DHAVE_SYS_SELECT_H=1 -DHAVE_SYS_SYSMACROS_H=1 -DHAVE_BSTRING_H=0 -DHAVE_GRP_H=1 -DHAVE_IO_H=0 -DHAVE_LIBGEN_H=1 -DHAVE_LIBUTIL_H=0 -DHAVE_PATHS_H=1 -DHAVE_STDINT_H=1 -DHAVE_STRINGS_H=1 -DHAVE_TERMIOS_H=1 -DHAVE_ULIMIT_H=1 -DHAVE_VALUES_H=1 -DHAVE_CAN_INTTYPES=1 -DHAVE_CAN_UCBINTS=1 -DHAVE_CAN_INT8TYPE=1 -DHAVE_CAN_UCBINT8=1 -DHAVE_RLIM_T=1 -DHAVE_SIG_T=1 -DHAVE_SYS_ERRLIST=1 -DHAVE_SYS_SIGNAME=0 -DHAVE_SYS_SIGLIST=1 -DHAVE_FLOCK=1 -DHAVE_LOCK_FCNTL=1 -DHAVE_GETRUSAGE=1 -DHAVE_GETSID=1 -DHAVE_GETTIMEOFDAY=1 -DHAVE_KILLPG=1 -DHAVE_MEMMOVE=1 -DHAVE_MKNOD=0 -DHAVE_MMAP=1 -DHAVE_NICE=1 -DHAVE_REVOKE=0 -DHAVE_SETLOCALE_CTYPE=1 -DHAVE_LANGINFO_CODESET=1 -DHAVE_SELECT=1 -DHAVE_SETRESUGID=1 -DHAVE_SETGROUPS=1 -DHAVE_STRERROR=0 -DHAVE_STRSIGNAL=0 -DHAVE_STRLCPY=0 -DHAVE_FLOCK_DECL=1 -DHAVE_REVOKE_DECL=1 -DHAVE_SYS_ERRLIST_DECL=1 -DHAVE_SYS_SIGLIST_DECL=1 -DHAVE_PERSISTENT_HISTORY=1 -DMKSH_BUILD_R=521 -c strlcpy.c || exit 1
tcfn=mksh
cc -std=c99 -pthread -Wno-deprecated-declarations -fno-asynchronous-unwind-tables -fno-strict-aliasing -fstack-protector-strong -Wall -fwrapv  -o $tcfn  lalloc.o eval.o exec.o expr.o funcs.o histrap.o jobs.o lex.o main.o misc.o shf.o syn.o tree.o var.o edit.o strlcpy.o  || for _f in ${tcfn}*; do case $_f in Build.sh|check.pl|check.t|dot.mkshrc|*.1|*.c|*.h|*.ico|*.opt) ;; *) rm -f "$_f" ;; esac; done
test -f $tcfn || exit 1; size $tcfn
