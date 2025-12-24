// usdGeom.h - UsdGeom module C wrapper
// Mirrors: pxr/usd/usdGeom/*.h

#ifndef OPENUSDINTEROP_USDGEOM_USDGEOM_H
#define OPENUSDINTEROP_USDGEOM_USDGEOM_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Cross-module dependencies
#include "../base/tf.h"
#include "../base/gf.h"
#include "../base/vt.h"
#include "../usd/sdf.h"
#include "../usd/usd.h"

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

typedef struct UsdGeomXformOpaque* UsdGeomXformRef;
typedef struct UsdGeomMeshOpaque* UsdGeomMeshRef;
typedef struct UsdGeomPointsOpaque* UsdGeomPointsRef;
typedef struct UsdGeomBasisCurvesOpaque* UsdGeomBasisCurvesRef;
typedef struct UsdGeomCameraOpaque* UsdGeomCameraRef;
typedef struct UsdGeomSphereOpaque* UsdGeomSphereRef;
typedef struct UsdGeomCubeOpaque* UsdGeomCubeRef;
typedef struct UsdGeomCylinderOpaque* UsdGeomCylinderRef;
typedef struct UsdGeomConeOpaque* UsdGeomConeRef;
typedef struct UsdGeomCapsuleOpaque* UsdGeomCapsuleRef;
typedef struct UsdGeomPrimvarOpaque* UsdGeomPrimvarRef;
typedef struct UsdGeomPrimvarsAPIOpaque* UsdGeomPrimvarsAPIRef;
typedef struct UsdGeomXformCacheOpaque* UsdGeomXformCacheRef;
typedef struct UsdGeomBBoxCacheOpaque* UsdGeomBBoxCacheRef;

// ============================================================================
// MARK: - Enums
// ============================================================================

/// Primvar interpolation modes
typedef enum {
    USDGEOM_INTERPOLATION_CONSTANT = 0,     ///< One value for the entire surface
    USDGEOM_INTERPOLATION_UNIFORM = 1,      ///< One value per face
    USDGEOM_INTERPOLATION_VARYING = 2,      ///< One value per vertex (bilinear)
    USDGEOM_INTERPOLATION_VERTEX = 3,       ///< One value per vertex (smooth)
    USDGEOM_INTERPOLATION_FACEVARYING = 4   ///< One value per face-vertex
} UsdGeomInterpolation;

/// Subdivision scheme for meshes
typedef enum {
    USDGEOM_SUBDIV_NONE = 0,           ///< No subdivision
    USDGEOM_SUBDIV_CATMULL_CLARK = 1,  ///< Catmull-Clark subdivision
    USDGEOM_SUBDIV_LOOP = 2,           ///< Loop subdivision
    USDGEOM_SUBDIV_BILINEAR = 3        ///< Bilinear subdivision
} UsdGeomSubdivisionScheme;

/// Curve type for basis curves
typedef enum {
    USDGEOM_CURVE_TYPE_LINEAR = 0,
    USDGEOM_CURVE_TYPE_CUBIC = 1
} UsdGeomCurveType;

/// Basis type for cubic curves
typedef enum {
    USDGEOM_BASIS_BEZIER = 0,
    USDGEOM_BASIS_BSPLINE = 1,
    USDGEOM_BASIS_CATMULL_ROM = 2
} UsdGeomBasis;

/// Curve wrap mode
typedef enum {
    USDGEOM_WRAP_NONPERIODIC = 0,
    USDGEOM_WRAP_PERIODIC = 1,
    USDGEOM_WRAP_PINNED = 2
} UsdGeomWrap;

/// Axis for geometric primitives
typedef enum {
    USDGEOM_AXIS_X = 0,
    USDGEOM_AXIS_Y = 1,
    USDGEOM_AXIS_Z = 2
} UsdGeomAxis;

/// Purpose values for visibility/rendering
typedef enum {
    USDGEOM_PURPOSE_DEFAULT = 0,
    USDGEOM_PURPOSE_RENDER = 1,
    USDGEOM_PURPOSE_PROXY = 2,
    USDGEOM_PURPOSE_GUIDE = 3
} UsdGeomPurpose;

// ============================================================================
// MARK: - UsdGeomXform (Transform Node)
// ============================================================================

/// Creates a UsdGeomXform schema on the given prim.
/// Caller must release with UsdGeomXform_Release.
USD_INTEROP_API UsdGeomXformRef UsdGeomXform_Get(UsdStageRef stage, SdfPathRef path);

/// Defines a new Xform prim at the given path.
/// Caller must release with UsdGeomXform_Release.
USD_INTEROP_API UsdGeomXformRef UsdGeomXform_Define(UsdStageRef stage, SdfPathRef path);

