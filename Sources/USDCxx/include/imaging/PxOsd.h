// PxOsd.h - OpenSubdiv Integration for SwiftUSD
// Mirrors: pxr/imaging/pxOsd/meshTopology.h, pxr/imaging/pxOsd/subdivTags.h
// C++ header with Swift interop annotations
//
// PxOsd provides OpenSubdiv integration for USD, including mesh topology
// representation and subdivision surface parameters.

#pragma once

#include "../Swift.h"
#include "../base/Tf.h"
#include "../base/Vt.h"

#include <atomic>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#if defined(USE_PIXAR_USD)
#include "pxr/pxr.h"
#include "pxr/imaging/pxOsd/meshTopology.h"
#include "pxr/imaging/pxOsd/subdivTags.h"
#include "pxr/imaging/pxOsd/meshTopologyValidation.h"
#include "pxr/imaging/pxOsd/tokens.h"
#endif // USE_PIXAR_USD

namespace swiftusd {

// ============================================================================
// MARK: - PxOsdSubdivScheme (Subdivision scheme enumeration)
// ============================================================================

/// Subdivision scheme types for mesh topology.
enum class PxOsdSubdivScheme : int32_t {
    None = 0,
    CatmullClark,
    Loop,
    Bilinear
};

/// Convert scheme token to enum
inline PxOsdSubdivScheme PxOsdSubdivSchemeFromToken(const Token& token) SWIFTUSD_NOEXCEPT {
    const char* text = token.GetText();
    if (!text || !*text) return PxOsdSubdivScheme::None;
    if (std::strcmp(text, "catmullClark") == 0) return PxOsdSubdivScheme::CatmullClark;
    if (std::strcmp(text, "loop") == 0) return PxOsdSubdivScheme::Loop;
    if (std::strcmp(text, "bilinear") == 0) return PxOsdSubdivScheme::Bilinear;
    return PxOsdSubdivScheme::None;
}

/// Convert enum to scheme token
inline Token PxOsdSubdivSchemeToToken(PxOsdSubdivScheme scheme) SWIFTUSD_NOEXCEPT {
    switch (scheme) {
        case PxOsdSubdivScheme::CatmullClark: return Token("catmullClark");
        case PxOsdSubdivScheme::Loop: return Token("loop");
        case PxOsdSubdivScheme::Bilinear: return Token("bilinear");
        default: return Token();
    }
}

// ============================================================================
// MARK: - PxOsdInterpolationRule (Vertex/FaceVarying interpolation)
// ============================================================================

/// Interpolation rules for boundary vertices and face-varying data.
enum class PxOsdInterpolationRule : int32_t {
    None = 0,
    All,
    CornersOnly,
    CornersPlus1,
    CornersPlus2,
    Boundaries,
    EdgeOnly,
    EdgeAndCorner,
    Smooth
};

/// Convert interpolation rule token to enum
inline PxOsdInterpolationRule PxOsdInterpolationRuleFromToken(const Token& token) SWIFTUSD_NOEXCEPT {
    const char* text = token.GetText();
    if (!text || !*text) return PxOsdInterpolationRule::None;
    if (std::strcmp(text, "all") == 0) return PxOsdInterpolationRule::All;
    if (std::strcmp(text, "cornersOnly") == 0) return PxOsdInterpolationRule::CornersOnly;
    if (std::strcmp(text, "cornersPlus1") == 0) return PxOsdInterpolationRule::CornersPlus1;
    if (std::strcmp(text, "cornersPlus2") == 0) return PxOsdInterpolationRule::CornersPlus2;
    if (std::strcmp(text, "boundaries") == 0) return PxOsdInterpolationRule::Boundaries;
    if (std::strcmp(text, "edgeOnly") == 0) return PxOsdInterpolationRule::EdgeOnly;
    if (std::strcmp(text, "edgeAndCorner") == 0) return PxOsdInterpolationRule::EdgeAndCorner;
    if (std::strcmp(text, "smooth") == 0) return PxOsdInterpolationRule::Smooth;
    return PxOsdInterpolationRule::None;
}

/// Convert enum to interpolation rule token
inline Token PxOsdInterpolationRuleToToken(PxOsdInterpolationRule rule) SWIFTUSD_NOEXCEPT {
    switch (rule) {
        case PxOsdInterpolationRule::All: return Token("all");
        case PxOsdInterpolationRule::CornersOnly: return Token("cornersOnly");
        case PxOsdInterpolationRule::CornersPlus1: return Token("cornersPlus1");
        case PxOsdInterpolationRule::CornersPlus2: return Token("cornersPlus2");
        case PxOsdInterpolationRule::Boundaries: return Token("boundaries");
        case PxOsdInterpolationRule::EdgeOnly: return Token("edgeOnly");
        case PxOsdInterpolationRule::EdgeAndCorner: return Token("edgeAndCorner");
        case PxOsdInterpolationRule::Smooth: return Token("smooth");
        default: return Token("none");
    }
}

// ============================================================================
// MARK: - PxOsdCreaseMethod
// ============================================================================

/// Methods for computing crease sharpness.
enum class PxOsdCreaseMethod : int32_t {
    None = 0,
    Uniform,
    Chaikin
};

/// Convert crease method token to enum
inline PxOsdCreaseMethod PxOsdCreaseMethodFromToken(const Token& token) SWIFTUSD_NOEXCEPT {
    const char* text = token.GetText();
    if (!text || !*text) return PxOsdCreaseMethod::None;
    if (std::strcmp(text, "uniform") == 0) return PxOsdCreaseMethod::Uniform;
    if (std::strcmp(text, "chaikin") == 0) return PxOsdCreaseMethod::Chaikin;
    return PxOsdCreaseMethod::None;
}

/// Convert enum to crease method token
inline Token PxOsdCreaseMethodToToken(PxOsdCreaseMethod method) SWIFTUSD_NOEXCEPT {
    switch (method) {
        case PxOsdCreaseMethod::Uniform: return Token("uniform");
        case PxOsdCreaseMethod::Chaikin: return Token("chaikin");
        default: return Token("none");
    }
}

// ============================================================================
// MARK: - PxOsdTriangleSubdivision
// ============================================================================

/// Triangle subdivision methods.
enum class PxOsdTriangleSubdivision : int32_t {
    None = 0,
    Smooth
};

/// Convert triangle subdivision token to enum
inline PxOsdTriangleSubdivision PxOsdTriangleSubdivisionFromToken(const Token& token) SWIFTUSD_NOEXCEPT {
    const char* text = token.GetText();
    if (!text || !*text) return PxOsdTriangleSubdivision::None;
    if (std::strcmp(text, "smooth") == 0) return PxOsdTriangleSubdivision::Smooth;
    return PxOsdTriangleSubdivision::None;
}

/// Convert enum to triangle subdivision token
inline Token PxOsdTriangleSubdivisionToToken(PxOsdTriangleSubdivision subdiv) SWIFTUSD_NOEXCEPT {
    switch (subdiv) {
        case PxOsdTriangleSubdivision::Smooth: return Token("smooth");
        default: return Token("none");
    }
}

// ============================================================================
// MARK: - PxOsdOrientation
// ============================================================================

/// Face winding orientation.
enum class PxOsdOrientation : int32_t {
    RightHanded = 0,
    LeftHanded
};

/// Convert orientation token to enum
inline PxOsdOrientation PxOsdOrientationFromToken(const Token& token) SWIFTUSD_NOEXCEPT {
    const char* text = token.GetText();
    if (!text || !*text) return PxOsdOrientation::RightHanded;
    if (std::strcmp(text, "leftHanded") == 0) return PxOsdOrientation::LeftHanded;
    return PxOsdOrientation::RightHanded;
}

/// Convert enum to orientation token
inline Token PxOsdOrientationToToken(PxOsdOrientation orient) SWIFTUSD_NOEXCEPT {
    switch (orient) {
        case PxOsdOrientation::LeftHanded: return Token("leftHanded");
        default: return Token("rightHanded");
    }
}

// ============================================================================
// MARK: - PxOsdMeshTopologyValidationCode
// ============================================================================

/// Validation error codes for mesh topology.
enum class PxOsdMeshTopologyValidationCode : int32_t {
    InvalidScheme = 0,
    InvalidOrientation,
    InvalidTriangleSubdivision,
    InvalidVertexInterpolationRule,
    InvalidFaceVaryingInterpolationRule,
    InvalidCreaseMethod,
    InvalidCreaseLengthElement,
    InvalidCreaseIndicesSize,
    InvalidCreaseIndicesElement,
    InvalidCreaseWeightsSize,
    NegativeCreaseWeights,
    InvalidCornerIndicesElement,
    NegativeCornerWeights,
    InvalidCornerWeightsSize,
    InvalidHoleIndicesElement,
    InvalidFaceVertexCountsElement,
    InvalidFaceVertexIndicesElement,
    InvalidFaceVertexIndicesSize
};

// ============================================================================
// MARK: - Forward declarations
// ============================================================================

class PxOsdSubdivTagsRef;
class PxOsdMeshTopologyRef;
class PxOsdMeshTopologyValidationRef;

// ============================================================================
// MARK: - PxOsdSubdivTagsRef
// ============================================================================

/// Tags for non-hierarchical subdivision surfaces.
///
/// This class contains parameters for subdivision surface control:
/// - Vertex and face-varying interpolation rules
/// - Crease edges with indices, lengths, and weights
/// - Corner vertices with indices and weights
/// - Triangle subdivision and crease methods
///
/// Usage:
/// ```swift
/// let tags = PxOsdSubdivTagsRef.Create()
/// tags?.SetVertexInterpolationRule(.cornersPlus1)
/// tags?.SetCreaseIndices(creaseIndices)
/// tags?.SetCreaseWeights(weights)
/// ```
class PxOsdSubdivTagsRef {
public:
    // ========================================================================
    // MARK: - Factory Methods
    // ========================================================================

