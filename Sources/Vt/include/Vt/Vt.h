#ifndef __PXR_BASE_VT_H__
#define __PXR_BASE_VT_H__

// vt
#include <Vt/api.h>
#include <Vt/array.h>
#include <Vt/dictionary.h>
#include <Vt/hash.h>
#include <Vt/streamOut.h>
#include <Vt/traits.h>
#include <Vt/typeHeaders.h>
#include <Vt/types.h>
#include <Vt/value.h>
#include <Vt/visitValue.h>

#include <Vt/debugCodes.h>
#include <Vt/arrayEdit.h>
#include <Vt/arrayEditBuilder.h>
#include <Vt/arrayEditOps.h>

// Python-related headers - only include when Python support is enabled
#ifdef PXR_PYTHON_SUPPORT_ENABLED
#include <Vt/arrayPyBuffer.h>
#include <Vt/pyOperators.h>
#include <Vt/valueFromPython.h>
#include <Vt/wrapArray.h>
#include <Vt/wrapArrayEdit.h>
#endif // PXR_PYTHON_SUPPORT_ENABLED

#endif  // __PXR_BASE_VT_H__
