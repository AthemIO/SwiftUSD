// Usd.h - Universal Scene Description Core for SwiftUSD
// Mirrors: pxr/usd/usd/stage.h, pxr/usd/usd/prim.h, pxr/usd/usd/attribute.h, pxr/usd/usd/timeCode.h
// C++ header with Swift interop annotations

#pragma once

#include "../Swift.h"
#include "../base/Tf.h"
#include "../base/Gf.h"
#include "../base/Vt.h"
#include "Sdf.h"

#include <atomic>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <limits>

#if defined(USE_PIXAR_USD)
#include "pxr/pxr.h"
#include "pxr/usd/usd/stage.h"
#include "pxr/usd/usd/prim.h"
#include "pxr/usd/usd/attribute.h"
#include "pxr/usd/usd/relationship.h"
#include "pxr/usd/usd/timeCode.h"
#include "pxr/usd/usd/primRange.h"
#endif

namespace swiftusd {

// Forward declarations
class StageRef;
class PrimRef;
class AttributeRef;
class RelationshipRef;

// ============================================================================
// MARK: - TimeCode (UsdTimeCode value type)
// ============================================================================

/// A time code value used to sample attribute values.
///
/// TimeCode represents a time at which data can be queried. Special values
/// include Default (for non-animated data) and EarliestTime (for the earliest
/// available sample).
///
/// This is a value type (SWIFT_SELF_CONTAINED) - cheap to copy.
struct TimeCode {
    double value;

    // Constructors
    TimeCode() SWIFTUSD_NOEXCEPT : value(std::numeric_limits<double>::quiet_NaN()) {}

    TimeCode(double v) SWIFTUSD_NOEXCEPT : value(v) {}

#if defined(USE_PIXAR_USD)
    TimeCode(const PXR_NS::UsdTimeCode& tc) SWIFTUSD_NOEXCEPT {
        if (tc.IsDefault()) {
            value = std::numeric_limits<double>::quiet_NaN();
        } else {
            value = tc.GetValue();
        }
    }

    PXR_NS::UsdTimeCode ToUsd() const SWIFTUSD_NOEXCEPT {
        if (IsDefault()) {
            return PXR_NS::UsdTimeCode::Default();
        }
        return PXR_NS::UsdTimeCode(value);
    }
#endif

    // Static factories
    static TimeCode Default() SWIFTUSD_NOEXCEPT {
        return TimeCode();  // NaN represents default
    }

    static TimeCode EarliestTime() SWIFTUSD_NOEXCEPT {
        return TimeCode(-std::numeric_limits<double>::max());
    }

    // Query methods
    bool IsDefault() const SWIFTUSD_NOEXCEPT {
        return std::isnan(value);
    }

    bool IsEarliestTime() const SWIFTUSD_NOEXCEPT {
        return value == -std::numeric_limits<double>::max();
    }

    bool IsNumeric() const SWIFTUSD_NOEXCEPT {
        return !std::isnan(value);
    }

    double GetValue() const SWIFTUSD_NOEXCEPT {
        return value;
    }

    // Comparison operators
    bool operator==(const TimeCode& other) const SWIFTUSD_NOEXCEPT {
        if (IsDefault() && other.IsDefault()) return true;
        if (IsDefault() || other.IsDefault()) return false;
        return value == other.value;
    }

    bool operator!=(const TimeCode& other) const SWIFTUSD_NOEXCEPT {
        return !(*this == other);
    }

    bool operator<(const TimeCode& other) const SWIFTUSD_NOEXCEPT {
        if (IsDefault()) return false;  // Default is "no time", not comparable
        if (other.IsDefault()) return true;
        return value < other.value;
    }

    bool operator<=(const TimeCode& other) const SWIFTUSD_NOEXCEPT {
        return *this == other || *this < other;
    }

    bool operator>(const TimeCode& other) const SWIFTUSD_NOEXCEPT {
        return other < *this;
    }

    bool operator>=(const TimeCode& other) const SWIFTUSD_NOEXCEPT {
        return *this == other || *this > other;
    }

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - StageRef (UsdStage reference type)
// ============================================================================

/// A container for scene description data.
///
/// StageRef is the main entry point for working with USD data. It provides
/// methods to create, open, and manipulate stages, as well as access to
/// prims, layers, and other stage-level functionality.
///
/// This is a reference-counted type.
class StageRef {
public:
    // Factory methods - return new StageRef* that caller owns
    static StageRef* CreateNew(const char* identifier) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        auto stage = PXR_NS::UsdStage::CreateNew(identifier ? identifier : "");
        if (!stage) return nullptr;
        return new StageRef(stage);
#else
        if (!identifier || !*identifier) return nullptr;
        return new StageRef(identifier, false);
#endif
    }

