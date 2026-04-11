// Error handling definitions

#ifndef Py_ERRORS_H
#define Py_ERRORS_H
#ifdef __cplusplus
extern "C" {
#endif

PyAPI_FUNC(void) PyErr_SetNone(PyObject *) Py_NOEXCEPT;
PyAPI_FUNC(void) PyErr_SetObject(PyObject *, PyObject *) Py_NOEXCEPT;
PyAPI_FUNC(void) PyErr_SetString(
    PyObject *exception,
    const char *string   /* decoded from utf-8 */
    ) Py_NOEXCEPT;
PyAPI_FUNC(PyObject *) PyErr_Occurred(void) Py_NOEXCEPT;
PyAPI_FUNC(void) PyErr_Clear(void) Py_NOEXCEPT;
PyAPI_FUNC(void) PyErr_Fetch(PyObject **, PyObject **, PyObject **) Py_NOEXCEPT;
PyAPI_FUNC(void) PyErr_Restore(PyObject *, PyObject *, PyObject *) Py_NOEXCEPT;
PyAPI_FUNC(PyObject *) PyErr_GetRaisedException(void) Py_NOEXCEPT;
PyAPI_FUNC(void) PyErr_SetRaisedException(PyObject *) Py_NOEXCEPT;
#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x030b0000
PyAPI_FUNC(PyObject*) PyErr_GetHandledException(void) Py_NOEXCEPT;
PyAPI_FUNC(void) PyErr_SetHandledException(PyObject *) Py_NOEXCEPT;
#endif
#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x03030000
PyAPI_FUNC(void) PyErr_GetExcInfo(PyObject **, PyObject **, PyObject **) Py_NOEXCEPT;
PyAPI_FUNC(void) PyErr_SetExcInfo(PyObject *, PyObject *, PyObject *) Py_NOEXCEPT;
#endif

/* Defined in Python/pylifecycle.c

   The Py_FatalError() function is replaced with a macro which logs
   automatically the name of the current function, unless the Py_LIMITED_API
   macro is defined. */
PyAPI_FUNC(void) _Py_NO_RETURN Py_FatalError(const char *message) Py_NOEXCEPT;

/* Error testing and normalization */
PyAPI_FUNC(int) PyErr_GivenExceptionMatches(PyObject *, PyObject *) Py_NOEXCEPT;
PyAPI_FUNC(int) PyErr_ExceptionMatches(PyObject *) Py_NOEXCEPT;
PyAPI_FUNC(void) PyErr_NormalizeException(PyObject**, PyObject**, PyObject**) Py_NOEXCEPT;

/* Traceback manipulation (PEP 3134) */
PyAPI_FUNC(int) PyException_SetTraceback(PyObject *, PyObject *) Py_NOEXCEPT;
PyAPI_FUNC(PyObject *) PyException_GetTraceback(PyObject *) Py_NOEXCEPT;

/* Cause manipulation (PEP 3134) */
PyAPI_FUNC(PyObject *) PyException_GetCause(PyObject *) Py_NOEXCEPT;
PyAPI_FUNC(void) PyException_SetCause(PyObject *, PyObject *) Py_NOEXCEPT;

/* Context manipulation (PEP 3134) */
PyAPI_FUNC(PyObject *) PyException_GetContext(PyObject *) Py_NOEXCEPT;
PyAPI_FUNC(void) PyException_SetContext(PyObject *, PyObject *) Py_NOEXCEPT;


PyAPI_FUNC(PyObject *) PyException_GetArgs(PyObject *) Py_NOEXCEPT;
PyAPI_FUNC(void) PyException_SetArgs(PyObject *, PyObject *) Py_NOEXCEPT;

/* */

#define PyExceptionClass_Check(x)                                       \
    (PyType_Check((x)) &&                                               \
     PyType_FastSubclass((PyTypeObject*)(x), Py_TPFLAGS_BASE_EXC_SUBCLASS))

#define PyExceptionInstance_Check(x)                    \
    PyType_FastSubclass(Py_TYPE(x), Py_TPFLAGS_BASE_EXC_SUBCLASS)

PyAPI_FUNC(const char *) PyExceptionClass_Name(PyObject *) Py_NOEXCEPT;

#define PyExceptionInstance_Class(x) _PyObject_CAST(Py_TYPE(x))

#define _PyBaseExceptionGroup_Check(x)                   \
    PyObject_TypeCheck((x), (PyTypeObject *)PyExc_BaseExceptionGroup)

/* Predefined exceptions */

PyAPI_DATA(PyObject *) PyExc_BaseException;
PyAPI_DATA(PyObject *) PyExc_Exception;
PyAPI_DATA(PyObject *) PyExc_BaseExceptionGroup;
#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x03050000
PyAPI_DATA(PyObject *) PyExc_StopAsyncIteration;
#endif
PyAPI_DATA(PyObject *) PyExc_StopIteration;
PyAPI_DATA(PyObject *) PyExc_GeneratorExit;
PyAPI_DATA(PyObject *) PyExc_ArithmeticError;
PyAPI_DATA(PyObject *) PyExc_LookupError;

PyAPI_DATA(PyObject *) PyExc_AssertionError;
PyAPI_DATA(PyObject *) PyExc_AttributeError;
PyAPI_DATA(PyObject *) PyExc_BufferError;
PyAPI_DATA(PyObject *) PyExc_EOFError;
PyAPI_DATA(PyObject *) PyExc_FloatingPointError;
PyAPI_DATA(PyObject *) PyExc_OSError;
PyAPI_DATA(PyObject *) PyExc_ImportError;
#if !defined(Py_LIMITED_API)
PyAPI_DATA(PyObject *) PyExc_ImportCycleError;
#endif
#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x03060000
PyAPI_DATA(PyObject *) PyExc_ModuleNotFoundError;
#endif
PyAPI_DATA(PyObject *) PyExc_IndexError;
PyAPI_DATA(PyObject *) PyExc_KeyError;
PyAPI_DATA(PyObject *) PyExc_KeyboardInterrupt;
PyAPI_DATA(PyObject *) PyExc_MemoryError;
PyAPI_DATA(PyObject *) PyExc_NameError;
PyAPI_DATA(PyObject *) PyExc_OverflowError;
PyAPI_DATA(PyObject *) PyExc_RuntimeError;
#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x03050000
PyAPI_DATA(PyObject *) PyExc_RecursionError;
#endif
PyAPI_DATA(PyObject *) PyExc_NotImplementedError;
PyAPI_DATA(PyObject *) PyExc_SyntaxError;
PyAPI_DATA(PyObject *) PyExc_IndentationError;
PyAPI_DATA(PyObject *) PyExc_TabError;
PyAPI_DATA(PyObject *) PyExc_ReferenceError;
PyAPI_DATA(PyObject *) PyExc_SystemError;
PyAPI_DATA(PyObject *) PyExc_SystemExit;
PyAPI_DATA(PyObject *) PyExc_TypeError;
PyAPI_DATA(PyObject *) PyExc_UnboundLocalError;
PyAPI_DATA(PyObject *) PyExc_UnicodeError;
PyAPI_DATA(PyObject *) PyExc_UnicodeEncodeError;
PyAPI_DATA(PyObject *) PyExc_UnicodeDecodeError;
PyAPI_DATA(PyObject *) PyExc_UnicodeTranslateError;
PyAPI_DATA(PyObject *) PyExc_ValueError;
PyAPI_DATA(PyObject *) PyExc_ZeroDivisionError;

#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x03030000
PyAPI_DATA(PyObject *) PyExc_BlockingIOError;
PyAPI_DATA(PyObject *) PyExc_BrokenPipeError;
PyAPI_DATA(PyObject *) PyExc_ChildProcessError;
PyAPI_DATA(PyObject *) PyExc_ConnectionError;
PyAPI_DATA(PyObject *) PyExc_ConnectionAbortedError;
PyAPI_DATA(PyObject *) PyExc_ConnectionRefusedError;
PyAPI_DATA(PyObject *) PyExc_ConnectionResetError;
PyAPI_DATA(PyObject *) PyExc_FileExistsError;
PyAPI_DATA(PyObject *) PyExc_FileNotFoundError;
PyAPI_DATA(PyObject *) PyExc_InterruptedError;
PyAPI_DATA(PyObject *) PyExc_IsADirectoryError;
PyAPI_DATA(PyObject *) PyExc_NotADirectoryError;
PyAPI_DATA(PyObject *) PyExc_PermissionError;
PyAPI_DATA(PyObject *) PyExc_ProcessLookupError;
PyAPI_DATA(PyObject *) PyExc_TimeoutError;
#endif


/* Compatibility aliases */
PyAPI_DATA(PyObject *) PyExc_EnvironmentError;
PyAPI_DATA(PyObject *) PyExc_IOError;
#ifdef MS_WINDOWS
PyAPI_DATA(PyObject *) PyExc_WindowsError;
#endif

/* Predefined warning categories */
PyAPI_DATA(PyObject *) PyExc_Warning;
PyAPI_DATA(PyObject *) PyExc_UserWarning;
PyAPI_DATA(PyObject *) PyExc_DeprecationWarning;
PyAPI_DATA(PyObject *) PyExc_PendingDeprecationWarning;
PyAPI_DATA(PyObject *) PyExc_SyntaxWarning;
PyAPI_DATA(PyObject *) PyExc_RuntimeWarning;
PyAPI_DATA(PyObject *) PyExc_FutureWarning;
PyAPI_DATA(PyObject *) PyExc_ImportWarning;
PyAPI_DATA(PyObject *) PyExc_UnicodeWarning;
PyAPI_DATA(PyObject *) PyExc_BytesWarning;
PyAPI_DATA(PyObject *) PyExc_EncodingWarning;
PyAPI_DATA(PyObject *) PyExc_ResourceWarning;


/* Convenience functions */

PyAPI_FUNC(int) PyErr_BadArgument(void) Py_NOEXCEPT;
PyAPI_FUNC(PyObject *) PyErr_NoMemory(void) Py_NOEXCEPT;
PyAPI_FUNC(PyObject *) PyErr_SetFromErrno(PyObject *) Py_NOEXCEPT;
PyAPI_FUNC(PyObject *) PyErr_SetFromErrnoWithFilenameObject(
    PyObject *, PyObject *) Py_NOEXCEPT;
#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x03040000
PyAPI_FUNC(PyObject *) PyErr_SetFromErrnoWithFilenameObjects(
    PyObject *, PyObject *, PyObject *) Py_NOEXCEPT;
#endif
PyAPI_FUNC(PyObject *) PyErr_SetFromErrnoWithFilename(
    PyObject *exc,
    const char *filename   /* decoded from the filesystem encoding */
    ) Py_NOEXCEPT;

PyAPI_FUNC(PyObject *) PyErr_Format(
    PyObject *exception,
    const char *format,   /* ASCII-encoded string  */
    ...
    ) Py_NOEXCEPT;
#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x03050000
PyAPI_FUNC(PyObject *) PyErr_FormatV(
    PyObject *exception,
    const char *format,
    va_list vargs) Py_NOEXCEPT;
#endif

#ifdef MS_WINDOWS
PyAPI_FUNC(PyObject *) PyErr_SetFromWindowsErrWithFilename(
    int ierr,
    const char *filename        /* decoded from the filesystem encoding */
    ) Py_NOEXCEPT;
PyAPI_FUNC(PyObject *) PyErr_SetFromWindowsErr(int) Py_NOEXCEPT;
PyAPI_FUNC(PyObject *) PyErr_SetExcFromWindowsErrWithFilenameObject(
    PyObject *,int, PyObject *) Py_NOEXCEPT;
#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x03040000
PyAPI_FUNC(PyObject *) PyErr_SetExcFromWindowsErrWithFilenameObjects(
    PyObject *,int, PyObject *, PyObject *) Py_NOEXCEPT;
#endif
PyAPI_FUNC(PyObject *) PyErr_SetExcFromWindowsErrWithFilename(
    PyObject *exc,
    int ierr,
    const char *filename        /* decoded from the filesystem encoding */
    ) Py_NOEXCEPT;
PyAPI_FUNC(PyObject *) PyErr_SetExcFromWindowsErr(PyObject *, int) Py_NOEXCEPT;
#endif /* MS_WINDOWS */

#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x03060000
PyAPI_FUNC(PyObject *) PyErr_SetImportErrorSubclass(PyObject *, PyObject *,
    PyObject *, PyObject *) Py_NOEXCEPT;
#endif
#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x03030000
PyAPI_FUNC(PyObject *) PyErr_SetImportError(PyObject *, PyObject *,
    PyObject *) Py_NOEXCEPT;
#endif

/* Export the old function so that the existing API remains available: */
PyAPI_FUNC(void) PyErr_BadInternalCall(void) Py_NOEXCEPT;
PyAPI_FUNC(void) _PyErr_BadInternalCall(const char *filename, int lineno) Py_NOEXCEPT;
/* Mask the old API with a call to the new API for code compiled under
   Python 2.0: */
#define PyErr_BadInternalCall() _PyErr_BadInternalCall(__FILE__, __LINE__)

/* Function to create a new exception */
PyAPI_FUNC(PyObject *) PyErr_NewException(
    const char *name, PyObject *base, PyObject *dict) Py_NOEXCEPT;
PyAPI_FUNC(PyObject *) PyErr_NewExceptionWithDoc(
    const char *name, const char *doc, PyObject *base, PyObject *dict) Py_NOEXCEPT;
PyAPI_FUNC(void) PyErr_WriteUnraisable(PyObject *) Py_NOEXCEPT;


/* In signalmodule.c */
PyAPI_FUNC(int) PyErr_CheckSignals(void) Py_NOEXCEPT;
PyAPI_FUNC(void) PyErr_SetInterrupt(void) Py_NOEXCEPT;
#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x030A0000
PyAPI_FUNC(int) PyErr_SetInterruptEx(int signum) Py_NOEXCEPT;
#endif

/* Support for adding program text to SyntaxErrors */
PyAPI_FUNC(void) PyErr_SyntaxLocation(
    const char *filename,       /* decoded from the filesystem encoding */
    int lineno) Py_NOEXCEPT;
PyAPI_FUNC(void) PyErr_SyntaxLocationEx(
    const char *filename,       /* decoded from the filesystem encoding */
    int lineno,
    int col_offset) Py_NOEXCEPT;
PyAPI_FUNC(PyObject *) PyErr_ProgramText(
    const char *filename,       /* decoded from the filesystem encoding */
    int lineno) Py_NOEXCEPT;

/* The following functions are used to create and modify unicode
   exceptions from C */

/* create a UnicodeDecodeError object */
PyAPI_FUNC(PyObject *) PyUnicodeDecodeError_Create(
    const char *encoding,       /* UTF-8 encoded string */
    const char *object,
    Py_ssize_t length,
    Py_ssize_t start,
    Py_ssize_t end,
    const char *reason          /* UTF-8 encoded string */
    ) Py_NOEXCEPT;

/* get the encoding attribute */
PyAPI_FUNC(PyObject *) PyUnicodeEncodeError_GetEncoding(PyObject *) Py_NOEXCEPT;
PyAPI_FUNC(PyObject *) PyUnicodeDecodeError_GetEncoding(PyObject *) Py_NOEXCEPT;

/* get the object attribute */
PyAPI_FUNC(PyObject *) PyUnicodeEncodeError_GetObject(PyObject *) Py_NOEXCEPT;
PyAPI_FUNC(PyObject *) PyUnicodeDecodeError_GetObject(PyObject *) Py_NOEXCEPT;
PyAPI_FUNC(PyObject *) PyUnicodeTranslateError_GetObject(PyObject *) Py_NOEXCEPT;

/* get the value of the start attribute (the int * may not be NULL)
   return 0 on success, -1 on failure */
PyAPI_FUNC(int) PyUnicodeEncodeError_GetStart(PyObject *, Py_ssize_t *) Py_NOEXCEPT;
PyAPI_FUNC(int) PyUnicodeDecodeError_GetStart(PyObject *, Py_ssize_t *) Py_NOEXCEPT;
PyAPI_FUNC(int) PyUnicodeTranslateError_GetStart(PyObject *, Py_ssize_t *) Py_NOEXCEPT;

/* assign a new value to the start attribute
   return 0 on success, -1 on failure */
PyAPI_FUNC(int) PyUnicodeEncodeError_SetStart(PyObject *, Py_ssize_t) Py_NOEXCEPT;
PyAPI_FUNC(int) PyUnicodeDecodeError_SetStart(PyObject *, Py_ssize_t) Py_NOEXCEPT;
PyAPI_FUNC(int) PyUnicodeTranslateError_SetStart(PyObject *, Py_ssize_t) Py_NOEXCEPT;

/* get the value of the end attribute (the int *may not be NULL)
 return 0 on success, -1 on failure */
PyAPI_FUNC(int) PyUnicodeEncodeError_GetEnd(PyObject *, Py_ssize_t *) Py_NOEXCEPT;
PyAPI_FUNC(int) PyUnicodeDecodeError_GetEnd(PyObject *, Py_ssize_t *) Py_NOEXCEPT;
PyAPI_FUNC(int) PyUnicodeTranslateError_GetEnd(PyObject *, Py_ssize_t *) Py_NOEXCEPT;

/* assign a new value to the end attribute
   return 0 on success, -1 on failure */
PyAPI_FUNC(int) PyUnicodeEncodeError_SetEnd(PyObject *, Py_ssize_t) Py_NOEXCEPT;
PyAPI_FUNC(int) PyUnicodeDecodeError_SetEnd(PyObject *, Py_ssize_t) Py_NOEXCEPT;
PyAPI_FUNC(int) PyUnicodeTranslateError_SetEnd(PyObject *, Py_ssize_t) Py_NOEXCEPT;

/* get the value of the reason attribute */
PyAPI_FUNC(PyObject *) PyUnicodeEncodeError_GetReason(PyObject *) Py_NOEXCEPT;
PyAPI_FUNC(PyObject *) PyUnicodeDecodeError_GetReason(PyObject *) Py_NOEXCEPT;
PyAPI_FUNC(PyObject *) PyUnicodeTranslateError_GetReason(PyObject *) Py_NOEXCEPT;

/* assign a new value to the reason attribute
   return 0 on success, -1 on failure */
PyAPI_FUNC(int) PyUnicodeEncodeError_SetReason(
    PyObject *exc,
    const char *reason          /* UTF-8 encoded string */
    ) Py_NOEXCEPT;
PyAPI_FUNC(int) PyUnicodeDecodeError_SetReason(
    PyObject *exc,
    const char *reason          /* UTF-8 encoded string */
    ) Py_NOEXCEPT;
PyAPI_FUNC(int) PyUnicodeTranslateError_SetReason(
    PyObject *exc,
    const char *reason          /* UTF-8 encoded string */
    ) Py_NOEXCEPT;

PyAPI_FUNC(int) PyOS_snprintf(char *str, size_t size, const char  *format, ...)
                        Py_GCC_ATTRIBUTE((format(printf, 3, 4))) Py_NOEXCEPT;
PyAPI_FUNC(int) PyOS_vsnprintf(char *str, size_t size, const char  *format, va_list va)
                        Py_GCC_ATTRIBUTE((format(printf, 3, 0))) Py_NOEXCEPT;

#ifndef Py_LIMITED_API
#  define Py_CPYTHON_ERRORS_H
#  include "cpython/pyerrors.h"
#  undef Py_CPYTHON_ERRORS_H
#endif

#ifdef __cplusplus
}
#endif
#endif /* !Py_ERRORS_H */
