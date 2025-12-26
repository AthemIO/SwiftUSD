// Xform.cpp - UsdGeomXform implementation
// This file includes real USD headers - NOT visible to Swift

#include "../include/Xform.hpp"
#include "../include/Stage.hpp"

#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usdGeom/xform.h>
#include <pxr/usd/usdGeom/xformOp.h>
#include <pxr/usd/usdGeom/xformable.h>
#include <pxr/base/gf/vec3d.h>
#include <pxr/base/gf/quatf.h>
#include <pxr/base/gf/matrix4d.h>

PXR_NAMESPACE_USING_DIRECTIVE

namespace pixarusd {

// ============================================================================
// MARK: - Helper to access PrimImpl
// ============================================================================

static UsdPrim GetUsdPrim(Prim* prim) {
    if (!prim) return UsdPrim();
    void* rawPtr = prim->GetRawPrimPtr();
    if (!rawPtr) return UsdPrim();
    return *static_cast<UsdPrim*>(rawPtr);
}

// ============================================================================
// MARK: - Xform Implementation
// ============================================================================

Xform::Xform(Prim* prim) : prim_(prim) {
    if (prim_) {
        retainPrim(prim_);
    }
}

Xform::~Xform() {
    if (prim_) {
        releasePrim(prim_);
    }
}

Xform* Xform::Get(Prim* prim) {
    if (!prim || !prim->IsValid()) {
        return nullptr;
    }

    UsdPrim usdPrim = GetUsdPrim(prim);
    UsdGeomXformable xformable(usdPrim);
    if (!xformable) {
        return nullptr;
    }

    return new Xform(prim);
}

bool Xform::IsValid() const {
    if (!prim_ || !prim_->IsValid()) return false;

    UsdPrim usdPrim = GetUsdPrim(prim_);
    return UsdGeomXformable(usdPrim).operator bool();
}

Prim* Xform::GetPrim() const {
    if (prim_) {
        retainPrim(prim_);
    }
    return prim_;
}

Matrix4d Xform::GetLocalTransform(TimeCode time) const {
    if (!IsValid()) return Matrix4d{};

    UsdPrim usdPrim = GetUsdPrim(prim_);
    UsdGeomXformable xformable(usdPrim);

    bool resetsXformStack = false;
    GfMatrix4d xform;
    xformable.GetLocalTransformation(&xform, &resetsXformStack, time.value);

    Matrix4d result;
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            result.m[row][col] = xform[row][col];
        }
    }
    return result;
}

Matrix4d Xform::GetWorldTransform(TimeCode time) const {
    if (!IsValid()) return Matrix4d{};

    UsdPrim usdPrim = GetUsdPrim(prim_);
    UsdGeomXformable xformable(usdPrim);

    GfMatrix4d xform = xformable.ComputeLocalToWorldTransform(time.value);

    Matrix4d result;
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            result.m[row][col] = xform[row][col];
        }
    }
    return result;
}

Vec3d Xform::GetTranslation(TimeCode time) const {
    if (!IsValid()) return Vec3d{};

    UsdPrim usdPrim = GetUsdPrim(prim_);
    UsdGeomXformable xformable(usdPrim);

    // Look for a translate op
    std::vector<UsdGeomXformOp> orderedOps;
    bool resetsXformStack = false;
    orderedOps = xformable.GetOrderedXformOps(&resetsXformStack);

    for (const auto& op : orderedOps) {
        if (op.GetOpType() == UsdGeomXformOp::TypeTranslate) {
            GfVec3d value;
            if (op.Get(&value, time.value)) {
                return Vec3d{value[0], value[1], value[2]};
            }
        }
    }

    // Fall back to extracting from matrix
    Matrix4d localTransform = GetLocalTransform(time);
    return Vec3d{localTransform.m[3][0], localTransform.m[3][1], localTransform.m[3][2]};
}

Quatf Xform::GetRotation(TimeCode time) const {
    if (!IsValid()) return Quatf{};

    UsdPrim usdPrim = GetUsdPrim(prim_);
    UsdGeomXformable xformable(usdPrim);

    std::vector<UsdGeomXformOp> orderedOps;
    bool resetsXformStack = false;
    orderedOps = xformable.GetOrderedXformOps(&resetsXformStack);

    for (const auto& op : orderedOps) {
        if (op.GetOpType() == UsdGeomXformOp::TypeOrient) {
            GfQuatf value;
            if (op.Get(&value, time.value)) {
                return Quatf{
                    value.GetReal(),
                    value.GetImaginary()[0],
                    value.GetImaginary()[1],
                    value.GetImaginary()[2]
                };
            }
        }
    }

    return Quatf::Identity();
}

