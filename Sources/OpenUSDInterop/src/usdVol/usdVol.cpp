// usdVol.cpp - UsdVol module C wrapper implementation
// Mirrors: pxr/usd/usdVol/*.h

#include "../../include/usdVol/usdVol.h"

// Check if we should use full USD library
#if defined(USE_PIXAR_USD) && USE_PIXAR_USD
    #define USD_USE_FULL 1
    #include <pxr/usd/usdVol/volume.h>
    #include <pxr/usd/usdVol/fieldBase.h>
    #include <pxr/usd/usdVol/fieldAsset.h>
    #include <pxr/usd/usdVol/field3DAsset.h>
    #include <pxr/usd/usdVol/openVDBAsset.h>
    #include <pxr/usd/usdVol/tokens.h>
    #include <pxr/usd/sdf/assetPath.h>
    PXR_NAMESPACE_USING_DIRECTIVE
#else
    #define USD_USE_FULL 0
#endif

#include <atomic>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>

// ============================================================================
// MARK: - Wrapper Structures
// ============================================================================

struct UsdVolVolumeOpaque {
#if USD_USE_FULL
    UsdVolVolume volume;
#endif
    std::atomic<int> refCount;
    bool valid;
    std::map<std::string, std::string> fieldPaths; // For stub mode

    UsdVolVolumeOpaque() : refCount(1), valid(false) {}

#if USD_USE_FULL
    explicit UsdVolVolumeOpaque(const UsdVolVolume& v)
        : volume(v), refCount(1), valid(true) {}
#endif
};

struct UsdVolFieldBaseOpaque {
#if USD_USE_FULL
    UsdVolFieldBase field;
#endif
    std::atomic<int> refCount;
    bool valid;

    UsdVolFieldBaseOpaque() : refCount(1), valid(false) {}

#if USD_USE_FULL
    explicit UsdVolFieldBaseOpaque(const UsdVolFieldBase& f)
        : field(f), refCount(1), valid(true) {}
#endif
};

struct UsdVolFieldAssetOpaque {
#if USD_USE_FULL
    UsdVolFieldAsset field;
#endif
    std::atomic<int> refCount;
    bool valid;
    std::string filePath;
    std::string fieldName;
    int fieldIndex;
    int fieldDataType;
    int vectorDataRoleHint;

    UsdVolFieldAssetOpaque() : refCount(1), valid(false), fieldIndex(0),
        fieldDataType(0), vectorDataRoleHint(0) {}

#if USD_USE_FULL
    explicit UsdVolFieldAssetOpaque(const UsdVolFieldAsset& f)
        : field(f), refCount(1), valid(true), fieldIndex(0),
          fieldDataType(0), vectorDataRoleHint(0) {}
#endif
};

struct UsdVolField3DAssetOpaque {
#if USD_USE_FULL
    UsdVolField3DAsset field;
#endif
    std::atomic<int> refCount;
    bool valid;
    std::string filePath;
    std::string fieldName;
    std::string fieldPurpose;
    int fieldIndex;
    int fieldDataType;

    UsdVolField3DAssetOpaque() : refCount(1), valid(false), fieldIndex(0),
        fieldDataType(0) {}

#if USD_USE_FULL
    explicit UsdVolField3DAssetOpaque(const UsdVolField3DAsset& f)
        : field(f), refCount(1), valid(true), fieldIndex(0), fieldDataType(0) {}
#endif
};

struct UsdVolOpenVDBAssetOpaque {
#if USD_USE_FULL
    UsdVolOpenVDBAsset field;
#endif
    std::atomic<int> refCount;
    bool valid;
    std::string filePath;
    std::string fieldName;
    int fieldIndex;
    int fieldDataType;
    int fieldClass;

    UsdVolOpenVDBAssetOpaque() : refCount(1), valid(false), fieldIndex(0),
        fieldDataType(0), fieldClass(0) {}

#if USD_USE_FULL
    explicit UsdVolOpenVDBAssetOpaque(const UsdVolOpenVDBAsset& f)
        : field(f), refCount(1), valid(true), fieldIndex(0),
          fieldDataType(0), fieldClass(0) {}
#endif
};

// ============================================================================
// MARK: - Helper Macros
// ============================================================================

#define CATCH_AND_RETURN(defaultValue) \
    catch (...) { return defaultValue; }

#define CATCH_AND_RETURN_RESULT \
    catch (...) { return USD_RESULT_ERROR; }

// ============================================================================
// MARK: - Helper Functions
// ============================================================================

