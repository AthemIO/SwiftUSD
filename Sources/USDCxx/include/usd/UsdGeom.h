// UsdGeom.h - Geometry Schemas for SwiftUSD
// Mirrors: pxr/usd/usdGeom/*.h (Xform, Mesh, Points, BasisCurves, Camera, Primvars, BoundingBox)
// C++ header with Swift interop annotations

#pragma once

#include "../Swift.h"
#include "../base/Gf.h"
#include "../base/Tf.h"
#include "../base/Vt.h"
#include "Sdf.h"
#include "Usd.h"

#include <atomic>
#include <cstring>
#include <string>
#include <vector>
#include <memory>

#if defined(USE_PIXAR_USD)
#include "pxr/pxr.h"
#include "pxr/usd/usdGeom/xform.h"
#include "pxr/usd/usdGeom/mesh.h"
#include "pxr/usd/usdGeom/points.h"
#include "pxr/usd/usdGeom/basisCurves.h"
#include "pxr/usd/usdGeom/camera.h"
#include "pxr/usd/usdGeom/sphere.h"
#include "pxr/usd/usdGeom/cube.h"
#include "pxr/usd/usdGeom/cylinder.h"
#include "pxr/usd/usdGeom/cone.h"
#include "pxr/usd/usdGeom/capsule.h"
#include "pxr/usd/usdGeom/primvar.h"
#include "pxr/usd/usdGeom/primvarsAPI.h"
#include "pxr/usd/usdGeom/xformCache.h"
#include "pxr/usd/usdGeom/bboxCache.h"
#include "pxr/usd/usdGeom/tokens.h"
#endif

namespace swiftusd {

// Forward declarations
class GeomXformRef;
class GeomMeshRef;
class GeomPointsRef;
class GeomBasisCurvesRef;
class GeomCameraRef;
class GeomSphereRef;
class GeomCubeRef;
class GeomCylinderRef;
class GeomConeRef;
class GeomCapsuleRef;
class GeomPrimvarRef;
class GeomPrimvarsAPIRef;
class GeomXformCacheRef;
class GeomBBoxCacheRef;

// ============================================================================
// MARK: - Enums (SWIFT_SELF_CONTAINED value types)
// ============================================================================

/// Interpolation mode for primvars
struct GeomInterpolation {
    enum Value : int {
        Constant = 0,    // One value for entire surface
        Uniform = 1,     // One value per face
        Varying = 2,     // One value per vertex (bilinear)
        Vertex = 3,      // One value per vertex (smooth)
        FaceVarying = 4  // One value per face-vertex
    };

    Value value;

    GeomInterpolation() SWIFTUSD_NOEXCEPT : value(Constant) {}
    GeomInterpolation(Value v) SWIFTUSD_NOEXCEPT : value(v) {}
    explicit GeomInterpolation(int v) SWIFTUSD_NOEXCEPT : value(static_cast<Value>(v)) {}

    bool operator==(const GeomInterpolation& other) const SWIFTUSD_NOEXCEPT { return value == other.value; }
    bool operator!=(const GeomInterpolation& other) const SWIFTUSD_NOEXCEPT { return value != other.value; }

#if defined(USE_PIXAR_USD)
    PXR_NS::TfToken ToUsd() const SWIFTUSD_NOEXCEPT {
        switch (value) {
            case Constant: return PXR_NS::UsdGeomTokens->constant;
            case Uniform: return PXR_NS::UsdGeomTokens->uniform;
            case Varying: return PXR_NS::UsdGeomTokens->varying;
            case Vertex: return PXR_NS::UsdGeomTokens->vertex;
            case FaceVarying: return PXR_NS::UsdGeomTokens->faceVarying;
            default: return PXR_NS::UsdGeomTokens->constant;
        }
    }

    static GeomInterpolation FromUsd(const PXR_NS::TfToken& token) SWIFTUSD_NOEXCEPT {
        if (token == PXR_NS::UsdGeomTokens->uniform) return GeomInterpolation(Uniform);
        if (token == PXR_NS::UsdGeomTokens->varying) return GeomInterpolation(Varying);
        if (token == PXR_NS::UsdGeomTokens->vertex) return GeomInterpolation(Vertex);
        if (token == PXR_NS::UsdGeomTokens->faceVarying) return GeomInterpolation(FaceVarying);
        return GeomInterpolation(Constant);
    }
#endif
} SWIFT_SELF_CONTAINED;

/// Subdivision scheme for meshes
struct SubdivisionScheme {
    enum Value : int {
        None = 0,        // No subdivision
        CatmullClark = 1, // Catmull-Clark
        Loop = 2,        // Loop subdivision
        Bilinear = 3     // Bilinear
    };

    Value value;

    SubdivisionScheme() SWIFTUSD_NOEXCEPT : value(None) {}
    SubdivisionScheme(Value v) SWIFTUSD_NOEXCEPT : value(v) {}

    bool operator==(const SubdivisionScheme& other) const SWIFTUSD_NOEXCEPT { return value == other.value; }
    bool operator!=(const SubdivisionScheme& other) const SWIFTUSD_NOEXCEPT { return value != other.value; }

#if defined(USE_PIXAR_USD)
    PXR_NS::TfToken ToUsd() const SWIFTUSD_NOEXCEPT {
        switch (value) {
            case CatmullClark: return PXR_NS::UsdGeomTokens->catmullClark;
            case Loop: return PXR_NS::UsdGeomTokens->loop;
            case Bilinear: return PXR_NS::UsdGeomTokens->bilinear;
            default: return PXR_NS::UsdGeomTokens->none;
        }
    }

    static SubdivisionScheme FromUsd(const PXR_NS::TfToken& token) SWIFTUSD_NOEXCEPT {
        if (token == PXR_NS::UsdGeomTokens->catmullClark) return SubdivisionScheme(CatmullClark);
        if (token == PXR_NS::UsdGeomTokens->loop) return SubdivisionScheme(Loop);
        if (token == PXR_NS::UsdGeomTokens->bilinear) return SubdivisionScheme(Bilinear);
        return SubdivisionScheme(None);
    }
#endif
} SWIFT_SELF_CONTAINED;

/// Axis orientation for primitives
struct GeomAxis {
    enum Value : int {
        X = 0,
        Y = 1,
        Z = 2
    };

    Value value;

    GeomAxis() SWIFTUSD_NOEXCEPT : value(Z) {}
    GeomAxis(Value v) SWIFTUSD_NOEXCEPT : value(v) {}

    bool operator==(const GeomAxis& other) const SWIFTUSD_NOEXCEPT { return value == other.value; }
    bool operator!=(const GeomAxis& other) const SWIFTUSD_NOEXCEPT { return value != other.value; }

#if defined(USE_PIXAR_USD)
    PXR_NS::TfToken ToUsd() const SWIFTUSD_NOEXCEPT {
        switch (value) {
            case X: return PXR_NS::UsdGeomTokens->x;
            case Y: return PXR_NS::UsdGeomTokens->y;
            default: return PXR_NS::UsdGeomTokens->z;
        }
    }

    static GeomAxis FromUsd(const PXR_NS::TfToken& token) SWIFTUSD_NOEXCEPT {
        if (token == PXR_NS::UsdGeomTokens->x) return GeomAxis(X);
        if (token == PXR_NS::UsdGeomTokens->y) return GeomAxis(Y);
        return GeomAxis(Z);
    }
#endif
} SWIFT_SELF_CONTAINED;

/// Purpose for visibility/rendering
struct GeomPurpose {
    enum Value : int {
        Default = 0,
        Render = 1,
        Proxy = 2,
        Guide = 3
    };