    static StageRef* CreateInMemory(const char* identifier = nullptr) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        PXR_NS::UsdStageRefPtr stage;
        if (identifier && *identifier) {
            stage = PXR_NS::UsdStage::CreateInMemory(identifier);
        } else {
            stage = PXR_NS::UsdStage::CreateInMemory();
        }
        if (!stage) return nullptr;
        return new StageRef(stage);
#else
        std::string id = identifier ? identifier : "anon:stage";
        return new StageRef(id, true);
#endif
    }

    static StageRef* Open(const char* path) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        auto stage = PXR_NS::UsdStage::Open(path ? path : "");
        if (!stage) return nullptr;
        return new StageRef(stage);
#else
        if (!path || !*path) return nullptr;
        return new StageRef(path, false);
#endif
    }

    // Prim access
    PrimRef* GetPrimAtPath(const Path& path) SWIFTUSD_NOEXCEPT;
    PrimRef* GetPseudoRoot() SWIFTUSD_NOEXCEPT;
    PrimRef* GetDefaultPrim() SWIFTUSD_NOEXCEPT;
    PrimRef* DefinePrim(const Path& path, const Token& typeName = Token()) SWIFTUSD_NOEXCEPT;
    PrimRef* OverridePrim(const Path& path) SWIFTUSD_NOEXCEPT;
    bool RemovePrim(const Path& path) SWIFTUSD_NOEXCEPT;

    // Stage properties
    double GetStartTimeCode() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _stage ? _stage->GetStartTimeCode() : 0.0;
#else
        return _startTimeCode;
#endif
    }

    double GetEndTimeCode() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _stage ? _stage->GetEndTimeCode() : 0.0;
#else
        return _endTimeCode;
#endif
    }

    double GetTimeCodesPerSecond() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _stage ? _stage->GetTimeCodesPerSecond() : 24.0;
#else
        return _timeCodesPerSecond;
#endif
    }

    double GetFramesPerSecond() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _stage ? _stage->GetFramesPerSecond() : 24.0;
#else
        return _framesPerSecond;
#endif
    }

    void SetStartTimeCode(double time) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_stage) _stage->SetStartTimeCode(time);
#else
        _startTimeCode = time;
#endif
    }

    void SetEndTimeCode(double time) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_stage) _stage->SetEndTimeCode(time);
#else
        _endTimeCode = time;
#endif
    }

    void SetTimeCodesPerSecond(double tcps) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_stage) _stage->SetTimeCodesPerSecond(tcps);
#else
        _timeCodesPerSecond = tcps;
#endif
    }

    void SetFramesPerSecond(double fps) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_stage) _stage->SetFramesPerSecond(fps);
#else
        _framesPerSecond = fps;
#endif
    }

    // Default prim
    void SetDefaultPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT;

    bool HasDefaultPrim() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _stage && _stage->HasDefaultPrim();
#else
        return !_defaultPrimPath.IsEmpty();
#endif
    }

    void ClearDefaultPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_stage) _stage->ClearDefaultPrim();
#else
        _defaultPrimPath = Path();
#endif
    }

    // Layers
    LayerRef* GetRootLayer() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_stage) return nullptr;
        auto layer = _stage->GetRootLayer();
        if (!layer) return nullptr;
        return new LayerRef(layer);
#else
        if (!_rootLayer) {
            _rootLayer = LayerRef::CreateAnonymous("rootLayer");
        }
        if (_rootLayer) _rootLayer->retain();
        return _rootLayer;
#endif
    }

    LayerRef* GetSessionLayer() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_stage) return nullptr;
        auto layer = _stage->GetSessionLayer();
        if (!layer) return nullptr;
        return new LayerRef(layer);
#else
        if (!_sessionLayer) {
            _sessionLayer = LayerRef::CreateAnonymous("sessionLayer");
        }
        if (_sessionLayer) _sessionLayer->retain();
        return _sessionLayer;
#endif
    }

    // I/O
    bool Save() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_stage) return false;
        _stage->Save();
        return true;
#else
        return true;
#endif
    }

    bool Export(const char* path, bool addSourceReference = false) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_stage || !path) return false;
        return _stage->Export(path, addSourceReference);
#else
        return path != nullptr;
#endif
    }

    // Flatten
    LayerRef* Flatten(bool addSourceFileComment = true) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_stage) return nullptr;
        auto layer = _stage->Flatten(addSourceFileComment);
        if (!layer) return nullptr;
        return new LayerRef(layer);
#else
        return LayerRef::CreateAnonymous("flattened");
#endif
    }

    // Reload
    void Reload() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_stage) _stage->Reload();
#endif
    }

    // Traversal - calls callback for each prim
    void Traverse(void (*callback)(PrimRef*, void*), void* context) SWIFTUSD_NOEXCEPT;

    // Get all root prims
    size_t GetRootPrimCount() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_stage) return 0;
        size_t count = 0;
        for (auto prim : _stage->GetPseudoRoot().GetChildren()) {
            (void)prim;
            ++count;
        }
        return count;
