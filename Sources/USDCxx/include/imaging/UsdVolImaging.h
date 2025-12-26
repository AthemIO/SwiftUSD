// UsdVolImaging.h - Volume Rendering Adapters for SwiftUSD
// Mirrors: pxr/usdImaging/usdVolImaging/
// C++ header with Swift interop annotations
//
// UsdVolImaging provides imaging adapters for volumetric data in USD,
// translating volume field definitions into Hydra render primitives.
// Key components include:
// - OpenVDBAssetAdapter: Handles OpenVDB volume field prims
// - Field3DAssetAdapter: Handles Field3D volume field prims
// - DataSourceFieldAsset: Scene index data source for field assets

#pragma once

#include "../Swift.h"
#include "../base/Tf.h"
#include "../base/Gf.h"
#include "../usd/Sdf.h"
#include "../usd/Usd.h"
#include "../usd/UsdVol.h"
#include "Hd.h"
#include "UsdImaging.h"

#if defined(USE_PIXAR_USD)
#include "pxr/pxr.h"
#include "pxr/usdImaging/usdVolImaging/openvdbAssetAdapter.h"
#include "pxr/usdImaging/usdVolImaging/field3dAssetAdapter.h"
#include "pxr/usdImaging/usdVolImaging/tokens.h"
#endif // USE_PIXAR_USD

namespace swiftusd {

// ============================================================================
// MARK: - Forward Declarations
// ============================================================================

class UsdVolImagingOpenVDBAssetAdapterRef;
class UsdVolImagingField3DAssetAdapterRef;

// ============================================================================
// MARK: - UsdVolImaging Tokens
// ============================================================================

/// Common tokens used in UsdVolImaging.
namespace UsdVolImagingTokens {

/// Field asset type tokens
inline Token Field3DAsset() { return Token("field3dAsset"); }
inline Token OpenVDBAsset() { return Token("openvdbAsset"); }

/// Volume field tokens
inline Token Volume() { return Token("Volume"); }
inline Token Field() { return Token("field"); }

/// Field attribute tokens
inline Token FilePath() { return Token("filePath"); }
inline Token FieldName() { return Token("fieldName"); }
inline Token FieldIndex() { return Token("fieldIndex"); }
inline Token FieldDataType() { return Token("fieldDataType"); }
inline Token VectorDataRoleHint() { return Token("vectorDataRoleHint"); }

/// OpenVDB-specific tokens
inline Token FieldClass() { return Token("fieldClass"); }
inline Token LevelSet() { return Token("levelSet"); }
inline Token FogVolume() { return Token("fogVolume"); }
inline Token Staggered() { return Token("staggered"); }
inline Token Unknown() { return Token("unknown"); }

/// Field3D-specific tokens
inline Token FieldPurpose() { return Token("fieldPurpose"); }

/// Data type tokens
inline Token Half() { return Token("half"); }
inline Token Float() { return Token("float"); }
inline Token Double() { return Token("double"); }
inline Token Half3() { return Token("half3"); }
inline Token Float3() { return Token("float3"); }
inline Token Double3() { return Token("double3"); }

} // namespace UsdVolImagingTokens

// ============================================================================
// MARK: - UsdVolImagingFieldParams (Field configuration parameters)
// ============================================================================

/// Configuration parameters for volume field imaging.
/// These describe the properties of a volume field for rendering.
struct UsdVolImagingFieldParams {
    /// The file path to the volume data file
    char filePath[512];

    /// The name of the field within the file
    char fieldName[128];

    /// Index to disambiguate multiple fields with the same name
    int32_t fieldIndex;

    /// Whether this field is valid for rendering
    bool isValid;

    UsdVolImagingFieldParams() SWIFTUSD_NOEXCEPT
        : fieldIndex(0)
        , isValid(false) {
        filePath[0] = '\0';
        fieldName[0] = '\0';
    }

