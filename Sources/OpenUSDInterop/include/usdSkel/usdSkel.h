// usdSkel.h - UsdSkel module C wrapper
// Mirrors: pxr/usd/usdSkel/*.h

#ifndef OPENUSDINTEROP_USDSKEL_USDSKEL_H
#define OPENUSDINTEROP_USDSKEL_USDSKEL_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Cross-module dependencies
#include "../base/tf.h"
#include "../base/gf.h"
#include "../base/vt.h"
#include "../usd/sdf.h"
#include "../usd/usd.h"
#include "../usdGeom/usdGeom.h"

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

typedef struct UsdSkelRootOpaque* UsdSkelRootRef;
typedef struct UsdSkelSkeletonOpaque* UsdSkelSkeletonRef;
typedef struct UsdSkelAnimationOpaque* UsdSkelAnimationRef;
typedef struct UsdSkelBlendShapeOpaque* UsdSkelBlendShapeRef;
typedef struct UsdSkelBindingAPIOpaque* UsdSkelBindingAPIRef;

// ============================================================================
// MARK: - Enums
// ============================================================================

/// Skinning method for skeletal deformation
typedef enum {
    USDSKEL_SKINNING_METHOD_CLASSIC_LINEAR = 0,  ///< Classic linear blend skinning
    USDSKEL_SKINNING_METHOD_DUAL_QUATERNION = 1  ///< Dual quaternion skinning
} UsdSkelSkinningMethod;

// ============================================================================
// MARK: - UsdSkelRoot (Skeleton Root)
// ============================================================================

/// Gets a UsdSkelRoot from a stage and path.
/// Caller must release with UsdSkelRoot_Release.
USD_INTEROP_API UsdSkelRootRef UsdSkelRoot_Get(UsdStageRef stage, SdfPathRef path);

/// Defines a new SkelRoot prim at the given path.
/// Caller must release with UsdSkelRoot_Release.
USD_INTEROP_API UsdSkelRootRef UsdSkelRoot_Define(UsdStageRef stage, SdfPathRef path);

