// Sdf.h - Scene Description Foundations for SwiftUSD
// Mirrors: pxr/usd/sdf/path.h, pxr/usd/sdf/layer.h
// C++ header with Swift interop annotations

#pragma once

#include "../Swift.h"
#include "../base/Tf.h"

#if defined(USE_PIXAR_USD)
#include "pxr/pxr.h"
#include "pxr/usd/sdf/path.h"
#include "pxr/usd/sdf/layer.h"
#include "pxr/usd/sdf/assetPath.h"
#endif

namespace swiftusd {

// Forward declarations
class LayerRef;

// ============================================================================
// MARK: - Path (SdfPath value type)
// ============================================================================

/// A path value used to locate objects in layers or scenegraphs.
///
/// Path is used in several ways:
/// - As a storage key for addressing and accessing values held in a layer
/// - As a namespace identity for scenegraph objects
/// - As a way to refer to other scenegraph objects through relative paths
///
/// Path components are separated by `/` for hierarchy and `.` for properties.
/// Examples:
/// - `/World` - absolute prim path
/// - `/World/Mesh` - nested prim path
/// - `/World/Mesh.points` - property path
///
/// This is a value type (SWIFT_SELF_CONTAINED) - cheap to copy.
struct Path {
#if defined(USE_PIXAR_USD)
    PXR_NS::SdfPath _path;
#else
    std::string _pathString;
#endif

    // Constructors
    Path() SWIFTUSD_NOEXCEPT
#if defined(USE_PIXAR_USD)
        : _path()
#else
        : _pathString()
#endif
    {}

    Path(const char* pathString) SWIFTUSD_NOEXCEPT
#if defined(USE_PIXAR_USD)
        : _path(pathString ? pathString : "")
#else
        : _pathString(pathString ? pathString : "")
#endif
    {}

    Path(const std::string& pathString) SWIFTUSD_NOEXCEPT
#if defined(USE_PIXAR_USD)
        : _path(pathString)
#else
        : _pathString(pathString)
#endif
    {}

#if defined(USE_PIXAR_USD)
    Path(const PXR_NS::SdfPath& path) SWIFTUSD_NOEXCEPT : _path(path) {}

    const PXR_NS::SdfPath& GetSdfPath() const SWIFTUSD_NOEXCEPT { return _path; }
#endif

    // Static factories
    static Path AbsoluteRoot() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return Path(PXR_NS::SdfPath::AbsoluteRootPath());
#else
        return Path("/");
#endif
    }

    static Path Empty() SWIFTUSD_NOEXCEPT {
        return Path();
    }

    static Path ReflexiveRelative() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return Path(PXR_NS::SdfPath::ReflexiveRelativePath());
#else
        return Path(".");
#endif
    }

    // Query methods
    SWIFT_RETURNS_INDEPENDENT_VALUE
    const char* GetText() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _path.GetText();
#else
        return _pathString.c_str();
#endif
    }

    SWIFT_RETURNS_INDEPENDENT_VALUE
    const char* GetName() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _path.GetName().c_str();
#else
        // Extract last component for standalone mode
        size_t lastSlash = _pathString.rfind('/');
        size_t lastDot = _pathString.rfind('.');
        size_t start = 0;
        if (lastDot != std::string::npos && (lastSlash == std::string::npos || lastDot > lastSlash)) {
            start = lastDot + 1;
        } else if (lastSlash != std::string::npos) {
            start = lastSlash + 1;
        }
        // Store in a static buffer for return (not thread-safe but simple)
        static thread_local std::string nameBuffer;
        nameBuffer = _pathString.substr(start);
        return nameBuffer.c_str();
#endif
    }

    Token GetNameToken() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return Token(_path.GetNameToken());
#else
        return Token(GetName());
#endif
    }

    bool IsEmpty() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _path.IsEmpty();
#else
        return _pathString.empty();