    Value value;

    GeomPurpose() SWIFTUSD_NOEXCEPT : value(Default) {}
    GeomPurpose(Value v) SWIFTUSD_NOEXCEPT : value(v) {}

    bool operator==(const GeomPurpose& other) const SWIFTUSD_NOEXCEPT { return value == other.value; }
    bool operator!=(const GeomPurpose& other) const SWIFTUSD_NOEXCEPT { return value != other.value; }

#if defined(USE_PIXAR_USD)
    PXR_NS::TfToken ToUsd() const SWIFTUSD_NOEXCEPT {
        switch (value) {
            case Render: return PXR_NS::UsdGeomTokens->render;
            case Proxy: return PXR_NS::UsdGeomTokens->proxy;
            case Guide: return PXR_NS::UsdGeomTokens->guide;
            default: return PXR_NS::UsdGeomTokens->default_;
        }
    }

    static GeomPurpose FromUsd(const PXR_NS::TfToken& token) SWIFTUSD_NOEXCEPT {
        if (token == PXR_NS::UsdGeomTokens->render) return GeomPurpose(Render);
        if (token == PXR_NS::UsdGeomTokens->proxy) return GeomPurpose(Proxy);
        if (token == PXR_NS::UsdGeomTokens->guide) return GeomPurpose(Guide);
        return GeomPurpose(Default);
    }
#endif
} SWIFT_SELF_CONTAINED;

/// Curve type for basis curves
struct CurveType {
    enum Value : int {
        Linear = 0,
        Cubic = 1
    };

    Value value;

    CurveType() SWIFTUSD_NOEXCEPT : value(Linear) {}
    CurveType(Value v) SWIFTUSD_NOEXCEPT : value(v) {}

    bool operator==(const CurveType& other) const SWIFTUSD_NOEXCEPT { return value == other.value; }
    bool operator!=(const CurveType& other) const SWIFTUSD_NOEXCEPT { return value != other.value; }

#if defined(USE_PIXAR_USD)
    PXR_NS::TfToken ToUsd() const SWIFTUSD_NOEXCEPT {
        return value == Cubic ? PXR_NS::UsdGeomTokens->cubic : PXR_NS::UsdGeomTokens->linear;
    }

    static CurveType FromUsd(const PXR_NS::TfToken& token) SWIFTUSD_NOEXCEPT {
        return token == PXR_NS::UsdGeomTokens->cubic ? CurveType(Cubic) : CurveType(Linear);
    }
#endif
} SWIFT_SELF_CONTAINED;

/// Basis for cubic curves
struct CurveBasis {
    enum Value : int {
        Bezier = 0,
        BSpline = 1,
        CatmullRom = 2
    };

    Value value;

    CurveBasis() SWIFTUSD_NOEXCEPT : value(Bezier) {}
    CurveBasis(Value v) SWIFTUSD_NOEXCEPT : value(v) {}

    bool operator==(const CurveBasis& other) const SWIFTUSD_NOEXCEPT { return value == other.value; }
    bool operator!=(const CurveBasis& other) const SWIFTUSD_NOEXCEPT { return value != other.value; }

#if defined(USE_PIXAR_USD)
    PXR_NS::TfToken ToUsd() const SWIFTUSD_NOEXCEPT {
        switch (value) {
            case BSpline: return PXR_NS::UsdGeomTokens->bspline;
            case CatmullRom: return PXR_NS::UsdGeomTokens->catmullRom;
            default: return PXR_NS::UsdGeomTokens->bezier;
        }
    }

    static CurveBasis FromUsd(const PXR_NS::TfToken& token) SWIFTUSD_NOEXCEPT {
        if (token == PXR_NS::UsdGeomTokens->bspline) return CurveBasis(BSpline);
        if (token == PXR_NS::UsdGeomTokens->catmullRom) return CurveBasis(CatmullRom);
        return CurveBasis(Bezier);
    }
#endif
} SWIFT_SELF_CONTAINED;

/// Wrap mode for curves
struct CurveWrap {
    enum Value : int {
        Nonperiodic = 0,
        Periodic = 1,
        Pinned = 2
    };

    Value value;

    CurveWrap() SWIFTUSD_NOEXCEPT : value(Nonperiodic) {}
    CurveWrap(Value v) SWIFTUSD_NOEXCEPT : value(v) {}

    bool operator==(const CurveWrap& other) const SWIFTUSD_NOEXCEPT { return value == other.value; }
    bool operator!=(const CurveWrap& other) const SWIFTUSD_NOEXCEPT { return value != other.value; }

#if defined(USE_PIXAR_USD)
    PXR_NS::TfToken ToUsd() const SWIFTUSD_NOEXCEPT {
        switch (value) {
            case Periodic: return PXR_NS::UsdGeomTokens->periodic;
            case Pinned: return PXR_NS::UsdGeomTokens->pinned;
            default: return PXR_NS::UsdGeomTokens->nonperiodic;
        }
    }

    static CurveWrap FromUsd(const PXR_NS::TfToken& token) SWIFTUSD_NOEXCEPT {
        if (token == PXR_NS::UsdGeomTokens->periodic) return CurveWrap(Periodic);
        if (token == PXR_NS::UsdGeomTokens->pinned) return CurveWrap(Pinned);
        return CurveWrap(Nonperiodic);
    }
#endif
} SWIFT_SELF_CONTAINED;

// ============================================================================
// MARK: - GeomXformRef (UsdGeomXform reference type)
// ============================================================================

/// A transformable container prim.
class GeomXformRef {
public:
    // Factory methods
    static GeomXformRef* Define(StageRef* stage, const Path& path) SWIFTUSD_NOEXCEPT {
        if (!stage) return nullptr;
#if defined(USE_PIXAR_USD)
        auto xform = PXR_NS::UsdGeomXform::Define(stage->GetUsdStage(), path.GetSdfPath());
        if (!xform) return nullptr;
        return new GeomXformRef(xform, stage);
#else
        return new GeomXformRef(path, stage);
#endif
    }

    static GeomXformRef* FromPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto xform = PXR_NS::UsdGeomXform(prim->GetUsdPrim());
        if (!xform) return nullptr;
        return new GeomXformRef(xform, prim->GetStage());
#else
        return new GeomXformRef(prim->GetPath(), prim->GetStage());
#endif
    }

    // Validity
    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_xform);
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    // Prim access
    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_xform) return nullptr;
        return new PrimRef(_xform.GetPrim(), _stage);
#else
        return _stage ? _stage->GetPrimAtPath(_path) : nullptr;
#endif
    }

    // Transform operations
    bool ComputeLocalToWorldTransform(TimeCode time, double* outMatrix) const SWIFTUSD_NOEXCEPT {
        if (!outMatrix) return false;
#if defined(USE_PIXAR_USD)
        if (!_xform) return false;
        PXR_NS::UsdGeomXformCache cache(time.ToUsd());
        auto matrix = cache.GetLocalToWorldTransform(_xform.GetPrim());
        const double* data = matrix.GetArray();
        for (int i = 0; i < 16; ++i) outMatrix[i] = data[i];
        return true;
#else
        // Return identity matrix
        for (int i = 0; i < 16; ++i) outMatrix[i] = (i % 5 == 0) ? 1.0 : 0.0;
        return true;
#endif
    }

    bool AddTranslateOp(double x, double y, double z, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_xform) return false;
        auto op = _xform.AddTranslateOp();
        return op.Set(PXR_NS::GfVec3d(x, y, z), time.ToUsd());
#else
        _translate = Vec3d(x, y, z);
        return true;
