// usdGeom.cpp - UsdGeom module C wrapper implementation
// Mirrors: pxr/usd/usdGeom/*.h

#include "../../include/usdGeom/usdGeom.h"

// Check if we should use full USD library
#if defined(USE_PIXAR_USD) && USE_PIXAR_USD
    #define USD_USE_FULL 1
    #include <pxr/usd/usdGeom/xform.h>
    #include <pxr/usd/usdGeom/mesh.h>
    #include <pxr/usd/usdGeom/points.h>
    #include <pxr/usd/usdGeom/basisCurves.h>
    #include <pxr/usd/usdGeom/camera.h>
    #include <pxr/usd/usdGeom/sphere.h>
    #include <pxr/usd/usdGeom/cube.h>
    #include <pxr/usd/usdGeom/cylinder.h>
    #include <pxr/usd/usdGeom/cone.h>
    #include <pxr/usd/usdGeom/capsule.h>
    #include <pxr/usd/usdGeom/primvar.h>
    #include <pxr/usd/usdGeom/primvarsAPI.h>
    #include <pxr/usd/usdGeom/xformCache.h>
    #include <pxr/usd/usdGeom/bboxCache.h>
    #include <pxr/usd/usdGeom/imageable.h>
    #include <pxr/base/gf/matrix4d.h>
    #include <pxr/base/gf/vec3f.h>
    #include <pxr/base/gf/vec3d.h>
    PXR_NAMESPACE_USING_DIRECTIVE
#else
    #define USD_USE_FULL 0
#endif

#include <atomic>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <mutex>

// ============================================================================
// MARK: - Wrapper Structures
// ============================================================================

struct UsdGeomXformOpaque {
#if USD_USE_FULL
    UsdGeomXform xform;
#endif
    std::atomic<int> refCount;
    bool valid;

    UsdGeomXformOpaque() : refCount(1), valid(false) {}

#if USD_USE_FULL
    explicit UsdGeomXformOpaque(const UsdGeomXform& x)
        : xform(x), refCount(1), valid(true) {}
#endif
};

struct UsdGeomMeshOpaque {
#if USD_USE_FULL
    UsdGeomMesh mesh;
#endif
    std::atomic<int> refCount;
    bool valid;
    std::vector<float> points;
    std::vector<int> faceVertexCounts;
    std::vector<int> faceVertexIndices;
    bool doubleSided;
    int subdivisionScheme;

    UsdGeomMeshOpaque() : refCount(1), valid(false), doubleSided(false), subdivisionScheme(0) {}

#if USD_USE_FULL
    explicit UsdGeomMeshOpaque(const UsdGeomMesh& m)
        : mesh(m), refCount(1), valid(true), doubleSided(false), subdivisionScheme(0) {}
#endif
};

struct UsdGeomPointsOpaque {
#if USD_USE_FULL
    UsdGeomPoints points;
#endif
    std::atomic<int> refCount;
    bool valid;

    UsdGeomPointsOpaque() : refCount(1), valid(false) {}

#if USD_USE_FULL
    explicit UsdGeomPointsOpaque(const UsdGeomPoints& p)
        : points(p), refCount(1), valid(true) {}
#endif
};

struct UsdGeomBasisCurvesOpaque {
#if USD_USE_FULL
    UsdGeomBasisCurves curves;
#endif
    std::atomic<int> refCount;
    bool valid;

    UsdGeomBasisCurvesOpaque() : refCount(1), valid(false) {}

#if USD_USE_FULL
    explicit UsdGeomBasisCurvesOpaque(const UsdGeomBasisCurves& c)
        : curves(c), refCount(1), valid(true) {}
#endif
};

struct UsdGeomCameraOpaque {
#if USD_USE_FULL
    UsdGeomCamera camera;
#endif
    std::atomic<int> refCount;
    bool valid;
    float focalLength;
    float horizontalAperture;
    float verticalAperture;
    float nearClip;
    float farClip;

    UsdGeomCameraOpaque() : refCount(1), valid(false),
        focalLength(50.0f), horizontalAperture(36.0f), verticalAperture(24.0f),
        nearClip(0.1f), farClip(10000.0f) {}

#if USD_USE_FULL
    explicit UsdGeomCameraOpaque(const UsdGeomCamera& c)
        : camera(c), refCount(1), valid(true),
          focalLength(50.0f), horizontalAperture(36.0f), verticalAperture(24.0f),
          nearClip(0.1f), farClip(10000.0f) {}
#endif
};

struct UsdGeomSphereOpaque {
#if USD_USE_FULL
    UsdGeomSphere sphere;
#endif
    std::atomic<int> refCount;
    bool valid;
    double radius;

    UsdGeomSphereOpaque() : refCount(1), valid(false), radius(1.0) {}

#if USD_USE_FULL
    explicit UsdGeomSphereOpaque(const UsdGeomSphere& s)
        : sphere(s), refCount(1), valid(true), radius(1.0) {}
#endif
};

struct UsdGeomCubeOpaque {
#if USD_USE_FULL
    UsdGeomCube cube;
#endif
    std::atomic<int> refCount;
    bool valid;
    double size;

    UsdGeomCubeOpaque() : refCount(1), valid(false), size(2.0) {}

#if USD_USE_FULL
    explicit UsdGeomCubeOpaque(const UsdGeomCube& c)
        : cube(c), refCount(1), valid(true), size(2.0) {}
#endif
};

struct UsdGeomCylinderOpaque {
#if USD_USE_FULL
    UsdGeomCylinder cylinder;
#endif
    std::atomic<int> refCount;
    bool valid;
    double height;
    double radius;
    int axis;

    UsdGeomCylinderOpaque() : refCount(1), valid(false), height(2.0), radius(1.0), axis(2) {}

#if USD_USE_FULL
    explicit UsdGeomCylinderOpaque(const UsdGeomCylinder& c)
        : cylinder(c), refCount(1), valid(true), height(2.0), radius(1.0), axis(2) {}
#endif
};

struct UsdGeomConeOpaque {
#if USD_USE_FULL
    UsdGeomCone cone;
#endif
    std::atomic<int> refCount;
    bool valid;
    double height;
    double radius;
    int axis;

    UsdGeomConeOpaque() : refCount(1), valid(false), height(2.0), radius(1.0), axis(2) {}

#if USD_USE_FULL
    explicit UsdGeomConeOpaque(const UsdGeomCone& c)
        : cone(c), refCount(1), valid(true), height(2.0), radius(1.0), axis(2) {}
#endif
};

struct UsdGeomCapsuleOpaque {
#if USD_USE_FULL
    UsdGeomCapsule capsule;
#endif
    std::atomic<int> refCount;
    bool valid;
    double height;
    double radius;
    int axis;

    UsdGeomCapsuleOpaque() : refCount(1), valid(false), height(2.0), radius(0.5), axis(2) {}

#if USD_USE_FULL
    explicit UsdGeomCapsuleOpaque(const UsdGeomCapsule& c)
        : capsule(c), refCount(1), valid(true), height(2.0), radius(0.5), axis(2) {}
#endif
};

struct UsdGeomPrimvarOpaque {
#if USD_USE_FULL
    UsdGeomPrimvar primvar;
#endif
    std::atomic<int> refCount;
    bool valid;

    UsdGeomPrimvarOpaque() : refCount(1), valid(false) {}

#if USD_USE_FULL
    explicit UsdGeomPrimvarOpaque(const UsdGeomPrimvar& p)
        : primvar(p), refCount(1), valid(true) {}
#endif
};

struct UsdGeomPrimvarsAPIOpaque {
#if USD_USE_FULL
    UsdGeomPrimvarsAPI api;
#endif
    std::atomic<int> refCount;
    bool valid;

    UsdGeomPrimvarsAPIOpaque() : refCount(1), valid(false) {}

#if USD_USE_FULL
    explicit UsdGeomPrimvarsAPIOpaque(const UsdGeomPrimvarsAPI& a)
        : api(a), refCount(1), valid(true) {}
#endif
};

struct UsdGeomXformCacheOpaque {
#if USD_USE_FULL
    UsdGeomXformCache cache;
#endif
    std::atomic<int> refCount;
    double time;

    UsdGeomXformCacheOpaque() : refCount(1), time(0.0) {}

#if USD_USE_FULL
    explicit UsdGeomXformCacheOpaque(const UsdTimeCode& t)
        : cache(t), refCount(1), time(t.IsDefault() ? 0.0 : t.GetValue()) {}
#endif
};

struct UsdGeomBBoxCacheOpaque {
#if USD_USE_FULL
    UsdGeomBBoxCache* cache;
#endif
    std::atomic<int> refCount;
    double time;

    UsdGeomBBoxCacheOpaque() : refCount(1), time(0.0) {
#if USD_USE_FULL
        cache = nullptr;
#endif
    }

    ~UsdGeomBBoxCacheOpaque() {
#if USD_USE_FULL
        delete cache;
#endif
    }
};

// ============================================================================
// MARK: - Helper Macros
// ============================================================================

#define CATCH_AND_RETURN(defaultValue) \
    catch (...) { return defaultValue; }

#define CATCH_AND_RETURN_RESULT \
    catch (...) { return USD_RESULT_ERROR; }

// ============================================================================
// MARK: - UsdGeomXform Implementation
// ============================================================================

