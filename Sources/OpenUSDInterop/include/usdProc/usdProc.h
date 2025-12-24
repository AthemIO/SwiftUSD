// usdProc.h - UsdProc module C wrapper
// Mirrors: pxr/usd/usdProc/*.h

#ifndef OPENUSDINTEROP_USDPROC_USDPROC_H
#define OPENUSDINTEROP_USDPROC_USDPROC_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Cross-module dependencies
#include "../base/tf.h"
#include "../usd/sdf.h"
#include "../usd/usd.h"

#ifndef USD_INTEROP_API
#if defined(_WIN32)
    #if defined(OPENUSDINTEROP_EXPORTS)
        #define USD_INTEROP_API __declspec(dllexport)
    #else
        #define USD_INTEROP_API __declspec(dllimport)
    #endif
#else
    #define USD_INTEROP_API __attribute__((visibility("default")))
#endif
#endif

#ifndef USD_RESULT_DEFINED
#define USD_RESULT_DEFINED
typedef enum {
    USD_RESULT_SUCCESS = 0,
    USD_RESULT_ERROR = 1,
    USD_RESULT_INVALID_ARGUMENT = 2,
    USD_RESULT_NOT_FOUND = 3,
    USD_RESULT_ALREADY_EXISTS = 4,
    USD_RESULT_OUT_OF_MEMORY = 5,
    USD_RESULT_IO_ERROR = 6
} UsdResult;
#endif

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// MARK: - Opaque Handle Types
// ============================================================================

typedef struct UsdProcGenerativeProceduralOpaque* UsdProcGenerativeProceduralRef;

// ============================================================================
// MARK: - UsdProcGenerativeProcedural
// ============================================================================
// Represents an abstract generative procedural prim which delivers its input
// parameters via properties within the "primvars:" namespace.
//
// It does not itself have any awareness or participation in the execution of
// the procedural but rather serves as a means of delivering a procedural's
// definition and input parameters.
//
// The value of its "proceduralSystem" property indicates to which system
// the procedural definition is meaningful.

/// Defines a new GenerativeProcedural prim at the given path.
/// Caller must release with UsdProcGenerativeProcedural_Release.
USD_INTEROP_API UsdProcGenerativeProceduralRef UsdProcGenerativeProcedural_Define(UsdStageRef stage, SdfPathRef path);

/// Gets a GenerativeProcedural at the given path.
/// Caller must release with UsdProcGenerativeProcedural_Release.
USD_INTEROP_API UsdProcGenerativeProceduralRef UsdProcGenerativeProcedural_Get(UsdStageRef stage, SdfPathRef path);

/// Creates from a prim.
/// Caller must release with UsdProcGenerativeProcedural_Release.
USD_INTEROP_API UsdProcGenerativeProceduralRef UsdProcGenerativeProcedural_FromPrim(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdProcGenerativeProceduralRef UsdProcGenerativeProcedural_Retain(UsdProcGenerativeProceduralRef proc);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdProcGenerativeProcedural_Release(UsdProcGenerativeProceduralRef proc);

/// Returns true if the procedural is valid.
USD_INTEROP_API bool UsdProcGenerativeProcedural_IsValid(UsdProcGenerativeProceduralRef proc);

/// Gets the underlying prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdProcGenerativeProcedural_GetPrim(UsdProcGenerativeProceduralRef proc);

// ============================================================================
// MARK: - ProceduralSystem Attribute
// ============================================================================
// The name or convention of the system responsible for evaluating the procedural.
// NOTE: A fallback value for this is typically set via an API schema.

/// Gets the proceduralSystem token value.
/// Returns a newly allocated string that must be freed with free().
USD_INTEROP_API char* UsdProcGenerativeProcedural_GetProceduralSystem(UsdProcGenerativeProceduralRef proc, UsdTimeCode time);

/// Sets the proceduralSystem token value.
USD_INTEROP_API UsdResult UsdProcGenerativeProcedural_SetProceduralSystem(UsdProcGenerativeProceduralRef proc, const char* system, UsdTimeCode time);

/// Creates the proceduralSystem attribute if it doesn't exist.
/// Returns true if the attribute was successfully created or already exists.
USD_INTEROP_API bool UsdProcGenerativeProcedural_CreateProceduralSystemAttr(UsdProcGenerativeProceduralRef proc, const char* defaultValue);

/// Checks if the proceduralSystem attribute has an authored value.
USD_INTEROP_API bool UsdProcGenerativeProcedural_HasProceduralSystem(UsdProcGenerativeProceduralRef proc);

// ============================================================================
// MARK: - Boundable API (inherited from UsdGeomBoundable)
// ============================================================================

/// Gets the extent (bounding box) at the given time.
/// outExtent must be a float[6] array: [minX, minY, minZ, maxX, maxY, maxZ]
USD_INTEROP_API UsdResult UsdProcGenerativeProcedural_GetExtent(UsdProcGenerativeProceduralRef proc, UsdTimeCode time, float* outExtent);

/// Sets the extent (bounding box) at the given time.
/// extent must be a float[6] array: [minX, minY, minZ, maxX, maxY, maxZ]
USD_INTEROP_API UsdResult UsdProcGenerativeProcedural_SetExtent(UsdProcGenerativeProceduralRef proc, const float* extent, UsdTimeCode time);

#ifdef __cplusplus
}
#endif

#endif // OPENUSDINTEROP_USDPROC_USDPROC_H
