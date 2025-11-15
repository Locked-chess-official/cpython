#ifndef Py_LEGACY_H
#define Py_LEGACY_H

#ifdef Py_OMIT_LEGACY_API
#include "pyconfig_undef.h"

#elif (!defined(Py_BUILD_CORE) \
 && !defined(Py_BUILD_CORE_BUILTIN) \
 && !defined(Py_BUILD_CORE_MODULE))
#ifndef _Python_MODULE_BUILDING

#include "pyconfig_warning.h"
#endif
// Here to redefine the omit CAPI
// Which macros and functions to redefine here belong to the developmenters.

#endif /* Py_OMIT_LEGACY_API */

#endif /* !Py_LEGACY_H */
