#ifndef Py_WARNINGS_H
#define Py_WARNINGS_H
#ifdef __cplusplus
extern "C" {
#endif

PyAPI_FUNC(int) PyErr_WarnEx(
    PyObject *category,
    const char *message,        /* UTF-8 encoded string */
    Py_ssize_t stack_level) Py_NOEXCEPT;

PyAPI_FUNC(int) PyErr_WarnFormat(
    PyObject *category,
    Py_ssize_t stack_level,
    const char *format,         /* ASCII-encoded string  */
    ...) Py_NOEXCEPT;

#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x03060000
/* Emit a ResourceWarning warning */
PyAPI_FUNC(int) PyErr_ResourceWarning(
    PyObject *source,
    Py_ssize_t stack_level,
    const char *format,         /* ASCII-encoded string  */
    ...) Py_NOEXCEPT;
#endif

PyAPI_FUNC(int) PyErr_WarnExplicit(
    PyObject *category,
    const char *message,        /* UTF-8 encoded string */
    const char *filename,       /* decoded from the filesystem encoding */
    int lineno,
    const char *module,         /* UTF-8 encoded string */
    PyObject *registry) Py_NOEXCEPT;

#ifndef Py_LIMITED_API
#  define Py_CPYTHON_WARNINGS_H
#  include "cpython/warnings.h"
#  undef Py_CPYTHON_WARNINGS_H
#endif

#ifdef __cplusplus
}
#endif
#endif /* !Py_WARNINGS_H */