#endif
    }

    bool IsAbsolutePath() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _path.IsAbsolutePath();
#else
        return !_pathString.empty() && _pathString[0] == '/';
#endif
    }

    bool IsAbsoluteRootPath() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _path.IsAbsoluteRootPath();
#else
        return _pathString == "/";
#endif
    }

    bool IsPrimPath() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _path.IsPrimPath();
#else
        // A prim path contains only slashes (no dots for properties)
        return !_pathString.empty() && _pathString.find('.') == std::string::npos;
#endif
    }

    bool IsPropertyPath() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _path.IsPropertyPath();
#else
        return _pathString.find('.') != std::string::npos;
#endif
    }

    bool IsAbsoluteRootOrPrimPath() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _path.IsAbsoluteRootOrPrimPath();
#else
        return IsAbsoluteRootPath() || (IsAbsolutePath() && IsPrimPath());
#endif
    }

    bool IsRootPrimPath() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _path.IsRootPrimPath();
#else
        // Root prim is like "/Foo" (exactly one slash at start, no more)
        if (_pathString.size() < 2 || _pathString[0] != '/') return false;
        return _pathString.find('/', 1) == std::string::npos &&
               _pathString.find('.') == std::string::npos;
#endif
    }

    size_t GetPathElementCount() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _path.GetPathElementCount();
#else
        if (_pathString.empty()) return 0;
        size_t count = 0;
        for (char c : _pathString) {
            if (c == '/' || c == '.') count++;
        }
        return count;
#endif
    }

    // Navigation methods
    Path GetParentPath() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return Path(_path.GetParentPath());
#else
        if (_pathString.empty() || _pathString == "/") return Path();

        size_t lastSlash = _pathString.rfind('/');
        size_t lastDot = _pathString.rfind('.');

        if (lastDot != std::string::npos && (lastSlash == std::string::npos || lastDot > lastSlash)) {
            return Path(_pathString.substr(0, lastDot));
        } else if (lastSlash != std::string::npos) {
            if (lastSlash == 0) return Path("/");
            return Path(_pathString.substr(0, lastSlash));
        }
        return Path();
#endif
    }

    Path GetPrimPath() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return Path(_path.GetPrimPath());
#else
        size_t dotPos = _pathString.find('.');
        if (dotPos != std::string::npos) {
            return Path(_pathString.substr(0, dotPos));
        }
        return *this;
#endif
    }

    // Path construction
    Path AppendChild(const Token& name) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return Path(_path.AppendChild(name.GetTfToken()));
#else
        if (IsEmpty()) return Path();
        std::string result = _pathString;
        if (result.back() != '/') result += '/';
        result += name.GetText();
        return Path(result);
#endif
    }

    Path AppendChild(const char* name) const SWIFTUSD_NOEXCEPT {
        return AppendChild(Token(name));
    }

    Path AppendProperty(const Token& name) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return Path(_path.AppendProperty(name.GetTfToken()));
#else
        if (IsEmpty()) return Path();
        return Path(_pathString + "." + name.GetText());
#endif
    }

    Path AppendProperty(const char* name) const SWIFTUSD_NOEXCEPT {
        return AppendProperty(Token(name));
    }

    Path AppendPath(const Path& suffix) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return Path(_path.AppendPath(suffix._path));
#else
        if (IsEmpty() || suffix.IsEmpty()) return Path();
        std::string result = _pathString;
        const char* suffixText = suffix.GetText();
        if (suffixText[0] == '/') suffixText++; // Skip leading slash on suffix
        if (!result.empty() && result.back() != '/') result += '/';
        result += suffixText;
        return Path(result);
#endif
    }

    Path ReplacePrefix(const Path& oldPrefix, const Path& newPrefix) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return Path(_path.ReplacePrefix(oldPrefix._path, newPrefix._path));