#endif
    }

    bool AddRotateXYZOp(double rotX, double rotY, double rotZ, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_xform) return false;
        auto op = _xform.AddRotateXYZOp();
        return op.Set(PXR_NS::GfVec3f(rotX, rotY, rotZ), time.ToUsd());
#else
        _rotate = Vec3d(rotX, rotY, rotZ);
        return true;
#endif
    }

    bool AddScaleOp(double scaleX, double scaleY, double scaleZ, TimeCode time = TimeCode::Default()) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_xform) return false;
        auto op = _xform.AddScaleOp();
        return op.Set(PXR_NS::GfVec3f(scaleX, scaleY, scaleZ), time.ToUsd());
#else
        _scale = Vec3d(scaleX, scaleY, scaleZ);
        return true;
#endif
    }

    bool ClearXformOpOrder() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_xform) return false;
        return _xform.ClearXformOpOrder();
#else
        _translate = Vec3d();
        _rotate = Vec3d();
        _scale = Vec3d(1, 1, 1);
        return true;
#endif
    }

    // Reference counting
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
    GeomXformRef(const PXR_NS::UsdGeomXform& xform, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _xform(xform), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~GeomXformRef() {
        if (_stage) _stage->release();
    }

    PXR_NS::UsdGeomXform _xform;
#else
    GeomXformRef(const Path& path, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path), _stage(stage), _translate(), _rotate(), _scale(1, 1, 1), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~GeomXformRef() {
        if (_stage) _stage->release();
    }

    Path _path;
    Vec3d _translate;
    Vec3d _rotate;
    Vec3d _scale;
#endif

    StageRef* _stage;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - GeomMeshRef (UsdGeomMesh reference type)
// ============================================================================

/// A polygonal mesh prim.
class GeomMeshRef {
public:
    // Factory methods
    static GeomMeshRef* Define(StageRef* stage, const Path& path) SWIFTUSD_NOEXCEPT {
        if (!stage) return nullptr;
#if defined(USE_PIXAR_USD)
        auto mesh = PXR_NS::UsdGeomMesh::Define(stage->GetUsdStage(), path.GetSdfPath());
        if (!mesh) return nullptr;
        return new GeomMeshRef(mesh, stage);
#else
        return new GeomMeshRef(path, stage);
#endif
    }

    static GeomMeshRef* FromPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto mesh = PXR_NS::UsdGeomMesh(prim->GetUsdPrim());
        if (!mesh) return nullptr;
        return new GeomMeshRef(mesh, prim->GetStage());
#else
        return new GeomMeshRef(prim->GetPath(), prim->GetStage());
#endif
    }

    // Validity
    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_mesh);
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    // Prim access
    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_mesh) return nullptr;
        return new PrimRef(_mesh.GetPrim(), _stage);
#else
        return _stage ? _stage->GetPrimAtPath(_path) : nullptr;
#endif
    }

    // Points
    size_t GetPointCount(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_mesh) return 0;
        PXR_NS::VtArray<PXR_NS::GfVec3f> points;
        _mesh.GetPointsAttr().Get(&points, time.ToUsd());
        return points.size();
#else
        return _points.size() / 3;
#endif
    }

    size_t GetPoints(TimeCode time, float* outBuffer, size_t maxCount) const SWIFTUSD_NOEXCEPT {
        if (!outBuffer) return 0;
#if defined(USE_PIXAR_USD)
        if (!_mesh) return 0;
        PXR_NS::VtArray<PXR_NS::GfVec3f> points;
        if (!_mesh.GetPointsAttr().Get(&points, time.ToUsd())) return 0;
        size_t count = std::min(points.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outBuffer[i * 3 + 0] = points[i][0];
            outBuffer[i * 3 + 1] = points[i][1];
            outBuffer[i * 3 + 2] = points[i][2];
        }
        return count;
#else
        size_t count = std::min(_points.size() / 3, maxCount);
        for (size_t i = 0; i < count * 3; ++i) {
            outBuffer[i] = _points[i];
        }
        return count;
#endif
    }

    bool SetPoints(TimeCode time, const float* points, size_t count) SWIFTUSD_NOEXCEPT {
        if (!points && count > 0) return false;
#if defined(USE_PIXAR_USD)
        if (!_mesh) return false;
        PXR_NS::VtArray<PXR_NS::GfVec3f> vtPoints(count);
        for (size_t i = 0; i < count; ++i) {
            vtPoints[i] = PXR_NS::GfVec3f(points[i * 3], points[i * 3 + 1], points[i * 3 + 2]);
        }
        return _mesh.GetPointsAttr().Set(vtPoints, time.ToUsd());
#else
        _points.resize(count * 3);
        for (size_t i = 0; i < count * 3; ++i) {
            _points[i] = points[i];
        }
        return true;
#endif
    }

    // Face vertex counts
    size_t GetFaceVertexCountsCount(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_mesh) return 0;
        PXR_NS::VtArray<int> counts;
        _mesh.GetFaceVertexCountsAttr().Get(&counts, time.ToUsd());
        return counts.size();
#else
        return _faceVertexCounts.size();
#endif
    }

    size_t GetFaceVertexCounts(TimeCode time, int* outBuffer, size_t maxCount) const SWIFTUSD_NOEXCEPT {
        if (!outBuffer) return 0;
#if defined(USE_PIXAR_USD)
        if (!_mesh) return 0;
        PXR_NS::VtArray<int> counts;
        if (!_mesh.GetFaceVertexCountsAttr().Get(&counts, time.ToUsd())) return 0;
        size_t count = std::min(counts.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outBuffer[i] = counts[i];
        }
        return count;
#else
        size_t count = std::min(_faceVertexCounts.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outBuffer[i] = _faceVertexCounts[i];
        }
        return count;
#endif
    }

    bool SetFaceVertexCounts(TimeCode time, const int* counts, size_t count) SWIFTUSD_NOEXCEPT {
        if (!counts && count > 0) return false;
#if defined(USE_PIXAR_USD)
        if (!_mesh) return false;
        PXR_NS::VtArray<int> vtCounts(count);
        for (size_t i = 0; i < count; ++i) {
            vtCounts[i] = counts[i];
        }
        return _mesh.GetFaceVertexCountsAttr().Set(vtCounts, time.ToUsd());
#else
        _faceVertexCounts.assign(counts, counts + count);
        return true;
#endif
    }

    // Face vertex indices
    size_t GetFaceVertexIndicesCount(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_mesh) return 0;
        PXR_NS::VtArray<int> indices;
        _mesh.GetFaceVertexIndicesAttr().Get(&indices, time.ToUsd());
        return indices.size();
#else
        return _faceVertexIndices.size();
#endif
    }

    size_t GetFaceVertexIndices(TimeCode time, int* outBuffer, size_t maxCount) const SWIFTUSD_NOEXCEPT {
        if (!outBuffer) return 0;
#if defined(USE_PIXAR_USD)
        if (!_mesh) return 0;
        PXR_NS::VtArray<int> indices;
        if (!_mesh.GetFaceVertexIndicesAttr().Get(&indices, time.ToUsd())) return 0;
        size_t count = std::min(indices.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outBuffer[i] = indices[i];
        }
        return count;
#else
        size_t count = std::min(_faceVertexIndices.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outBuffer[i] = _faceVertexIndices[i];
        }
        return count;
#endif
    }

    bool SetFaceVertexIndices(TimeCode time, const int* indices, size_t count) SWIFTUSD_NOEXCEPT {
        if (!indices && count > 0) return false;
#if defined(USE_PIXAR_USD)
        if (!_mesh) return false;
        PXR_NS::VtArray<int> vtIndices(count);
        for (size_t i = 0; i < count; ++i) {
            vtIndices[i] = indices[i];
        }
        return _mesh.GetFaceVertexIndicesAttr().Set(vtIndices, time.ToUsd());
#else
        _faceVertexIndices.assign(indices, indices + count);
        return true;
#endif
    }

    // Subdivision scheme
    SubdivisionScheme GetSubdivisionScheme() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_mesh) return SubdivisionScheme();
        PXR_NS::TfToken scheme;
        _mesh.GetSubdivisionSchemeAttr().Get(&scheme);
        return SubdivisionScheme::FromUsd(scheme);