    /// Create empty subdivision tags.
    static PxOsdSubdivTagsRef* Create() SWIFTUSD_NOEXCEPT {
        return new PxOsdSubdivTagsRef();
    }

    /// Create subdivision tags with all parameters.
    static PxOsdSubdivTagsRef* Create(
        PxOsdInterpolationRule vertexInterpolationRule,
        PxOsdInterpolationRule faceVaryingInterpolationRule,
        PxOsdCreaseMethod creaseMethod,
        PxOsdTriangleSubdivision triangleSubdivision,
        const IntArray* creaseIndices,
        const IntArray* creaseLengths,
        const FloatArray* creaseWeights,
        const IntArray* cornerIndices,
        const FloatArray* cornerWeights) SWIFTUSD_NOEXCEPT
    {
        auto* ref = new PxOsdSubdivTagsRef();
        ref->_vtxInterpolationRule = vertexInterpolationRule;
        ref->_fvarInterpolationRule = faceVaryingInterpolationRule;
        ref->_creaseMethod = creaseMethod;
        ref->_triangleSubdivision = triangleSubdivision;
        if (creaseIndices) ref->_creaseIndices = creaseIndices->_data;
        if (creaseLengths) ref->_creaseLengths = creaseLengths->_data;
        if (creaseWeights) ref->_creaseWeights = creaseWeights->_data;
        if (cornerIndices) ref->_cornerIndices = cornerIndices->_data;
        if (cornerWeights) ref->_cornerWeights = cornerWeights->_data;
        return ref;
    }

