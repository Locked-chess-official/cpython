/* Abstract Object Interface (many thanks to Jim Fulton) */

#ifndef Py_ABSTRACTOBJECT_H
#define Py_ABSTRACTOBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

/* === Object Protocol ================================================== */

/* Implemented elsewhere:

   int PyObject_Print(PyObject *o, FILE *fp, int flags);

   Print an object 'o' on file 'fp'.  Returns -1 on error. The flags argument
   is used to enable certain printing options. The only option currently
   supported is Py_PRINT_RAW. By default (flags=0), PyObject_Print() formats
   the object by calling PyObject_Repr(). If flags equals to Py_PRINT_RAW, it
   formats the object by calling PyObject_Str(). */


/* Implemented elsewhere:

   int PyObject_HasAttrString(PyObject *o, const char *attr_name);

   Returns 1 if object 'o' has the attribute attr_name, and 0 otherwise.

   This is equivalent to the Python expression: hasattr(o,attr_name).

   This function always succeeds. */


/* Implemented elsewhere:

   PyObject* PyObject_GetAttrString(PyObject *o, const char *attr_name);

   Retrieve an attributed named attr_name form object o.
   Returns the attribute value on success, or NULL on failure.

   This is the equivalent of the Python expression: o.attr_name. */


/* Implemented elsewhere:

   int PyObject_HasAttr(PyObject *o, PyObject *attr_name);

   Returns 1 if o has the attribute attr_name, and 0 otherwise.

   This is equivalent to the Python expression: hasattr(o,attr_name).

   This function always succeeds. */


/* Implemented elsewhere:

   int PyObject_HasAttrStringWithError(PyObject *o, const char *attr_name);

   Returns 1 if object 'o' has the attribute attr_name, and 0 otherwise.
   This is equivalent to the Python expression: hasattr(o,attr_name).
   Returns -1 on failure. */


/* Implemented elsewhere:

   int PyObject_HasAttrWithError(PyObject *o, PyObject *attr_name);

   Returns 1 if o has the attribute attr_name, and 0 otherwise.
   This is equivalent to the Python expression: hasattr(o,attr_name).
   Returns -1 on failure. */


/* Implemented elsewhere:

   PyObject* PyObject_GetAttr(PyObject *o, PyObject *attr_name);

   Retrieve an attributed named 'attr_name' form object 'o'.
   Returns the attribute value on success, or NULL on failure.

   This is the equivalent of the Python expression: o.attr_name. */


/* Implemented elsewhere:

   int PyObject_GetOptionalAttr(PyObject *obj, PyObject *attr_name, PyObject **result);

   Variant of PyObject_GetAttr() which doesn't raise AttributeError
   if the attribute is not found.

   If the attribute is found, return 1 and set *result to a new strong
   reference to the attribute.
   If the attribute is not found, return 0 and set *result to NULL;
   the AttributeError is silenced.
   If an error other than AttributeError is raised, return -1 and
   set *result to NULL.
*/


/* Implemented elsewhere:

   int PyObject_GetOptionalAttrString(PyObject *obj, const char *attr_name, PyObject **result);

   Variant of PyObject_GetAttrString() which doesn't raise AttributeError
   if the attribute is not found.

   If the attribute is found, return 1 and set *result to a new strong
   reference to the attribute.
   If the attribute is not found, return 0 and set *result to NULL;
   the AttributeError is silenced.
   If an error other than AttributeError is raised, return -1 and
   set *result to NULL.
*/


/* Implemented elsewhere:

   int PyObject_SetAttrString(PyObject *o, const char *attr_name, PyObject *v);

   Set the value of the attribute named attr_name, for object 'o',
   to the value 'v'. Raise an exception and return -1 on failure; return 0 on
   success.

   This is the equivalent of the Python statement o.attr_name=v. */


/* Implemented elsewhere:

   int PyObject_SetAttr(PyObject *o, PyObject *attr_name, PyObject *v);

   Set the value of the attribute named attr_name, for object 'o', to the value
   'v'. an exception and return -1 on failure; return 0 on success.

   This is the equivalent of the Python statement o.attr_name=v. */

/* Implemented elsewhere:

   int PyObject_DelAttrString(PyObject *o, const char *attr_name);

   Delete attribute named attr_name, for object o. Returns
   -1 on failure.

   This is the equivalent of the Python statement: del o.attr_name.

   Implemented as a macro in the limited C API 3.12 and older. */
#if defined(Py_LIMITED_API) && Py_LIMITED_API+0 < 0x030d0000
#  define PyObject_DelAttrString(O, A) PyObject_SetAttrString((O), (A), NULL)
#endif


