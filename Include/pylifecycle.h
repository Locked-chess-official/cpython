
/* Interfaces to configure, query, create & destroy the Python runtime */

#ifndef Py_PYLIFECYCLE_H
#define Py_PYLIFECYCLE_H
#ifdef __cplusplus
extern "C" {
#endif


/* Initialization and finalization */
PyAPI_FUNC(void) Py_Initialize(void) Py_NOEXCEPT;
PyAPI_FUNC(void) Py_InitializeEx(int) Py_NOEXCEPT;
PyAPI_FUNC(void) Py_Finalize(void) Py_NOEXCEPT;
#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x03060000
PyAPI_FUNC(int) Py_FinalizeEx(void) Py_NOEXCEPT;
#endif
PyAPI_FUNC(int) Py_IsInitialized(void) Py_NOEXCEPT;

/* Subinterpreter support */
PyAPI_FUNC(PyThreadState *) Py_NewInterpreter(void) Py_NOEXCEPT;
PyAPI_FUNC(void) Py_EndInterpreter(PyThreadState *) Py_NOEXCEPT;


/* Py_PyAtExit is for the atexit module, Py_AtExit is for low-level
 * exit functions.
 */
PyAPI_FUNC(int) Py_AtExit(void (*func)(void)) Py_NOEXCEPT;

PyAPI_FUNC(void) _Py_NO_RETURN Py_Exit(int) Py_NOEXCEPT;

/* Bootstrap __main__ (defined in Modules/main.c) */
PyAPI_FUNC(int) Py_Main(int argc, wchar_t **argv) Py_NOEXCEPT;
PyAPI_FUNC(int) Py_BytesMain(int argc, char **argv) Py_NOEXCEPT;

/* In pathconfig.c */
Py_DEPRECATED(3.11) PyAPI_FUNC(void) Py_SetProgramName(const wchar_t *) Py_NOEXCEPT;
Py_DEPRECATED(3.11) PyAPI_FUNC(void) Py_SetPythonHome(const wchar_t *) Py_NOEXCEPT;

#ifdef MS_WINDOWS
int _Py_CheckPython3(void);
#endif

/* In their own files */
PyAPI_FUNC(const char *) Py_GetVersion(void) Py_NOEXCEPT;
PyAPI_FUNC(const char *) Py_GetPlatform(void) Py_NOEXCEPT;
PyAPI_FUNC(const char *) Py_GetCopyright(void) Py_NOEXCEPT;
PyAPI_FUNC(const char *) Py_GetCompiler(void) Py_NOEXCEPT;
PyAPI_FUNC(const char *) Py_GetBuildInfo(void) Py_NOEXCEPT;

/* Signals */
typedef void (*PyOS_sighandler_t)(int);
PyAPI_FUNC(PyOS_sighandler_t) PyOS_getsig(int) Py_NOEXCEPT;
PyAPI_FUNC(PyOS_sighandler_t) PyOS_setsig(int, PyOS_sighandler_t) Py_NOEXCEPT;

#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x030B0000
PyAPI_DATA(const unsigned long) Py_Version;
#endif

#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x030D0000
PyAPI_FUNC(int) Py_IsFinalizing(void) Py_NOEXCEPT;
#endif

#ifndef Py_LIMITED_API
#  define Py_CPYTHON_PYLIFECYCLE_H
#  include "cpython/pylifecycle.h"
#  undef Py_CPYTHON_PYLIFECYCLE_H
#endif

#ifdef __cplusplus
}
#endif
#endif /* !Py_PYLIFECYCLE_H */