#else
        const char* oldText = oldPrefix.GetText();
        size_t oldLen = std::strlen(oldText);
        if (_pathString.compare(0, oldLen, oldText) != 0) {
            return *this;
        }
        return Path(newPrefix.GetText() + _pathString.substr(oldLen));
#endif
    }

    bool HasPrefix(const Path& prefix) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _path.HasPrefix(prefix._path);
#else
        const char* prefixText = prefix.GetText();
        size_t prefixLen = std::strlen(prefixText);
        if (_pathString.size() < prefixLen) return false;
        return _pathString.compare(0, prefixLen, prefixText) == 0;
#endif
    }

    Path GetCommonPrefix(const Path& other) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return Path(_path.GetCommonPrefix(other._path));
#else
        const char* a = _pathString.c_str();
        const char* b = other._pathString.c_str();
        size_t i = 0;
        size_t lastSep = 0;
        while (a[i] && b[i] && a[i] == b[i]) {
            if (a[i] == '/' || a[i] == '.') lastSep = i + 1;
            i++;
        }
        if (!a[i] || !b[i]) {
            if (a[i] == '/' || a[i] == '.' || !a[i]) lastSep = i;
        }
        return Path(_pathString.substr(0, lastSep > 0 ? lastSep : i));
#endif
    }

    Path MakeAbsolutePath(const Path& anchor) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return Path(_path.MakeAbsolutePath(anchor._path));
#else
        if (IsAbsolutePath()) return *this;
        if (anchor.IsEmpty() || !anchor.IsAbsolutePath()) return Path();
        return anchor.AppendPath(*this);
#endif
    }

    Path MakeRelativePath(const Path& anchor) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return Path(_path.MakeRelativePath(anchor._path));
#else
        // Simplified relative path computation
        if (!IsAbsolutePath() || !anchor.IsAbsolutePath()) return *this;
        if (HasPrefix(anchor)) {
            std::string relative = _pathString.substr(anchor._pathString.size());
            if (!relative.empty() && relative[0] == '/') {
                relative = relative.substr(1);
            }
            return relative.empty() ? Path(".") : Path(relative);
        }
        return *this;
#endif
    }

    // Hashing
    size_t GetHash() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _path.GetHash();
#else
        return std::hash<std::string>{}(_pathString);
#endif
    }

    // Comparison operators
    bool operator==(const Path& other) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _path == other._path;
#else
        return _pathString == other._pathString;
#endif
    }

    bool operator!=(const Path& other) const SWIFTUSD_NOEXCEPT {
        return !(*this == other);
    }

    bool operator<(const Path& other) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _path < other._path;
#else
        return _pathString < other._pathString;
#endif
    }

    bool operator<=(const Path& other) const SWIFTUSD_NOEXCEPT {
        return !(other < *this);
    }

    bool operator>(const Path& other) const SWIFTUSD_NOEXCEPT {
        return other < *this;
    }

    bool operator>=(const Path& other) const SWIFTUSD_NOEXCEPT {
        return !(*this < other);
    }

    // Implicit conversion to bool
    explicit operator bool() const SWIFTUSD_NOEXCEPT {
        return !IsEmpty();
    }

    // Validation
    static bool IsValidPathString(const char* pathString) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        std::string errMsg;
        return PXR_NS::SdfPath::IsValidPathString(pathString ? pathString : "", &errMsg);
#else
        if (!pathString || !*pathString) return true; // Empty is valid
        // Basic validation for standalone mode
        // Paths should start with / (absolute) or be relative
        // Should not contain invalid characters
        for (const char* p = pathString; *p; ++p) {
            char c = *p;
            // Allow alphanumeric, underscore, slash, dot, and some special chars
            if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
                  (c >= '0' && c <= '9') || c == '_' || c == '/' ||
                  c == '.' || c == '{' || c == '}' || c == '=' || c == ':')) {
                return false;
            }
        }
        return true;
#endif
    }

    static bool IsValidIdentifier(const char* name) SWIFTUSD_NOEXCEPT {
        return swiftusd::IsValidIdentifier(name);
    }

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - AssetPath (SdfAssetPath value type)
// ============================================================================