/* Implemented elsewhere:

   int PyObject_DelAttr(PyObject *o, PyObject *attr_name);

   Delete attribute named attr_name, for object o. Returns -1
   on failure.  This is the equivalent of the Python
   statement: del o.attr_name.

   Implemented as a macro in the limited C API 3.12 and older. */
#if defined(Py_LIMITED_API) && Py_LIMITED_API+0 < 0x030d0000
#  define PyObject_DelAttr(O, A) PyObject_SetAttr((O), (A), NULL)
#endif


/* Implemented elsewhere:

   PyObject *PyObject_Repr(PyObject *o);

   Compute the string representation of object 'o'.  Returns the
   string representation on success, NULL on failure.

   This is the equivalent of the Python expression: repr(o).

   Called by the repr() built-in function. */


/* Implemented elsewhere:

   PyObject *PyObject_Str(PyObject *o);

   Compute the string representation of object, o.  Returns the
   string representation on success, NULL on failure.

   This is the equivalent of the Python expression: str(o).

   Called by the str() and print() built-in functions. */


/* Declared elsewhere

   PyAPI_FUNC(int) PyCallable_Check(PyObject *o) Py_NOEXCEPT;

   Determine if the object, o, is callable.  Return 1 if the object is callable
   and 0 otherwise.

   This function always succeeds. */


#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x03090000
/* Call a callable Python object without any arguments */
PyAPI_FUNC(PyObject *) PyObject_CallNoArgs(PyObject *func) Py_NOEXCEPT;
#endif


/* Call a callable Python object 'callable' with arguments given by the
   tuple 'args' and keywords arguments given by the dictionary 'kwargs'.

   'args' must not be NULL, use an empty tuple if no arguments are
   needed. If no named arguments are needed, 'kwargs' can be NULL.

   This is the equivalent of the Python expression:
   callable(*args, **kwargs). */
PyAPI_FUNC(PyObject *) PyObject_Call(PyObject *callable,
                                     PyObject *args, PyObject *kwargs) Py_NOEXCEPT;


/* Call a callable Python object 'callable', with arguments given by the
   tuple 'args'.  If no arguments are needed, then 'args' can be NULL.

   Returns the result of the call on success, or NULL on failure.

   This is the equivalent of the Python expression:
   callable(*args). */
PyAPI_FUNC(PyObject *) PyObject_CallObject(PyObject *callable,
                                           PyObject *args) Py_NOEXCEPT;

/* Call a callable Python object, callable, with a variable number of C
   arguments. The C arguments are described using a mkvalue-style format
   string.

   The format may be NULL, indicating that no arguments are provided.

   Returns the result of the call on success, or NULL on failure.

   This is the equivalent of the Python expression:
   callable(arg1, arg2, ...). */
PyAPI_FUNC(PyObject *) PyObject_CallFunction(PyObject *callable,
                                             const char *format, ...) Py_NOEXCEPT;

/* Call the method named 'name' of object 'obj' with a variable number of
   C arguments.  The C arguments are described by a mkvalue format string.

   The format can be NULL, indicating that no arguments are provided.

   Returns the result of the call on success, or NULL on failure.

   This is the equivalent of the Python expression:
   obj.name(arg1, arg2, ...). */
PyAPI_FUNC(PyObject *) PyObject_CallMethod(PyObject *obj,
                                           const char *name,
                                           const char *format, ...) Py_NOEXCEPT;

/* Call a callable Python object 'callable' with a variable number of C
   arguments. The C arguments are provided as PyObject* values, terminated
   by a NULL.

   Returns the result of the call on success, or NULL on failure.

   This is the equivalent of the Python expression:
   callable(arg1, arg2, ...). */
PyAPI_FUNC(PyObject *) PyObject_CallFunctionObjArgs(PyObject *callable,
                                                    ...) Py_NOEXCEPT;

/* Call the method named 'name' of object 'obj' with a variable number of
   C arguments.  The C arguments are provided as PyObject* values, terminated
   by NULL.

   Returns the result of the call on success, or NULL on failure.

   This is the equivalent of the Python expression: obj.name(*args). */

PyAPI_FUNC(PyObject *) PyObject_CallMethodObjArgs(
    PyObject *obj,
    PyObject *name,
    ...) Py_NOEXCEPT;

/* Given a vectorcall nargsf argument, return the actual number of arguments.
 * (For use outside the limited API, this is re-defined as a static inline
 * function in cpython/abstract.h)
 */
PyAPI_FUNC(Py_ssize_t) PyVectorcall_NARGS(size_t nargsf) Py_NOEXCEPT;

/* Call "callable" (which must support vectorcall) with positional arguments
   "tuple" and keyword arguments "dict". "dict" may also be NULL */
PyAPI_FUNC(PyObject *) PyVectorcall_Call(PyObject *callable, PyObject *tuple, PyObject *dict) Py_NOEXCEPT;

