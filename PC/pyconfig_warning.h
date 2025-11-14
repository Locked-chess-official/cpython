/*
* This file is used to define macros that are not used in the Python source code.
* These macros are defined to warning users that they are not used.
* After a few version this file will not exist.
* Then use them will be a compilation error.
*/

#ifndef Py_PYCONFIG_WARNING_H
#define Py_PYCONFIG_WARNING_H

/* Cross-compiler warning macro */
#if defined(__GNUC__) || defined(__clang__)
#  define _Py_Config_Macro_Warning(msg) __attribute__((deprecated(msg)))
#elif defined(_MSC_VER)
#  define _Py_Config_Macro_Warning(msg) __declspec(deprecated(msg))
#else
#  define _Py_Config_Macro_Warning(msg)
#endif

#undef HAVE_X509_VERIFY_PARAM_SET1_HOST
#define HAVE_X509_VERIFY_PARAM_SET1_HOST _Py_Config_Macro_Warning("macro HAVE_X509_VERIFY_PARAM_SET1_HOST shouldn't be used") 1
#undef HAVE_DUP
#define HAVE_DUP _Py_Config_Macro_Warning("macro HAVE_DUP shouldn't be used") 1
#undef HAVE_SOCKET
#define HAVE_SOCKET _Py_Config_Macro_Warning("macro HAVE_SOCKET shouldn't be used") 1
#undef HAVE_SETSOCKOPT
#define HAVE_SETSOCKOPT _Py_Config_Macro_Warning("macro HAVE_SETSOCKOPT shouldn't be used") 1
#undef HAVE_SENDTO
#define HAVE_SENDTO _Py_Config_Macro_Warning("macro HAVE_SENDTO shouldn't be used") 1
#undef HAVE_RECVFROM
#define HAVE_RECVFROM _Py_Config_Macro_Warning("macro HAVE_RECVFROM shouldn't be used") 1
#undef HAVE_LISTEN
#define HAVE_LISTEN _Py_Config_Macro_Warning("macro HAVE_LISTEN shouldn't be used") 1
#undef HAVE_GETSOCKNAME
#define HAVE_GETSOCKNAME _Py_Config_Macro_Warning("macro HAVE_GETSOCKNAME shouldn't be used") 1
#undef HAVE_CONNECT
#define HAVE_CONNECT _Py_Config_Macro_Warning("macro HAVE_CONNECT shouldn't be used") 1
#undef HAVE_BIND
#define HAVE_BIND _Py_Config_Macro_Warning("macro HAVE_BIND shouldn't be used") 1
#undef HAVE_ACCEPT
#define HAVE_ACCEPT _Py_Config_Macro_Warning("macro HAVE_ACCEPT shouldn't be used") 1
#undef HAVE_INET_NTOA
#define HAVE_INET_NTOA _Py_Config_Macro_Warning("macro HAVE_INET_NTOA shouldn't be used") 1
#undef HAVE_INET_PTON
#define HAVE_INET_PTON _Py_Config_Macro_Warning("macro HAVE_INET_PTON shouldn't be used") 1
#undef HAVE_GETSERVBYPORT
#define HAVE_GETSERVBYPORT _Py_Config_Macro_Warning("macro HAVE_GETSERVBYPORT shouldn't be used") 1
#undef HAVE_GETSERVBYNAME
#define HAVE_GETSERVBYNAME _Py_Config_Macro_Warning("macro HAVE_GETSERVBYNAME shouldn't be used") 1
#undef HAVE_GETPROTOBYNAME
#define HAVE_GETPROTOBYNAME _Py_Config_Macro_Warning("macro HAVE_GETPROTOBYNAME shouldn't be used") 1
#undef HAVE_GETHOSTBYNAME
#define HAVE_GETHOSTBYNAME _Py_Config_Macro_Warning("macro HAVE_GETHOSTBYNAME shouldn't be used") 1
#undef HAVE_GETHOSTBYADDR
#define HAVE_GETHOSTBYADDR _Py_Config_Macro_Warning("macro HAVE_GETHOSTBYADDR shouldn't be used") 1
#undef HAVE_GETHOSTNAME
#define HAVE_GETHOSTNAME _Py_Config_Macro_Warning("macro HAVE_GETHOSTNAME shouldn't be used") 1
#undef HAVE_ERFC
#define HAVE_ERFC _Py_Config_Macro_Warning("macro HAVE_ERFC shouldn't be used") 1
#undef HAVE_ERF
#define HAVE_ERF _Py_Config_Macro_Warning("macro HAVE_ERF shouldn't be used") 1
#undef DOUBLE_IS_LITTLE_ENDIAN_IEEE754
#define DOUBLE_IS_LITTLE_ENDIAN_IEEE754 _Py_Config_Macro_Warning("macro DOUBLE_IS_LITTLE_ENDIAN_IEEE754 shouldn't be used") 1
#undef HAVE_LIBSOCKET
#define HAVE_LIBSOCKET _Py_Config_Macro_Warning("macro HAVE_LIBSOCKET shouldn't be used") 1
#undef SIZEOF_PID_T
#define SIZEOF_PID_T _Py_Config_Macro_Warning("macro SIZEOF_PID_T shouldn't be used") SIZEOF_INT
#undef SIZEOF__BOOL
#define SIZEOF__BOOL _Py_Config_Macro_Warning("macro SIZEOF__BOOL shouldn't be used") 1
#undef SIZEOF_WCHAR_T
#define SIZEOF_WCHAR_T _Py_Config_Macro_Warning("macro SIZEOF_WCHAR_T shouldn't be used") 2
#undef HAVE_WCHAR_H
#define HAVE_WCHAR_H _Py_Config_Macro_Warning("macro HAVE_WCHAR_H shouldn't be used") 1
#undef HAVE_SYS_TYPES_H
#define HAVE_SYS_TYPES_H _Py_Config_Macro_Warning("macro HAVE_SYS_TYPES_H shouldn't be used") 1
#undef HAVE_SYS_STAT_H
#define HAVE_SYS_STAT_H _Py_Config_Macro_Warning("macro HAVE_SYS_STAT_H shouldn't be used") 1
#undef HAVE_STDDEF_H
#define HAVE_STDDEF_H _Py_Config_Macro_Warning("macro HAVE_STDDEF_H shouldn't be used") 1
#undef HAVE_SIGNAL_H
#define HAVE_SIGNAL_H _Py_Config_Macro_Warning("macro HAVE_SIGNAL_H shouldn't be used") 1
#undef HAVE_PROCESS_H
#define HAVE_PROCESS_H _Py_Config_Macro_Warning("macro HAVE_PROCESS_H shouldn't be used") 1
#undef HAVE_FCNTL_H
#define HAVE_FCNTL_H _Py_Config_Macro_Warning("macro HAVE_FCNTL_H shouldn't be used") 1
#undef HAVE_ERRNO_H
#define HAVE_ERRNO_H _Py_Config_Macro_Warning("macro HAVE_ERRNO_H shouldn't be used") 1
#undef HAVE_ZLIB_COPY
#define HAVE_ZLIB_COPY _Py_Config_Macro_Warning("macro HAVE_ZLIB_COPY shouldn't be used") 1
#undef HAVE_WCSXFRM
#define HAVE_WCSXFRM _Py_Config_Macro_Warning("macro HAVE_WCSXFRM shouldn't be used") 1
#undef HAVE_WCSCOLL
#define HAVE_WCSCOLL _Py_Config_Macro_Warning("macro HAVE_WCSCOLL shouldn't be used") 1
#ifdef HAVE_WCSFTIME
#undef HAVE_WCSFTIME
#define HAVE_WCSFTIME _Py_Config_Macro_Warning("macro HAVE_WCSFTIME shouldn't be used") 1
#endif
#undef HAVE_UMASK
#define HAVE_UMASK _Py_Config_Macro_Warning("macro HAVE_UMASK shouldn't be used") 1
#undef HAVE_SHUTDOWN
#define HAVE_SHUTDOWN _Py_Config_Macro_Warning("macro HAVE_SHUTDOWN shouldn't be used") 1
#undef HAVE_SETVBUF
#define HAVE_SETVBUF _Py_Config_Macro_Warning("macro HAVE_SETVBUF shouldn't be used")
#undef HAVE_MKTIME
#define HAVE_MKTIME _Py_Config_Macro_Warning("macro HAVE_MKTIME shouldn't be used")
#undef HAVE_GETPID
#define HAVE_GETPID _Py_Config_Macro_Warning("macro HAVE_GETPID shouldn't be used")
#undef HAVE_GETPEERNAME
#define HAVE_GETPEERNAME _Py_Config_Macro_Warning("macro HAVE_GETPEERNAME shouldn't be used")
#undef HAVE_FTIME
#define HAVE_FTIME _Py_Config_Macro_Warning("macro HAVE_FTIME shouldn't be used")
#undef HAVE_DYNAMIC_LOADING
#define HAVE_DYNAMIC_LOADING _Py_Config_Macro_Warning("macro HAVE_DYNAMIC_LOADING shouldn't be used")
#undef WITH_MIMALLOC
#define WITH_MIMALLOC _Py_Config_Macro_Warning("macro WITH_MIMALLOC shouldn't be used") 1
#undef WITH_PYMALLOC
#define WITH_PYMALLOC _Py_Config_Macro_Warning("macro WITH_PYMALLOC shouldn't be used") 1
#undef WITH_DOC_STRINGS
#define WITH_DOC_STRINGS _Py_Config_Macro_Warning("macro WITH_DOC_STRINGS shouldn't be used") 1
#undef WITH_DECIMAL_CONTEXTVAR
#define WITH_DECIMAL_CONTEXTVAR _Py_Config_Macro_Warning("macro WITH_DECIMAL_CONTEXTVAR shouldn't be used") 1
#undef HAVE_PROTOTYPES
#define HAVE_PROTOTYPES _Py_Config_Macro_Warning("macro HAVE_PROTOTYPES shouldn't be used") 1
#undef HAVE_PUTENV
#define HAVE_PUTENV _Py_Config_Macro_Warning("macro HAVE_PUTENV shouldn't be used") 1
#undef STDC_HEADERS
#define STDC_HEADERS _Py_Config_Macro_Warning("macro STDC_HEADERS shouldn't be used") 1
#undef RETSIGTYPE
#define RETSIGTYPE _Py_Config_Macro_Warning("macro RETSIGTYPE shouldn't be used") void
#undef HAVE_TZNAME
#define HAVE_TZNAME _Py_Config_Macro_Warning("macro HAVE_TZNAME shouldn't be used")
#undef HAVE_DECL_TZNAME
#define HAVE_DECL_TZNAME _Py_Config_Macro_Warning("macro HAVE_DECL_TZNAME shouldn't be used") 1
#undef HAVE_DIRECT_H
#define HAVE_DIRECT_H _Py_Config_Macro_Warning("macro HAVE_DIRECT_H shouldn't be used") 1
#undef HAVE_CONIO_H
#define HAVE_CONIO_H _Py_Config_Macro_Warning("macro HAVE_CONIO_H shouldn't be used") 1
#ifdef HAVE_UINTPTR_T
#undef HAVE_UINTPTR_T
#define HAVE_UINTPTR_T _Py_Config_Macro_Warning("macro HAVE_UINTPTR_T shouldn't be used") 1
#endif
#ifdef HAVE_INTPTR_T
#undef HAVE_INTPTR_T
#define HAVE_INTPTR_T _Py_Config_Macro_Warning("macro HAVE_INTPTR_T shouldn't be used") 1
#endif
#ifdef MS_WIN32
#undef SIZEOF_FLOAT
#define SIZEOF_FLOAT _Py_Config_Macro_Warning("macro SIZEOF_FLOAT shouldn't be used") 4
#undef SIZEOF_DOUBLE
#define SIZEOF_DOUBLE _Py_Config_Macro_Warning("macro SIZEOF_DOUBLE shouldn't be used") 8
#undef SIZEOF_LONG_LONG
#define SIZEOF_LONG_LONG _Py_Config_Macro_Warning("macro SIZEOF_LONG_LONG shouldn't be used") 8
#undef ALIGNOF_LONG
#define ALIGNOF_LONG _Py_Config_Macro_Warning("macro ALIGNOF_LONG shouldn't be used") 4
#undef SIZEOF_LONG
#define SIZEOF_LONG _Py_Config_Macro_Warning("macro SIZEOF_LONG shouldn't be used") 4
#undef SIZEOF_INT
#define SIZEOF_INT _Py_Config_Macro_Warning("macro SIZEOF_INT shouldn't be used") 4
#undef SIZEOF_SHORT
#define SIZEOF_SHORT _Py_Config_Macro_Warning("macro SIZEOF_SHORT shouldn't be used") 2
#endif
#ifdef MS_WIN64
#       undef ALIGNOF_MAX_ALIGN_T
#       define ALIGNOF_MAX_ALIGN_T _Py_Config_Macro_Warning("macro ALIGNOF_MAX_ALIGN_T shouldn't be used") 8
#       undef ALIGNOF_SIZE_T
#       define ALIGNOF_SIZE_T _Py_Config_Macro_Warning("macro ALIGNOF_SIZE_T shouldn't be used") 8
#       undef SIZEOF_SIZE_T
#       define SIZEOF_SIZE_T _Py_Config_Macro_Warning("macro SIZEOF_SIZE_T shouldn't be used") 8
#       undef SIZEOF_HKEY
#       define SIZEOF_HKEY _Py_Config_Macro_Warning("macro SIZEOF_HKEY shouldn't be used") 8
#       undef SIZEOF_FPOS_T
#       define SIZEOF_FPOS_T _Py_Config_Macro_Warning("macro SIZEOF_FPOS_T shouldn't be used") 8
#       undef SIZEOF_OFF_T
#       define SIZEOF_OFF_T _Py_Config_Macro_Warning("macro SIZEOF_OFF_T shouldn't be used") 4
#       undef SIZEOF_VOID_P
#       define SIZEOF_VOID_P _Py_Config_Macro_Warning("macro SIZEOF_VOID_P shouldn't be used") 8
#       undef PLATFORM
#       define PLATFORM _Py_Config_Macro_Warning("macro PLATFORM shouldn't be used") "win64"

