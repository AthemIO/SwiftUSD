// vt.h - VtArray C wrapper
// Mirrors: pxr/base/vt/array.h

#ifndef OPENUSDINTEROP_BASE_VT_H
#define OPENUSDINTEROP_BASE_VT_H

#include "../OpenUSDInterop.h"
#include "gf.h"

#ifdef __cplusplus
extern "C" {
#endif

// MARK: - VtArray Types

/// Enum for VtArray element types.
typedef enum {
    USD_VT_TYPE_BOOL,
    USD_VT_TYPE_INT,
    USD_VT_TYPE_FLOAT,
    USD_VT_TYPE_DOUBLE,
    USD_VT_TYPE_VEC3F,
    USD_VT_TYPE_VEC3D,
    USD_VT_TYPE_MATRIX4D,
    USD_VT_TYPE_TOKEN,
    USD_VT_TYPE_STRING
} UsdVtArrayType;

// MARK: - VtArray Creation

/// Creates an empty array of the given type.
USD_INTEROP_API UsdResult UsdVtArray_Create(UsdVtArrayType type, UsdVtArrayRef* outArray);

/// Creates an array with the given capacity.
USD_INTEROP_API UsdResult UsdVtArray_CreateWithCapacity(UsdVtArrayType type, size_t capacity, UsdVtArrayRef* outArray);

/// Destroys an array.
USD_INTEROP_API void UsdVtArray_Destroy(UsdVtArrayRef array);

// MARK: - VtArray Properties

/// Returns the number of elements in the array.
USD_INTEROP_API size_t UsdVtArray_GetSize(UsdVtArrayRef array);

/// Returns the type of elements in the array.
USD_INTEROP_API UsdVtArrayType UsdVtArray_GetType(UsdVtArrayRef array);

/// Returns true if the array is empty.
USD_INTEROP_API bool UsdVtArray_IsEmpty(UsdVtArrayRef array);

// MARK: - VtArray Data Access

/// Returns a pointer to the raw data. Caller must cast to appropriate type.
USD_INTEROP_API const void* UsdVtArray_GetData(UsdVtArrayRef array);

/// Returns a mutable pointer to the raw data.
USD_INTEROP_API void* UsdVtArray_GetMutableData(UsdVtArrayRef array);

// MARK: - VtArray<int>

/// Creates an int array from data.
USD_INTEROP_API UsdResult UsdVtIntArray_CreateFromData(const int32_t* data, size_t count, UsdVtArrayRef* outArray);

/// Appends an int to the array.
USD_INTEROP_API UsdResult UsdVtIntArray_Append(UsdVtArrayRef array, int32_t value);

// MARK: - VtArray<float>

/// Creates a float array from data.
USD_INTEROP_API UsdResult UsdVtFloatArray_CreateFromData(const float* data, size_t count, UsdVtArrayRef* outArray);

/// Appends a float to the array.
USD_INTEROP_API UsdResult UsdVtFloatArray_Append(UsdVtArrayRef array, float value);

// MARK: - VtArray<GfVec3f>

/// Creates a Vec3f array from data.
USD_INTEROP_API UsdResult UsdVtVec3fArray_CreateFromData(const UsdGfVec3f* data, size_t count, UsdVtArrayRef* outArray);

/// Appends a Vec3f to the array.
USD_INTEROP_API UsdResult UsdVtVec3fArray_Append(UsdVtArrayRef array, UsdGfVec3f value);

/// Gets an element from the array.
USD_INTEROP_API UsdGfVec3f UsdVtVec3fArray_GetAt(UsdVtArrayRef array, size_t index);

#ifdef __cplusplus
}
#endif

#endif // OPENUSDINTEROP_BASE_VT_H