#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x030C0000
#define PY_VECTORCALL_ARGUMENTS_OFFSET \
    (_Py_STATIC_CAST(size_t, 1) << (8 * sizeof(size_t) - 1))

/* Perform a PEP 590-style vector call on 'callable' */
PyAPI_FUNC(PyObject *) PyObject_Vectorcall(
    PyObject *callable,
    PyObject *const *args,
    size_t nargsf,
    PyObject *kwnames) Py_NOEXCEPT;

/* Call the method 'name' on args[0] with arguments in args[1..nargsf-1]. */
PyAPI_FUNC(PyObject *) PyObject_VectorcallMethod(
    PyObject *name, PyObject *const *args,
    size_t nargsf, PyObject *kwnames) Py_NOEXCEPT;
#endif

/* Implemented elsewhere:

   Py_hash_t PyObject_Hash(PyObject *o);

   Compute and return the hash, hash_value, of an object, o.  On
   failure, return -1.

   This is the equivalent of the Python expression: hash(o). */


/* Implemented elsewhere:

   int PyObject_IsTrue(PyObject *o);

   Returns 1 if the object, o, is considered to be true, 0 if o is
   considered to be false and -1 on failure.

   This is equivalent to the Python expression: not not o. */


/* Implemented elsewhere:

   int PyObject_Not(PyObject *o);

   Returns 0 if the object, o, is considered to be true, 1 if o is
   considered to be false and -1 on failure.

   This is equivalent to the Python expression: not o. */


/* Get the type of an object.

   On success, returns a type object corresponding to the object type of object
   'o'. On failure, returns NULL.

   This is equivalent to the Python expression: type(o) */
PyAPI_FUNC(PyObject *) PyObject_Type(PyObject *o) Py_NOEXCEPT;


/* Return the size of object 'o'.  If the object 'o' provides both sequence and
   mapping protocols, the sequence size is returned.

   On error, -1 is returned.

   This is the equivalent to the Python expression: len(o) */
PyAPI_FUNC(Py_ssize_t) PyObject_Size(PyObject *o) Py_NOEXCEPT;


/* For DLL compatibility */
#undef PyObject_Length
PyAPI_FUNC(Py_ssize_t) PyObject_Length(PyObject *o) Py_NOEXCEPT;
#define PyObject_Length PyObject_Size

/* Return element of 'o' corresponding to the object 'key'. Return NULL
  on failure.

  This is the equivalent of the Python expression: o[key] */
PyAPI_FUNC(PyObject *) PyObject_GetItem(PyObject *o, PyObject *key) Py_NOEXCEPT;


/* Map the object 'key' to the value 'v' into 'o'.

   Raise an exception and return -1 on failure; return 0 on success.

   This is the equivalent of the Python statement: o[key]=v. */
PyAPI_FUNC(int) PyObject_SetItem(PyObject *o, PyObject *key, PyObject *v) Py_NOEXCEPT;

/* Remove the mapping for the string 'key' from the object 'o'.
   Returns -1 on failure.

   This is equivalent to the Python statement: del o[key]. */
PyAPI_FUNC(int) PyObject_DelItemString(PyObject *o, const char *key) Py_NOEXCEPT;

/* Delete the mapping for the object 'key' from the object 'o'.
   Returns -1 on failure.

   This is the equivalent of the Python statement: del o[key]. */
PyAPI_FUNC(int) PyObject_DelItem(PyObject *o, PyObject *key) Py_NOEXCEPT;


/* Takes an arbitrary object and returns the result of calling
   obj.__format__(format_spec). */
PyAPI_FUNC(PyObject *) PyObject_Format(PyObject *obj,
                                       PyObject *format_spec) Py_NOEXCEPT;


/* ==== Iterators ================================================ */

/* Takes an object and returns an iterator for it.
   This is typically a new iterator but if the argument is an iterator, this
   returns itself. */
PyAPI_FUNC(PyObject *) PyObject_GetIter(PyObject *) Py_NOEXCEPT;

/* Takes an AsyncIterable object and returns an AsyncIterator for it.
   This is typically a new iterator but if the argument is an AsyncIterator,
   this returns itself. */
PyAPI_FUNC(PyObject *) PyObject_GetAIter(PyObject *) Py_NOEXCEPT;

/* Returns non-zero if the object 'obj' provides iterator protocols, and 0 otherwise.

   This function always succeeds. */
PyAPI_FUNC(int) PyIter_Check(PyObject *) Py_NOEXCEPT;

/* Returns non-zero if the object 'obj' provides AsyncIterator protocols, and 0 otherwise.

   This function always succeeds. */
PyAPI_FUNC(int) PyAIter_Check(PyObject *) Py_NOEXCEPT;

