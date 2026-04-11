#ifndef Py_CPYTHON_ERRORS_H
#  error "this header file must not be included directly"
#endif

/* Error objects */

/* PyException_HEAD defines the initial segment of every exception class. */
#define PyException_HEAD PyObject_HEAD PyObject *dict;\
             PyObject *args; PyObject *notes; PyObject *traceback;\
             PyObject *context; PyObject *cause;\
             char suppress_context;

typedef struct {
    PyException_HEAD
} PyBaseExceptionObject;

typedef struct {
    PyException_HEAD
    PyObject *msg;
    PyObject *excs;
    PyObject *excs_str;
} PyBaseExceptionGroupObject;

typedef struct {
    PyException_HEAD
    PyObject *msg;
    PyObject *filename;
    PyObject *lineno;
    PyObject *offset;
    PyObject *end_lineno;
    PyObject *end_offset;
    PyObject *text;
    PyObject *print_file_and_line;
    PyObject *metadata;
} PySyntaxErrorObject;

typedef struct {
    PyException_HEAD
    PyObject *msg;
    PyObject *name;
    PyObject *path;
    PyObject *name_from;
} PyImportErrorObject;

typedef struct {
    PyException_HEAD
    PyObject *encoding;
    PyObject *object;
    Py_ssize_t start;
    Py_ssize_t end;
    PyObject *reason;
} PyUnicodeErrorObject;

typedef struct {
    PyException_HEAD
    PyObject *code;
} PySystemExitObject;

typedef struct {
    PyException_HEAD
    PyObject *myerrno;
    PyObject *strerror;
    PyObject *filename;
    PyObject *filename2;
#ifdef MS_WINDOWS
    PyObject *winerror;
#endif
    Py_ssize_t written;   /* only for BlockingIOError, -1 otherwise */
} PyOSErrorObject;

typedef struct {
    PyException_HEAD
    PyObject *value;
} PyStopIterationObject;

typedef struct {
    PyException_HEAD
    PyObject *name;
} PyNameErrorObject;

typedef struct {
    PyException_HEAD
    PyObject *obj;
    PyObject *name;
} PyAttributeErrorObject;

/* Compatibility typedefs */
typedef PyOSErrorObject PyEnvironmentErrorObject;
#ifdef MS_WINDOWS
typedef PyOSErrorObject PyWindowsErrorObject;
#endif

/* Context manipulation (PEP 3134) */

PyAPI_FUNC(void) _PyErr_ChainExceptions1(PyObject *) Py_NOEXCEPT;

/* In exceptions.c */

PyAPI_FUNC(PyObject*) PyUnstable_Exc_PrepReraiseStar(
     PyObject *orig,
     PyObject *excs) Py_NOEXCEPT;

/* In signalmodule.c */

PyAPI_FUNC(int) PySignal_SetWakeupFd(int fd) Py_NOEXCEPT;

/* Support for adding program text to SyntaxErrors */

PyAPI_FUNC(void) PyErr_SyntaxLocationObject(
    PyObject *filename,
    int lineno,
    int col_offset) Py_NOEXCEPT;

PyAPI_FUNC(void) PyErr_RangedSyntaxLocationObject(
    PyObject *filename,
    int lineno,
    int col_offset,
    int end_lineno,
    int end_col_offset) Py_NOEXCEPT;

PyAPI_FUNC(PyObject *) PyErr_ProgramTextObject(
    PyObject *filename,
    int lineno) Py_NOEXCEPT;

PyAPI_FUNC(void) _Py_NO_RETURN _Py_FatalErrorFunc(
    const char *func,
    const char *message) Py_NOEXCEPT;

PyAPI_FUNC(void) PyErr_FormatUnraisable(const char *, ...) Py_NOEXCEPT;

PyAPI_DATA(PyObject *) PyExc_PythonFinalizationError;

#define Py_FatalError(message) _Py_FatalErrorFunc(__func__, (message))