    // ========================================================================
    // MARK: - Interpolation Rules
    // ========================================================================

    /// Get the vertex boundary interpolation rule.
    PxOsdInterpolationRule GetVertexInterpolationRule() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return PxOsdInterpolationRuleFromToken(Token(_tags.GetVertexInterpolationRule()));
#else
        return _vtxInterpolationRule;
#endif
    }

    /// Set the vertex boundary interpolation rule.
    void SetVertexInterpolationRule(PxOsdInterpolationRule rule) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        _tags.SetVertexInterpolationRule(
            PxOsdInterpolationRuleToToken(rule).GetTfToken());
#else
        _vtxInterpolationRule = rule;
#endif
    }

    /// Get the face-varying boundary interpolation rule.
    PxOsdInterpolationRule GetFaceVaryingInterpolationRule() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return PxOsdInterpolationRuleFromToken(Token(_tags.GetFaceVaryingInterpolationRule()));
#else
        return _fvarInterpolationRule;
#endif
    }

    /// Set the face-varying boundary interpolation rule.
    void SetFaceVaryingInterpolationRule(PxOsdInterpolationRule rule) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        _tags.SetFaceVaryingInterpolationRule(
            PxOsdInterpolationRuleToToken(rule).GetTfToken());
#else
        _fvarInterpolationRule = rule;
#endif
    }

    // ========================================================================
    // MARK: - Crease Method
    // ========================================================================

    /// Get the creasing method.
    PxOsdCreaseMethod GetCreaseMethod() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return PxOsdCreaseMethodFromToken(Token(_tags.GetCreaseMethod()));
#else
        return _creaseMethod;
#endif
    }

    /// Set the creasing method.
    void SetCreaseMethod(PxOsdCreaseMethod method) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        _tags.SetCreaseMethod(PxOsdCreaseMethodToToken(method).GetTfToken());
#else
        _creaseMethod = method;
#endif
    }

    // ========================================================================
    // MARK: - Triangle Subdivision
    // ========================================================================

    /// Get the triangle subdivision method.
    PxOsdTriangleSubdivision GetTriangleSubdivision() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return PxOsdTriangleSubdivisionFromToken(Token(_tags.GetTriangleSubdivision()));
#else
        return _triangleSubdivision;
#endif
    }

    /// Set the triangle subdivision method.
    void SetTriangleSubdivision(PxOsdTriangleSubdivision subdiv) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        _tags.SetTriangleSubdivision(
            PxOsdTriangleSubdivisionToToken(subdiv).GetTfToken());
#else
        _triangleSubdivision = subdiv;
#endif
    }

    // ========================================================================
    // MARK: - Crease Data
    // ========================================================================

    /// Get the edge crease indices.
    IntArray* GetCreaseIndices() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return new IntArray(_tags.GetCreaseIndices());
#else
        return new IntArray(_creaseIndices.data(), _creaseIndices.size());
#endif
    }

    /// Set the edge crease indices.
    void SetCreaseIndices(const IntArray* indices) SWIFTUSD_NOEXCEPT {
        if (!indices) return;
#if defined(USE_PIXAR_USD)
        _tags.SetCreaseIndices(indices->ToVt());
#else
        _creaseIndices = indices->_data;
#endif
    }

    /// Get the edge crease loop lengths.
    IntArray* GetCreaseLengths() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return new IntArray(_tags.GetCreaseLengths());
#else
        return new IntArray(_creaseLengths.data(), _creaseLengths.size());