#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x030e0000
/* Return 1 and set 'item' to the next item of 'iter' on success.
 * Return 0 and set 'item' to NULL when there are no remaining values.
 * Return -1, set 'item' to NULL and set an exception on error.
 */
PyAPI_FUNC(int) PyIter_NextItem(PyObject *iter, PyObject **item) Py_NOEXCEPT;
#endif

/* Takes an iterator object and calls its tp_iternext slot,
   returning the next value.

   If the iterator is exhausted, this returns NULL without setting an
   exception.

   NULL with an exception means an error occurred.

   Prefer PyIter_NextItem() instead. */
PyAPI_FUNC(PyObject *) PyIter_Next(PyObject *) Py_NOEXCEPT;

#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x030A0000

/* Takes generator, coroutine or iterator object and sends the value into it.
   Returns:
   - PYGEN_RETURN (0) if generator has returned.
     'result' parameter is filled with return value
   - PYGEN_ERROR (-1) if exception was raised.
     'result' parameter is NULL
   - PYGEN_NEXT (1) if generator has yielded.
     'result' parameter is filled with yielded value. */
PyAPI_FUNC(PySendResult) PyIter_Send(PyObject *, PyObject *, PyObject **) Py_NOEXCEPT;
#endif


/* === Number Protocol ================================================== */

/* Returns 1 if the object 'o' provides numeric protocols, and 0 otherwise.

   This function always succeeds. */
PyAPI_FUNC(int) PyNumber_Check(PyObject *o) Py_NOEXCEPT;

/* Returns the result of adding o1 and o2, or NULL on failure.

   This is the equivalent of the Python expression: o1 + o2. */
PyAPI_FUNC(PyObject *) PyNumber_Add(PyObject *o1, PyObject *o2) Py_NOEXCEPT;

/* Returns the result of subtracting o2 from o1, or NULL on failure.

   This is the equivalent of the Python expression: o1 - o2. */
PyAPI_FUNC(PyObject *) PyNumber_Subtract(PyObject *o1, PyObject *o2) Py_NOEXCEPT;

/* Returns the result of multiplying o1 and o2, or NULL on failure.

   This is the equivalent of the Python expression: o1 * o2. */
PyAPI_FUNC(PyObject *) PyNumber_Multiply(PyObject *o1, PyObject *o2) Py_NOEXCEPT;

#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x03050000
/* This is the equivalent of the Python expression: o1 @ o2. */
PyAPI_FUNC(PyObject *) PyNumber_MatrixMultiply(PyObject *o1, PyObject *o2) Py_NOEXCEPT;
#endif

/* Returns the result of dividing o1 by o2 giving an integral result,
   or NULL on failure.

   This is the equivalent of the Python expression: o1 // o2. */
PyAPI_FUNC(PyObject *) PyNumber_FloorDivide(PyObject *o1, PyObject *o2) Py_NOEXCEPT;

/* Returns the result of dividing o1 by o2 giving a float result, or NULL on
   failure.

   This is the equivalent of the Python expression: o1 / o2. */
PyAPI_FUNC(PyObject *) PyNumber_TrueDivide(PyObject *o1, PyObject *o2) Py_NOEXCEPT;

/* Returns the remainder of dividing o1 by o2, or NULL on failure.

   This is the equivalent of the Python expression: o1 % o2. */
PyAPI_FUNC(PyObject *) PyNumber_Remainder(PyObject *o1, PyObject *o2) Py_NOEXCEPT;

/* See the built-in function divmod.

   Returns NULL on failure.

   This is the equivalent of the Python expression: divmod(o1, o2). */
PyAPI_FUNC(PyObject *) PyNumber_Divmod(PyObject *o1, PyObject *o2) Py_NOEXCEPT;

/* See the built-in function pow. Returns NULL on failure.

   This is the equivalent of the Python expression: pow(o1, o2, o3),
   where o3 is optional. */
PyAPI_FUNC(PyObject *) PyNumber_Power(PyObject *o1, PyObject *o2,
                                      PyObject *o3) Py_NOEXCEPT;

/* Returns the negation of o on success, or NULL on failure.

 This is the equivalent of the Python expression: -o. */
PyAPI_FUNC(PyObject *) PyNumber_Negative(PyObject *o) Py_NOEXCEPT;

/* Returns the positive of o on success, or NULL on failure.

   This is the equivalent of the Python expression: +o. */
PyAPI_FUNC(PyObject *) PyNumber_Positive(PyObject *o) Py_NOEXCEPT;

/* Returns the absolute value of 'o', or NULL on failure.

   This is the equivalent of the Python expression: abs(o). */
PyAPI_FUNC(PyObject *) PyNumber_Absolute(PyObject *o) Py_NOEXCEPT;