#elif defined(MS_WIN32)
#       undef ALIGNOF_MAX_ALIGN_T
#       define ALIGNOF_MAX_ALIGN_T _Py_Config_Macro_Warning("macro ALIGNOF_MAX_ALIGN_T shouldn't be used") 4
#       undef SIZEOF_TIME_T
#       if defined(_MSC_VER) && _MSC_VER >= 1400
#       define SIZEOF_TIME_T _Py_Config_Macro_Warning("macro SIZEOF_TIME_T shouldn't be used") 8
#       else
#       define SIZEOF_TIME_T _Py_Config_Macro_Warning("macro SIZEOF_TIME_T shouldn't be used") 4
#       endif
#       undef ALIGNOF_SIZE_T
#       define ALIGNOF_SIZE_T _Py_Config_Macro_Warning("macro ALIGNOF_SIZE_T shouldn't be used") 4
#       undef SIZEOF_SIZE_T
#       define SIZEOF_SIZE_T _Py_Config_Macro_Warning("macro SIZEOF_SIZE_T shouldn't be used") 4
#       undef SIZEOF_HKEY
#       define SIZEOF_HKEY _Py_Config_Macro_Warning("macro SIZEOF_HKEY shouldn't be used") 4
#       undef SIZEOF_FPOS_T
#       define SIZEOF_FPOS_T _Py_Config_Macro_Warning("macro SIZEOF_FPOS_T shouldn't be used") 8
#       undef SIZEOF_OFF_T
#       define SIZEOF_OFF_T _Py_Config_Macro_Warning("macro SIZEOF_OFF_T shouldn't be used") 4
#       undef SIZEOF_VOID_P
#       define SIZEOF_VOID_P _Py_Config_Macro_Warning("macro SIZEOF_VOID_P shouldn't be used") 4
#       undef HAVE_LARGEFILE_SUPPORT
#       undef PLATFORM
#       define HAVE_LARGEFILE_SUPPORT _Py_Config_Macro_Warning("macro HAVE_LARGEFILE_SUPPORT shouldn't be used")
#       define PLATFORM _Py_Config_Macro_Warning("macro PLATFORM shouldn't be used") "win32"
#endif