#if USD_USE_FULL
static UsdVolFieldDataType TokenToFieldDataType(const TfToken& token) {
    if (token == UsdVolTokens->half) return USDVOL_FIELD_DATA_TYPE_HALF;
    if (token == UsdVolTokens->float_) return USDVOL_FIELD_DATA_TYPE_FLOAT;
    if (token == UsdVolTokens->double_) return USDVOL_FIELD_DATA_TYPE_DOUBLE;
    if (token == UsdVolTokens->int_) return USDVOL_FIELD_DATA_TYPE_INT;
    if (token == UsdVolTokens->uint) return USDVOL_FIELD_DATA_TYPE_UINT;
    if (token == UsdVolTokens->int64) return USDVOL_FIELD_DATA_TYPE_INT64;
    if (token == UsdVolTokens->half2) return USDVOL_FIELD_DATA_TYPE_HALF2;
    if (token == UsdVolTokens->float2) return USDVOL_FIELD_DATA_TYPE_FLOAT2;
    if (token == UsdVolTokens->double2) return USDVOL_FIELD_DATA_TYPE_DOUBLE2;
    if (token == UsdVolTokens->int2) return USDVOL_FIELD_DATA_TYPE_INT2;
    if (token == UsdVolTokens->half3) return USDVOL_FIELD_DATA_TYPE_HALF3;
    if (token == UsdVolTokens->float3) return USDVOL_FIELD_DATA_TYPE_FLOAT3;
    if (token == UsdVolTokens->double3) return USDVOL_FIELD_DATA_TYPE_DOUBLE3;
    if (token == UsdVolTokens->int3) return USDVOL_FIELD_DATA_TYPE_INT3;
    if (token == UsdVolTokens->matrix3d) return USDVOL_FIELD_DATA_TYPE_MATRIX3D;
    if (token == UsdVolTokens->matrix4d) return USDVOL_FIELD_DATA_TYPE_MATRIX4D;
    if (token == UsdVolTokens->quatd) return USDVOL_FIELD_DATA_TYPE_QUATD;
    if (token == UsdVolTokens->bool_) return USDVOL_FIELD_DATA_TYPE_BOOL;
    if (token == UsdVolTokens->mask) return USDVOL_FIELD_DATA_TYPE_MASK;
    if (token == UsdVolTokens->string) return USDVOL_FIELD_DATA_TYPE_STRING;
    return USDVOL_FIELD_DATA_TYPE_UNKNOWN;
}

static TfToken FieldDataTypeToToken(UsdVolFieldDataType type) {
    switch (type) {
        case USDVOL_FIELD_DATA_TYPE_HALF: return UsdVolTokens->half;
        case USDVOL_FIELD_DATA_TYPE_FLOAT: return UsdVolTokens->float_;
        case USDVOL_FIELD_DATA_TYPE_DOUBLE: return UsdVolTokens->double_;
        case USDVOL_FIELD_DATA_TYPE_INT: return UsdVolTokens->int_;
        case USDVOL_FIELD_DATA_TYPE_UINT: return UsdVolTokens->uint;
        case USDVOL_FIELD_DATA_TYPE_INT64: return UsdVolTokens->int64;
        case USDVOL_FIELD_DATA_TYPE_HALF2: return UsdVolTokens->half2;
        case USDVOL_FIELD_DATA_TYPE_FLOAT2: return UsdVolTokens->float2;
        case USDVOL_FIELD_DATA_TYPE_DOUBLE2: return UsdVolTokens->double2;
        case USDVOL_FIELD_DATA_TYPE_INT2: return UsdVolTokens->int2;
        case USDVOL_FIELD_DATA_TYPE_HALF3: return UsdVolTokens->half3;
        case USDVOL_FIELD_DATA_TYPE_FLOAT3: return UsdVolTokens->float3;
        case USDVOL_FIELD_DATA_TYPE_DOUBLE3: return UsdVolTokens->double3;
        case USDVOL_FIELD_DATA_TYPE_INT3: return UsdVolTokens->int3;
        case USDVOL_FIELD_DATA_TYPE_MATRIX3D: return UsdVolTokens->matrix3d;
        case USDVOL_FIELD_DATA_TYPE_MATRIX4D: return UsdVolTokens->matrix4d;
        case USDVOL_FIELD_DATA_TYPE_QUATD: return UsdVolTokens->quatd;
        case USDVOL_FIELD_DATA_TYPE_BOOL: return UsdVolTokens->bool_;
        case USDVOL_FIELD_DATA_TYPE_MASK: return UsdVolTokens->mask;
        case USDVOL_FIELD_DATA_TYPE_STRING: return UsdVolTokens->string;
        default: return TfToken();
    }
}

