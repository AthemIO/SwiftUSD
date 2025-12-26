// UsdVol.h - Volume Schemas for SwiftUSD
// Mirrors: pxr/usd/usdVol/*.h (Volume, FieldBase, FieldAsset, OpenVDBAsset, Field3DAsset)
// C++ header with Swift interop annotations

#pragma once

#include "../Swift.h"
#include "../base/Gf.h"
#include "../base/Tf.h"
#include "../base/Vt.h"
#include "Sdf.h"
#include "Usd.h"
#include "UsdGeom.h"

#if defined(USE_PIXAR_USD)
#include "pxr/pxr.h"
#include "pxr/usd/usdVol/volume.h"
#include "pxr/usd/usdVol/fieldBase.h"
#include "pxr/usd/usdVol/fieldAsset.h"
#include "pxr/usd/usdVol/openVDBAsset.h"
#include "pxr/usd/usdVol/field3DAsset.h"
#include "pxr/usd/usdVol/tokens.h"
#endif

namespace swiftusd {

// Forward declarations
class VolVolumeRef;
class VolFieldBaseRef;
class VolFieldAssetRef;
class VolOpenVDBAssetRef;
class VolField3DAssetRef;

// ============================================================================
// MARK: - VectorDataRoleHint (enum value type)
// ============================================================================

/// Hint for how vector data should be interpreted.
/// Maps to UsdVolTokens: None, Point, Normal, Vector, Color
struct VectorDataRoleHint {
    enum Value : int {
        None = 0,    // No special role
        Point = 1,   // Position data
        Normal = 2,  // Normal vector
        Vector = 3,  // Generic vector
        Color = 4    // Color data
    };
    Value value;

    VectorDataRoleHint() SWIFTUSD_NOEXCEPT : value(None) {}
    VectorDataRoleHint(Value v) SWIFTUSD_NOEXCEPT : value(v) {}

    bool operator==(const VectorDataRoleHint& other) const SWIFTUSD_NOEXCEPT {
        return value == other.value;
    }

    bool operator!=(const VectorDataRoleHint& other) const SWIFTUSD_NOEXCEPT {
        return value != other.value;
    }

#if defined(USE_PIXAR_USD)
    PXR_NS::TfToken ToUsd() const SWIFTUSD_NOEXCEPT {
        switch (value) {
            case None: return PXR_NS::UsdVolTokens->None_;
            case Point: return PXR_NS::UsdVolTokens->Point;
            case Normal: return PXR_NS::UsdVolTokens->Normal;
            case Vector: return PXR_NS::UsdVolTokens->Vector;
            case Color: return PXR_NS::UsdVolTokens->Color;
            default: return PXR_NS::UsdVolTokens->None_;
        }
    }

    static VectorDataRoleHint FromUsd(const PXR_NS::TfToken& token) SWIFTUSD_NOEXCEPT {
        if (token == PXR_NS::UsdVolTokens->Point) return VectorDataRoleHint(Point);
        if (token == PXR_NS::UsdVolTokens->Normal) return VectorDataRoleHint(Normal);
        if (token == PXR_NS::UsdVolTokens->Vector) return VectorDataRoleHint(Vector);
        if (token == PXR_NS::UsdVolTokens->Color) return VectorDataRoleHint(Color);
        return VectorDataRoleHint(None);
    }
#endif
} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - OpenVDBFieldClass (enum value type)
// ============================================================================

/// Classification for OpenVDB grids.
/// Maps to UsdVolTokens: levelSet, fogVolume, staggered, unknown
struct OpenVDBFieldClass {
    enum Value : int {
        Unknown = 0,    // Unknown grid class
        LevelSet = 1,   // Level set (signed distance field)
        FogVolume = 2,  // Fog volume
        Staggered = 3   // Staggered MAC grid
    };
    Value value;

    OpenVDBFieldClass() SWIFTUSD_NOEXCEPT : value(Unknown) {}
    OpenVDBFieldClass(Value v) SWIFTUSD_NOEXCEPT : value(v) {}

    bool operator==(const OpenVDBFieldClass& other) const SWIFTUSD_NOEXCEPT {
        return value == other.value;
    }

