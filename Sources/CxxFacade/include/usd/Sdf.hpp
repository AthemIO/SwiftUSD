// Sdf.hpp - Scene Description Foundation facade
// CLEAN HEADER - NO pxr includes!
#pragma once

#include "../CxxFacade.h"
#include "../base/Tf.hpp"
#include "../base/Vt.hpp"

namespace cxxfacade {
namespace sdf {

// ============================================================================
// SdfLayerOffset - Time offset and scale between layers
// ============================================================================

/// SdfLayerOffset - Represents a time offset and scale between layers.
///
/// The SdfLayerOffset class is an affine transform, providing both a scale and
/// a translate. It supports vector algebra semantics for composing
/// SdfLayerOffsets together via multiplication.
///
/// For example, suppose layer A uses layer B, with an offset of X:
/// when bringing animation from B into A, you first apply the scale of X, and
/// then the offset. Suppose you have a scale of 2 and an offset of 24:
/// first multiply B's frame numbers by 2, and then add 24. The animation from
/// B as seen in A will take twice as long and start 24 frames later.
class SdfLayerOffset {
public:
    /// Constructs a new SdfLayerOffset instance.
    /// Default is identity: offset=0.0, scale=1.0
    SdfLayerOffset(double offset = 0.0, double scale = 1.0);

    /// Copy constructor
    SdfLayerOffset(const SdfLayerOffset& other);

    /// Move constructor
    SdfLayerOffset(SdfLayerOffset&& other) noexcept;

    /// Destructor
    ~SdfLayerOffset();

    /// Copy assignment
    SdfLayerOffset& operator=(const SdfLayerOffset& other);

    /// Move assignment
    SdfLayerOffset& operator=(SdfLayerOffset&& other) noexcept;

    /// Returns the time offset.
    double GetOffset() const;

    /// Returns the time scale factor.
    double GetScale() const;

    /// Sets the time offset.
    void SetOffset(double newOffset);

    /// Sets the time scale factor.
    void SetScale(double newScale);

    /// Returns true if this is an identity transformation,
    /// with an offset of 0.0 and a scale of 1.0.
    bool IsIdentity() const;

    /// Returns true if this offset is valid, i.e. both the offset and
    /// scale are finite (not infinite or NaN).
    bool IsValid() const;

    /// Gets the inverse offset, which performs the opposite transformation.
    SdfLayerOffset GetInverse() const;

    /// Returns hash for this offset.
    size_t GetHash() const;

    /// Comparison operators
    bool operator==(const SdfLayerOffset& rhs) const;
    bool operator!=(const SdfLayerOffset& rhs) const;
    bool operator<(const SdfLayerOffset& rhs) const;
    bool operator>(const SdfLayerOffset& rhs) const;
    bool operator<=(const SdfLayerOffset& rhs) const;
    bool operator>=(const SdfLayerOffset& rhs) const;

    /// Composes this with the offset rhs, such that the resulting
    /// offset is equivalent to first applying rhs and then *this.
    SdfLayerOffset operator*(const SdfLayerOffset& rhs) const;

    /// Applies the offset to the given value.
    double operator*(double rhs) const;

    void* GetImpl() const { return impl_; }
    static SdfLayerOffset FromImpl(void* impl);

private:
    void* impl_;
};

// ============================================================================
// SdfPath - Scene description path
// ============================================================================

/// SdfPath - Scene description path for identifying prims and properties
class SdfPath {
public:
    /// Create an empty path
    SdfPath();

    /// Create a path from a string (e.g., "/World/Cube")
    explicit SdfPath(const std::string& path);

    /// Create a path from a C string
    explicit SdfPath(const char* path);

    /// Copy constructor
    SdfPath(const SdfPath& other);

    /// Move constructor
    SdfPath(SdfPath&& other) noexcept;

    /// Destructor
    ~SdfPath();

    /// Copy assignment
    SdfPath& operator=(const SdfPath& other);

    /// Move assignment
    SdfPath& operator=(SdfPath&& other) noexcept;

    /// Get the string representation
    std::string GetString() const;

    /// Get the path as a C string (for debugging)
    const char* GetText() const;

    /// Check if the path is empty
    bool IsEmpty() const;

    /// Check if this is an absolute path (starts with /)
    bool IsAbsolutePath() const;

    /// Check if this is a prim path (not a property path)
    bool IsPrimPath() const;

    /// Check if this is a property path
    bool IsPropertyPath() const;

    /// Get the parent path
    SdfPath GetParentPath() const;

    /// Get the name (last element)
    tf::TfToken GetName() const;

    /// Append a child element
    SdfPath AppendChild(const tf::TfToken& childName) const;

    /// Append a property
    SdfPath AppendProperty(const tf::TfToken& propName) const;

    /// The absolute root path "/"
    static SdfPath AbsoluteRootPath();

    /// The empty path
    static SdfPath EmptyPath();

    /// Comparison operators
    bool operator==(const SdfPath& other) const;
    bool operator!=(const SdfPath& other) const;
    bool operator<(const SdfPath& other) const;

    void* GetImpl() const { return impl_; }
    static SdfPath FromImpl(void* impl);

private:
    void* impl_;
};

/// SdfAssetPath - Asset path for referencing external files
class SdfAssetPath {
public:
    SdfAssetPath();
    explicit SdfAssetPath(const std::string& path);
    SdfAssetPath(const std::string& path, const std::string& resolvedPath);
    SdfAssetPath(const SdfAssetPath& other);
    SdfAssetPath(SdfAssetPath&& other) noexcept;
    ~SdfAssetPath();

    SdfAssetPath& operator=(const SdfAssetPath& other);
    SdfAssetPath& operator=(SdfAssetPath&& other) noexcept;

    /// Get the authored asset path
    std::string GetAssetPath() const;

    /// Get the resolved asset path (after asset resolution)
    std::string GetResolvedPath() const;

    bool operator==(const SdfAssetPath& other) const;
    bool operator!=(const SdfAssetPath& other) const;

    void* GetImpl() const { return impl_; }
    static SdfAssetPath FromImpl(void* impl);

private:
    void* impl_;
};

/// SdfLayer - Scene description layer (USD file)
class SdfLayer {
public:
    /// Create a null layer reference
    SdfLayer();

    /// Copy constructor
    SdfLayer(const SdfLayer& other);

    /// Move constructor
    SdfLayer(SdfLayer&& other) noexcept;

    /// Destructor
    ~SdfLayer();

    /// Copy assignment
    SdfLayer& operator=(const SdfLayer& other);

    /// Move assignment
    SdfLayer& operator=(SdfLayer&& other) noexcept;

    /// Check if this is a valid layer reference
    bool IsValid() const;

    /// Get the layer identifier (file path or anonymous ID)
    std::string GetIdentifier() const;

    /// Get the layer's display name
    std::string GetDisplayName() const;

    /// Check if this is an anonymous layer
    bool IsAnonymous() const;

    /// Check if the layer has been modified
    bool IsDirty() const;

    /// Save the layer to disk
    bool Save() const;

    /// Export the layer to a file
    bool Export(const std::string& filename) const;

    /// Clear all contents
    void Clear();

    /// Create a new anonymous layer
    static SdfLayer CreateAnonymous();

    /// Create a new layer at the given path
    static SdfLayer CreateNew(const std::string& identifier);

    /// Find or open an existing layer
    static SdfLayer FindOrOpen(const std::string& identifier);

    void* GetImpl() const { return impl_; }
    static SdfLayer FromImpl(void* impl);

private:
    void* impl_;
};

} // namespace sdf
} // namespace cxxfacade
