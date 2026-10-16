#!/usr/bin/env python3
"""Generate PC/configure.toml (probe-driven) from the ground up.

Every macro in the configurable tail of PC/pyconfig.h is emitted either as a
probe (header existence, function existence, sizeof, type fallback, struct
member, declaration, endianness) or as a documented literal (build-policy
switch, external-library dependency, platform constant, or dead autoconf
boilerplate that CPython no longer references).

Function probes use per-platform headers so that MSVC (underscore-prefixed CRT
names declared in <process.h>/<io.h>) and POSIX (<unistd.h>/<sys/...>) are both
detected correctly.
"""
from pathlib import Path

OUT = Path("PC/configure.toml")


def lit(s):
    """Emit `code` as a TOML literal string ('''...''') so C escapes survive."""
    assert "'''" not in s, "code contains ''' which cannot be in a literal string"
    return "'''\n" + s + "\n'''"


def header_entry(macro, hdr):
    return (
        f"[[analyse.bool]]\n"
        f'macro = "{macro}"\n'
        f'comment = "Define to 1 if you have the <{hdr}> header file."\n'
        f'header = ["{hdr}"]\n\n'
    )


# (macro, win_header_or_None, posix_header, win_lib, unix_lib)
# win_header None => posix_header is used unconditionally on all platforms.
# win_lib None   => no extra link library on Windows.
FUNCS = [
    # --- socket family ---
    ("HAVE_SOCKET", "winsock2.h", "sys/socket.h", "ws2_32", None),
    ("HAVE_ACCEPT", "winsock2.h", "sys/socket.h", "ws2_32", None),
    ("HAVE_BIND", "winsock2.h", "sys/socket.h", "ws2_32", None),
    ("HAVE_CONNECT", "winsock2.h", "sys/socket.h", "ws2_32", None),
    ("HAVE_GETSOCKNAME", "winsock2.h", "sys/socket.h", "ws2_32", None),
    ("HAVE_LISTEN", "winsock2.h", "sys/socket.h", "ws2_32", None),
    ("HAVE_RECVFROM", "winsock2.h", "sys/socket.h", "ws2_32", None),
    ("HAVE_SENDTO", "winsock2.h", "sys/socket.h", "ws2_32", None),
    ("HAVE_SETSOCKOPT", "winsock2.h", "sys/socket.h", "ws2_32", None),
    ("HAVE_SHUTDOWN", "winsock2.h", "sys/socket.h", "ws2_32", None),
    ("HAVE_GETPEERNAME", "winsock2.h", "sys/socket.h", "ws2_32", None),
    # --- network info ---
    ("HAVE_GETHOSTNAME", "winsock2.h", "unistd.h", "ws2_32", None),
    ("HAVE_GETHOSTBYADDR", "winsock2.h", "netdb.h", "ws2_32", None),
    ("HAVE_GETHOSTBYNAME", "winsock2.h", "netdb.h", "ws2_32", None),
    ("HAVE_GETPROTOBYNAME", "winsock2.h", "netdb.h", "ws2_32", None),
    ("HAVE_GETSERVBYNAME", "winsock2.h", "netdb.h", "ws2_32", None),
    ("HAVE_GETSERVBYPORT", "winsock2.h", "netdb.h", "ws2_32", None),
    ("HAVE_INET_PTON", "ws2tcpip.h", "arpa/inet.h", "ws2_32", None),
    ("HAVE_INET_NTOA", "winsock2.h", "arpa/inet.h", "ws2_32", None),
    # --- process / file (CRT: underscore names on MSVC) ---
    ("HAVE_GETPID", "process.h", "unistd.h", None, None),
    ("HAVE_DUP", "io.h", "unistd.h", None, None),
    ("HAVE_PUTENV", None, "stdlib.h", None, None),
    ("HAVE_UMASK", "io.h", "sys/stat.h", None, None),
    ("HAVE_MKTIME", None, "time.h", None, None),
    ("HAVE_SETVBUF", None, "stdio.h", None, None),
    ("HAVE_FTIME", None, "sys/timeb.h", None, None),
    # --- unix-only (probe fails on MSVC -> /* #undef */) ---
    ("HAVE_GETPGRP", None, "unistd.h", None, None),
    ("HAVE_GETTIMEOFDAY", None, "sys/time.h", None, None),
    ("HAVE_GETWD", None, "unistd.h", None, None),
    ("HAVE_LSTAT", None, "sys/stat.h", None, None),
    ("HAVE_NICE", None, "unistd.h", None, None),
    ("HAVE_READLINK", None, "unistd.h", None, None),
    ("HAVE_SETPGID", None, "unistd.h", None, None),
    ("HAVE_SETPGRP", None, "unistd.h", None, None),
    ("HAVE_SETSID", None, "unistd.h", None, None),
    ("HAVE_SIGINTERRUPT", None, "signal.h", None, None),
    ("HAVE_SYMLINK", None, "unistd.h", None, None),
    ("HAVE_TCGETPGRP", None, "unistd.h", None, None),
    ("HAVE_TCSETPGRP", None, "unistd.h", None, None),
    ("HAVE_TIMES", None, "sys/times.h", None, None),
    ("HAVE_UNAME", None, "sys/utsname.h", None, None),
    ("HAVE_WAITPID", None, "sys/wait.h", None, None),
    # --- wide char ---
    ("HAVE_WCSCOLL", None, "wchar.h", None, None),
    ("HAVE_WCSXFRM", None, "wchar.h", None, None),
    ("HAVE_WCSFTIME", None, "wchar.h", None, None),
    # --- math ---
    ("HAVE_ERF", None, "math.h", None, "m"),
    ("HAVE_ERFC", None, "math.h", None, "m"),
]