static UsdVolVectorDataRoleHint TokenToVectorDataRoleHint(const TfToken& token) {
    if (token == UsdVolTokens->None_) return USDVOL_VECTOR_DATA_ROLE_NONE;
    if (token == UsdVolTokens->Point) return USDVOL_VECTOR_DATA_ROLE_POINT;
    if (token == UsdVolTokens->Normal) return USDVOL_VECTOR_DATA_ROLE_NORMAL;
    if (token == UsdVolTokens->Vector) return USDVOL_VECTOR_DATA_ROLE_VECTOR;
    if (token == UsdVolTokens->Color) return USDVOL_VECTOR_DATA_ROLE_COLOR;
    return USDVOL_VECTOR_DATA_ROLE_NONE;
}

static TfToken VectorDataRoleHintToToken(UsdVolVectorDataRoleHint hint) {
    switch (hint) {
        case USDVOL_VECTOR_DATA_ROLE_NONE: return UsdVolTokens->None_;
        case USDVOL_VECTOR_DATA_ROLE_POINT: return UsdVolTokens->Point;
        case USDVOL_VECTOR_DATA_ROLE_NORMAL: return UsdVolTokens->Normal;
        case USDVOL_VECTOR_DATA_ROLE_VECTOR: return UsdVolTokens->Vector;
        case USDVOL_VECTOR_DATA_ROLE_COLOR: return UsdVolTokens->Color;
        default: return UsdVolTokens->None_;
    }
}

static UsdVolFieldClass TokenToFieldClass(const TfToken& token) {
    if (token == UsdVolTokens->levelSet) return USDVOL_FIELD_CLASS_LEVEL_SET;
    if (token == UsdVolTokens->fogVolume) return USDVOL_FIELD_CLASS_FOG_VOLUME;
    if (token == UsdVolTokens->staggered) return USDVOL_FIELD_CLASS_STAGGERED;
    if (token == UsdVolTokens->unknown) return USDVOL_FIELD_CLASS_UNKNOWN;
    return USDVOL_FIELD_CLASS_UNKNOWN;
}

static TfToken FieldClassToToken(UsdVolFieldClass fieldClass) {
    switch (fieldClass) {
        case USDVOL_FIELD_CLASS_LEVEL_SET: return UsdVolTokens->levelSet;
        case USDVOL_FIELD_CLASS_FOG_VOLUME: return UsdVolTokens->fogVolume;
        case USDVOL_FIELD_CLASS_STAGGERED: return UsdVolTokens->staggered;
        case USDVOL_FIELD_CLASS_UNKNOWN: return UsdVolTokens->unknown;
        default: return UsdVolTokens->unknown;
    }
}
#endif

// ============================================================================
// MARK: - UsdVolVolume Implementation
// ============================================================================

