#ifndef Py_CPYTHON_PYLIFECYCLE_H
#  error "this header file must not be included directly"
#endif

/* Py_FrozenMain is kept out of the Limited API until documented and present
   in all builds of Python */
PyAPI_FUNC(int) Py_FrozenMain(int argc, char **argv) Py_NOEXCEPT;

/* PEP 432 Multi-phase initialization API (Private while provisional!) */

PyAPI_FUNC(PyStatus) Py_PreInitialize(
    const PyPreConfig *src_config) Py_NOEXCEPT;
PyAPI_FUNC(PyStatus) Py_PreInitializeFromBytesArgs(
    const PyPreConfig *src_config,
    Py_ssize_t argc,
    char **argv) Py_NOEXCEPT;
PyAPI_FUNC(PyStatus) Py_PreInitializeFromArgs(
    const PyPreConfig *src_config,
    Py_ssize_t argc,
    wchar_t **argv) Py_NOEXCEPT;


/* Initialization and finalization */

PyAPI_FUNC(PyStatus) Py_InitializeFromConfig(
    const PyConfig *config) Py_NOEXCEPT;

// Python 3.8 provisional API (PEP 587)
PyAPI_FUNC(PyStatus) _Py_InitializeMain(void) Py_NOEXCEPT;

PyAPI_FUNC(int) Py_RunMain(void) Py_NOEXCEPT;


PyAPI_FUNC(void) _Py_NO_RETURN Py_ExitStatusException(PyStatus err) Py_NOEXCEPT;

PyAPI_FUNC(int) Py_FdIsInteractive(FILE *, const char *) Py_NOEXCEPT;

/* --- PyInterpreterConfig ------------------------------------ */

#define PyInterpreterConfig_DEFAULT_GIL (0)
#define PyInterpreterConfig_SHARED_GIL (1)
#define PyInterpreterConfig_OWN_GIL (2)

typedef struct {
    // XXX "allow_object_sharing"?  "own_objects"?
    int use_main_obmalloc;
    int allow_fork;
    int allow_exec;
    int allow_threads;
    int allow_daemon_threads;
    int check_multi_interp_extensions;
    int gil;
} PyInterpreterConfig;

#define _PyInterpreterConfig_INIT \
    { \
        .use_main_obmalloc = 0, \
        .allow_fork = 0, \
        .allow_exec = 0, \
        .allow_threads = 1, \
        .allow_daemon_threads = 0, \
        .check_multi_interp_extensions = 1, \
        .gil = PyInterpreterConfig_OWN_GIL, \
    }

// gh-117649: The free-threaded build does not currently support single-phase
// init extensions in subinterpreters. For now, we ensure that
// `check_multi_interp_extensions` is always `1`, even in the legacy config.
#ifdef Py_GIL_DISABLED
#  define _PyInterpreterConfig_LEGACY_CHECK_MULTI_INTERP_EXTENSIONS 1
#else
#  define _PyInterpreterConfig_LEGACY_CHECK_MULTI_INTERP_EXTENSIONS 0
#endif

#define _PyInterpreterConfig_LEGACY_INIT \
    { \
        .use_main_obmalloc = 1, \
        .allow_fork = 1, \
        .allow_exec = 1, \
        .allow_threads = 1, \
        .allow_daemon_threads = 1, \
        .check_multi_interp_extensions = _PyInterpreterConfig_LEGACY_CHECK_MULTI_INTERP_EXTENSIONS, \
        .gil = PyInterpreterConfig_SHARED_GIL, \
    }

PyAPI_FUNC(PyStatus) Py_NewInterpreterFromConfig(
    PyThreadState **tstate_p,
    const PyInterpreterConfig *config) Py_NOEXCEPT;

typedef void (*atexit_datacallbackfunc)(void *);
PyAPI_FUNC(int) PyUnstable_AtExit(
        PyInterpreterState *, atexit_datacallbackfunc, void *) Py_NOEXCEPT;