#else
        return _subdivisionScheme;
#endif
    }

    bool SetSubdivisionScheme(SubdivisionScheme scheme) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_mesh) return false;
        return _mesh.GetSubdivisionSchemeAttr().Set(scheme.ToUsd());
#else
        _subdivisionScheme = scheme;
        return true;
#endif
    }

    // Double-sided
    bool GetDoubleSided() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_mesh) return false;
        bool doubleSided = false;
        _mesh.GetDoubleSidedAttr().Get(&doubleSided);
        return doubleSided;
#else
        return _doubleSided;
#endif
    }

    bool SetDoubleSided(bool doubleSided) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_mesh) return false;
        return _mesh.GetDoubleSidedAttr().Set(doubleSided);
#else
        _doubleSided = doubleSided;
        return true;
#endif
    }

    // Reference counting
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
    GeomMeshRef(const PXR_NS::UsdGeomMesh& mesh, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _mesh(mesh), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~GeomMeshRef() {
        if (_stage) _stage->release();
    }

    PXR_NS::UsdGeomMesh _mesh;
#else
    GeomMeshRef(const Path& path, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path), _stage(stage), _doubleSided(false), _refCount(1) {
        if (_stage) _stage->retain();
    }

    ~GeomMeshRef() {
        if (_stage) _stage->release();
    }

    Path _path;
    std::vector<float> _points;
    std::vector<int> _faceVertexCounts;
    std::vector<int> _faceVertexIndices;
    SubdivisionScheme _subdivisionScheme;
    bool _doubleSided;
#endif

    StageRef* _stage;
    std::atomic<int> _refCount;

} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - GeomPointsRef (UsdGeomPoints reference type)
// ============================================================================

/// A point cloud prim.
class GeomPointsRef {
public:
    static GeomPointsRef* Define(StageRef* stage, const Path& path) SWIFTUSD_NOEXCEPT {
        if (!stage) return nullptr;
#if defined(USE_PIXAR_USD)
        auto points = PXR_NS::UsdGeomPoints::Define(stage->GetUsdStage(), path.GetSdfPath());
        if (!points) return nullptr;
        return new GeomPointsRef(points, stage);
#else
        return new GeomPointsRef(path, stage);
#endif
    }

    static GeomPointsRef* FromPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto points = PXR_NS::UsdGeomPoints(prim->GetUsdPrim());
        if (!points) return nullptr;
        return new GeomPointsRef(points, prim->GetStage());
#else
        return new GeomPointsRef(prim->GetPath(), prim->GetStage());
#endif
    }

    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_points);
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_points) return nullptr;
        return new PrimRef(_points.GetPrim(), _stage);
#else
        return _stage ? _stage->GetPrimAtPath(_path) : nullptr;
#endif
    }

    // Points
    size_t GetPointCount(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_points) return 0;
        PXR_NS::VtArray<PXR_NS::GfVec3f> pts;
        _points.GetPointsAttr().Get(&pts, time.ToUsd());
        return pts.size();
#else
        return _pointData.size() / 3;
#endif
    }

    size_t GetPoints(TimeCode time, float* outBuffer, size_t maxCount) const SWIFTUSD_NOEXCEPT {
        if (!outBuffer) return 0;
#if defined(USE_PIXAR_USD)
        if (!_points) return 0;
        PXR_NS::VtArray<PXR_NS::GfVec3f> pts;
        if (!_points.GetPointsAttr().Get(&pts, time.ToUsd())) return 0;
        size_t count = std::min(pts.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outBuffer[i * 3 + 0] = pts[i][0];
            outBuffer[i * 3 + 1] = pts[i][1];
            outBuffer[i * 3 + 2] = pts[i][2];
        }
        return count;
#else
        size_t count = std::min(_pointData.size() / 3, maxCount);
        for (size_t i = 0; i < count * 3; ++i) {
            outBuffer[i] = _pointData[i];
        }
        return count;
#endif
    }

    bool SetPoints(TimeCode time, const float* pts, size_t count) SWIFTUSD_NOEXCEPT {
        if (!pts && count > 0) return false;
#if defined(USE_PIXAR_USD)
        if (!_points) return false;
        PXR_NS::VtArray<PXR_NS::GfVec3f> vtPts(count);
        for (size_t i = 0; i < count; ++i) {
            vtPts[i] = PXR_NS::GfVec3f(pts[i * 3], pts[i * 3 + 1], pts[i * 3 + 2]);
        }
        return _points.GetPointsAttr().Set(vtPts, time.ToUsd());
#else
        _pointData.resize(count * 3);
        for (size_t i = 0; i < count * 3; ++i) {
            _pointData[i] = pts[i];
        }
        return true;
#endif
    }

    // Widths
    size_t GetWidthsCount(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_points) return 0;
        PXR_NS::VtArray<float> widths;
        _points.GetWidthsAttr().Get(&widths, time.ToUsd());
        return widths.size();
#else
        return _widths.size();
#endif
    }

    size_t GetWidths(TimeCode time, float* outBuffer, size_t maxCount) const SWIFTUSD_NOEXCEPT {
        if (!outBuffer) return 0;
#if defined(USE_PIXAR_USD)
        if (!_points) return 0;
        PXR_NS::VtArray<float> widths;
        if (!_points.GetWidthsAttr().Get(&widths, time.ToUsd())) return 0;
        size_t count = std::min(widths.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outBuffer[i] = widths[i];
        }
        return count;
#else
        size_t count = std::min(_widths.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outBuffer[i] = _widths[i];
        }
        return count;
#endif
    }

    bool SetWidths(TimeCode time, const float* widths, size_t count) SWIFTUSD_NOEXCEPT {
        if (!widths && count > 0) return false;
#if defined(USE_PIXAR_USD)
        if (!_points) return false;
        PXR_NS::VtArray<float> vtWidths(count);
        for (size_t i = 0; i < count; ++i) {
            vtWidths[i] = widths[i];
        }
        return _points.GetWidthsAttr().Set(vtWidths, time.ToUsd());
#else
        _widths.assign(widths, widths + count);
        return true;
#endif
    }

    // IDs
    size_t GetIdsCount(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_points) return 0;
        PXR_NS::VtArray<int64_t> ids;
        _points.GetIdsAttr().Get(&ids, time.ToUsd());
        return ids.size();
#else
        return _ids.size();
