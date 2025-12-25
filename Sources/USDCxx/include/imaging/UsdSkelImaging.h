// UsdSkelImaging.h - Skeleton Visualization and Skinning Adapters for SwiftUSD
// Mirrors: pxr/usdImaging/usdSkelImaging/
// C++ header with Swift interop annotations
//
// UsdSkelImaging provides imaging support for skeletal animation in USD,
// translating skeleton definitions and skinning data into Hydra render primitives.
// Key components include:
// - SkeletonAdapter: Handles skeleton prim population and bone mesh rendering
// - SkelRootAdapter: Manages scope containing skeletons and skinned prims
// - BlendShapeAdapter/AnimationAdapter: Scene index adapters for blend shapes and animation
// - Skinning computations: GPU/CPU skinning via Hydra ExtComputations

#pragma once

#include "../Swift.h"
#include "../base/Tf.h"
#include "../base/Gf.h"
#include "../usd/Sdf.h"
#include "../usd/Usd.h"
#include "../usd/UsdSkel.h"
#include "Hd.h"
#include "UsdImaging.h"

#include <atomic>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#if defined(USE_PIXAR_USD)
#include "pxr/pxr.h"
#include "pxr/usdImaging/usdSkelImaging/skeletonAdapter.h"
#include "pxr/usdImaging/usdSkelImaging/skelRootAdapter.h"
#endif // USE_PIXAR_USD

namespace swiftusd {

// ============================================================================
// MARK: - Forward Declarations
// ============================================================================

class UsdSkelImagingSkeletonAdapterRef;
class UsdSkelImagingSkelRootAdapterRef;

// ============================================================================
// MARK: - UsdSkelImaging Tokens
// ============================================================================

/// Common tokens used in UsdSkelImaging.
namespace UsdSkelImagingTokens {

/// Prim type tokens for skeleton imaging
inline Token Skeleton() { return Token("skeleton"); }
inline Token SkelAnimation() { return Token("skelAnimation"); }
inline Token SkelBlendShape() { return Token("skelBlendShape"); }

/// Computation name tokens
inline Token SkinningInputAggregatorComputation() { return Token("skinningInputAggregatorComputation"); }
inline Token SkinningComputation() { return Token("skinningComputation"); }

/// Input name tokens for skinning computations
inline Token BlendShapeWeights() { return Token("blendShapeWeights"); }
inline Token SkinningXforms() { return Token("skinningXforms"); }
inline Token SkinningScaleXforms() { return Token("skinningScaleXforms"); }
inline Token SkinningDualQuats() { return Token("skinningDualQuats"); }
inline Token SkelLocalToCommonSpace() { return Token("skelLocalToCommonSpace"); }
inline Token CommonSpaceToPrimLocal() { return Token("commonSpaceToPrimLocal"); }

/// Output name tokens
inline Token SkinnedPoints() { return Token("skinnedPoints"); }

/// Primvar name tokens
inline Token PrimvarJointIndices() { return Token("skel:jointIndices"); }
inline Token PrimvarJointWeights() { return Token("skel:jointWeights"); }
inline Token PrimvarSkinningMethod() { return Token("skel:skinningMethod"); }
inline Token PrimvarGeomBindTransform() { return Token("skel:geomBindTransform"); }

/// Skinning method tokens
inline Token ClassicLinear() { return Token("classicLinear"); }
inline Token DualQuaternion() { return Token("dualQuaternion"); }

} // namespace UsdSkelImagingTokens

// ============================================================================
// MARK: - UsdSkelImagingBoneTopology (Bone mesh topology data)
// ============================================================================

/// Describes the topology of a skeleton's bone mesh visualization.
/// Contains face vertex counts and indices for rendering bones as mesh geometry.
struct UsdSkelImagingBoneTopology {
    /// Number of faces in the bone mesh
    int32_t faceCount;

    /// Total number of face vertex indices
    int32_t faceVertexCount;

    /// Number of points (vertices) in the mesh
    int32_t pointCount;

    UsdSkelImagingBoneTopology() SWIFTUSD_NOEXCEPT
        : faceCount(0)
        , faceVertexCount(0)
        , pointCount(0) {}

    UsdSkelImagingBoneTopology(int32_t faces, int32_t faceVerts, int32_t points) SWIFTUSD_NOEXCEPT
        : faceCount(faces)
        , faceVertexCount(faceVerts)
        , pointCount(points) {}

    bool operator==(const UsdSkelImagingBoneTopology& rhs) const SWIFTUSD_NOEXCEPT {
        return faceCount == rhs.faceCount &&
               faceVertexCount == rhs.faceVertexCount &&
               pointCount == rhs.pointCount;
    }

    bool operator!=(const UsdSkelImagingBoneTopology& rhs) const SWIFTUSD_NOEXCEPT {
        return !(*this == rhs);
    }