extern "C" {

UsdVolVolumeRef UsdVolVolume_Get(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        UsdVolVolume volume = UsdVolVolume::Get(stage->stage, path->path);
        if (volume) {
            return new UsdVolVolumeOpaque(volume);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdVolVolumeRef UsdVolVolume_Define(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        UsdVolVolume volume = UsdVolVolume::Define(stage->stage, path->path);
        if (volume) {
            return new UsdVolVolumeOpaque(volume);
        }
        return nullptr;
#else
        auto* wrapper = new UsdVolVolumeOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdVolVolumeRef UsdVolVolume_FromPrim(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdVolVolume volume(prim->prim);
        if (volume) {
            return new UsdVolVolumeOpaque(volume);
        }
        return nullptr;
#else
        auto* wrapper = new UsdVolVolumeOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdVolVolumeRef UsdVolVolume_Retain(UsdVolVolumeRef volume) {
    if (!volume) return nullptr;
    volume->refCount.fetch_add(1, std::memory_order_relaxed);
    return volume;
}

void UsdVolVolume_Release(UsdVolVolumeRef volume) {
    if (!volume) return;
    if (volume->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete volume;
    }
}

bool UsdVolVolume_IsValid(UsdVolVolumeRef volume) {
    if (!volume) return false;
#if USD_USE_FULL
    return volume->volume;
#else
    return volume->valid;
#endif
}

UsdPrimRef UsdVolVolume_GetPrim(UsdVolVolumeRef volume) {
    if (!volume) return nullptr;
#if USD_USE_FULL
    try {
        UsdPrim prim = volume->volume.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

size_t UsdVolVolume_GetFieldPathCount(UsdVolVolumeRef volume) {
    if (!volume) return 0;
#if USD_USE_FULL
    try {
        UsdVolVolume::FieldMap fieldMap = volume->volume.GetFieldPaths();
        return fieldMap.size();
    } CATCH_AND_RETURN(0)
#else
    return volume->fieldPaths.size();
#endif
}

size_t UsdVolVolume_GetFieldPaths(
    UsdVolVolumeRef volume,
    TfTokenRef* outNames,
    SdfPathRef* outPaths,
    size_t maxCount
) {
    if (!volume || !outNames || !outPaths) return 0;
#if USD_USE_FULL
    try {
        UsdVolVolume::FieldMap fieldMap = volume->volume.GetFieldPaths();
        size_t count = std::min(fieldMap.size(), maxCount);
        size_t i = 0;
        for (const auto& pair : fieldMap) {
            if (i >= count) break;
            outNames[i] = new TfTokenOpaque(pair.first);
            outPaths[i] = new SdfPathOpaque(pair.second);
            ++i;
        }
        return i;
    } CATCH_AND_RETURN(0)
#else
    // In stub mode, return the count but don't populate arrays
    // since we can't create TfTokenOpaque/SdfPathOpaque here
    return 0;
#endif
}

bool UsdVolVolume_HasFieldRelationship(UsdVolVolumeRef volume, TfTokenRef name) {
    if (!volume || !name) return false;
#if USD_USE_FULL
    try {
        return volume->volume.HasFieldRelationship(name->token);
    } CATCH_AND_RETURN(false)
#else
    // In stub mode, always return false since we can't access TfTokenOpaque internals
    return false;
#endif
}

SdfPathRef UsdVolVolume_GetFieldPath(UsdVolVolumeRef volume, TfTokenRef name) {
    if (!volume || !name) return nullptr;
#if USD_USE_FULL
    try {
        SdfPath path = volume->volume.GetFieldPath(name->token);
        if (!path.IsEmpty()) {
            return new SdfPathOpaque(path);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    // In stub mode, return nullptr since we can't access TfTokenOpaque internals
    return nullptr;
#endif
}

UsdResult UsdVolVolume_CreateFieldRelationship(
    UsdVolVolumeRef volume,
    TfTokenRef name,
    SdfPathRef fieldPath
) {
    if (!volume || !name || !fieldPath) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        if (volume->volume.CreateFieldRelationship(name->token, fieldPath->path)) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
    } CATCH_AND_RETURN_RESULT
#else
    // In stub mode, just return success (no-op)
    return USD_RESULT_SUCCESS;
#endif
}

UsdResult UsdVolVolume_BlockFieldRelationship(UsdVolVolumeRef volume, TfTokenRef name) {
    if (!volume || !name) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        if (volume->volume.BlockFieldRelationship(name->token)) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
    } CATCH_AND_RETURN_RESULT
#else
    // In stub mode, just return success (no-op)
    return USD_RESULT_SUCCESS;
#endif
}

// ============================================================================
// MARK: - UsdVolFieldBase Implementation
// ============================================================================

UsdVolFieldBaseRef UsdVolFieldBase_FromPrim(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdVolFieldBase field(prim->prim);
        if (field) {
            return new UsdVolFieldBaseOpaque(field);
        }
        return nullptr;
#else
        auto* wrapper = new UsdVolFieldBaseOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdVolFieldBaseRef UsdVolFieldBase_Retain(UsdVolFieldBaseRef field) {
    if (!field) return nullptr;
    field->refCount.fetch_add(1, std::memory_order_relaxed);
    return field;
}

void UsdVolFieldBase_Release(UsdVolFieldBaseRef field) {
    if (!field) return;
    if (field->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete field;
    }
}

bool UsdVolFieldBase_IsValid(UsdVolFieldBaseRef field) {
    if (!field) return false;
#if USD_USE_FULL
    return field->field;
#else
    return field->valid;
#endif
}

UsdPrimRef UsdVolFieldBase_GetPrim(UsdVolFieldBaseRef field) {
    if (!field) return nullptr;
#if USD_USE_FULL
    try {
        UsdPrim prim = field->field.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

// ============================================================================
// MARK: - UsdVolFieldAsset Implementation
// ============================================================================

UsdVolFieldAssetRef UsdVolFieldAsset_FromPrim(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdVolFieldAsset field(prim->prim);
        if (field) {
            return new UsdVolFieldAssetOpaque(field);
        }
        return nullptr;
#else
        auto* wrapper = new UsdVolFieldAssetOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdVolFieldAssetRef UsdVolFieldAsset_Retain(UsdVolFieldAssetRef field) {
    if (!field) return nullptr;
    field->refCount.fetch_add(1, std::memory_order_relaxed);
    return field;
}

void UsdVolFieldAsset_Release(UsdVolFieldAssetRef field) {
    if (!field) return;
    if (field->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete field;
    }
}

bool UsdVolFieldAsset_IsValid(UsdVolFieldAssetRef field) {
    if (!field) return false;
#if USD_USE_FULL
    return field->field;
#else
    return field->valid;
#endif
}

UsdPrimRef UsdVolFieldAsset_GetPrim(UsdVolFieldAssetRef field) {
    if (!field) return nullptr;
#if USD_USE_FULL
    try {
        UsdPrim prim = field->field.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

char* UsdVolFieldAsset_GetFilePath(UsdVolFieldAssetRef field, UsdTimeCode time) {
    if (!field) return nullptr;
#if USD_USE_FULL
    try {
        SdfAssetPath assetPath;
        if (field->field.GetFilePathAttr().Get(&assetPath,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time))) {
            std::string path = assetPath.GetAssetPath();
            char* result = static_cast<char*>(malloc(path.length() + 1));
            if (result) {
                strcpy(result, path.c_str());
            }
            return result;
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    char* result = static_cast<char*>(malloc(field->filePath.length() + 1));
    if (result) {
        strcpy(result, field->filePath.c_str());
    }
    return result;
#endif
}

UsdResult UsdVolFieldAsset_SetFilePath(
    UsdVolFieldAssetRef field,
    UsdTimeCode time,
    const char* filePath
) {
    if (!field || !filePath) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        SdfAssetPath assetPath(filePath);
        if (field->field.GetFilePathAttr().Set(assetPath,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time))) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
    } CATCH_AND_RETURN_RESULT
#else
    field->filePath = filePath;
    return USD_RESULT_SUCCESS;
#endif
}

TfTokenRef UsdVolFieldAsset_GetFieldName(UsdVolFieldAssetRef field, UsdTimeCode time) {
    if (!field) return nullptr;
#if USD_USE_FULL
    try {
        TfToken fieldName;
        if (field->field.GetFieldNameAttr().Get(&fieldName,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time))) {
            return new TfTokenOpaque(fieldName);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    // In stub mode, return nullptr since we can't create TfTokenOpaque
    return nullptr;
#endif
}

UsdResult UsdVolFieldAsset_SetFieldName(
    UsdVolFieldAssetRef field,
    UsdTimeCode time,
    TfTokenRef fieldName
) {
    if (!field || !fieldName) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        if (field->field.GetFieldNameAttr().Set(fieldName->token,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time))) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
    } CATCH_AND_RETURN_RESULT
#else
    // In stub mode, just return success (no-op)
    return USD_RESULT_SUCCESS;
#endif
}

int UsdVolFieldAsset_GetFieldIndex(UsdVolFieldAssetRef field, UsdTimeCode time) {
    if (!field) return 0;
#if USD_USE_FULL
    try {
        int fieldIndex = 0;
        field->field.GetFieldIndexAttr().Get(&fieldIndex,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return fieldIndex;
    } CATCH_AND_RETURN(0)
#else
    return field->fieldIndex;
#endif
}

UsdResult UsdVolFieldAsset_SetFieldIndex(
    UsdVolFieldAssetRef field,
    UsdTimeCode time,
    int fieldIndex
) {
    if (!field) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        if (field->field.GetFieldIndexAttr().Set(fieldIndex,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time))) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
    } CATCH_AND_RETURN_RESULT
#else
    field->fieldIndex = fieldIndex;
    return USD_RESULT_SUCCESS;
#endif
}

UsdVolFieldDataType UsdVolFieldAsset_GetFieldDataType(UsdVolFieldAssetRef field) {
    if (!field) return USDVOL_FIELD_DATA_TYPE_UNKNOWN;
#if USD_USE_FULL
    try {
        TfToken dataType;
        if (field->field.GetFieldDataTypeAttr().Get(&dataType)) {
            return TokenToFieldDataType(dataType);
        }
        return USDVOL_FIELD_DATA_TYPE_UNKNOWN;
    } CATCH_AND_RETURN(USDVOL_FIELD_DATA_TYPE_UNKNOWN)
#else
    return static_cast<UsdVolFieldDataType>(field->fieldDataType);
#endif
}

UsdResult UsdVolFieldAsset_SetFieldDataType(
    UsdVolFieldAssetRef field,
    UsdVolFieldDataType dataType
) {
    if (!field) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        TfToken token = FieldDataTypeToToken(dataType);
        if (!token.IsEmpty() && field->field.GetFieldDataTypeAttr().Set(token)) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
    } CATCH_AND_RETURN_RESULT
#else
    field->fieldDataType = static_cast<int>(dataType);
    return USD_RESULT_SUCCESS;
#endif
}

UsdVolVectorDataRoleHint UsdVolFieldAsset_GetVectorDataRoleHint(UsdVolFieldAssetRef field) {
    if (!field) return USDVOL_VECTOR_DATA_ROLE_NONE;
#if USD_USE_FULL
    try {
        TfToken roleHint;
        if (field->field.GetVectorDataRoleHintAttr().Get(&roleHint)) {
            return TokenToVectorDataRoleHint(roleHint);
        }
        return USDVOL_VECTOR_DATA_ROLE_NONE;
    } CATCH_AND_RETURN(USDVOL_VECTOR_DATA_ROLE_NONE)
#else
    return static_cast<UsdVolVectorDataRoleHint>(field->vectorDataRoleHint);
#endif
}

UsdResult UsdVolFieldAsset_SetVectorDataRoleHint(
    UsdVolFieldAssetRef field,
    UsdVolVectorDataRoleHint roleHint
) {
    if (!field) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        TfToken token = VectorDataRoleHintToToken(roleHint);
        if (field->field.GetVectorDataRoleHintAttr().Set(token)) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
    } CATCH_AND_RETURN_RESULT
#else
    field->vectorDataRoleHint = static_cast<int>(roleHint);
    return USD_RESULT_SUCCESS;
#endif
}

// ============================================================================
// MARK: - UsdVolField3DAsset Implementation
// ============================================================================

UsdVolField3DAssetRef UsdVolField3DAsset_Define(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        UsdVolField3DAsset field = UsdVolField3DAsset::Define(stage->stage, path->path);
        if (field) {
            return new UsdVolField3DAssetOpaque(field);
        }
        return nullptr;
#else
        auto* wrapper = new UsdVolField3DAssetOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdVolField3DAssetRef UsdVolField3DAsset_FromPrim(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdVolField3DAsset field(prim->prim);
        if (field) {
            return new UsdVolField3DAssetOpaque(field);
        }
        return nullptr;
#else
        auto* wrapper = new UsdVolField3DAssetOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdVolField3DAssetRef UsdVolField3DAsset_Retain(UsdVolField3DAssetRef field) {
    if (!field) return nullptr;
    field->refCount.fetch_add(1, std::memory_order_relaxed);
    return field;
}

void UsdVolField3DAsset_Release(UsdVolField3DAssetRef field) {
    if (!field) return;
    if (field->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete field;
    }
}

bool UsdVolField3DAsset_IsValid(UsdVolField3DAssetRef field) {
    if (!field) return false;
#if USD_USE_FULL
    return field->field;
#else
    return field->valid;
#endif
}

UsdPrimRef UsdVolField3DAsset_GetPrim(UsdVolField3DAssetRef field) {
    if (!field) return nullptr;
#if USD_USE_FULL
    try {
        UsdPrim prim = field->field.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

char* UsdVolField3DAsset_GetFilePath(UsdVolField3DAssetRef field, UsdTimeCode time) {
    if (!field) return nullptr;
#if USD_USE_FULL
    try {
        SdfAssetPath assetPath;
        if (field->field.GetFilePathAttr().Get(&assetPath,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time))) {
            std::string path = assetPath.GetAssetPath();
            char* result = static_cast<char*>(malloc(path.length() + 1));
            if (result) {
                strcpy(result, path.c_str());
            }
            return result;
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    char* result = static_cast<char*>(malloc(field->filePath.length() + 1));
    if (result) {
        strcpy(result, field->filePath.c_str());
    }
    return result;
#endif
}

UsdResult UsdVolField3DAsset_SetFilePath(
    UsdVolField3DAssetRef field,
    UsdTimeCode time,
    const char* filePath
) {
    if (!field || !filePath) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        SdfAssetPath assetPath(filePath);
        if (field->field.GetFilePathAttr().Set(assetPath,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time))) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
    } CATCH_AND_RETURN_RESULT
#else
    field->filePath = filePath;
    return USD_RESULT_SUCCESS;
#endif
}

TfTokenRef UsdVolField3DAsset_GetFieldName(UsdVolField3DAssetRef field, UsdTimeCode time) {
    if (!field) return nullptr;
#if USD_USE_FULL
    try {
        TfToken fieldName;
        if (field->field.GetFieldNameAttr().Get(&fieldName,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time))) {
            return new TfTokenOpaque(fieldName);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    // In stub mode, return nullptr since we can't create TfTokenOpaque
    return nullptr;
#endif
}

UsdResult UsdVolField3DAsset_SetFieldName(
    UsdVolField3DAssetRef field,
    UsdTimeCode time,
    TfTokenRef fieldName
) {
    if (!field || !fieldName) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        if (field->field.GetFieldNameAttr().Set(fieldName->token,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time))) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
    } CATCH_AND_RETURN_RESULT
#else
    // In stub mode, just return success (no-op)
    return USD_RESULT_SUCCESS;
#endif
}

int UsdVolField3DAsset_GetFieldIndex(UsdVolField3DAssetRef field, UsdTimeCode time) {
    if (!field) return 0;
#if USD_USE_FULL
    try {
        int fieldIndex = 0;
        field->field.GetFieldIndexAttr().Get(&fieldIndex,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return fieldIndex;
    } CATCH_AND_RETURN(0)
#else
    return field->fieldIndex;
#endif
}

UsdResult UsdVolField3DAsset_SetFieldIndex(
    UsdVolField3DAssetRef field,
    UsdTimeCode time,
    int fieldIndex
) {
    if (!field) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        if (field->field.GetFieldIndexAttr().Set(fieldIndex,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time))) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
    } CATCH_AND_RETURN_RESULT
#else
    field->fieldIndex = fieldIndex;
    return USD_RESULT_SUCCESS;
#endif
}

UsdVolFieldDataType UsdVolField3DAsset_GetFieldDataType(UsdVolField3DAssetRef field) {
    if (!field) return USDVOL_FIELD_DATA_TYPE_UNKNOWN;
#if USD_USE_FULL
    try {
        TfToken dataType;
        if (field->field.GetFieldDataTypeAttr().Get(&dataType)) {
            return TokenToFieldDataType(dataType);
        }
        return USDVOL_FIELD_DATA_TYPE_UNKNOWN;
    } CATCH_AND_RETURN(USDVOL_FIELD_DATA_TYPE_UNKNOWN)
#else
    return static_cast<UsdVolFieldDataType>(field->fieldDataType);
#endif
}

UsdResult UsdVolField3DAsset_SetFieldDataType(
    UsdVolField3DAssetRef field,
    UsdVolFieldDataType dataType
) {
    if (!field) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        TfToken token = FieldDataTypeToToken(dataType);
        if (!token.IsEmpty() && field->field.GetFieldDataTypeAttr().Set(token)) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
    } CATCH_AND_RETURN_RESULT
#else
    field->fieldDataType = static_cast<int>(dataType);
    return USD_RESULT_SUCCESS;
#endif
}

TfTokenRef UsdVolField3DAsset_GetFieldPurpose(UsdVolField3DAssetRef field) {
    if (!field) return nullptr;
#if USD_USE_FULL
    try {
        TfToken purpose;
        if (field->field.GetFieldPurposeAttr().Get(&purpose)) {
            return new TfTokenOpaque(purpose);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    // In stub mode, return nullptr since we can't create TfTokenOpaque
    return nullptr;
#endif
}

UsdResult UsdVolField3DAsset_SetFieldPurpose(
    UsdVolField3DAssetRef field,
    TfTokenRef purpose
) {
    if (!field || !purpose) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        if (field->field.GetFieldPurposeAttr().Set(purpose->token)) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
    } CATCH_AND_RETURN_RESULT
#else
    // In stub mode, just return success (no-op)
    return USD_RESULT_SUCCESS;
#endif
}

// ============================================================================
// MARK: - UsdVolOpenVDBAsset Implementation
// ============================================================================

UsdVolOpenVDBAssetRef UsdVolOpenVDBAsset_Define(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        UsdVolOpenVDBAsset field = UsdVolOpenVDBAsset::Define(stage->stage, path->path);
        if (field) {
            return new UsdVolOpenVDBAssetOpaque(field);
        }
        return nullptr;
#else
        auto* wrapper = new UsdVolOpenVDBAssetOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdVolOpenVDBAssetRef UsdVolOpenVDBAsset_FromPrim(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdVolOpenVDBAsset field(prim->prim);
        if (field) {
            return new UsdVolOpenVDBAssetOpaque(field);
        }
        return nullptr;
#else
        auto* wrapper = new UsdVolOpenVDBAssetOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdVolOpenVDBAssetRef UsdVolOpenVDBAsset_Retain(UsdVolOpenVDBAssetRef field) {
    if (!field) return nullptr;
    field->refCount.fetch_add(1, std::memory_order_relaxed);
    return field;
}

void UsdVolOpenVDBAsset_Release(UsdVolOpenVDBAssetRef field) {
    if (!field) return;
    if (field->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete field;
    }
}

bool UsdVolOpenVDBAsset_IsValid(UsdVolOpenVDBAssetRef field) {
    if (!field) return false;
#if USD_USE_FULL
    return field->field;
#else
    return field->valid;
#endif
}

UsdPrimRef UsdVolOpenVDBAsset_GetPrim(UsdVolOpenVDBAssetRef field) {
    if (!field) return nullptr;
#if USD_USE_FULL
    try {
        UsdPrim prim = field->field.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

char* UsdVolOpenVDBAsset_GetFilePath(UsdVolOpenVDBAssetRef field, UsdTimeCode time) {
    if (!field) return nullptr;
#if USD_USE_FULL
    try {
        SdfAssetPath assetPath;
        if (field->field.GetFilePathAttr().Get(&assetPath,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time))) {
            std::string path = assetPath.GetAssetPath();
            char* result = static_cast<char*>(malloc(path.length() + 1));
            if (result) {
                strcpy(result, path.c_str());
            }
            return result;
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    char* result = static_cast<char*>(malloc(field->filePath.length() + 1));
    if (result) {
        strcpy(result, field->filePath.c_str());
    }
    return result;
#endif
}

UsdResult UsdVolOpenVDBAsset_SetFilePath(
    UsdVolOpenVDBAssetRef field,
    UsdTimeCode time,
    const char* filePath
) {
    if (!field || !filePath) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        SdfAssetPath assetPath(filePath);
        if (field->field.GetFilePathAttr().Set(assetPath,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time))) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
    } CATCH_AND_RETURN_RESULT
#else
    field->filePath = filePath;
    return USD_RESULT_SUCCESS;
#endif
}

TfTokenRef UsdVolOpenVDBAsset_GetFieldName(UsdVolOpenVDBAssetRef field, UsdTimeCode time) {
    if (!field) return nullptr;
#if USD_USE_FULL
    try {
        TfToken fieldName;
        if (field->field.GetFieldNameAttr().Get(&fieldName,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time))) {
            return new TfTokenOpaque(fieldName);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    // In stub mode, return nullptr since we can't create TfTokenOpaque
    return nullptr;
#endif
}

UsdResult UsdVolOpenVDBAsset_SetFieldName(
    UsdVolOpenVDBAssetRef field,
    UsdTimeCode time,
    TfTokenRef fieldName
) {
    if (!field || !fieldName) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        if (field->field.GetFieldNameAttr().Set(fieldName->token,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time))) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
    } CATCH_AND_RETURN_RESULT
#else
    // In stub mode, just return success (no-op)
    return USD_RESULT_SUCCESS;
#endif
}

int UsdVolOpenVDBAsset_GetFieldIndex(UsdVolOpenVDBAssetRef field, UsdTimeCode time) {
    if (!field) return 0;
#if USD_USE_FULL
    try {
        int fieldIndex = 0;
        field->field.GetFieldIndexAttr().Get(&fieldIndex,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return fieldIndex;
    } CATCH_AND_RETURN(0)
#else
    return field->fieldIndex;
#endif
}

UsdResult UsdVolOpenVDBAsset_SetFieldIndex(
    UsdVolOpenVDBAssetRef field,
    UsdTimeCode time,
    int fieldIndex
) {
    if (!field) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        if (field->field.GetFieldIndexAttr().Set(fieldIndex,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time))) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
    } CATCH_AND_RETURN_RESULT
#else
    field->fieldIndex = fieldIndex;
    return USD_RESULT_SUCCESS;
#endif
}

UsdVolFieldDataType UsdVolOpenVDBAsset_GetFieldDataType(UsdVolOpenVDBAssetRef field) {
    if (!field) return USDVOL_FIELD_DATA_TYPE_UNKNOWN;
#if USD_USE_FULL
    try {
        TfToken dataType;
        if (field->field.GetFieldDataTypeAttr().Get(&dataType)) {
            return TokenToFieldDataType(dataType);
        }
        return USDVOL_FIELD_DATA_TYPE_UNKNOWN;
    } CATCH_AND_RETURN(USDVOL_FIELD_DATA_TYPE_UNKNOWN)
#else
    return static_cast<UsdVolFieldDataType>(field->fieldDataType);
#endif
}

UsdResult UsdVolOpenVDBAsset_SetFieldDataType(
    UsdVolOpenVDBAssetRef field,
    UsdVolFieldDataType dataType
) {
    if (!field) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        TfToken token = FieldDataTypeToToken(dataType);
        if (!token.IsEmpty() && field->field.GetFieldDataTypeAttr().Set(token)) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
    } CATCH_AND_RETURN_RESULT
#else
    field->fieldDataType = static_cast<int>(dataType);
    return USD_RESULT_SUCCESS;
#endif
}

UsdVolFieldClass UsdVolOpenVDBAsset_GetFieldClass(UsdVolOpenVDBAssetRef field) {
    if (!field) return USDVOL_FIELD_CLASS_UNKNOWN;
#if USD_USE_FULL
    try {
        TfToken fieldClass;
        if (field->field.GetFieldClassAttr().Get(&fieldClass)) {
            return TokenToFieldClass(fieldClass);
        }
        return USDVOL_FIELD_CLASS_UNKNOWN;
    } CATCH_AND_RETURN(USDVOL_FIELD_CLASS_UNKNOWN)
#else
    return static_cast<UsdVolFieldClass>(field->fieldClass);
#endif
}

UsdResult UsdVolOpenVDBAsset_SetFieldClass(
    UsdVolOpenVDBAssetRef field,
    UsdVolFieldClass fieldClass
) {
    if (!field) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        TfToken token = FieldClassToToken(fieldClass);
        if (field->field.GetFieldClassAttr().Set(token)) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
    } CATCH_AND_RETURN_RESULT
#else
    field->fieldClass = static_cast<int>(fieldClass);
    return USD_RESULT_SUCCESS;
#endif
}

// ============================================================================
// MARK: - Utility Functions
// ============================================================================

void UsdVol_FreeString(char* str) {
    free(str);
}

} // extern "C"
