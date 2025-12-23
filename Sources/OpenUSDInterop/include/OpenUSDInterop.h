// OpenUSDInterop.h - Umbrella header for USD C++ interop
// This header exposes C-compatible wrappers for Pixar's USD C++ API

#ifndef OPENUSDINTEROP_H
#define OPENUSDINTEROP_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// MARK: - API Export Macros

#if defined(_WIN32)
    #if defined(OPENUSDINTEROP_EXPORTS)
        #define USD_INTEROP_API __declspec(dllexport)
    #else
        #define USD_INTEROP_API __declspec(dllimport)
    #endif
#else
    #define USD_INTEROP_API __attribute__((visibility("default")))
#endif

// MARK: - Result Type

/// Result codes for USD interop operations.
typedef enum {
    USD_RESULT_SUCCESS = 0,
    USD_RESULT_ERROR = 1,
    USD_RESULT_INVALID_ARGUMENT = 2,
    USD_RESULT_NOT_FOUND = 3,
    USD_RESULT_ALREADY_EXISTS = 4,
    USD_RESULT_OUT_OF_MEMORY = 5,
    USD_RESULT_IO_ERROR = 6
} UsdResult;

// MARK: - Opaque Handle Types

/// Opaque handle to a TfToken
typedef struct UsdTfToken_s* UsdTfTokenRef;

/// Opaque handle to an SdfPath
typedef struct UsdSdfPath_s* UsdSdfPathRef;

/// Opaque handle to an SdfLayer
typedef struct UsdSdfLayer_s* UsdSdfLayerRef;

/// Opaque handle to a UsdStage
typedef struct UsdStage_s* UsdStageRef;

/// Opaque handle to a UsdPrim
typedef struct UsdPrim_s* UsdPrimRef;

/// Opaque handle to a UsdAttribute
typedef struct UsdAttribute_s* UsdAttributeRef;

/// Opaque handle to a VtArray
typedef struct UsdVtArray_s* UsdVtArrayRef;

// MARK: - Time Code

/// Represents a USD time code value
typedef struct {
    double value;
} UsdTimeCode;

/// Returns the default time code
USD_INTEROP_API UsdTimeCode UsdTimeCode_Default(void);

/// Returns the earliest time code
USD_INTEROP_API UsdTimeCode UsdTimeCode_EarliestTime(void);

// MARK: - Include Component Headers

#include "base/tf.h"
#include "base/gf.h"
#include "base/vt.h"
#include "usd/stage.h"
#include "usd/prim.h"
#include "usd/attribute.h"
#include "usdGeom/mesh.h"
#include "usdGeom/xform.h"
#include "usdShade/material.h"

#ifdef __cplusplus
}
#endif

#endif // OPENUSDINTEROP_H