#endif
    }

    size_t GetIds(TimeCode time, int64_t* outBuffer, size_t maxCount) const SWIFTUSD_NOEXCEPT {
        if (!outBuffer) return 0;
#if defined(USE_PIXAR_USD)
        if (!_points) return 0;
        PXR_NS::VtArray<int64_t> ids;
        if (!_points.GetIdsAttr().Get(&ids, time.ToUsd())) return 0;
        size_t count = std::min(ids.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outBuffer[i] = ids[i];
        }
        return count;
#else
        size_t count = std::min(_ids.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outBuffer[i] = _ids[i];
        }
        return count;
#endif
    }

    bool SetIds(TimeCode time, const int64_t* ids, size_t count) SWIFTUSD_NOEXCEPT {
        if (!ids && count > 0) return false;
#if defined(USE_PIXAR_USD)
        if (!_points) return false;
        PXR_NS::VtArray<int64_t> vtIds(count);
        for (size_t i = 0; i < count; ++i) {
            vtIds[i] = ids[i];
        }
        return _points.GetIdsAttr().Set(vtIds, time.ToUsd());
#else
        _ids.assign(ids, ids + count);
        return true;
#endif
    }

    void retain() SWIFTUSD_NOEXCEPT { _refCount.fetch_add(1, std::memory_order_relaxed); }
    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) delete this;
    }

private:
#if defined(USE_PIXAR_USD)
    GeomPointsRef(const PXR_NS::UsdGeomPoints& points, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _points(points), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }
    ~GeomPointsRef() { if (_stage) _stage->release(); }
    PXR_NS::UsdGeomPoints _points;
#else
    GeomPointsRef(const Path& path, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }
    ~GeomPointsRef() { if (_stage) _stage->release(); }
    Path _path;
    std::vector<float> _pointData;
    std::vector<float> _widths;
    std::vector<int64_t> _ids;
#endif
    StageRef* _stage;
    std::atomic<int> _refCount;
} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - GeomBasisCurvesRef (UsdGeomBasisCurves reference type)
// ============================================================================

/// A collection of parametric curves.
class GeomBasisCurvesRef {
public:
    static GeomBasisCurvesRef* Define(StageRef* stage, const Path& path) SWIFTUSD_NOEXCEPT {
        if (!stage) return nullptr;
#if defined(USE_PIXAR_USD)
        auto curves = PXR_NS::UsdGeomBasisCurves::Define(stage->GetUsdStage(), path.GetSdfPath());
        if (!curves) return nullptr;
        return new GeomBasisCurvesRef(curves, stage);
#else
        return new GeomBasisCurvesRef(path, stage);
#endif
    }

    static GeomBasisCurvesRef* FromPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto curves = PXR_NS::UsdGeomBasisCurves(prim->GetUsdPrim());
        if (!curves) return nullptr;
        return new GeomBasisCurvesRef(curves, prim->GetStage());
#else
        return new GeomBasisCurvesRef(prim->GetPath(), prim->GetStage());
#endif
    }

    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_curves);
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_curves) return nullptr;
        return new PrimRef(_curves.GetPrim(), _stage);
#else
        return _stage ? _stage->GetPrimAtPath(_path) : nullptr;
#endif
    }

    // Curve type
    CurveType GetType() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_curves) return CurveType();
        PXR_NS::TfToken type;
        _curves.GetTypeAttr().Get(&type);
        return CurveType::FromUsd(type);
#else
        return _curveType;
#endif
    }

    bool SetType(CurveType type) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_curves) return false;
        return _curves.GetTypeAttr().Set(type.ToUsd());
#else
        _curveType = type;
        return true;
#endif
    }

    // Basis
    CurveBasis GetBasis() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_curves) return CurveBasis();
        PXR_NS::TfToken basis;
        _curves.GetBasisAttr().Get(&basis);
        return CurveBasis::FromUsd(basis);
#else
        return _basis;
#endif
    }

    bool SetBasis(CurveBasis basis) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_curves) return false;
        return _curves.GetBasisAttr().Set(basis.ToUsd());
#else
        _basis = basis;
        return true;
#endif
    }

    // Wrap
    CurveWrap GetWrap() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_curves) return CurveWrap();
        PXR_NS::TfToken wrap;
        _curves.GetWrapAttr().Get(&wrap);
        return CurveWrap::FromUsd(wrap);
#else
        return _wrap;
#endif
    }

    bool SetWrap(CurveWrap wrap) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_curves) return false;
        return _curves.GetWrapAttr().Set(wrap.ToUsd());
#else
        _wrap = wrap;
        return true;
#endif
    }

    // Curve vertex counts
    size_t GetCurveVertexCountsCount(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_curves) return 0;
        PXR_NS::VtArray<int> counts;
        _curves.GetCurveVertexCountsAttr().Get(&counts, time.ToUsd());
        return counts.size();
#else
        return _curveVertexCounts.size();
#endif
    }

    size_t GetCurveVertexCounts(TimeCode time, int* outBuffer, size_t maxCount) const SWIFTUSD_NOEXCEPT {
        if (!outBuffer) return 0;
#if defined(USE_PIXAR_USD)
        if (!_curves) return 0;
        PXR_NS::VtArray<int> counts;
        if (!_curves.GetCurveVertexCountsAttr().Get(&counts, time.ToUsd())) return 0;
        size_t count = std::min(counts.size(), maxCount);
        for (size_t i = 0; i < count; ++i) outBuffer[i] = counts[i];
        return count;
#else
        size_t count = std::min(_curveVertexCounts.size(), maxCount);
        for (size_t i = 0; i < count; ++i) outBuffer[i] = _curveVertexCounts[i];
        return count;
#endif
    }

    bool SetCurveVertexCounts(TimeCode time, const int* counts, size_t count) SWIFTUSD_NOEXCEPT {
        if (!counts && count > 0) return false;
#if defined(USE_PIXAR_USD)
        if (!_curves) return false;
        PXR_NS::VtArray<int> vtCounts(counts, counts + count);
        return _curves.GetCurveVertexCountsAttr().Set(vtCounts, time.ToUsd());
#else
        _curveVertexCounts.assign(counts, counts + count);
        return true;
#endif
    }

    // Points
    size_t GetPointCount(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_curves) return 0;
        PXR_NS::VtArray<PXR_NS::GfVec3f> pts;
        _curves.GetPointsAttr().Get(&pts, time.ToUsd());
        return pts.size();
#else
        return _points.size() / 3;
#endif
    }

    size_t GetPoints(TimeCode time, float* outBuffer, size_t maxCount) const SWIFTUSD_NOEXCEPT {
        if (!outBuffer) return 0;
#if defined(USE_PIXAR_USD)
        if (!_curves) return 0;
        PXR_NS::VtArray<PXR_NS::GfVec3f> pts;
        if (!_curves.GetPointsAttr().Get(&pts, time.ToUsd())) return 0;
        size_t count = std::min(pts.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outBuffer[i * 3 + 0] = pts[i][0];
            outBuffer[i * 3 + 1] = pts[i][1];
            outBuffer[i * 3 + 2] = pts[i][2];
        }
        return count;
#else
        size_t count = std::min(_points.size() / 3, maxCount);
        for (size_t i = 0; i < count * 3; ++i) outBuffer[i] = _points[i];
        return count;
#endif
    }

    bool SetPoints(TimeCode time, const float* pts, size_t count) SWIFTUSD_NOEXCEPT {
        if (!pts && count > 0) return false;
#if defined(USE_PIXAR_USD)
        if (!_curves) return false;
        PXR_NS::VtArray<PXR_NS::GfVec3f> vtPts(count);
        for (size_t i = 0; i < count; ++i) {
            vtPts[i] = PXR_NS::GfVec3f(pts[i * 3], pts[i * 3 + 1], pts[i * 3 + 2]);
        }
        return _curves.GetPointsAttr().Set(vtPts, time.ToUsd());
#else
        _points.resize(count * 3);
        for (size_t i = 0; i < count * 3; ++i) _points[i] = pts[i];
        return true;
#endif
    }

    // Widths
    size_t GetWidthsCount(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_curves) return 0;
        PXR_NS::VtArray<float> widths;
        _curves.GetWidthsAttr().Get(&widths, time.ToUsd());
        return widths.size();