#endif
    }

    /// Set the edge crease loop lengths.
    void SetCreaseLengths(const IntArray* lengths) SWIFTUSD_NOEXCEPT {
        if (!lengths) return;
#if defined(USE_PIXAR_USD)
        _tags.SetCreaseLengths(lengths->ToVt());
#else
        _creaseLengths = lengths->_data;
#endif
    }

    /// Get the edge crease weights.
    FloatArray* GetCreaseWeights() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return new FloatArray(_tags.GetCreaseWeights());
#else
        return new FloatArray(_creaseWeights.data(), _creaseWeights.size());
#endif
    }

    /// Set the edge crease weights.
    void SetCreaseWeights(const FloatArray* weights) SWIFTUSD_NOEXCEPT {
        if (!weights) return;
#if defined(USE_PIXAR_USD)
        _tags.SetCreaseWeights(weights->ToVt());
#else
        _creaseWeights = weights->_data;
#endif
    }

    // ========================================================================
    // MARK: - Corner Data
    // ========================================================================

    /// Get the corner vertex indices.
    IntArray* GetCornerIndices() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return new IntArray(_tags.GetCornerIndices());
#else
        return new IntArray(_cornerIndices.data(), _cornerIndices.size());
#endif
    }

    /// Set the corner vertex indices.
    void SetCornerIndices(const IntArray* indices) SWIFTUSD_NOEXCEPT {
        if (!indices) return;
#if defined(USE_PIXAR_USD)
        _tags.SetCornerIndices(indices->ToVt());
#else
        _cornerIndices = indices->_data;
#endif
    }

    /// Get the corner weights.
    FloatArray* GetCornerWeights() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return new FloatArray(_tags.GetCornerWeights());
#else
        return new FloatArray(_cornerWeights.data(), _cornerWeights.size());
#endif
    }

    /// Set the corner weights.
    void SetCornerWeights(const FloatArray* weights) SWIFTUSD_NOEXCEPT {
        if (!weights) return;
#if defined(USE_PIXAR_USD)
        _tags.SetCornerWeights(weights->ToVt());
#else
        _cornerWeights = weights->_data;
#endif
    }

    // ========================================================================
    // MARK: - Hash
    // ========================================================================

    /// Compute a hash value for instancing.
    uint64_t ComputeHash() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<uint64_t>(_tags.ComputeHash());
#else
        // Simple hash combining all data
        size_t h = 0;
        h ^= std::hash<int>{}(static_cast<int>(_vtxInterpolationRule)) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<int>{}(static_cast<int>(_fvarInterpolationRule)) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<int>{}(static_cast<int>(_creaseMethod)) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<int>{}(static_cast<int>(_triangleSubdivision)) + 0x9e3779b9 + (h << 6) + (h >> 2);
        for (auto v : _creaseIndices) h ^= std::hash<int>{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
        for (auto v : _creaseLengths) h ^= std::hash<int>{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
        for (auto v : _creaseWeights) h ^= std::hash<float>{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
        for (auto v : _cornerIndices) h ^= std::hash<int>{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
        for (auto v : _cornerWeights) h ^= std::hash<float>{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return static_cast<uint64_t>(h);
#endif
    }

    // ========================================================================
    // MARK: - Reference Counting
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
    friend class PxOsdMeshTopologyRef;

    PxOsdSubdivTagsRef() SWIFTUSD_NOEXCEPT
        : _vtxInterpolationRule(PxOsdInterpolationRule::None),
          _fvarInterpolationRule(PxOsdInterpolationRule::None),
          _creaseMethod(PxOsdCreaseMethod::None),
          _triangleSubdivision(PxOsdTriangleSubdivision::None),
          _refCount(1) {}

#if defined(USE_PIXAR_USD)
    explicit PxOsdSubdivTagsRef(const PXR_NS::PxOsdSubdivTags& tags) SWIFTUSD_NOEXCEPT
        : _tags(tags), _refCount(1) {}

    PXR_NS::PxOsdSubdivTags _tags;
#else
    PxOsdInterpolationRule _vtxInterpolationRule;
    PxOsdInterpolationRule _fvarInterpolationRule;
    PxOsdCreaseMethod _creaseMethod;
    PxOsdTriangleSubdivision _triangleSubdivision;
    std::vector<int32_t> _creaseIndices;
    std::vector<int32_t> _creaseLengths;
    std::vector<float> _creaseWeights;
    std::vector<int32_t> _cornerIndices;
    std::vector<float> _cornerWeights;
#endif

    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - PxOsdMeshTopologyValidationRef
// ============================================================================

/// Validation result for mesh topology.
///
/// Contains any validation errors found when validating a PxOsdMeshTopology.
/// An empty validation (no errors) indicates valid topology.
///
/// Usage:
/// ```swift
/// let validation = topology.Validate()
/// if validation.IsValid() {
///     // Topology is valid
/// } else {
///     // Check validation.GetErrorCount() and iterate errors
/// }
/// ```
class PxOsdMeshTopologyValidationRef {
public:
    // ========================================================================
    // MARK: - Query Methods
    // ========================================================================

    /// Returns true if the topology is valid (no errors).
    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_validation);
#else
        return _errors.empty();
#endif
    }

    /// Returns the number of validation errors.
    size_t GetErrorCount() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        size_t count = 0;
        for (auto it = _validation.begin(); it != _validation.end(); ++it) {
            ++count;
        }
        return count;
#else
        return _errors.size();
#endif
    }

    /// Get the error code at the specified index.
    PxOsdMeshTopologyValidationCode GetErrorCode(size_t index) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        size_t i = 0;
        for (auto it = _validation.begin(); it != _validation.end(); ++it) {
            if (i == index) {
                return static_cast<PxOsdMeshTopologyValidationCode>(it->code);
            }
            ++i;
        }
        return PxOsdMeshTopologyValidationCode::InvalidScheme;
#else
        if (index < _errors.size()) {
            return _errors[index].code;
        }
        return PxOsdMeshTopologyValidationCode::InvalidScheme;
#endif
    }

    /// Get the error message at the specified index.
    SWIFT_RETURNS_INDEPENDENT_VALUE
    const char* GetErrorMessage(size_t index) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        size_t i = 0;
        for (auto it = _validation.begin(); it != _validation.end(); ++it) {
            if (i == index) {
                _messageCache = it->message;
                return _messageCache.c_str();
            }
            ++i;
        }
        return "";
#else
        if (index < _errors.size()) {
            return _errors[index].message.c_str();
        }
        return "";
#endif
    }

    // ========================================================================
    // MARK: - Reference Counting
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
    friend class PxOsdMeshTopologyRef;

    struct Error {
        PxOsdMeshTopologyValidationCode code;
        std::string message;
    };

    PxOsdMeshTopologyValidationRef() SWIFTUSD_NOEXCEPT : _refCount(1) {}