    bool operator!=(const OpenVDBFieldClass& other) const SWIFTUSD_NOEXCEPT {
        return value != other.value;
    }

#if defined(USE_PIXAR_USD)
    PXR_NS::TfToken ToUsd() const SWIFTUSD_NOEXCEPT {
        switch (value) {
            case LevelSet: return PXR_NS::UsdVolTokens->levelSet;
            case FogVolume: return PXR_NS::UsdVolTokens->fogVolume;
            case Staggered: return PXR_NS::UsdVolTokens->staggered;
            default: return PXR_NS::UsdVolTokens->unknown;
        }
    }

    static OpenVDBFieldClass FromUsd(const PXR_NS::TfToken& token) SWIFTUSD_NOEXCEPT {
        if (token == PXR_NS::UsdVolTokens->levelSet) return OpenVDBFieldClass(LevelSet);
        if (token == PXR_NS::UsdVolTokens->fogVolume) return OpenVDBFieldClass(FogVolume);
        if (token == PXR_NS::UsdVolTokens->staggered) return OpenVDBFieldClass(Staggered);
        return OpenVDBFieldClass(Unknown);
    }
#endif
} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - VolVolumeRef (UsdVolVolume reference type)
// ============================================================================

/// A renderable volume primitive.
///
/// A volume is made up of any number of FieldBase primitives bound together
/// in this volume. Each FieldBase primitive is specified as a relationship
/// with a namespace prefix of "field".
class VolVolumeRef {
public:
    // Factory methods
    static VolVolumeRef* Define(StageRef* stage, const Path& path) SWIFTUSD_NOEXCEPT {
        if (!stage) return nullptr;
#if defined(USE_PIXAR_USD)
        auto volume = PXR_NS::UsdVolVolume::Define(stage->GetUsdStage(), path.GetSdfPath());
        if (!volume) return nullptr;
        return new VolVolumeRef(volume, stage);
#else
        auto* prim = stage->DefinePrim(path, Token("Volume"));
        if (!prim) return nullptr;
        prim->release();
        return new VolVolumeRef(path, stage);
#endif
    }

    static VolVolumeRef* FromPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto volume = PXR_NS::UsdVolVolume(prim->GetUsdPrim());
        if (!volume) return nullptr;
        return new VolVolumeRef(volume, prim->GetStage());
#else
        return new VolVolumeRef(prim->GetPath(), prim->GetStage());
#endif
    }

    // Validity
    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_volume);
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    // Prim access
    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_volume) return nullptr;
        return new PrimRef(_volume.GetPrim(), _stage);
#else
        return _stage ? _stage->GetPrimAtPath(_path) : nullptr;
#endif
    }

    // Field relationship methods

    /// Creates a relationship on this volume that targets the specified field.
    /// If an existing relationship exists with the same name, it is replaced.
    bool CreateFieldRelationship(const Token& name, const Path& fieldPath) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_volume) return false;
        return _volume.CreateFieldRelationship(name.GetTfToken(), fieldPath.GetSdfPath());
#else
        _fieldRelationships[name.GetText()] = fieldPath;
        return true;
#endif
    }

    /// Checks if there is an existing field relationship with the given name.
    bool HasFieldRelationship(const Token& name) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_volume) return false;
        return _volume.HasFieldRelationship(name.GetTfToken());
#else
        return _fieldRelationships.find(name.GetText()) != _fieldRelationships.end();
#endif
    }

    /// Gets the path to the field for the given relationship name.
    /// Returns empty path if not found.
    Path GetFieldPath(const Token& name) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_volume) return Path();
        PXR_NS::SdfPath sdfPath = _volume.GetFieldPath(name.GetTfToken());
        return Path(sdfPath);
#else
        auto it = _fieldRelationships.find(name.GetText());
        if (it != _fieldRelationships.end()) {
            return it->second;
        }
        return Path();
#endif
    }

    /// Blocks an existing field relationship on this volume.
    /// Returns true if the relationship existed, false otherwise.
    bool BlockFieldRelationship(const Token& name) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_volume) return false;
        return _volume.BlockFieldRelationship(name.GetTfToken());
