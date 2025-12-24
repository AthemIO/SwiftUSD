// usdVol.h - UsdVol module C wrapper
// Mirrors: pxr/usd/usdVol/*.h

#ifndef OPENUSDINTEROP_USDVOL_USDVOL_H
#define OPENUSDINTEROP_USDVOL_USDVOL_H

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

typedef struct UsdVolVolumeOpaque* UsdVolVolumeRef;
typedef struct UsdVolFieldBaseOpaque* UsdVolFieldBaseRef;
typedef struct UsdVolFieldAssetOpaque* UsdVolFieldAssetRef;
typedef struct UsdVolField3DAssetOpaque* UsdVolField3DAssetRef;
typedef struct UsdVolOpenVDBAssetOpaque* UsdVolOpenVDBAssetRef;

// ============================================================================
// MARK: - Enums
// ============================================================================

/// Field data type values
typedef enum {
    USDVOL_FIELD_DATA_TYPE_UNKNOWN = 0,
    USDVOL_FIELD_DATA_TYPE_HALF = 1,
    USDVOL_FIELD_DATA_TYPE_FLOAT = 2,
    USDVOL_FIELD_DATA_TYPE_DOUBLE = 3,
    USDVOL_FIELD_DATA_TYPE_INT = 4,
    USDVOL_FIELD_DATA_TYPE_UINT = 5,
    USDVOL_FIELD_DATA_TYPE_INT64 = 6,
    USDVOL_FIELD_DATA_TYPE_HALF2 = 7,
    USDVOL_FIELD_DATA_TYPE_FLOAT2 = 8,
    USDVOL_FIELD_DATA_TYPE_DOUBLE2 = 9,
    USDVOL_FIELD_DATA_TYPE_INT2 = 10,
    USDVOL_FIELD_DATA_TYPE_HALF3 = 11,
    USDVOL_FIELD_DATA_TYPE_FLOAT3 = 12,
    USDVOL_FIELD_DATA_TYPE_DOUBLE3 = 13,
    USDVOL_FIELD_DATA_TYPE_INT3 = 14,
    USDVOL_FIELD_DATA_TYPE_MATRIX3D = 15,
    USDVOL_FIELD_DATA_TYPE_MATRIX4D = 16,
    USDVOL_FIELD_DATA_TYPE_QUATD = 17,
    USDVOL_FIELD_DATA_TYPE_BOOL = 18,
    USDVOL_FIELD_DATA_TYPE_MASK = 19,
    USDVOL_FIELD_DATA_TYPE_STRING = 20
} UsdVolFieldDataType;

/// Vector data role hint values
typedef enum {
    USDVOL_VECTOR_DATA_ROLE_NONE = 0,
    USDVOL_VECTOR_DATA_ROLE_POINT = 1,
    USDVOL_VECTOR_DATA_ROLE_NORMAL = 2,
    USDVOL_VECTOR_DATA_ROLE_VECTOR = 3,
    USDVOL_VECTOR_DATA_ROLE_COLOR = 4
} UsdVolVectorDataRoleHint;

/// OpenVDB field class values
typedef enum {
    USDVOL_FIELD_CLASS_UNKNOWN = 0,
    USDVOL_FIELD_CLASS_LEVEL_SET = 1,
    USDVOL_FIELD_CLASS_FOG_VOLUME = 2,
    USDVOL_FIELD_CLASS_STAGGERED = 3
} UsdVolFieldClass;

// ============================================================================
// MARK: - UsdVolVolume (Volume Primitive)
// ============================================================================

/// Creates a UsdVolVolume schema on the given prim.
/// Caller must release with UsdVolVolume_Release.
USD_INTEROP_API UsdVolVolumeRef UsdVolVolume_Get(UsdStageRef stage, SdfPathRef path);

/// Defines a new Volume prim at the given path.
/// Caller must release with UsdVolVolume_Release.
USD_INTEROP_API UsdVolVolumeRef UsdVolVolume_Define(UsdStageRef stage, SdfPathRef path);