#undef HAVE_DECLSPEC_DLL
#define HAVE_DECLSPEC_DLL _Py_Config_Macro_Warning("macro HAVE_DECLSPEC_DLL shouldn't be used")

#if !defined(MS_NO_COREDLL) && !defined(Py_NO_ENABLE_SHARED)
#       undef MS_COREDLL
#       define MS_COREDLL _Py_Config_Macro_Warning("macro MS_COREDLL shouldn't be used")
#endif /* !MS_NO_COREDLL && ... */

#ifdef _WIN64
#undef MS_WIN64
#define MS_WIN64 _Py_Config_Macro_Warning("macro MS_WIN64 shouldn't be used")
#endif

#undef HAVE_PY_SSIZE_T
#define HAVE_PY_SSIZE_T _Py_Config_Macro_Warning("macro HAVE_PY_SSIZE_T shouldn't be used") 1

#if defined(Py_BUILD_CORE) || defined(Py_BUILD_CORE_BUILTIN) || defined(Py_BUILD_CORE_MODULE)
#if defined(MS_WINDOWS_DESKTOP) || defined(MS_WINDOWS_APP) || defined(MS_WINDOWS_SYSTEM)
#undef HAVE_WINDOWS_CONSOLE_IO
#define HAVE_WINDOWS_CONSOLE_IO _Py_Config_Macro_Warning("macro HAVE_WINDOWS_CONSOLE_IO shouldn't be used") 1
#endif