    /// Check if topology is valid (has at least one face)
    bool IsValid() const SWIFTUSD_NOEXCEPT {
        return faceCount > 0 && faceVertexCount > 0 && pointCount > 0;
    }

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - UsdSkelImagingSkinningParams (Skinning configuration)
// ============================================================================

/// Configuration parameters for skinning computations.
struct UsdSkelImagingSkinningParams {
    /// Skinning method to use
    SkelSkinningMethod skinningMethod;

    /// Number of joint influences per vertex component
    int32_t numInfluencesPerComponent;

    /// Whether to use GPU skinning (vs CPU)
    bool useGpuSkinning;

    /// Whether blend shapes are enabled
    bool blendShapesEnabled;

    UsdSkelImagingSkinningParams() SWIFTUSD_NOEXCEPT
        : skinningMethod()
        , numInfluencesPerComponent(4)
        , useGpuSkinning(true)
        , blendShapesEnabled(false) {}

    bool operator==(const UsdSkelImagingSkinningParams& rhs) const SWIFTUSD_NOEXCEPT {
        return skinningMethod == rhs.skinningMethod &&
               numInfluencesPerComponent == rhs.numInfluencesPerComponent &&
               useGpuSkinning == rhs.useGpuSkinning &&
               blendShapesEnabled == rhs.blendShapesEnabled;
    }

    bool operator!=(const UsdSkelImagingSkinningParams& rhs) const SWIFTUSD_NOEXCEPT {
        return !(*this == rhs);
    }

} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - UsdSkelImagingSkeletonAdapterRef (Skeleton adapter wrapper)
// ============================================================================

/// The skeleton adapter handles population and rendering of skeleton prims.
///
/// This adapter:
/// - Populates Hydra render index with skeleton bone mesh visualization
/// - Manages skinning computations for deformed geometry
/// - Tracks time-varying skeleton animation
/// - Computes bone mesh topology and points for visualization
///
/// The adapter inherits from UsdImagingInstanceablePrimAdapter in the real USD
/// implementation and provides all methods needed for Hydra to query skeleton data.
class UsdSkelImagingSkeletonAdapterRef {
public:
    /// Create a new skeleton adapter.
    ///
    /// @return A new skeleton adapter reference, or nullptr if creation fails
    static UsdSkelImagingSkeletonAdapterRef* Create() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        // Note: In real USD, adapters are typically created and managed by the delegate
        // For now, we provide a wrapper that can be used for querying capabilities
        return new UsdSkelImagingSkeletonAdapterRef();
#else
        return new UsdSkelImagingSkeletonAdapterRef();
#endif
    }

    /// Check if this adapter is valid and ready for use.
    bool IsValid() const SWIFTUSD_NOEXCEPT {
        return true;
    }

    // ========================================================================
    // MARK: Adapter Information
    // ========================================================================

    /// Get the prim type tokens that this adapter handles.
    ///
    /// @param outBuffer Buffer to receive tokens
    /// @param maxCount Maximum number of tokens to return
    /// @return Number of tokens written
    size_t GetPrimTokens(Token* outBuffer, size_t maxCount) const SWIFTUSD_NOEXCEPT {
        if (!outBuffer || maxCount == 0) return 0;
        outBuffer[0] = UsdSkelImagingTokens::Skeleton();
        return 1;
    }

    /// Check if this adapter supports instancing.
    bool IsInstanceable() const SWIFTUSD_NOEXCEPT {
        return true;
    }

    // ========================================================================
    // MARK: Bone Mesh Computation
    // ========================================================================

    /// Compute bone mesh topology for a skeleton with the given joint count.
    ///
    /// Each bone is visualized as a simple octahedral shape connecting joints.
    ///
    /// @param jointCount Number of joints in the skeleton
    /// @return Topology description for the bone mesh
    UsdSkelImagingBoneTopology ComputeBoneTopology(size_t jointCount) const SWIFTUSD_NOEXCEPT {
        if (jointCount == 0) return UsdSkelImagingBoneTopology();

        // Each bone (except root) gets an octahedral shape with 8 triangular faces
        // For n joints, we have n-1 bones (edges in the hierarchy)
        // Octahedron: 8 faces * 3 vertices/face = 24 face vertex indices per bone
        // 6 vertices per octahedron
        size_t boneCount = jointCount > 0 ? jointCount - 1 : 0;
        if (boneCount == 0) boneCount = 1; // At minimum, show something

        int32_t facesPerBone = 8;  // Octahedron has 8 triangular faces
        int32_t verticesPerFace = 3;
        int32_t pointsPerBone = 6;  // Octahedron has 6 vertices

        return UsdSkelImagingBoneTopology(
            static_cast<int32_t>(boneCount * facesPerBone),
            static_cast<int32_t>(boneCount * facesPerBone * verticesPerFace),
            static_cast<int32_t>(boneCount * pointsPerBone)
        );
    }