    bool operator==(const UsdVolImagingFieldParams& rhs) const SWIFTUSD_NOEXCEPT {
        return fieldIndex == rhs.fieldIndex &&
               isValid == rhs.isValid &&
               std::strcmp(filePath, rhs.filePath) == 0 &&
               std::strcmp(fieldName, rhs.fieldName) == 0;
    }

    bool operator!=(const UsdVolImagingFieldParams& rhs) const SWIFTUSD_NOEXCEPT {
        return !(*this == rhs);
    }

    /// Check if the field parameters are valid
    bool IsValid() const SWIFTUSD_NOEXCEPT {
        return isValid && filePath[0] != '\0';
    }

    /// Set the file path
    void SetFilePath(const char* path) SWIFTUSD_NOEXCEPT {
        if (path) {
            std::strncpy(filePath, path, sizeof(filePath) - 1);
            filePath[sizeof(filePath) - 1] = '\0';
        } else {
            filePath[0] = '\0';
        }
    }

    /// Set the field name
    void SetFieldName(const char* name) SWIFTUSD_NOEXCEPT {
        if (name) {
            std::strncpy(fieldName, name, sizeof(fieldName) - 1);
            fieldName[sizeof(fieldName) - 1] = '\0';
        } else {
            fieldName[0] = '\0';
        }
    }

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - UsdVolImagingOpenVDBAssetAdapterRef (OpenVDB adapter wrapper)
// ============================================================================

/// The OpenVDB asset adapter handles population and rendering of OpenVDB
/// volume field prims.
///
/// This adapter:
/// - Registers OpenVDB field prims with the Hydra render index
/// - Provides field data to volume rendering backends
/// - Handles time-varying field data
/// - Supports OpenVDB-specific attributes like field class
///
/// OpenVDB is a widely-used volume data format that stores sparse voxel
/// data efficiently using a hierarchical data structure.
class UsdVolImagingOpenVDBAssetAdapterRef {
public:
    /// Create a new OpenVDB asset adapter.
    ///
    /// @return A new OpenVDB asset adapter reference, or nullptr if creation fails
    static UsdVolImagingOpenVDBAssetAdapterRef* Create() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        // Note: In real USD, adapters are created and managed by the delegate
        // We provide a wrapper for querying capabilities
        return new UsdVolImagingOpenVDBAssetAdapterRef();
#else
        return new UsdVolImagingOpenVDBAssetAdapterRef();
#endif
    }

    /// Check if this adapter is valid and ready for use.
    bool IsValid() const SWIFTUSD_NOEXCEPT {
        return true;
    }

    // ========================================================================
    // MARK: Adapter Information
    // ========================================================================

    /// Get the prim type token that this adapter handles.
    Token GetPrimTypeToken() const SWIFTUSD_NOEXCEPT {
        return Token("OpenVDBAsset");
    }

    /// Get the imaging token for this field type.
    Token GetImagingToken() const SWIFTUSD_NOEXCEPT {
        return UsdVolImagingTokens::OpenVDBAsset();
    }

    /// Check if this adapter supports instancing.
    bool IsInstanceable() const SWIFTUSD_NOEXCEPT {
        return false; // Field adapters typically don't support instancing
    }

    // ========================================================================
    // MARK: Field Information
    // ========================================================================

    /// Get the supported file extensions for OpenVDB.
    ///
    /// @param outBuffer Buffer to receive extension tokens
    /// @param maxCount Maximum number of extensions to return
    /// @return Number of extensions written
    size_t GetSupportedExtensions(Token* outBuffer, size_t maxCount) const SWIFTUSD_NOEXCEPT {
        if (!outBuffer || maxCount == 0) return 0;

        size_t count = 0;
        if (count < maxCount) outBuffer[count++] = Token("vdb");
        return count;
    }

    /// Check if a file extension is supported by this adapter.
    bool IsExtensionSupported(const char* extension) const SWIFTUSD_NOEXCEPT {
        if (!extension) return false;
        return std::strcmp(extension, "vdb") == 0 ||
               std::strcmp(extension, "VDB") == 0;
    }