#if defined(USE_PIXAR_USD)
    explicit PxOsdMeshTopologyValidationRef(const PXR_NS::PxOsdMeshTopologyValidation& validation) SWIFTUSD_NOEXCEPT
        : _validation(validation), _refCount(1) {}

    PXR_NS::PxOsdMeshTopologyValidation _validation;
    mutable std::string _messageCache;
#else
    std::vector<Error> _errors;
#endif

    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - PxOsdMeshTopologyRef
// ============================================================================

/// Topology data for subdivision meshes.
///
/// Once constructed, this class is immutable (except when assigned or moved).
/// To modify properties, use the With* methods which return modified copies.
///
/// Usage:
/// ```swift
/// // Create a simple quad topology
/// let faceVertexCounts = IntArray.Create([4])
/// let faceVertexIndices = IntArray.Create([0, 1, 2, 3])
/// let topology = PxOsdMeshTopologyRef.Create(
///     .catmullClark, .rightHanded, faceVertexCounts, faceVertexIndices)
///
/// // Modify scheme
/// let loopTopology = topology?.WithScheme(.loop)
/// ```
class PxOsdMeshTopologyRef {
public:
    // ========================================================================
    // MARK: - Factory Methods
    // ========================================================================

    /// Create empty mesh topology.
    static PxOsdMeshTopologyRef* Create() SWIFTUSD_NOEXCEPT {
        return new PxOsdMeshTopologyRef();
    }

    /// Create mesh topology without holes or subdiv tags.
    static PxOsdMeshTopologyRef* Create(
        PxOsdSubdivScheme scheme,
        PxOsdOrientation orientation,
        const IntArray* faceVertexCounts,
        const IntArray* faceVertexIndices) SWIFTUSD_NOEXCEPT
    {
        if (!faceVertexCounts || !faceVertexIndices) return nullptr;
#if defined(USE_PIXAR_USD)
        PXR_NS::PxOsdMeshTopology topo(
            PxOsdSubdivSchemeToToken(scheme).GetTfToken(),
            PxOsdOrientationToToken(orientation).GetTfToken(),
            faceVertexCounts->ToVt(),
            faceVertexIndices->ToVt());
        return new PxOsdMeshTopologyRef(topo);
#else
        auto* ref = new PxOsdMeshTopologyRef();
        ref->_scheme = scheme;
        ref->_orientation = orientation;
        ref->_faceVertexCounts = faceVertexCounts->_data;
        ref->_faceVertexIndices = faceVertexIndices->_data;
        return ref;
#endif
    }

    /// Create mesh topology with holes.
    static PxOsdMeshTopologyRef* Create(
        PxOsdSubdivScheme scheme,
        PxOsdOrientation orientation,
        const IntArray* faceVertexCounts,
        const IntArray* faceVertexIndices,
        const IntArray* holeIndices) SWIFTUSD_NOEXCEPT
    {
        if (!faceVertexCounts || !faceVertexIndices) return nullptr;
#if defined(USE_PIXAR_USD)
        PXR_NS::PxOsdMeshTopology topo(
            PxOsdSubdivSchemeToToken(scheme).GetTfToken(),
            PxOsdOrientationToToken(orientation).GetTfToken(),
            faceVertexCounts->ToVt(),
            faceVertexIndices->ToVt(),
            holeIndices ? holeIndices->ToVt() : PXR_NS::VtIntArray());
        return new PxOsdMeshTopologyRef(topo);
#else
        auto* ref = new PxOsdMeshTopologyRef();
        ref->_scheme = scheme;
        ref->_orientation = orientation;
        ref->_faceVertexCounts = faceVertexCounts->_data;
        ref->_faceVertexIndices = faceVertexIndices->_data;
        if (holeIndices) ref->_holeIndices = holeIndices->_data;
        return ref;
#endif
    }