/* Returns the bitwise negation of 'o' on success, or NULL on failure.

   This is the equivalent of the Python expression: ~o. */
PyAPI_FUNC(PyObject *) PyNumber_Invert(PyObject *o) Py_NOEXCEPT;

/* Returns the result of left shifting o1 by o2 on success, or NULL on failure.

   This is the equivalent of the Python expression: o1 << o2. */
PyAPI_FUNC(PyObject *) PyNumber_Lshift(PyObject *o1, PyObject *o2) Py_NOEXCEPT;

/* Returns the result of right shifting o1 by o2 on success, or NULL on
   failure.

   This is the equivalent of the Python expression: o1 >> o2. */
PyAPI_FUNC(PyObject *) PyNumber_Rshift(PyObject *o1, PyObject *o2) Py_NOEXCEPT;

/* Returns the result of bitwise and of o1 and o2 on success, or NULL on
   failure.

   This is the equivalent of the Python expression: o1 & o2. */
PyAPI_FUNC(PyObject *) PyNumber_And(PyObject *o1, PyObject *o2) Py_NOEXCEPT;

/* Returns the bitwise exclusive or of o1 by o2 on success, or NULL on failure.

   This is the equivalent of the Python expression: o1 ^ o2. */
PyAPI_FUNC(PyObject *) PyNumber_Xor(PyObject *o1, PyObject *o2) Py_NOEXCEPT;

/* Returns the result of bitwise or on o1 and o2 on success, or NULL on
   failure.

   This is the equivalent of the Python expression: o1 | o2. */
PyAPI_FUNC(PyObject *) PyNumber_Or(PyObject *o1, PyObject *o2) Py_NOEXCEPT;

/* Returns 1 if obj is an index integer (has the nb_index slot of the
   tp_as_number structure filled in), and 0 otherwise. */
PyAPI_FUNC(int) PyIndex_Check(PyObject *) Py_NOEXCEPT;

/* Returns the object 'o' converted to a Python int, or NULL with an exception
   raised on failure. */
PyAPI_FUNC(PyObject *) PyNumber_Index(PyObject *o) Py_NOEXCEPT;

/* Returns the object 'o' converted to Py_ssize_t by going through
   PyNumber_Index() first.

   If an overflow error occurs while converting the int to Py_ssize_t, then the
   second argument 'exc' is the error-type to return.  If it is NULL, then the
   overflow error is cleared and the value is clipped. */
PyAPI_FUNC(Py_ssize_t) PyNumber_AsSsize_t(PyObject *o, PyObject *exc) Py_NOEXCEPT;

/* Returns the object 'o' converted to an integer object on success, or NULL
   on failure.

   This is the equivalent of the Python expression: int(o). */
PyAPI_FUNC(PyObject *) PyNumber_Long(PyObject *o) Py_NOEXCEPT;

/* Returns the object 'o' converted to a float object on success, or NULL
  on failure.

  This is the equivalent of the Python expression: float(o). */
PyAPI_FUNC(PyObject *) PyNumber_Float(PyObject *o) Py_NOEXCEPT;


/* --- In-place variants of (some of) the above number protocol functions -- */

/* Returns the result of adding o2 to o1, possibly in-place, or NULL
   on failure.

   This is the equivalent of the Python expression: o1 += o2. */
PyAPI_FUNC(PyObject *) PyNumber_InPlaceAdd(PyObject *o1, PyObject *o2) Py_NOEXCEPT;

/* Returns the result of subtracting o2 from o1, possibly in-place or
   NULL on failure.

   This is the equivalent of the Python expression: o1 -= o2. */
PyAPI_FUNC(PyObject *) PyNumber_InPlaceSubtract(PyObject *o1, PyObject *o2) Py_NOEXCEPT;

/* Returns the result of multiplying o1 by o2, possibly in-place, or NULL on
   failure.

   This is the equivalent of the Python expression: o1 *= o2. */
PyAPI_FUNC(PyObject *) PyNumber_InPlaceMultiply(PyObject *o1, PyObject *o2) Py_NOEXCEPT;

#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x03050000
/* This is the equivalent of the Python expression: o1 @= o2. */
PyAPI_FUNC(PyObject *) PyNumber_InPlaceMatrixMultiply(PyObject *o1, PyObject *o2) Py_NOEXCEPT;
#endif

/* Returns the result of dividing o1 by o2 giving an integral result, possibly
   in-place, or NULL on failure.

   This is the equivalent of the Python expression: o1 /= o2. */
PyAPI_FUNC(PyObject *) PyNumber_InPlaceFloorDivide(PyObject *o1,
                                                   PyObject *o2) Py_NOEXCEPT;

/* Returns the result of dividing o1 by o2 giving a float result, possibly
   in-place, or null on failure.

   This is the equivalent of the Python expression: o1 /= o2. */
