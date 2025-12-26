// UsdSkel.hpp - Skeletal Animation Schemas facade
// CLEAN HEADER - NO pxr includes!
//
// This module provides wrappers for UsdSkel schema classes including skeletons,
// skeleton roots, animations, blend shapes, and related API schemas.
#pragma once

#include "../CxxFacade.h"
#include "../usd/Usd.hpp"
#include "../usd/Sdf.hpp"
#include "../base/Gf.hpp"
#include "../base/Tf.hpp"
#include "../base/Vt.hpp"
#include "../usdGeom/UsdGeom.hpp"

namespace cxxfacade {
namespace usdSkel {

// ============================================================================
// Forward Declarations
// ============================================================================

class UsdSkelSkeleton;
class UsdSkelRoot;
class UsdSkelAnimation;
class UsdSkelBindingAPI;
class UsdSkelBlendShape;
class UsdSkelTopology;
class UsdSkelCache;
class UsdSkelAnimQuery;
class UsdSkelSkeletonQuery;
class UsdSkelSkinningQuery;

// ============================================================================
// UsdSkelTopology - Skeleton topology representation
// ============================================================================

/// UsdSkelTopology - Object holding information describing skeleton topology.
///
/// This provides the hierarchical information needed to reason about joint
/// relationships in a manner suitable to computations.
class UsdSkelTopology {
public:
    UsdSkelTopology();
    UsdSkelTopology(const UsdSkelTopology& other);
    UsdSkelTopology(UsdSkelTopology&& other) noexcept;
    ~UsdSkelTopology();

    UsdSkelTopology& operator=(const UsdSkelTopology& other);
    UsdSkelTopology& operator=(UsdSkelTopology&& other) noexcept;

    /// Construct a topology from an array of joint path tokens
    explicit UsdSkelTopology(const std::vector<tf::TfToken>& jointPaths);

    /// Construct a topology from an array of parent indices
    explicit UsdSkelTopology(const std::vector<int>& parentIndices);

    /// Check if this topology is valid
    bool IsValid() const;
    explicit operator bool() const { return IsValid(); }

    /// Validate the topology
    bool Validate(std::string* reason = nullptr) const;

    /// Get the parent indices array
    std::vector<int> GetParentIndices() const;

    /// Get the number of joints
    size_t GetNumJoints() const;

    /// Get the parent of a joint (returns -1 for root joints)
    int GetParent(size_t index) const;

    /// Check if a joint is a root joint
    bool IsRoot(size_t index) const;

    /// Equality comparison
    bool operator==(const UsdSkelTopology& other) const;
    bool operator!=(const UsdSkelTopology& other) const;