#else
        auto it = _fieldRelationships.find(name.GetText());
        if (it != _fieldRelationships.end()) {
            _fieldRelationships.erase(it);
            return true;
        }
        return false;
#endif
    }

    // GeomGprim inherited properties (extent, visibility, purpose)

    /// Get the visibility of this volume
    Token GetVisibility(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_volume) return Token("inherited");
        PXR_NS::TfToken visibility;
        _volume.GetVisibilityAttr().Get(&visibility, time.ToUsd());
        return Token(visibility);
#else
        return _visibility;
#endif
    }

    /// Set the visibility of this volume
    bool SetVisibility(const Token& visibility, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_volume) return false;
        return _volume.GetVisibilityAttr().Set(visibility.GetTfToken(), time.ToUsd());
#else
        _visibility = visibility;
        return true;
#endif
    }

    /// Get the purpose of this volume
    Token GetPurpose() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_volume) return Token("default");
        PXR_NS::TfToken purpose;
        _volume.GetPurposeAttr().Get(&purpose);
        return Token(purpose);
#else
        return _purpose;
#endif
    }

    /// Set the purpose of this volume
    bool SetPurpose(const Token& purpose) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_volume) return false;
        return _volume.GetPurposeAttr().Set(purpose.GetTfToken());
#else
        _purpose = purpose;
        return true;
#endif
    }

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
#if defined(USE_PIXAR_USD)
    VolVolumeRef(const PXR_NS::UsdVolVolume& volume, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _volume(volume), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~VolVolumeRef() {
        if (_stage) _stage->release();
    }

    PXR_NS::UsdVolVolume _volume;
#else
    VolVolumeRef(const Path& path, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path), _stage(stage),
          _visibility("inherited"), _purpose("default"),
          _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~VolVolumeRef() {
        if (_stage) _stage->release();
    }

    Path _path;
    std::map<std::string, Path> _fieldRelationships;
    Token _visibility;
    Token _purpose;
#endif

    StageRef* _stage;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - VolFieldBaseRef (UsdVolFieldBase reference type)
// ============================================================================

/// Base class for field primitives.
/// This is an abstract typed schema - you cannot define a FieldBase directly,
/// but you can wrap existing prims.
class VolFieldBaseRef {
public:
    // Factory method (no Define since this is abstract)
    static VolFieldBaseRef* FromPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto field = PXR_NS::UsdVolFieldBase(prim->GetUsdPrim());
        if (!field) return nullptr;
        return new VolFieldBaseRef(field, prim->GetStage());
#else
        return new VolFieldBaseRef(prim->GetPath(), prim->GetStage());
#endif
    }

    // Validity
    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_field);
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    // Prim access
    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_field) return nullptr;
        return new PrimRef(_field.GetPrim(), _stage);
#else
        return _stage ? _stage->GetPrimAtPath(_path) : nullptr;
#endif
    }

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
#if defined(USE_PIXAR_USD)
    VolFieldBaseRef(const PXR_NS::UsdVolFieldBase& field, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _field(field), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~VolFieldBaseRef() {
        if (_stage) _stage->release();
    }

    PXR_NS::UsdVolFieldBase _field;
#else
    VolFieldBaseRef(const Path& path, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~VolFieldBaseRef() {
        if (_stage) _stage->release();
    }

    Path _path;
#endif

    StageRef* _stage;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - VolFieldAssetRef (UsdVolFieldAsset reference type)
// ============================================================================

/// Base class for field primitives defined by an external file.
/// This is an abstract typed schema - you cannot define a FieldAsset directly.
class VolFieldAssetRef {
public:
    // Factory method (no Define since this is abstract)
    static VolFieldAssetRef* FromPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto field = PXR_NS::UsdVolFieldAsset(prim->GetUsdPrim());
        if (!field) return nullptr;
        return new VolFieldAssetRef(field, prim->GetStage());
#else
        return new VolFieldAssetRef(prim->GetPath(), prim->GetStage());
#endif
    }

    // Validity
    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_field);
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    // Prim access
    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_field) return nullptr;
        return new PrimRef(_field.GetPrim(), _stage);