PyAPI_FUNC(PyObject *) PyNumber_InPlaceTrueDivide(PyObject *o1,
                                                  PyObject *o2) Py_NOEXCEPT;

/* Returns the remainder of dividing o1 by o2, possibly in-place, or NULL on
   failure.

   This is the equivalent of the Python expression: o1 %= o2. */
PyAPI_FUNC(PyObject *) PyNumber_InPlaceRemainder(PyObject *o1, PyObject *o2) Py_NOEXCEPT;

/* Returns the result of raising o1 to the power of o2, possibly in-place,
   or NULL on failure.

   This is the equivalent of the Python expression: o1 **= o2,
   or o1 = pow(o1, o2, o3) if o3 is present. */
PyAPI_FUNC(PyObject *) PyNumber_InPlacePower(PyObject *o1, PyObject *o2,
                                             PyObject *o3) Py_NOEXCEPT;

/* Returns the result of left shifting o1 by o2, possibly in-place, or NULL
   on failure.

   This is the equivalent of the Python expression: o1 <<= o2. */
PyAPI_FUNC(PyObject *) PyNumber_InPlaceLshift(PyObject *o1, PyObject *o2) Py_NOEXCEPT;

/* Returns the result of right shifting o1 by o2, possibly in-place or NULL
   on failure.

   This is the equivalent of the Python expression: o1 >>= o2. */
PyAPI_FUNC(PyObject *) PyNumber_InPlaceRshift(PyObject *o1, PyObject *o2) Py_NOEXCEPT;

/* Returns the result of bitwise and of o1 and o2, possibly in-place, or NULL
   on failure.

   This is the equivalent of the Python expression: o1 &= o2. */
PyAPI_FUNC(PyObject *) PyNumber_InPlaceAnd(PyObject *o1, PyObject *o2) Py_NOEXCEPT;

/* Returns the bitwise exclusive or of o1 by o2, possibly in-place, or NULL
   on failure.

   This is the equivalent of the Python expression: o1 ^= o2. */
PyAPI_FUNC(PyObject *) PyNumber_InPlaceXor(PyObject *o1, PyObject *o2) Py_NOEXCEPT;

/* Returns the result of bitwise or of o1 and o2, possibly in-place,
   or NULL on failure.

   This is the equivalent of the Python expression: o1 |= o2. */
PyAPI_FUNC(PyObject *) PyNumber_InPlaceOr(PyObject *o1, PyObject *o2) Py_NOEXCEPT;

/* Returns the integer n converted to a string with a base, with a base
   marker of 0b, 0o or 0x prefixed if applicable.

   If n is not an int object, it is converted with PyNumber_Index first. */
PyAPI_FUNC(PyObject *) PyNumber_ToBase(PyObject *n, int base) Py_NOEXCEPT;


/* === Sequence protocol ================================================ */

/* Return 1 if the object provides sequence protocol, and zero
   otherwise.

   This function always succeeds. */
PyAPI_FUNC(int) PySequence_Check(PyObject *o) Py_NOEXCEPT;

/* Return the size of sequence object o, or -1 on failure. */
PyAPI_FUNC(Py_ssize_t) PySequence_Size(PyObject *o) Py_NOEXCEPT;

/* For DLL compatibility */
#undef PySequence_Length
PyAPI_FUNC(Py_ssize_t) PySequence_Length(PyObject *o) Py_NOEXCEPT;
#define PySequence_Length PySequence_Size


/* Return the concatenation of o1 and o2 on success, and NULL on failure.

   This is the equivalent of the Python expression: o1 + o2. */
PyAPI_FUNC(PyObject *) PySequence_Concat(PyObject *o1, PyObject *o2) Py_NOEXCEPT;

/* Return the result of repeating sequence object 'o' 'count' times,
  or NULL on failure.

  This is the equivalent of the Python expression: o * count. */
PyAPI_FUNC(PyObject *) PySequence_Repeat(PyObject *o, Py_ssize_t count) Py_NOEXCEPT;

/* Return the ith element of o, or NULL on failure.

   This is the equivalent of the Python expression: o[i]. */
PyAPI_FUNC(PyObject *) PySequence_GetItem(PyObject *o, Py_ssize_t i) Py_NOEXCEPT;

/* Return the slice of sequence object o between i1 and i2, or NULL on failure.

   This is the equivalent of the Python expression: o[i1:i2]. */
PyAPI_FUNC(PyObject *) PySequence_GetSlice(PyObject *o, Py_ssize_t i1, Py_ssize_t i2) Py_NOEXCEPT;

/* Assign object 'v' to the ith element of the sequence 'o'. Raise an exception
   and return -1 on failure; return 0 on success.

   This is the equivalent of the Python statement o[i] = v. */