Vec3d Xform::GetScale(TimeCode time) const {
    if (!IsValid()) return Vec3d{1, 1, 1};

    UsdPrim usdPrim = GetUsdPrim(prim_);
    UsdGeomXformable xformable(usdPrim);

    std::vector<UsdGeomXformOp> orderedOps;
    bool resetsXformStack = false;
    orderedOps = xformable.GetOrderedXformOps(&resetsXformStack);

    for (const auto& op : orderedOps) {
        if (op.GetOpType() == UsdGeomXformOp::TypeScale) {
            GfVec3d value;
            if (op.Get(&value, time.value)) {
                return Vec3d{value[0], value[1], value[2]};
            }
        }
    }

    return Vec3d{1, 1, 1};
}

bool Xform::SetTranslation(const Vec3d& translation, TimeCode time) {
    if (!IsValid()) return false;

    UsdPrim usdPrim = GetUsdPrim(prim_);
    UsdGeomXformable xformable(usdPrim);

    // Look for existing translate op
    std::vector<UsdGeomXformOp> orderedOps;
    bool resetsXformStack = false;
    orderedOps = xformable.GetOrderedXformOps(&resetsXformStack);

    for (auto& op : orderedOps) {
        if (op.GetOpType() == UsdGeomXformOp::TypeTranslate) {
            return op.Set(GfVec3d(translation.x, translation.y, translation.z), time.value);
        }
    }

    // Create new translate op
    UsdGeomXformOp translateOp = xformable.AddTranslateOp();
    return translateOp.Set(GfVec3d(translation.x, translation.y, translation.z), time.value);
}

bool Xform::SetRotation(const Quatf& rotation, TimeCode time) {
    if (!IsValid()) return false;

    UsdPrim usdPrim = GetUsdPrim(prim_);
    UsdGeomXformable xformable(usdPrim);

    std::vector<UsdGeomXformOp> orderedOps;
    bool resetsXformStack = false;
    orderedOps = xformable.GetOrderedXformOps(&resetsXformStack);

    for (auto& op : orderedOps) {
        if (op.GetOpType() == UsdGeomXformOp::TypeOrient) {
            GfQuatf quat(rotation.w, GfVec3f(rotation.x, rotation.y, rotation.z));
            return op.Set(quat, time.value);
        }
    }

    // Create new orient op
    UsdGeomXformOp orientOp = xformable.AddOrientOp();
    GfQuatf quat(rotation.w, GfVec3f(rotation.x, rotation.y, rotation.z));
    return orientOp.Set(quat, time.value);
}

bool Xform::SetScale(const Vec3d& scale, TimeCode time) {
    if (!IsValid()) return false;

    UsdPrim usdPrim = GetUsdPrim(prim_);
    UsdGeomXformable xformable(usdPrim);

    std::vector<UsdGeomXformOp> orderedOps;
    bool resetsXformStack = false;
    orderedOps = xformable.GetOrderedXformOps(&resetsXformStack);

    for (auto& op : orderedOps) {
        if (op.GetOpType() == UsdGeomXformOp::TypeScale) {
            return op.Set(GfVec3d(scale.x, scale.y, scale.z), time.value);
        }
    }

    // Create new scale op
    UsdGeomXformOp scaleOp = xformable.AddScaleOp();
    return scaleOp.Set(GfVec3d(scale.x, scale.y, scale.z), time.value);
}

bool Xform::SetLocalTransform(const Matrix4d& matrix, TimeCode time) {
    if (!IsValid()) return false;

    UsdPrim usdPrim = GetUsdPrim(prim_);
    UsdGeomXformable xformable(usdPrim);

    // Convert to GfMatrix4d
    GfMatrix4d gfMatrix;
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            gfMatrix[row][col] = matrix.m[row][col];
        }
    }

    // Look for existing transform op
    std::vector<UsdGeomXformOp> orderedOps;
    bool resetsXformStack = false;
    orderedOps = xformable.GetOrderedXformOps(&resetsXformStack);

    for (auto& op : orderedOps) {
        if (op.GetOpType() == UsdGeomXformOp::TypeTransform) {
            return op.Set(gfMatrix, time.value);
        }
    }

    // Create new transform op
    UsdGeomXformOp transformOp = xformable.AddTransformOp();
    return transformOp.Set(gfMatrix, time.value);
}

bool Xform::ClearXformOpOrder() {
    if (!IsValid()) return false;

    UsdPrim usdPrim = GetUsdPrim(prim_);
    UsdGeomXformable xformable(usdPrim);

    return xformable.ClearXformOpOrder();
}

bool Xform::ResetToIdentity() {
    if (!IsValid()) return false;

    UsdPrim usdPrim = GetUsdPrim(prim_);
    UsdGeomXformable xformable(usdPrim);

    // Clear existing ops
    xformable.ClearXformOpOrder();

    // Add identity transform
    UsdGeomXformOp transformOp = xformable.AddTransformOp();
    return transformOp.Set(GfMatrix4d(1.0));
}

} // namespace pixarusd

// Global retain/release functions for Swift interop
void retainXform(pixarusd::Xform* x) {
    if (x) {
        x->refCount_.fetch_add(1, std::memory_order_relaxed);
    }
}

void releaseXform(pixarusd::Xform* x) {
    if (x && x->refCount_.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete x;
    }
}