#else
        return _stage ? _stage->GetPrimAtPath(_path) : nullptr;
#endif
    }

    // File path - asset path to the volume file
    const char* GetFilePath() const SWIFTUSD_NOEXCEPT SWIFT_RETURNS_INDEPENDENT_VALUE {
#if defined(USE_PIXAR_USD)
        if (!_field) return "";
        PXR_NS::SdfAssetPath assetPath;
        _field.GetFilePathAttr().Get(&assetPath);
        _cachedFilePath = assetPath.GetAssetPath();
        return _cachedFilePath.c_str();
#else
        return _filePath.c_str();
#endif
    }

    bool SetFilePath(const char* filePath, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
        if (!filePath) return false;
#if defined(USE_PIXAR_USD)
        if (!_field) return false;
        return _field.GetFilePathAttr().Set(PXR_NS::SdfAssetPath(filePath), time.ToUsd());
#else
        _filePath = filePath;
        return true;
#endif
    }

    // Field name - name of the field within the file
    Token GetFieldName(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_field) return Token();
        PXR_NS::TfToken fieldName;
        _field.GetFieldNameAttr().Get(&fieldName, time.ToUsd());
        return Token(fieldName);
#else
        return _fieldName;
#endif
    }

    bool SetFieldName(const Token& fieldName, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_field) return false;
        return _field.GetFieldNameAttr().Set(fieldName.GetTfToken(), time.ToUsd());
#else
        _fieldName = fieldName;
        return true;
#endif
    }

    // Field index - disambiguates multiple fields with same name
    int GetFieldIndex(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_field) return 0;
        int fieldIndex = 0;
        _field.GetFieldIndexAttr().Get(&fieldIndex, time.ToUsd());
        return fieldIndex;
#else
        return _fieldIndex;
#endif
    }

    bool SetFieldIndex(int fieldIndex, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_field) return false;
        return _field.GetFieldIndexAttr().Set(fieldIndex, time.ToUsd());
#else
        _fieldIndex = fieldIndex;
        return true;
#endif
    }

    // Field data type - token indicating the data type
    Token GetFieldDataType(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_field) return Token();
        PXR_NS::TfToken dataType;
        _field.GetFieldDataTypeAttr().Get(&dataType, time.ToUsd());
        return Token(dataType);
#else
        return _fieldDataType;
#endif
    }

    bool SetFieldDataType(const Token& dataType, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_field) return false;
        return _field.GetFieldDataTypeAttr().Set(dataType.GetTfToken(), time.ToUsd());
#else
        _fieldDataType = dataType;
        return true;
#endif
    }

    // Vector data role hint
    VectorDataRoleHint GetVectorDataRoleHint(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_field) return VectorDataRoleHint();
        PXR_NS::TfToken roleHint;
        _field.GetVectorDataRoleHintAttr().Get(&roleHint, time.ToUsd());
        return VectorDataRoleHint::FromUsd(roleHint);
#else
        return _vectorDataRoleHint;
#endif
    }

    bool SetVectorDataRoleHint(VectorDataRoleHint roleHint, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_field) return false;
        return _field.GetVectorDataRoleHintAttr().Set(roleHint.ToUsd(), time.ToUsd());
#else
        _vectorDataRoleHint = roleHint;
        return true;