#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
#undef MS_WINDOWS_DESKTOP
#define MS_WINDOWS_DESKTOP _Py_Config_Macro_Warning("macro MS_WINDOWS_DESKTOP shouldn't be used")
#endif
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
#undef MS_WINDOWS_APP
#define MS_WINDOWS_APP _Py_Config_Macro_Warning("macro MS_WINDOWS_APP shouldn't be used")
#endif
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_SYSTEM)
#undef MS_WINDOWS_SYSTEM
#define MS_WINDOWS_SYSTEM _Py_Config_Macro_Warning("macro MS_WINDOWS_SYSTEM shouldn't be used")
#endif
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_GAMES)
#undef MS_WINDOWS_GAMES
#define MS_WINDOWS_GAMES _Py_Config_Macro_Warning("macro MS_WINDOWS_GAMES shouldn't be used")
#endif

#endif /* Py_BUILD_CORE || Py_BUILD_CORE_BUILTIN || Py_BUILD_CORE_MODULE */

#undef HAVE_STRFTIME
#define HAVE_STRFTIME _Py_Config_Macro_Warning("macro HAVE_STRFTIME shouldn't be used")
#undef DONT_HAVE_SIG_ALARM
#define DONT_HAVE_SIG_ALARM _Py_Config_Macro_Warning("macro DONT_HAVE_SIG_ALARM shouldn't be used")
#undef DONT_HAVE_SIG_PAUSE
#define DONT_HAVE_SIG_PAUSE _Py_Config_Macro_Warning("macro DONT_HAVE_SIG_PAUSE shouldn't be used")
#undef LONG_BIT
#define LONG_BIT _Py_Config_Macro_Warning("macro LONG_BIT shouldn't be used") 32
#undef WORD_BIT
#define WORD_BIT _Py_Config_Macro_Warning("macro WORD_BIT shouldn't be used") 32