PyAPI_FUNC(int) PySequence_SetItem(PyObject *o, Py_ssize_t i, PyObject *v) Py_NOEXCEPT;

/* Delete the 'i'-th element of the sequence 'v'. Returns -1 on failure.

   This is the equivalent of the Python statement: del o[i]. */
PyAPI_FUNC(int) PySequence_DelItem(PyObject *o, Py_ssize_t i) Py_NOEXCEPT;

/* Assign the sequence object 'v' to the slice in sequence object 'o',
   from 'i1' to 'i2'. Returns -1 on failure.

   This is the equivalent of the Python statement: o[i1:i2] = v. */
PyAPI_FUNC(int) PySequence_SetSlice(PyObject *o, Py_ssize_t i1, Py_ssize_t i2,
                                    PyObject *v) Py_NOEXCEPT;

/* Delete the slice in sequence object 'o' from 'i1' to 'i2'.
   Returns -1 on failure.

   This is the equivalent of the Python statement: del o[i1:i2]. */
PyAPI_FUNC(int) PySequence_DelSlice(PyObject *o, Py_ssize_t i1, Py_ssize_t i2) Py_NOEXCEPT;

/* Returns the sequence 'o' as a tuple on success, and NULL on failure.

   This is equivalent to the Python expression: tuple(o). */
PyAPI_FUNC(PyObject *) PySequence_Tuple(PyObject *o) Py_NOEXCEPT;

/* Returns the sequence 'o' as a list on success, and NULL on failure.
   This is equivalent to the Python expression: list(o) */
PyAPI_FUNC(PyObject *) PySequence_List(PyObject *o) Py_NOEXCEPT;

/* Return the sequence 'o' as a list, unless it's already a tuple or list.

   Use PySequence_Fast_GET_ITEM to access the members of this list, and
   PySequence_Fast_GET_SIZE to get its length.

   Returns NULL on failure.  If the object does not support iteration, raises a
   TypeError exception with 'm' as the message text. */
PyAPI_FUNC(PyObject *) PySequence_Fast(PyObject *o, const char* m) Py_NOEXCEPT;

/* Return the number of occurrences on value on 'o', that is, return
   the number of keys for which o[key] == value.

   On failure, return -1.  This is equivalent to the Python expression:
   o.count(value). */
PyAPI_FUNC(Py_ssize_t) PySequence_Count(PyObject *o, PyObject *value) Py_NOEXCEPT;

/* Return 1 if 'ob' is in the sequence 'seq'; 0 if 'ob' is not in the sequence
   'seq'; -1 on error.

   Use __contains__ if possible, else _PySequence_IterSearch(). */
PyAPI_FUNC(int) PySequence_Contains(PyObject *seq, PyObject *ob) Py_NOEXCEPT;

/* For DLL-level backwards compatibility */
#undef PySequence_In
/* Determine if the sequence 'o' contains 'value'. If an item in 'o' is equal
   to 'value', return 1, otherwise return 0. On error, return -1.

   This is equivalent to the Python expression: value in o. */
PyAPI_FUNC(int) PySequence_In(PyObject *o, PyObject *value) Py_NOEXCEPT;

/* For source-level backwards compatibility */
#define PySequence_In PySequence_Contains


/* Return the first index for which o[i] == value.
   On error, return -1.

   This is equivalent to the Python expression: o.index(value). */
PyAPI_FUNC(Py_ssize_t) PySequence_Index(PyObject *o, PyObject *value) Py_NOEXCEPT;


/* --- In-place versions of some of the above Sequence functions --- */

/* Append sequence 'o2' to sequence 'o1', in-place when possible. Return the
   resulting object, which could be 'o1', or NULL on failure.

  This is the equivalent of the Python expression: o1 += o2. */
PyAPI_FUNC(PyObject *) PySequence_InPlaceConcat(PyObject *o1, PyObject *o2) Py_NOEXCEPT;

/* Repeat sequence 'o' by 'count', in-place when possible. Return the resulting
   object, which could be 'o', or NULL on failure.

   This is the equivalent of the Python expression: o1 *= count.  */
PyAPI_FUNC(PyObject *) PySequence_InPlaceRepeat(PyObject *o, Py_ssize_t count) Py_NOEXCEPT;


/* === Mapping protocol ================================================= */

/* Return 1 if the object provides mapping protocol, and 0 otherwise.

   This function always succeeds. */
PyAPI_FUNC(int) PyMapping_Check(PyObject *o) Py_NOEXCEPT;

/* Returns the number of keys in mapping object 'o' on success, and -1 on
  failure. This is equivalent to the Python expression: len(o). */
PyAPI_FUNC(Py_ssize_t) PyMapping_Size(PyObject *o) Py_NOEXCEPT;