    /// Get field parameters from a volume field prim.
    ///
    /// @param fieldPrim The OpenVDB field prim
    /// @return Field parameters describing the volume data
    UsdVolImagingFieldParams GetFieldParams(VolOpenVDBAssetRef* fieldPrim) const SWIFTUSD_NOEXCEPT {
        UsdVolImagingFieldParams params;
        if (!fieldPrim || !fieldPrim->IsValid()) return params;

        params.SetFilePath(fieldPrim->GetFilePath());
        params.SetFieldName(fieldPrim->GetFieldName().GetText());
        params.fieldIndex = fieldPrim->GetFieldIndex();
        params.isValid = true;

        return params;
    }

    /// Get the field class for an OpenVDB field.
    ///
    /// @param fieldPrim The OpenVDB field prim
    /// @return Token representing the field class (levelSet, fogVolume, staggered, unknown)
    Token GetFieldClass(VolOpenVDBAssetRef* fieldPrim) const SWIFTUSD_NOEXCEPT {
        if (!fieldPrim || !fieldPrim->IsValid()) {
            return UsdVolImagingTokens::Unknown();
        }

        OpenVDBFieldClass fieldClass = fieldPrim->GetFieldClass();
        switch (fieldClass.value) {
            case OpenVDBFieldClass::LevelSet:
                return UsdVolImagingTokens::LevelSet();
            case OpenVDBFieldClass::FogVolume:
                return UsdVolImagingTokens::FogVolume();
            case OpenVDBFieldClass::Staggered:
                return UsdVolImagingTokens::Staggered();
            default:
                return UsdVolImagingTokens::Unknown();
        }
    }

    // ========================================================================
    // MARK: Reference Counting
    // ========================================================================

    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
    UsdVolImagingOpenVDBAssetAdapterRef() SWIFTUSD_NOEXCEPT
        : _refCount(1) {}

    ~UsdVolImagingOpenVDBAssetAdapterRef() = default;

    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - UsdVolImagingField3DAssetAdapterRef (Field3D adapter wrapper)
// ============================================================================

/// The Field3D asset adapter handles population and rendering of Field3D
/// volume field prims.
///
/// This adapter:
/// - Registers Field3D field prims with the Hydra render index
/// - Provides field data to volume rendering backends
/// - Handles time-varying field data
/// - Supports Field3D-specific attributes like field purpose
///
/// Field3D is a library and file format for storing voxel data developed
/// by Sony Pictures Imageworks.
class UsdVolImagingField3DAssetAdapterRef {
public:
    /// Create a new Field3D asset adapter.
    ///
    /// @return A new Field3D asset adapter reference, or nullptr if creation fails
    static UsdVolImagingField3DAssetAdapterRef* Create() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return new UsdVolImagingField3DAssetAdapterRef();
#else
        return new UsdVolImagingField3DAssetAdapterRef();
#endif
    }

    /// Check if this adapter is valid and ready for use.
    bool IsValid() const SWIFTUSD_NOEXCEPT {
        return true;
    }

    // ========================================================================
    // MARK: Adapter Information
    // ========================================================================

    /// Get the prim type token that this adapter handles.
    Token GetPrimTypeToken() const SWIFTUSD_NOEXCEPT {
        return Token("Field3DAsset");
    }

    /// Get the imaging token for this field type.
    Token GetImagingToken() const SWIFTUSD_NOEXCEPT {
        return UsdVolImagingTokens::Field3DAsset();
    }

    /// Check if this adapter supports instancing.
    bool IsInstanceable() const SWIFTUSD_NOEXCEPT {
        return false; // Field adapters typically don't support instancing
    }

    // ========================================================================
    // MARK: Field Information
    // ========================================================================

    /// Get the supported file extensions for Field3D.
    ///
    /// @param outBuffer Buffer to receive extension tokens
    /// @param maxCount Maximum number of extensions to return
    /// @return Number of extensions written
    size_t GetSupportedExtensions(Token* outBuffer, size_t maxCount) const SWIFTUSD_NOEXCEPT {
        if (!outBuffer || maxCount == 0) return 0;

        size_t count = 0;
        if (count < maxCount) outBuffer[count++] = Token("f3d");
        return count;
    }

