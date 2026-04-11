#ifndef _Py_CPYTHON_MARSHAL_H
#  error "this header file must not be included directly"
#endif

PyAPI_FUNC(PyObject *) PyMarshal_ReadObjectFromString(const char *,
                                                      Py_ssize_t) Py_NOEXCEPT;
PyAPI_FUNC(PyObject *) PyMarshal_WriteObjectToString(PyObject *, int) Py_NOEXCEPT;

#define Py_MARSHAL_VERSION 6

PyAPI_FUNC(long) PyMarshal_ReadLongFromFile(FILE *) Py_NOEXCEPT;
PyAPI_FUNC(int) PyMarshal_ReadShortFromFile(FILE *) Py_NOEXCEPT;
PyAPI_FUNC(PyObject *) PyMarshal_ReadObjectFromFile(FILE *) Py_NOEXCEPT;
PyAPI_FUNC(PyObject *) PyMarshal_ReadLastObjectFromFile(FILE *) Py_NOEXCEPT;

PyAPI_FUNC(void) PyMarshal_WriteLongToFile(long, FILE *, int) Py_NOEXCEPT;
PyAPI_FUNC(void) PyMarshal_WriteObjectToFile(PyObject *, FILE *, int) Py_NOEXCEPT;