/* For DLL compatibility */
#undef PyMapping_Length
PyAPI_FUNC(Py_ssize_t) PyMapping_Length(PyObject *o) Py_NOEXCEPT;
#define PyMapping_Length PyMapping_Size


/* Implemented as a macro:

   int PyMapping_DelItemString(PyObject *o, const char *key);

   Remove the mapping for the string 'key' from the mapping 'o'. Returns -1 on
   failure.

   This is equivalent to the Python statement: del o[key]. */
#define PyMapping_DelItemString(O, K) PyObject_DelItemString((O), (K))

/* Implemented as a macro:

   int PyMapping_DelItem(PyObject *o, PyObject *key);

   Remove the mapping for the object 'key' from the mapping object 'o'.
   Returns -1 on failure.

   This is equivalent to the Python statement: del o[key]. */
#define PyMapping_DelItem(O, K) PyObject_DelItem((O), (K))

/* On success, return 1 if the mapping object 'o' has the key 'key',
   and 0 otherwise.

   This is equivalent to the Python expression: key in o.

   This function always succeeds. */
PyAPI_FUNC(int) PyMapping_HasKeyString(PyObject *o, const char *key) Py_NOEXCEPT;

/* Return 1 if the mapping object has the key 'key', and 0 otherwise.

   This is equivalent to the Python expression: key in o.

   This function always succeeds. */
PyAPI_FUNC(int) PyMapping_HasKey(PyObject *o, PyObject *key) Py_NOEXCEPT;

/* Return 1 if the mapping object has the key 'key', and 0 otherwise.
   This is equivalent to the Python expression: key in o.
   On failure, return -1. */

PyAPI_FUNC(int) PyMapping_HasKeyWithError(PyObject *o, PyObject *key) Py_NOEXCEPT;

/* Return 1 if the mapping object has the key 'key', and 0 otherwise.
   This is equivalent to the Python expression: key in o.
   On failure, return -1. */

PyAPI_FUNC(int) PyMapping_HasKeyStringWithError(PyObject *o, const char *key) Py_NOEXCEPT;

/* On success, return a list of the keys in mapping object 'o'.
   On failure, return NULL. */
PyAPI_FUNC(PyObject *) PyMapping_Keys(PyObject *o) Py_NOEXCEPT;

/* On success, return a list of the values in mapping object 'o'.
   On failure, return NULL. */
PyAPI_FUNC(PyObject *) PyMapping_Values(PyObject *o) Py_NOEXCEPT;

/* On success, return a list of the items in mapping object 'o',
   where each item is a tuple containing a key-value pair. On failure, return
   NULL. */
PyAPI_FUNC(PyObject *) PyMapping_Items(PyObject *o) Py_NOEXCEPT;

/* Return element of 'o' corresponding to the string 'key' or NULL on failure.

   This is the equivalent of the Python expression: o[key]. */
PyAPI_FUNC(PyObject *) PyMapping_GetItemString(PyObject *o,
                                               const char *key) Py_NOEXCEPT;

/* Variants of PyObject_GetItem() and PyMapping_GetItemString() which don't
   raise KeyError if the key is not found.

   If the key is found, return 1 and set *result to a new strong
   reference to the corresponding value.
   If the key is not found, return 0 and set *result to NULL;
   the KeyError is silenced.
   If an error other than KeyError is raised, return -1 and
   set *result to NULL.
*/
#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x030d0000
PyAPI_FUNC(int) PyMapping_GetOptionalItem(PyObject *, PyObject *, PyObject **) Py_NOEXCEPT;
PyAPI_FUNC(int) PyMapping_GetOptionalItemString(PyObject *, const char *, PyObject **) Py_NOEXCEPT;
#endif

/* Map the string 'key' to the value 'v' in the mapping 'o'.
   Returns -1 on failure.

   This is the equivalent of the Python statement: o[key]=v. */
PyAPI_FUNC(int) PyMapping_SetItemString(PyObject *o, const char *key,
                                        PyObject *value) Py_NOEXCEPT;

/* isinstance(object, typeorclass) */
PyAPI_FUNC(int) PyObject_IsInstance(PyObject *object, PyObject *typeorclass) Py_NOEXCEPT;

/* issubclass(object, typeorclass) */
PyAPI_FUNC(int) PyObject_IsSubclass(PyObject *object, PyObject *typeorclass) Py_NOEXCEPT;

#ifndef Py_LIMITED_API
#  define Py_CPYTHON_ABSTRACTOBJECT_H
#  include "cpython/abstract.h"
#  undef Py_CPYTHON_ABSTRACTOBJECT_H
#endif

#ifdef __cplusplus
}
#endif
#endif /* Py_ABSTRACTOBJECT_H */
