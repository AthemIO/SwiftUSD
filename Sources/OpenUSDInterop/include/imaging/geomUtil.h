// geomUtil.h - GeomUtil C wrapper
// Mirrors: pxr/imaging/geomUtil/*.h (Mesh generation utilities)

#ifndef OPENUSDINTEROP_IMAGING_GEOMUTIL_H
#define OPENUSDINTEROP_IMAGING_GEOMUTIL_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

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

/// Opaque handle to generated mesh data (points, normals, face vertex counts, face vertex indices).
typedef struct GeomUtilMeshDataOpaque* GeomUtilMeshDataRef;

// ============================================================================
// MARK: - Normal Interpolation Enum
// ============================================================================

/// Normal interpolation mode for mesh generation.
/// Mirrors pxr::GeomUtilMeshGeneratorBase normal interpolation tokens.
typedef enum {
    /// Single normal for entire geometry (e.g., plane, disk).
    GEOMUTIL_NORMALS_CONSTANT = 0,
    /// One normal per face (e.g., cuboid).
    GEOMUTIL_NORMALS_UNIFORM = 1,
    /// One normal per vertex (e.g., sphere, cylinder, cone, capsule).
    GEOMUTIL_NORMALS_VERTEX = 2
} GeomUtilNormalsInterpolation;

// ============================================================================
// MARK: - GeomUtilMeshData Lifecycle
// ============================================================================

/// Increments the reference count.
USD_INTEROP_API GeomUtilMeshDataRef GeomUtilMeshData_Retain(GeomUtilMeshDataRef mesh);

/// Decrements the reference count and frees if zero.
USD_INTEROP_API void GeomUtilMeshData_Release(GeomUtilMeshDataRef mesh);

// ============================================================================
// MARK: - GeomUtilMeshData Accessors
// ============================================================================

/// Gets the number of points in the mesh.
USD_INTEROP_API size_t GeomUtilMeshData_GetPointCount(GeomUtilMeshDataRef mesh);

/// Gets the points (vertices) as float[3] array.
/// - Parameters:
///   - mesh: The mesh data.
///   - outPoints: Output buffer for points (3 floats per point).
///   - maxCount: Maximum number of points to copy.
/// - Returns: Actual number of points copied.
USD_INTEROP_API size_t GeomUtilMeshData_GetPoints(
    GeomUtilMeshDataRef mesh,
    float* outPoints,
    size_t maxCount
);

/// Gets the number of normals in the mesh.
USD_INTEROP_API size_t GeomUtilMeshData_GetNormalCount(GeomUtilMeshDataRef mesh);

/// Gets the normals as float[3] array.
/// - Parameters:
///   - mesh: The mesh data.
///   - outNormals: Output buffer for normals (3 floats per normal).
///   - maxCount: Maximum number of normals to copy.
/// - Returns: Actual number of normals copied.
USD_INTEROP_API size_t GeomUtilMeshData_GetNormals(
    GeomUtilMeshDataRef mesh,
    float* outNormals,
    size_t maxCount
);

/// Gets the normal interpolation mode.
USD_INTEROP_API GeomUtilNormalsInterpolation GeomUtilMeshData_GetNormalsInterpolation(
    GeomUtilMeshDataRef mesh
);

/// Gets the number of faces in the mesh.
USD_INTEROP_API size_t GeomUtilMeshData_GetFaceCount(GeomUtilMeshDataRef mesh);

/// Gets the face vertex counts array.
/// - Parameters:
///   - mesh: The mesh data.
///   - outCounts: Output buffer for face vertex counts.
///   - maxCount: Maximum number of counts to copy.
/// - Returns: Actual number of counts copied.
USD_INTEROP_API size_t GeomUtilMeshData_GetFaceVertexCounts(
    GeomUtilMeshDataRef mesh,
    int* outCounts,
    size_t maxCount
);

/// Gets the total number of face vertex indices.
USD_INTEROP_API size_t GeomUtilMeshData_GetFaceVertexIndexCount(GeomUtilMeshDataRef mesh);

/// Gets the face vertex indices array.
/// - Parameters:
///   - mesh: The mesh data.
///   - outIndices: Output buffer for face vertex indices.
///   - maxCount: Maximum number of indices to copy.
/// - Returns: Actual number of indices copied.
USD_INTEROP_API size_t GeomUtilMeshData_GetFaceVertexIndices(
    GeomUtilMeshDataRef mesh,
    int* outIndices,
    size_t maxCount
);

// ============================================================================
// MARK: - Sphere Mesh Generation
// ============================================================================

/// Generates a sphere mesh.
/// - Parameters:
///   - numRadial: Number of radial segments (minimum 3).
///   - numAxial: Number of axial segments (minimum 2).
///   - radius: Sphere radius.
///   - sweepDegrees: Sweep angle in degrees (360 for full sphere).
///   - transform: Optional 4x4 transformation matrix (16 floats, row-major). NULL for identity.
/// - Returns: Mesh data handle, or NULL on failure. Caller must release.
USD_INTEROP_API GeomUtilMeshDataRef GeomUtil_GenerateSphereMesh(
    size_t numRadial,
    size_t numAxial,
    float radius,
    float sweepDegrees,
    const float* transform
);

/// Computes the number of points for a sphere mesh.
USD_INTEROP_API size_t GeomUtil_ComputeSpherePointCount(
    size_t numRadial,
    size_t numAxial,
    bool closedSweep
);

// ============================================================================
// MARK: - Cuboid Mesh Generation
// ============================================================================

