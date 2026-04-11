#ifndef Py_INTRCHECK_H
#define Py_INTRCHECK_H
#ifdef __cplusplus
extern "C" {
#endif

PyAPI_FUNC(int) PyOS_InterruptOccurred(void) Py_NOEXCEPT;

#ifdef HAVE_FORK
#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x03070000
PyAPI_FUNC(void) PyOS_BeforeFork(void) Py_NOEXCEPT;
PyAPI_FUNC(void) PyOS_AfterFork_Parent(void) Py_NOEXCEPT;
PyAPI_FUNC(void) PyOS_AfterFork_Child(void) Py_NOEXCEPT;
#endif
#endif

/* Deprecated, please use PyOS_AfterFork_Child() instead */
Py_DEPRECATED(3.7) PyAPI_FUNC(void) PyOS_AfterFork(void) Py_NOEXCEPT;

#ifdef __cplusplus
}
#endif
#endif /* !Py_INTRCHECK_H */