#else
        return _prims.size();
#endif
    }

    // Identifier
    SWIFT_RETURNS_INDEPENDENT_VALUE
    const char* GetIdentifier() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_stage) return "";
        _identifierCache = _stage->GetRootLayer()->GetIdentifier();
        return _identifierCache.c_str();
#else
        return _identifier.c_str();
#endif
    }

#if defined(USE_PIXAR_USD)
    PXR_NS::UsdStageRefPtr GetUsdStage() const SWIFTUSD_NOEXCEPT { return _stage; }
#endif

    // Reference counting for SWIFT_SHARED_REFERENCE
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
    explicit StageRef(const PXR_NS::UsdStageRefPtr& stage) SWIFTUSD_NOEXCEPT
        : _stage(stage), _refCount(1) {}

    PXR_NS::UsdStageRefPtr _stage;
    mutable std::string _identifierCache;
#else
    struct PrimData {
        std::string name;
        Token typeName;
        bool isActive;
        bool isDefined;
        std::map<std::string, PrimData> children;
        std::map<std::string, std::pair<Token, std::string>> attributes; // name -> (typeName, value as string)

        PrimData() : isActive(true), isDefined(true) {}
    };

    StageRef(const std::string& identifier, bool isInMemory) SWIFTUSD_NOEXCEPT
        : _identifier(identifier),
          _isInMemory(isInMemory),
          _startTimeCode(0.0),
          _endTimeCode(0.0),
          _timeCodesPerSecond(24.0),
          _framesPerSecond(24.0),
          _rootLayer(nullptr),
          _sessionLayer(nullptr),
          _refCount(1) {}

    ~StageRef() {
        if (_rootLayer) _rootLayer->release();
        if (_sessionLayer) _sessionLayer->release();
    }

    std::string _identifier;
    bool _isInMemory;
    double _startTimeCode;
    double _endTimeCode;
    double _timeCodesPerSecond;
    double _framesPerSecond;
    Path _defaultPrimPath;
    std::map<std::string, PrimData> _prims;  // Root prims by path
    LayerRef* _rootLayer;
    LayerRef* _sessionLayer;
#endif

    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - PrimRef (UsdPrim reference type)
// ============================================================================

/// A reference to a prim in a USD stage.
///
/// PrimRef provides access to prim metadata, properties, relationships,
/// and hierarchical navigation. Prims are the primary organizational
/// unit in a USD stage.
///
/// This is a reference-counted type.
class PrimRef {
public:
    // Identity
    Path GetPath() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return Path(_prim.GetPath());
#else
        return _path;
#endif
    }

    Token GetName() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return Token(_prim.GetName());
#else
        return _path.GetNameToken();
#endif
    }

    Token GetTypeName() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return Token(_prim.GetTypeName());
#else
        return _typeName;
#endif
    }

    // State
    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _prim.IsValid();
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    bool IsActive() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _prim.IsActive();
#else
        return _isActive;
#endif
    }

    bool IsDefined() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _prim.IsDefined();
#else
        return _isDefined;
#endif
    }

    bool IsAbstract() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _prim.IsAbstract();
#else
        return _isAbstract;
#endif
    }

    bool IsModel() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _prim.IsModel();
#else
        return false;  // Simplified for standalone
#endif
    }

    bool IsGroup() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _prim.IsGroup();
#else
        return false;  // Simplified for standalone
#endif
    }

    void SetActive(bool active) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        _prim.SetActive(active);
#else
        _isActive = active;
#endif
    }

    // Type checking
    bool IsA(const Token& schemaType) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _prim.IsA(PXR_NS::TfType::FindByName(schemaType.GetTfToken().GetString()));
#else
        // Simplified type checking - just compare type names
        return _typeName == schemaType;
#endif
    }

    bool HasAPI(const Token& apiSchema) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _prim.HasAPI(PXR_NS::TfType::FindByName(apiSchema.GetTfToken().GetString()));
#else
        return false;  // Simplified for standalone
#endif
    }

    // Hierarchy
    PrimRef* GetParent() SWIFTUSD_NOEXCEPT;

    size_t GetChildCount() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        size_t count = 0;
        for (auto child : _prim.GetChildren()) {
            (void)child;
            ++count;
        }
        return count;
#else
        return _children.size();
