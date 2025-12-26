// Mesh.cpp - UsdGeomMesh implementation
// This file includes real USD headers - NOT visible to Swift

#include "../include/Mesh.hpp"
#include "../include/Stage.hpp"

#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usdGeom/mesh.h>
#include <pxr/usd/usdGeom/primvar.h>
#include <pxr/usd/usdGeom/primvarsAPI.h>
#include <pxr/base/gf/vec2f.h>
#include <pxr/base/gf/vec3f.h>
#include <pxr/base/vt/array.h>

PXR_NAMESPACE_USING_DIRECTIVE

namespace pixarusd {

// ============================================================================
// MARK: - Helper to access PrimImpl
// ============================================================================

namespace detail {
    struct PrimImpl;
}

// We need to access PrimImpl to get the UsdPrim, but it's defined in Stage.cpp
// For now, use the raw pointer API
static UsdPrim GetUsdPrim(Prim* prim) {
    if (!prim) return UsdPrim();
    void* rawPtr = prim->GetRawPrimPtr();
    if (!rawPtr) return UsdPrim();
    return *static_cast<UsdPrim*>(rawPtr);
}

// ============================================================================
// MARK: - Mesh Implementation
// ============================================================================

Mesh::Mesh(Prim* prim) : prim_(prim) {
    if (prim_) {
        retainPrim(prim_);
    }
}

Mesh::~Mesh() {
    if (prim_) {
        releasePrim(prim_);
    }
}

Mesh* Mesh::Get(Prim* prim) {
    if (!prim || !prim->IsValid()) {
        return nullptr;
    }

    UsdPrim usdPrim = GetUsdPrim(prim);
    UsdGeomMesh mesh(usdPrim);
    if (!mesh) {
        return nullptr;
    }

    return new Mesh(prim);
}

bool Mesh::IsValid() const {
    if (!prim_ || !prim_->IsValid()) return false;

    UsdPrim usdPrim = GetUsdPrim(prim_);
    return UsdGeomMesh(usdPrim).operator bool();
}

Prim* Mesh::GetPrim() const {
    if (prim_) {
        retainPrim(prim_);
    }
    return prim_;
}

MeshData Mesh::GetGeometry(TimeCode time) const {
    MeshData data;
    if (!IsValid()) return data;

    UsdPrim usdPrim = GetUsdPrim(prim_);
    UsdGeomMesh mesh(usdPrim);

    // Get points
    VtArray<GfVec3f> points;
    mesh.GetPointsAttr().Get(&points, time.value);
    data.points.reserve(points.size());
    for (const auto& p : points) {
        data.points.push_back(Vec3f{p[0], p[1], p[2]});
    }

    // Get normals
    VtArray<GfVec3f> normals;
    mesh.GetNormalsAttr().Get(&normals, time.value);
    data.normals.reserve(normals.size());
    for (const auto& n : normals) {
        data.normals.push_back(Vec3f{n[0], n[1], n[2]});
    }

    // Get face vertex counts
    VtArray<int> faceVertexCounts;
    mesh.GetFaceVertexCountsAttr().Get(&faceVertexCounts);
    data.faceVertexCounts = std::vector<int32_t>(faceVertexCounts.begin(), faceVertexCounts.end());

    // Get face vertex indices
    VtArray<int> faceVertexIndices;
    mesh.GetFaceVertexIndicesAttr().Get(&faceVertexIndices);
    data.faceVertexIndices = std::vector<int32_t>(faceVertexIndices.begin(), faceVertexIndices.end());

    // Get UVs (st primvar)
    UsdGeomPrimvarsAPI primvarsAPI(usdPrim);
    UsdGeomPrimvar stPrimvar = primvarsAPI.GetPrimvar(TfToken("st"));
    if (stPrimvar) {
        VtArray<GfVec2f> uvs;
        stPrimvar.Get(&uvs, time.value);
        data.uvs.reserve(uvs.size());
        for (const auto& uv : uvs) {
            data.uvs.push_back(Vec2f{uv[0], uv[1]});
        }
    }

    return data;
}

bool Mesh::SetGeometry(const MeshData& data, TimeCode time) {
    if (!IsValid()) return false;

    UsdPrim usdPrim = GetUsdPrim(prim_);
    UsdGeomMesh mesh(usdPrim);

    bool success = true;

    // Set points
    if (!data.points.empty()) {
        VtArray<GfVec3f> points;
        points.reserve(data.points.size());
        for (const auto& p : data.points) {
            points.push_back(GfVec3f(p.x, p.y, p.z));
        }
        success &= mesh.GetPointsAttr().Set(points, time.value);
    }

    // Set normals
    if (!data.normals.empty()) {
        VtArray<GfVec3f> normals;
        normals.reserve(data.normals.size());
        for (const auto& n : data.normals) {
            normals.push_back(GfVec3f(n.x, n.y, n.z));
        }
        success &= mesh.GetNormalsAttr().Set(normals, time.value);
    }

    // Set face vertex counts
    if (!data.faceVertexCounts.empty()) {
        VtArray<int> faceVertexCounts(data.faceVertexCounts.begin(), data.faceVertexCounts.end());
        success &= mesh.GetFaceVertexCountsAttr().Set(faceVertexCounts);
    }

    // Set face vertex indices
    if (!data.faceVertexIndices.empty()) {
        VtArray<int> faceVertexIndices(data.faceVertexIndices.begin(), data.faceVertexIndices.end());
        success &= mesh.GetFaceVertexIndicesAttr().Set(faceVertexIndices);
    }

    // Set UVs
    if (!data.uvs.empty()) {
        VtArray<GfVec2f> uvs;
        uvs.reserve(data.uvs.size());
        for (const auto& uv : data.uvs) {
            uvs.push_back(GfVec2f(uv.x, uv.y));
        }
        UsdGeomPrimvarsAPI primvarsAPI(usdPrim);
        UsdGeomPrimvar stPrimvar = primvarsAPI.CreatePrimvar(
            TfToken("st"),
            SdfValueTypeNames->TexCoord2fArray,
            UsdGeomTokens->faceVarying
        );
        success &= stPrimvar.Set(uvs, time.value);
    }

    return success;
}

std::vector<Vec3f> Mesh::GetPoints(TimeCode time) const {
    if (!IsValid()) return {};

    UsdPrim usdPrim = GetUsdPrim(prim_);
    UsdGeomMesh mesh(usdPrim);

    VtArray<GfVec3f> points;
    mesh.GetPointsAttr().Get(&points, time.value);

    std::vector<Vec3f> result;
    result.reserve(points.size());
    for (const auto& p : points) {
        result.push_back(Vec3f{p[0], p[1], p[2]});
    }
    return result;
}

std::vector<Vec3f> Mesh::GetNormals(TimeCode time) const {
    if (!IsValid()) return {};

    UsdPrim usdPrim = GetUsdPrim(prim_);
    UsdGeomMesh mesh(usdPrim);

    VtArray<GfVec3f> normals;
    mesh.GetNormalsAttr().Get(&normals, time.value);

    std::vector<Vec3f> result;
    result.reserve(normals.size());
    for (const auto& n : normals) {
        result.push_back(Vec3f{n[0], n[1], n[2]});
    }
    return result;
}

std::vector<int32_t> Mesh::GetFaceVertexCounts() const {
    if (!IsValid()) return {};

    UsdPrim usdPrim = GetUsdPrim(prim_);
    UsdGeomMesh mesh(usdPrim);

    VtArray<int> faceVertexCounts;
    mesh.GetFaceVertexCountsAttr().Get(&faceVertexCounts);

    return std::vector<int32_t>(faceVertexCounts.begin(), faceVertexCounts.end());
}

std::vector<int32_t> Mesh::GetFaceVertexIndices() const {
    if (!IsValid()) return {};

    UsdPrim usdPrim = GetUsdPrim(prim_);
    UsdGeomMesh mesh(usdPrim);

    VtArray<int> faceVertexIndices;
    mesh.GetFaceVertexIndicesAttr().Get(&faceVertexIndices);

    return std::vector<int32_t>(faceVertexIndices.begin(), faceVertexIndices.end());
}

std::vector<Vec2f> Mesh::GetUVs(TimeCode time) const {
    if (!IsValid()) return {};

    UsdPrim usdPrim = GetUsdPrim(prim_);
    UsdGeomPrimvarsAPI primvarsAPI(usdPrim);
    UsdGeomPrimvar stPrimvar = primvarsAPI.GetPrimvar(TfToken("st"));
    if (!stPrimvar) return {};

    VtArray<GfVec2f> uvs;
    stPrimvar.Get(&uvs, time.value);

    std::vector<Vec2f> result;
    result.reserve(uvs.size());
    for (const auto& uv : uvs) {
        result.push_back(Vec2f{uv[0], uv[1]});
    }
    return result;
}

bool Mesh::SetPoints(const std::vector<Vec3f>& points, TimeCode time) {
    if (!IsValid()) return false;

    UsdPrim usdPrim = GetUsdPrim(prim_);
    UsdGeomMesh mesh(usdPrim);

    VtArray<GfVec3f> vtPoints;
    vtPoints.reserve(points.size());
    for (const auto& p : points) {
        vtPoints.push_back(GfVec3f(p.x, p.y, p.z));
    }
    return mesh.GetPointsAttr().Set(vtPoints, time.value);
}

bool Mesh::SetNormals(const std::vector<Vec3f>& normals, TimeCode time) {
    if (!IsValid()) return false;

    UsdPrim usdPrim = GetUsdPrim(prim_);
    UsdGeomMesh mesh(usdPrim);

    VtArray<GfVec3f> vtNormals;
    vtNormals.reserve(normals.size());
    for (const auto& n : normals) {
        vtNormals.push_back(GfVec3f(n.x, n.y, n.z));
    }
    return mesh.GetNormalsAttr().Set(vtNormals, time.value);
}

bool Mesh::SetFaceVertexCounts(const std::vector<int32_t>& counts) {
    if (!IsValid()) return false;

    UsdPrim usdPrim = GetUsdPrim(prim_);
    UsdGeomMesh mesh(usdPrim);

    VtArray<int> vtCounts(counts.begin(), counts.end());
    return mesh.GetFaceVertexCountsAttr().Set(vtCounts);
}

bool Mesh::SetFaceVertexIndices(const std::vector<int32_t>& indices) {
    if (!IsValid()) return false;

    UsdPrim usdPrim = GetUsdPrim(prim_);
    UsdGeomMesh mesh(usdPrim);

    VtArray<int> vtIndices(indices.begin(), indices.end());
    return mesh.GetFaceVertexIndicesAttr().Set(vtIndices);
}

bool Mesh::SetUVs(const std::vector<Vec2f>& uvs, TimeCode time) {
    if (!IsValid()) return false;

    UsdPrim usdPrim = GetUsdPrim(prim_);
    UsdGeomPrimvarsAPI primvarsAPI(usdPrim);
    UsdGeomPrimvar stPrimvar = primvarsAPI.CreatePrimvar(
        TfToken("st"),
        SdfValueTypeNames->TexCoord2fArray,
        UsdGeomTokens->faceVarying
    );

    VtArray<GfVec2f> vtUVs;
    vtUVs.reserve(uvs.size());
    for (const auto& uv : uvs) {
        vtUVs.push_back(GfVec2f(uv.x, uv.y));
    }
    return stPrimvar.Set(vtUVs, time.value);
}

BBox3d Mesh::ComputeBoundingBox(TimeCode time) const {
    if (!prim_) return BBox3d{};
    return prim_->ComputeWorldBound(time);
}

size_t Mesh::GetPointCount(TimeCode time) const {
    return GetPoints(time).size();
}

size_t Mesh::GetFaceCount() const {
    return GetFaceVertexCounts().size();
}

} // namespace pixarusd

// Global retain/release functions for Swift interop
void retainMesh(pixarusd::Mesh* m) {
    if (m) {
        m->refCount_.fetch_add(1, std::memory_order_relaxed);
    }
}

void releaseMesh(pixarusd::Mesh* m) {
    if (m && m->refCount_.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete m;
    }
}
