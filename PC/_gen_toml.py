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
import os

SRC_DIR = Path(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

OUT = SRC_DIR / "PC" / "pyconfig.h"


def lit(s):
    """Emit `code` as a TOML literal string ('''...''') so C escapes survive."""
    assert "'''" not in s, "code contains ''' which cannot be in a literal string"
    return "'''\n" + s + "\n'''"


def _toml_list(items):
    """Render a Python sequence of strings as a TOML array literal."""
    return "[" + ", ".join(f'"{x}"' for x in items) + "]"


# =============================================================================
# Macro inventory — add new macros here.
#
# Each section below is a list of tuples with a fixed, documented shape. The
# generator functions (`*_entry`, defined further down) turn each tuple into a
# single [[analyse.*]] entry in configure.toml. If you add a macro, pick the
# matching section and follow that section's tuple shape; the one-off probes
# that don't fit any recurring shape are written directly in main().
#
# Headers and libraries are ALWAYS lists (even a single item), so a new macro
# is added the same way regardless of how many headers/libraries it needs.
# Use None for "not applicable on this platform / no such field".
# =============================================================================

# Header existence: (macro, [headers])
#   A plain existence check — "can the compiler find this header?". Unlike
#   FUNCS/SIZES/TYPE_FALLBACKS there is deliberately no win/posix split: a
#   header either exists on the active toolchain or it doesn't.
HEADERS = [
    ("HAVE_CONIO_H", ["conio.h"]),
    ("HAVE_DIRECT_H", ["direct.h"]),
    ("HAVE_DLFCN_H", ["dlfcn.h"]),
    ("HAVE_ERRNO_H", ["errno.h"]),
    ("HAVE_FCNTL_H", ["fcntl.h"]),
    ("HAVE_PROCESS_H", ["process.h"]),
    ("HAVE_SIGNAL_H", ["signal.h"]),
    ("HAVE_STDDEF_H", ["stddef.h"]),
    ("HAVE_SYS_AUDIOIO_H", ["sys/audioio.h"]),
    ("HAVE_SYS_PARAM_H", ["sys/param.h"]),
    ("HAVE_SYS_SELECT_H", ["sys/select.h"]),
    ("HAVE_SYS_STAT_H", ["sys/stat.h"]),
    ("HAVE_SYS_TIME_H", ["sys/time.h"]),
    ("HAVE_SYS_TIMES_H", ["sys/times.h"]),
    ("HAVE_SYS_TYPES_H", ["sys/types.h"]),
    ("HAVE_SYS_UN_H", ["sys/un.h"]),
    ("HAVE_SYS_UTSNAME_H", ["sys/utsname.h"]),
    ("HAVE_UNISTD_H", ["unistd.h"]),
    ("HAVE_UTIME_H", ["utime.h"]),
    ("HAVE_WCHAR_H", ["wchar.h"]),
]

# Function existence:
#   (macro, func, win_headers, posix_headers, win_libs, unix_libs, extra_cmd)
#     macro          "HAVE_..." macro name
#     func           the C function the probe takes the address of, written
#                    verbatim into the probe body. It is decoupled from the
#                    macro name (no HAVE_ stripping) so macro renames or a
#                    future Py_/_Py_ prefix requirement cannot corrupt it.
#     win_headers    headers included under `#ifdef _WIN32` (None if none)
#     posix_headers  headers included on other platforms; always emitted as
#                    `header` (required)
#     win_libs       MSVC link libraries -> `libs` (None if none)
#     unix_libs      POSIX `-l` libraries -> `unix_libs` (None if none)
#     extra_cmd      extra compiler flags/commands -> `extra_cmd` (None if none)
#   win_headers None => posix_headers are included unconditionally.
FUNCS = [
    # --- socket family ---
    ("HAVE_SOCKET", "socket", ["winsock2.h"], ["sys/socket.h"], ["ws2_32"], None, None),
    ("HAVE_ACCEPT", "accept", ["winsock2.h"], ["sys/socket.h"], ["ws2_32"], None, None),
    ("HAVE_BIND", "bind", ["winsock2.h"], ["sys/socket.h"], ["ws2_32"], None, None),
    ("HAVE_CONNECT", "connect", ["winsock2.h"], ["sys/socket.h"], ["ws2_32"], None, None),
    ("HAVE_GETSOCKNAME", "getsockname", ["winsock2.h"], ["sys/socket.h"], ["ws2_32"], None, None),
    ("HAVE_LISTEN", "listen", ["winsock2.h"], ["sys/socket.h"], ["ws2_32"], None, None),
    ("HAVE_RECVFROM", "recvfrom", ["winsock2.h"], ["sys/socket.h"], ["ws2_32"], None, None),
    ("HAVE_SENDTO", "sendto", ["winsock2.h"], ["sys/socket.h"], ["ws2_32"], None, None),
    ("HAVE_SETSOCKOPT", "setsockopt", ["winsock2.h"], ["sys/socket.h"], ["ws2_32"], None, None),
    ("HAVE_SHUTDOWN", "shutdown", ["winsock2.h"], ["sys/socket.h"], ["ws2_32"], None, None),
    ("HAVE_GETPEERNAME", "getpeername", ["winsock2.h"], ["sys/socket.h"], ["ws2_32"], None, None),
    # --- network info ---
    ("HAVE_GETHOSTNAME", "gethostname", ["winsock2.h"], ["unistd.h"], ["ws2_32"], None, None),
    ("HAVE_GETHOSTBYADDR", "gethostbyaddr", ["winsock2.h"], ["netdb.h"], ["ws2_32"], None, None),
    ("HAVE_GETHOSTBYNAME", "gethostbyname", ["winsock2.h"], ["netdb.h"], ["ws2_32"], None, None),
    ("HAVE_GETPROTOBYNAME", "getprotobyname", ["winsock2.h"], ["netdb.h"], ["ws2_32"], None, None),
    ("HAVE_GETSERVBYNAME", "getservbyname", ["winsock2.h"], ["netdb.h"], ["ws2_32"], None, None),
    ("HAVE_GETSERVBYPORT", "getservbyport", ["winsock2.h"], ["netdb.h"], ["ws2_32"], None, None),
    ("HAVE_INET_PTON", "inet_pton", ["ws2tcpip.h"], ["arpa/inet.h"], ["ws2_32"], None, None),
    ("HAVE_INET_NTOA", "inet_ntoa", ["winsock2.h"], ["arpa/inet.h"], ["ws2_32"], None, None),
    # --- process / file (CRT: underscore names on MSVC) ---
    ("HAVE_GETPID", "getpid", ["process.h"], ["unistd.h"], None, None, None),
    ("HAVE_DUP", "dup", ["io.h"], ["unistd.h"], None, None, None),
    ("HAVE_PUTENV", "putenv", None, ["stdlib.h"], None, None, None),
    ("HAVE_UMASK", "umask", ["io.h"], ["sys/stat.h"], None, None, None),
    ("HAVE_MKTIME", "mktime", None, ["time.h"], None, None, None),
    ("HAVE_SETVBUF", "setvbuf", None, ["stdio.h"], None, None, None),
    ("HAVE_FTIME", "ftime", None, ["sys/timeb.h"], None, None, None),
    # --- unix-only (probe fails on MSVC -> /* #undef */) ---
    ("HAVE_GETPGRP", "getpgrp", None, ["unistd.h"], None, None, None),
    ("HAVE_GETTIMEOFDAY", "gettimeofday", None, ["sys/time.h"], None, None, None),
    ("HAVE_GETWD", "getwd", None, ["unistd.h"], None, None, None),
    ("HAVE_LSTAT", "lstat", None, ["sys/stat.h"], None, None, None),
    ("HAVE_NICE", "nice", None, ["unistd.h"], None, None, None),
    ("HAVE_READLINK", "readlink", None, ["unistd.h"], None, None, None),
    ("HAVE_SETPGID", "setpgid", None, ["unistd.h"], None, None, None),
    ("HAVE_SETPGRP", "setpgrp", None, ["unistd.h"], None, None, None),
    ("HAVE_SETSID", "setsid", None, ["unistd.h"], None, None, None),
    ("HAVE_SIGINTERRUPT", "siginterrupt", None, ["signal.h"], None, None, None),
    ("HAVE_SYMLINK", "symlink", None, ["unistd.h"], None, None, None),
    ("HAVE_TCGETPGRP", "tcgetpgrp", None, ["unistd.h"], None, None, None),
    ("HAVE_TCSETPGRP", "tcsetpgrp", None, ["unistd.h"], None, None, None),
    ("HAVE_TIMES", "times", None, ["sys/times.h"], None, None, None),
    ("HAVE_UNAME", "uname", None, ["sys/utsname.h"], None, None, None),
    ("HAVE_WAITPID", "waitpid", None, ["sys/wait.h"], None, None, None),
    # --- wide char ---
    ("HAVE_WCSCOLL", "wcscoll", None, ["wchar.h"], None, None, None),
    ("HAVE_WCSXFRM", "wcsxfrm", None, ["wchar.h"], None, None, None),
    ("HAVE_WCSFTIME", "wcsftime", None, ["wchar.h"], None, None, None),
    # --- math ---
    ("HAVE_ERF", "erf", None, ["math.h"], None, ["m"], None),
    ("HAVE_ERFC", "erfc", None, ["math.h"], None, ["m"], None),
]

# Sizeof:
#   (macro, type, win_headers, posix_headers, win_libs, unix_libs, extra_cmd)
#   `type` is the C type whose sizeof() is printed. The probe's own <stdio.h>
#   (for printf) is added by size_entry(), so it is not listed here.
SIZES = [
    ("SIZEOF_WCHAR_T", "wchar_t", None, ["stddef.h"], None, None, None),
    ("SIZEOF__BOOL", "_Bool", None, ["stddef.h"], None, None, None),
]

# Type fallbacks (invert: define only when the type is absent):
#   (macro, win_headers, posix_headers, win_libs, unix_libs, extra_cmd, fallback_value)
#   The comment is auto-generated from fallback_value + posix_headers[0].
TYPE_FALLBACKS = [
    ("pid_t", None, ["sys/types.h"], None, None, None, "int"),
    ("mode_t", None, ["sys/types.h"], None, None, None, "int"),
    ("off_t", None, ["sys/types.h"], None, None, None, "long"),
    ("gid_t", None, ["sys/types.h"], None, None, None, "int"),
    ("uid_t", None, ["sys/types.h"], None, None, None, "int"),
    ("size_t", None, ["stddef.h"], None, None, None, "unsigned int"),
]

# Literals (not probeable): (macro, value, guard, comment)
#   value is emitted verbatim into TOML (`false`, `true`, `""`, `32766`, ...).
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


# =============================================================================
# Generator helpers — turn one inventory tuple into one [[analyse.*]] entry.
# =============================================================================

def _probe_fields(win_headers, posix_headers, win_libs, unix_libs, extra_cmd):
    """Shared field block for [[analyse.bool]] probe entries.

    `header` always carries the non-Windows headers; `win_header` is only
    emitted when there are Windows-specific headers. Libraries are lists and
    `None` means "none on this platform".
    """
    out = f'header = {_toml_list(posix_headers)}\n'
    if win_headers is not None:
        out += f'win_header = {_toml_list(win_headers)}\n'
    if win_libs is not None:
        out += f'libs = {_toml_list(win_libs)}\n'
    if unix_libs is not None:
        out += f'unix_libs = {_toml_list(unix_libs)}\n'
    elif win_libs is not None:
        # Windows-only link library: keep the POSIX side explicitly empty so
        # configure.py links no extra library there (instead of reusing `libs`).
        out += 'unix_libs = []\n'
    if extra_cmd is not None:
        out += f'extra_cmd = {_toml_list(extra_cmd)}\n'
    return out


def header_entry(macro, hdrs):
    hdr_note = ", ".join(f"<{h}>" for h in hdrs)
    return (
        f"[[analyse.bool]]\n"
        f'macro = "{macro}"\n'
        f'comment = "Define to 1 if you have the {hdr_note} header file."\n'
        f'header = {_toml_list(hdrs)}\n\n'
    )


def func_entry(macro, func, win_headers, posix_headers, win_libs, unix_libs, extra_cmd=None):
    out = (
        f"[[analyse.bool]]\n"
        f'macro = "{macro}"\n'
        f'comment = "Define to 1 if you have the `{func}\' function."\n'
    )
    out += _probe_fields(win_headers, posix_headers, win_libs, unix_libs, extra_cmd)
    code = (
        "int main(void) {\n"
        f"    volatile void *p = (void *)&{func};\n"
        "    (void)p;\n"
        "    return 0;\n"
        "}\n"
    )
    out += f"code = {lit(code)}\n\n"
    return out


def size_entry(macro, typ, win_headers, posix_headers, win_libs, unix_libs, extra_cmd=None):
    # printf() is needed on every platform, so <stdio.h> lives in the probe
    # body itself; `header`/`win_header` carry only the type's own headers.
    code = (
        "#include <stdio.h>\n"
        "int main(void) {\n"
        f'    printf("%d\\n", (int)sizeof({typ}));\n'
        "    return 0;\n"
        "}\n"
    )
    out = (
        f"[[analyse.number]]\n"
        f'macro = "{macro}"\n'
        f'comment = "The size of `{typ}\', as computed by sizeof."\n'
    )
    out += _probe_fields(win_headers, posix_headers, win_libs, unix_libs, extra_cmd)
    out += f"code = {lit(code)}\n\n"
    return out


def type_fallback_entry(macro, win_headers, posix_headers, win_libs, unix_libs, extra_cmd, val):
    hdr = posix_headers[0]
    comment = f"Define to `{val}' if <{hdr}> doesn't define."
    code = (
        "int main(void) {\n"
        f"    (void)sizeof({macro});\n"
        "    return 0;\n"
        "}\n"
    )
    out = (
        f"[[analyse.bool]]\n"
        f'macro = "{macro}"\n'
        f'comment = "{comment}"\n'
    )
    out += _probe_fields(win_headers, posix_headers, win_libs, unix_libs, extra_cmd)
    out += (
        f"code = {lit(code)}\n"
        f"invert = true\n"
        f'value = "{val}"\n\n'
    )
    return out


def literal_entry(macro, val, guard, comment):
    out = (
        f"[[analyse.literal]]\n"
        f'macro = "{macro}"\n'
        f'value = {val}\n'
        f'comment = "{comment}"\n'
    )
    if guard:
        out += f'guard = "{guard}"\n'
    return out + "\n"


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
    parts.append("# ---------------- Header existence ----------------\n")
    for macro, hdrs in HEADERS:
        parts.append(header_entry(macro, hdrs))

    # ---- functions ----
    parts.append("# ---------------- Function existence ----------------\n")
    for args in FUNCS:
        parts.append(func_entry(*args))

    # ---- sizes ----
    parts.append("# ---------------- Sizes ----------------\n")
    for args in SIZES:
        parts.append(size_entry(*args))

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
    for args in TYPE_FALLBACKS:
        parts.append(type_fallback_entry(*args))

    # GETGROUPS_T fallback (custom probe body, written inline)
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

    # ---- features (custom probe bodies, written inline) ----
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
        'header = ["dlfcn.h"]\n'
        'win_header = ["windows.h"]\n'
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
    for args in LITERALS:
        parts.append(literal_entry(*args))

    OUT.write_text("".join(parts), encoding="utf-8")
    print(f"wrote {OUT}")


if __name__ == "__main__":
    main()