#endif
    }

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
#if defined(USE_PIXAR_USD)
    VolFieldAssetRef(const PXR_NS::UsdVolFieldAsset& field, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _field(field), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~VolFieldAssetRef() {
        if (_stage) _stage->release();
    }

    PXR_NS::UsdVolFieldAsset _field;
    mutable std::string _cachedFilePath;
#else
    VolFieldAssetRef(const Path& path, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path), _stage(stage),
          _fieldIndex(0), _vectorDataRoleHint(),
          _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~VolFieldAssetRef() {
        if (_stage) _stage->release();
    }

    Path _path;
    std::string _filePath;
    Token _fieldName;
    int _fieldIndex;
    Token _fieldDataType;
    VectorDataRoleHint _vectorDataRoleHint;
#endif

    StageRef* _stage;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - VolOpenVDBAssetRef (UsdVolOpenVDBAsset reference type)
// ============================================================================

/// OpenVDB field primitive.
/// The FieldAsset filePath attribute must specify a file in the OpenVDB format.
class VolOpenVDBAssetRef {
public:
    // Factory methods
    static VolOpenVDBAssetRef* Define(StageRef* stage, const Path& path) SWIFTUSD_NOEXCEPT {
        if (!stage) return nullptr;
#if defined(USE_PIXAR_USD)
        auto field = PXR_NS::UsdVolOpenVDBAsset::Define(stage->GetUsdStage(), path.GetSdfPath());
        if (!field) return nullptr;
        return new VolOpenVDBAssetRef(field, stage);
#else
        auto* prim = stage->DefinePrim(path, Token("OpenVDBAsset"));
        if (!prim) return nullptr;
        prim->release();
        return new VolOpenVDBAssetRef(path, stage);
#endif
    }

    static VolOpenVDBAssetRef* FromPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto field = PXR_NS::UsdVolOpenVDBAsset(prim->GetUsdPrim());
        if (!field) return nullptr;
        return new VolOpenVDBAssetRef(field, prim->GetStage());
#else
        return new VolOpenVDBAssetRef(prim->GetPath(), prim->GetStage());
#endif
    }

    // Validity
    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_field);
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    // Prim access
    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_field) return nullptr;
        return new PrimRef(_field.GetPrim(), _stage);
#else
        return _stage ? _stage->GetPrimAtPath(_path) : nullptr;
#endif
    }

    // File path - inherited from FieldAsset
    const char* GetFilePath() const SWIFTUSD_NOEXCEPT SWIFT_RETURNS_INDEPENDENT_VALUE {
#if defined(USE_PIXAR_USD)
        if (!_field) return "";
        PXR_NS::SdfAssetPath assetPath;
        _field.GetFilePathAttr().Get(&assetPath);
        _cachedFilePath = assetPath.GetAssetPath();
        return _cachedFilePath.c_str();
#else
        return _filePath.c_str();
#endif
    }

    bool SetFilePath(const char* filePath, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
        if (!filePath) return false;
#if defined(USE_PIXAR_USD)
        if (!_field) return false;
        return _field.GetFilePathAttr().Set(PXR_NS::SdfAssetPath(filePath), time.ToUsd());
#else
        _filePath = filePath;
        return true;
#endif
    }

    // Field name - inherited from FieldAsset
    Token GetFieldName(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_field) return Token();
        PXR_NS::TfToken fieldName;
        _field.GetFieldNameAttr().Get(&fieldName, time.ToUsd());
        return Token(fieldName);
#else
        return _fieldName;
#endif
    }

    bool SetFieldName(const Token& fieldName, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_field) return false;
        return _field.GetFieldNameAttr().Set(fieldName.GetTfToken(), time.ToUsd());
#else
        _fieldName = fieldName;
        return true;
#endif
    }

    // Field index - inherited from FieldAsset
    int GetFieldIndex(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_field) return 0;
        int fieldIndex = 0;
        _field.GetFieldIndexAttr().Get(&fieldIndex, time.ToUsd());
        return fieldIndex;
#else
        return _fieldIndex;
#endif
    }

    bool SetFieldIndex(int fieldIndex, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_field) return false;
        return _field.GetFieldIndexAttr().Set(fieldIndex, time.ToUsd());
#else
        _fieldIndex = fieldIndex;
        return true;
#endif
    }

    // Field data type - OpenVDB supports many types
    Token GetFieldDataType(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_field) return Token();
        PXR_NS::TfToken dataType;
        _field.GetFieldDataTypeAttr().Get(&dataType, time.ToUsd());
        return Token(dataType);
#else
        return _fieldDataType;
#endif
    }

    bool SetFieldDataType(const Token& dataType, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_field) return false;
        return _field.GetFieldDataTypeAttr().Set(dataType.GetTfToken(), time.ToUsd());
#else
        _fieldDataType = dataType;
        return true;
#endif
    }

    // Vector data role hint - inherited from FieldAsset
    VectorDataRoleHint GetVectorDataRoleHint(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_field) return VectorDataRoleHint();
        PXR_NS::TfToken roleHint;
        _field.GetVectorDataRoleHintAttr().Get(&roleHint, time.ToUsd());
        return VectorDataRoleHint::FromUsd(roleHint);
