
/* Interfaces to parse and execute pieces of python code */

#ifndef Py_PYTHONRUN_H
#define Py_PYTHONRUN_H
#ifdef __cplusplus
extern "C" {
#endif

PyAPI_FUNC(PyObject *) Py_CompileString(const char *, const char *, int) Py_NOEXCEPT;

PyAPI_FUNC(void) PyErr_Print(void) Py_NOEXCEPT;
PyAPI_FUNC(void) PyErr_PrintEx(int) Py_NOEXCEPT;
PyAPI_FUNC(void) PyErr_Display(PyObject *, PyObject *, PyObject *) Py_NOEXCEPT;

#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x030C0000
PyAPI_FUNC(void) PyErr_DisplayException(PyObject *) Py_NOEXCEPT;
#endif


/* Stuff with no proper home (yet) */
PyAPI_DATA(int) (*PyOS_InputHook)(void);

#if defined(WIN32)
#  define USE_STACKCHECK
#endif
#ifdef USE_STACKCHECK
/* Check that we aren't overflowing our stack */
PyAPI_FUNC(int) PyOS_CheckStack(void) Py_NOEXCEPT;
#endif


#ifndef Py_LIMITED_API
#  define Py_CPYTHON_PYTHONRUN_H
#  include "cpython/pythonrun.h"
#  undef Py_CPYTHON_PYTHONRUN_H
#endif

#ifdef __cplusplus
}
#endif
#endif /* !Py_PYTHONRUN_H */