#else
        return _widths.size();
#endif
    }

    size_t GetWidths(TimeCode time, float* outBuffer, size_t maxCount) const SWIFTUSD_NOEXCEPT {
        if (!outBuffer) return 0;
#if defined(USE_PIXAR_USD)
        if (!_curves) return 0;
        PXR_NS::VtArray<float> widths;
        if (!_curves.GetWidthsAttr().Get(&widths, time.ToUsd())) return 0;
        size_t count = std::min(widths.size(), maxCount);
        for (size_t i = 0; i < count; ++i) outBuffer[i] = widths[i];
        return count;
#else
        size_t count = std::min(_widths.size(), maxCount);
        for (size_t i = 0; i < count; ++i) outBuffer[i] = _widths[i];
        return count;
#endif
    }

    bool SetWidths(TimeCode time, const float* widths, size_t count) SWIFTUSD_NOEXCEPT {
        if (!widths && count > 0) return false;
#if defined(USE_PIXAR_USD)
        if (!_curves) return false;
        PXR_NS::VtArray<float> vtWidths(widths, widths + count);
        return _curves.GetWidthsAttr().Set(vtWidths, time.ToUsd());
#else
        _widths.assign(widths, widths + count);
        return true;
#endif
    }

    void retain() SWIFTUSD_NOEXCEPT { _refCount.fetch_add(1, std::memory_order_relaxed); }
    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) delete this;
    }

private:
#if defined(USE_PIXAR_USD)
    GeomBasisCurvesRef(const PXR_NS::UsdGeomBasisCurves& curves, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _curves(curves), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }
    ~GeomBasisCurvesRef() { if (_stage) _stage->release(); }
    PXR_NS::UsdGeomBasisCurves _curves;
#else
    GeomBasisCurvesRef(const Path& path, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }
    ~GeomBasisCurvesRef() { if (_stage) _stage->release(); }
    Path _path;
    CurveType _curveType;
    CurveBasis _basis;
    CurveWrap _wrap;
    std::vector<int> _curveVertexCounts;
    std::vector<float> _points;
    std::vector<float> _widths;
#endif
    StageRef* _stage;
    std::atomic<int> _refCount;
} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - GeomCameraRef (UsdGeomCamera reference type)
// ============================================================================

/// A camera prim.
class GeomCameraRef {
public:
    static GeomCameraRef* Define(StageRef* stage, const Path& path) SWIFTUSD_NOEXCEPT {
        if (!stage) return nullptr;
#if defined(USE_PIXAR_USD)
        auto camera = PXR_NS::UsdGeomCamera::Define(stage->GetUsdStage(), path.GetSdfPath());
        if (!camera) return nullptr;
        return new GeomCameraRef(camera, stage);
#else
        return new GeomCameraRef(path, stage);
#endif
    }

    static GeomCameraRef* FromPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto camera = PXR_NS::UsdGeomCamera(prim->GetUsdPrim());
        if (!camera) return nullptr;
        return new GeomCameraRef(camera, prim->GetStage());
#else
        return new GeomCameraRef(prim->GetPath(), prim->GetStage());
#endif
    }

    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_camera);
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_camera) return nullptr;
        return new PrimRef(_camera.GetPrim(), _stage);
#else
        return _stage ? _stage->GetPrimAtPath(_path) : nullptr;
#endif
    }

    // Focal length
    float GetFocalLength(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_camera) return 50.0f;
        float focalLength = 50.0f;
        _camera.GetFocalLengthAttr().Get(&focalLength, time.ToUsd());
        return focalLength;
#else
        return _focalLength;
#endif
    }

    bool SetFocalLength(TimeCode time, float focalLength) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_camera) return false;
        return _camera.GetFocalLengthAttr().Set(focalLength, time.ToUsd());
#else
        _focalLength = focalLength;
        return true;
#endif
    }

    // Horizontal aperture
    float GetHorizontalAperture(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_camera) return 20.955f;
        float aperture = 20.955f;
        _camera.GetHorizontalApertureAttr().Get(&aperture, time.ToUsd());
        return aperture;
#else
        return _horizontalAperture;
#endif
    }

    bool SetHorizontalAperture(TimeCode time, float aperture) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_camera) return false;
        return _camera.GetHorizontalApertureAttr().Set(aperture, time.ToUsd());
#else
        _horizontalAperture = aperture;
        return true;
#endif
    }

    // Vertical aperture
    float GetVerticalAperture(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_camera) return 15.2908f;
        float aperture = 15.2908f;
        _camera.GetVerticalApertureAttr().Get(&aperture, time.ToUsd());
        return aperture;
#else
        return _verticalAperture;
#endif
    }

    bool SetVerticalAperture(TimeCode time, float aperture) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_camera) return false;
        return _camera.GetVerticalApertureAttr().Set(aperture, time.ToUsd());
#else
        _verticalAperture = aperture;
        return true;
#endif
    }

    // Clipping range
    bool GetClippingRange(TimeCode time, float* outNear, float* outFar) const SWIFTUSD_NOEXCEPT {
        if (!outNear || !outFar) return false;
#if defined(USE_PIXAR_USD)
        if (!_camera) return false;
        PXR_NS::GfVec2f range(1.0f, 1000000.0f);
        _camera.GetClippingRangeAttr().Get(&range, time.ToUsd());
        *outNear = range[0];
        *outFar = range[1];
        return true;
#else
        *outNear = _clippingNear;
        *outFar = _clippingFar;
        return true;
#endif
    }

    bool SetClippingRange(TimeCode time, float nearClip, float farClip) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_camera) return false;
        return _camera.GetClippingRangeAttr().Set(PXR_NS::GfVec2f(nearClip, farClip), time.ToUsd());
#else
        _clippingNear = nearClip;
        _clippingFar = farClip;
        return true;
#endif
    }

    void retain() SWIFTUSD_NOEXCEPT { _refCount.fetch_add(1, std::memory_order_relaxed); }
    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) delete this;
    }

private:
#if defined(USE_PIXAR_USD)
    GeomCameraRef(const PXR_NS::UsdGeomCamera& camera, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _camera(camera), _stage(stage), _refCount(1) {
        if (_stage) _stage->retain();
    }
    ~GeomCameraRef() { if (_stage) _stage->release(); }
    PXR_NS::UsdGeomCamera _camera;
#else
    GeomCameraRef(const Path& path, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path), _stage(stage),
          _focalLength(50.0f), _horizontalAperture(20.955f), _verticalAperture(15.2908f),
          _clippingNear(1.0f), _clippingFar(1000000.0f), _refCount(1) {
        if (_stage) _stage->retain();
    }
    ~GeomCameraRef() { if (_stage) _stage->release(); }
    Path _path;
    float _focalLength;
    float _horizontalAperture;
    float _verticalAperture;
    float _clippingNear;
    float _clippingFar;
#endif
    StageRef* _stage;
    std::atomic<int> _refCount;
} SWIFT_UNSAFE_REFERENCE;

// ============================================================================
// MARK: - Primitive Shapes (Sphere, Cube, Cylinder, Cone, Capsule)
// ============================================================================

/// A sphere primitive.
class GeomSphereRef {
public:
    static GeomSphereRef* Define(StageRef* stage, const Path& path) SWIFTUSD_NOEXCEPT {
        if (!stage) return nullptr;
#if defined(USE_PIXAR_USD)
        auto sphere = PXR_NS::UsdGeomSphere::Define(stage->GetUsdStage(), path.GetSdfPath());
        if (!sphere) return nullptr;
        return new GeomSphereRef(sphere, stage);
#else
        return new GeomSphereRef(path, stage);
#endif
    }