#endif
    }

    void GetChildren(PrimRef** outChildren, size_t maxCount) SWIFTUSD_NOEXCEPT;

    PrimRef* GetChild(const Token& name) SWIFTUSD_NOEXCEPT;

    // Get all children as a contiguous array (caller must free)
    size_t GetAllChildren(PrimRef*** outChildren) SWIFTUSD_NOEXCEPT;

    // Attributes
    AttributeRef* GetAttribute(const Token& name) SWIFTUSD_NOEXCEPT;

    AttributeRef* CreateAttribute(const Token& name, const Token& typeName,
                                   bool custom = true) SWIFTUSD_NOEXCEPT;

    bool HasAttribute(const Token& name) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _prim.HasAttribute(name.GetTfToken());
#else
        return _attributes.find(std::string(name.GetText())) != _attributes.end();
#endif
    }

    size_t GetAttributeCount() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _prim.GetAttributes().size();
#else
        return _attributes.size();
#endif
    }

    // Relationships
    RelationshipRef* GetRelationship(const Token& name) SWIFTUSD_NOEXCEPT;

    RelationshipRef* CreateRelationship(const Token& name, bool custom = true) SWIFTUSD_NOEXCEPT;

    bool HasRelationship(const Token& name) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _prim.HasRelationship(name.GetTfToken());
#else
        return _relationships.find(std::string(name.GetText())) != _relationships.end();
#endif
    }

    // Metadata
    bool HasMetadata(const Token& key) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _prim.HasMetadata(key.GetTfToken());
#else
        return _metadata.find(std::string(key.GetText())) != _metadata.end();
#endif
    }

    Token GetKind() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        PXR_NS::TfToken kind;
        _prim.GetMetadata(PXR_NS::SdfFieldKeys->Kind, &kind);
        return Token(kind);
#else
        return _kind;
#endif
    }

    bool SetKind(const Token& kind) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _prim.SetMetadata(PXR_NS::SdfFieldKeys->Kind, kind.GetTfToken());
#else
        _kind = kind;
        return true;
#endif
    }

    // Stage access
    StageRef* GetStage() SWIFTUSD_NOEXCEPT {
        if (_stage) _stage->retain();
        return _stage;
    }

#if defined(USE_PIXAR_USD)
    const PXR_NS::UsdPrim& GetUsdPrim() const SWIFTUSD_NOEXCEPT { return _prim; }
#endif

    // Reference counting for SWIFT_SHARED_REFERENCE
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
    friend class StageRef;
    friend class AttributeRef;
    friend class RelationshipRef;

#if defined(USE_PIXAR_USD)
    PrimRef(const PXR_NS::UsdPrim& prim, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _prim(prim), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~PrimRef() {
        if (_stage) _stage->release();
    }

    PXR_NS::UsdPrim _prim;
#else
    struct AttributeData {
        Token typeName;
        bool custom;
        std::map<double, std::string> timeSamples;  // time -> value as string
        std::string defaultValue;

        AttributeData() : custom(true) {}
    };

    struct RelationshipData {
        bool custom;
        std::vector<Path> targets;

        RelationshipData() : custom(true) {}
    };

    PrimRef(const Path& path, const Token& typeName, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path),
          _typeName(typeName),
          _isActive(true),
          _isDefined(true),
          _isAbstract(false),
          _stage(stage),
          _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~PrimRef() {
        if (_stage) _stage->release();
    }

    Path _path;
    Token _typeName;
    bool _isActive;
    bool _isDefined;
    bool _isAbstract;
    Token _kind;
    std::vector<PrimRef*> _children;
    std::map<std::string, AttributeData> _attributes;
    std::map<std::string, RelationshipData> _relationships;
    std::map<std::string, std::string> _metadata;
#endif

    StageRef* _stage;  // Non-owning in USE_PIXAR_USD, owning in standalone
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - AttributeRef (UsdAttribute reference type)
// ============================================================================

/// A reference to an attribute on a prim.
///
/// AttributeRef provides typed access to attribute values, with support
/// for time-sampled and default values.
///
/// This is a reference-counted type.
class AttributeRef {
public:
    Token GetName() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return Token(_attr.GetName());
#else
        return _name;
#endif
    }

    Token GetTypeName() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return Token(_attr.GetTypeName().GetAsToken());
#else
        return _typeName;
#endif
    }

    Path GetPath() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return Path(_attr.GetPath());
#else
        return _prim ? _prim->GetPath().AppendProperty(_name) : Path();
#endif
    }

    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _attr.IsValid();
#else
        return _prim != nullptr;
#endif
    }

    bool IsDefined() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _attr.IsDefined();
#else
        return _prim != nullptr;
#endif
    }

    bool IsAuthored() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _attr.IsAuthored();
#else
        return _hasValue;
#endif
    }

    bool IsCustom() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _attr.IsCustom();
#else
        return _isCustom;
#endif
    }

    bool HasValue() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _attr.HasValue();
#else
        return _hasValue;
#endif
    }

    // Value access (typed getters)
    bool GetBool(TimeCode time, bool* outValue) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _attr.Get(outValue, time.ToUsd());