    /// Create mesh topology with holes and subdiv tags.
    static PxOsdMeshTopologyRef* Create(
        PxOsdSubdivScheme scheme,
        PxOsdOrientation orientation,
        const IntArray* faceVertexCounts,
        const IntArray* faceVertexIndices,
        const IntArray* holeIndices,
        const PxOsdSubdivTagsRef* subdivTags) SWIFTUSD_NOEXCEPT
    {
        if (!faceVertexCounts || !faceVertexIndices) return nullptr;
#if defined(USE_PIXAR_USD)
        PXR_NS::PxOsdMeshTopology topo(
            PxOsdSubdivSchemeToToken(scheme).GetTfToken(),
            PxOsdOrientationToToken(orientation).GetTfToken(),
            faceVertexCounts->ToVt(),
            faceVertexIndices->ToVt(),
            holeIndices ? holeIndices->ToVt() : PXR_NS::VtIntArray(),
            subdivTags ? subdivTags->_tags : PXR_NS::PxOsdSubdivTags());
        return new PxOsdMeshTopologyRef(topo);
#else
        auto* ref = new PxOsdMeshTopologyRef();
        ref->_scheme = scheme;
        ref->_orientation = orientation;
        ref->_faceVertexCounts = faceVertexCounts->_data;
        ref->_faceVertexIndices = faceVertexIndices->_data;
        if (holeIndices) ref->_holeIndices = holeIndices->_data;
        if (subdivTags) {
            ref->_subdivTags._vtxInterpolationRule = subdivTags->_vtxInterpolationRule;
            ref->_subdivTags._fvarInterpolationRule = subdivTags->_fvarInterpolationRule;
            ref->_subdivTags._creaseMethod = subdivTags->_creaseMethod;
            ref->_subdivTags._triangleSubdivision = subdivTags->_triangleSubdivision;
            ref->_subdivTags._creaseIndices = subdivTags->_creaseIndices;
            ref->_subdivTags._creaseLengths = subdivTags->_creaseLengths;
            ref->_subdivTags._creaseWeights = subdivTags->_creaseWeights;
            ref->_subdivTags._cornerIndices = subdivTags->_cornerIndices;
            ref->_subdivTags._cornerWeights = subdivTags->_cornerWeights;
        }
        return ref;
#endif
    }

    // ========================================================================
    // MARK: - Accessors
    // ========================================================================

    /// Returns the subdivision scheme.
    PxOsdSubdivScheme GetScheme() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return PxOsdSubdivSchemeFromToken(Token(_topology.GetScheme()));
#else
        return _scheme;
#endif
    }

    /// Returns the face orientation.
    PxOsdOrientation GetOrientation() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return PxOsdOrientationFromToken(Token(_topology.GetOrientation()));
#else
        return _orientation;
#endif
    }

    /// Returns face vertex counts.
    IntArray* GetFaceVertexCounts() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return new IntArray(_topology.GetFaceVertexCounts());
#else
        return new IntArray(_faceVertexCounts.data(), _faceVertexCounts.size());
#endif
    }

    /// Returns face vertex indices.
    IntArray* GetFaceVertexIndices() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return new IntArray(_topology.GetFaceVertexIndices());
#else
        return new IntArray(_faceVertexIndices.data(), _faceVertexIndices.size());
#endif
    }

    /// Returns hole face indices.
    IntArray* GetHoleIndices() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return new IntArray(_topology.GetHoleIndices());
#else
        return new IntArray(_holeIndices.data(), _holeIndices.size());
#endif
    }

    /// Returns subdivision tags.
    PxOsdSubdivTagsRef* GetSubdivTags() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return new PxOsdSubdivTagsRef(_topology.GetSubdivTags());
#else
        auto* tags = new PxOsdSubdivTagsRef();
        tags->_vtxInterpolationRule = _subdivTags._vtxInterpolationRule;
        tags->_fvarInterpolationRule = _subdivTags._fvarInterpolationRule;
        tags->_creaseMethod = _subdivTags._creaseMethod;
        tags->_triangleSubdivision = _subdivTags._triangleSubdivision;
        tags->_creaseIndices = _subdivTags._creaseIndices;
        tags->_creaseLengths = _subdivTags._creaseLengths;
        tags->_creaseWeights = _subdivTags._creaseWeights;
        tags->_cornerIndices = _subdivTags._cornerIndices;
        tags->_cornerWeights = _subdivTags._cornerWeights;
        return tags;