    /// Check if a file extension is supported by this adapter.
    bool IsExtensionSupported(const char* extension) const SWIFTUSD_NOEXCEPT {
        if (!extension) return false;
        return std::strcmp(extension, "f3d") == 0 ||
               std::strcmp(extension, "F3D") == 0;
    }

    /// Get field parameters from a volume field prim.
    ///
    /// @param fieldPrim The Field3D field prim
    /// @return Field parameters describing the volume data
    UsdVolImagingFieldParams GetFieldParams(VolField3DAssetRef* fieldPrim) const SWIFTUSD_NOEXCEPT {
        UsdVolImagingFieldParams params;
        if (!fieldPrim || !fieldPrim->IsValid()) return params;

        params.SetFilePath(fieldPrim->GetFilePath());
        params.SetFieldName(fieldPrim->GetFieldName().GetText());
        params.fieldIndex = fieldPrim->GetFieldIndex();
        params.isValid = true;

        return params;
    }

    /// Get the field purpose for a Field3D field.
    ///
    /// @param fieldPrim The Field3D field prim
    /// @return Token representing the field purpose
    Token GetFieldPurpose(VolField3DAssetRef* fieldPrim) const SWIFTUSD_NOEXCEPT {
        if (!fieldPrim || !fieldPrim->IsValid()) {
            return Token();
        }
        return fieldPrim->GetFieldPurpose();
    }

    // ========================================================================
    // MARK: Reference Counting
    // ========================================================================

    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
    UsdVolImagingField3DAssetAdapterRef() SWIFTUSD_NOEXCEPT
        : _refCount(1) {}

    ~UsdVolImagingField3DAssetAdapterRef() = default;

    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - Utility Functions
// ============================================================================

/// Get the field asset type token for a given file extension.
///
/// @param extension File extension (without the dot)
/// @return Token for the corresponding field asset type, or empty token if unknown
inline Token UsdVolImagingGetFieldTypeForExtension(const char* extension) SWIFTUSD_NOEXCEPT {
    if (!extension) return Token();

    if (std::strcmp(extension, "vdb") == 0 ||
        std::strcmp(extension, "VDB") == 0) {
        return UsdVolImagingTokens::OpenVDBAsset();
    }

    if (std::strcmp(extension, "f3d") == 0 ||
        std::strcmp(extension, "F3D") == 0) {
        return UsdVolImagingTokens::Field3DAsset();
    }

    return Token();
}

/// Check if a file extension is a supported volume format.
///
/// @param extension File extension (without the dot)
/// @return true if the extension represents a supported volume format
inline bool UsdVolImagingIsVolumeExtension(const char* extension) SWIFTUSD_NOEXCEPT {
    if (!extension) return false;

    return std::strcmp(extension, "vdb") == 0 ||
           std::strcmp(extension, "VDB") == 0 ||
           std::strcmp(extension, "f3d") == 0 ||
           std::strcmp(extension, "F3D") == 0;
}

/// Get the volume prim type token.
inline Token UsdVolImagingGetVolumePrimType() SWIFTUSD_NOEXCEPT {
    return UsdVolImagingTokens::Volume();
}

/// Check if a token represents a volume field prim type.
///
/// @param primType Token to check
/// @return true if the token is a recognized volume field type
inline bool UsdVolImagingIsFieldPrimType(const Token& primType) SWIFTUSD_NOEXCEPT {
    return primType == Token("OpenVDBAsset") ||
           primType == Token("Field3DAsset") ||
           primType == Token("FieldAsset");
}

/// Get the field relationship prefix used in Volume prims.
/// Field relationships are named field:<fieldName>.
inline Token UsdVolImagingGetFieldRelationshipPrefix() SWIFTUSD_NOEXCEPT {
    return UsdVolImagingTokens::Field();
}

} // namespace swiftusd