    static GeomSphereRef* FromPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto sphere = PXR_NS::UsdGeomSphere(prim->GetUsdPrim());
        if (!sphere) return nullptr;
        return new GeomSphereRef(sphere, prim->GetStage());
#else
        return new GeomSphereRef(prim->GetPath(), prim->GetStage());
#endif
    }

    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_sphere);
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_sphere) return nullptr;
        return new PrimRef(_sphere.GetPrim(), _stage);
#else
        return _stage ? _stage->GetPrimAtPath(_path) : nullptr;
#endif
    }

    double GetRadius(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_sphere) return 1.0;
        double radius = 1.0;
        _sphere.GetRadiusAttr().Get(&radius, time.ToUsd());
        return radius;
#else
        return _radius;
#endif
    }

    bool SetRadius(TimeCode time, double radius) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_sphere) return false;
        return _sphere.GetRadiusAttr().Set(radius, time.ToUsd());
#else
        _radius = radius;
        return true;
#endif
    }

    void retain() SWIFTUSD_NOEXCEPT { _refCount.fetch_add(1, std::memory_order_relaxed); }
    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) delete this;
    }

private:
#if defined(USE_PIXAR_USD)
    GeomSphereRef(const PXR_NS::UsdGeomSphere& sphere, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _sphere(sphere), _stage(stage), _refCount(1) { if (_stage) _stage->retain(); }
    ~GeomSphereRef() { if (_stage) _stage->release(); }
    PXR_NS::UsdGeomSphere _sphere;
#else
    GeomSphereRef(const Path& path, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path), _stage(stage), _radius(1.0), _refCount(1) { if (_stage) _stage->retain(); }
    ~GeomSphereRef() { if (_stage) _stage->release(); }
    Path _path;
    double _radius;
#endif
    StageRef* _stage;
    std::atomic<int> _refCount;
} SWIFT_UNSAFE_REFERENCE;

/// A cube primitive.
class GeomCubeRef {
public:
    static GeomCubeRef* Define(StageRef* stage, const Path& path) SWIFTUSD_NOEXCEPT {
        if (!stage) return nullptr;
#if defined(USE_PIXAR_USD)
        auto cube = PXR_NS::UsdGeomCube::Define(stage->GetUsdStage(), path.GetSdfPath());
        if (!cube) return nullptr;
        return new GeomCubeRef(cube, stage);
#else
        return new GeomCubeRef(path, stage);
#endif
    }

    static GeomCubeRef* FromPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto cube = PXR_NS::UsdGeomCube(prim->GetUsdPrim());
        if (!cube) return nullptr;
        return new GeomCubeRef(cube, prim->GetStage());
#else
        return new GeomCubeRef(prim->GetPath(), prim->GetStage());
#endif
    }

    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_cube);
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_cube) return nullptr;
        return new PrimRef(_cube.GetPrim(), _stage);
#else
        return _stage ? _stage->GetPrimAtPath(_path) : nullptr;
#endif
    }

    double GetSize(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_cube) return 2.0;
        double size = 2.0;
        _cube.GetSizeAttr().Get(&size, time.ToUsd());
        return size;
#else
        return _size;
#endif
    }

    bool SetSize(TimeCode time, double size) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_cube) return false;
        return _cube.GetSizeAttr().Set(size, time.ToUsd());
#else
        _size = size;
        return true;
#endif
    }

    void retain() SWIFTUSD_NOEXCEPT { _refCount.fetch_add(1, std::memory_order_relaxed); }
    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) delete this;
    }

private:
#if defined(USE_PIXAR_USD)
    GeomCubeRef(const PXR_NS::UsdGeomCube& cube, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _cube(cube), _stage(stage), _refCount(1) { if (_stage) _stage->retain(); }
    ~GeomCubeRef() { if (_stage) _stage->release(); }
    PXR_NS::UsdGeomCube _cube;
#else
    GeomCubeRef(const Path& path, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path), _stage(stage), _size(2.0), _refCount(1) { if (_stage) _stage->retain(); }
    ~GeomCubeRef() { if (_stage) _stage->release(); }
    Path _path;
    double _size;
#endif
    StageRef* _stage;
    std::atomic<int> _refCount;
} SWIFT_UNSAFE_REFERENCE;

/// A cylinder primitive.
class GeomCylinderRef {
public:
    static GeomCylinderRef* Define(StageRef* stage, const Path& path) SWIFTUSD_NOEXCEPT {
        if (!stage) return nullptr;
#if defined(USE_PIXAR_USD)
        auto cyl = PXR_NS::UsdGeomCylinder::Define(stage->GetUsdStage(), path.GetSdfPath());
        if (!cyl) return nullptr;
        return new GeomCylinderRef(cyl, stage);
#else
        return new GeomCylinderRef(path, stage);
#endif
    }

    static GeomCylinderRef* FromPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto cyl = PXR_NS::UsdGeomCylinder(prim->GetUsdPrim());
        if (!cyl) return nullptr;
        return new GeomCylinderRef(cyl, prim->GetStage());
#else
        return new GeomCylinderRef(prim->GetPath(), prim->GetStage());
#endif
    }

    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_cylinder);
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_cylinder) return nullptr;
        return new PrimRef(_cylinder.GetPrim(), _stage);
#else
        return _stage ? _stage->GetPrimAtPath(_path) : nullptr;
#endif
    }

    double GetHeight(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_cylinder) return 2.0;
        double height = 2.0;
        _cylinder.GetHeightAttr().Get(&height, time.ToUsd());
        return height;
#else
        return _height;
#endif
    }

    bool SetHeight(TimeCode time, double height) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_cylinder) return false;
        return _cylinder.GetHeightAttr().Set(height, time.ToUsd());
#else
        _height = height;
        return true;
#endif
    }

    double GetRadius(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_cylinder) return 1.0;
        double radius = 1.0;
        _cylinder.GetRadiusAttr().Get(&radius, time.ToUsd());
        return radius;
#else
        return _radius;
#endif
    }

    bool SetRadius(TimeCode time, double radius) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_cylinder) return false;
        return _cylinder.GetRadiusAttr().Set(radius, time.ToUsd());
#else
        _radius = radius;
        return true;
#endif
    }

    GeomAxis GetAxis() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_cylinder) return GeomAxis();
        PXR_NS::TfToken axis;
        _cylinder.GetAxisAttr().Get(&axis);
        return GeomAxis::FromUsd(axis);
#else
        return _axis;
#endif
    }

    bool SetAxis(GeomAxis axis) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_cylinder) return false;
        return _cylinder.GetAxisAttr().Set(axis.ToUsd());
#else
        _axis = axis;
        return true;
#endif
    }

    void retain() SWIFTUSD_NOEXCEPT { _refCount.fetch_add(1, std::memory_order_relaxed); }
    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) delete this;
    }

private:
#if defined(USE_PIXAR_USD)
    GeomCylinderRef(const PXR_NS::UsdGeomCylinder& cyl, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _cylinder(cyl), _stage(stage), _refCount(1) { if (_stage) _stage->retain(); }
    ~GeomCylinderRef() { if (_stage) _stage->release(); }
    PXR_NS::UsdGeomCylinder _cylinder;
#else
    GeomCylinderRef(const Path& path, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path), _stage(stage), _height(2.0), _radius(1.0), _refCount(1) {
        if (_stage) _stage->retain();
    }
    ~GeomCylinderRef() { if (_stage) _stage->release(); }
    Path _path;
    double _height;
    double _radius;
    GeomAxis _axis;
#endif
    StageRef* _stage;
    std::atomic<int> _refCount;
} SWIFT_UNSAFE_REFERENCE;