#else
        return _vectorDataRoleHint;
#endif
    }

    bool SetVectorDataRoleHint(VectorDataRoleHint roleHint, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_field) return false;
        return _field.GetVectorDataRoleHintAttr().Set(roleHint.ToUsd(), time.ToUsd());
#else
        _vectorDataRoleHint = roleHint;
        return true;
#endif
    }

    // Field class - OpenVDB-specific (levelSet, fogVolume, staggered, unknown)
    OpenVDBFieldClass GetFieldClass(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_field) return OpenVDBFieldClass();
        PXR_NS::TfToken fieldClass;
        _field.GetFieldClassAttr().Get(&fieldClass, time.ToUsd());
        return OpenVDBFieldClass::FromUsd(fieldClass);
#else
        return _fieldClass;
#endif
    }

    bool SetFieldClass(OpenVDBFieldClass fieldClass, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_field) return false;
        return _field.GetFieldClassAttr().Set(fieldClass.ToUsd(), time.ToUsd());
#else
        _fieldClass = fieldClass;
        return true;
#endif
    }

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
#if defined(USE_PIXAR_USD)
    VolOpenVDBAssetRef(const PXR_NS::UsdVolOpenVDBAsset& field, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _field(field), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~VolOpenVDBAssetRef() {
        if (_stage) _stage->release();
    }

    PXR_NS::UsdVolOpenVDBAsset _field;
    mutable std::string _cachedFilePath;
#else
    VolOpenVDBAssetRef(const Path& path, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path), _stage(stage),
          _fieldIndex(0), _vectorDataRoleHint(), _fieldClass(),
          _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~VolOpenVDBAssetRef() {
        if (_stage) _stage->release();
    }

    Path _path;
    std::string _filePath;
    Token _fieldName;
    int _fieldIndex;
    Token _fieldDataType;
    VectorDataRoleHint _vectorDataRoleHint;
    OpenVDBFieldClass _fieldClass;
#endif

    StageRef* _stage;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - VolField3DAssetRef (UsdVolField3DAsset reference type)
// ============================================================================

/// Field3D field primitive.
/// The FieldAsset filePath attribute must specify a file in the Field3D format.
class VolField3DAssetRef {
public:
    // Factory methods
    static VolField3DAssetRef* Define(StageRef* stage, const Path& path) SWIFTUSD_NOEXCEPT {
        if (!stage) return nullptr;
#if defined(USE_PIXAR_USD)
        auto field = PXR_NS::UsdVolField3DAsset::Define(stage->GetUsdStage(), path.GetSdfPath());
        if (!field) return nullptr;
        return new VolField3DAssetRef(field, stage);
#else
        auto* prim = stage->DefinePrim(path, Token("Field3DAsset"));
        if (!prim) return nullptr;
        prim->release();
        return new VolField3DAssetRef(path, stage);
#endif
    }

    static VolField3DAssetRef* FromPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto field = PXR_NS::UsdVolField3DAsset(prim->GetUsdPrim());
        if (!field) return nullptr;
        return new VolField3DAssetRef(field, prim->GetStage());
#else
        return new VolField3DAssetRef(prim->GetPath(), prim->GetStage());
#endif
    }

    // Validity
    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_field);
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    // Prim access
    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_field) return nullptr;
        return new PrimRef(_field.GetPrim(), _stage);
#else
        return _stage ? _stage->GetPrimAtPath(_path) : nullptr;
#endif
    }

    // File path - inherited from FieldAsset
    const char* GetFilePath() const SWIFTUSD_NOEXCEPT SWIFT_RETURNS_INDEPENDENT_VALUE {
#if defined(USE_PIXAR_USD)
        if (!_field) return "";
        PXR_NS::SdfAssetPath assetPath;
        _field.GetFilePathAttr().Get(&assetPath);
        _cachedFilePath = assetPath.GetAssetPath();
        return _cachedFilePath.c_str();
#else
        return _filePath.c_str();
#endif
    }

    bool SetFilePath(const char* filePath, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
        if (!filePath) return false;
#if defined(USE_PIXAR_USD)
        if (!_field) return false;
        return _field.GetFilePathAttr().Set(PXR_NS::SdfAssetPath(filePath), time.ToUsd());
#else
        _filePath = filePath;
        return true;
#endif
    }

    // Field name - inherited from FieldAsset
    Token GetFieldName(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_field) return Token();
        PXR_NS::TfToken fieldName;
        _field.GetFieldNameAttr().Get(&fieldName, time.ToUsd());
        return Token(fieldName);