extern "C" {

UsdGeomXformRef UsdGeomXform_Get(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        UsdGeomXform xform = UsdGeomXform::Get(stage->stage, path->path);
        if (xform) {
            return new UsdGeomXformOpaque(xform);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdGeomXformRef UsdGeomXform_Define(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        UsdGeomXform xform = UsdGeomXform::Define(stage->stage, path->path);
        if (xform) {
            return new UsdGeomXformOpaque(xform);
        }
        return nullptr;
#else
        auto* wrapper = new UsdGeomXformOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdGeomXformRef UsdGeomXform_FromPrim(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdGeomXform xform(prim->prim);
        if (xform) {
            return new UsdGeomXformOpaque(xform);
        }
        return nullptr;
#else
        auto* wrapper = new UsdGeomXformOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdGeomXformRef UsdGeomXform_Retain(UsdGeomXformRef xform) {
    if (!xform) return nullptr;
    xform->refCount.fetch_add(1, std::memory_order_relaxed);
    return xform;
}

void UsdGeomXform_Release(UsdGeomXformRef xform) {
    if (!xform) return;
    if (xform->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete xform;
    }
}

bool UsdGeomXform_IsValid(UsdGeomXformRef xform) {
    if (!xform) return false;
#if USD_USE_FULL
    return xform->xform;
#else
    return xform->valid;
#endif
}

UsdPrimRef UsdGeomXform_GetPrim(UsdGeomXformRef xform) {
    if (!xform) return nullptr;
#if USD_USE_FULL
    try {
        UsdPrim prim = xform->xform.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    // In stub mode, we can't create UsdPrimOpaque from here
    // Return nullptr - the Swift layer will handle this gracefully
    return nullptr;
#endif
}

UsdResult UsdGeomXform_ComputeLocalToWorldTransform(
    UsdGeomXformRef xform,
    UsdTimeCode time,
    double* outMatrix
) {
    if (!xform || !outMatrix) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        GfMatrix4d matrix = xform->xform.ComputeLocalToWorldTransform(
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time)
        );
        const double* data = matrix.data();
        for (int i = 0; i < 16; ++i) {
            outMatrix[i] = data[i];
        }
        return USD_RESULT_SUCCESS;
#else
        // Return identity matrix
        for (int i = 0; i < 16; ++i) {
            outMatrix[i] = (i % 5 == 0) ? 1.0 : 0.0;
        }
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdGeomXform_ComputeParentToWorldTransform(
    UsdGeomXformRef xform,
    UsdTimeCode time,
    double* outMatrix
) {
    if (!xform || !outMatrix) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        GfMatrix4d matrix = xform->xform.ComputeParentToWorldTransform(
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time)
        );
        const double* data = matrix.data();
        for (int i = 0; i < 16; ++i) {
            outMatrix[i] = data[i];
        }
        return USD_RESULT_SUCCESS;
#else
        // Return identity matrix
        for (int i = 0; i < 16; ++i) {
            outMatrix[i] = (i % 5 == 0) ? 1.0 : 0.0;
        }
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdGeomXform_SetTransformMatrix(
    UsdGeomXformRef xform,
    UsdTimeCode time,
    const double* matrix
) {
    if (!xform || !matrix) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        GfMatrix4d mat;
        for (int i = 0; i < 16; ++i) {
            mat.data()[i] = matrix[i];
        }
        bool resetStack = false;
        xform->xform.SetResetXformStack(resetStack);
        auto op = xform->xform.MakeMatrixXform();
        op.Set(mat, time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdGeomXform_AddTranslateOp(
    UsdGeomXformRef xform,
    double x, double y, double z,
    UsdTimeCode time
) {
    if (!xform) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        auto op = xform->xform.AddTranslateOp();
        op.Set(GfVec3d(x, y, z), time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdGeomXform_AddRotateXYZOp(
    UsdGeomXformRef xform,
    double rotX, double rotY, double rotZ,
    UsdTimeCode time
) {
    if (!xform) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        auto op = xform->xform.AddRotateXYZOp();
        op.Set(GfVec3f(static_cast<float>(rotX), static_cast<float>(rotY), static_cast<float>(rotZ)),
               time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdGeomXform_AddScaleOp(
    UsdGeomXformRef xform,
    double scaleX, double scaleY, double scaleZ,
    UsdTimeCode time
) {
    if (!xform) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        auto op = xform->xform.AddScaleOp();
        op.Set(GfVec3f(static_cast<float>(scaleX), static_cast<float>(scaleY), static_cast<float>(scaleZ)),
               time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdGeomXform_ClearXformOpOrder(UsdGeomXformRef xform) {
    if (!xform) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        xform->xform.ClearXformOpOrder();
        return USD_RESULT_SUCCESS;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

bool UsdGeomXform_GetResetXformStack(UsdGeomXformRef xform) {
    if (!xform) return false;
#if USD_USE_FULL
    return xform->xform.GetResetXformStack();
#else
    return false;
#endif
}

UsdResult UsdGeomXform_SetResetXformStack(UsdGeomXformRef xform, bool reset) {
    if (!xform) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        xform->xform.SetResetXformStack(reset);
        return USD_RESULT_SUCCESS;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

// ============================================================================
// MARK: - UsdGeomMesh Implementation
// ============================================================================

UsdGeomMeshRef UsdGeomMesh_Get(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        UsdGeomMesh mesh = UsdGeomMesh::Get(stage->stage, path->path);
        if (mesh) {
            return new UsdGeomMeshOpaque(mesh);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdGeomMeshRef UsdGeomMesh_Define(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        UsdGeomMesh mesh = UsdGeomMesh::Define(stage->stage, path->path);
        if (mesh) {
            return new UsdGeomMeshOpaque(mesh);
        }
        return nullptr;
#else
        auto* wrapper = new UsdGeomMeshOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdGeomMeshRef UsdGeomMesh_FromPrim(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdGeomMesh mesh(prim->prim);
        if (mesh) {
            return new UsdGeomMeshOpaque(mesh);
        }
        return nullptr;
#else
        auto* wrapper = new UsdGeomMeshOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdGeomMeshRef UsdGeomMesh_Retain(UsdGeomMeshRef mesh) {
    if (!mesh) return nullptr;
    mesh->refCount.fetch_add(1, std::memory_order_relaxed);
    return mesh;
}

void UsdGeomMesh_Release(UsdGeomMeshRef mesh) {
    if (!mesh) return;
    if (mesh->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete mesh;
    }
}

bool UsdGeomMesh_IsValid(UsdGeomMeshRef mesh) {
    if (!mesh) return false;
#if USD_USE_FULL
    return mesh->mesh;
#else
    return mesh->valid;
#endif
}

UsdPrimRef UsdGeomMesh_GetPrim(UsdGeomMeshRef mesh) {
    if (!mesh) return nullptr;
#if USD_USE_FULL
    try {
        UsdPrim prim = mesh->mesh.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

size_t UsdGeomMesh_GetPointCount(UsdGeomMeshRef mesh, UsdTimeCode time) {
    if (!mesh) return 0;
    try {
#if USD_USE_FULL
        VtVec3fArray points;
        mesh->mesh.GetPointsAttr().Get(&points,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return points.size();
#else
        return mesh->points.size() / 3;
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdGeomMesh_GetPoints(
    UsdGeomMeshRef mesh,
    UsdTimeCode time,
    float* outPoints,
    size_t maxCount
) {
    if (!mesh || !outPoints) return 0;
    try {
#if USD_USE_FULL
        VtVec3fArray points;
        mesh->mesh.GetPointsAttr().Get(&points,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        size_t count = std::min(points.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outPoints[i * 3 + 0] = points[i][0];
            outPoints[i * 3 + 1] = points[i][1];
            outPoints[i * 3 + 2] = points[i][2];
        }
        return count;
#else
        size_t pointCount = mesh->points.size() / 3;
        size_t count = std::min(pointCount, maxCount);
        std::memcpy(outPoints, mesh->points.data(), count * 3 * sizeof(float));
        return count;
#endif
    } CATCH_AND_RETURN(0)
}

UsdResult UsdGeomMesh_SetPoints(
    UsdGeomMeshRef mesh,
    UsdTimeCode time,
    const float* points,
    size_t count
) {
    if (!mesh || !points) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        VtVec3fArray vtPoints(count);
        for (size_t i = 0; i < count; ++i) {
            vtPoints[i] = GfVec3f(points[i * 3 + 0], points[i * 3 + 1], points[i * 3 + 2]);
        }
        mesh->mesh.GetPointsAttr().Set(vtPoints,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
#else
        mesh->points.resize(count * 3);
        std::memcpy(mesh->points.data(), points, count * 3 * sizeof(float));
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

size_t UsdGeomMesh_GetFaceVertexCountsCount(UsdGeomMeshRef mesh, UsdTimeCode time) {
    if (!mesh) return 0;
    try {
#if USD_USE_FULL
        VtIntArray counts;
        mesh->mesh.GetFaceVertexCountsAttr().Get(&counts,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return counts.size();
#else
        return mesh->faceVertexCounts.size();
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdGeomMesh_GetFaceVertexCounts(
    UsdGeomMeshRef mesh,
    UsdTimeCode time,
    int* outCounts,
    size_t maxCount
) {
    if (!mesh || !outCounts) return 0;
    try {
#if USD_USE_FULL
        VtIntArray counts;
        mesh->mesh.GetFaceVertexCountsAttr().Get(&counts,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        size_t count = std::min(counts.size(), maxCount);
        std::memcpy(outCounts, counts.data(), count * sizeof(int));
        return count;
#else
        size_t count = std::min(mesh->faceVertexCounts.size(), maxCount);
        std::memcpy(outCounts, mesh->faceVertexCounts.data(), count * sizeof(int));
        return count;
#endif
    } CATCH_AND_RETURN(0)
}

UsdResult UsdGeomMesh_SetFaceVertexCounts(
    UsdGeomMeshRef mesh,
    UsdTimeCode time,
    const int* counts,
    size_t count
) {
    if (!mesh || !counts) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        VtIntArray vtCounts(count);
        std::memcpy(vtCounts.data(), counts, count * sizeof(int));
        mesh->mesh.GetFaceVertexCountsAttr().Set(vtCounts,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
#else
        mesh->faceVertexCounts.resize(count);
        std::memcpy(mesh->faceVertexCounts.data(), counts, count * sizeof(int));
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

size_t UsdGeomMesh_GetFaceVertexIndicesCount(UsdGeomMeshRef mesh, UsdTimeCode time) {
    if (!mesh) return 0;
    try {
#if USD_USE_FULL
        VtIntArray indices;
        mesh->mesh.GetFaceVertexIndicesAttr().Get(&indices,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return indices.size();
#else
        return mesh->faceVertexIndices.size();
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdGeomMesh_GetFaceVertexIndices(
    UsdGeomMeshRef mesh,
    UsdTimeCode time,
    int* outIndices,
    size_t maxCount
) {
    if (!mesh || !outIndices) return 0;
    try {
#if USD_USE_FULL
        VtIntArray indices;
        mesh->mesh.GetFaceVertexIndicesAttr().Get(&indices,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        size_t count = std::min(indices.size(), maxCount);
        std::memcpy(outIndices, indices.data(), count * sizeof(int));
        return count;
#else
        size_t count = std::min(mesh->faceVertexIndices.size(), maxCount);
        std::memcpy(outIndices, mesh->faceVertexIndices.data(), count * sizeof(int));
        return count;
#endif
    } CATCH_AND_RETURN(0)
}

UsdResult UsdGeomMesh_SetFaceVertexIndices(
    UsdGeomMeshRef mesh,
    UsdTimeCode time,
    const int* indices,
    size_t count
) {
    if (!mesh || !indices) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        VtIntArray vtIndices(count);
        std::memcpy(vtIndices.data(), indices, count * sizeof(int));
        mesh->mesh.GetFaceVertexIndicesAttr().Set(vtIndices,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
#else
        mesh->faceVertexIndices.resize(count);
        std::memcpy(mesh->faceVertexIndices.data(), indices, count * sizeof(int));
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

size_t UsdGeomMesh_GetNormalsCount(UsdGeomMeshRef mesh, UsdTimeCode time) {
    if (!mesh) return 0;
#if USD_USE_FULL
    try {
        VtVec3fArray normals;
        mesh->mesh.GetNormalsAttr().Get(&normals,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return normals.size();
    } CATCH_AND_RETURN(0)
#else
    return 0;
#endif
}

size_t UsdGeomMesh_GetNormals(
    UsdGeomMeshRef mesh,
    UsdTimeCode time,
    float* outNormals,
    size_t maxCount
) {
    if (!mesh || !outNormals) return 0;
#if USD_USE_FULL
    try {
        VtVec3fArray normals;
        mesh->mesh.GetNormalsAttr().Get(&normals,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        size_t count = std::min(normals.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outNormals[i * 3 + 0] = normals[i][0];
            outNormals[i * 3 + 1] = normals[i][1];
            outNormals[i * 3 + 2] = normals[i][2];
        }
        return count;
    } CATCH_AND_RETURN(0)
#else
    return 0;
#endif
}

UsdResult UsdGeomMesh_SetNormals(
    UsdGeomMeshRef mesh,
    UsdTimeCode time,
    const float* normals,
    size_t count,
    UsdGeomInterpolation interpolation
) {
    if (!mesh || !normals) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        VtVec3fArray vtNormals(count);
        for (size_t i = 0; i < count; ++i) {
            vtNormals[i] = GfVec3f(normals[i * 3 + 0], normals[i * 3 + 1], normals[i * 3 + 2]);
        }
        mesh->mesh.GetNormalsAttr().Set(vtNormals,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    return USD_RESULT_SUCCESS;
#endif
}

UsdGeomInterpolation UsdGeomMesh_GetNormalsInterpolation(UsdGeomMeshRef mesh) {
    if (!mesh) return USDGEOM_INTERPOLATION_VERTEX;
#if USD_USE_FULL
    TfToken interp = mesh->mesh.GetNormalsInterpolation();
    if (interp == UsdGeomTokens->constant) return USDGEOM_INTERPOLATION_CONSTANT;
    if (interp == UsdGeomTokens->uniform) return USDGEOM_INTERPOLATION_UNIFORM;
    if (interp == UsdGeomTokens->varying) return USDGEOM_INTERPOLATION_VARYING;
    if (interp == UsdGeomTokens->vertex) return USDGEOM_INTERPOLATION_VERTEX;
    if (interp == UsdGeomTokens->faceVarying) return USDGEOM_INTERPOLATION_FACEVARYING;
    return USDGEOM_INTERPOLATION_VERTEX;
#else
    return USDGEOM_INTERPOLATION_VERTEX;
#endif
}

UsdGeomSubdivisionScheme UsdGeomMesh_GetSubdivisionScheme(UsdGeomMeshRef mesh) {
    if (!mesh) return USDGEOM_SUBDIV_NONE;
#if USD_USE_FULL
    TfToken scheme;
    mesh->mesh.GetSubdivisionSchemeAttr().Get(&scheme);
    if (scheme == UsdGeomTokens->none) return USDGEOM_SUBDIV_NONE;
    if (scheme == UsdGeomTokens->catmullClark) return USDGEOM_SUBDIV_CATMULL_CLARK;
    if (scheme == UsdGeomTokens->loop) return USDGEOM_SUBDIV_LOOP;
    if (scheme == UsdGeomTokens->bilinear) return USDGEOM_SUBDIV_BILINEAR;
    return USDGEOM_SUBDIV_NONE;
#else
    return static_cast<UsdGeomSubdivisionScheme>(mesh->subdivisionScheme);
#endif
}

UsdResult UsdGeomMesh_SetSubdivisionScheme(
    UsdGeomMeshRef mesh,
    UsdGeomSubdivisionScheme scheme
) {
    if (!mesh) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        TfToken schemeToken;
        switch (scheme) {
            case USDGEOM_SUBDIV_NONE: schemeToken = UsdGeomTokens->none; break;
            case USDGEOM_SUBDIV_CATMULL_CLARK: schemeToken = UsdGeomTokens->catmullClark; break;
            case USDGEOM_SUBDIV_LOOP: schemeToken = UsdGeomTokens->loop; break;
            case USDGEOM_SUBDIV_BILINEAR: schemeToken = UsdGeomTokens->bilinear; break;
            default: schemeToken = UsdGeomTokens->none; break;
        }
        mesh->mesh.GetSubdivisionSchemeAttr().Set(schemeToken);
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    mesh->subdivisionScheme = static_cast<int>(scheme);
    return USD_RESULT_SUCCESS;
#endif
}

UsdResult UsdGeomMesh_ComputeExtent(
    UsdGeomMeshRef mesh,
    UsdTimeCode time,
    float* outExtent
) {
    if (!mesh || !outExtent) return USD_RESULT_INVALID_ARGUMENT;
    // Return default extent
    outExtent[0] = -1.0f;
    outExtent[1] = -1.0f;
    outExtent[2] = -1.0f;
    outExtent[3] = 1.0f;
    outExtent[4] = 1.0f;
    outExtent[5] = 1.0f;
    return USD_RESULT_SUCCESS;
}

UsdResult UsdGeomMesh_GetExtent(
    UsdGeomMeshRef mesh,
    UsdTimeCode time,
    float* outExtent
) {
    if (!mesh || !outExtent) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        VtVec3fArray extent;
        if (mesh->mesh.GetExtentAttr().Get(&extent,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time))) {
            if (extent.size() >= 2) {
                outExtent[0] = extent[0][0];
                outExtent[1] = extent[0][1];
                outExtent[2] = extent[0][2];
                outExtent[3] = extent[1][0];
                outExtent[4] = extent[1][1];
                outExtent[5] = extent[1][2];
                return USD_RESULT_SUCCESS;
            }
        }
        return USD_RESULT_ERROR;
    } CATCH_AND_RETURN_RESULT
#else
    outExtent[0] = -1.0f;
    outExtent[1] = -1.0f;
    outExtent[2] = -1.0f;
    outExtent[3] = 1.0f;
    outExtent[4] = 1.0f;
    outExtent[5] = 1.0f;
    return USD_RESULT_SUCCESS;
#endif
}

UsdResult UsdGeomMesh_SetExtent(
    UsdGeomMeshRef mesh,
    UsdTimeCode time,
    const float* extent
) {
    if (!mesh || !extent) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        VtVec3fArray vtExtent(2);
        vtExtent[0] = GfVec3f(extent[0], extent[1], extent[2]);
        vtExtent[1] = GfVec3f(extent[3], extent[4], extent[5]);
        mesh->mesh.GetExtentAttr().Set(vtExtent,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    return USD_RESULT_SUCCESS;
#endif
}

bool UsdGeomMesh_GetDoubleSided(UsdGeomMeshRef mesh) {
    if (!mesh) return false;
#if USD_USE_FULL
    bool doubleSided = false;
    mesh->mesh.GetDoubleSidedAttr().Get(&doubleSided);
    return doubleSided;
#else
    return mesh->doubleSided;
#endif
}

UsdResult UsdGeomMesh_SetDoubleSided(UsdGeomMeshRef mesh, bool doubleSided) {
    if (!mesh) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        mesh->mesh.GetDoubleSidedAttr().Set(doubleSided);
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    mesh->doubleSided = doubleSided;
    return USD_RESULT_SUCCESS;
#endif
}

// ============================================================================
// MARK: - UsdGeomSphere Implementation
// ============================================================================

UsdGeomSphereRef UsdGeomSphere_Define(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        UsdGeomSphere sphere = UsdGeomSphere::Define(stage->stage, path->path);
        if (sphere) {
            return new UsdGeomSphereOpaque(sphere);
        }
        return nullptr;
#else
        auto* wrapper = new UsdGeomSphereOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdGeomSphereRef UsdGeomSphere_FromPrim(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdGeomSphere sphere(prim->prim);
        if (sphere) {
            return new UsdGeomSphereOpaque(sphere);
        }
        return nullptr;
#else
        auto* wrapper = new UsdGeomSphereOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdGeomSphereRef UsdGeomSphere_Retain(UsdGeomSphereRef sphere) {
    if (!sphere) return nullptr;
    sphere->refCount.fetch_add(1, std::memory_order_relaxed);
    return sphere;
}

void UsdGeomSphere_Release(UsdGeomSphereRef sphere) {
    if (!sphere) return;
    if (sphere->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete sphere;
    }
}

bool UsdGeomSphere_IsValid(UsdGeomSphereRef sphere) {
    if (!sphere) return false;
#if USD_USE_FULL
    return sphere->sphere;
#else
    return sphere->valid;
#endif
}

UsdPrimRef UsdGeomSphere_GetPrim(UsdGeomSphereRef sphere) {
    if (!sphere) return nullptr;
#if USD_USE_FULL
    try {
        UsdPrim prim = sphere->sphere.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

double UsdGeomSphere_GetRadius(UsdGeomSphereRef sphere, UsdTimeCode time) {
    if (!sphere) return 1.0;
#if USD_USE_FULL
    double radius = 1.0;
    sphere->sphere.GetRadiusAttr().Get(&radius,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return radius;
#else
    return sphere->radius;
#endif
}

UsdResult UsdGeomSphere_SetRadius(
    UsdGeomSphereRef sphere,
    UsdTimeCode time,
    double radius
) {
    if (!sphere) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        sphere->sphere.GetRadiusAttr().Set(radius,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    sphere->radius = radius;
    return USD_RESULT_SUCCESS;
#endif
}

UsdResult UsdGeomSphere_ComputeExtent(
    double radius,
    float* outExtent
) {
    if (!outExtent) return USD_RESULT_INVALID_ARGUMENT;
    float r = static_cast<float>(radius);
    outExtent[0] = -r;
    outExtent[1] = -r;
    outExtent[2] = -r;
    outExtent[3] = r;
    outExtent[4] = r;
    outExtent[5] = r;
    return USD_RESULT_SUCCESS;
}

// ============================================================================
// MARK: - UsdGeomCube Implementation
// ============================================================================

UsdGeomCubeRef UsdGeomCube_Define(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        UsdGeomCube cube = UsdGeomCube::Define(stage->stage, path->path);
        if (cube) {
            return new UsdGeomCubeOpaque(cube);
        }
        return nullptr;
#else
        auto* wrapper = new UsdGeomCubeOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdGeomCubeRef UsdGeomCube_FromPrim(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdGeomCube cube(prim->prim);
        if (cube) {
            return new UsdGeomCubeOpaque(cube);
        }
        return nullptr;
#else
        auto* wrapper = new UsdGeomCubeOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdGeomCubeRef UsdGeomCube_Retain(UsdGeomCubeRef cube) {
    if (!cube) return nullptr;
    cube->refCount.fetch_add(1, std::memory_order_relaxed);
    return cube;
}

void UsdGeomCube_Release(UsdGeomCubeRef cube) {
    if (!cube) return;
    if (cube->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete cube;
    }
}

bool UsdGeomCube_IsValid(UsdGeomCubeRef cube) {
    if (!cube) return false;
#if USD_USE_FULL
    return cube->cube;
#else
    return cube->valid;
#endif
}

UsdPrimRef UsdGeomCube_GetPrim(UsdGeomCubeRef cube) {
    if (!cube) return nullptr;
#if USD_USE_FULL
    try {
        UsdPrim prim = cube->cube.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

double UsdGeomCube_GetSize(UsdGeomCubeRef cube, UsdTimeCode time) {
    if (!cube) return 2.0;
#if USD_USE_FULL
    double size = 2.0;
    cube->cube.GetSizeAttr().Get(&size,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return size;
#else
    return cube->size;
#endif
}

UsdResult UsdGeomCube_SetSize(
    UsdGeomCubeRef cube,
    UsdTimeCode time,
    double size
) {
    if (!cube) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        cube->cube.GetSizeAttr().Set(size,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    cube->size = size;
    return USD_RESULT_SUCCESS;
#endif
}

UsdResult UsdGeomCube_ComputeExtent(
    double size,
    float* outExtent
) {
    if (!outExtent) return USD_RESULT_INVALID_ARGUMENT;
    float half = static_cast<float>(size / 2.0);
    outExtent[0] = -half;
    outExtent[1] = -half;
    outExtent[2] = -half;
    outExtent[3] = half;
    outExtent[4] = half;
    outExtent[5] = half;
    return USD_RESULT_SUCCESS;
}

// ============================================================================
// MARK: - UsdGeomCylinder Implementation
// ============================================================================

UsdGeomCylinderRef UsdGeomCylinder_Define(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        UsdGeomCylinder cylinder = UsdGeomCylinder::Define(stage->stage, path->path);
        if (cylinder) {
            return new UsdGeomCylinderOpaque(cylinder);
        }
        return nullptr;
#else
        auto* wrapper = new UsdGeomCylinderOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdGeomCylinderRef UsdGeomCylinder_FromPrim(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdGeomCylinder cylinder(prim->prim);
        if (cylinder) {
            return new UsdGeomCylinderOpaque(cylinder);
        }
        return nullptr;
#else
        auto* wrapper = new UsdGeomCylinderOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdGeomCylinderRef UsdGeomCylinder_Retain(UsdGeomCylinderRef cylinder) {
    if (!cylinder) return nullptr;
    cylinder->refCount.fetch_add(1, std::memory_order_relaxed);
    return cylinder;
}

void UsdGeomCylinder_Release(UsdGeomCylinderRef cylinder) {
    if (!cylinder) return;
    if (cylinder->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete cylinder;
    }
}

bool UsdGeomCylinder_IsValid(UsdGeomCylinderRef cylinder) {
    if (!cylinder) return false;
#if USD_USE_FULL
    return cylinder->cylinder;
#else
    return cylinder->valid;
#endif
}

UsdPrimRef UsdGeomCylinder_GetPrim(UsdGeomCylinderRef cylinder) {
    if (!cylinder) return nullptr;
#if USD_USE_FULL
    try {
        UsdPrim prim = cylinder->cylinder.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

double UsdGeomCylinder_GetHeight(UsdGeomCylinderRef cylinder, UsdTimeCode time) {
    if (!cylinder) return 2.0;
#if USD_USE_FULL
    double height = 2.0;
    cylinder->cylinder.GetHeightAttr().Get(&height,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return height;
#else
    return cylinder->height;
#endif
}

UsdResult UsdGeomCylinder_SetHeight(
    UsdGeomCylinderRef cylinder,
    UsdTimeCode time,
    double height
) {
    if (!cylinder) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        cylinder->cylinder.GetHeightAttr().Set(height,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    cylinder->height = height;
    return USD_RESULT_SUCCESS;
#endif
}

double UsdGeomCylinder_GetRadius(UsdGeomCylinderRef cylinder, UsdTimeCode time) {
    if (!cylinder) return 1.0;
#if USD_USE_FULL
    double radius = 1.0;
    cylinder->cylinder.GetRadiusAttr().Get(&radius,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return radius;
#else
    return cylinder->radius;
#endif
}

UsdResult UsdGeomCylinder_SetRadius(
    UsdGeomCylinderRef cylinder,
    UsdTimeCode time,
    double radius
) {
    if (!cylinder) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        cylinder->cylinder.GetRadiusAttr().Set(radius,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    cylinder->radius = radius;
    return USD_RESULT_SUCCESS;
#endif
}

UsdGeomAxis UsdGeomCylinder_GetAxis(UsdGeomCylinderRef cylinder) {
    if (!cylinder) return USDGEOM_AXIS_Z;
#if USD_USE_FULL
    TfToken axis;
    cylinder->cylinder.GetAxisAttr().Get(&axis);
    if (axis == UsdGeomTokens->x) return USDGEOM_AXIS_X;
    if (axis == UsdGeomTokens->y) return USDGEOM_AXIS_Y;
    return USDGEOM_AXIS_Z;
#else
    return static_cast<UsdGeomAxis>(cylinder->axis);
#endif
}

UsdResult UsdGeomCylinder_SetAxis(UsdGeomCylinderRef cylinder, UsdGeomAxis axis) {
    if (!cylinder) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        TfToken axisToken;
        switch (axis) {
            case USDGEOM_AXIS_X: axisToken = UsdGeomTokens->x; break;
            case USDGEOM_AXIS_Y: axisToken = UsdGeomTokens->y; break;
            default: axisToken = UsdGeomTokens->z; break;
        }
        cylinder->cylinder.GetAxisAttr().Set(axisToken);
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    cylinder->axis = static_cast<int>(axis);
    return USD_RESULT_SUCCESS;
#endif
}

// ============================================================================
// MARK: - UsdGeomCone Implementation
// ============================================================================

UsdGeomConeRef UsdGeomCone_Define(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        UsdGeomCone cone = UsdGeomCone::Define(stage->stage, path->path);
        if (cone) {
            return new UsdGeomConeOpaque(cone);
        }
        return nullptr;
#else
        auto* wrapper = new UsdGeomConeOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdGeomConeRef UsdGeomCone_FromPrim(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdGeomCone cone(prim->prim);
        if (cone) {
            return new UsdGeomConeOpaque(cone);
        }
        return nullptr;
#else
        auto* wrapper = new UsdGeomConeOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdGeomConeRef UsdGeomCone_Retain(UsdGeomConeRef cone) {
    if (!cone) return nullptr;
    cone->refCount.fetch_add(1, std::memory_order_relaxed);
    return cone;
}

void UsdGeomCone_Release(UsdGeomConeRef cone) {
    if (!cone) return;
    if (cone->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete cone;
    }
}

bool UsdGeomCone_IsValid(UsdGeomConeRef cone) {
    if (!cone) return false;
#if USD_USE_FULL
    return cone->cone;
#else
    return cone->valid;
#endif
}

UsdPrimRef UsdGeomCone_GetPrim(UsdGeomConeRef cone) {
    if (!cone) return nullptr;
#if USD_USE_FULL
    try {
        UsdPrim prim = cone->cone.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

double UsdGeomCone_GetHeight(UsdGeomConeRef cone, UsdTimeCode time) {
    if (!cone) return 2.0;
#if USD_USE_FULL
    double height = 2.0;
    cone->cone.GetHeightAttr().Get(&height,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return height;
#else
    return cone->height;
#endif
}

UsdResult UsdGeomCone_SetHeight(
    UsdGeomConeRef cone,
    UsdTimeCode time,
    double height
) {
    if (!cone) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        cone->cone.GetHeightAttr().Set(height,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    cone->height = height;
    return USD_RESULT_SUCCESS;
#endif
}

double UsdGeomCone_GetRadius(UsdGeomConeRef cone, UsdTimeCode time) {
    if (!cone) return 1.0;
#if USD_USE_FULL
    double radius = 1.0;
    cone->cone.GetRadiusAttr().Get(&radius,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return radius;
#else
    return cone->radius;
#endif
}

UsdResult UsdGeomCone_SetRadius(
    UsdGeomConeRef cone,
    UsdTimeCode time,
    double radius
) {
    if (!cone) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        cone->cone.GetRadiusAttr().Set(radius,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    cone->radius = radius;
    return USD_RESULT_SUCCESS;
#endif
}

UsdGeomAxis UsdGeomCone_GetAxis(UsdGeomConeRef cone) {
    if (!cone) return USDGEOM_AXIS_Z;
#if USD_USE_FULL
    TfToken axis;
    cone->cone.GetAxisAttr().Get(&axis);
    if (axis == UsdGeomTokens->x) return USDGEOM_AXIS_X;
    if (axis == UsdGeomTokens->y) return USDGEOM_AXIS_Y;
    return USDGEOM_AXIS_Z;
#else
    return static_cast<UsdGeomAxis>(cone->axis);
#endif
}

UsdResult UsdGeomCone_SetAxis(UsdGeomConeRef cone, UsdGeomAxis axis) {
    if (!cone) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        TfToken axisToken;
        switch (axis) {
            case USDGEOM_AXIS_X: axisToken = UsdGeomTokens->x; break;
            case USDGEOM_AXIS_Y: axisToken = UsdGeomTokens->y; break;
            default: axisToken = UsdGeomTokens->z; break;
        }
        cone->cone.GetAxisAttr().Set(axisToken);
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    cone->axis = static_cast<int>(axis);
    return USD_RESULT_SUCCESS;
#endif
}

// ============================================================================
// MARK: - UsdGeomCapsule Implementation
// ============================================================================

UsdGeomCapsuleRef UsdGeomCapsule_Define(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        UsdGeomCapsule capsule = UsdGeomCapsule::Define(stage->stage, path->path);
        if (capsule) {
            return new UsdGeomCapsuleOpaque(capsule);
        }
        return nullptr;
#else
        auto* wrapper = new UsdGeomCapsuleOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdGeomCapsuleRef UsdGeomCapsule_FromPrim(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdGeomCapsule capsule(prim->prim);
        if (capsule) {
            return new UsdGeomCapsuleOpaque(capsule);
        }
        return nullptr;
#else
        auto* wrapper = new UsdGeomCapsuleOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdGeomCapsuleRef UsdGeomCapsule_Retain(UsdGeomCapsuleRef capsule) {
    if (!capsule) return nullptr;
    capsule->refCount.fetch_add(1, std::memory_order_relaxed);
    return capsule;
}

void UsdGeomCapsule_Release(UsdGeomCapsuleRef capsule) {
    if (!capsule) return;
    if (capsule->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete capsule;
    }
}

bool UsdGeomCapsule_IsValid(UsdGeomCapsuleRef capsule) {
    if (!capsule) return false;
#if USD_USE_FULL
    return capsule->capsule;
#else
    return capsule->valid;
#endif
}

UsdPrimRef UsdGeomCapsule_GetPrim(UsdGeomCapsuleRef capsule) {
    if (!capsule) return nullptr;
#if USD_USE_FULL
    try {
        UsdPrim prim = capsule->capsule.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

double UsdGeomCapsule_GetHeight(UsdGeomCapsuleRef capsule, UsdTimeCode time) {
    if (!capsule) return 2.0;
#if USD_USE_FULL
    double height = 2.0;
    capsule->capsule.GetHeightAttr().Get(&height,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return height;
#else
    return capsule->height;
#endif
}

UsdResult UsdGeomCapsule_SetHeight(
    UsdGeomCapsuleRef capsule,
    UsdTimeCode time,
    double height
) {
    if (!capsule) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        capsule->capsule.GetHeightAttr().Set(height,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    capsule->height = height;
    return USD_RESULT_SUCCESS;
#endif
}

double UsdGeomCapsule_GetRadius(UsdGeomCapsuleRef capsule, UsdTimeCode time) {
    if (!capsule) return 0.5;
#if USD_USE_FULL
    double radius = 0.5;
    capsule->capsule.GetRadiusAttr().Get(&radius,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return radius;
#else
    return capsule->radius;
#endif
}

UsdResult UsdGeomCapsule_SetRadius(
    UsdGeomCapsuleRef capsule,
    UsdTimeCode time,
    double radius
) {
    if (!capsule) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        capsule->capsule.GetRadiusAttr().Set(radius,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    capsule->radius = radius;
    return USD_RESULT_SUCCESS;
#endif
}

UsdGeomAxis UsdGeomCapsule_GetAxis(UsdGeomCapsuleRef capsule) {
    if (!capsule) return USDGEOM_AXIS_Z;
#if USD_USE_FULL
    TfToken axis;
    capsule->capsule.GetAxisAttr().Get(&axis);
    if (axis == UsdGeomTokens->x) return USDGEOM_AXIS_X;
    if (axis == UsdGeomTokens->y) return USDGEOM_AXIS_Y;
    return USDGEOM_AXIS_Z;
#else
    return static_cast<UsdGeomAxis>(capsule->axis);
#endif
}

UsdResult UsdGeomCapsule_SetAxis(UsdGeomCapsuleRef capsule, UsdGeomAxis axis) {
    if (!capsule) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        TfToken axisToken;
        switch (axis) {
            case USDGEOM_AXIS_X: axisToken = UsdGeomTokens->x; break;
            case USDGEOM_AXIS_Y: axisToken = UsdGeomTokens->y; break;
            default: axisToken = UsdGeomTokens->z; break;
        }
        capsule->capsule.GetAxisAttr().Set(axisToken);
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    capsule->axis = static_cast<int>(axis);
    return USD_RESULT_SUCCESS;
#endif
}

// ============================================================================
// MARK: - UsdGeomCamera Implementation
// ============================================================================

UsdGeomCameraRef UsdGeomCamera_Define(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        UsdGeomCamera camera = UsdGeomCamera::Define(stage->stage, path->path);
        if (camera) {
            return new UsdGeomCameraOpaque(camera);
        }
        return nullptr;
#else
        auto* wrapper = new UsdGeomCameraOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdGeomCameraRef UsdGeomCamera_FromPrim(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdGeomCamera camera(prim->prim);
        if (camera) {
            return new UsdGeomCameraOpaque(camera);
        }
        return nullptr;
#else
        auto* wrapper = new UsdGeomCameraOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdGeomCameraRef UsdGeomCamera_Retain(UsdGeomCameraRef camera) {
    if (!camera) return nullptr;
    camera->refCount.fetch_add(1, std::memory_order_relaxed);
    return camera;
}

void UsdGeomCamera_Release(UsdGeomCameraRef camera) {
    if (!camera) return;
    if (camera->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete camera;
    }
}

bool UsdGeomCamera_IsValid(UsdGeomCameraRef camera) {
    if (!camera) return false;
#if USD_USE_FULL
    return camera->camera;
#else
    return camera->valid;
#endif
}

UsdPrimRef UsdGeomCamera_GetPrim(UsdGeomCameraRef camera) {
    if (!camera) return nullptr;
#if USD_USE_FULL
    try {
        UsdPrim prim = camera->camera.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

float UsdGeomCamera_GetFocalLength(UsdGeomCameraRef camera, UsdTimeCode time) {
    if (!camera) return 50.0f;
#if USD_USE_FULL
    float focalLength = 50.0f;
    camera->camera.GetFocalLengthAttr().Get(&focalLength,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return focalLength;
#else
    return camera->focalLength;
#endif
}

UsdResult UsdGeomCamera_SetFocalLength(
    UsdGeomCameraRef camera,
    UsdTimeCode time,
    float focalLength
) {
    if (!camera) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        camera->camera.GetFocalLengthAttr().Set(focalLength,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    camera->focalLength = focalLength;
    return USD_RESULT_SUCCESS;
#endif
}

float UsdGeomCamera_GetHorizontalAperture(UsdGeomCameraRef camera, UsdTimeCode time) {
    if (!camera) return 36.0f;
#if USD_USE_FULL
    float aperture = 36.0f;
    camera->camera.GetHorizontalApertureAttr().Get(&aperture,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return aperture;
#else
    return camera->horizontalAperture;
#endif
}

UsdResult UsdGeomCamera_SetHorizontalAperture(
    UsdGeomCameraRef camera,
    UsdTimeCode time,
    float aperture
) {
    if (!camera) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        camera->camera.GetHorizontalApertureAttr().Set(aperture,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    camera->horizontalAperture = aperture;
    return USD_RESULT_SUCCESS;
#endif
}

float UsdGeomCamera_GetVerticalAperture(UsdGeomCameraRef camera, UsdTimeCode time) {
    if (!camera) return 24.0f;
#if USD_USE_FULL
    float aperture = 24.0f;
    camera->camera.GetVerticalApertureAttr().Get(&aperture,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return aperture;
#else
    return camera->verticalAperture;
#endif
}

UsdResult UsdGeomCamera_SetVerticalAperture(
    UsdGeomCameraRef camera,
    UsdTimeCode time,
    float aperture
) {
    if (!camera) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        camera->camera.GetVerticalApertureAttr().Set(aperture,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    camera->verticalAperture = aperture;
    return USD_RESULT_SUCCESS;
#endif
}

UsdResult UsdGeomCamera_GetClippingRange(
    UsdGeomCameraRef camera,
    UsdTimeCode time,
    float* nearClip,
    float* farClip
) {
    if (!camera || !nearClip || !farClip) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        GfVec2f range(0.1f, 10000.0f);
        camera->camera.GetClippingRangeAttr().Get(&range,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        *nearClip = range[0];
        *farClip = range[1];
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    *nearClip = camera->nearClip;
    *farClip = camera->farClip;
    return USD_RESULT_SUCCESS;
#endif
}

UsdResult UsdGeomCamera_SetClippingRange(
    UsdGeomCameraRef camera,
    UsdTimeCode time,
    float nearClip,
    float farClip
) {
    if (!camera) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        GfVec2f range(nearClip, farClip);
        camera->camera.GetClippingRangeAttr().Set(range,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    camera->nearClip = nearClip;
    camera->farClip = farClip;
    return USD_RESULT_SUCCESS;
#endif
}

float UsdGeomCamera_GetFocusDistance(UsdGeomCameraRef camera, UsdTimeCode time) {
    if (!camera) return 0.0f;
#if USD_USE_FULL
    float focusDistance = 0.0f;
    camera->camera.GetFocusDistanceAttr().Get(&focusDistance,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return focusDistance;
#else
    return 0.0f;
#endif
}

UsdResult UsdGeomCamera_SetFocusDistance(
    UsdGeomCameraRef camera,
    UsdTimeCode time,
    float focusDistance
) {
    if (!camera) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        camera->camera.GetFocusDistanceAttr().Set(focusDistance,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    return USD_RESULT_SUCCESS;
#endif
}

float UsdGeomCamera_GetFStop(UsdGeomCameraRef camera, UsdTimeCode time) {
    if (!camera) return 0.0f;
#if USD_USE_FULL
    float fStop = 0.0f;
    camera->camera.GetFStopAttr().Get(&fStop,
        time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    return fStop;
#else
    return 0.0f;
#endif
}

UsdResult UsdGeomCamera_SetFStop(
    UsdGeomCameraRef camera,
    UsdTimeCode time,
    float fStop
) {
    if (!camera) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        camera->camera.GetFStopAttr().Set(fStop,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    return USD_RESULT_SUCCESS;
#endif
}

// ============================================================================
// MARK: - UsdGeomPoints Implementation
// ============================================================================

UsdGeomPointsRef UsdGeomPoints_Define(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        UsdGeomPoints points = UsdGeomPoints::Define(stage->stage, path->path);
        if (points) {
            return new UsdGeomPointsOpaque(points);
        }
        return nullptr;
#else
        auto* wrapper = new UsdGeomPointsOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdGeomPointsRef UsdGeomPoints_FromPrim(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdGeomPoints points(prim->prim);
        if (points) {
            return new UsdGeomPointsOpaque(points);
        }
        return nullptr;
#else
        auto* wrapper = new UsdGeomPointsOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdGeomPointsRef UsdGeomPoints_Retain(UsdGeomPointsRef points) {
    if (!points) return nullptr;
    points->refCount.fetch_add(1, std::memory_order_relaxed);
    return points;
}

void UsdGeomPoints_Release(UsdGeomPointsRef points) {
    if (!points) return;
    if (points->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete points;
    }
}

bool UsdGeomPoints_IsValid(UsdGeomPointsRef points) {
    if (!points) return false;
#if USD_USE_FULL
    return points->points;
#else
    return points->valid;
#endif
}

UsdPrimRef UsdGeomPoints_GetPrim(UsdGeomPointsRef points) {
    if (!points) return nullptr;
#if USD_USE_FULL
    try {
        UsdPrim prim = points->points.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

size_t UsdGeomPoints_GetPointCount(UsdGeomPointsRef points, UsdTimeCode time) {
    if (!points) return 0;
#if USD_USE_FULL
    try {
        VtVec3fArray pointsArray;
        points->points.GetPointsAttr().Get(&pointsArray,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return pointsArray.size();
    } CATCH_AND_RETURN(0)
#else
    return 0;
#endif
}

size_t UsdGeomPoints_GetPoints(UsdGeomPointsRef points, UsdTimeCode time, float* outPoints, size_t maxCount) {
    if (!points || !outPoints) return 0;
#if USD_USE_FULL
    try {
        VtVec3fArray pointsArray;
        points->points.GetPointsAttr().Get(&pointsArray,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        size_t count = std::min(pointsArray.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outPoints[i * 3 + 0] = pointsArray[i][0];
            outPoints[i * 3 + 1] = pointsArray[i][1];
            outPoints[i * 3 + 2] = pointsArray[i][2];
        }
        return count;
    } CATCH_AND_RETURN(0)
#else
    return 0;
#endif
}

UsdResult UsdGeomPoints_SetPoints(UsdGeomPointsRef points, UsdTimeCode time, const float* pointsData, size_t count) {
    if (!points || !pointsData) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        VtVec3fArray vtPoints(count);
        for (size_t i = 0; i < count; ++i) {
            vtPoints[i] = GfVec3f(pointsData[i * 3 + 0], pointsData[i * 3 + 1], pointsData[i * 3 + 2]);
        }
        points->points.GetPointsAttr().Set(vtPoints,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    return USD_RESULT_SUCCESS;
#endif
}

size_t UsdGeomPoints_GetWidthsCount(UsdGeomPointsRef points, UsdTimeCode time) {
    if (!points) return 0;
#if USD_USE_FULL
    try {
        VtFloatArray widths;
        points->points.GetWidthsAttr().Get(&widths,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return widths.size();
    } CATCH_AND_RETURN(0)
#else
    return 0;
#endif
}

size_t UsdGeomPoints_GetWidths(UsdGeomPointsRef points, UsdTimeCode time, float* outWidths, size_t maxCount) {
    if (!points || !outWidths) return 0;
#if USD_USE_FULL
    try {
        VtFloatArray widths;
        points->points.GetWidthsAttr().Get(&widths,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        size_t count = std::min(widths.size(), maxCount);
        std::memcpy(outWidths, widths.data(), count * sizeof(float));
        return count;
    } CATCH_AND_RETURN(0)
#else
    return 0;
#endif
}

UsdResult UsdGeomPoints_SetWidths(UsdGeomPointsRef points, UsdTimeCode time, const float* widths, size_t count) {
    if (!points || !widths) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        VtFloatArray vtWidths(count);
        std::memcpy(vtWidths.data(), widths, count * sizeof(float));
        points->points.GetWidthsAttr().Set(vtWidths,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    return USD_RESULT_SUCCESS;
#endif
}

size_t UsdGeomPoints_GetIdsCount(UsdGeomPointsRef points, UsdTimeCode time) {
    if (!points) return 0;
#if USD_USE_FULL
    try {
        VtInt64Array ids;
        points->points.GetIdsAttr().Get(&ids,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return ids.size();
    } CATCH_AND_RETURN(0)
#else
    return 0;
#endif
}

size_t UsdGeomPoints_GetIds(UsdGeomPointsRef points, UsdTimeCode time, int64_t* outIds, size_t maxCount) {
    if (!points || !outIds) return 0;
#if USD_USE_FULL
    try {
        VtInt64Array ids;
        points->points.GetIdsAttr().Get(&ids,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        size_t count = std::min(ids.size(), maxCount);
        std::memcpy(outIds, ids.data(), count * sizeof(int64_t));
        return count;
    } CATCH_AND_RETURN(0)
#else
    return 0;
#endif
}

UsdResult UsdGeomPoints_SetIds(UsdGeomPointsRef points, UsdTimeCode time, const int64_t* ids, size_t count) {
    if (!points || !ids) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        VtInt64Array vtIds(count);
        std::memcpy(vtIds.data(), ids, count * sizeof(int64_t));
        points->points.GetIdsAttr().Set(vtIds,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    return USD_RESULT_SUCCESS;
#endif
}

// ============================================================================
// MARK: - UsdGeomBasisCurves Implementation
// ============================================================================

UsdGeomBasisCurvesRef UsdGeomBasisCurves_Define(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        UsdGeomBasisCurves curves = UsdGeomBasisCurves::Define(stage->stage, path->path);
        if (curves) {
            return new UsdGeomBasisCurvesOpaque(curves);
        }
        return nullptr;
#else
        auto* wrapper = new UsdGeomBasisCurvesOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdGeomBasisCurvesRef UsdGeomBasisCurves_FromPrim(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdGeomBasisCurves curves(prim->prim);
        if (curves) {
            return new UsdGeomBasisCurvesOpaque(curves);
        }
        return nullptr;
#else
        auto* wrapper = new UsdGeomBasisCurvesOpaque();
        wrapper->valid = true;
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdGeomBasisCurvesRef UsdGeomBasisCurves_Retain(UsdGeomBasisCurvesRef curves) {
    if (!curves) return nullptr;
    curves->refCount.fetch_add(1, std::memory_order_relaxed);
    return curves;
}

void UsdGeomBasisCurves_Release(UsdGeomBasisCurvesRef curves) {
    if (!curves) return;
    if (curves->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete curves;
    }
}

bool UsdGeomBasisCurves_IsValid(UsdGeomBasisCurvesRef curves) {
    if (!curves) return false;
#if USD_USE_FULL
    return curves->curves;
#else
    return curves->valid;
#endif
}

UsdPrimRef UsdGeomBasisCurves_GetPrim(UsdGeomBasisCurvesRef curves) {
    if (!curves) return nullptr;
#if USD_USE_FULL
    try {
        UsdPrim prim = curves->curves.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

UsdGeomCurveType UsdGeomBasisCurves_GetType(UsdGeomBasisCurvesRef curves) {
    if (!curves) return USDGEOM_CURVE_TYPE_LINEAR;
#if USD_USE_FULL
    try {
        TfToken type;
        curves->curves.GetTypeAttr().Get(&type);
        if (type == UsdGeomTokens->cubic) return USDGEOM_CURVE_TYPE_CUBIC;
        return USDGEOM_CURVE_TYPE_LINEAR;
    } CATCH_AND_RETURN(USDGEOM_CURVE_TYPE_LINEAR)
#else
    return USDGEOM_CURVE_TYPE_LINEAR;
#endif
}

UsdResult UsdGeomBasisCurves_SetType(UsdGeomBasisCurvesRef curves, UsdGeomCurveType type) {
    if (!curves) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        TfToken typeToken = (type == USDGEOM_CURVE_TYPE_CUBIC)
            ? UsdGeomTokens->cubic : UsdGeomTokens->linear;
        curves->curves.GetTypeAttr().Set(typeToken);
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    return USD_RESULT_SUCCESS;
#endif
}

UsdGeomBasis UsdGeomBasisCurves_GetBasis(UsdGeomBasisCurvesRef curves) {
    if (!curves) return USDGEOM_BASIS_BEZIER;
#if USD_USE_FULL
    try {
        TfToken basis;
        curves->curves.GetBasisAttr().Get(&basis);
        if (basis == UsdGeomTokens->bspline) return USDGEOM_BASIS_BSPLINE;
        if (basis == UsdGeomTokens->catmullRom) return USDGEOM_BASIS_CATMULL_ROM;
        return USDGEOM_BASIS_BEZIER;
    } CATCH_AND_RETURN(USDGEOM_BASIS_BEZIER)
#else
    return USDGEOM_BASIS_BEZIER;
#endif
}

UsdResult UsdGeomBasisCurves_SetBasis(UsdGeomBasisCurvesRef curves, UsdGeomBasis basis) {
    if (!curves) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        TfToken basisToken;
        switch (basis) {
            case USDGEOM_BASIS_BSPLINE: basisToken = UsdGeomTokens->bspline; break;
            case USDGEOM_BASIS_CATMULL_ROM: basisToken = UsdGeomTokens->catmullRom; break;
            default: basisToken = UsdGeomTokens->bezier; break;
        }
        curves->curves.GetBasisAttr().Set(basisToken);
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    return USD_RESULT_SUCCESS;
#endif
}

UsdGeomWrap UsdGeomBasisCurves_GetWrap(UsdGeomBasisCurvesRef curves) {
    if (!curves) return USDGEOM_WRAP_NONPERIODIC;
#if USD_USE_FULL
    try {
        TfToken wrap;
        curves->curves.GetWrapAttr().Get(&wrap);
        if (wrap == UsdGeomTokens->periodic) return USDGEOM_WRAP_PERIODIC;
        if (wrap == UsdGeomTokens->pinned) return USDGEOM_WRAP_PINNED;
        return USDGEOM_WRAP_NONPERIODIC;
    } CATCH_AND_RETURN(USDGEOM_WRAP_NONPERIODIC)
#else
    return USDGEOM_WRAP_NONPERIODIC;
#endif
}

UsdResult UsdGeomBasisCurves_SetWrap(UsdGeomBasisCurvesRef curves, UsdGeomWrap wrap) {
    if (!curves) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        TfToken wrapToken;
        switch (wrap) {
            case USDGEOM_WRAP_PERIODIC: wrapToken = UsdGeomTokens->periodic; break;
            case USDGEOM_WRAP_PINNED: wrapToken = UsdGeomTokens->pinned; break;
            default: wrapToken = UsdGeomTokens->nonperiodic; break;
        }
        curves->curves.GetWrapAttr().Set(wrapToken);
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    return USD_RESULT_SUCCESS;
#endif
}

size_t UsdGeomBasisCurves_GetCurveVertexCountsCount(UsdGeomBasisCurvesRef curves, UsdTimeCode time) {
    if (!curves) return 0;
#if USD_USE_FULL
    try {
        VtIntArray counts;
        curves->curves.GetCurveVertexCountsAttr().Get(&counts,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return counts.size();
    } CATCH_AND_RETURN(0)
#else
    return 0;
#endif
}

size_t UsdGeomBasisCurves_GetCurveVertexCounts(UsdGeomBasisCurvesRef curves, UsdTimeCode time, int* outCounts, size_t maxCount) {
    if (!curves || !outCounts) return 0;
#if USD_USE_FULL
    try {
        VtIntArray counts;
        curves->curves.GetCurveVertexCountsAttr().Get(&counts,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        size_t count = std::min(counts.size(), maxCount);
        std::memcpy(outCounts, counts.data(), count * sizeof(int));
        return count;
    } CATCH_AND_RETURN(0)
#else
    return 0;
#endif
}

UsdResult UsdGeomBasisCurves_SetCurveVertexCounts(UsdGeomBasisCurvesRef curves, UsdTimeCode time, const int* counts, size_t count) {
    if (!curves || !counts) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        VtIntArray vtCounts(count);
        std::memcpy(vtCounts.data(), counts, count * sizeof(int));
        curves->curves.GetCurveVertexCountsAttr().Set(vtCounts,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    return USD_RESULT_SUCCESS;
#endif
}

size_t UsdGeomBasisCurves_GetPointCount(UsdGeomBasisCurvesRef curves, UsdTimeCode time) {
    if (!curves) return 0;
#if USD_USE_FULL
    try {
        VtVec3fArray points;
        curves->curves.GetPointsAttr().Get(&points,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return points.size();
    } CATCH_AND_RETURN(0)
#else
    return 0;
#endif
}

size_t UsdGeomBasisCurves_GetPoints(UsdGeomBasisCurvesRef curves, UsdTimeCode time, float* outPoints, size_t maxCount) {
    if (!curves || !outPoints) return 0;
#if USD_USE_FULL
    try {
        VtVec3fArray points;
        curves->curves.GetPointsAttr().Get(&points,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        size_t count = std::min(points.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outPoints[i * 3 + 0] = points[i][0];
            outPoints[i * 3 + 1] = points[i][1];
            outPoints[i * 3 + 2] = points[i][2];
        }
        return count;
    } CATCH_AND_RETURN(0)
#else
    return 0;
#endif
}

UsdResult UsdGeomBasisCurves_SetPoints(UsdGeomBasisCurvesRef curves, UsdTimeCode time, const float* points, size_t count) {
    if (!curves || !points) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        VtVec3fArray vtPoints(count);
        for (size_t i = 0; i < count; ++i) {
            vtPoints[i] = GfVec3f(points[i * 3 + 0], points[i * 3 + 1], points[i * 3 + 2]);
        }
        curves->curves.GetPointsAttr().Set(vtPoints,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    return USD_RESULT_SUCCESS;
#endif
}

size_t UsdGeomBasisCurves_GetWidthsCount(UsdGeomBasisCurvesRef curves, UsdTimeCode time) {
    if (!curves) return 0;
#if USD_USE_FULL
    try {
        VtFloatArray widths;
        curves->curves.GetWidthsAttr().Get(&widths,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return widths.size();
    } CATCH_AND_RETURN(0)
#else
    return 0;
#endif
}

size_t UsdGeomBasisCurves_GetWidths(UsdGeomBasisCurvesRef curves, UsdTimeCode time, float* outWidths, size_t maxCount) {
    if (!curves || !outWidths) return 0;
#if USD_USE_FULL
    try {
        VtFloatArray widths;
        curves->curves.GetWidthsAttr().Get(&widths,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        size_t count = std::min(widths.size(), maxCount);
        std::memcpy(outWidths, widths.data(), count * sizeof(float));
        return count;
    } CATCH_AND_RETURN(0)
#else
    return 0;
#endif
}

UsdResult UsdGeomBasisCurves_SetWidths(UsdGeomBasisCurvesRef curves, UsdTimeCode time, const float* widths, size_t count) {
    if (!curves || !widths) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        VtFloatArray vtWidths(count);
        std::memcpy(vtWidths.data(), widths, count * sizeof(float));
        curves->curves.GetWidthsAttr().Set(vtWidths,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    return USD_RESULT_SUCCESS;
#endif
}

// ============================================================================
// MARK: - UsdGeomPrimvar Implementation
// ============================================================================

UsdGeomPrimvarRef UsdGeomPrimvar_FromAttribute(UsdAttributeRef attr) {
    if (!attr) return nullptr;
#if USD_USE_FULL
    try {
        UsdGeomPrimvar primvar(attr->attr);
        if (primvar) {
            return new UsdGeomPrimvarOpaque(primvar);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

UsdGeomPrimvarRef UsdGeomPrimvar_Retain(UsdGeomPrimvarRef primvar) {
    if (!primvar) return nullptr;
    primvar->refCount.fetch_add(1, std::memory_order_relaxed);
    return primvar;
}

void UsdGeomPrimvar_Release(UsdGeomPrimvarRef primvar) {
    if (!primvar) return;
    if (primvar->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete primvar;
    }
}

bool UsdGeomPrimvar_IsValid(UsdGeomPrimvarRef primvar) {
    if (!primvar) return false;
#if USD_USE_FULL
    return primvar->primvar;
#else
    return primvar->valid;
#endif
}

bool UsdGeomPrimvar_HasValue(UsdGeomPrimvarRef primvar) {
    if (!primvar) return false;
#if USD_USE_FULL
    return primvar->primvar.HasValue();
#else
    return false;
#endif
}

bool UsdGeomPrimvar_HasAuthoredValue(UsdGeomPrimvarRef primvar) {
    if (!primvar) return false;
#if USD_USE_FULL
    return primvar->primvar.HasAuthoredValue();
#else
    return false;
#endif
}

TfTokenRef UsdGeomPrimvar_GetName(UsdGeomPrimvarRef primvar) {
    if (!primvar) return nullptr;
#if USD_USE_FULL
    try {
        TfToken name = primvar->primvar.GetName();
        return new TfTokenOpaque(name);
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

TfTokenRef UsdGeomPrimvar_GetBaseName(UsdGeomPrimvarRef primvar) {
    if (!primvar) return nullptr;
#if USD_USE_FULL
    try {
        TfToken baseName = primvar->primvar.GetPrimvarName();
        return new TfTokenOpaque(baseName);
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

UsdGeomInterpolation UsdGeomPrimvar_GetInterpolation(UsdGeomPrimvarRef primvar) {
    if (!primvar) return USDGEOM_INTERPOLATION_CONSTANT;
#if USD_USE_FULL
    try {
        TfToken interp = primvar->primvar.GetInterpolation();
        if (interp == UsdGeomTokens->constant) return USDGEOM_INTERPOLATION_CONSTANT;
        if (interp == UsdGeomTokens->uniform) return USDGEOM_INTERPOLATION_UNIFORM;
        if (interp == UsdGeomTokens->varying) return USDGEOM_INTERPOLATION_VARYING;
        if (interp == UsdGeomTokens->vertex) return USDGEOM_INTERPOLATION_VERTEX;
        if (interp == UsdGeomTokens->faceVarying) return USDGEOM_INTERPOLATION_FACEVARYING;
        return USDGEOM_INTERPOLATION_CONSTANT;
    } CATCH_AND_RETURN(USDGEOM_INTERPOLATION_CONSTANT)
#else
    return USDGEOM_INTERPOLATION_CONSTANT;
#endif
}

UsdResult UsdGeomPrimvar_SetInterpolation(UsdGeomPrimvarRef primvar, UsdGeomInterpolation interpolation) {
    if (!primvar) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        TfToken interpToken;
        switch (interpolation) {
            case USDGEOM_INTERPOLATION_CONSTANT: interpToken = UsdGeomTokens->constant; break;
            case USDGEOM_INTERPOLATION_UNIFORM: interpToken = UsdGeomTokens->uniform; break;
            case USDGEOM_INTERPOLATION_VARYING: interpToken = UsdGeomTokens->varying; break;
            case USDGEOM_INTERPOLATION_VERTEX: interpToken = UsdGeomTokens->vertex; break;
            case USDGEOM_INTERPOLATION_FACEVARYING: interpToken = UsdGeomTokens->faceVarying; break;
            default: interpToken = UsdGeomTokens->constant; break;
        }
        primvar->primvar.SetInterpolation(interpToken);
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    return USD_RESULT_SUCCESS;
#endif
}

int UsdGeomPrimvar_GetElementSize(UsdGeomPrimvarRef primvar) {
    if (!primvar) return 1;
#if USD_USE_FULL
    return primvar->primvar.GetElementSize();
#else
    return 1;
#endif
}

UsdResult UsdGeomPrimvar_SetElementSize(UsdGeomPrimvarRef primvar, int elementSize) {
    if (!primvar) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        primvar->primvar.SetElementSize(elementSize);
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    return USD_RESULT_SUCCESS;
#endif
}

bool UsdGeomPrimvar_IsIndexed(UsdGeomPrimvarRef primvar) {
    if (!primvar) return false;
#if USD_USE_FULL
    return primvar->primvar.IsIndexed();
#else
    return false;
#endif
}

UsdAttributeRef UsdGeomPrimvar_GetAttr(UsdGeomPrimvarRef primvar) {
    if (!primvar) return nullptr;
#if USD_USE_FULL
    try {
        UsdAttribute attr = primvar->primvar.GetAttr();
        if (attr) {
            return new UsdAttributeOpaque(attr);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

// ============================================================================
// MARK: - UsdGeomPrimvarsAPI Implementation
// ============================================================================

UsdGeomPrimvarsAPIRef UsdGeomPrimvarsAPI_Get(UsdPrimRef prim) {
    if (!prim) return nullptr;
#if USD_USE_FULL
    try {
        UsdGeomPrimvarsAPI api(prim->prim);
        return new UsdGeomPrimvarsAPIOpaque(api);
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

UsdGeomPrimvarsAPIRef UsdGeomPrimvarsAPI_Retain(UsdGeomPrimvarsAPIRef api) {
    if (!api) return nullptr;
    api->refCount.fetch_add(1, std::memory_order_relaxed);
    return api;
}

void UsdGeomPrimvarsAPI_Release(UsdGeomPrimvarsAPIRef api) {
    if (!api) return;
    if (api->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete api;
    }
}

bool UsdGeomPrimvarsAPI_IsValid(UsdGeomPrimvarsAPIRef api) {
    if (!api) return false;
#if USD_USE_FULL
    return api->api;
#else
    return api->valid;
#endif
}

UsdGeomPrimvarRef UsdGeomPrimvarsAPI_CreatePrimvar(
    UsdGeomPrimvarsAPIRef api,
    TfTokenRef name,
    TfTokenRef typeName,
    UsdGeomInterpolation interpolation
) {
    if (!api || !name || !typeName) return nullptr;
#if USD_USE_FULL
    try {
        TfToken interpToken;
        switch (interpolation) {
            case USDGEOM_INTERPOLATION_CONSTANT: interpToken = UsdGeomTokens->constant; break;
            case USDGEOM_INTERPOLATION_UNIFORM: interpToken = UsdGeomTokens->uniform; break;
            case USDGEOM_INTERPOLATION_VARYING: interpToken = UsdGeomTokens->varying; break;
            case USDGEOM_INTERPOLATION_VERTEX: interpToken = UsdGeomTokens->vertex; break;
            case USDGEOM_INTERPOLATION_FACEVARYING: interpToken = UsdGeomTokens->faceVarying; break;
            default: interpToken = UsdGeomTokens->constant; break;
        }
        SdfValueTypeName sdfTypeName = SdfSchema::GetInstance().FindType(typeName->token);
        UsdGeomPrimvar primvar = api->api.CreatePrimvar(name->token, sdfTypeName, interpToken);
        if (primvar) {
            return new UsdGeomPrimvarOpaque(primvar);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

UsdGeomPrimvarRef UsdGeomPrimvarsAPI_CreatePrimvarWithElementSize(
    UsdGeomPrimvarsAPIRef api,
    TfTokenRef name,
    TfTokenRef typeName,
    UsdGeomInterpolation interpolation,
    int elementSize
) {
    if (!api || !name || !typeName) return nullptr;
#if USD_USE_FULL
    try {
        TfToken interpToken;
        switch (interpolation) {
            case USDGEOM_INTERPOLATION_CONSTANT: interpToken = UsdGeomTokens->constant; break;
            case USDGEOM_INTERPOLATION_UNIFORM: interpToken = UsdGeomTokens->uniform; break;
            case USDGEOM_INTERPOLATION_VARYING: interpToken = UsdGeomTokens->varying; break;
            case USDGEOM_INTERPOLATION_VERTEX: interpToken = UsdGeomTokens->vertex; break;
            case USDGEOM_INTERPOLATION_FACEVARYING: interpToken = UsdGeomTokens->faceVarying; break;
            default: interpToken = UsdGeomTokens->constant; break;
        }
        SdfValueTypeName sdfTypeName = SdfSchema::GetInstance().FindType(typeName->token);
        UsdGeomPrimvar primvar = api->api.CreatePrimvar(name->token, sdfTypeName, interpToken, elementSize);
        if (primvar) {
            return new UsdGeomPrimvarOpaque(primvar);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

UsdGeomPrimvarRef UsdGeomPrimvarsAPI_GetPrimvar(UsdGeomPrimvarsAPIRef api, TfTokenRef name) {
    if (!api || !name) return nullptr;
#if USD_USE_FULL
    try {
        UsdGeomPrimvar primvar = api->api.GetPrimvar(name->token);
        if (primvar) {
            return new UsdGeomPrimvarOpaque(primvar);
        }
        return nullptr;
    } CATCH_AND_RETURN(nullptr)
#else
    return nullptr;
#endif
}

bool UsdGeomPrimvarsAPI_HasPrimvar(UsdGeomPrimvarsAPIRef api, TfTokenRef name) {
    if (!api || !name) return false;
#if USD_USE_FULL
    return api->api.HasPrimvar(name->token);
#else
    return false;
#endif
}

UsdResult UsdGeomPrimvarsAPI_RemovePrimvar(UsdGeomPrimvarsAPIRef api, TfTokenRef name) {
    if (!api || !name) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        if (api->api.RemovePrimvar(name->token)) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_NOT_FOUND;
    } CATCH_AND_RETURN_RESULT
#else
    return USD_RESULT_SUCCESS;
#endif
}

size_t UsdGeomPrimvarsAPI_GetPrimvarCount(UsdGeomPrimvarsAPIRef api) {
    if (!api) return 0;
#if USD_USE_FULL
    try {
        std::vector<UsdGeomPrimvar> primvars = api->api.GetPrimvars();
        return primvars.size();
    } CATCH_AND_RETURN(0)
#else
    return 0;
#endif
}

size_t UsdGeomPrimvarsAPI_GetPrimvars(UsdGeomPrimvarsAPIRef api, UsdGeomPrimvarRef* outPrimvars, size_t maxCount) {
    if (!api || !outPrimvars) return 0;
#if USD_USE_FULL
    try {
        std::vector<UsdGeomPrimvar> primvars = api->api.GetPrimvars();
        size_t count = std::min(primvars.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outPrimvars[i] = new UsdGeomPrimvarOpaque(primvars[i]);
        }
        return count;
    } CATCH_AND_RETURN(0)
#else
    return 0;
#endif
}

// ============================================================================
// MARK: - UsdGeomXformCache Implementation
// ============================================================================

UsdGeomXformCacheRef UsdGeomXformCache_Create(UsdTimeCode time) {
#if USD_USE_FULL
    try {
        UsdTimeCode tc = time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time);
        return new UsdGeomXformCacheOpaque(tc);
    } CATCH_AND_RETURN(nullptr)
#else
    auto* wrapper = new UsdGeomXformCacheOpaque();
    wrapper->time = time.isDefault ? 0.0 : time.time;
    return wrapper;
#endif
}

UsdGeomXformCacheRef UsdGeomXformCache_Retain(UsdGeomXformCacheRef cache) {
    if (!cache) return nullptr;
    cache->refCount.fetch_add(1, std::memory_order_relaxed);
    return cache;
}

void UsdGeomXformCache_Release(UsdGeomXformCacheRef cache) {
    if (!cache) return;
    if (cache->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete cache;
    }
}

UsdResult UsdGeomXformCache_GetLocalToWorldTransform(UsdGeomXformCacheRef cache, UsdPrimRef prim, double* outMatrix) {
    if (!cache || !prim || !outMatrix) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        GfMatrix4d matrix = cache->cache.GetLocalToWorldTransform(prim->prim);
        const double* data = matrix.data();
        for (int i = 0; i < 16; ++i) {
            outMatrix[i] = data[i];
        }
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    // Return identity matrix
    for (int i = 0; i < 16; ++i) {
        outMatrix[i] = (i % 5 == 0) ? 1.0 : 0.0;
    }
    return USD_RESULT_SUCCESS;
#endif
}

UsdResult UsdGeomXformCache_GetParentToWorldTransform(UsdGeomXformCacheRef cache, UsdPrimRef prim, double* outMatrix) {
    if (!cache || !prim || !outMatrix) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        GfMatrix4d matrix = cache->cache.GetParentToWorldTransform(prim->prim);
        const double* data = matrix.data();
        for (int i = 0; i < 16; ++i) {
            outMatrix[i] = data[i];
        }
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    for (int i = 0; i < 16; ++i) {
        outMatrix[i] = (i % 5 == 0) ? 1.0 : 0.0;
    }
    return USD_RESULT_SUCCESS;
#endif
}

UsdResult UsdGeomXformCache_GetLocalTransformation(UsdGeomXformCacheRef cache, UsdPrimRef prim, double* outMatrix, bool* outResetsXformStack) {
    if (!cache || !prim || !outMatrix || !outResetsXformStack) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        bool resetsStack = false;
        GfMatrix4d matrix = cache->cache.GetLocalTransformation(prim->prim, &resetsStack);
        const double* data = matrix.data();
        for (int i = 0; i < 16; ++i) {
            outMatrix[i] = data[i];
        }
        *outResetsXformStack = resetsStack;
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    for (int i = 0; i < 16; ++i) {
        outMatrix[i] = (i % 5 == 0) ? 1.0 : 0.0;
    }
    *outResetsXformStack = false;
    return USD_RESULT_SUCCESS;
#endif
}

UsdResult UsdGeomXformCache_ComputeRelativeTransform(UsdGeomXformCacheRef cache, UsdPrimRef prim, UsdPrimRef ancestorPrim, double* outMatrix) {
    if (!cache || !prim || !ancestorPrim || !outMatrix) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        bool resetStack = false;
        GfMatrix4d matrix = cache->cache.ComputeRelativeTransform(prim->prim, ancestorPrim->prim, &resetStack);
        const double* data = matrix.data();
        for (int i = 0; i < 16; ++i) {
            outMatrix[i] = data[i];
        }
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    for (int i = 0; i < 16; ++i) {
        outMatrix[i] = (i % 5 == 0) ? 1.0 : 0.0;
    }
    return USD_RESULT_SUCCESS;
#endif
}

void UsdGeomXformCache_SetTime(UsdGeomXformCacheRef cache, UsdTimeCode time) {
    if (!cache) return;
#if USD_USE_FULL
    cache->cache.SetTime(time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
#endif
    cache->time = time.isDefault ? 0.0 : time.time;
}

UsdTimeCode UsdGeomXformCache_GetTime(UsdGeomXformCacheRef cache) {
    UsdTimeCode tc = {0, true};
    if (!cache) return tc;
#if USD_USE_FULL
    UsdTimeCode usdTime = cache->cache.GetTime();
    tc.time = usdTime.IsDefault() ? 0.0 : usdTime.GetValue();
    tc.isDefault = usdTime.IsDefault();
#else
    tc.time = cache->time;
    tc.isDefault = false;
#endif
    return tc;
}

void UsdGeomXformCache_Clear(UsdGeomXformCacheRef cache) {
    if (!cache) return;
#if USD_USE_FULL
    cache->cache.Clear();
#endif
}

// ============================================================================
// MARK: - UsdGeomBBoxCache Implementation
// ============================================================================

UsdGeomBBoxCacheRef UsdGeomBBoxCache_Create(
    UsdTimeCode time,
    const UsdGeomPurpose* purposes,
    size_t purposeCount,
    bool useExtentsHint,
    bool ignoreVisibility
) {
#if USD_USE_FULL
    try {
        std::vector<TfToken> purposeTokens;
        for (size_t i = 0; i < purposeCount; ++i) {
            switch (purposes[i]) {
                case USDGEOM_PURPOSE_DEFAULT: purposeTokens.push_back(UsdGeomTokens->default_); break;
                case USDGEOM_PURPOSE_RENDER: purposeTokens.push_back(UsdGeomTokens->render); break;
                case USDGEOM_PURPOSE_PROXY: purposeTokens.push_back(UsdGeomTokens->proxy); break;
                case USDGEOM_PURPOSE_GUIDE: purposeTokens.push_back(UsdGeomTokens->guide); break;
            }
        }
        UsdTimeCode tc = time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time);
        auto* wrapper = new UsdGeomBBoxCacheOpaque();
        wrapper->cache = new UsdGeomBBoxCache(tc, TfTokenVector(purposeTokens.begin(), purposeTokens.end()), useExtentsHint, ignoreVisibility);
        wrapper->time = time.isDefault ? 0.0 : time.time;
        return wrapper;
    } CATCH_AND_RETURN(nullptr)
#else
    auto* wrapper = new UsdGeomBBoxCacheOpaque();
    wrapper->time = time.isDefault ? 0.0 : time.time;
    return wrapper;
#endif
}

UsdGeomBBoxCacheRef UsdGeomBBoxCache_Retain(UsdGeomBBoxCacheRef cache) {
    if (!cache) return nullptr;
    cache->refCount.fetch_add(1, std::memory_order_relaxed);
    return cache;
}

void UsdGeomBBoxCache_Release(UsdGeomBBoxCacheRef cache) {
    if (!cache) return;
    if (cache->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete cache;
    }
}

UsdResult UsdGeomBBoxCache_ComputeWorldBound(UsdGeomBBoxCacheRef cache, UsdPrimRef prim, double* outMin, double* outMax) {
    if (!cache || !prim || !outMin || !outMax) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        GfBBox3d bbox = cache->cache->ComputeWorldBound(prim->prim);
        GfRange3d range = bbox.ComputeAlignedRange();
        GfVec3d min = range.GetMin();
        GfVec3d max = range.GetMax();
        outMin[0] = min[0]; outMin[1] = min[1]; outMin[2] = min[2];
        outMax[0] = max[0]; outMax[1] = max[1]; outMax[2] = max[2];
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    outMin[0] = outMin[1] = outMin[2] = -1.0;
    outMax[0] = outMax[1] = outMax[2] = 1.0;
    return USD_RESULT_SUCCESS;
#endif
}

UsdResult UsdGeomBBoxCache_ComputeLocalBound(UsdGeomBBoxCacheRef cache, UsdPrimRef prim, double* outMin, double* outMax) {
    if (!cache || !prim || !outMin || !outMax) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        GfBBox3d bbox = cache->cache->ComputeLocalBound(prim->prim);
        GfRange3d range = bbox.ComputeAlignedRange();
        GfVec3d min = range.GetMin();
        GfVec3d max = range.GetMax();
        outMin[0] = min[0]; outMin[1] = min[1]; outMin[2] = min[2];
        outMax[0] = max[0]; outMax[1] = max[1]; outMax[2] = max[2];
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    outMin[0] = outMin[1] = outMin[2] = -1.0;
    outMax[0] = outMax[1] = outMax[2] = 1.0;
    return USD_RESULT_SUCCESS;
#endif
}

UsdResult UsdGeomBBoxCache_ComputeUntransformedBound(UsdGeomBBoxCacheRef cache, UsdPrimRef prim, double* outMin, double* outMax) {
    if (!cache || !prim || !outMin || !outMax) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        GfBBox3d bbox = cache->cache->ComputeUntransformedBound(prim->prim);
        GfRange3d range = bbox.ComputeAlignedRange();
        GfVec3d min = range.GetMin();
        GfVec3d max = range.GetMax();
        outMin[0] = min[0]; outMin[1] = min[1]; outMin[2] = min[2];
        outMax[0] = max[0]; outMax[1] = max[1]; outMax[2] = max[2];
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    outMin[0] = outMin[1] = outMin[2] = -1.0;
    outMax[0] = outMax[1] = outMax[2] = 1.0;
    return USD_RESULT_SUCCESS;
#endif
}

void UsdGeomBBoxCache_Clear(UsdGeomBBoxCacheRef cache) {
    if (!cache) return;
#if USD_USE_FULL
    if (cache->cache) {
        cache->cache->Clear();
    }
#endif
}

void UsdGeomBBoxCache_SetTime(UsdGeomBBoxCacheRef cache, UsdTimeCode time) {
    if (!cache) return;
#if USD_USE_FULL
    if (cache->cache) {
        cache->cache->SetTime(time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
    }
#endif
    cache->time = time.isDefault ? 0.0 : time.time;
}

UsdTimeCode UsdGeomBBoxCache_GetTime(UsdGeomBBoxCacheRef cache) {
    UsdTimeCode tc = {0, true};
    if (!cache) return tc;
#if USD_USE_FULL
    if (cache->cache) {
        UsdTimeCode usdTime = cache->cache->GetTime();
        tc.time = usdTime.IsDefault() ? 0.0 : usdTime.GetValue();
        tc.isDefault = usdTime.IsDefault();
        return tc;
    }
#endif
    tc.time = cache->time;
    tc.isDefault = false;
    return tc;
}

// ============================================================================
// MARK: - Visibility and Purpose Implementation
// ============================================================================

bool UsdGeom_GetVisibility(UsdPrimRef prim, UsdTimeCode time) {
    if (!prim) return true;
#if USD_USE_FULL
    try {
        UsdGeomImageable imageable(prim->prim);
        if (!imageable) return true;
        TfToken visibility;
        imageable.GetVisibilityAttr().Get(&visibility,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return visibility != UsdGeomTokens->invisible;
    } CATCH_AND_RETURN(true)
#else
    return true;
#endif
}

UsdResult UsdGeom_SetVisibility(UsdPrimRef prim, UsdTimeCode time, bool visible) {
    if (!prim) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        UsdGeomImageable imageable(prim->prim);
        if (!imageable) return USD_RESULT_ERROR;
        TfToken visibility = visible ? UsdGeomTokens->inherited : UsdGeomTokens->invisible;
        imageable.GetVisibilityAttr().Set(visibility,
            time.isDefault ? UsdTimeCode::Default() : UsdTimeCode(time.time));
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    return USD_RESULT_SUCCESS;
#endif
}

UsdGeomPurpose UsdGeom_GetPurpose(UsdPrimRef prim) {
    if (!prim) return USDGEOM_PURPOSE_DEFAULT;
#if USD_USE_FULL
    try {
        UsdGeomImageable imageable(prim->prim);
        if (!imageable) return USDGEOM_PURPOSE_DEFAULT;
        TfToken purpose;
        imageable.GetPurposeAttr().Get(&purpose);
        if (purpose == UsdGeomTokens->render) return USDGEOM_PURPOSE_RENDER;
        if (purpose == UsdGeomTokens->proxy) return USDGEOM_PURPOSE_PROXY;
        if (purpose == UsdGeomTokens->guide) return USDGEOM_PURPOSE_GUIDE;
        return USDGEOM_PURPOSE_DEFAULT;
    } CATCH_AND_RETURN(USDGEOM_PURPOSE_DEFAULT)
#else
    return USDGEOM_PURPOSE_DEFAULT;
#endif
}

UsdResult UsdGeom_SetPurpose(UsdPrimRef prim, UsdGeomPurpose purpose) {
    if (!prim) return USD_RESULT_INVALID_ARGUMENT;
#if USD_USE_FULL
    try {
        UsdGeomImageable imageable(prim->prim);
        if (!imageable) return USD_RESULT_ERROR;
        TfToken purposeToken;
        switch (purpose) {
            case USDGEOM_PURPOSE_RENDER: purposeToken = UsdGeomTokens->render; break;
            case USDGEOM_PURPOSE_PROXY: purposeToken = UsdGeomTokens->proxy; break;
            case USDGEOM_PURPOSE_GUIDE: purposeToken = UsdGeomTokens->guide; break;
            default: purposeToken = UsdGeomTokens->default_; break;
        }
        imageable.GetPurposeAttr().Set(purposeToken);
        return USD_RESULT_SUCCESS;
    } CATCH_AND_RETURN_RESULT
#else
    return USD_RESULT_SUCCESS;
#endif
}

} // extern "C"