#else
        if (!outValue || !_hasValue) return false;
        *outValue = _boolValue;
        return true;
#endif
    }

    bool GetInt(TimeCode time, int* outValue) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _attr.Get(outValue, time.ToUsd());
#else
        if (!outValue || !_hasValue) return false;
        *outValue = _intValue;
        return true;
#endif
    }

    bool GetFloat(TimeCode time, float* outValue) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _attr.Get(outValue, time.ToUsd());
#else
        if (!outValue || !_hasValue) return false;
        *outValue = _floatValue;
        return true;
#endif
    }

    bool GetDouble(TimeCode time, double* outValue) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _attr.Get(outValue, time.ToUsd());
#else
        if (!outValue || !_hasValue) return false;
        *outValue = _doubleValue;
        return true;
#endif
    }

    bool GetString(TimeCode time, const char** outValue) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        std::string val;
        if (!_attr.Get(&val, time.ToUsd())) return false;
        _stringCache = val;
        *outValue = _stringCache.c_str();
        return true;
#else
        if (!outValue || !_hasValue) return false;
        *outValue = _stringValue.c_str();
        return true;
#endif
    }

    bool GetToken(TimeCode time, Token* outValue) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        PXR_NS::TfToken val;
        if (!_attr.Get(&val, time.ToUsd())) return false;
        *outValue = Token(val);
        return true;
#else
        if (!outValue || !_hasValue) return false;
        *outValue = _tokenValue;
        return true;
#endif
    }

    bool GetVec3f(TimeCode time, Vec3f* outValue) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        PXR_NS::GfVec3f val;
        if (!_attr.Get(&val, time.ToUsd())) return false;
        *outValue = Vec3f(val);
        return true;
#else
        if (!outValue || !_hasValue) return false;
        *outValue = _vec3fValue;
        return true;
#endif
    }

    bool GetVec3d(TimeCode time, Vec3d* outValue) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        PXR_NS::GfVec3d val;
        if (!_attr.Get(&val, time.ToUsd())) return false;
        *outValue = Vec3d(val);
        return true;
#else
        if (!outValue || !_hasValue) return false;
        *outValue = _vec3dValue;
        return true;
#endif
    }

    bool GetMatrix4d(TimeCode time, Matrix4d* outValue) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        PXR_NS::GfMatrix4d val;
        if (!_attr.Get(&val, time.ToUsd())) return false;
        *outValue = Matrix4d(val);
        return true;
#else
        if (!outValue || !_hasValue) return false;
        *outValue = _matrix4dValue;
        return true;
#endif
    }

    // Array access
    Vec3fArray* GetVec3fArray(TimeCode time) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        PXR_NS::VtArray<PXR_NS::GfVec3f> val;
        if (!_attr.Get(&val, time.ToUsd())) return nullptr;
        return new Vec3fArray(val);
#else
        return _vec3fArrayValue ? new Vec3fArray(*_vec3fArrayValue) : nullptr;
#endif
    }

    IntArray* GetIntArray(TimeCode time) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        PXR_NS::VtArray<int> val;
        if (!_attr.Get(&val, time.ToUsd())) return nullptr;
        return new IntArray(val);
#else
        return _intArrayValue ? new IntArray(*_intArrayValue) : nullptr;
#endif
    }

    FloatArray* GetFloatArray(TimeCode time) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        PXR_NS::VtArray<float> val;
        if (!_attr.Get(&val, time.ToUsd())) return nullptr;
        return new FloatArray(val);
#else
        return _floatArrayValue ? new FloatArray(*_floatArrayValue) : nullptr;
#endif
    }

    // Value setters
    bool SetBool(bool value, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _attr.Set(value, time.ToUsd());
#else
        _boolValue = value;
        _hasValue = true;
        return true;
#endif
    }

    bool SetInt(int value, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _attr.Set(value, time.ToUsd());
#else
        _intValue = value;
        _hasValue = true;
        return true;
#endif
    }

    bool SetFloat(float value, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _attr.Set(value, time.ToUsd());
#else
        _floatValue = value;
        _hasValue = true;
        return true;
#endif
    }

    bool SetDouble(double value, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _attr.Set(value, time.ToUsd());
#else
        _doubleValue = value;
        _hasValue = true;
        return true;
#endif
    }

    bool SetString(const char* value, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _attr.Set(std::string(value ? value : ""), time.ToUsd());
#else
        _stringValue = value ? value : "";
        _hasValue = true;
        return true;
#endif
    }

    bool SetToken(Token value, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _attr.Set(value.GetTfToken(), time.ToUsd());
#else
        _tokenValue = value;
        _hasValue = true;
        return true;
#endif
    }

    bool SetVec3f(Vec3f value, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _attr.Set(value.ToGf(), time.ToUsd());
#else
        _vec3fValue = value;
        _hasValue = true;
        return true;
#endif
    }

    bool SetVec3d(Vec3d value, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _attr.Set(value.ToGf(), time.ToUsd());
#else
        _vec3dValue = value;
        _hasValue = true;
        return true;
#endif
    }

    bool SetMatrix4d(Matrix4d value, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _attr.Set(value.ToGf(), time.ToUsd());
#else
        _matrix4dValue = value;
        _hasValue = true;
        return true;
#endif
    }

    bool SetVec3fArray(Vec3fArray* array, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!array) return false;
        return _attr.Set(array->ToVt(), time.ToUsd());