#else
        return _fieldName;
#endif
    }

    bool SetFieldName(const Token& fieldName, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_field) return false;
        return _field.GetFieldNameAttr().Set(fieldName.GetTfToken(), time.ToUsd());
#else
        _fieldName = fieldName;
        return true;
#endif
    }

    // Field index - inherited from FieldAsset
    int GetFieldIndex(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_field) return 0;
        int fieldIndex = 0;
        _field.GetFieldIndexAttr().Get(&fieldIndex, time.ToUsd());
        return fieldIndex;
#else
        return _fieldIndex;
#endif
    }

    bool SetFieldIndex(int fieldIndex, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_field) return false;
        return _field.GetFieldIndexAttr().Set(fieldIndex, time.ToUsd());
#else
        _fieldIndex = fieldIndex;
        return true;
#endif
    }

    // Field data type - Field3D supports: half, float, double, half3, float3, double3
    Token GetFieldDataType(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_field) return Token();
        PXR_NS::TfToken dataType;
        _field.GetFieldDataTypeAttr().Get(&dataType, time.ToUsd());
        return Token(dataType);
#else
        return _fieldDataType;
#endif
    }

    bool SetFieldDataType(const Token& dataType, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_field) return false;
        return _field.GetFieldDataTypeAttr().Set(dataType.GetTfToken(), time.ToUsd());
#else
        _fieldDataType = dataType;
        return true;
#endif
    }

    // Vector data role hint - inherited from FieldAsset
    VectorDataRoleHint GetVectorDataRoleHint(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_field) return VectorDataRoleHint();
        PXR_NS::TfToken roleHint;
        _field.GetVectorDataRoleHintAttr().Get(&roleHint, time.ToUsd());
        return VectorDataRoleHint::FromUsd(roleHint);
#else
        return _vectorDataRoleHint;
#endif
    }

    bool SetVectorDataRoleHint(VectorDataRoleHint roleHint, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_field) return false;
        return _field.GetVectorDataRoleHintAttr().Set(roleHint.ToUsd(), time.ToUsd());
#else
        _vectorDataRoleHint = roleHint;
        return true;
#endif
    }

    // Field purpose - Field3D-specific optional grouping/purpose token
    Token GetFieldPurpose(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_field) return Token();
        PXR_NS::TfToken purpose;
        _field.GetFieldPurposeAttr().Get(&purpose, time.ToUsd());
        return Token(purpose);
#else
        return _fieldPurpose;
#endif
    }

    bool SetFieldPurpose(const Token& purpose, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_field) return false;
        return _field.GetFieldPurposeAttr().Set(purpose.GetTfToken(), time.ToUsd());
#else
        _fieldPurpose = purpose;
        return true;
#endif
    }

    // Reference counting
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
#if defined(USE_PIXAR_USD)
    VolField3DAssetRef(const PXR_NS::UsdVolField3DAsset& field, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _field(field), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~VolField3DAssetRef() {
        if (_stage) _stage->release();
    }

    PXR_NS::UsdVolField3DAsset _field;
    mutable std::string _cachedFilePath;
#else
    VolField3DAssetRef(const Path& path, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path), _stage(stage),
          _fieldIndex(0), _vectorDataRoleHint(),
          _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~VolField3DAssetRef() {
        if (_stage) _stage->release();
    }

    Path _path;
    std::string _filePath;
    Token _fieldName;
    int _fieldIndex;
    Token _fieldDataType;
    VectorDataRoleHint _vectorDataRoleHint;
    Token _fieldPurpose;
#endif

    StageRef* _stage;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

} // namespace swiftusd