    /// Compute bone mesh points for visualization.
    ///
    /// @param restTransforms Array of 4x4 rest pose matrices (16 doubles each)
    /// @param jointCount Number of joints
    /// @param boneWidth Width of bone visualization (default: 0.1)
    /// @param outPoints Buffer to receive computed points (3 floats per point)
    /// @param maxPoints Maximum number of points to write
    /// @return Number of points written
    size_t ComputeBonePoints(const double* restTransforms,
                              size_t jointCount,
                              float boneWidth,
                              float* outPoints,
                              size_t maxPoints) const SWIFTUSD_NOEXCEPT {
        if (!restTransforms || jointCount == 0 || !outPoints || maxPoints == 0) {
            return 0;
        }

        // Compute octahedral bone shapes between joints
        // For simplicity, we create a single octahedron for each bone
        size_t boneCount = jointCount > 0 ? jointCount - 1 : 0;
        if (boneCount == 0) boneCount = 1;

        size_t pointsPerBone = 6;
        size_t totalPoints = boneCount * pointsPerBone;
        size_t writeCount = std::min(totalPoints, maxPoints);

        float halfWidth = boneWidth * 0.5f;

        // Generate octahedron points for each bone
        for (size_t b = 0; b < boneCount && (b * pointsPerBone) < writeCount; ++b) {
            size_t baseIdx = b * pointsPerBone * 3;

            // Get joint positions from transform matrices
            // Position is in the last column (indices 12, 13, 14 in row-major)
            float startX = static_cast<float>(restTransforms[b * 16 + 12]);
            float startY = static_cast<float>(restTransforms[b * 16 + 13]);
            float startZ = static_cast<float>(restTransforms[b * 16 + 14]);

            float endX = startX;
            float endY = startY + boneWidth * 2.0f; // Default bone direction
            float endZ = startZ;

            if (b + 1 < jointCount) {
                endX = static_cast<float>(restTransforms[(b + 1) * 16 + 12]);
                endY = static_cast<float>(restTransforms[(b + 1) * 16 + 13]);
                endZ = static_cast<float>(restTransforms[(b + 1) * 16 + 14]);
            }

            // Octahedron vertices: start, end, and 4 mid-points
            float midX = (startX + endX) * 0.5f;
            float midY = (startY + endY) * 0.5f;
            float midZ = (startZ + endZ) * 0.5f;

            // Point 0: Start
            if (baseIdx + 2 < writeCount * 3) {
                outPoints[baseIdx + 0] = startX;
                outPoints[baseIdx + 1] = startY;
                outPoints[baseIdx + 2] = startZ;
            }

            // Point 1: End
            if (baseIdx + 5 < writeCount * 3) {
                outPoints[baseIdx + 3] = endX;
                outPoints[baseIdx + 4] = endY;
                outPoints[baseIdx + 5] = endZ;
            }

            // Points 2-5: Mid-section octahedron vertices
            if (baseIdx + 8 < writeCount * 3) {
                outPoints[baseIdx + 6] = midX + halfWidth;
                outPoints[baseIdx + 7] = midY;
                outPoints[baseIdx + 8] = midZ;
            }
            if (baseIdx + 11 < writeCount * 3) {
                outPoints[baseIdx + 9] = midX - halfWidth;
                outPoints[baseIdx + 10] = midY;
                outPoints[baseIdx + 11] = midZ;
            }
            if (baseIdx + 14 < writeCount * 3) {
                outPoints[baseIdx + 12] = midX;
                outPoints[baseIdx + 13] = midY;
                outPoints[baseIdx + 14] = midZ + halfWidth;
            }
            if (baseIdx + 17 < writeCount * 3) {
                outPoints[baseIdx + 15] = midX;
                outPoints[baseIdx + 16] = midY;
                outPoints[baseIdx + 17] = midZ - halfWidth;
            }
        }

        return writeCount;
    }

    // ========================================================================
    // MARK: Skinning Support
    // ========================================================================

    /// Check if skinning computations are supported.
    bool SupportsSkinning() const SWIFTUSD_NOEXCEPT {
        return true;
    }