/// Contains an asset path and optional resolved path.
///
/// AssetPath is used to reference external assets in USD layers.
struct AssetPath {
#if defined(USE_PIXAR_USD)
    PXR_NS::SdfAssetPath _assetPath;
#else
    std::string _assetPath;
    std::string _resolvedPath;
#endif

    AssetPath() SWIFTUSD_NOEXCEPT
#if defined(USE_PIXAR_USD)
        : _assetPath()
#else
        : _assetPath(), _resolvedPath()
#endif
    {}

    AssetPath(const char* path) SWIFTUSD_NOEXCEPT
#if defined(USE_PIXAR_USD)
        : _assetPath(path ? path : "")
#else
        : _assetPath(path ? path : ""), _resolvedPath()
#endif
    {}

    AssetPath(const std::string& path) SWIFTUSD_NOEXCEPT
#if defined(USE_PIXAR_USD)
        : _assetPath(path)
#else
        : _assetPath(path), _resolvedPath()
#endif
    {}

    AssetPath(const char* path, const char* resolvedPath) SWIFTUSD_NOEXCEPT
#if defined(USE_PIXAR_USD)
        : _assetPath(path ? path : "", resolvedPath ? resolvedPath : "")
#else
        : _assetPath(path ? path : ""), _resolvedPath(resolvedPath ? resolvedPath : "")
#endif
    {}

#if defined(USE_PIXAR_USD)
    AssetPath(const PXR_NS::SdfAssetPath& path) SWIFTUSD_NOEXCEPT : _assetPath(path) {}

    const PXR_NS::SdfAssetPath& GetSdfAssetPath() const SWIFTUSD_NOEXCEPT { return _assetPath; }
#endif

    SWIFT_RETURNS_INDEPENDENT_VALUE
    const char* GetAssetPath() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _assetPath.GetAssetPath().c_str();
#else
        return _assetPath.c_str();
#endif
    }

    SWIFT_RETURNS_INDEPENDENT_VALUE
    const char* GetResolvedPath() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _assetPath.GetResolvedPath().c_str();
#else
        return _resolvedPath.c_str();
#endif
    }

    bool IsEmpty() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _assetPath.GetAssetPath().empty();
#else
        return _assetPath.empty();
#endif
    }

    size_t GetHash() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return std::hash<PXR_NS::SdfAssetPath>{}(_assetPath);
#else
        return std::hash<std::string>{}(_assetPath);
#endif
    }

    bool operator==(const AssetPath& other) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _assetPath == other._assetPath;
#else
        return _assetPath == other._assetPath;
#endif
    }

    bool operator!=(const AssetPath& other) const SWIFTUSD_NOEXCEPT {
        return !(*this == other);
    }

    bool operator<(const AssetPath& other) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _assetPath < other._assetPath;
#else
        return _assetPath < other._assetPath;
#endif
    }

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - LayerRef (SdfLayer reference type)
// ============================================================================

/// A scene description container (layer) that can combine with other layers.
///
/// This is a reference-counted type.
/// Use factory methods to create instances.
class LayerRef {
public:
    // Factory methods - return new LayerRef* that caller owns
    static LayerRef* CreateNew(const char* identifier) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        auto layer = PXR_NS::SdfLayer::CreateNew(identifier ? identifier : "");
        if (!layer) return nullptr;
        return new LayerRef(layer);
#else
        if (!identifier || !*identifier) return nullptr;
        return new LayerRef(identifier, false);
#endif
    }

    static LayerRef* CreateAnonymous(const char* tag = nullptr) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        auto layer = PXR_NS::SdfLayer::CreateAnonymous(tag ? tag : "");
        if (!layer) return nullptr;
        return new LayerRef(layer);
#else
        std::string id = std::string("anon:") + (tag ? tag : "layer");
        return new LayerRef(id, true);