/// Creates from a prim.
/// Caller must release with UsdVolVolume_Release.
USD_INTEROP_API UsdVolVolumeRef UsdVolVolume_FromPrim(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdVolVolumeRef UsdVolVolume_Retain(UsdVolVolumeRef volume);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdVolVolume_Release(UsdVolVolumeRef volume);

/// Returns true if the volume is valid.
USD_INTEROP_API bool UsdVolVolume_IsValid(UsdVolVolumeRef volume);

/// Gets the underlying prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdVolVolume_GetPrim(UsdVolVolumeRef volume);

/// Gets the number of field relationships.
USD_INTEROP_API size_t UsdVolVolume_GetFieldPathCount(UsdVolVolumeRef volume);

/// Gets field paths in batch (count then fill pattern).
/// outNames and outPaths must have space for maxCount elements.
/// outNames are TfTokenRef that caller must release.
/// outPaths are SdfPathRef that caller must release.
/// Returns actual count written.
USD_INTEROP_API size_t UsdVolVolume_GetFieldPaths(
    UsdVolVolumeRef volume,
    TfTokenRef* outNames,
    SdfPathRef* outPaths,
    size_t maxCount
);

/// Checks if a field relationship exists with the given name.
USD_INTEROP_API bool UsdVolVolume_HasFieldRelationship(UsdVolVolumeRef volume, TfTokenRef name);

/// Gets the path for a specific field relationship.
/// Caller must release with SdfPath_Release.
USD_INTEROP_API SdfPathRef UsdVolVolume_GetFieldPath(UsdVolVolumeRef volume, TfTokenRef name);

/// Creates a field relationship on this volume.
USD_INTEROP_API UsdResult UsdVolVolume_CreateFieldRelationship(
    UsdVolVolumeRef volume,
    TfTokenRef name,
    SdfPathRef fieldPath
);

/// Blocks an existing field relationship.
USD_INTEROP_API UsdResult UsdVolVolume_BlockFieldRelationship(UsdVolVolumeRef volume, TfTokenRef name);

// ============================================================================
// MARK: - UsdVolFieldBase (Base class for field primitives)
// ============================================================================

/// Creates from a prim.
/// Caller must release with UsdVolFieldBase_Release.
USD_INTEROP_API UsdVolFieldBaseRef UsdVolFieldBase_FromPrim(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdVolFieldBaseRef UsdVolFieldBase_Retain(UsdVolFieldBaseRef field);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdVolFieldBase_Release(UsdVolFieldBaseRef field);

/// Returns true if the field is valid.
USD_INTEROP_API bool UsdVolFieldBase_IsValid(UsdVolFieldBaseRef field);

/// Gets the underlying prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdVolFieldBase_GetPrim(UsdVolFieldBaseRef field);

// ============================================================================
// MARK: - UsdVolFieldAsset (Base class for file-based fields)
// ============================================================================

/// Creates from a prim.
/// Caller must release with UsdVolFieldAsset_Release.
USD_INTEROP_API UsdVolFieldAssetRef UsdVolFieldAsset_FromPrim(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdVolFieldAssetRef UsdVolFieldAsset_Retain(UsdVolFieldAssetRef field);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdVolFieldAsset_Release(UsdVolFieldAssetRef field);

/// Returns true if the field is valid.
USD_INTEROP_API bool UsdVolFieldAsset_IsValid(UsdVolFieldAssetRef field);

/// Gets the underlying prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdVolFieldAsset_GetPrim(UsdVolFieldAssetRef field);

/// Gets the file path.
/// Caller must free with UsdVol_FreeString.
USD_INTEROP_API char* UsdVolFieldAsset_GetFilePath(UsdVolFieldAssetRef field, UsdTimeCode time);

/// Sets the file path.
USD_INTEROP_API UsdResult UsdVolFieldAsset_SetFilePath(
    UsdVolFieldAssetRef field,
    UsdTimeCode time,
    const char* filePath
);

/// Gets the field name within the file.
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef UsdVolFieldAsset_GetFieldName(UsdVolFieldAssetRef field, UsdTimeCode time);

/// Sets the field name within the file.
USD_INTEROP_API UsdResult UsdVolFieldAsset_SetFieldName(
    UsdVolFieldAssetRef field,
    UsdTimeCode time,
    TfTokenRef fieldName
);

/// Gets the field index for disambiguation.
USD_INTEROP_API int UsdVolFieldAsset_GetFieldIndex(UsdVolFieldAssetRef field, UsdTimeCode time);

/// Sets the field index for disambiguation.
USD_INTEROP_API UsdResult UsdVolFieldAsset_SetFieldIndex(
    UsdVolFieldAssetRef field,
    UsdTimeCode time,
    int fieldIndex
);

/// Gets the field data type.
USD_INTEROP_API UsdVolFieldDataType UsdVolFieldAsset_GetFieldDataType(UsdVolFieldAssetRef field);

/// Sets the field data type.
USD_INTEROP_API UsdResult UsdVolFieldAsset_SetFieldDataType(
    UsdVolFieldAssetRef field,
    UsdVolFieldDataType dataType
);

/// Gets the vector data role hint.
USD_INTEROP_API UsdVolVectorDataRoleHint UsdVolFieldAsset_GetVectorDataRoleHint(UsdVolFieldAssetRef field);

/// Sets the vector data role hint.
USD_INTEROP_API UsdResult UsdVolFieldAsset_SetVectorDataRoleHint(
    UsdVolFieldAssetRef field,
    UsdVolVectorDataRoleHint roleHint
);

// ============================================================================
// MARK: - UsdVolField3DAsset (Field3D format field)
// ============================================================================

/// Defines a new Field3DAsset prim at the given path.
/// Caller must release with UsdVolField3DAsset_Release.
USD_INTEROP_API UsdVolField3DAssetRef UsdVolField3DAsset_Define(UsdStageRef stage, SdfPathRef path);

/// Creates from a prim.
/// Caller must release with UsdVolField3DAsset_Release.
USD_INTEROP_API UsdVolField3DAssetRef UsdVolField3DAsset_FromPrim(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdVolField3DAssetRef UsdVolField3DAsset_Retain(UsdVolField3DAssetRef field);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdVolField3DAsset_Release(UsdVolField3DAssetRef field);

/// Returns true if the field is valid.
USD_INTEROP_API bool UsdVolField3DAsset_IsValid(UsdVolField3DAssetRef field);

/// Gets the underlying prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdVolField3DAsset_GetPrim(UsdVolField3DAssetRef field);

/// Gets the file path.
/// Caller must free with UsdVol_FreeString.
USD_INTEROP_API char* UsdVolField3DAsset_GetFilePath(UsdVolField3DAssetRef field, UsdTimeCode time);

/// Sets the file path.
USD_INTEROP_API UsdResult UsdVolField3DAsset_SetFilePath(
    UsdVolField3DAssetRef field,
    UsdTimeCode time,
    const char* filePath
);

/// Gets the field name within the file.
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef UsdVolField3DAsset_GetFieldName(UsdVolField3DAssetRef field, UsdTimeCode time);

/// Sets the field name within the file.
USD_INTEROP_API UsdResult UsdVolField3DAsset_SetFieldName(
    UsdVolField3DAssetRef field,
    UsdTimeCode time,
    TfTokenRef fieldName
);

/// Gets the field index for disambiguation.
USD_INTEROP_API int UsdVolField3DAsset_GetFieldIndex(UsdVolField3DAssetRef field, UsdTimeCode time);

/// Sets the field index for disambiguation.
USD_INTEROP_API UsdResult UsdVolField3DAsset_SetFieldIndex(
    UsdVolField3DAssetRef field,
    UsdTimeCode time,
    int fieldIndex
);

/// Gets the field data type.
USD_INTEROP_API UsdVolFieldDataType UsdVolField3DAsset_GetFieldDataType(UsdVolField3DAssetRef field);

/// Sets the field data type.
USD_INTEROP_API UsdResult UsdVolField3DAsset_SetFieldDataType(
    UsdVolField3DAssetRef field,
    UsdVolFieldDataType dataType
);

/// Gets the field purpose (Field3D-specific).
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef UsdVolField3DAsset_GetFieldPurpose(UsdVolField3DAssetRef field);

/// Sets the field purpose (Field3D-specific).
USD_INTEROP_API UsdResult UsdVolField3DAsset_SetFieldPurpose(
    UsdVolField3DAssetRef field,
    TfTokenRef purpose
);

// ============================================================================
// MARK: - UsdVolOpenVDBAsset (OpenVDB format field)
// ============================================================================

/// Defines a new OpenVDBAsset prim at the given path.
/// Caller must release with UsdVolOpenVDBAsset_Release.
USD_INTEROP_API UsdVolOpenVDBAssetRef UsdVolOpenVDBAsset_Define(UsdStageRef stage, SdfPathRef path);

/// Creates from a prim.
/// Caller must release with UsdVolOpenVDBAsset_Release.
USD_INTEROP_API UsdVolOpenVDBAssetRef UsdVolOpenVDBAsset_FromPrim(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdVolOpenVDBAssetRef UsdVolOpenVDBAsset_Retain(UsdVolOpenVDBAssetRef field);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdVolOpenVDBAsset_Release(UsdVolOpenVDBAssetRef field);

/// Returns true if the field is valid.
USD_INTEROP_API bool UsdVolOpenVDBAsset_IsValid(UsdVolOpenVDBAssetRef field);

/// Gets the underlying prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdVolOpenVDBAsset_GetPrim(UsdVolOpenVDBAssetRef field);

/// Gets the file path.
/// Caller must free with UsdVol_FreeString.
USD_INTEROP_API char* UsdVolOpenVDBAsset_GetFilePath(UsdVolOpenVDBAssetRef field, UsdTimeCode time);

/// Sets the file path.
USD_INTEROP_API UsdResult UsdVolOpenVDBAsset_SetFilePath(
    UsdVolOpenVDBAssetRef field,
    UsdTimeCode time,
    const char* filePath
);

/// Gets the field name within the file.
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef UsdVolOpenVDBAsset_GetFieldName(UsdVolOpenVDBAssetRef field, UsdTimeCode time);

/// Sets the field name within the file.
USD_INTEROP_API UsdResult UsdVolOpenVDBAsset_SetFieldName(
    UsdVolOpenVDBAssetRef field,
    UsdTimeCode time,
    TfTokenRef fieldName
);

/// Gets the field index for disambiguation.
USD_INTEROP_API int UsdVolOpenVDBAsset_GetFieldIndex(UsdVolOpenVDBAssetRef field, UsdTimeCode time);

/// Sets the field index for disambiguation.
USD_INTEROP_API UsdResult UsdVolOpenVDBAsset_SetFieldIndex(
    UsdVolOpenVDBAssetRef field,
    UsdTimeCode time,
    int fieldIndex
);

/// Gets the field data type.
USD_INTEROP_API UsdVolFieldDataType UsdVolOpenVDBAsset_GetFieldDataType(UsdVolOpenVDBAssetRef field);

/// Sets the field data type.
USD_INTEROP_API UsdResult UsdVolOpenVDBAsset_SetFieldDataType(
    UsdVolOpenVDBAssetRef field,
    UsdVolFieldDataType dataType
);

/// Gets the field class (OpenVDB-specific).
USD_INTEROP_API UsdVolFieldClass UsdVolOpenVDBAsset_GetFieldClass(UsdVolOpenVDBAssetRef field);

/// Sets the field class (OpenVDB-specific).
USD_INTEROP_API UsdResult UsdVolOpenVDBAsset_SetFieldClass(
    UsdVolOpenVDBAssetRef field,
    UsdVolFieldClass fieldClass
);

// ============================================================================
// MARK: - Utility Functions
// ============================================================================

/// Frees a string allocated by UsdVol functions.
USD_INTEROP_API void UsdVol_FreeString(char* str);

#ifdef __cplusplus
}
#endif

#endif // OPENUSDINTEROP_USDVOL_USDVOL_H