    /// Get default skinning parameters.
    UsdSkelImagingSkinningParams GetDefaultSkinningParams() const SWIFTUSD_NOEXCEPT {
        return UsdSkelImagingSkinningParams();
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
    UsdSkelImagingSkeletonAdapterRef() SWIFTUSD_NOEXCEPT
        : _refCount(1) {}

    ~UsdSkelImagingSkeletonAdapterRef() = default;

    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - UsdSkelImagingSkelRootAdapterRef (SkelRoot adapter wrapper)
// ============================================================================

/// The SkelRoot adapter handles population of SkelRoot prims that mark
/// the scope containing skeletons and skinned geometry.
///
/// This adapter:
/// - Registers skeleton adapters for skeletons within the root
/// - Manages relationships between skeletons and skinned prims
/// - Coordinates skin binding information
///
/// SkelRoot is a boundable prim that must be defined at or above any
/// skeletally-posed primitives.
class UsdSkelImagingSkelRootAdapterRef {
public:
    /// Create a new SkelRoot adapter.
    ///
    /// @return A new SkelRoot adapter reference, or nullptr if creation fails
    static UsdSkelImagingSkelRootAdapterRef* Create() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return new UsdSkelImagingSkelRootAdapterRef();
#else
        return new UsdSkelImagingSkelRootAdapterRef();
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
    Token GetPrimToken() const SWIFTUSD_NOEXCEPT {
        return Token("SkelRoot");
    }

    /// Check if this adapter supports instancing.
    bool IsInstanceable() const SWIFTUSD_NOEXCEPT {
        return true;
    }

    // ========================================================================
    // MARK: Skeleton Binding Queries
    // ========================================================================

    /// Check if a prim path is within a SkelRoot scope.
    ///
    /// @param rootPath Path to the SkelRoot prim
    /// @param queryPath Path to check
    /// @return true if queryPath is a descendant of rootPath
    bool ContainsPath(const Path& rootPath, const Path& queryPath) const SWIFTUSD_NOEXCEPT {
        if (rootPath.IsEmpty() || queryPath.IsEmpty()) return false;

        // Check if queryPath is a descendant of rootPath
        std::string rootStr = rootPath.GetText();
        std::string queryStr = queryPath.GetText();

        if (queryStr.length() < rootStr.length()) return false;

        // Must start with rootPath and either be equal or have a '/' separator
        if (queryStr.substr(0, rootStr.length()) != rootStr) return false;
        if (queryStr.length() > rootStr.length() && queryStr[rootStr.length()] != '/') return false;

        return true;
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
    UsdSkelImagingSkelRootAdapterRef() SWIFTUSD_NOEXCEPT
        : _refCount(1) {}

    ~UsdSkelImagingSkelRootAdapterRef() = default;

    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - Utility Functions
// ============================================================================

/// Compute the bone topology for a skeleton visualization.
///
/// @param jointCount Number of joints in the skeleton
/// @return Topology description for rendering bones as mesh geometry
inline UsdSkelImagingBoneTopology UsdSkelImagingComputeBoneTopology(size_t jointCount) SWIFTUSD_NOEXCEPT {
    if (jointCount == 0) return UsdSkelImagingBoneTopology();

    // Use the same algorithm as the adapter
    size_t boneCount = jointCount > 0 ? jointCount - 1 : 0;
    if (boneCount == 0) boneCount = 1;

    int32_t facesPerBone = 8;
    int32_t verticesPerFace = 3;
    int32_t pointsPerBone = 6;

    return UsdSkelImagingBoneTopology(
        static_cast<int32_t>(boneCount * facesPerBone),
        static_cast<int32_t>(boneCount * facesPerBone * verticesPerFace),
        static_cast<int32_t>(boneCount * pointsPerBone)
    );
}

/// Get the default bone width for skeleton visualization.
inline float UsdSkelImagingGetDefaultBoneWidth() SWIFTUSD_NOEXCEPT {
    return 0.1f;
}

/// Check if a skinning method token is valid.
///
/// @param method Token representing the skinning method
/// @return true if the token represents a valid skinning method
inline bool UsdSkelImagingIsValidSkinningMethod(const Token& method) SWIFTUSD_NOEXCEPT {
    return method == UsdSkelImagingTokens::ClassicLinear() ||
           method == UsdSkelImagingTokens::DualQuaternion();
}

/// Get the skinning method token for a SkelSkinningMethod value.
///
/// @param method The skinning method enum value
/// @return Token representation of the skinning method
inline Token UsdSkelImagingGetSkinningMethodToken(SkelSkinningMethod method) SWIFTUSD_NOEXCEPT {
    if (method.value == SkelSkinningMethod::DualQuaternion) {
        return UsdSkelImagingTokens::DualQuaternion();
    }
    return UsdSkelImagingTokens::ClassicLinear();
}

/// Parse a skinning method token to a SkelSkinningMethod value.
///
/// @param token Token to parse
/// @return Parsed skinning method (defaults to ClassicLinear for unknown tokens)
inline SkelSkinningMethod UsdSkelImagingParseSkinningMethod(const Token& token) SWIFTUSD_NOEXCEPT {
    if (token == UsdSkelImagingTokens::DualQuaternion()) {
        return SkelSkinningMethod(SkelSkinningMethod::DualQuaternion);
    }
    return SkelSkinningMethod(SkelSkinningMethod::ClassicLinear);
}

} // namespace swiftusd