#else
        if (!array) return false;
        _vec3fArrayValue = std::make_unique<Vec3fArray>(*array);
        _hasValue = true;
        return true;
#endif
    }

    bool SetIntArray(IntArray* array, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!array) return false;
        return _attr.Set(array->ToVt(), time.ToUsd());
#else
        if (!array) return false;
        _intArrayValue = std::make_unique<IntArray>(*array);
        _hasValue = true;
        return true;
#endif
    }

    bool SetFloatArray(FloatArray* array, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!array) return false;
        return _attr.Set(array->ToVt(), time.ToUsd());
#else
        if (!array) return false;
        _floatArrayValue = std::make_unique<FloatArray>(*array);
        _hasValue = true;
        return true;
#endif
    }

    // Clear value
    bool Clear() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _attr.Clear();
#else
        _hasValue = false;
        return true;
#endif
    }

    // Prim access
    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
        if (_prim) _prim->retain();
        return _prim;
    }

    // Reference counting for SWIFT_SHARED_REFERENCE
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
    friend class PrimRef;
    friend class ShadeInputRef;
    friend class ShadeOutputRef;
    friend class ShadeShaderRef;
    friend class ShadeNodeGraphRef;
    friend class ShadeMaterialRef;

#if defined(USE_PIXAR_USD)
    AttributeRef(const PXR_NS::UsdAttribute& attr, PrimRef* prim) SWIFTUSD_NOEXCEPT
        : _attr(attr), _prim(prim), _refCount(1) {
        if (_prim) _prim->retain();
    }

    ~AttributeRef() {
        if (_prim) _prim->release();
    }

    PXR_NS::UsdAttribute _attr;
    mutable std::string _stringCache;
#else
    AttributeRef(const Token& name, const Token& typeName, PrimRef* prim, bool custom) SWIFTUSD_NOEXCEPT
        : _name(name),
          _typeName(typeName),
          _prim(prim),
          _isCustom(custom),
          _hasValue(false),
          _boolValue(false),
          _intValue(0),
          _floatValue(0.0f),
          _doubleValue(0.0),
          _refCount(1) {
        if (_prim) _prim->retain();
    }

    ~AttributeRef() {
        if (_prim) _prim->release();
    }

    Token _name;
    Token _typeName;
    bool _isCustom;
    bool _hasValue;

    // Value storage (simplified - in real implementation would use variant)
    bool _boolValue;
    int _intValue;
    float _floatValue;
    double _doubleValue;
    std::string _stringValue;
    Token _tokenValue;
    Vec3f _vec3fValue;
    Vec3d _vec3dValue;
    Matrix4d _matrix4dValue;
    std::unique_ptr<Vec3fArray> _vec3fArrayValue;
    std::unique_ptr<IntArray> _intArrayValue;
    std::unique_ptr<FloatArray> _floatArrayValue;
#endif

    PrimRef* _prim;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - RelationshipRef (UsdRelationship reference type)
// ============================================================================

/// A reference to a relationship on a prim.
///
/// RelationshipRef provides access to prim path targets, allowing
/// prims to reference other prims in the stage.
///
/// This is a reference-counted type.
class RelationshipRef {
public:
    Token GetName() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return Token(_rel.GetName());
#else
        return _name;
#endif
    }

    Path GetPath() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return Path(_rel.GetPath());
#else
        return _prim ? _prim->GetPath().AppendProperty(_name) : Path();
#endif
    }

    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _rel.IsValid();
#else
        return _prim != nullptr;
#endif
    }

    bool IsCustom() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _rel.IsCustom();
#else
        return _isCustom;
#endif
    }

    // Target access
    size_t GetTargetCount() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        PXR_NS::SdfPathVector targets;
        _rel.GetTargets(&targets);
        return targets.size();
#else
        return _targets.size();
#endif
    }

    void GetTargets(Path* outTargets, size_t maxCount) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        PXR_NS::SdfPathVector targets;
        _rel.GetTargets(&targets);
        size_t count = std::min(targets.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outTargets[i] = Path(targets[i]);
        }
#else
        size_t count = std::min(_targets.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outTargets[i] = _targets[i];
        }