#undef MS_WIN32
#define MS_WIN32 _Py_Config_Macro_Warning("macro MS_WIN32 shouldn't be used")
#undef MS_WINDOWS
#define MS_WINDOWS _Py_Config_Macro_Warning("macro MS_WINDOWS shouldn't be used")
#undef NT_THREADS
#define NT_THREADS _Py_Config_Macro_Warning("macro NT_THREADS shouldn't be used")
#undef WITH_THREAD
#define WITH_THREAD _Py_Config_Macro_Warning("macro WITH_THREAD shouldn't be used")

#ifndef NETSCAPE_PI
#undef USE_SOCKET
#define USE_SOCKET _Py_Config_Macro_Warning("macro USE_SOCKET shouldn't be used")
#endif

#undef HAVE_IO_H
#define HAVE_IO_H _Py_Config_Macro_Warning("macro HAVE_IO_H shouldn't be used")
#undef HAVE_SYS_UTIME_H
#define HAVE_SYS_UTIME_H _Py_Config_Macro_Warning("macro HAVE_SYS_UTIME_H shouldn't be used")
#undef HAVE_TEMPNAM
#define HAVE_TEMPNAM _Py_Config_Macro_Warning("macro HAVE_TEMPNAM shouldn't be used")
#undef HAVE_TMPFILE
#define HAVE_TMPFILE _Py_Config_Macro_Warning("macro HAVE_TMPFILE shouldn't be used")
#undef HAVE_TMPNAM
#define HAVE_TMPNAM _Py_Config_Macro_Warning("macro HAVE_TMPNAM shouldn't be used")
#undef HAVE_CLOCK
#define HAVE_CLOCK _Py_Config_Macro_Warning("macro HAVE_CLOCK shouldn't be used")
#undef HAVE_STRERROR
#define HAVE_STRERROR _Py_Config_Macro_Warning("macro HAVE_STRERROR shouldn't be used")

#endif /* !Py_PYCONFIG_WARNING_H */