/// Creates a SkelRoot from a prim.
/// Caller must release with UsdSkelRoot_Release.
USD_INTEROP_API UsdSkelRootRef UsdSkelRoot_FromPrim(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdSkelRootRef UsdSkelRoot_Retain(UsdSkelRootRef root);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdSkelRoot_Release(UsdSkelRootRef root);

/// Returns true if the SkelRoot is valid.
USD_INTEROP_API bool UsdSkelRoot_IsValid(UsdSkelRootRef root);

/// Gets the underlying prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdSkelRoot_GetPrim(UsdSkelRootRef root);

/// Finds the SkelRoot at or above the given prim.
/// Caller must release with UsdSkelRoot_Release.
USD_INTEROP_API UsdSkelRootRef UsdSkelRoot_Find(UsdPrimRef prim);

// ============================================================================
// MARK: - UsdSkelSkeleton (Skeleton Definition)
// ============================================================================

/// Gets a UsdSkelSkeleton from a stage and path.
/// Caller must release with UsdSkelSkeleton_Release.
USD_INTEROP_API UsdSkelSkeletonRef UsdSkelSkeleton_Get(UsdStageRef stage, SdfPathRef path);

/// Defines a new Skeleton prim at the given path.
/// Caller must release with UsdSkelSkeleton_Release.
USD_INTEROP_API UsdSkelSkeletonRef UsdSkelSkeleton_Define(UsdStageRef stage, SdfPathRef path);

/// Creates a Skeleton from a prim.
/// Caller must release with UsdSkelSkeleton_Release.
USD_INTEROP_API UsdSkelSkeletonRef UsdSkelSkeleton_FromPrim(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdSkelSkeletonRef UsdSkelSkeleton_Retain(UsdSkelSkeletonRef skeleton);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdSkelSkeleton_Release(UsdSkelSkeletonRef skeleton);

/// Returns true if the Skeleton is valid.
USD_INTEROP_API bool UsdSkelSkeleton_IsValid(UsdSkelSkeletonRef skeleton);

/// Gets the underlying prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdSkelSkeleton_GetPrim(UsdSkelSkeletonRef skeleton);

// MARK: - Skeleton Joints

/// Gets the number of joints in the skeleton.
USD_INTEROP_API size_t UsdSkelSkeleton_GetJointCount(UsdSkelSkeletonRef skeleton);

/// Gets the joint paths (tokens).
/// outJoints must have space for maxCount tokens.
/// Caller must release each token with TfToken_Release.
USD_INTEROP_API size_t UsdSkelSkeleton_GetJoints(
    UsdSkelSkeletonRef skeleton,
    TfTokenRef* outJoints,
    size_t maxCount
);

/// Sets the joint paths.
USD_INTEROP_API UsdResult UsdSkelSkeleton_SetJoints(
    UsdSkelSkeletonRef skeleton,
    const TfTokenRef* joints,
    size_t count
);

/// Gets the joint names.
/// outNames must have space for maxCount tokens.
/// Caller must release each token with TfToken_Release.
USD_INTEROP_API size_t UsdSkelSkeleton_GetJointNames(
    UsdSkelSkeletonRef skeleton,
    TfTokenRef* outNames,
    size_t maxCount
);

/// Sets the joint names.
USD_INTEROP_API UsdResult UsdSkelSkeleton_SetJointNames(
    UsdSkelSkeletonRef skeleton,
    const TfTokenRef* names,
    size_t count
);

// MARK: - Skeleton Transforms

/// Gets the bind transforms (world space).
/// outTransforms receives count * 16 doubles (row-major 4x4 matrices).
/// Returns the number of transforms written.
USD_INTEROP_API size_t UsdSkelSkeleton_GetBindTransforms(
    UsdSkelSkeletonRef skeleton,
    double* outTransforms,
    size_t maxCount
);

/// Sets the bind transforms (world space).
/// transforms is count * 16 doubles (row-major 4x4 matrices).
USD_INTEROP_API UsdResult UsdSkelSkeleton_SetBindTransforms(
    UsdSkelSkeletonRef skeleton,
    const double* transforms,
    size_t count
);

/// Gets the rest transforms (local space).
/// outTransforms receives count * 16 doubles (row-major 4x4 matrices).
/// Returns the number of transforms written.
USD_INTEROP_API size_t UsdSkelSkeleton_GetRestTransforms(
    UsdSkelSkeletonRef skeleton,
    double* outTransforms,
    size_t maxCount
);

/// Sets the rest transforms (local space).
/// transforms is count * 16 doubles (row-major 4x4 matrices).
USD_INTEROP_API UsdResult UsdSkelSkeleton_SetRestTransforms(
    UsdSkelSkeletonRef skeleton,
    const double* transforms,
    size_t count
);

// ============================================================================
// MARK: - UsdSkelAnimation (Skeleton Animation)
// ============================================================================

/// Gets a UsdSkelAnimation from a stage and path.
/// Caller must release with UsdSkelAnimation_Release.
USD_INTEROP_API UsdSkelAnimationRef UsdSkelAnimation_Get(UsdStageRef stage, SdfPathRef path);

/// Defines a new SkelAnimation prim at the given path.
/// Caller must release with UsdSkelAnimation_Release.
USD_INTEROP_API UsdSkelAnimationRef UsdSkelAnimation_Define(UsdStageRef stage, SdfPathRef path);

/// Creates a SkelAnimation from a prim.
/// Caller must release with UsdSkelAnimation_Release.
USD_INTEROP_API UsdSkelAnimationRef UsdSkelAnimation_FromPrim(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdSkelAnimationRef UsdSkelAnimation_Retain(UsdSkelAnimationRef anim);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdSkelAnimation_Release(UsdSkelAnimationRef anim);

/// Returns true if the Animation is valid.
USD_INTEROP_API bool UsdSkelAnimation_IsValid(UsdSkelAnimationRef anim);

/// Gets the underlying prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdSkelAnimation_GetPrim(UsdSkelAnimationRef anim);

// MARK: - Animation Joints

/// Gets the number of joints in the animation.
USD_INTEROP_API size_t UsdSkelAnimation_GetJointCount(UsdSkelAnimationRef anim);

/// Gets the joint tokens.
/// outJoints must have space for maxCount tokens.
/// Caller must release each token with TfToken_Release.
USD_INTEROP_API size_t UsdSkelAnimation_GetJoints(
    UsdSkelAnimationRef anim,
    TfTokenRef* outJoints,
    size_t maxCount
);

/// Sets the joint tokens.
USD_INTEROP_API UsdResult UsdSkelAnimation_SetJoints(
    UsdSkelAnimationRef anim,
    const TfTokenRef* joints,
    size_t count
);

// MARK: - Animation Transforms

/// Gets the translations at a given time.
/// outTranslations receives count * 3 floats (x, y, z interleaved).
/// Returns the number of translations written.
USD_INTEROP_API size_t UsdSkelAnimation_GetTranslations(
    UsdSkelAnimationRef anim,
    UsdTimeCode time,
    float* outTranslations,
    size_t maxCount
);

/// Sets the translations at a given time.
/// translations is count * 3 floats (x, y, z interleaved).
USD_INTEROP_API UsdResult UsdSkelAnimation_SetTranslations(
    UsdSkelAnimationRef anim,
    UsdTimeCode time,
    const float* translations,
    size_t count
);

/// Gets the rotations at a given time.
/// outRotations receives count * 4 floats (w, x, y, z quaternion interleaved).
/// Returns the number of rotations written.
USD_INTEROP_API size_t UsdSkelAnimation_GetRotations(
    UsdSkelAnimationRef anim,
    UsdTimeCode time,
    float* outRotations,
    size_t maxCount
);

/// Sets the rotations at a given time.
/// rotations is count * 4 floats (w, x, y, z quaternion interleaved).
USD_INTEROP_API UsdResult UsdSkelAnimation_SetRotations(
    UsdSkelAnimationRef anim,
    UsdTimeCode time,
    const float* rotations,
    size_t count
);

/// Gets the scales at a given time.
/// outScales receives count * 3 half-floats (x, y, z interleaved) as floats.
/// Returns the number of scales written.
USD_INTEROP_API size_t UsdSkelAnimation_GetScales(
    UsdSkelAnimationRef anim,
    UsdTimeCode time,
    float* outScales,
    size_t maxCount
);

/// Sets the scales at a given time.
/// scales is count * 3 floats (x, y, z interleaved), will be stored as half-floats.
USD_INTEROP_API UsdResult UsdSkelAnimation_SetScales(
    UsdSkelAnimationRef anim,
    UsdTimeCode time,
    const float* scales,
    size_t count
);

// MARK: - Animation BlendShapes

/// Gets the number of blend shapes in the animation.
USD_INTEROP_API size_t UsdSkelAnimation_GetBlendShapeCount(UsdSkelAnimationRef anim);

/// Gets the blend shape tokens.
/// outBlendShapes must have space for maxCount tokens.
/// Caller must release each token with TfToken_Release.
USD_INTEROP_API size_t UsdSkelAnimation_GetBlendShapes(
    UsdSkelAnimationRef anim,
    TfTokenRef* outBlendShapes,
    size_t maxCount
);

/// Sets the blend shape tokens.
USD_INTEROP_API UsdResult UsdSkelAnimation_SetBlendShapes(
    UsdSkelAnimationRef anim,
    const TfTokenRef* blendShapes,
    size_t count
);

/// Gets the blend shape weights at a given time.
/// outWeights must have space for maxCount floats.
/// Returns the number of weights written.
USD_INTEROP_API size_t UsdSkelAnimation_GetBlendShapeWeights(
    UsdSkelAnimationRef anim,
    UsdTimeCode time,
    float* outWeights,
    size_t maxCount
);

/// Sets the blend shape weights at a given time.
USD_INTEROP_API UsdResult UsdSkelAnimation_SetBlendShapeWeights(
    UsdSkelAnimationRef anim,
    UsdTimeCode time,
    const float* weights,
    size_t count
);

// ============================================================================
// MARK: - UsdSkelBlendShape (Blend Shape Target)
// ============================================================================

/// Gets a UsdSkelBlendShape from a stage and path.
/// Caller must release with UsdSkelBlendShape_Release.
USD_INTEROP_API UsdSkelBlendShapeRef UsdSkelBlendShape_Get(UsdStageRef stage, SdfPathRef path);

/// Defines a new BlendShape prim at the given path.
/// Caller must release with UsdSkelBlendShape_Release.
USD_INTEROP_API UsdSkelBlendShapeRef UsdSkelBlendShape_Define(UsdStageRef stage, SdfPathRef path);

/// Creates a BlendShape from a prim.
/// Caller must release with UsdSkelBlendShape_Release.
USD_INTEROP_API UsdSkelBlendShapeRef UsdSkelBlendShape_FromPrim(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdSkelBlendShapeRef UsdSkelBlendShape_Retain(UsdSkelBlendShapeRef blendShape);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdSkelBlendShape_Release(UsdSkelBlendShapeRef blendShape);

/// Returns true if the BlendShape is valid.
USD_INTEROP_API bool UsdSkelBlendShape_IsValid(UsdSkelBlendShapeRef blendShape);

/// Gets the underlying prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdSkelBlendShape_GetPrim(UsdSkelBlendShapeRef blendShape);

// MARK: - BlendShape Offsets

/// Gets the number of offsets.
USD_INTEROP_API size_t UsdSkelBlendShape_GetOffsetsCount(UsdSkelBlendShapeRef blendShape);

/// Gets the position offsets.
/// outOffsets receives count * 3 floats (x, y, z interleaved).
/// Returns the number of offsets written.
USD_INTEROP_API size_t UsdSkelBlendShape_GetOffsets(
    UsdSkelBlendShapeRef blendShape,
    float* outOffsets,
    size_t maxCount
);

/// Sets the position offsets.
/// offsets is count * 3 floats (x, y, z interleaved).
USD_INTEROP_API UsdResult UsdSkelBlendShape_SetOffsets(
    UsdSkelBlendShapeRef blendShape,
    const float* offsets,
    size_t count
);

/// Gets the normal offsets.
/// outNormalOffsets receives count * 3 floats (x, y, z interleaved).
/// Returns the number of offsets written.
USD_INTEROP_API size_t UsdSkelBlendShape_GetNormalOffsets(
    UsdSkelBlendShapeRef blendShape,
    float* outNormalOffsets,
    size_t maxCount
);

/// Sets the normal offsets.
/// normalOffsets is count * 3 floats (x, y, z interleaved).
USD_INTEROP_API UsdResult UsdSkelBlendShape_SetNormalOffsets(
    UsdSkelBlendShapeRef blendShape,
    const float* normalOffsets,
    size_t count
);

// MARK: - BlendShape Point Indices

/// Gets the number of point indices.
USD_INTEROP_API size_t UsdSkelBlendShape_GetPointIndicesCount(UsdSkelBlendShapeRef blendShape);

/// Gets the point indices.
/// outIndices must have space for maxCount ints.
/// Returns the number of indices written.
USD_INTEROP_API size_t UsdSkelBlendShape_GetPointIndices(
    UsdSkelBlendShapeRef blendShape,
    int* outIndices,
    size_t maxCount
);

/// Sets the point indices.
USD_INTEROP_API UsdResult UsdSkelBlendShape_SetPointIndices(
    UsdSkelBlendShapeRef blendShape,
    const int* indices,
    size_t count
);

// ============================================================================
// MARK: - UsdSkelBindingAPI (Skeletal Binding API Schema)
// ============================================================================

/// Gets a UsdSkelBindingAPI from a prim.
/// Caller must release with UsdSkelBindingAPI_Release.
USD_INTEROP_API UsdSkelBindingAPIRef UsdSkelBindingAPI_Get(UsdPrimRef prim);

/// Applies the BindingAPI to a prim.
/// Caller must release with UsdSkelBindingAPI_Release.
USD_INTEROP_API UsdSkelBindingAPIRef UsdSkelBindingAPI_Apply(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdSkelBindingAPIRef UsdSkelBindingAPI_Retain(UsdSkelBindingAPIRef api);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdSkelBindingAPI_Release(UsdSkelBindingAPIRef api);

/// Returns true if the BindingAPI is valid.
USD_INTEROP_API bool UsdSkelBindingAPI_IsValid(UsdSkelBindingAPIRef api);

/// Gets the underlying prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdSkelBindingAPI_GetPrim(UsdSkelBindingAPIRef api);

// MARK: - BindingAPI Skeleton Relationship

/// Gets the skeleton relationship target.
/// Caller must release with SdfPath_Release.
USD_INTEROP_API SdfPathRef UsdSkelBindingAPI_GetSkeletonPath(UsdSkelBindingAPIRef api);

/// Creates and sets the skeleton relationship target.
USD_INTEROP_API UsdResult UsdSkelBindingAPI_SetSkeletonPath(
    UsdSkelBindingAPIRef api,
    SdfPathRef path
);

// MARK: - BindingAPI Animation Source Relationship

/// Gets the animation source relationship target.
/// Caller must release with SdfPath_Release.
USD_INTEROP_API SdfPathRef UsdSkelBindingAPI_GetAnimationSourcePath(UsdSkelBindingAPIRef api);

/// Creates and sets the animation source relationship target.
USD_INTEROP_API UsdResult UsdSkelBindingAPI_SetAnimationSourcePath(
    UsdSkelBindingAPIRef api,
    SdfPathRef path
);

// MARK: - BindingAPI Joint Influences

/// Gets the number of joint indices.
USD_INTEROP_API size_t UsdSkelBindingAPI_GetJointIndicesCount(UsdSkelBindingAPIRef api);

/// Gets the joint indices.
/// outIndices must have space for maxCount ints.
/// Returns the number of indices written.
USD_INTEROP_API size_t UsdSkelBindingAPI_GetJointIndices(
    UsdSkelBindingAPIRef api,
    int* outIndices,
    size_t maxCount
);

/// Sets the joint indices.
/// If constant is true, uses constant interpolation (rigid binding).
/// elementSize specifies how many influences per point (-1 for auto).
USD_INTEROP_API UsdResult UsdSkelBindingAPI_SetJointIndices(
    UsdSkelBindingAPIRef api,
    const int* indices,
    size_t count,
    bool constant,
    int elementSize
);

/// Gets the number of joint weights.
USD_INTEROP_API size_t UsdSkelBindingAPI_GetJointWeightsCount(UsdSkelBindingAPIRef api);

/// Gets the joint weights.
/// outWeights must have space for maxCount floats.
/// Returns the number of weights written.
USD_INTEROP_API size_t UsdSkelBindingAPI_GetJointWeights(
    UsdSkelBindingAPIRef api,
    float* outWeights,
    size_t maxCount
);

/// Sets the joint weights.
/// If constant is true, uses constant interpolation (rigid binding).
/// elementSize specifies how many influences per point (-1 for auto).
USD_INTEROP_API UsdResult UsdSkelBindingAPI_SetJointWeights(
    UsdSkelBindingAPIRef api,
    const float* weights,
    size_t count,
    bool constant,
    int elementSize
);

/// Convenience method for setting rigid joint influence.
/// Makes the primitive rigidly deformed by a single joint.
USD_INTEROP_API UsdResult UsdSkelBindingAPI_SetRigidJointInfluence(
    UsdSkelBindingAPIRef api,
    int jointIndex,
    float weight
);

// MARK: - BindingAPI Geometry Bind Transform

/// Gets the geometry bind transform.
/// outMatrix receives 16 doubles in row-major order.
USD_INTEROP_API UsdResult UsdSkelBindingAPI_GetGeomBindTransform(
    UsdSkelBindingAPIRef api,
    double* outMatrix
);

/// Sets the geometry bind transform.
/// matrix is 16 doubles in row-major order.
USD_INTEROP_API UsdResult UsdSkelBindingAPI_SetGeomBindTransform(
    UsdSkelBindingAPIRef api,
    const double* matrix
);

// MARK: - BindingAPI Skinning Method

/// Gets the skinning method.
USD_INTEROP_API UsdSkelSkinningMethod UsdSkelBindingAPI_GetSkinningMethod(UsdSkelBindingAPIRef api);

/// Sets the skinning method.
USD_INTEROP_API UsdResult UsdSkelBindingAPI_SetSkinningMethod(
    UsdSkelBindingAPIRef api,
    UsdSkelSkinningMethod method
);

#ifdef __cplusplus
}
#endif

#endif // OPENUSDINTEROP_USDSKEL_USDSKEL_H