#endif
    }

    static LayerRef* FindOrOpen(const char* identifier) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        auto layer = PXR_NS::SdfLayer::FindOrOpen(identifier ? identifier : "");
        if (!layer) return nullptr;
        return new LayerRef(layer);
#else
        if (!identifier || !*identifier) return nullptr;
        return new LayerRef(identifier, false);
#endif
    }

    static LayerRef* Find(const char* identifier) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        auto layer = PXR_NS::SdfLayer::Find(identifier ? identifier : "");
        if (!layer) return nullptr;
        return new LayerRef(layer);
#else
        // Standalone mode doesn't have a layer registry
        return nullptr;
#endif
    }

    // Properties
    SWIFT_RETURNS_INDEPENDENT_VALUE
    const char* GetIdentifier() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _layer ? _layer->GetIdentifier().c_str() : "";
#else
        return _identifier.c_str();
#endif
    }

    SWIFT_RETURNS_INDEPENDENT_VALUE
    const char* GetDisplayName() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_layer) return "";
        _displayNameCache = _layer->GetDisplayName();
        return _displayNameCache.c_str();
#else
        // Extract filename from path
        size_t lastSlash = _identifier.rfind('/');
        if (lastSlash != std::string::npos) {
            _displayNameCache = _identifier.substr(lastSlash + 1);
        } else {
            _displayNameCache = _identifier;
        }
        return _displayNameCache.c_str();
#endif
    }

    bool IsDirty() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _layer && _layer->IsDirty();
#else
        return _isDirty;
#endif
    }

    bool IsAnonymous() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _layer && _layer->IsAnonymous();
#else
        return _isAnonymous;
#endif
    }

    bool IsEmpty() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return !_layer || _layer->IsEmpty();
#else
        return _rootPrimNames.empty();
#endif
    }

    // Content operations
    bool Save(bool force = false) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_layer || _layer->IsAnonymous()) return false;
        return _layer->Save(force);
#else
        _isDirty = false;
        return true;
#endif
    }

    bool Export(const char* path) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_layer || !path) return false;
        return _layer->Export(path);
#else
        return path != nullptr;
#endif
    }

    void Clear() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_layer) _layer->Clear();
#else
        _rootPrimNames.clear();
        _isDirty = true;
#endif
    }

    bool Reload(bool force = false) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_layer) return false;
        return _layer->Reload(force);
#else
        return true;
#endif
    }

    // Metadata
    Token GetDefaultPrim() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_layer) return Token();
        return Token(_layer->GetDefaultPrim());
#else
        return _defaultPrim;
#endif
    }

    void SetDefaultPrim(const Token& name) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_layer) _layer->SetDefaultPrim(name.GetTfToken());
#else
        _defaultPrim = name;
        _isDirty = true;
#endif
    }

    double GetStartTimeCode() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_layer) return 0.0;
        return _layer->GetStartTimeCode();
#else
        return _startTimeCode;
#endif
    }

    void SetStartTimeCode(double timeCode) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_layer) _layer->SetStartTimeCode(timeCode);
#else
        _startTimeCode = timeCode;
        _hasStartTimeCode = true;
        _isDirty = true;
#endif
    }

    bool HasStartTimeCode() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _layer && _layer->HasStartTimeCode();
#else
        return _hasStartTimeCode;
#endif
    }

    double GetEndTimeCode() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_layer) return 0.0;
        return _layer->GetEndTimeCode();
#else
        return _endTimeCode;
#endif
    }

    void SetEndTimeCode(double timeCode) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_layer) _layer->SetEndTimeCode(timeCode);
#else
        _endTimeCode = timeCode;
        _hasEndTimeCode = true;
        _isDirty = true;
#endif
    }

    bool HasEndTimeCode() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _layer && _layer->HasEndTimeCode();
#else
        return _hasEndTimeCode;