#endif
    }

    /// Returns the number of faces.
    size_t GetFaceCount() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _topology.GetFaceVertexCounts().size();
#else
        return _faceVertexCounts.size();
#endif
    }

    /// Returns the total number of vertex indices.
    size_t GetVertexIndexCount() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return _topology.GetFaceVertexIndices().size();
#else
        return _faceVertexIndices.size();
#endif
    }

    // ========================================================================
    // MARK: - Builder Methods (return modified copies)
    // ========================================================================

    /// Return a copy with a different scheme.
    PxOsdMeshTopologyRef* WithScheme(PxOsdSubdivScheme scheme) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return new PxOsdMeshTopologyRef(
            _topology.WithScheme(PxOsdSubdivSchemeToToken(scheme).GetTfToken()));
#else
        auto* ref = new PxOsdMeshTopologyRef(*this);
        ref->_scheme = scheme;
        return ref;
#endif
    }

    /// Return a copy with a different orientation.
    PxOsdMeshTopologyRef* WithOrientation(PxOsdOrientation orientation) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return new PxOsdMeshTopologyRef(
            _topology.WithOrientation(PxOsdOrientationToToken(orientation).GetTfToken()));
#else
        auto* ref = new PxOsdMeshTopologyRef(*this);
        ref->_orientation = orientation;
        return ref;
#endif
    }

    /// Return a copy with different hole indices.
    PxOsdMeshTopologyRef* WithHoleIndices(const IntArray* holeIndices) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return new PxOsdMeshTopologyRef(
            _topology.WithHoleIndices(holeIndices ? holeIndices->ToVt() : PXR_NS::VtIntArray()));
#else
        auto* ref = new PxOsdMeshTopologyRef(*this);
        ref->_holeIndices = holeIndices ? holeIndices->_data : std::vector<int32_t>();
        return ref;
#endif
    }

    /// Return a copy with different subdiv tags.
    PxOsdMeshTopologyRef* WithSubdivTags(const PxOsdSubdivTagsRef* subdivTags) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return new PxOsdMeshTopologyRef(
            _topology.WithSubdivTags(subdivTags ? subdivTags->_tags : PXR_NS::PxOsdSubdivTags()));
#else
        auto* ref = new PxOsdMeshTopologyRef(*this);
        if (subdivTags) {
            ref->_subdivTags._vtxInterpolationRule = subdivTags->_vtxInterpolationRule;
            ref->_subdivTags._fvarInterpolationRule = subdivTags->_fvarInterpolationRule;
            ref->_subdivTags._creaseMethod = subdivTags->_creaseMethod;
            ref->_subdivTags._triangleSubdivision = subdivTags->_triangleSubdivision;
            ref->_subdivTags._creaseIndices = subdivTags->_creaseIndices;
            ref->_subdivTags._creaseLengths = subdivTags->_creaseLengths;
            ref->_subdivTags._creaseWeights = subdivTags->_creaseWeights;
            ref->_subdivTags._cornerIndices = subdivTags->_cornerIndices;
            ref->_subdivTags._cornerWeights = subdivTags->_cornerWeights;
        } else {
            ref->_subdivTags = SubdivTagsData();
        }
        return ref;
#endif
    }

    // ========================================================================
    // MARK: - Validation
    // ========================================================================

    /// Validate the topology and return a validation result.
    PxOsdMeshTopologyValidationRef* Validate() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return new PxOsdMeshTopologyValidationRef(_topology.Validate());
#else
        auto* validation = new PxOsdMeshTopologyValidationRef();
        // Basic standalone validation
        // Check face vertex counts
        for (size_t i = 0; i < _faceVertexCounts.size(); ++i) {
            if (_faceVertexCounts[i] < 3) {
                validation->_errors.push_back({
                    PxOsdMeshTopologyValidationCode::InvalidFaceVertexCountsElement,
                    "Face vertex count less than 3"
                });
                break;
            }
        }
        // Check indices sum matches face vertex indices size
        int64_t sum = 0;
        for (auto c : _faceVertexCounts) sum += c;
        if (static_cast<size_t>(sum) != _faceVertexIndices.size()) {
            validation->_errors.push_back({
                PxOsdMeshTopologyValidationCode::InvalidFaceVertexIndicesSize,
                "Face vertex indices size mismatch"
            });
        }
        // Check for negative indices
        for (auto idx : _faceVertexIndices) {
            if (idx < 0) {
                validation->_errors.push_back({
                    PxOsdMeshTopologyValidationCode::InvalidFaceVertexIndicesElement,
                    "Negative face vertex index"
                });
                break;
            }
        }
        return validation;
#endif
    }

    // ========================================================================
    // MARK: - Hash and Equality
    // ========================================================================

    /// Compute a hash value for instancing.
    uint64_t ComputeHash() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<uint64_t>(_topology.ComputeHash());