/// Creates from a prim.
/// Caller must release with UsdGeomXform_Release.
USD_INTEROP_API UsdGeomXformRef UsdGeomXform_FromPrim(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdGeomXformRef UsdGeomXform_Retain(UsdGeomXformRef xform);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdGeomXform_Release(UsdGeomXformRef xform);

/// Returns true if the xform is valid.
USD_INTEROP_API bool UsdGeomXform_IsValid(UsdGeomXformRef xform);

/// Gets the underlying prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdGeomXform_GetPrim(UsdGeomXformRef xform);

/// Computes the local-to-world transform matrix.
/// Returns 16 doubles in row-major order.
USD_INTEROP_API UsdResult UsdGeomXform_ComputeLocalToWorldTransform(
    UsdGeomXformRef xform,
    UsdTimeCode time,
    double* outMatrix
);

/// Computes the parent-to-world transform matrix.
/// Returns 16 doubles in row-major order.
USD_INTEROP_API UsdResult UsdGeomXform_ComputeParentToWorldTransform(
    UsdGeomXformRef xform,
    UsdTimeCode time,
    double* outMatrix
);

/// Sets the local transform using a 4x4 matrix.
/// Matrix is 16 doubles in row-major order.
USD_INTEROP_API UsdResult UsdGeomXform_SetTransformMatrix(
    UsdGeomXformRef xform,
    UsdTimeCode time,
    const double* matrix
);

/// Adds a translate operation.
USD_INTEROP_API UsdResult UsdGeomXform_AddTranslateOp(
    UsdGeomXformRef xform,
    double x, double y, double z,
    UsdTimeCode time
);

/// Adds a rotate XYZ operation (in degrees).
USD_INTEROP_API UsdResult UsdGeomXform_AddRotateXYZOp(
    UsdGeomXformRef xform,
    double rotX, double rotY, double rotZ,
    UsdTimeCode time
);

/// Adds a scale operation.
USD_INTEROP_API UsdResult UsdGeomXform_AddScaleOp(
    UsdGeomXformRef xform,
    double scaleX, double scaleY, double scaleZ,
    UsdTimeCode time
);

/// Clears all xform operations.
USD_INTEROP_API UsdResult UsdGeomXform_ClearXformOpOrder(UsdGeomXformRef xform);

/// Returns true if the xform op order resets the transform stack.
USD_INTEROP_API bool UsdGeomXform_GetResetXformStack(UsdGeomXformRef xform);

/// Sets whether the xform op order should reset the transform stack.
USD_INTEROP_API UsdResult UsdGeomXform_SetResetXformStack(UsdGeomXformRef xform, bool reset);

// ============================================================================
// MARK: - UsdGeomMesh
// ============================================================================

/// Creates a UsdGeomMesh schema on the given prim.
/// Caller must release with UsdGeomMesh_Release.
USD_INTEROP_API UsdGeomMeshRef UsdGeomMesh_Get(UsdStageRef stage, SdfPathRef path);

/// Defines a new Mesh prim at the given path.
/// Caller must release with UsdGeomMesh_Release.
USD_INTEROP_API UsdGeomMeshRef UsdGeomMesh_Define(UsdStageRef stage, SdfPathRef path);

/// Creates from a prim.
/// Caller must release with UsdGeomMesh_Release.
USD_INTEROP_API UsdGeomMeshRef UsdGeomMesh_FromPrim(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdGeomMeshRef UsdGeomMesh_Retain(UsdGeomMeshRef mesh);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdGeomMesh_Release(UsdGeomMeshRef mesh);

/// Returns true if the mesh is valid.
USD_INTEROP_API bool UsdGeomMesh_IsValid(UsdGeomMeshRef mesh);

/// Gets the underlying prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdGeomMesh_GetPrim(UsdGeomMeshRef mesh);

// MARK: - Mesh Points

/// Gets the number of points.
USD_INTEROP_API size_t UsdGeomMesh_GetPointCount(UsdGeomMeshRef mesh, UsdTimeCode time);

/// Gets the points array.
/// outPoints must have space for count * 3 floats (x, y, z interleaved).
/// Returns actual count written.
USD_INTEROP_API size_t UsdGeomMesh_GetPoints(
    UsdGeomMeshRef mesh,
    UsdTimeCode time,
    float* outPoints,
    size_t maxCount
);

/// Sets the points array.
/// points is count * 3 floats (x, y, z interleaved).
USD_INTEROP_API UsdResult UsdGeomMesh_SetPoints(
    UsdGeomMeshRef mesh,
    UsdTimeCode time,
    const float* points,
    size_t count
);

// MARK: - Mesh Topology

/// Gets the number of face vertex counts.
USD_INTEROP_API size_t UsdGeomMesh_GetFaceVertexCountsCount(UsdGeomMeshRef mesh, UsdTimeCode time);

/// Gets the face vertex counts array.
/// Returns actual count written.
USD_INTEROP_API size_t UsdGeomMesh_GetFaceVertexCounts(
    UsdGeomMeshRef mesh,
    UsdTimeCode time,
    int* outCounts,
    size_t maxCount
);

/// Sets the face vertex counts array.
USD_INTEROP_API UsdResult UsdGeomMesh_SetFaceVertexCounts(
    UsdGeomMeshRef mesh,
    UsdTimeCode time,
    const int* counts,
    size_t count
);

/// Gets the number of face vertex indices.
USD_INTEROP_API size_t UsdGeomMesh_GetFaceVertexIndicesCount(UsdGeomMeshRef mesh, UsdTimeCode time);

/// Gets the face vertex indices array.
/// Returns actual count written.
USD_INTEROP_API size_t UsdGeomMesh_GetFaceVertexIndices(
    UsdGeomMeshRef mesh,
    UsdTimeCode time,
    int* outIndices,
    size_t maxCount
);

/// Sets the face vertex indices array.
USD_INTEROP_API UsdResult UsdGeomMesh_SetFaceVertexIndices(
    UsdGeomMeshRef mesh,
    UsdTimeCode time,
    const int* indices,
    size_t count
);

// MARK: - Mesh Normals

/// Gets the number of normals.
USD_INTEROP_API size_t UsdGeomMesh_GetNormalsCount(UsdGeomMeshRef mesh, UsdTimeCode time);

/// Gets the normals array.
/// outNormals must have space for count * 3 floats.
USD_INTEROP_API size_t UsdGeomMesh_GetNormals(
    UsdGeomMeshRef mesh,
    UsdTimeCode time,
    float* outNormals,
    size_t maxCount
);

/// Sets the normals array.
USD_INTEROP_API UsdResult UsdGeomMesh_SetNormals(
    UsdGeomMeshRef mesh,
    UsdTimeCode time,
    const float* normals,
    size_t count,
    UsdGeomInterpolation interpolation
);

/// Gets the normals interpolation.
USD_INTEROP_API UsdGeomInterpolation UsdGeomMesh_GetNormalsInterpolation(UsdGeomMeshRef mesh);

// MARK: - Mesh Subdivision

/// Gets the subdivision scheme.
USD_INTEROP_API UsdGeomSubdivisionScheme UsdGeomMesh_GetSubdivisionScheme(UsdGeomMeshRef mesh);

/// Sets the subdivision scheme.
USD_INTEROP_API UsdResult UsdGeomMesh_SetSubdivisionScheme(
    UsdGeomMeshRef mesh,
    UsdGeomSubdivisionScheme scheme
);

// MARK: - Mesh Extent

/// Computes the extent (bounding box) of the mesh.
/// outExtent receives 6 floats: minX, minY, minZ, maxX, maxY, maxZ.
USD_INTEROP_API UsdResult UsdGeomMesh_ComputeExtent(
    UsdGeomMeshRef mesh,
    UsdTimeCode time,
    float* outExtent
);

/// Gets the authored extent attribute.
/// outExtent receives 6 floats: minX, minY, minZ, maxX, maxY, maxZ.
USD_INTEROP_API UsdResult UsdGeomMesh_GetExtent(
    UsdGeomMeshRef mesh,
    UsdTimeCode time,
    float* outExtent
);

/// Sets the extent attribute.
USD_INTEROP_API UsdResult UsdGeomMesh_SetExtent(
    UsdGeomMeshRef mesh,
    UsdTimeCode time,
    const float* extent
);

// MARK: - Mesh Display Properties

/// Returns true if the mesh is double-sided.
USD_INTEROP_API bool UsdGeomMesh_GetDoubleSided(UsdGeomMeshRef mesh);

/// Sets whether the mesh is double-sided.
USD_INTEROP_API UsdResult UsdGeomMesh_SetDoubleSided(UsdGeomMeshRef mesh, bool doubleSided);

// ============================================================================
// MARK: - UsdGeomPoints
// ============================================================================

/// Defines a new Points prim at the given path.
/// Caller must release with UsdGeomPoints_Release.
USD_INTEROP_API UsdGeomPointsRef UsdGeomPoints_Define(UsdStageRef stage, SdfPathRef path);

/// Creates from a prim.
/// Caller must release with UsdGeomPoints_Release.
USD_INTEROP_API UsdGeomPointsRef UsdGeomPoints_FromPrim(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdGeomPointsRef UsdGeomPoints_Retain(UsdGeomPointsRef points);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdGeomPoints_Release(UsdGeomPointsRef points);

/// Returns true if the points prim is valid.
USD_INTEROP_API bool UsdGeomPoints_IsValid(UsdGeomPointsRef points);

/// Gets the underlying prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdGeomPoints_GetPrim(UsdGeomPointsRef points);

/// Gets the number of points.
USD_INTEROP_API size_t UsdGeomPoints_GetPointCount(UsdGeomPointsRef points, UsdTimeCode time);

/// Gets the points array.
/// outPoints must have space for count * 3 floats.
USD_INTEROP_API size_t UsdGeomPoints_GetPoints(
    UsdGeomPointsRef points,
    UsdTimeCode time,
    float* outPoints,
    size_t maxCount
);

/// Sets the points array.
USD_INTEROP_API UsdResult UsdGeomPoints_SetPoints(
    UsdGeomPointsRef points,
    UsdTimeCode time,
    const float* pointsData,
    size_t count
);

/// Gets the number of widths.
USD_INTEROP_API size_t UsdGeomPoints_GetWidthsCount(UsdGeomPointsRef points, UsdTimeCode time);

/// Gets the widths array.
USD_INTEROP_API size_t UsdGeomPoints_GetWidths(
    UsdGeomPointsRef points,
    UsdTimeCode time,
    float* outWidths,
    size_t maxCount
);

/// Sets the widths array.
USD_INTEROP_API UsdResult UsdGeomPoints_SetWidths(
    UsdGeomPointsRef points,
    UsdTimeCode time,
    const float* widths,
    size_t count
);

/// Gets the number of IDs.
USD_INTEROP_API size_t UsdGeomPoints_GetIdsCount(UsdGeomPointsRef points, UsdTimeCode time);

/// Gets the IDs array.
USD_INTEROP_API size_t UsdGeomPoints_GetIds(
    UsdGeomPointsRef points,
    UsdTimeCode time,
    int64_t* outIds,
    size_t maxCount
);

/// Sets the IDs array.
USD_INTEROP_API UsdResult UsdGeomPoints_SetIds(
    UsdGeomPointsRef points,
    UsdTimeCode time,
    const int64_t* ids,
    size_t count
);

// ============================================================================
// MARK: - UsdGeomBasisCurves
// ============================================================================

/// Defines a new BasisCurves prim at the given path.
/// Caller must release with UsdGeomBasisCurves_Release.
USD_INTEROP_API UsdGeomBasisCurvesRef UsdGeomBasisCurves_Define(UsdStageRef stage, SdfPathRef path);

/// Creates from a prim.
/// Caller must release with UsdGeomBasisCurves_Release.
USD_INTEROP_API UsdGeomBasisCurvesRef UsdGeomBasisCurves_FromPrim(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdGeomBasisCurvesRef UsdGeomBasisCurves_Retain(UsdGeomBasisCurvesRef curves);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdGeomBasisCurves_Release(UsdGeomBasisCurvesRef curves);

/// Returns true if the curves prim is valid.
USD_INTEROP_API bool UsdGeomBasisCurves_IsValid(UsdGeomBasisCurvesRef curves);

/// Gets the underlying prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdGeomBasisCurves_GetPrim(UsdGeomBasisCurvesRef curves);

/// Gets the curve type.
USD_INTEROP_API UsdGeomCurveType UsdGeomBasisCurves_GetType(UsdGeomBasisCurvesRef curves);

/// Sets the curve type.
USD_INTEROP_API UsdResult UsdGeomBasisCurves_SetType(
    UsdGeomBasisCurvesRef curves,
    UsdGeomCurveType type
);

/// Gets the basis.
USD_INTEROP_API UsdGeomBasis UsdGeomBasisCurves_GetBasis(UsdGeomBasisCurvesRef curves);

/// Sets the basis.
USD_INTEROP_API UsdResult UsdGeomBasisCurves_SetBasis(
    UsdGeomBasisCurvesRef curves,
    UsdGeomBasis basis
);

/// Gets the wrap mode.
USD_INTEROP_API UsdGeomWrap UsdGeomBasisCurves_GetWrap(UsdGeomBasisCurvesRef curves);

/// Sets the wrap mode.
USD_INTEROP_API UsdResult UsdGeomBasisCurves_SetWrap(
    UsdGeomBasisCurvesRef curves,
    UsdGeomWrap wrap
);

/// Gets the number of curve vertex counts.
USD_INTEROP_API size_t UsdGeomBasisCurves_GetCurveVertexCountsCount(
    UsdGeomBasisCurvesRef curves,
    UsdTimeCode time
);

/// Gets the curve vertex counts.
USD_INTEROP_API size_t UsdGeomBasisCurves_GetCurveVertexCounts(
    UsdGeomBasisCurvesRef curves,
    UsdTimeCode time,
    int* outCounts,
    size_t maxCount
);

/// Sets the curve vertex counts.
USD_INTEROP_API UsdResult UsdGeomBasisCurves_SetCurveVertexCounts(
    UsdGeomBasisCurvesRef curves,
    UsdTimeCode time,
    const int* counts,
    size_t count
);

/// Gets the number of points.
USD_INTEROP_API size_t UsdGeomBasisCurves_GetPointCount(UsdGeomBasisCurvesRef curves, UsdTimeCode time);

/// Gets the points.
USD_INTEROP_API size_t UsdGeomBasisCurves_GetPoints(
    UsdGeomBasisCurvesRef curves,
    UsdTimeCode time,
    float* outPoints,
    size_t maxCount
);

/// Sets the points.
USD_INTEROP_API UsdResult UsdGeomBasisCurves_SetPoints(
    UsdGeomBasisCurvesRef curves,
    UsdTimeCode time,
    const float* points,
    size_t count
);

/// Gets the number of widths.
USD_INTEROP_API size_t UsdGeomBasisCurves_GetWidthsCount(UsdGeomBasisCurvesRef curves, UsdTimeCode time);

/// Gets the widths.
USD_INTEROP_API size_t UsdGeomBasisCurves_GetWidths(
    UsdGeomBasisCurvesRef curves,
    UsdTimeCode time,
    float* outWidths,
    size_t maxCount
);

/// Sets the widths.
USD_INTEROP_API UsdResult UsdGeomBasisCurves_SetWidths(
    UsdGeomBasisCurvesRef curves,
    UsdTimeCode time,
    const float* widths,
    size_t count
);

// ============================================================================
// MARK: - UsdGeomCamera
// ============================================================================

/// Defines a new Camera prim at the given path.
/// Caller must release with UsdGeomCamera_Release.
USD_INTEROP_API UsdGeomCameraRef UsdGeomCamera_Define(UsdStageRef stage, SdfPathRef path);

/// Creates from a prim.
/// Caller must release with UsdGeomCamera_Release.
USD_INTEROP_API UsdGeomCameraRef UsdGeomCamera_FromPrim(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdGeomCameraRef UsdGeomCamera_Retain(UsdGeomCameraRef camera);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdGeomCamera_Release(UsdGeomCameraRef camera);

/// Returns true if the camera is valid.
USD_INTEROP_API bool UsdGeomCamera_IsValid(UsdGeomCameraRef camera);

/// Gets the underlying prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdGeomCamera_GetPrim(UsdGeomCameraRef camera);

/// Gets the focal length in mm.
USD_INTEROP_API float UsdGeomCamera_GetFocalLength(UsdGeomCameraRef camera, UsdTimeCode time);

/// Sets the focal length in mm.
USD_INTEROP_API UsdResult UsdGeomCamera_SetFocalLength(
    UsdGeomCameraRef camera,
    UsdTimeCode time,
    float focalLength
);

/// Gets the horizontal aperture in mm.
USD_INTEROP_API float UsdGeomCamera_GetHorizontalAperture(UsdGeomCameraRef camera, UsdTimeCode time);

/// Sets the horizontal aperture in mm.
USD_INTEROP_API UsdResult UsdGeomCamera_SetHorizontalAperture(
    UsdGeomCameraRef camera,
    UsdTimeCode time,
    float aperture
);

/// Gets the vertical aperture in mm.
USD_INTEROP_API float UsdGeomCamera_GetVerticalAperture(UsdGeomCameraRef camera, UsdTimeCode time);

/// Sets the vertical aperture in mm.
USD_INTEROP_API UsdResult UsdGeomCamera_SetVerticalAperture(
    UsdGeomCameraRef camera,
    UsdTimeCode time,
    float aperture
);

/// Gets the clipping range.
USD_INTEROP_API UsdResult UsdGeomCamera_GetClippingRange(
    UsdGeomCameraRef camera,
    UsdTimeCode time,
    float* nearClip,
    float* farClip
);

/// Sets the clipping range.
USD_INTEROP_API UsdResult UsdGeomCamera_SetClippingRange(
    UsdGeomCameraRef camera,
    UsdTimeCode time,
    float nearClip,
    float farClip
);

/// Gets the focus distance.
USD_INTEROP_API float UsdGeomCamera_GetFocusDistance(UsdGeomCameraRef camera, UsdTimeCode time);

/// Sets the focus distance.
USD_INTEROP_API UsdResult UsdGeomCamera_SetFocusDistance(
    UsdGeomCameraRef camera,
    UsdTimeCode time,
    float focusDistance
);

/// Gets the f-stop.
USD_INTEROP_API float UsdGeomCamera_GetFStop(UsdGeomCameraRef camera, UsdTimeCode time);

/// Sets the f-stop.
USD_INTEROP_API UsdResult UsdGeomCamera_SetFStop(
    UsdGeomCameraRef camera,
    UsdTimeCode time,
    float fStop
);

// ============================================================================
// MARK: - UsdGeomSphere
// ============================================================================

/// Defines a new Sphere prim at the given path.
/// Caller must release with UsdGeomSphere_Release.
USD_INTEROP_API UsdGeomSphereRef UsdGeomSphere_Define(UsdStageRef stage, SdfPathRef path);

/// Creates from a prim.
/// Caller must release with UsdGeomSphere_Release.
USD_INTEROP_API UsdGeomSphereRef UsdGeomSphere_FromPrim(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdGeomSphereRef UsdGeomSphere_Retain(UsdGeomSphereRef sphere);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdGeomSphere_Release(UsdGeomSphereRef sphere);

/// Returns true if the sphere is valid.
USD_INTEROP_API bool UsdGeomSphere_IsValid(UsdGeomSphereRef sphere);

/// Gets the underlying prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdGeomSphere_GetPrim(UsdGeomSphereRef sphere);

/// Gets the radius.
USD_INTEROP_API double UsdGeomSphere_GetRadius(UsdGeomSphereRef sphere, UsdTimeCode time);

/// Sets the radius.
USD_INTEROP_API UsdResult UsdGeomSphere_SetRadius(
    UsdGeomSphereRef sphere,
    UsdTimeCode time,
    double radius
);

/// Computes the extent.
USD_INTEROP_API UsdResult UsdGeomSphere_ComputeExtent(
    double radius,
    float* outExtent
);

// ============================================================================
// MARK: - UsdGeomCube
// ============================================================================

/// Defines a new Cube prim at the given path.
/// Caller must release with UsdGeomCube_Release.
USD_INTEROP_API UsdGeomCubeRef UsdGeomCube_Define(UsdStageRef stage, SdfPathRef path);

/// Creates from a prim.
/// Caller must release with UsdGeomCube_Release.
USD_INTEROP_API UsdGeomCubeRef UsdGeomCube_FromPrim(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdGeomCubeRef UsdGeomCube_Retain(UsdGeomCubeRef cube);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdGeomCube_Release(UsdGeomCubeRef cube);

/// Returns true if the cube is valid.
USD_INTEROP_API bool UsdGeomCube_IsValid(UsdGeomCubeRef cube);

/// Gets the underlying prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdGeomCube_GetPrim(UsdGeomCubeRef cube);

/// Gets the size.
USD_INTEROP_API double UsdGeomCube_GetSize(UsdGeomCubeRef cube, UsdTimeCode time);

/// Sets the size.
USD_INTEROP_API UsdResult UsdGeomCube_SetSize(
    UsdGeomCubeRef cube,
    UsdTimeCode time,
    double size
);

/// Computes the extent.
USD_INTEROP_API UsdResult UsdGeomCube_ComputeExtent(
    double size,
    float* outExtent
);

// ============================================================================
// MARK: - UsdGeomCylinder
// ============================================================================

/// Defines a new Cylinder prim at the given path.
/// Caller must release with UsdGeomCylinder_Release.
USD_INTEROP_API UsdGeomCylinderRef UsdGeomCylinder_Define(UsdStageRef stage, SdfPathRef path);

/// Creates from a prim.
/// Caller must release with UsdGeomCylinder_Release.
USD_INTEROP_API UsdGeomCylinderRef UsdGeomCylinder_FromPrim(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdGeomCylinderRef UsdGeomCylinder_Retain(UsdGeomCylinderRef cylinder);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdGeomCylinder_Release(UsdGeomCylinderRef cylinder);

/// Returns true if the cylinder is valid.
USD_INTEROP_API bool UsdGeomCylinder_IsValid(UsdGeomCylinderRef cylinder);

/// Gets the underlying prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdGeomCylinder_GetPrim(UsdGeomCylinderRef cylinder);

/// Gets the height.
USD_INTEROP_API double UsdGeomCylinder_GetHeight(UsdGeomCylinderRef cylinder, UsdTimeCode time);

/// Sets the height.
USD_INTEROP_API UsdResult UsdGeomCylinder_SetHeight(
    UsdGeomCylinderRef cylinder,
    UsdTimeCode time,
    double height
);

/// Gets the radius.
USD_INTEROP_API double UsdGeomCylinder_GetRadius(UsdGeomCylinderRef cylinder, UsdTimeCode time);

/// Sets the radius.
USD_INTEROP_API UsdResult UsdGeomCylinder_SetRadius(
    UsdGeomCylinderRef cylinder,
    UsdTimeCode time,
    double radius
);

/// Gets the axis.
USD_INTEROP_API UsdGeomAxis UsdGeomCylinder_GetAxis(UsdGeomCylinderRef cylinder);

/// Sets the axis.
USD_INTEROP_API UsdResult UsdGeomCylinder_SetAxis(UsdGeomCylinderRef cylinder, UsdGeomAxis axis);

// ============================================================================
// MARK: - UsdGeomCone
// ============================================================================

/// Defines a new Cone prim at the given path.
/// Caller must release with UsdGeomCone_Release.
USD_INTEROP_API UsdGeomConeRef UsdGeomCone_Define(UsdStageRef stage, SdfPathRef path);

/// Creates from a prim.
/// Caller must release with UsdGeomCone_Release.
USD_INTEROP_API UsdGeomConeRef UsdGeomCone_FromPrim(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdGeomConeRef UsdGeomCone_Retain(UsdGeomConeRef cone);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdGeomCone_Release(UsdGeomConeRef cone);

/// Returns true if the cone is valid.
USD_INTEROP_API bool UsdGeomCone_IsValid(UsdGeomConeRef cone);

/// Gets the underlying prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdGeomCone_GetPrim(UsdGeomConeRef cone);

/// Gets the height.
USD_INTEROP_API double UsdGeomCone_GetHeight(UsdGeomConeRef cone, UsdTimeCode time);

/// Sets the height.
USD_INTEROP_API UsdResult UsdGeomCone_SetHeight(
    UsdGeomConeRef cone,
    UsdTimeCode time,
    double height
);

/// Gets the radius.
USD_INTEROP_API double UsdGeomCone_GetRadius(UsdGeomConeRef cone, UsdTimeCode time);

/// Sets the radius.
USD_INTEROP_API UsdResult UsdGeomCone_SetRadius(
    UsdGeomConeRef cone,
    UsdTimeCode time,
    double radius
);

/// Gets the axis.
USD_INTEROP_API UsdGeomAxis UsdGeomCone_GetAxis(UsdGeomConeRef cone);

/// Sets the axis.
USD_INTEROP_API UsdResult UsdGeomCone_SetAxis(UsdGeomConeRef cone, UsdGeomAxis axis);

// ============================================================================
// MARK: - UsdGeomCapsule
// ============================================================================

/// Defines a new Capsule prim at the given path.
/// Caller must release with UsdGeomCapsule_Release.
USD_INTEROP_API UsdGeomCapsuleRef UsdGeomCapsule_Define(UsdStageRef stage, SdfPathRef path);

/// Creates from a prim.
/// Caller must release with UsdGeomCapsule_Release.
USD_INTEROP_API UsdGeomCapsuleRef UsdGeomCapsule_FromPrim(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdGeomCapsuleRef UsdGeomCapsule_Retain(UsdGeomCapsuleRef capsule);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdGeomCapsule_Release(UsdGeomCapsuleRef capsule);

/// Returns true if the capsule is valid.
USD_INTEROP_API bool UsdGeomCapsule_IsValid(UsdGeomCapsuleRef capsule);

/// Gets the underlying prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdGeomCapsule_GetPrim(UsdGeomCapsuleRef capsule);

/// Gets the height.
USD_INTEROP_API double UsdGeomCapsule_GetHeight(UsdGeomCapsuleRef capsule, UsdTimeCode time);

/// Sets the height.
USD_INTEROP_API UsdResult UsdGeomCapsule_SetHeight(
    UsdGeomCapsuleRef capsule,
    UsdTimeCode time,
    double height
);

/// Gets the radius.
USD_INTEROP_API double UsdGeomCapsule_GetRadius(UsdGeomCapsuleRef capsule, UsdTimeCode time);

/// Sets the radius.
USD_INTEROP_API UsdResult UsdGeomCapsule_SetRadius(
    UsdGeomCapsuleRef capsule,
    UsdTimeCode time,
    double radius
);

/// Gets the axis.
USD_INTEROP_API UsdGeomAxis UsdGeomCapsule_GetAxis(UsdGeomCapsuleRef capsule);

/// Sets the axis.
USD_INTEROP_API UsdResult UsdGeomCapsule_SetAxis(UsdGeomCapsuleRef capsule, UsdGeomAxis axis);

// ============================================================================
// MARK: - UsdGeomPrimvar
// ============================================================================

/// Creates a primvar from an attribute.
/// Caller must release with UsdGeomPrimvar_Release.
USD_INTEROP_API UsdGeomPrimvarRef UsdGeomPrimvar_FromAttribute(UsdAttributeRef attr);

/// Increments reference count.
USD_INTEROP_API UsdGeomPrimvarRef UsdGeomPrimvar_Retain(UsdGeomPrimvarRef primvar);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdGeomPrimvar_Release(UsdGeomPrimvarRef primvar);

/// Returns true if the primvar is valid.
USD_INTEROP_API bool UsdGeomPrimvar_IsValid(UsdGeomPrimvarRef primvar);

/// Returns true if the primvar has a value.
USD_INTEROP_API bool UsdGeomPrimvar_HasValue(UsdGeomPrimvarRef primvar);

/// Returns true if the primvar has authored value.
USD_INTEROP_API bool UsdGeomPrimvar_HasAuthoredValue(UsdGeomPrimvarRef primvar);

/// Gets the primvar name.
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef UsdGeomPrimvar_GetName(UsdGeomPrimvarRef primvar);

/// Gets the primvar's base name (without primvars: prefix).
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef UsdGeomPrimvar_GetBaseName(UsdGeomPrimvarRef primvar);

/// Gets the interpolation.
USD_INTEROP_API UsdGeomInterpolation UsdGeomPrimvar_GetInterpolation(UsdGeomPrimvarRef primvar);

/// Sets the interpolation.
USD_INTEROP_API UsdResult UsdGeomPrimvar_SetInterpolation(
    UsdGeomPrimvarRef primvar,
    UsdGeomInterpolation interpolation
);

/// Gets the element size.
USD_INTEROP_API int UsdGeomPrimvar_GetElementSize(UsdGeomPrimvarRef primvar);

/// Sets the element size.
USD_INTEROP_API UsdResult UsdGeomPrimvar_SetElementSize(UsdGeomPrimvarRef primvar, int elementSize);

/// Returns true if the primvar is indexed.
USD_INTEROP_API bool UsdGeomPrimvar_IsIndexed(UsdGeomPrimvarRef primvar);

/// Gets the underlying attribute.
/// Caller must release with UsdAttribute_Release.
USD_INTEROP_API UsdAttributeRef UsdGeomPrimvar_GetAttr(UsdGeomPrimvarRef primvar);

// ============================================================================
// MARK: - UsdGeomPrimvarsAPI
// ============================================================================

/// Creates a PrimvarsAPI for a prim.
/// Caller must release with UsdGeomPrimvarsAPI_Release.
USD_INTEROP_API UsdGeomPrimvarsAPIRef UsdGeomPrimvarsAPI_Get(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdGeomPrimvarsAPIRef UsdGeomPrimvarsAPI_Retain(UsdGeomPrimvarsAPIRef api);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdGeomPrimvarsAPI_Release(UsdGeomPrimvarsAPIRef api);

/// Returns true if the API is valid.
USD_INTEROP_API bool UsdGeomPrimvarsAPI_IsValid(UsdGeomPrimvarsAPIRef api);

/// Creates a primvar.
/// Caller must release with UsdGeomPrimvar_Release.
USD_INTEROP_API UsdGeomPrimvarRef UsdGeomPrimvarsAPI_CreatePrimvar(
    UsdGeomPrimvarsAPIRef api,
    TfTokenRef name,
    TfTokenRef typeName,
    UsdGeomInterpolation interpolation
);

/// Creates a primvar with element size.
/// Caller must release with UsdGeomPrimvar_Release.
USD_INTEROP_API UsdGeomPrimvarRef UsdGeomPrimvarsAPI_CreatePrimvarWithElementSize(
    UsdGeomPrimvarsAPIRef api,
    TfTokenRef name,
    TfTokenRef typeName,
    UsdGeomInterpolation interpolation,
    int elementSize
);

/// Gets a primvar by name.
/// Caller must release with UsdGeomPrimvar_Release.
USD_INTEROP_API UsdGeomPrimvarRef UsdGeomPrimvarsAPI_GetPrimvar(
    UsdGeomPrimvarsAPIRef api,
    TfTokenRef name
);

/// Returns true if a primvar with the given name exists.
USD_INTEROP_API bool UsdGeomPrimvarsAPI_HasPrimvar(UsdGeomPrimvarsAPIRef api, TfTokenRef name);

/// Removes a primvar.
USD_INTEROP_API UsdResult UsdGeomPrimvarsAPI_RemovePrimvar(
    UsdGeomPrimvarsAPIRef api,
    TfTokenRef name
);

/// Gets the count of primvars.
USD_INTEROP_API size_t UsdGeomPrimvarsAPI_GetPrimvarCount(UsdGeomPrimvarsAPIRef api);

/// Gets all primvars.
/// outPrimvars must have space for maxCount primvars.
/// Caller must release each with UsdGeomPrimvar_Release.
USD_INTEROP_API size_t UsdGeomPrimvarsAPI_GetPrimvars(
    UsdGeomPrimvarsAPIRef api,
    UsdGeomPrimvarRef* outPrimvars,
    size_t maxCount
);

// ============================================================================
// MARK: - UsdGeomXformCache
// ============================================================================

/// Creates a new XformCache for the given time.
/// Caller must release with UsdGeomXformCache_Release.
USD_INTEROP_API UsdGeomXformCacheRef UsdGeomXformCache_Create(UsdTimeCode time);

/// Increments reference count.
USD_INTEROP_API UsdGeomXformCacheRef UsdGeomXformCache_Retain(UsdGeomXformCacheRef cache);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdGeomXformCache_Release(UsdGeomXformCacheRef cache);

/// Computes the local-to-world transform for a prim.
/// outMatrix receives 16 doubles in row-major order.
USD_INTEROP_API UsdResult UsdGeomXformCache_GetLocalToWorldTransform(
    UsdGeomXformCacheRef cache,
    UsdPrimRef prim,
    double* outMatrix
);

/// Computes the parent-to-world transform for a prim.
/// outMatrix receives 16 doubles in row-major order.
USD_INTEROP_API UsdResult UsdGeomXformCache_GetParentToWorldTransform(
    UsdGeomXformCacheRef cache,
    UsdPrimRef prim,
    double* outMatrix
);

/// Computes the local transform for a prim.
/// outMatrix receives 16 doubles in row-major order.
/// outResetsXformStack indicates whether the xform stack is reset.
USD_INTEROP_API UsdResult UsdGeomXformCache_GetLocalTransformation(
    UsdGeomXformCacheRef cache,
    UsdPrimRef prim,
    double* outMatrix,
    bool* outResetsXformStack
);

/// Computes the relative transform between two prims.
/// outMatrix receives 16 doubles in row-major order.
USD_INTEROP_API UsdResult UsdGeomXformCache_ComputeRelativeTransform(
    UsdGeomXformCacheRef cache,
    UsdPrimRef prim,
    UsdPrimRef ancestorPrim,
    double* outMatrix
);

/// Sets the time for the cache.
USD_INTEROP_API void UsdGeomXformCache_SetTime(UsdGeomXformCacheRef cache, UsdTimeCode time);

/// Gets the time for the cache.
USD_INTEROP_API UsdTimeCode UsdGeomXformCache_GetTime(UsdGeomXformCacheRef cache);

/// Clears the cache.
USD_INTEROP_API void UsdGeomXformCache_Clear(UsdGeomXformCacheRef cache);

// ============================================================================
// MARK: - UsdGeomBBoxCache
// ============================================================================

/// Creates a new BBoxCache for the given time and purposes.
/// purposes is an array of purpose enums.
/// Caller must release with UsdGeomBBoxCache_Release.
USD_INTEROP_API UsdGeomBBoxCacheRef UsdGeomBBoxCache_Create(
    UsdTimeCode time,
    const UsdGeomPurpose* purposes,
    size_t purposeCount,
    bool useExtentsHint,
    bool ignoreVisibility
);

/// Increments reference count.
USD_INTEROP_API UsdGeomBBoxCacheRef UsdGeomBBoxCache_Retain(UsdGeomBBoxCacheRef cache);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdGeomBBoxCache_Release(UsdGeomBBoxCacheRef cache);

/// Computes the world-space bounding box for a prim.
/// outMin and outMax each receive 3 doubles.
USD_INTEROP_API UsdResult UsdGeomBBoxCache_ComputeWorldBound(
    UsdGeomBBoxCacheRef cache,
    UsdPrimRef prim,
    double* outMin,
    double* outMax
);

/// Computes the local-space bounding box for a prim.
/// outMin and outMax each receive 3 doubles.
USD_INTEROP_API UsdResult UsdGeomBBoxCache_ComputeLocalBound(
    UsdGeomBBoxCacheRef cache,
    UsdPrimRef prim,
    double* outMin,
    double* outMax
);

/// Computes the untransformed bounding box for a prim.
/// outMin and outMax each receive 3 doubles.
USD_INTEROP_API UsdResult UsdGeomBBoxCache_ComputeUntransformedBound(
    UsdGeomBBoxCacheRef cache,
    UsdPrimRef prim,
    double* outMin,
    double* outMax
);

/// Clears the cache.
USD_INTEROP_API void UsdGeomBBoxCache_Clear(UsdGeomBBoxCacheRef cache);

/// Sets the time for the cache.
USD_INTEROP_API void UsdGeomBBoxCache_SetTime(UsdGeomBBoxCacheRef cache, UsdTimeCode time);

/// Gets the time from the cache.
USD_INTEROP_API UsdTimeCode UsdGeomBBoxCache_GetTime(UsdGeomBBoxCacheRef cache);

// ============================================================================
// MARK: - Visibility and Purpose Helpers
// ============================================================================

/// Gets the visibility attribute value.
/// Returns true for visible, false for invisible.
USD_INTEROP_API bool UsdGeom_GetVisibility(UsdPrimRef prim, UsdTimeCode time);

/// Sets the visibility attribute value.
USD_INTEROP_API UsdResult UsdGeom_SetVisibility(UsdPrimRef prim, UsdTimeCode time, bool visible);

/// Gets the purpose attribute value.
USD_INTEROP_API UsdGeomPurpose UsdGeom_GetPurpose(UsdPrimRef prim);

/// Sets the purpose attribute value.
USD_INTEROP_API UsdResult UsdGeom_SetPurpose(UsdPrimRef prim, UsdGeomPurpose purpose);

#ifdef __cplusplus
}
#endif

#endif // OPENUSDINTEROP_USDGEOM_USDGEOM_H
