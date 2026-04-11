/* Module definition and import interface */

#ifndef Py_IMPORT_H
#define Py_IMPORT_H
#ifdef __cplusplus
extern "C" {
#endif

PyAPI_FUNC(long) PyImport_GetMagicNumber(void) Py_NOEXCEPT;
PyAPI_FUNC(const char *) PyImport_GetMagicTag(void) Py_NOEXCEPT;
PyAPI_FUNC(PyObject *) PyImport_ExecCodeModule(
    const char *name,           /* UTF-8 encoded string */
    PyObject *co
    ) Py_NOEXCEPT;
PyAPI_FUNC(PyObject *) PyImport_ExecCodeModuleEx(
    const char *name,           /* UTF-8 encoded string */
    PyObject *co,
    const char *pathname        /* decoded from the filesystem encoding */
    ) Py_NOEXCEPT;
PyAPI_FUNC(PyObject *) PyImport_ExecCodeModuleWithPathnames(
    const char *name,           /* UTF-8 encoded string */
    PyObject *co,
    const char *pathname,       /* decoded from the filesystem encoding */
    const char *cpathname       /* decoded from the filesystem encoding */
    ) Py_NOEXCEPT;
#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x03030000
PyAPI_FUNC(PyObject *) PyImport_ExecCodeModuleObject(
    PyObject *name,
    PyObject *co,
    PyObject *pathname,
    PyObject *cpathname
    ) Py_NOEXCEPT;
#endif
PyAPI_FUNC(PyObject *) PyImport_GetModuleDict(void) Py_NOEXCEPT;
#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x03070000
PyAPI_FUNC(PyObject *) PyImport_GetModule(PyObject *name) Py_NOEXCEPT;
#endif
#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x03030000
PyAPI_FUNC(PyObject *) PyImport_AddModuleObject(
    PyObject *name
    ) Py_NOEXCEPT;
#endif
PyAPI_FUNC(PyObject *) PyImport_AddModule(
    const char *name            /* UTF-8 encoded string */
    ) Py_NOEXCEPT;
#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x030d0000
PyAPI_FUNC(PyObject *) PyImport_AddModuleRef(
    const char *name            /* UTF-8 encoded string */
    ) Py_NOEXCEPT;
#endif
PyAPI_FUNC(PyObject *) PyImport_ImportModule(
    const char *name            /* UTF-8 encoded string */
    ) Py_NOEXCEPT;
PyAPI_FUNC(PyObject *) PyImport_ImportModuleLevel(
    const char *name,           /* UTF-8 encoded string */
    PyObject *globals,
    PyObject *locals,
    PyObject *fromlist,
    int level
    ) Py_NOEXCEPT;
#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x03050000
PyAPI_FUNC(PyObject *) PyImport_ImportModuleLevelObject(
    PyObject *name,
    PyObject *globals,
    PyObject *locals,
    PyObject *fromlist,
    int level
    ) Py_NOEXCEPT;
#endif

#define PyImport_ImportModuleEx(n, g, l, f) \
    PyImport_ImportModuleLevel((n), (g), (l), (f), 0)

PyAPI_FUNC(PyObject *) PyImport_GetImporter(PyObject *path) Py_NOEXCEPT;
PyAPI_FUNC(PyObject *) PyImport_Import(PyObject *name) Py_NOEXCEPT;
PyAPI_FUNC(PyObject *) PyImport_ReloadModule(PyObject *m) Py_NOEXCEPT;
#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x03030000
PyAPI_FUNC(int) PyImport_ImportFrozenModuleObject(
    PyObject *name
    ) Py_NOEXCEPT;
#endif
PyAPI_FUNC(int) PyImport_ImportFrozenModule(
    const char *name            /* UTF-8 encoded string */
    ) Py_NOEXCEPT;

PyAPI_FUNC(int) PyImport_AppendInittab(
    const char *name,           /* ASCII encoded string */
    PyObject* (*initfunc)(void)
    ) Py_NOEXCEPT;

typedef enum {
    PyImport_LAZY_NORMAL,
    PyImport_LAZY_ALL,
    PyImport_LAZY_NONE
} PyImport_LazyImportsMode;

#ifndef Py_LIMITED_API
PyAPI_FUNC(int) PyImport_SetLazyImportsMode(PyImport_LazyImportsMode mode) Py_NOEXCEPT;
PyAPI_FUNC(int) PyImport_SetLazyImportsFilter(PyObject *filter) Py_NOEXCEPT;

PyAPI_FUNC(PyImport_LazyImportsMode) PyImport_GetLazyImportsMode(void) Py_NOEXCEPT;
PyAPI_FUNC(PyObject *) PyImport_GetLazyImportsFilter(void) Py_NOEXCEPT;
#endif

#ifndef Py_LIMITED_API
#  define Py_CPYTHON_IMPORT_H
#  include "cpython/import.h"
#  undef Py_CPYTHON_IMPORT_H
#endif

#ifdef __cplusplus
}
#endif
#endif /* !Py_IMPORT_H */