/// Generates a cuboid (rectangular box) mesh.
/// - Parameters:
///   - xLength: Length along X axis.
///   - yLength: Length along Y axis.
///   - zLength: Length along Z axis.
///   - transform: Optional 4x4 transformation matrix (16 floats, row-major). NULL for identity.
/// - Returns: Mesh data handle, or NULL on failure. Caller must release.
USD_INTEROP_API GeomUtilMeshDataRef GeomUtil_GenerateCuboidMesh(
    float xLength,
    float yLength,
    float zLength,
    const float* transform
);

/// Computes the number of points for a cuboid mesh (fixed topology).
USD_INTEROP_API size_t GeomUtil_ComputeCuboidPointCount(void);

// ============================================================================
// MARK: - Cylinder Mesh Generation
// ============================================================================

/// Generates a cylinder mesh.
/// - Parameters:
///   - numRadial: Number of radial segments (minimum 3).
///   - bottomRadius: Radius at the bottom.
///   - topRadius: Radius at the top.
///   - height: Height of the cylinder.
///   - sweepDegrees: Sweep angle in degrees (360 for full cylinder).
///   - transform: Optional 4x4 transformation matrix (16 floats, row-major). NULL for identity.
/// - Returns: Mesh data handle, or NULL on failure. Caller must release.
USD_INTEROP_API GeomUtilMeshDataRef GeomUtil_GenerateCylinderMesh(
    size_t numRadial,
    float bottomRadius,
    float topRadius,
    float height,
    float sweepDegrees,
    const float* transform
);

/// Computes the number of points for a cylinder mesh.
USD_INTEROP_API size_t GeomUtil_ComputeCylinderPointCount(
    size_t numRadial,
    bool closedSweep
);

// ============================================================================
// MARK: - Cone Mesh Generation
// ============================================================================

/// Generates a cone mesh.
/// - Parameters:
///   - numRadial: Number of radial segments (minimum 3).
///   - radius: Base radius.
///   - height: Height of the cone.
///   - sweepDegrees: Sweep angle in degrees (360 for full cone).
///   - transform: Optional 4x4 transformation matrix (16 floats, row-major). NULL for identity.
/// - Returns: Mesh data handle, or NULL on failure. Caller must release.
USD_INTEROP_API GeomUtilMeshDataRef GeomUtil_GenerateConeMesh(
    size_t numRadial,
    float radius,
    float height,
    float sweepDegrees,
    const float* transform
);

/// Computes the number of points for a cone mesh.
USD_INTEROP_API size_t GeomUtil_ComputeConePointCount(
    size_t numRadial,
    bool closedSweep
);

// ============================================================================
// MARK: - Capsule Mesh Generation
// ============================================================================

/// Generates a capsule mesh (cylinder with hemispherical caps).
/// - Parameters:
///   - numRadial: Number of radial segments (minimum 3).
///   - numCapAxial: Number of axial segments per hemisphere cap (minimum 1).
///   - bottomRadius: Radius of the bottom cap.
///   - topRadius: Radius of the top cap.
///   - height: Height of the cylindrical portion.
///   - sweepDegrees: Sweep angle in degrees (360 for full capsule).
///   - transform: Optional 4x4 transformation matrix (16 floats, row-major). NULL for identity.
/// - Returns: Mesh data handle, or NULL on failure. Caller must release.
USD_INTEROP_API GeomUtilMeshDataRef GeomUtil_GenerateCapsuleMesh(
    size_t numRadial,
    size_t numCapAxial,
    float bottomRadius,
    float topRadius,
    float height,
    float sweepDegrees,
    const float* transform
);

/// Computes the number of points for a capsule mesh.
USD_INTEROP_API size_t GeomUtil_ComputeCapsulePointCount(
    size_t numRadial,
    size_t numCapAxial,
    bool closedSweep
);

// ============================================================================
// MARK: - Plane Mesh Generation
// ============================================================================

/// Generates a plane mesh.
/// - Parameters:
///   - xLength: Length along X axis.
///   - yLength: Length along Y axis.
///   - transform: Optional 4x4 transformation matrix (16 floats, row-major). NULL for identity.
/// - Returns: Mesh data handle, or NULL on failure. Caller must release.
USD_INTEROP_API GeomUtilMeshDataRef GeomUtil_GeneratePlaneMesh(
    float xLength,
    float yLength,
    const float* transform
);

/// Computes the number of points for a plane mesh (fixed topology).
USD_INTEROP_API size_t GeomUtil_ComputePlanePointCount(void);

// ============================================================================
// MARK: - Disk Mesh Generation
// ============================================================================

/// Generates a disk mesh.
/// - Parameters:
///   - numRadial: Number of radial segments (minimum 3).
///   - radius: Disk radius.
///   - sweepDegrees: Sweep angle in degrees (360 for full disk).
///   - transform: Optional 4x4 transformation matrix (16 floats, row-major). NULL for identity.
/// - Returns: Mesh data handle, or NULL on failure. Caller must release.
USD_INTEROP_API GeomUtilMeshDataRef GeomUtil_GenerateDiskMesh(
    size_t numRadial,
    float radius,
    float sweepDegrees,
    const float* transform
);

/// Computes the number of points for a disk mesh.
USD_INTEROP_API size_t GeomUtil_ComputeDiskPointCount(
    size_t numRadial,
    bool closedSweep
);

#ifdef __cplusplus
}
#endif

#endif // OPENUSDINTEROP_IMAGING_GEOMUTIL_H