#endif
    }

    double GetTimeCodesPerSecond() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_layer) return 24.0;
        return _layer->GetTimeCodesPerSecond();
#else
        return _timeCodesPerSecond;
#endif
    }

    void SetTimeCodesPerSecond(double tcps) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_layer) _layer->SetTimeCodesPerSecond(tcps);
#else
        _timeCodesPerSecond = tcps;
        _isDirty = true;
#endif
    }

    double GetFramesPerSecond() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_layer) return 24.0;
        return _layer->GetFramesPerSecond();
#else
        return _framesPerSecond;
#endif
    }

    void SetFramesPerSecond(double fps) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (_layer) _layer->SetFramesPerSecond(fps);
#else
        _framesPerSecond = fps;
        _isDirty = true;
#endif
    }

    // Sublayers
    size_t GetNumSubLayers() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_layer) return 0;
        return _layer->GetSubLayerPaths().size();
#else
        return _subLayerPaths.size();
#endif
    }

    void InsertSubLayerPath(const char* path, int index = -1) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_layer || !path) return;
        if (index < 0) {
            _layer->GetSubLayerPaths().push_back(path);
        } else {
            _layer->InsertSubLayerPath(path, static_cast<size_t>(index));
        }
#else
        if (!path) return;
        if (index < 0 || static_cast<size_t>(index) >= _subLayerPaths.size()) {
            _subLayerPaths.push_back(path);
        } else {
            _subLayerPaths.insert(_subLayerPaths.begin() + index, path);
        }
        _isDirty = true;
#endif
    }

    void RemoveSubLayerPath(size_t index) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_layer || index >= _layer->GetSubLayerPaths().size()) return;
        _layer->RemoveSubLayerPath(index);
#else
        if (index >= _subLayerPaths.size()) return;
        _subLayerPaths.erase(_subLayerPaths.begin() + static_cast<ptrdiff_t>(index));
        _isDirty = true;
#endif
    }

    // Root prims
    size_t GetRootPrimCount() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_layer) return 0;
        return _layer->GetRootPrims().size();
#else
        return _rootPrimNames.size();
#endif
    }

    // Path operations
    bool HasSpec(const Path& path) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_layer) return false;
        return _layer->HasSpec(path.GetSdfPath());
#else
        // Standalone mode - check if path is in our root prims
        const char* text = path.GetText();
        for (const auto& name : _rootPrimNames) {
            std::string rootPath = "/" + name;
            if (rootPath == text) return true;
        }
        return false;
#endif
    }

#if defined(USE_PIXAR_USD)
    PXR_NS::SdfLayerHandle GetSdfLayer() const SWIFTUSD_NOEXCEPT { return _layer; }
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
    explicit LayerRef(const PXR_NS::SdfLayerHandle& layer) SWIFTUSD_NOEXCEPT
        : _layer(layer), _refCount(1) {}

    PXR_NS::SdfLayerHandle _layer;
    mutable std::string _displayNameCache;
#else
    LayerRef(const std::string& identifier, bool isAnonymous) SWIFTUSD_NOEXCEPT
        : _identifier(identifier),
          _isAnonymous(isAnonymous),
          _isDirty(false),
          _startTimeCode(0.0),
          _endTimeCode(0.0),
          _timeCodesPerSecond(24.0),
          _framesPerSecond(24.0),
          _hasStartTimeCode(false),
          _hasEndTimeCode(false),
          _refCount(1) {}

    std::string _identifier;
    mutable std::string _displayNameCache;
    bool _isAnonymous;
    bool _isDirty;
    double _startTimeCode;
    double _endTimeCode;
    double _timeCodesPerSecond;
    double _framesPerSecond;
    bool _hasStartTimeCode;
    bool _hasEndTimeCode;
    Token _defaultPrim;
    std::vector<std::string> _subLayerPaths;
    std::vector<std::string> _rootPrimNames;
#endif

    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

} // namespace swiftusd