def func_entry(macro, win_header, posix_header, win_lib, unix_lib):
    sym = macro[len("HAVE_"):].lower()
    out = (
        f"[[analyse.bool]]\n"
        f'macro = "{macro}"\n'
        f'comment = "Define to 1 if you have the `{sym}\' function."\n'
    )
    if win_header is None:
        out += f'header = ["{posix_header}"]\n'
    else:
        out += (
            f'pre = ["#ifdef _WIN32", "#include <{win_header}>", '
            f'"#else", "#include <{posix_header}>", "#endif"]\n'
        )
    if win_lib is not None:
        out += f'libs = ["{win_lib}"]\nunix_libs = []\n'
    if unix_lib is not None:
        out += f'unix_libs = ["{unix_lib}"]\n'
    code = (
        "int main(void) {\n"
        f"    volatile void *p = (void *)&{sym};\n"
        "    (void)p;\n"
        "    return 0;\n"
        "}\n"
    )
    out += f"code = {lit(code)}\n\n"
    return out


def main():
    parts = []
    parts.append("# PC/pyconfig.h configurable-macro description (probe-driven).\n")
    parts.append("# Generated by PC/_gen_toml.py from the macro inventory of PC/pyconfig.h.\n")
    parts.append("#\n")
    parts.append("# Every detectable macro is resolved by compiling+running a tiny C probe\n")
    parts.append("# against the active toolchain (configure.py injects the forced _CRT_*\n")
    parts.append("# prelude so MSVC sees the same CRT name-space the real build does).\n")
    parts.append("# The only literals left are build-policy switches, external-library\n")
    parts.append("# dependencies, platform constants, and dead autoconf boilerplate.\n")
    parts.append("#\n")
    parts.append("# Modes:\n")
    parts.append("#   [[analyse.bool]]     define MACRO <value> on success, /* #undef */ on fail\n")
    parts.append("#   [[analyse.number]]   define MACRO to the probe stdout (integer)\n")
    parts.append("#   [[analyse.string]]   define MACRO to the probe stdout (double-quoted)\n")
    parts.append("#   [[analyse.literal]]  emit a fixed value (no probe)\n")
    parts.append("#\n")
    parts.append("# NOTE: `code` must be a TOML literal string ('''...''') so C escapes survive.\n\n")
    parts.append("[analyse]\n\n")

    # ---- headers ----
    HEADERS = [
        ("HAVE_CONIO_H", "conio.h"),
        ("HAVE_DIRECT_H", "direct.h"),
        ("HAVE_DLFCN_H", "dlfcn.h"),
        ("HAVE_ERRNO_H", "errno.h"),
        ("HAVE_FCNTL_H", "fcntl.h"),
        ("HAVE_PROCESS_H", "process.h"),
        ("HAVE_SIGNAL_H", "signal.h"),
        ("HAVE_STDDEF_H", "stddef.h"),
        ("HAVE_SYS_AUDIOIO_H", "sys/audioio.h"),
        ("HAVE_SYS_PARAM_H", "sys/param.h"),
        ("HAVE_SYS_SELECT_H", "sys/select.h"),
        ("HAVE_SYS_STAT_H", "sys/stat.h"),
        ("HAVE_SYS_TIME_H", "sys/time.h"),
        ("HAVE_SYS_TIMES_H", "sys/times.h"),
        ("HAVE_SYS_TYPES_H", "sys/types.h"),
        ("HAVE_SYS_UN_H", "sys/un.h"),
        ("HAVE_SYS_UTSNAME_H", "sys/utsname.h"),
        ("HAVE_UNISTD_H", "unistd.h"),
        ("HAVE_UTIME_H", "utime.h"),
        ("HAVE_WCHAR_H", "wchar.h"),
    ]
    parts.append("# ---------------- Header existence ----------------\n")
    for macro, hdr in HEADERS:
        parts.append(header_entry(macro, hdr))

    # ---- functions ----
    parts.append("# ---------------- Function existence ----------------\n")
    for args in FUNCS:
        parts.append(func_entry(*args))

    # ---- sizes ----
    parts.append("# ---------------- Sizes ----------------\n")
    sizes = [
        ("SIZEOF_WCHAR_T", "wchar_t", "stddef.h"),
        ("SIZEOF__BOOL", "_Bool", "stddef.h"),
    ]
    for macro, typ, hdr in sizes:
        code = (
            "#include <stdio.h>\n"
            f"#include <{hdr}>\n"
            "int main(void) {\n"
            f'    printf("%d\\n", (int)sizeof({typ}));\n'
            "    return 0;\n"
            "}\n"
        )
        parts.append(
            f"[[analyse.number]]\n"
            f'macro = "{macro}"\n'
            f'comment = "The size of `{typ}\', as computed by sizeof."\n'
            f'header = ["stdio.h", "{hdr}"]\n'
            f"code = {lit(code)}\n\n"
        )

    # pid_t needs a WIN32 fallback typedef so the probe can still measure it
    code_pid = (
        "#include <stdio.h>\n"
        "#ifdef _WIN32\n"
        "typedef int pid_t;\n"
        "#else\n"
        "#include <sys/types.h>\n"
        "#endif\n"
        "int main(void) {\n"
        '    printf("%d\\n", (int)sizeof(pid_t));\n'
        "    return 0;\n"
        "}\n"
    )
    parts.append(
        "[[analyse.number]]\n"
        'macro = "SIZEOF_PID_T"\n'
        'comment = "The size of `pid_t\', as computed by sizeof."\n'
        'header = ["stdio.h"]\n'
        f"code = {lit(code_pid)}\n\n"
    )

    # ---- type fallbacks (invert: define only when the type is absent) ----
    parts.append("# ---------------- Type fallbacks (invert: define only when absent) ----------------\n")
    type_fallbacks = [
        ("pid_t", "sys/types.h", "int", "Define to `int' if <sys/types.h> doesn't define."),
        ("mode_t", "sys/types.h", "int", "Define to `int' if <sys/types.h> doesn't define."),
        ("off_t", "sys/types.h", "long", "Define to `long' if <sys/types.h> doesn't define."),
        ("gid_t", "sys/types.h", "int", "Define to `int' if <sys/types.h> doesn't define."),
        ("uid_t", "sys/types.h", "int", "Define to `int' if <sys/types.h> doesn't define."),
        ("size_t", "stddef.h", "unsigned int", "Define to `unsigned int' if <stddef.h> doesn't define."),
    ]
    for name, hdr, val, comment in type_fallbacks:
        code = f"#include <{hdr}>\nint main(void) {{\n    (void)sizeof({name});\n    return 0;\n}}\n"
        parts.append(
            f"[[analyse.bool]]\n"
            f'macro = "{name}"\n'
            f'comment = "{comment}"\n'
            f'header = ["{hdr}"]\n'
            f"code = {lit(code)}\n"
            f"invert = true\n"
            f'value = "{val}"\n\n'
        )

    # GETGROUPS_T fallback
    parts.append(
        "[[analyse.bool]]\n"
        'macro = "GETGROUPS_T"\n'
        'comment = "Define to the type of elements in the array set by `getgroups\'. Usually `int\' or `gid_t\'."\n'
        'header = ["unistd.h", "sys/types.h"]\n'
        "code = '''\n"
        "int main(void) {\n"
        "    (void)sizeof(getgroups(0, 0));\n"
        "    return 0;\n"
        "}\n"
        "'''\n"
        "invert = true\n"
        'value = "int"\n\n'
    )

    # RETSIGTYPE: modern compilers always use void
    parts.append(
        "[[analyse.bool]]\n"
        'macro = "RETSIGTYPE"\n'
        'comment = "Define as the return type of signal handlers (`int\' or `void\')."\n'
        'header = ["signal.h"]\n'
        "code = '''\n"
        "static void handler(int sig) { (void)sig; }\n"
        "int main(void) {\n"
        "    void (*p)(int) = handler;\n"
        "    (void)p;\n"
        "    return 0;\n"
        "}\n"
        "'''\n"
        "invert = true\n"
        'value = "void"\n\n'
    )

    # ---- timezone declarations ----
    parts.append("# ---------------- timezone declarations ----------------\n")
    # ---- timezone declarations ----
    # NOTE: these are fixed literals on Windows. MSVC <time.h> declares only
    # _tzname/_timezone/_daylight (exposed via _CRT_NONSTDC_NO_DEPRECATE as
    # tzname/timezone/daylight by timemodule.c's own mapping), and never declares
    # altzone.  Self-declaring `extern` probes would be self-fulfilling (always
    # compile+link) and give the wrong answer, so pin them to the known Windows
    # values instead of probing.
    parts.append(
        "[[analyse.literal]]\n"
        'macro = "HAVE_DECL_TZNAME"\n'
        'value = true\n'
        'comment = "Define to 1 if you have the declaration of `tzname\'. (Windows: CRT provides _tzname)"\n\n'
    )
    parts.append(
        "[[analyse.literal]]\n"
        'macro = "HAVE_TZNAME"\n'
        'value = ""\n'
        'comment = "Define if you don\'t have tm_zone but do have the external array tzname."\n\n'
    )
    parts.append(
        "[[analyse.bool]]\n"
        'macro = "HAVE_STRUCT_TM_TM_ZONE"\n'
        'comment = "Define to 1 if your `struct tm\' has a `tm_zone\' member."\n'
        'header = ["time.h"]\n'
        "code = '''\n"
        "#include <time.h>\n"
        "int main(void) {\n"
        "    struct tm t;\n"
        "    (void)t.tm_zone;\n"
        "    return 0;\n"
        "}\n\n"
        "'''\n\n"
    )
    parts.append(
        "[[analyse.literal]]\n"
        'macro = "HAVE_ALTZONE"\n'
        'value = false\n'
        'comment = "Define to 1 if the `altzone\' variable is declared. (not in MSVC <time.h>)"\n\n'
    )

    # NOTE: HAVE_TM_ZONE is the deprecated autoheader alias for
    # HAVE_STRUCT_TM_TM_ZONE; CPython code only uses the latter (probed above).
    # Keep the alias as a literal undef for shape parity with the old header.
    parts.append(
        "[[analyse.literal]]\n"
        'macro = "HAVE_TM_ZONE"\n'
        'value = false\n'
        'comment = "Deprecated alias of HAVE_STRUCT_TM_TM_ZONE."\n\n'
    )

    # ---- features ----
    parts.append("# ---------------- Features ----------------\n")
    parts.append(
        "[[analyse.bool]]\n"
        'macro = "STDC_HEADERS"\n'
        'comment = "Define to 1 if you have the ANSI C header files."\n'
        'header = ["stdlib.h", "stdarg.h", "string.h", "float.h"]\n'
        "code = '''\n"
        "int main(void) {\n"
        "    (void)sizeof(float);\n"
        "    return 0;\n"
        "}\n"
        "'''\n\n"
    )
    parts.append(
        "[[analyse.bool]]\n"
        'macro = "HAVE_PROTOTYPES"\n'
        'comment = "Define to 1 if your compiler supports function prototypes."\n'
        'header = ["stdio.h"]\n'
        "code = '''\n"
        "int add(int a, int b);\n"
        "int main(void) { return add(1, 2) - 3; }\n"
        "int add(int a, int b) { return a + b; }\n"
        "'''\n\n"
    )
    parts.append(
        "[[analyse.bool]]\n"
        'macro = "HAVE_DYNAMIC_LOADING"\n'
        'comment = "Define to 1 if you want support for dynamic loading of modules."\n'
        'pre = ["#ifdef _WIN32", "#include <windows.h>", "#else", "#include <dlfcn.h>", "#endif"]\n'
        "code = '''\n"
        "#ifdef _WIN32\n"
        "int main(void) {\n"
        '    HMODULE h = LoadLibraryA("kernel32.dll");\n'
        "    if (h) FreeLibrary(h);\n"
        "    return 0;\n"
        "}\n"
        "#else\n"
        "int main(void) {\n"
        "    void *h = dlopen(0, 0);\n"
        "    if (h) dlclose(h);\n"
        "    return 0;\n"
        "}\n"
        "#endif\n"
        "'''\n"
        'unix_libs = ["dl"]\n\n'
    )

    # ---- endianness (runtime probe) ----
    parts.append("# ---------------- Byte order ----------------\n")
    parts.append(
        "[[analyse.bool]]\n"
        'macro = "DOUBLE_IS_LITTLE_ENDIAN_IEEE754"\n'
        'comment = "Define to 1 if C doubles are 64-bit IEEE 754, least significant byte first."\n'
        'header = ["string.h"]\n'
        "code = '''\n"
        "int main(void) {\n"
        "    double d = 1.0;\n"
        "    unsigned char b[8];\n"
        "    memcpy(b, &d, 8);\n"
        "    /* 1.0 = 0x3FF0000000000000; little-endian first byte is 0x00. */\n"
        "    return (b[0] == 0x00) ? 0 : 1;\n"
        "}\n"
        "'''\n"
        "run = true\n\n"
    )

    # ---- literals: build policy / external deps / platform constants / dead boilerplate ----
    parts.append("# ---------------- Literals (not probeable) ----------------\n")

    LITERALS = [
        # dead autoconf boilerplate not referenced by CPython (kept for shape parity)
        ("const", "false", "", "Define to empty if the keyword does not work. Always available."),
        ("_POSIX_1_SOURCE", "false", "", "Define if you need POSIX.1 features. Never on Windows."),
        ("_POSIX_SOURCE", "false", "", "Define if you need POSIX features. Never on Windows."),
        ("_ALL_SOURCE", "false", "!defined(_ALL_SOURCE)", "Define on AIX 3. Never on Windows."),
        ("DIRENT", "false", "", "Define if you have dirent.h. (legacy autoconf macro)"),
        ("NDIR", "false", "", "Define if you have ndir.h. (legacy autoconf macro)"),
        ("SYSDIR", "false", "", "Define if you have sys/dir.h. (legacy autoconf macro)"),
        ("SYSNDIR", "false", "", "Define if you have sys/ndir.h. (legacy autoconf macro)"),
        ("TIME_WITH_SYS_TIME", "false", "", "Define if both <time.h> and <sys/time.h> may be included. (unused)"),
        ("TM_IN_SYS_TIME", "false", "", "Define if struct tm is in <sys/time.h>. (unused)"),
        ("SYS_SELECT_WITH_SYS_TIME", "false", "", "Define if <sys/select.h> needs <sys/time.h>. (unused)"),
        ("VOID_CLOSEDIR", "false", "", "Define if closedir returns void. (unused)"),
        ("GETPGRP_HAVE_ARGS", "false", "", "Define if getpgrp takes an argument. (unused; code uses GETPGRP_HAVE_ARG)"),
        # build-policy switches
        ("WITH_DECIMAL_CONTEXTVAR", "true", "", "Define to 1 to build the _decimal module with contextvar support."),
        ("WITH_DOC_STRINGS", "true", "", "Define to 1 to enable documentation strings."),
        ("WITH_PYMALLOC", "true", "", "Define to 1 to compile in pymalloc, the Python memory allocator."),
        ("WITH_MIMALLOC", "true", "", "Define to 1 to compile in mimalloc as the pymalloc backend."),
        ("WITH_THREAD", "false", "", "Define to 1 if you want native threads. (Windows uses NT_THREADS in the static head)"),
        ("WITH_READLINE", "false", "", "Define if you want to build the readline module. (external dep)"),
        ("HAVE_CLOCK", "false", "", "Define to 1 if you have the `clock' function. (see static head)"),
        # external library dependencies (not detected by a bare probe)
        ("HAVE_LIBSOCKET", "true", "", "Define if you have the `socket' library. (Windows: winsock)"),
        ("HAVE_LIBDL", "false", "", "Define if you have the `dl' library."),
        ("HAVE_LIBMPC", "false", "", "Define if you have the `mpc' library."),
        ("HAVE_LIBSEQ", "false", "", "Define if you have the `seq' library."),
        ("HAVE_LIBSUN", "false", "", "Define if you have the `sun' library."),
        ("HAVE_LIBTERMCAP", "false", "", "Define if you have the `termcap' library."),
        ("HAVE_LIBTERMLIB", "false", "", "Define if you have the `termlib' library."),
        ("HAVE_LIBTHREAD", "false", "", "Define if you have the `thread' library."),
        # OpenSSL / zlib features (present because PC/pyconfig.h hardcodes them)
        ("HAVE_ZLIB_COPY", "true", "", "Define if the zlib library has inflateCopy."),
        ("HAVE_X509_VERIFY_PARAM_SET1_HOST", "true", "", "Define if OpenSSL has X509_VERIFY_PARAM_set1_host."),
        # platform constants
        ("_PYTHONFRAMEWORK", "'\"\"'", "", "Mac framework name; empty on Windows."),
        ("_PYTHREAD_NAME_MAXLEN", "32766", "", "Max length of a thread name on Windows."),
        ("Py_SOCKET_FD_CAN_BE_GE_FD_SETSIZE", "\"\"", "", "Define if socket fds can exceed FD_SETSIZE. (Windows: bare define)"),
    ]
    for macro, val, guard, comment in LITERALS:
        parts.append(
            f"[[analyse.literal]]\n"
            f'macro = "{macro}"\n'
            f'value = {val}\n'
            f'comment = "{comment}"\n'
        )
        if guard:
            parts.append(f'guard = "{guard}"\n')
        parts.append("\n")

    OUT.write_text("".join(parts), encoding="utf-8")
    print(f"wrote {OUT}")


if __name__ == "__main__":
    main()