#endif
    }

    bool AddTarget(const Path& target, bool position = false) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _rel.AddTarget(target.GetSdfPath(),
            position ? PXR_NS::UsdListPositionFrontOfAppendList : PXR_NS::UsdListPositionBackOfAppendList);
#else
        _targets.push_back(target);
        return true;
#endif
    }

    bool RemoveTarget(const Path& target) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _rel.RemoveTarget(target.GetSdfPath());
#else
        auto it = std::find_if(_targets.begin(), _targets.end(),
            [&target](const Path& p) { return p == target; });
        if (it != _targets.end()) {
            _targets.erase(it);
            return true;
        }
        return false;
#endif
    }

    bool SetTargets(const Path* targets, size_t count) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        PXR_NS::SdfPathVector pathVec;
        pathVec.reserve(count);
        for (size_t i = 0; i < count; ++i) {
            pathVec.push_back(targets[i].GetSdfPath());
        }
        return _rel.SetTargets(pathVec);
#else
        _targets.clear();
        _targets.reserve(count);
        for (size_t i = 0; i < count; ++i) {
            _targets.push_back(targets[i]);
        }
        return true;
#endif
    }

    bool ClearTargets(bool removeSpec) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _rel.ClearTargets(removeSpec);
#else
        _targets.clear();
        return true;
#endif
    }

    // Prim access
    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
        if (_prim) _prim->retain();
        return _prim;
    }

    // Reference counting for SWIFT_SHARED_REFERENCE
    void retain() SWIFTUSD_NOEXCEPT {
        _refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }

private:
    friend class PrimRef;

#if defined(USE_PIXAR_USD)
    RelationshipRef(const PXR_NS::UsdRelationship& rel, PrimRef* prim) SWIFTUSD_NOEXCEPT
        : _rel(rel), _prim(prim), _refCount(1) {
        if (_prim) _prim->retain();
    }

    ~RelationshipRef() {
        if (_prim) _prim->release();
    }

    PXR_NS::UsdRelationship _rel;
#else
    RelationshipRef(const Token& name, PrimRef* prim, bool custom) SWIFTUSD_NOEXCEPT
        : _name(name), _prim(prim), _isCustom(custom), _refCount(1) {
        if (_prim) _prim->retain();
    }

    ~RelationshipRef() {
        if (_prim) _prim->release();
    }

    Token _name;
    bool _isCustom;
    std::vector<Path> _targets;
#endif

    PrimRef* _prim;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - StageRef Method Implementations
// ============================================================================

inline PrimRef* StageRef::GetPrimAtPath(const Path& path) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    if (!_stage) return nullptr;
    auto prim = _stage->GetPrimAtPath(path.GetSdfPath());
    if (!prim.IsValid()) return nullptr;
    return new PrimRef(prim, this);
#else
    // Simplified lookup - would need proper path parsing in real implementation
    const char* pathStr = path.GetText();
    if (!pathStr || pathStr[0] != '/') return nullptr;

    // For now, just return a new prim at the requested path
    auto it = _prims.find(std::string(pathStr));
    if (it != _prims.end()) {
        return new PrimRef(path, Token(it->second.typeName), this);
    }
    return nullptr;
#endif
}

inline PrimRef* StageRef::GetPseudoRoot() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    if (!_stage) return nullptr;
    return new PrimRef(_stage->GetPseudoRoot(), this);
#else
    return new PrimRef(Path::AbsoluteRoot(), Token(), this);
#endif
}

inline PrimRef* StageRef::GetDefaultPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    if (!_stage || !_stage->HasDefaultPrim()) return nullptr;
    return new PrimRef(_stage->GetDefaultPrim(), this);
#else
    if (_defaultPrimPath.IsEmpty()) return nullptr;
    return GetPrimAtPath(_defaultPrimPath);
#endif
}

inline PrimRef* StageRef::DefinePrim(const Path& path, const Token& typeName) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    if (!_stage) return nullptr;
    auto prim = _stage->DefinePrim(path.GetSdfPath(), typeName.GetTfToken());
    if (!prim.IsValid()) return nullptr;
    return new PrimRef(prim, this);
#else
    std::string pathStr(path.GetText());
    PrimData data;
    data.name = path.GetName();
    data.typeName = typeName;
    data.isActive = true;
    data.isDefined = true;
    _prims[pathStr] = data;
    return new PrimRef(path, typeName, this);
#endif
}

inline PrimRef* StageRef::OverridePrim(const Path& path) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    if (!_stage) return nullptr;
    auto prim = _stage->OverridePrim(path.GetSdfPath());
    if (!prim.IsValid()) return nullptr;
    return new PrimRef(prim, this);
#else
    std::string pathStr(path.GetText());
    PrimData data;
    data.name = path.GetName();
    data.isActive = true;
    data.isDefined = false;
    _prims[pathStr] = data;
    return new PrimRef(path, Token(), this);