#else
        size_t h = 0;
        h ^= std::hash<int>{}(static_cast<int>(_scheme)) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<int>{}(static_cast<int>(_orientation)) + 0x9e3779b9 + (h << 6) + (h >> 2);
        for (auto v : _faceVertexCounts) h ^= std::hash<int>{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
        for (auto v : _faceVertexIndices) h ^= std::hash<int>{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
        for (auto v : _holeIndices) h ^= std::hash<int>{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return static_cast<uint64_t>(h);
#endif
    }

    /// Check equality with another topology.
    bool Equals(const PxOsdMeshTopologyRef* other) const SWIFTUSD_NOEXCEPT {
        if (!other) return false;
#if defined(USE_PIXAR_USD)
        return _topology == other->_topology;
#else
        return _scheme == other->_scheme &&
               _orientation == other->_orientation &&
               _faceVertexCounts == other->_faceVertexCounts &&
               _faceVertexIndices == other->_faceVertexIndices &&
               _holeIndices == other->_holeIndices;
#endif
    }

    // ========================================================================
    // MARK: - Reference Counting
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
    PxOsdMeshTopologyRef() SWIFTUSD_NOEXCEPT
        : _scheme(PxOsdSubdivScheme::None),
          _orientation(PxOsdOrientation::RightHanded),
          _refCount(1) {}

    // Copy constructor for builder methods
    PxOsdMeshTopologyRef(const PxOsdMeshTopologyRef& other) SWIFTUSD_NOEXCEPT
#if defined(USE_PIXAR_USD)
        : _topology(other._topology),
#else
        : _scheme(other._scheme),
          _orientation(other._orientation),
          _faceVertexCounts(other._faceVertexCounts),
          _faceVertexIndices(other._faceVertexIndices),
          _holeIndices(other._holeIndices),
          _subdivTags(other._subdivTags),
#endif
          _refCount(1) {}

#if defined(USE_PIXAR_USD)
    explicit PxOsdMeshTopologyRef(const PXR_NS::PxOsdMeshTopology& topology) SWIFTUSD_NOEXCEPT
        : _topology(topology), _refCount(1) {}

    PXR_NS::PxOsdMeshTopology _topology;
#else
    struct SubdivTagsData {
        PxOsdInterpolationRule _vtxInterpolationRule = PxOsdInterpolationRule::None;
        PxOsdInterpolationRule _fvarInterpolationRule = PxOsdInterpolationRule::None;
        PxOsdCreaseMethod _creaseMethod = PxOsdCreaseMethod::None;
        PxOsdTriangleSubdivision _triangleSubdivision = PxOsdTriangleSubdivision::None;
        std::vector<int32_t> _creaseIndices;
        std::vector<int32_t> _creaseLengths;
        std::vector<float> _creaseWeights;
        std::vector<int32_t> _cornerIndices;
        std::vector<float> _cornerWeights;
    };

    PxOsdSubdivScheme _scheme;
    PxOsdOrientation _orientation;
    std::vector<int32_t> _faceVertexCounts;
    std::vector<int32_t> _faceVertexIndices;
    std::vector<int32_t> _holeIndices;
    SubdivTagsData _subdivTags;
#endif

    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - PxOsd Token Constants
// ============================================================================

/// Standard tokens for PxOsd.
namespace PxOsdTokens {
    /// Subdivision schemes
    inline Token CatmullClark() SWIFTUSD_NOEXCEPT { return Token("catmullClark"); }
    inline Token Loop() SWIFTUSD_NOEXCEPT { return Token("loop"); }
    inline Token Bilinear() SWIFTUSD_NOEXCEPT { return Token("bilinear"); }

    /// Interpolation rules
    inline Token None() SWIFTUSD_NOEXCEPT { return Token("none"); }
    inline Token All() SWIFTUSD_NOEXCEPT { return Token("all"); }
    inline Token CornersOnly() SWIFTUSD_NOEXCEPT { return Token("cornersOnly"); }
    inline Token CornersPlus1() SWIFTUSD_NOEXCEPT { return Token("cornersPlus1"); }
    inline Token CornersPlus2() SWIFTUSD_NOEXCEPT { return Token("cornersPlus2"); }
    inline Token Boundaries() SWIFTUSD_NOEXCEPT { return Token("boundaries"); }
    inline Token EdgeOnly() SWIFTUSD_NOEXCEPT { return Token("edgeOnly"); }
    inline Token EdgeAndCorner() SWIFTUSD_NOEXCEPT { return Token("edgeAndCorner"); }

    /// Crease methods
    inline Token Uniform() SWIFTUSD_NOEXCEPT { return Token("uniform"); }
    inline Token Chaikin() SWIFTUSD_NOEXCEPT { return Token("chaikin"); }

    /// Orientations
    inline Token LeftHanded() SWIFTUSD_NOEXCEPT { return Token("leftHanded"); }
    inline Token RightHanded() SWIFTUSD_NOEXCEPT { return Token("rightHanded"); }

    /// Triangle subdivision
    inline Token Smooth() SWIFTUSD_NOEXCEPT { return Token("smooth"); }
}

} // namespace swiftusd
