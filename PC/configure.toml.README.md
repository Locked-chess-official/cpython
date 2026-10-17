# PC/pyconfig.h via configure.toml (probe-driven prototype)

This prototype implements the idea of generating `PC/pyconfig.h` from a
declarative `configure.toml` instead of hand-maintaining the header.

## Files

| File | Purpose |
| --- | --- |
| `PC/configure.py`    | The generator. Parses the TOML, runs C probes, writes the header. |
| `PC/configure.toml`  | Probe-driven description of every configurable macro. |
| `PC/pyconfig.h.in`   | Static template; `@PYCONFIG_PROBES@` is replaced with the generated block. |
| `PCbuild/configure.bat` | Windows entry point (Python 3.11+). |
| `PC/_gen_toml.py`    | Regenerates `configure.toml` from the macro inventory. |

## Design

`PC/pyconfig.h` is split into two regions:

1. **Static head** — compiler/platform logic (MSVC/minGW/clang/LCC, `_MSC_VER`,
   `MS_WINxx`, `Py_ssize_t`, arch-dependent `SIZEOF_*`/`ALIGNOF_*`, the forced
   `_W64`, `_CRT_*` deprecation silencers, `Py_ENABLE_SHARED`, etc.). Kept
   verbatim in `PC/pyconfig.h.in`.

2. **Configurable tail** — the `/* Fairly standard from here! */` autoconf
   section (~120 feature macros). Now described by `configure.toml`.

## Probe modes

### [[analyse.bool]] — feature present/absent
Success → `#define MACRO 1` (or `value`, or bare if `value = ""`);
failure → `/* #undef MACRO */`.

Used for: header existence, function existence, struct members, declarations,
and `invert`/`value` type-fallbacks (e.g. define `pid_t` only when the system
doesn't already).

### [[analyse.number]] — measure something
Compiles + runs the probe, captures stdout (an integer) → `#define MACRO <int>`.
Used for `SIZEOF_WCHAR_T`, `SIZEOF__BOOL`, `SIZEOF_PID_T`.

### [[analyse.string]] — capture a string
`#define MACRO "<stdout>"`. (Not currently used by any macro.)

### [[analyse.literal]] — not probeable
Fixed value. Used for: build-policy switches (`WITH_*`), external-library
dependencies (`HAVE_LIB*`, `HAVE_ZLIB_COPY`, `HAVE_X509_...`), platform
constants (`_PYTHREAD_NAME_MAXLEN`, `_PYTHONFRAMEWORK`), and dead autoconf
boilerplate kept for shape parity (`const`, `_POSIX_*`, `DIRENT`, etc.).

## Fields

`macro`, `comment`, `header` (list, POSIX/non-Windows), `win_header` (list,
Windows-specific, wrapped in `#ifdef _WIN32`), `code` (probe main-body, TOML
literal string), `libs` (MSVC `.lib`), `unix_libs` (POSIX `-l`),
`extra_cmd` (list, extra compile flags/commands), `guard` (`#if` wrapper),
`run` (bool, execute probe), `invert` (bool, define on probe failure),
`value` (bool/literal token).

Function probes use **per-platform headers** so MSVC and POSIX both resolve:
e.g. `HAVE_SOCKET` probes `<winsock2.h>` on `_WIN32` and `<sys/socket.h>`
elsewhere; `HAVE_GETPID` probes `<process.h>` (MSVC, where `_CRT_NONSTDC_NO_DEPRECATE`
exposes `getpid`) and `<unistd.h>` elsewhere.

## Usage

On Windows (Developer Command Prompt for VS):

```
PCbuild\configure.bat
PCbuild\configure.bat cl
```

Development/testing on any platform with a C compiler:

```
python PC/configure.py --compiler gcc --output /tmp/pyconfig.h
```

## Requirements

* Python 3.11+ (stdlib `tomllib`).