/// A cone primitive.
class GeomConeRef {
public:
    static GeomConeRef* Define(StageRef* stage, const Path& path) SWIFTUSD_NOEXCEPT {
        if (!stage) return nullptr;
#if defined(USE_PIXAR_USD)
        auto cone = PXR_NS::UsdGeomCone::Define(stage->GetUsdStage(), path.GetSdfPath());
        if (!cone) return nullptr;
        return new GeomConeRef(cone, stage);
#else
        return new GeomConeRef(path, stage);
#endif
    }

    static GeomConeRef* FromPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto cone = PXR_NS::UsdGeomCone(prim->GetUsdPrim());
        if (!cone) return nullptr;
        return new GeomConeRef(cone, prim->GetStage());
#else
        return new GeomConeRef(prim->GetPath(), prim->GetStage());
#endif
    }

    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_cone);
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_cone) return nullptr;
        return new PrimRef(_cone.GetPrim(), _stage);
#else
        return _stage ? _stage->GetPrimAtPath(_path) : nullptr;
#endif
    }

    double GetHeight(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_cone) return 2.0;
        double height = 2.0;
        _cone.GetHeightAttr().Get(&height, time.ToUsd());
        return height;
#else
        return _height;
#endif
    }

    bool SetHeight(TimeCode time, double height) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_cone) return false;
        return _cone.GetHeightAttr().Set(height, time.ToUsd());
#else
        _height = height;
        return true;
#endif
    }

    double GetRadius(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_cone) return 1.0;
        double radius = 1.0;
        _cone.GetRadiusAttr().Get(&radius, time.ToUsd());
        return radius;
#else
        return _radius;
#endif
    }

    bool SetRadius(TimeCode time, double radius) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_cone) return false;
        return _cone.GetRadiusAttr().Set(radius, time.ToUsd());
#else
        _radius = radius;
        return true;
#endif
    }

    GeomAxis GetAxis() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_cone) return GeomAxis();
        PXR_NS::TfToken axis;
        _cone.GetAxisAttr().Get(&axis);
        return GeomAxis::FromUsd(axis);
#else
        return _axis;
#endif
    }

    bool SetAxis(GeomAxis axis) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_cone) return false;
        return _cone.GetAxisAttr().Set(axis.ToUsd());
#else
        _axis = axis;
        return true;
#endif
    }

    void retain() SWIFTUSD_NOEXCEPT { _refCount.fetch_add(1, std::memory_order_relaxed); }
    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) delete this;
    }

private:
#if defined(USE_PIXAR_USD)
    GeomConeRef(const PXR_NS::UsdGeomCone& cone, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _cone(cone), _stage(stage), _refCount(1) { if (_stage) _stage->retain(); }
    ~GeomConeRef() { if (_stage) _stage->release(); }
    PXR_NS::UsdGeomCone _cone;
#else
    GeomConeRef(const Path& path, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path), _stage(stage), _height(2.0), _radius(1.0), _refCount(1) {
        if (_stage) _stage->retain();
    }
    ~GeomConeRef() { if (_stage) _stage->release(); }
    Path _path;
    double _height;
    double _radius;
    GeomAxis _axis;
#endif
    StageRef* _stage;
    std::atomic<int> _refCount;
} SWIFT_UNSAFE_REFERENCE;

/// A capsule primitive (cylinder with hemispherical caps).
class GeomCapsuleRef {
public:
    static GeomCapsuleRef* Define(StageRef* stage, const Path& path) SWIFTUSD_NOEXCEPT {
        if (!stage) return nullptr;
#if defined(USE_PIXAR_USD)
        auto capsule = PXR_NS::UsdGeomCapsule::Define(stage->GetUsdStage(), path.GetSdfPath());
        if (!capsule) return nullptr;
        return new GeomCapsuleRef(capsule, stage);
#else
        return new GeomCapsuleRef(path, stage);
#endif
    }

    static GeomCapsuleRef* FromPrim(PrimRef* prim) SWIFTUSD_NOEXCEPT {
        if (!prim) return nullptr;
#if defined(USE_PIXAR_USD)
        auto capsule = PXR_NS::UsdGeomCapsule(prim->GetUsdPrim());
        if (!capsule) return nullptr;
        return new GeomCapsuleRef(capsule, prim->GetStage());
#else
        return new GeomCapsuleRef(prim->GetPath(), prim->GetStage());
#endif
    }

    bool IsValid() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        return static_cast<bool>(_capsule);
#else
        return !_path.IsEmpty() && _stage != nullptr;
#endif
    }

    PrimRef* GetPrim() SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_capsule) return nullptr;
        return new PrimRef(_capsule.GetPrim(), _stage);
#else
        return _stage ? _stage->GetPrimAtPath(_path) : nullptr;
#endif
    }

    double GetHeight(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_capsule) return 2.0;
        double height = 2.0;
        _capsule.GetHeightAttr().Get(&height, time.ToUsd());
        return height;
#else
        return _height;
#endif
    }

    bool SetHeight(TimeCode time, double height) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_capsule) return false;
        return _capsule.GetHeightAttr().Set(height, time.ToUsd());
#else
        _height = height;
        return true;
#endif
    }

    double GetRadius(TimeCode time = TimeCode::Default()) const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_capsule) return 0.5;
        double radius = 0.5;
        _capsule.GetRadiusAttr().Get(&radius, time.ToUsd());
        return radius;
#else
        return _radius;
#endif
    }

    bool SetRadius(TimeCode time, double radius) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_capsule) return false;
        return _capsule.GetRadiusAttr().Set(radius, time.ToUsd());
#else
        _radius = radius;
        return true;
#endif
    }

    GeomAxis GetAxis() const SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_capsule) return GeomAxis();
        PXR_NS::TfToken axis;
        _capsule.GetAxisAttr().Get(&axis);
        return GeomAxis::FromUsd(axis);
#else
        return _axis;
#endif
    }

    bool SetAxis(GeomAxis axis) SWIFTUSD_NOEXCEPT {
#if defined(USE_PIXAR_USD)
        if (!_capsule) return false;
        return _capsule.GetAxisAttr().Set(axis.ToUsd());
#else
        _axis = axis;
        return true;
#endif
    }

    void retain() SWIFTUSD_NOEXCEPT { _refCount.fetch_add(1, std::memory_order_relaxed); }
    void release() SWIFTUSD_NOEXCEPT {
        if (_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) delete this;
    }

private:
#if defined(USE_PIXAR_USD)
    GeomCapsuleRef(const PXR_NS::UsdGeomCapsule& capsule, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _capsule(capsule), _stage(stage), _refCount(1) { if (_stage) _stage->retain(); }
    ~GeomCapsuleRef() { if (_stage) _stage->release(); }
    PXR_NS::UsdGeomCapsule _capsule;
#else
    GeomCapsuleRef(const Path& path, StageRef* stage) SWIFTUSD_NOEXCEPT
        : _path(path), _stage(stage), _height(2.0), _radius(0.5), _refCount(1) {
        if (_stage) _stage->retain();
    }
    ~GeomCapsuleRef() { if (_stage) _stage->release(); }
    Path _path;
    double _height;
    double _radius;
    GeomAxis _axis;
#endif
    StageRef* _stage;
    std::atomic<int> _refCount;
} SWIFT_UNSAFE_REFERENCE;

} // namespace swiftusd