#endif
}

inline bool StageRef::RemovePrim(const Path& path) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    if (!_stage) return false;
    return _stage->RemovePrim(path.GetSdfPath());
#else
    std::string pathStr(path.GetText());
    return _prims.erase(pathStr) > 0;
#endif
}

inline void StageRef::SetDefaultPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    if (_stage && prim) {
        _stage->SetDefaultPrim(prim->GetUsdPrim());
    }
#else
    if (prim) {
        _defaultPrimPath = prim->GetPath();
    }
#endif
}

inline void StageRef::Traverse(void (*callback)(PrimRef*, void*), void* context) SWIFTUSD_NOEXCEPT {
    if (!callback) return;

#if defined(USE_PIXAR_USD)
    if (!_stage) return;
    for (auto prim : _stage->Traverse()) {
        PrimRef* primRef = new PrimRef(prim, this);
        callback(primRef, context);
        primRef->release();
    }
#else
    for (const auto& pair : _prims) {
        Path path(pair.first);
        PrimRef* primRef = new PrimRef(path, pair.second.typeName, this);
        callback(primRef, context);
        primRef->release();
    }
#endif
}

// ============================================================================
// MARK: - PrimRef Method Implementations
// ============================================================================

inline PrimRef* PrimRef::GetParent() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    auto parent = _prim.GetParent();
    if (!parent.IsValid()) return nullptr;
    return new PrimRef(parent, _stage);
#else
    Path parentPath = _path.GetParentPath();
    if (parentPath.IsEmpty()) return nullptr;
    return _stage ? _stage->GetPrimAtPath(parentPath) : nullptr;
#endif
}

inline void PrimRef::GetChildren(PrimRef** outChildren, size_t maxCount) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    size_t i = 0;
    for (auto child : _prim.GetChildren()) {
        if (i >= maxCount) break;
        outChildren[i++] = new PrimRef(child, _stage);
    }
#else
    size_t count = std::min(_children.size(), maxCount);
    for (size_t i = 0; i < count; ++i) {
        _children[i]->retain();
        outChildren[i] = _children[i];
    }
#endif
}

inline PrimRef* PrimRef::GetChild(const Token& name) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    auto child = _prim.GetChild(name.GetTfToken());
    if (!child.IsValid()) return nullptr;
    return new PrimRef(child, _stage);
#else
    for (auto* child : _children) {
        if (child->GetName() == name) {
            child->retain();
            return child;
        }
    }
    return nullptr;
#endif
}

inline size_t PrimRef::GetAllChildren(PrimRef*** outChildren) SWIFTUSD_NOEXCEPT {
    size_t count = GetChildCount();
    if (count == 0) {
        *outChildren = nullptr;
        return 0;
    }

    *outChildren = new PrimRef*[count];
    GetChildren(*outChildren, count);
    return count;
}

inline AttributeRef* PrimRef::GetAttribute(const Token& name) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    auto attr = _prim.GetAttribute(name.GetTfToken());
    if (!attr.IsValid()) return nullptr;
    return new AttributeRef(attr, this);
#else
    auto it = _attributes.find(std::string(name.GetText()));
    if (it == _attributes.end()) return nullptr;
    return new AttributeRef(name, it->second.typeName, this, it->second.custom);
#endif
}

inline AttributeRef* PrimRef::CreateAttribute(const Token& name, const Token& typeName,
                                                bool custom) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    auto sdfType = PXR_NS::SdfValueTypeNames->Find(typeName.GetTfToken().GetString());
    auto attr = _prim.CreateAttribute(name.GetTfToken(), sdfType, custom);
    if (!attr.IsValid()) return nullptr;
    return new AttributeRef(attr, this);
#else
    AttributeData data;
    data.typeName = typeName;
    data.custom = custom;
    _attributes[std::string(name.GetText())] = data;
    return new AttributeRef(name, typeName, this, custom);
#endif
}

inline RelationshipRef* PrimRef::GetRelationship(const Token& name) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    auto rel = _prim.GetRelationship(name.GetTfToken());
    if (!rel.IsValid()) return nullptr;
    return new RelationshipRef(rel, this);
#else
    auto it = _relationships.find(std::string(name.GetText()));
    if (it == _relationships.end()) return nullptr;
    return new RelationshipRef(name, this, it->second.custom);
#endif
}

inline RelationshipRef* PrimRef::CreateRelationship(const Token& name, bool custom) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
    auto rel = _prim.CreateRelationship(name.GetTfToken(), custom);
    if (!rel.IsValid()) return nullptr;
    return new RelationshipRef(rel, this);
#else
    RelationshipData data;
    data.custom = custom;
    _relationships[std::string(name.GetText())] = data;
    return new RelationshipRef(name, this, custom);
#endif
}

} // namespace swiftusd