    void* GetImpl() const { return impl_; }
    static UsdSkelTopology FromImpl(void* impl);

private:
    void* impl_;
};

// ============================================================================
// UsdSkelSkeleton - Skeleton prim schema
// ============================================================================

/// UsdSkelSkeleton - Describes a skeleton.
///
/// A skeleton is defined by a set of joints, their hierarchy (expressed through
/// path tokens), and bind/rest transforms.
class UsdSkelSkeleton : public usdGeom::UsdGeomBoundable {
public:
    explicit UsdSkelSkeleton(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdSkelSkeleton(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdSkelSkeleton();

    /// Get this prim at path on stage
    static UsdSkelSkeleton Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Define a skeleton at the given path
    static UsdSkelSkeleton Define(usd::UsdStage& stage, const sdf::SdfPath& path);

    // ----- Joints -----

    /// Get the joints attribute - ordered joint paths
    usd::UsdAttribute GetJointsAttr() const;

    /// Create the joints attribute
    usd::UsdAttribute CreateJointsAttr();

    /// Get the jointNames attribute - optional unique names
    usd::UsdAttribute GetJointNamesAttr() const;

    /// Create the jointNames attribute
    usd::UsdAttribute CreateJointNamesAttr();

    // ----- Transforms -----

    /// Get the bindTransforms attribute - world-space bind poses
    usd::UsdAttribute GetBindTransformsAttr() const;

    /// Create the bindTransforms attribute
    usd::UsdAttribute CreateBindTransformsAttr();

    /// Get the restTransforms attribute - local-space rest poses
    usd::UsdAttribute GetRestTransformsAttr() const;

    /// Create the restTransforms attribute
    usd::UsdAttribute CreateRestTransformsAttr();

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdSkelRoot - Skeleton root prim schema
// ============================================================================

/// UsdSkelRoot - Boundable prim type used to identify a scope beneath which
/// skeletally-posed primitives are defined.
///
/// A SkelRoot must be defined at or above a skinned primitive for any skinning
/// behaviors in UsdSkel.
class UsdSkelRoot : public usdGeom::UsdGeomBoundable {
public:
    explicit UsdSkelRoot(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdSkelRoot(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdSkelRoot();

    /// Get this prim at path on stage
    static UsdSkelRoot Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Define a skeleton root at the given path
    static UsdSkelRoot Define(usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Find the skel root at or above the given prim
    static UsdSkelRoot Find(const usd::UsdPrim& prim);

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdSkelAnimation - Skeletal animation prim schema
// ============================================================================

/// UsdSkelAnimation - Describes a skel animation, where joint animation is
/// stored in a vectorized form.
class UsdSkelAnimation : public usd::UsdTyped {
public:
    explicit UsdSkelAnimation(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdSkelAnimation(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdSkelAnimation();

    /// Get this prim at path on stage
    static UsdSkelAnimation Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Define an animation at the given path
    static UsdSkelAnimation Define(usd::UsdStage& stage, const sdf::SdfPath& path);

    // ----- Joints -----

    /// Get the joints attribute - which joints this animation applies to
    usd::UsdAttribute GetJointsAttr() const;

    /// Create the joints attribute
    usd::UsdAttribute CreateJointsAttr();

    // ----- Joint Transforms -----

    /// Get the translations attribute - joint-local translations
    usd::UsdAttribute GetTranslationsAttr() const;

    /// Create the translations attribute
    usd::UsdAttribute CreateTranslationsAttr();

    /// Get the rotations attribute - joint-local quaternion rotations
    usd::UsdAttribute GetRotationsAttr() const;

    /// Create the rotations attribute
    usd::UsdAttribute CreateRotationsAttr();

    /// Get the scales attribute - joint-local scales
    usd::UsdAttribute GetScalesAttr() const;

    /// Create the scales attribute
    usd::UsdAttribute CreateScalesAttr();

    // ----- Blend Shapes -----

    /// Get the blendShapes attribute - which blend shapes this animation applies to
    usd::UsdAttribute GetBlendShapesAttr() const;

    /// Create the blendShapes attribute
    usd::UsdAttribute CreateBlendShapesAttr();

    /// Get the blendShapeWeights attribute - weights for each blend shape
    usd::UsdAttribute GetBlendShapeWeightsAttr() const;

    /// Create the blendShapeWeights attribute
    usd::UsdAttribute CreateBlendShapeWeightsAttr();

    // ----- Convenience Methods -----

    /// Get resolved transforms at the given time
    bool GetTransforms(std::vector<gf::Matrix4d>* xforms,
                       usd::UsdTimeCode time = usd::UsdTimeCode::Default()) const;

    /// Set transforms (must be orthogonal)
    bool SetTransforms(const std::vector<gf::Matrix4d>& xforms,
                       usd::UsdTimeCode time = usd::UsdTimeCode::Default());

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdSkelBindingAPI - API for skeletal bindings
// ============================================================================

/// UsdSkelBindingAPI - API for authoring and extracting skinning-related data.
///
/// This schema provides properties for binding skeletons, animations, and
/// joint influences to geometry prims.
class UsdSkelBindingAPI : public usd::UsdAPISchemaBase {
public:
    explicit UsdSkelBindingAPI(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdSkelBindingAPI(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdSkelBindingAPI();

    /// Get this API on the prim at path on stage
    static UsdSkelBindingAPI Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Apply this API schema to the given prim
    static UsdSkelBindingAPI Apply(const usd::UsdPrim& prim);

    /// Check if this API can be applied to the given prim
    static bool CanApply(const usd::UsdPrim& prim, std::string* whyNot = nullptr);

    // ----- Skinning Method -----

    /// Get the skinning method attribute
    usd::UsdAttribute GetSkinningMethodAttr() const;

    /// Create the skinning method attribute
    usd::UsdAttribute CreateSkinningMethodAttr();

    // ----- Geom Bind Transform -----

    /// Get the geom bind transform attribute
    usd::UsdAttribute GetGeomBindTransformAttr() const;

    /// Create the geom bind transform attribute
    usd::UsdAttribute CreateGeomBindTransformAttr();

    // ----- Joints -----

    /// Get the joints attribute
    usd::UsdAttribute GetJointsAttr() const;

    /// Create the joints attribute
    usd::UsdAttribute CreateJointsAttr();

    /// Get the joint indices attribute
    usd::UsdAttribute GetJointIndicesAttr() const;

    /// Create the joint indices attribute
    usd::UsdAttribute CreateJointIndicesAttr();

    /// Get the joint weights attribute
    usd::UsdAttribute GetJointWeightsAttr() const;

    /// Create the joint weights attribute
    usd::UsdAttribute CreateJointWeightsAttr();

    // ----- Blend Shapes -----

    /// Get the blend shapes attribute
    usd::UsdAttribute GetBlendShapesAttr() const;

    /// Create the blend shapes attribute
    usd::UsdAttribute CreateBlendShapesAttr();

    // ----- Relationships -----

    /// Get the animation source relationship
    usd::UsdRelationship GetAnimationSourceRel() const;

    /// Create the animation source relationship
    usd::UsdRelationship CreateAnimationSourceRel();

    /// Get the skeleton relationship
    usd::UsdRelationship GetSkeletonRel() const;

    /// Create the skeleton relationship
    usd::UsdRelationship CreateSkeletonRel();

    /// Get the blend shape targets relationship
    usd::UsdRelationship GetBlendShapeTargetsRel() const;

    /// Create the blend shape targets relationship
    usd::UsdRelationship CreateBlendShapeTargetsRel();

    // ----- Convenience Methods -----

    /// Get the joint indices as a primvar
    usdGeom::UsdGeomPrimvar GetJointIndicesPrimvar() const;

    /// Create the joint indices primvar
    usdGeom::UsdGeomPrimvar CreateJointIndicesPrimvar(bool constant, int elementSize = -1);

    /// Get the joint weights as a primvar
    usdGeom::UsdGeomPrimvar GetJointWeightsPrimvar() const;

    /// Create the joint weights primvar
    usdGeom::UsdGeomPrimvar CreateJointWeightsPrimvar(bool constant, int elementSize = -1);

    /// Set rigid joint influence (single joint for entire prim)
    bool SetRigidJointInfluence(int jointIndex, float weight = 1.0f);

    /// Get the bound skeleton
    bool GetSkeleton(UsdSkelSkeleton* skel) const;

    /// Get the animation source prim
    bool GetAnimationSource(usd::UsdPrim* prim) const;

    /// Get the inherited skeleton
    UsdSkelSkeleton GetInheritedSkeleton() const;

    /// Get the inherited animation source
    usd::UsdPrim GetInheritedAnimationSource() const;

    /// Validate joint indices
    static bool ValidateJointIndices(const std::vector<int>& indices,
                                     size_t numJoints,
                                     std::string* reason = nullptr);

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdSkelBlendShape - Blend shape prim schema
// ============================================================================

/// UsdSkelBlendShape - Describes a target blend shape.
class UsdSkelBlendShape : public usd::UsdTyped {
public:
    explicit UsdSkelBlendShape(const usd::UsdPrim& prim = usd::UsdPrim());
    explicit UsdSkelBlendShape(const usd::UsdSchemaBase& schemaObj);
    virtual ~UsdSkelBlendShape();

    /// Get this prim at path on stage
    static UsdSkelBlendShape Get(const usd::UsdStage& stage, const sdf::SdfPath& path);

    /// Define a blend shape at the given path
    static UsdSkelBlendShape Define(usd::UsdStage& stage, const sdf::SdfPath& path);

    // ----- Offsets -----

    /// Get the offsets attribute - position offsets from base pose
    usd::UsdAttribute GetOffsetsAttr() const;

    /// Create the offsets attribute
    usd::UsdAttribute CreateOffsetsAttr();

    /// Get the normalOffsets attribute - normal offsets from base pose
    usd::UsdAttribute GetNormalOffsetsAttr() const;

    /// Create the normalOffsets attribute
    usd::UsdAttribute CreateNormalOffsetsAttr();

    // ----- Point Indices -----

    /// Get the pointIndices attribute - sparse indices into the mesh
    usd::UsdAttribute GetPointIndicesAttr() const;

    /// Create the pointIndices attribute
    usd::UsdAttribute CreatePointIndicesAttr();

    // ----- Inbetweens -----

    /// Check if this blend shape has an inbetween with the given name
    bool HasInbetween(const tf::TfToken& name) const;

    /// Validate point indices
    static bool ValidatePointIndices(const std::vector<int>& indices,
                                     size_t numPoints,
                                     std::string* reason = nullptr);

protected:
    usd::UsdSchemaKind _GetSchemaKind() const override;
};

// ============================================================================
// UsdSkelAnimQuery - Animation query object
// ============================================================================

/// UsdSkelAnimQuery - Class providing efficient queries of animation primitives.
class UsdSkelAnimQuery {
public:
    UsdSkelAnimQuery();
    UsdSkelAnimQuery(const UsdSkelAnimQuery& other);
    UsdSkelAnimQuery(UsdSkelAnimQuery&& other) noexcept;
    ~UsdSkelAnimQuery();

    UsdSkelAnimQuery& operator=(const UsdSkelAnimQuery& other);
    UsdSkelAnimQuery& operator=(UsdSkelAnimQuery&& other) noexcept;

    /// Check if this query is valid
    bool IsValid() const;
    explicit operator bool() const { return IsValid(); }

    /// Get the prim this query reads from
    usd::UsdPrim GetPrim() const;

    /// Compute joint local transforms
    bool ComputeJointLocalTransforms(std::vector<gf::Matrix4d>* xforms,
                                     usd::UsdTimeCode time = usd::UsdTimeCode::Default()) const;

    /// Compute blend shape weights
    bool ComputeBlendShapeWeights(std::vector<float>* weights,
                                  usd::UsdTimeCode time = usd::UsdTimeCode::Default()) const;

    /// Get the joint order
    std::vector<tf::TfToken> GetJointOrder() const;

    /// Get the blend shape order
    std::vector<tf::TfToken> GetBlendShapeOrder() const;

    /// Check if joint transforms might be time-varying
    bool JointTransformsMightBeTimeVarying() const;

    /// Check if blend shape weights might be time-varying
    bool BlendShapeWeightsMightBeTimeVarying() const;

    /// Equality comparison
    bool operator==(const UsdSkelAnimQuery& other) const;
    bool operator!=(const UsdSkelAnimQuery& other) const;

    void* GetImpl() const { return impl_; }
    static UsdSkelAnimQuery FromImpl(void* impl);

private:
    void* impl_;
};

// ============================================================================
// UsdSkelSkeletonQuery - Skeleton query object
// ============================================================================

/// UsdSkelSkeletonQuery - Primary interface to reading bound skeleton data.
class UsdSkelSkeletonQuery {
public:
    UsdSkelSkeletonQuery();
    UsdSkelSkeletonQuery(const UsdSkelSkeletonQuery& other);
    UsdSkelSkeletonQuery(UsdSkelSkeletonQuery&& other) noexcept;
    ~UsdSkelSkeletonQuery();

    UsdSkelSkeletonQuery& operator=(const UsdSkelSkeletonQuery& other);
    UsdSkelSkeletonQuery& operator=(UsdSkelSkeletonQuery&& other) noexcept;

    /// Check if this query is valid
    bool IsValid() const;
    explicit operator bool() const { return IsValid(); }

    /// Check if the skeleton has a bind pose
    bool HasBindPose() const;

    /// Check if the skeleton has a rest pose
    bool HasRestPose() const;

    /// Get the prim
    usd::UsdPrim GetPrim() const;

    /// Get the skeleton
    UsdSkelSkeleton GetSkeleton() const;

    /// Get the animation query
    UsdSkelAnimQuery GetAnimQuery() const;

    /// Get the topology
    UsdSkelTopology GetTopology() const;

    /// Get the joint order
    std::vector<tf::TfToken> GetJointOrder() const;

    /// Compute joint local transforms
    bool ComputeJointLocalTransforms(std::vector<gf::Matrix4d>* xforms,
                                     usd::UsdTimeCode time,
                                     bool atRest = false) const;

    /// Compute joint skeleton-space transforms
    bool ComputeJointSkelTransforms(std::vector<gf::Matrix4d>* xforms,
                                    usd::UsdTimeCode time,
                                    bool atRest = false) const;

    /// Compute skinning transforms
    bool ComputeSkinningTransforms(std::vector<gf::Matrix4d>* xforms,
                                   usd::UsdTimeCode time) const;

    /// Get joint world bind transforms
    bool GetJointWorldBindTransforms(std::vector<gf::Matrix4d>* xforms) const;

    /// Equality comparison
    bool operator==(const UsdSkelSkeletonQuery& other) const;
    bool operator!=(const UsdSkelSkeletonQuery& other) const;

    void* GetImpl() const { return impl_; }
    static UsdSkelSkeletonQuery FromImpl(void* impl);

private:
    void* impl_;
};

// ============================================================================
// UsdSkelSkinningQuery - Skinning query object
// ============================================================================

/// UsdSkelSkinningQuery - Object used for querying resolved skinning bindings.
class UsdSkelSkinningQuery {
public:
    UsdSkelSkinningQuery();
    UsdSkelSkinningQuery(const UsdSkelSkinningQuery& other);
    UsdSkelSkinningQuery(UsdSkelSkinningQuery&& other) noexcept;
    ~UsdSkelSkinningQuery();

    UsdSkelSkinningQuery& operator=(const UsdSkelSkinningQuery& other);
    UsdSkelSkinningQuery& operator=(UsdSkelSkinningQuery&& other) noexcept;

    /// Check if this query is valid
    bool IsValid() const;
    explicit operator bool() const { return IsValid(); }

    /// Get the prim
    usd::UsdPrim GetPrim() const;

    /// Check if there are blend shapes
    bool HasBlendShapes() const;

    /// Check if there are joint influences
    bool HasJointInfluences() const;

    /// Get the number of influences per component
    int GetNumInfluencesPerComponent() const;

    /// Get the interpolation
    tf::TfToken GetInterpolation() const;

    /// Check if rigidly deformed
    bool IsRigidlyDeformed() const;

    /// Get the skinning method
    tf::TfToken GetSkinningMethod() const;

    /// Get the geometry bind transform
    gf::Matrix4d GetGeomBindTransform(usd::UsdTimeCode time = usd::UsdTimeCode::Default()) const;

    /// Compute joint influences
    bool ComputeJointInfluences(std::vector<int>* indices,
                                std::vector<float>* weights,
                                usd::UsdTimeCode time = usd::UsdTimeCode::Default()) const;

    /// Compute skinned points
    bool ComputeSkinnedPoints(const std::vector<gf::Matrix4d>& xforms,
                              std::vector<gf::Vec3f>* points,
                              usd::UsdTimeCode time = usd::UsdTimeCode::Default()) const;

    void* GetImpl() const { return impl_; }
    static UsdSkelSkinningQuery FromImpl(void* impl);

private:
    void* impl_;
};

// ============================================================================
// UsdSkelCache - Cache for skeleton queries
// ============================================================================

/// UsdSkelCache - Thread-safe cache for accessing skeleton query objects.
class UsdSkelCache {
public:
    UsdSkelCache();
    UsdSkelCache(const UsdSkelCache& other);
    UsdSkelCache(UsdSkelCache&& other) noexcept;
    ~UsdSkelCache();

    UsdSkelCache& operator=(const UsdSkelCache& other);
    UsdSkelCache& operator=(UsdSkelCache&& other) noexcept;

    /// Clear the cache
    void Clear();

    /// Populate the cache for a skeleton root
    bool Populate(const UsdSkelRoot& root);

    /// Get a skeleton query for a skeleton
    UsdSkelSkeletonQuery GetSkelQuery(const UsdSkelSkeleton& skel) const;

    /// Get an animation query for an animation
    UsdSkelAnimQuery GetAnimQuery(const UsdSkelAnimation& anim) const;

    /// Get a skinning query for a prim
    UsdSkelSkinningQuery GetSkinningQuery(const usd::UsdPrim& prim) const;

    void* GetImpl() const { return impl_; }

private:
    void* impl_;
};

// ============================================================================
// Token Constants
// ============================================================================

/// Common tokens used in UsdSkel
namespace UsdSkelTokens {
    // Schema types
    tf::TfToken skeleton();
    tf::TfToken skelRoot();
    tf::TfToken skelAnimation();
    tf::TfToken blendShape();

    // Skinning methods
    tf::TfToken classicLinear();
    tf::TfToken dualQuaternion();

    // Joint weights interpolation
    tf::TfToken constant();
    tf::TfToken vertex();
}

// ============================================================================
// Utility Functions
// ============================================================================

/// Compute joint local transforms from TRS components
bool ComputeJointLocalTransforms(const UsdSkelTopology& topology,
                                 const std::vector<gf::Vec3f>& translations,
                                 const std::vector<gf::Quatf>& rotations,
                                 const std::vector<gf::Vec3f>& scales,
                                 std::vector<gf::Matrix4d>* xforms);

/// Concatenate joint transforms from local to skeleton space
bool ConcatJointTransforms(const UsdSkelTopology& topology,
                           const std::vector<gf::Matrix4d>& localXforms,
                           std::vector<gf::Matrix4d>* skelXforms);

/// Compute skinning transforms from skeleton transforms and bind transforms
bool ComputeSkinningTransforms(const std::vector<gf::Matrix4d>& skelXforms,
                               const std::vector<gf::Matrix4d>& bindXforms,
                               std::vector<gf::Matrix4d>* skinningXforms);

/// Deform points using linear blend skinning
bool DeformPointsWithLBS(const std::vector<gf::Matrix4d>& skinningXforms,
                         const std::vector<int>& jointIndices,
                         const std::vector<float>& jointWeights,
                         int numInfluencesPerPoint,
                         std::vector<gf::Vec3f>* points);

} // namespace usdSkel
} // namespace cxxfacade
