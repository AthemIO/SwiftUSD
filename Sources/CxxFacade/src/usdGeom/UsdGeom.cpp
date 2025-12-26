// UsdGeom.cpp - Geometry Schemas implementation
#include "../../include/usdGeom/UsdGeom.hpp"

// Pixar USD includes - ONLY in .cpp files!
#include <pxr/usd/usdGeom/imageable.h>
#include <pxr/usd/usdGeom/xformable.h>
#include <pxr/usd/usdGeom/xform.h>
#include <pxr/usd/usdGeom/scope.h>
#include <pxr/usd/usdGeom/boundable.h>
#include <pxr/usd/usdGeom/gprim.h>
#include <pxr/usd/usdGeom/pointBased.h>
#include <pxr/usd/usdGeom/mesh.h>
#include <pxr/usd/usdGeom/points.h>
#include <pxr/usd/usdGeom/curves.h>
#include <pxr/usd/usdGeom/basisCurves.h>
#include <pxr/usd/usdGeom/nurbsCurves.h>
#include <pxr/usd/usdGeom/camera.h>
#include <pxr/usd/usdGeom/cube.h>
#include <pxr/usd/usdGeom/sphere.h>
#include <pxr/usd/usdGeom/cylinder.h>
#include <pxr/usd/usdGeom/capsule.h>
#include <pxr/usd/usdGeom/cone.h>
#include <pxr/usd/usdGeom/xformOp.h>
#include <pxr/usd/usdGeom/primvar.h>
#include <pxr/usd/usdGeom/primvarsAPI.h>
#include <pxr/usd/usdGeom/tokens.h>
#include <pxr/usd/usdGeom/bboxCache.h>
#include <pxr/base/gf/bbox3d.h>
#include <pxr/base/gf/frustum.h>
#include <pxr/base/tf/token.h>
#include <cstring>

PXR_NAMESPACE_USING_DIRECTIVE

namespace cxxfacade {
namespace usdGeom {

// ============================================================================
// Helper Functions
// ============================================================================

static inline pxr::UsdTimeCode toPxrTimeCode(const usd::UsdTimeCode& tc) {
    if (tc.IsDefault()) {
        return pxr::UsdTimeCode::Default();
    }
    return pxr::UsdTimeCode(tc.GetValue());
}

static inline pxr::UsdGeomXformOp::Precision toPxrPrecision(UsdGeomXformOpPrecision p) {
    switch (p) {
        case UsdGeomXformOpPrecision::PrecisionDouble:
            return pxr::UsdGeomXformOp::PrecisionDouble;
        case UsdGeomXformOpPrecision::PrecisionFloat:
            return pxr::UsdGeomXformOp::PrecisionFloat;
        case UsdGeomXformOpPrecision::PrecisionHalf:
            return pxr::UsdGeomXformOp::PrecisionHalf;
    }
    return pxr::UsdGeomXformOp::PrecisionDouble;
}

static inline UsdGeomXformOpType fromPxrOpType(pxr::UsdGeomXformOp::Type t) {
    switch (t) {
        case pxr::UsdGeomXformOp::TypeTranslate: return UsdGeomXformOpType::TypeTranslate;
        case pxr::UsdGeomXformOp::TypeScale: return UsdGeomXformOpType::TypeScale;
        case pxr::UsdGeomXformOp::TypeRotateX: return UsdGeomXformOpType::TypeRotateX;
        case pxr::UsdGeomXformOp::TypeRotateY: return UsdGeomXformOpType::TypeRotateY;
        case pxr::UsdGeomXformOp::TypeRotateZ: return UsdGeomXformOpType::TypeRotateZ;
        case pxr::UsdGeomXformOp::TypeRotateXYZ: return UsdGeomXformOpType::TypeRotateXYZ;
        case pxr::UsdGeomXformOp::TypeRotateXZY: return UsdGeomXformOpType::TypeRotateXZY;
        case pxr::UsdGeomXformOp::TypeRotateYXZ: return UsdGeomXformOpType::TypeRotateYXZ;
        case pxr::UsdGeomXformOp::TypeRotateYZX: return UsdGeomXformOpType::TypeRotateYZX;
        case pxr::UsdGeomXformOp::TypeRotateZXY: return UsdGeomXformOpType::TypeRotateZXY;
        case pxr::UsdGeomXformOp::TypeRotateZYX: return UsdGeomXformOpType::TypeRotateZYX;
        case pxr::UsdGeomXformOp::TypeOrient: return UsdGeomXformOpType::TypeOrient;
        case pxr::UsdGeomXformOp::TypeTransform: return UsdGeomXformOpType::TypeTransform;
        default: return UsdGeomXformOpType::TypeInvalid;
    }
}

static inline UsdGeomXformOpPrecision fromPxrPrecision(pxr::UsdGeomXformOp::Precision p) {
    switch (p) {
        case pxr::UsdGeomXformOp::PrecisionDouble: return UsdGeomXformOpPrecision::PrecisionDouble;
        case pxr::UsdGeomXformOp::PrecisionFloat: return UsdGeomXformOpPrecision::PrecisionFloat;
        case pxr::UsdGeomXformOp::PrecisionHalf: return UsdGeomXformOpPrecision::PrecisionHalf;
        default: return UsdGeomXformOpPrecision::PrecisionDouble;
    }
}

// Helper to get pxr::UsdPrim from our wrapper
static inline pxr::UsdPrim* asPrim(void* impl) {
    return static_cast<pxr::UsdPrim*>(impl);
}

// ============================================================================
// UsdGeomXformOp implementation
// ============================================================================

static inline pxr::UsdGeomXformOp* asXformOp(void* impl) {
    return static_cast<pxr::UsdGeomXformOp*>(impl);
}

UsdGeomXformOp::UsdGeomXformOp() : impl_(new pxr::UsdGeomXformOp()) {}

UsdGeomXformOp::UsdGeomXformOp(const UsdGeomXformOp& other)
    : impl_(new pxr::UsdGeomXformOp(*asXformOp(other.impl_))) {}

UsdGeomXformOp::UsdGeomXformOp(UsdGeomXformOp&& other) noexcept : impl_(other.impl_) {
    other.impl_ = nullptr;
}

UsdGeomXformOp::~UsdGeomXformOp() {
    delete asXformOp(impl_);
}

UsdGeomXformOp& UsdGeomXformOp::operator=(const UsdGeomXformOp& other) {
    if (this != &other) {
        delete asXformOp(impl_);
        impl_ = new pxr::UsdGeomXformOp(*asXformOp(other.impl_));
    }
    return *this;
}

UsdGeomXformOp& UsdGeomXformOp::operator=(UsdGeomXformOp&& other) noexcept {
    if (this != &other) {
        delete asXformOp(impl_);
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

bool UsdGeomXformOp::IsValid() const {
    return impl_ && asXformOp(impl_)->IsDefined();
}

UsdGeomXformOpType UsdGeomXformOp::GetOpType() const {
    if (!IsValid()) return UsdGeomXformOpType::TypeInvalid;
    return fromPxrOpType(asXformOp(impl_)->GetOpType());
}

UsdGeomXformOpPrecision UsdGeomXformOp::GetPrecision() const {
    if (!IsValid()) return UsdGeomXformOpPrecision::PrecisionDouble;
    return fromPxrPrecision(asXformOp(impl_)->GetPrecision());
}

tf::TfToken UsdGeomXformOp::GetOpName() const {
    if (!IsValid()) return tf::TfToken();
    return tf::TfToken::FromImpl(new pxr::TfToken(asXformOp(impl_)->GetOpName()));
}

usd::UsdAttribute UsdGeomXformOp::GetAttr() const {
    if (!IsValid()) return usd::UsdAttribute();
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(asXformOp(impl_)->GetAttr()));
}

bool UsdGeomXformOp::IsInverseOp() const {
    return IsValid() && asXformOp(impl_)->IsInverseOp();
}

bool UsdGeomXformOp::GetOpTransform(gf::Matrix4d* transform, usd::UsdTimeCode time) const {
    if (!IsValid() || !transform) return false;
    pxr::GfMatrix4d mat = asXformOp(impl_)->GetOpTransform(toPxrTimeCode(time));
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            transform->data[r][c] = mat[r][c];
        }
    }
    return true;
}

UsdGeomXformOp UsdGeomXformOp::FromImpl(void* impl) {
    UsdGeomXformOp op;
    delete asXformOp(op.impl_);
    op.impl_ = impl;
    return op;
}

// ============================================================================
// UsdGeomImageable implementation
// ============================================================================

UsdGeomImageable::UsdGeomImageable(const usd::UsdPrim& prim) : UsdTyped(prim) {}

UsdGeomImageable::UsdGeomImageable(const usd::UsdSchemaBase& schemaObj) : UsdTyped(schemaObj) {}

UsdGeomImageable::~UsdGeomImageable() {}

UsdGeomImageable UsdGeomImageable::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    return UsdGeomImageable(stage.GetPrimAtPath(path));
}

usd::UsdAttribute UsdGeomImageable::GetVisibilityAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomImageable imageable(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(imageable.GetVisibilityAttr()));
}

usd::UsdAttribute UsdGeomImageable::CreateVisibilityAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomImageable imageable(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(imageable.CreateVisibilityAttr()));
}

tf::TfToken UsdGeomImageable::ComputeVisibility(usd::UsdTimeCode time) const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return tf::TfToken();
    pxr::UsdGeomImageable imageable(*asPrim(prim.GetImpl()));
    return tf::TfToken::FromImpl(new pxr::TfToken(imageable.ComputeVisibility(toPxrTimeCode(time))));
}

void UsdGeomImageable::MakeInvisible(usd::UsdTimeCode time) {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return;
    pxr::UsdGeomImageable imageable(*asPrim(prim.GetImpl()));
    imageable.MakeInvisible(toPxrTimeCode(time));
}

void UsdGeomImageable::MakeVisible(usd::UsdTimeCode time) {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return;
    pxr::UsdGeomImageable imageable(*asPrim(prim.GetImpl()));
    imageable.MakeVisible(toPxrTimeCode(time));
}

usd::UsdAttribute UsdGeomImageable::GetPurposeAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomImageable imageable(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(imageable.GetPurposeAttr()));
}

usd::UsdAttribute UsdGeomImageable::CreatePurposeAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomImageable imageable(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(imageable.CreatePurposeAttr()));
}

tf::TfToken UsdGeomImageable::ComputePurpose() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return tf::TfToken();
    pxr::UsdGeomImageable imageable(*asPrim(prim.GetImpl()));
    pxr::TfToken purposeInfo = imageable.ComputePurpose();
    return tf::TfToken::FromImpl(new pxr::TfToken(purposeInfo));
}

usd::UsdRelationship UsdGeomImageable::GetProxyPrimRel() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdRelationship();
    pxr::UsdGeomImageable imageable(*asPrim(prim.GetImpl()));
    return usd::UsdRelationship::FromImpl(new pxr::UsdRelationship(imageable.GetProxyPrimRel()));
}

usd::UsdRelationship UsdGeomImageable::CreateProxyPrimRel() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdRelationship();
    pxr::UsdGeomImageable imageable(*asPrim(prim.GetImpl()));
    return usd::UsdRelationship::FromImpl(new pxr::UsdRelationship(imageable.CreateProxyPrimRel()));
}

gf::BBox3d UsdGeomImageable::ComputeLocalBound(usd::UsdTimeCode time,
                                                const tf::TfToken& purpose1,
                                                const tf::TfToken& purpose2,
                                                const tf::TfToken& purpose3,
                                                const tf::TfToken& purpose4) const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return gf::BBox3d();

    // Build purposes array
    pxr::TfTokenVector purposes;
    if (strlen(purpose1.GetText()) > 0) {
        purposes.push_back(*static_cast<pxr::TfToken*>(purpose1.GetImpl()));
    }
    if (strlen(purpose2.GetText()) > 0) {
        purposes.push_back(*static_cast<pxr::TfToken*>(purpose2.GetImpl()));
    }
    if (strlen(purpose3.GetText()) > 0) {
        purposes.push_back(*static_cast<pxr::TfToken*>(purpose3.GetImpl()));
    }
    if (strlen(purpose4.GetText()) > 0) {
        purposes.push_back(*static_cast<pxr::TfToken*>(purpose4.GetImpl()));
    }

    if (purposes.empty()) {
        purposes = { pxr::UsdGeomTokens->default_, pxr::UsdGeomTokens->render };
    }

    pxr::UsdGeomImageable imageable(*asPrim(prim.GetImpl()));
    pxr::GfBBox3d pxrBbox = imageable.ComputeLocalBound(toPxrTimeCode(time), purposes[0]);

    gf::Range3d range(
        gf::Vec3d(pxrBbox.GetRange().GetMin()[0], pxrBbox.GetRange().GetMin()[1], pxrBbox.GetRange().GetMin()[2]),
        gf::Vec3d(pxrBbox.GetRange().GetMax()[0], pxrBbox.GetRange().GetMax()[1], pxrBbox.GetRange().GetMax()[2])
    );

    gf::Matrix4d matrix;
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            matrix.data[r][c] = pxrBbox.GetMatrix()[r][c];
        }
    }

    return gf::BBox3d(range, matrix);
}

gf::BBox3d UsdGeomImageable::ComputeWorldBound(usd::UsdTimeCode time,
                                                const tf::TfToken& purpose1,
                                                const tf::TfToken& purpose2,
                                                const tf::TfToken& purpose3,
                                                const tf::TfToken& purpose4) const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return gf::BBox3d();

    // Build purposes array
    pxr::TfTokenVector purposes;
    if (strlen(purpose1.GetText()) > 0) {
        purposes.push_back(*static_cast<pxr::TfToken*>(purpose1.GetImpl()));
    }
    if (purposes.empty()) {
        purposes = { pxr::UsdGeomTokens->default_, pxr::UsdGeomTokens->render };
    }

    pxr::UsdGeomImageable imageable(*asPrim(prim.GetImpl()));
    pxr::GfBBox3d pxrBbox = imageable.ComputeWorldBound(toPxrTimeCode(time), purposes[0]);

    gf::Range3d range(
        gf::Vec3d(pxrBbox.GetRange().GetMin()[0], pxrBbox.GetRange().GetMin()[1], pxrBbox.GetRange().GetMin()[2]),
        gf::Vec3d(pxrBbox.GetRange().GetMax()[0], pxrBbox.GetRange().GetMax()[1], pxrBbox.GetRange().GetMax()[2])
    );

    gf::Matrix4d matrix;
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            matrix.data[r][c] = pxrBbox.GetMatrix()[r][c];
        }
    }

    return gf::BBox3d(range, matrix);
}

usd::UsdSchemaKind UsdGeomImageable::_GetSchemaKind() const {
    return usd::UsdSchemaKind::AbstractTyped;
}

// ============================================================================
// UsdGeomXformable implementation
// ============================================================================

UsdGeomXformable::UsdGeomXformable(const usd::UsdPrim& prim) : UsdGeomImageable(prim) {}

UsdGeomXformable::UsdGeomXformable(const usd::UsdSchemaBase& schemaObj) : UsdGeomImageable(schemaObj) {}

UsdGeomXformable::~UsdGeomXformable() {}

UsdGeomXformable UsdGeomXformable::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    return UsdGeomXformable(stage.GetPrimAtPath(path));
}

UsdGeomXformOp UsdGeomXformable::AddTranslateOp(UsdGeomXformOpPrecision precision,
                                                  const tf::TfToken& opSuffix,
                                                  bool isInverseOp) {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdGeomXformOp();
    pxr::UsdGeomXformable xformable(*asPrim(prim.GetImpl()));
    pxr::TfToken suffix = strlen(opSuffix.GetText()) > 0 ? *static_cast<pxr::TfToken*>(opSuffix.GetImpl()) : pxr::TfToken();
    return UsdGeomXformOp::FromImpl(new pxr::UsdGeomXformOp(
        xformable.AddTranslateOp(toPxrPrecision(precision), suffix, isInverseOp)));
}

UsdGeomXformOp UsdGeomXformable::AddScaleOp(UsdGeomXformOpPrecision precision,
                                              const tf::TfToken& opSuffix,
                                              bool isInverseOp) {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdGeomXformOp();
    pxr::UsdGeomXformable xformable(*asPrim(prim.GetImpl()));
    pxr::TfToken suffix = strlen(opSuffix.GetText()) > 0 ? *static_cast<pxr::TfToken*>(opSuffix.GetImpl()) : pxr::TfToken();
    return UsdGeomXformOp::FromImpl(new pxr::UsdGeomXformOp(
        xformable.AddScaleOp(toPxrPrecision(precision), suffix, isInverseOp)));
}

UsdGeomXformOp UsdGeomXformable::AddRotateXOp(UsdGeomXformOpPrecision precision,
                                                const tf::TfToken& opSuffix,
                                                bool isInverseOp) {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdGeomXformOp();
    pxr::UsdGeomXformable xformable(*asPrim(prim.GetImpl()));
    pxr::TfToken suffix = strlen(opSuffix.GetText()) > 0 ? *static_cast<pxr::TfToken*>(opSuffix.GetImpl()) : pxr::TfToken();
    return UsdGeomXformOp::FromImpl(new pxr::UsdGeomXformOp(
        xformable.AddRotateXOp(toPxrPrecision(precision), suffix, isInverseOp)));
}

UsdGeomXformOp UsdGeomXformable::AddRotateYOp(UsdGeomXformOpPrecision precision,
                                                const tf::TfToken& opSuffix,
                                                bool isInverseOp) {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdGeomXformOp();
    pxr::UsdGeomXformable xformable(*asPrim(prim.GetImpl()));
    pxr::TfToken suffix = strlen(opSuffix.GetText()) > 0 ? *static_cast<pxr::TfToken*>(opSuffix.GetImpl()) : pxr::TfToken();
    return UsdGeomXformOp::FromImpl(new pxr::UsdGeomXformOp(
        xformable.AddRotateYOp(toPxrPrecision(precision), suffix, isInverseOp)));
}

UsdGeomXformOp UsdGeomXformable::AddRotateZOp(UsdGeomXformOpPrecision precision,
                                                const tf::TfToken& opSuffix,
                                                bool isInverseOp) {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdGeomXformOp();
    pxr::UsdGeomXformable xformable(*asPrim(prim.GetImpl()));
    pxr::TfToken suffix = strlen(opSuffix.GetText()) > 0 ? *static_cast<pxr::TfToken*>(opSuffix.GetImpl()) : pxr::TfToken();
    return UsdGeomXformOp::FromImpl(new pxr::UsdGeomXformOp(
        xformable.AddRotateZOp(toPxrPrecision(precision), suffix, isInverseOp)));
}

UsdGeomXformOp UsdGeomXformable::AddRotateXYZOp(UsdGeomXformOpPrecision precision,
                                                  const tf::TfToken& opSuffix,
                                                  bool isInverseOp) {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdGeomXformOp();
    pxr::UsdGeomXformable xformable(*asPrim(prim.GetImpl()));
    pxr::TfToken suffix = strlen(opSuffix.GetText()) > 0 ? *static_cast<pxr::TfToken*>(opSuffix.GetImpl()) : pxr::TfToken();
    return UsdGeomXformOp::FromImpl(new pxr::UsdGeomXformOp(
        xformable.AddRotateXYZOp(toPxrPrecision(precision), suffix, isInverseOp)));
}

UsdGeomXformOp UsdGeomXformable::AddOrientOp(UsdGeomXformOpPrecision precision,
                                               const tf::TfToken& opSuffix,
                                               bool isInverseOp) {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdGeomXformOp();
    pxr::UsdGeomXformable xformable(*asPrim(prim.GetImpl()));
    pxr::TfToken suffix = strlen(opSuffix.GetText()) > 0 ? *static_cast<pxr::TfToken*>(opSuffix.GetImpl()) : pxr::TfToken();
    return UsdGeomXformOp::FromImpl(new pxr::UsdGeomXformOp(
        xformable.AddOrientOp(toPxrPrecision(precision), suffix, isInverseOp)));
}

UsdGeomXformOp UsdGeomXformable::AddTransformOp(UsdGeomXformOpPrecision precision,
                                                  const tf::TfToken& opSuffix,
                                                  bool isInverseOp) {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdGeomXformOp();
    pxr::UsdGeomXformable xformable(*asPrim(prim.GetImpl()));
    pxr::TfToken suffix = strlen(opSuffix.GetText()) > 0 ? *static_cast<pxr::TfToken*>(opSuffix.GetImpl()) : pxr::TfToken();
    return UsdGeomXformOp::FromImpl(new pxr::UsdGeomXformOp(
        xformable.AddTransformOp(toPxrPrecision(precision), suffix, isInverseOp)));
}

std::vector<UsdGeomXformOp> UsdGeomXformable::GetOrderedXformOps(bool* resetsXformStack) const {
    std::vector<UsdGeomXformOp> result;
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return result;

    bool resets = false;
    pxr::UsdGeomXformable xformable(*asPrim(prim.GetImpl()));
    auto pxrOps = xformable.GetOrderedXformOps(&resets);

    for (const auto& op : pxrOps) {
        result.push_back(UsdGeomXformOp::FromImpl(new pxr::UsdGeomXformOp(op)));
    }

    if (resetsXformStack) {
        *resetsXformStack = resets;
    }

    return result;
}

bool UsdGeomXformable::SetResetXformStack(bool resetXform) {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return false;
    pxr::UsdGeomXformable xformable(*asPrim(prim.GetImpl()));
    return xformable.SetResetXformStack(resetXform);
}

bool UsdGeomXformable::GetResetXformStack() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return false;
    pxr::UsdGeomXformable xformable(*asPrim(prim.GetImpl()));
    return xformable.GetResetXformStack();
}

bool UsdGeomXformable::ClearXformOpOrder() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return false;
    pxr::UsdGeomXformable xformable(*asPrim(prim.GetImpl()));
    return xformable.ClearXformOpOrder();
}

UsdGeomXformOp UsdGeomXformable::MakeMatrixXform() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdGeomXformOp();
    pxr::UsdGeomXformable xformable(*asPrim(prim.GetImpl()));
    return UsdGeomXformOp::FromImpl(new pxr::UsdGeomXformOp(xformable.MakeMatrixXform()));
}

bool UsdGeomXformable::GetLocalTransformation(gf::Matrix4d* transform,
                                               bool* resetsXformStack,
                                               usd::UsdTimeCode time) const {
    if (!transform) return false;
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return false;

    bool resets = false;
    pxr::GfMatrix4d pxrMatrix;
    pxr::UsdGeomXformable xformable(*asPrim(prim.GetImpl()));
    bool result = xformable.GetLocalTransformation(&pxrMatrix, &resets, toPxrTimeCode(time));

    if (result) {
        for (int r = 0; r < 4; ++r) {
            for (int c = 0; c < 4; ++c) {
                transform->data[r][c] = pxrMatrix[r][c];
            }
        }
    }

    if (resetsXformStack) {
        *resetsXformStack = resets;
    }

    return result;
}

bool UsdGeomXformable::TransformMightBeTimeVarying() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return false;
    pxr::UsdGeomXformable xformable(*asPrim(prim.GetImpl()));
    return xformable.TransformMightBeTimeVarying();
}

usd::UsdSchemaKind UsdGeomXformable::_GetSchemaKind() const {
    return usd::UsdSchemaKind::AbstractTyped;
}

// ============================================================================
// UsdGeomXform implementation
// ============================================================================

UsdGeomXform::UsdGeomXform(const usd::UsdPrim& prim) : UsdGeomXformable(prim) {}

UsdGeomXform::UsdGeomXform(const usd::UsdSchemaBase& schemaObj) : UsdGeomXformable(schemaObj) {}

UsdGeomXform::~UsdGeomXform() {}

UsdGeomXform UsdGeomXform::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    return UsdGeomXform(stage.GetPrimAtPath(path));
}

UsdGeomXform UsdGeomXform::Define(usd::UsdStage& stage, const sdf::SdfPath& path) {
    usd::UsdPrim prim = stage.DefinePrim(path, tf::TfToken::FromImpl(new pxr::TfToken("Xform")));
    return UsdGeomXform(prim);
}

usd::UsdSchemaKind UsdGeomXform::_GetSchemaKind() const {
    return usd::UsdSchemaKind::ConcreteTyped;
}

// ============================================================================
// UsdGeomScope implementation
// ============================================================================

UsdGeomScope::UsdGeomScope(const usd::UsdPrim& prim) : UsdGeomImageable(prim) {}

UsdGeomScope::UsdGeomScope(const usd::UsdSchemaBase& schemaObj) : UsdGeomImageable(schemaObj) {}

UsdGeomScope::~UsdGeomScope() {}

UsdGeomScope UsdGeomScope::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    return UsdGeomScope(stage.GetPrimAtPath(path));
}

UsdGeomScope UsdGeomScope::Define(usd::UsdStage& stage, const sdf::SdfPath& path) {
    usd::UsdPrim prim = stage.DefinePrim(path, tf::TfToken::FromImpl(new pxr::TfToken("Scope")));
    return UsdGeomScope(prim);
}

usd::UsdSchemaKind UsdGeomScope::_GetSchemaKind() const {
    return usd::UsdSchemaKind::ConcreteTyped;
}

// ============================================================================
// UsdGeomBoundable implementation
// ============================================================================

UsdGeomBoundable::UsdGeomBoundable(const usd::UsdPrim& prim) : UsdGeomXformable(prim) {}

UsdGeomBoundable::UsdGeomBoundable(const usd::UsdSchemaBase& schemaObj) : UsdGeomXformable(schemaObj) {}

UsdGeomBoundable::~UsdGeomBoundable() {}

UsdGeomBoundable UsdGeomBoundable::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    return UsdGeomBoundable(stage.GetPrimAtPath(path));
}

usd::UsdAttribute UsdGeomBoundable::GetExtentAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomBoundable boundable(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(boundable.GetExtentAttr()));
}

usd::UsdAttribute UsdGeomBoundable::CreateExtentAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomBoundable boundable(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(boundable.CreateExtentAttr()));
}

bool UsdGeomBoundable::ComputeExtentFromPlugins(const UsdGeomBoundable& boundable,
                                                  usd::UsdTimeCode time,
                                                  std::vector<gf::Vec3f>* extent) {
    if (!extent) return false;
    usd::UsdPrim prim = boundable.GetPrim();
    if (!prim.IsValid()) return false;

    pxr::VtVec3fArray pxrExtent;
    pxr::UsdGeomBoundable pxrBoundable(*asPrim(prim.GetImpl()));
    bool result = pxr::UsdGeomBoundable::ComputeExtentFromPlugins(pxrBoundable, toPxrTimeCode(time), &pxrExtent);

    if (result) {
        extent->clear();
        for (const auto& v : pxrExtent) {
            extent->push_back(gf::Vec3f(v[0], v[1], v[2]));
        }
    }

    return result;
}

usd::UsdSchemaKind UsdGeomBoundable::_GetSchemaKind() const {
    return usd::UsdSchemaKind::AbstractTyped;
}

// ============================================================================
// UsdGeomGprim implementation
// ============================================================================

UsdGeomGprim::UsdGeomGprim(const usd::UsdPrim& prim) : UsdGeomBoundable(prim) {}

UsdGeomGprim::UsdGeomGprim(const usd::UsdSchemaBase& schemaObj) : UsdGeomBoundable(schemaObj) {}

UsdGeomGprim::~UsdGeomGprim() {}

UsdGeomGprim UsdGeomGprim::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    return UsdGeomGprim(stage.GetPrimAtPath(path));
}

usd::UsdAttribute UsdGeomGprim::GetDoubleSidedAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomGprim gprim(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(gprim.GetDoubleSidedAttr()));
}

usd::UsdAttribute UsdGeomGprim::CreateDoubleSidedAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomGprim gprim(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(gprim.CreateDoubleSidedAttr()));
}

usd::UsdAttribute UsdGeomGprim::GetOrientationAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomGprim gprim(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(gprim.GetOrientationAttr()));
}

usd::UsdAttribute UsdGeomGprim::CreateOrientationAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomGprim gprim(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(gprim.CreateOrientationAttr()));
}

usd::UsdAttribute UsdGeomGprim::GetDisplayColorAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomGprim gprim(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(gprim.GetDisplayColorAttr()));
}

usd::UsdAttribute UsdGeomGprim::CreateDisplayColorAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomGprim gprim(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(gprim.CreateDisplayColorAttr()));
}

usd::UsdAttribute UsdGeomGprim::GetDisplayOpacityAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomGprim gprim(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(gprim.GetDisplayOpacityAttr()));
}

usd::UsdAttribute UsdGeomGprim::CreateDisplayOpacityAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomGprim gprim(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(gprim.CreateDisplayOpacityAttr()));
}

usd::UsdSchemaKind UsdGeomGprim::_GetSchemaKind() const {
    return usd::UsdSchemaKind::AbstractTyped;
}

// ============================================================================
// UsdGeomPointBased implementation
// ============================================================================

UsdGeomPointBased::UsdGeomPointBased(const usd::UsdPrim& prim) : UsdGeomGprim(prim) {}

UsdGeomPointBased::UsdGeomPointBased(const usd::UsdSchemaBase& schemaObj) : UsdGeomGprim(schemaObj) {}

UsdGeomPointBased::~UsdGeomPointBased() {}

UsdGeomPointBased UsdGeomPointBased::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    return UsdGeomPointBased(stage.GetPrimAtPath(path));
}

usd::UsdAttribute UsdGeomPointBased::GetPointsAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomPointBased pointBased(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(pointBased.GetPointsAttr()));
}

usd::UsdAttribute UsdGeomPointBased::CreatePointsAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomPointBased pointBased(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(pointBased.CreatePointsAttr()));
}

usd::UsdAttribute UsdGeomPointBased::GetVelocitiesAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomPointBased pointBased(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(pointBased.GetVelocitiesAttr()));
}

usd::UsdAttribute UsdGeomPointBased::CreateVelocitiesAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomPointBased pointBased(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(pointBased.CreateVelocitiesAttr()));
}

usd::UsdAttribute UsdGeomPointBased::GetAccelerationsAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomPointBased pointBased(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(pointBased.GetAccelerationsAttr()));
}

usd::UsdAttribute UsdGeomPointBased::CreateAccelerationsAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomPointBased pointBased(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(pointBased.CreateAccelerationsAttr()));
}

usd::UsdAttribute UsdGeomPointBased::GetNormalsAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomPointBased pointBased(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(pointBased.GetNormalsAttr()));
}

usd::UsdAttribute UsdGeomPointBased::CreateNormalsAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomPointBased pointBased(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(pointBased.CreateNormalsAttr()));
}

tf::TfToken UsdGeomPointBased::GetNormalsInterpolation() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return tf::TfToken();
    pxr::UsdGeomPointBased pointBased(*asPrim(prim.GetImpl()));
    return tf::TfToken::FromImpl(new pxr::TfToken(pointBased.GetNormalsInterpolation()));
}

bool UsdGeomPointBased::SetNormalsInterpolation(const tf::TfToken& interpolation) {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return false;
    pxr::UsdGeomPointBased pointBased(*asPrim(prim.GetImpl()));
    return pointBased.SetNormalsInterpolation(*static_cast<pxr::TfToken*>(interpolation.GetImpl()));
}

bool UsdGeomPointBased::ComputePointsAtTime(std::vector<gf::Vec3f>* points,
                                             usd::UsdTimeCode time,
                                             usd::UsdTimeCode baseTime) const {
    if (!points) return false;
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return false;

    pxr::VtVec3fArray pxrPoints;
    pxr::UsdGeomPointBased pointBased(*asPrim(prim.GetImpl()));
    bool result = pointBased.ComputePointsAtTime(&pxrPoints, toPxrTimeCode(time), toPxrTimeCode(baseTime));

    if (result) {
        points->clear();
        for (const auto& v : pxrPoints) {
            points->push_back(gf::Vec3f(v[0], v[1], v[2]));
        }
    }

    return result;
}

usd::UsdSchemaKind UsdGeomPointBased::_GetSchemaKind() const {
    return usd::UsdSchemaKind::AbstractTyped;
}

// ============================================================================
// UsdGeomMesh implementation
// ============================================================================

UsdGeomMesh::UsdGeomMesh(const usd::UsdPrim& prim) : UsdGeomPointBased(prim) {}

UsdGeomMesh::UsdGeomMesh(const usd::UsdSchemaBase& schemaObj) : UsdGeomPointBased(schemaObj) {}

UsdGeomMesh::~UsdGeomMesh() {}

UsdGeomMesh UsdGeomMesh::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    return UsdGeomMesh(stage.GetPrimAtPath(path));
}

UsdGeomMesh UsdGeomMesh::Define(usd::UsdStage& stage, const sdf::SdfPath& path) {
    usd::UsdPrim prim = stage.DefinePrim(path, tf::TfToken::FromImpl(new pxr::TfToken("Mesh")));
    return UsdGeomMesh(prim);
}

usd::UsdAttribute UsdGeomMesh::GetFaceVertexIndicesAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomMesh mesh(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(mesh.GetFaceVertexIndicesAttr()));
}

usd::UsdAttribute UsdGeomMesh::CreateFaceVertexIndicesAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomMesh mesh(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(mesh.CreateFaceVertexIndicesAttr()));
}

usd::UsdAttribute UsdGeomMesh::GetFaceVertexCountsAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomMesh mesh(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(mesh.GetFaceVertexCountsAttr()));
}

usd::UsdAttribute UsdGeomMesh::CreateFaceVertexCountsAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomMesh mesh(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(mesh.CreateFaceVertexCountsAttr()));
}

size_t UsdGeomMesh::GetFaceCount(usd::UsdTimeCode time) const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return 0;
    pxr::UsdGeomMesh mesh(*asPrim(prim.GetImpl()));
    return mesh.GetFaceCount(toPxrTimeCode(time));
}

usd::UsdAttribute UsdGeomMesh::GetSubdivisionSchemeAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomMesh mesh(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(mesh.GetSubdivisionSchemeAttr()));
}

usd::UsdAttribute UsdGeomMesh::CreateSubdivisionSchemeAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomMesh mesh(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(mesh.CreateSubdivisionSchemeAttr()));
}

usd::UsdAttribute UsdGeomMesh::GetInterpolateBoundaryAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomMesh mesh(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(mesh.GetInterpolateBoundaryAttr()));
}

usd::UsdAttribute UsdGeomMesh::CreateInterpolateBoundaryAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomMesh mesh(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(mesh.CreateInterpolateBoundaryAttr()));
}

usd::UsdAttribute UsdGeomMesh::GetFaceVaryingLinearInterpolationAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomMesh mesh(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(mesh.GetFaceVaryingLinearInterpolationAttr()));
}

usd::UsdAttribute UsdGeomMesh::CreateFaceVaryingLinearInterpolationAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomMesh mesh(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(mesh.CreateFaceVaryingLinearInterpolationAttr()));
}

usd::UsdAttribute UsdGeomMesh::GetHoleIndicesAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomMesh mesh(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(mesh.GetHoleIndicesAttr()));
}

usd::UsdAttribute UsdGeomMesh::CreateHoleIndicesAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomMesh mesh(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(mesh.CreateHoleIndicesAttr()));
}

usd::UsdAttribute UsdGeomMesh::GetCornerIndicesAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomMesh mesh(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(mesh.GetCornerIndicesAttr()));
}

usd::UsdAttribute UsdGeomMesh::CreateCornerIndicesAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomMesh mesh(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(mesh.CreateCornerIndicesAttr()));
}

usd::UsdAttribute UsdGeomMesh::GetCornerSharpnessesAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomMesh mesh(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(mesh.GetCornerSharpnessesAttr()));
}

usd::UsdAttribute UsdGeomMesh::CreateCornerSharpnessesAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomMesh mesh(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(mesh.CreateCornerSharpnessesAttr()));
}

usd::UsdAttribute UsdGeomMesh::GetCreaseIndicesAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomMesh mesh(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(mesh.GetCreaseIndicesAttr()));
}

usd::UsdAttribute UsdGeomMesh::CreateCreaseIndicesAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomMesh mesh(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(mesh.CreateCreaseIndicesAttr()));
}

usd::UsdAttribute UsdGeomMesh::GetCreaseLengthsAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomMesh mesh(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(mesh.GetCreaseLengthsAttr()));
}

usd::UsdAttribute UsdGeomMesh::CreateCreaseLengthsAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomMesh mesh(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(mesh.CreateCreaseLengthsAttr()));
}

usd::UsdAttribute UsdGeomMesh::GetCreaseSharpnessesAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomMesh mesh(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(mesh.GetCreaseSharpnessesAttr()));
}

usd::UsdAttribute UsdGeomMesh::CreateCreaseSharpnessesAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomMesh mesh(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(mesh.CreateCreaseSharpnessesAttr()));
}

bool UsdGeomMesh::ValidateTopology(const std::vector<int>& faceVertexIndices,
                                    const std::vector<int>& faceVertexCounts,
                                    size_t numPoints,
                                    std::string* reason) {
    pxr::VtIntArray pxrIndices(faceVertexIndices.begin(), faceVertexIndices.end());
    pxr::VtIntArray pxrCounts(faceVertexCounts.begin(), faceVertexCounts.end());
    return pxr::UsdGeomMesh::ValidateTopology(pxrIndices, pxrCounts, numPoints, reason);
}

float UsdGeomMesh::GetSharpnessInfinite() {
    return pxr::UsdGeomMesh::SHARPNESS_INFINITE;
}

usd::UsdSchemaKind UsdGeomMesh::_GetSchemaKind() const {
    return usd::UsdSchemaKind::ConcreteTyped;
}

// ============================================================================
// UsdGeomPoints implementation
// ============================================================================

UsdGeomPoints::UsdGeomPoints(const usd::UsdPrim& prim) : UsdGeomPointBased(prim) {}

UsdGeomPoints::UsdGeomPoints(const usd::UsdSchemaBase& schemaObj) : UsdGeomPointBased(schemaObj) {}

UsdGeomPoints::~UsdGeomPoints() {}

UsdGeomPoints UsdGeomPoints::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    return UsdGeomPoints(stage.GetPrimAtPath(path));
}

UsdGeomPoints UsdGeomPoints::Define(usd::UsdStage& stage, const sdf::SdfPath& path) {
    usd::UsdPrim prim = stage.DefinePrim(path, tf::TfToken::FromImpl(new pxr::TfToken("Points")));
    return UsdGeomPoints(prim);
}

usd::UsdAttribute UsdGeomPoints::GetWidthsAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomPoints points(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(points.GetWidthsAttr()));
}

usd::UsdAttribute UsdGeomPoints::CreateWidthsAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomPoints points(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(points.CreateWidthsAttr()));
}

usd::UsdAttribute UsdGeomPoints::GetIdsAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomPoints points(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(points.GetIdsAttr()));
}

usd::UsdAttribute UsdGeomPoints::CreateIdsAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomPoints points(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(points.CreateIdsAttr()));
}

usd::UsdSchemaKind UsdGeomPoints::_GetSchemaKind() const {
    return usd::UsdSchemaKind::ConcreteTyped;
}

// ============================================================================
// UsdGeomCurves implementation
// ============================================================================

UsdGeomCurves::UsdGeomCurves(const usd::UsdPrim& prim) : UsdGeomPointBased(prim) {}

UsdGeomCurves::UsdGeomCurves(const usd::UsdSchemaBase& schemaObj) : UsdGeomPointBased(schemaObj) {}

UsdGeomCurves::~UsdGeomCurves() {}

UsdGeomCurves UsdGeomCurves::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    return UsdGeomCurves(stage.GetPrimAtPath(path));
}

usd::UsdAttribute UsdGeomCurves::GetCurveVertexCountsAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomCurves curves(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(curves.GetCurveVertexCountsAttr()));
}

usd::UsdAttribute UsdGeomCurves::CreateCurveVertexCountsAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomCurves curves(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(curves.CreateCurveVertexCountsAttr()));
}

usd::UsdAttribute UsdGeomCurves::GetWidthsAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomCurves curves(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(curves.GetWidthsAttr()));
}

usd::UsdAttribute UsdGeomCurves::CreateWidthsAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomCurves curves(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(curves.CreateWidthsAttr()));
}

usd::UsdSchemaKind UsdGeomCurves::_GetSchemaKind() const {
    return usd::UsdSchemaKind::AbstractTyped;
}

// ============================================================================
// UsdGeomBasisCurves implementation
// ============================================================================

UsdGeomBasisCurves::UsdGeomBasisCurves(const usd::UsdPrim& prim) : UsdGeomCurves(prim) {}

UsdGeomBasisCurves::UsdGeomBasisCurves(const usd::UsdSchemaBase& schemaObj) : UsdGeomCurves(schemaObj) {}

UsdGeomBasisCurves::~UsdGeomBasisCurves() {}

UsdGeomBasisCurves UsdGeomBasisCurves::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    return UsdGeomBasisCurves(stage.GetPrimAtPath(path));
}

UsdGeomBasisCurves UsdGeomBasisCurves::Define(usd::UsdStage& stage, const sdf::SdfPath& path) {
    usd::UsdPrim prim = stage.DefinePrim(path, tf::TfToken::FromImpl(new pxr::TfToken("BasisCurves")));
    return UsdGeomBasisCurves(prim);
}

usd::UsdAttribute UsdGeomBasisCurves::GetTypeAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomBasisCurves curves(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(curves.GetTypeAttr()));
}

usd::UsdAttribute UsdGeomBasisCurves::CreateTypeAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomBasisCurves curves(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(curves.CreateTypeAttr()));
}

usd::UsdAttribute UsdGeomBasisCurves::GetBasisAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomBasisCurves curves(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(curves.GetBasisAttr()));
}

usd::UsdAttribute UsdGeomBasisCurves::CreateBasisAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomBasisCurves curves(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(curves.CreateBasisAttr()));
}

usd::UsdAttribute UsdGeomBasisCurves::GetWrapAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomBasisCurves curves(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(curves.GetWrapAttr()));
}

usd::UsdAttribute UsdGeomBasisCurves::CreateWrapAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomBasisCurves curves(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(curves.CreateWrapAttr()));
}

usd::UsdSchemaKind UsdGeomBasisCurves::_GetSchemaKind() const {
    return usd::UsdSchemaKind::ConcreteTyped;
}

// ============================================================================
// UsdGeomNurbsCurves implementation
// ============================================================================

UsdGeomNurbsCurves::UsdGeomNurbsCurves(const usd::UsdPrim& prim) : UsdGeomCurves(prim) {}

UsdGeomNurbsCurves::UsdGeomNurbsCurves(const usd::UsdSchemaBase& schemaObj) : UsdGeomCurves(schemaObj) {}

UsdGeomNurbsCurves::~UsdGeomNurbsCurves() {}

UsdGeomNurbsCurves UsdGeomNurbsCurves::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    return UsdGeomNurbsCurves(stage.GetPrimAtPath(path));
}

UsdGeomNurbsCurves UsdGeomNurbsCurves::Define(usd::UsdStage& stage, const sdf::SdfPath& path) {
    usd::UsdPrim prim = stage.DefinePrim(path, tf::TfToken::FromImpl(new pxr::TfToken("NurbsCurves")));
    return UsdGeomNurbsCurves(prim);
}

usd::UsdAttribute UsdGeomNurbsCurves::GetOrderAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomNurbsCurves curves(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(curves.GetOrderAttr()));
}

usd::UsdAttribute UsdGeomNurbsCurves::CreateOrderAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomNurbsCurves curves(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(curves.CreateOrderAttr()));
}

usd::UsdAttribute UsdGeomNurbsCurves::GetKnotsAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomNurbsCurves curves(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(curves.GetKnotsAttr()));
}

usd::UsdAttribute UsdGeomNurbsCurves::CreateKnotsAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomNurbsCurves curves(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(curves.CreateKnotsAttr()));
}

usd::UsdAttribute UsdGeomNurbsCurves::GetRangesAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomNurbsCurves curves(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(curves.GetRangesAttr()));
}

usd::UsdAttribute UsdGeomNurbsCurves::CreateRangesAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomNurbsCurves curves(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(curves.CreateRangesAttr()));
}

usd::UsdSchemaKind UsdGeomNurbsCurves::_GetSchemaKind() const {
    return usd::UsdSchemaKind::ConcreteTyped;
}

// ============================================================================
// UsdGeomCamera implementation
// ============================================================================

UsdGeomCamera::UsdGeomCamera(const usd::UsdPrim& prim) : UsdGeomXformable(prim) {}

UsdGeomCamera::UsdGeomCamera(const usd::UsdSchemaBase& schemaObj) : UsdGeomXformable(schemaObj) {}

UsdGeomCamera::~UsdGeomCamera() {}

UsdGeomCamera UsdGeomCamera::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    return UsdGeomCamera(stage.GetPrimAtPath(path));
}

UsdGeomCamera UsdGeomCamera::Define(usd::UsdStage& stage, const sdf::SdfPath& path) {
    usd::UsdPrim prim = stage.DefinePrim(path, tf::TfToken::FromImpl(new pxr::TfToken("Camera")));
    return UsdGeomCamera(prim);
}

#define CAMERA_ATTR_IMPL(Name) \
usd::UsdAttribute UsdGeomCamera::Get##Name##Attr() const { \
    usd::UsdPrim prim = GetPrim(); \
    if (!prim.IsValid()) return usd::UsdAttribute(); \
    pxr::UsdGeomCamera camera(*asPrim(prim.GetImpl())); \
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(camera.Get##Name##Attr())); \
} \
usd::UsdAttribute UsdGeomCamera::Create##Name##Attr() { \
    usd::UsdPrim prim = GetPrim(); \
    if (!prim.IsValid()) return usd::UsdAttribute(); \
    pxr::UsdGeomCamera camera(*asPrim(prim.GetImpl())); \
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(camera.Create##Name##Attr())); \
}

CAMERA_ATTR_IMPL(Projection)
CAMERA_ATTR_IMPL(FocalLength)
CAMERA_ATTR_IMPL(HorizontalAperture)
CAMERA_ATTR_IMPL(VerticalAperture)
CAMERA_ATTR_IMPL(HorizontalApertureOffset)
CAMERA_ATTR_IMPL(VerticalApertureOffset)
CAMERA_ATTR_IMPL(ClippingRange)
CAMERA_ATTR_IMPL(ClippingPlanes)
CAMERA_ATTR_IMPL(FStop)
CAMERA_ATTR_IMPL(FocusDistance)
CAMERA_ATTR_IMPL(ShutterOpen)
CAMERA_ATTR_IMPL(ShutterClose)

#undef CAMERA_ATTR_IMPL

gf::Matrix4d UsdGeomCamera::GetCamera(usd::UsdTimeCode time) const {
    gf::Matrix4d result;
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return result;

    pxr::UsdGeomCamera camera(*asPrim(prim.GetImpl()));
    pxr::GfCamera gfCamera = camera.GetCamera(toPxrTimeCode(time));

    // Get the view/projection matrix
    pxr::GfMatrix4d frustum = gfCamera.GetFrustum().ComputeProjectionMatrix();
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            result.data[r][c] = frustum[r][c];
        }
    }

    return result;
}

usd::UsdSchemaKind UsdGeomCamera::_GetSchemaKind() const {
    return usd::UsdSchemaKind::ConcreteTyped;
}

// ============================================================================
// Geometric Primitives implementations
// ============================================================================

// Macro to simplify gprim implementations
#define GPRIM_IMPL(ClassName, TypeName) \
ClassName::ClassName(const usd::UsdPrim& prim) : UsdGeomGprim(prim) {} \
ClassName::ClassName(const usd::UsdSchemaBase& schemaObj) : UsdGeomGprim(schemaObj) {} \
ClassName::~ClassName() {} \
ClassName ClassName::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) { \
    return ClassName(stage.GetPrimAtPath(path)); \
} \
ClassName ClassName::Define(usd::UsdStage& stage, const sdf::SdfPath& path) { \
    usd::UsdPrim prim = stage.DefinePrim(path, tf::TfToken::FromImpl(new pxr::TfToken(TypeName))); \
    return ClassName(prim); \
} \
usd::UsdSchemaKind ClassName::_GetSchemaKind() const { \
    return usd::UsdSchemaKind::ConcreteTyped; \
}

GPRIM_IMPL(UsdGeomCube, "Cube")
GPRIM_IMPL(UsdGeomSphere, "Sphere")
GPRIM_IMPL(UsdGeomCylinder, "Cylinder")
GPRIM_IMPL(UsdGeomCapsule, "Capsule")
GPRIM_IMPL(UsdGeomCone, "Cone")

#undef GPRIM_IMPL

// Cube specific
usd::UsdAttribute UsdGeomCube::GetSizeAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomCube cube(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(cube.GetSizeAttr()));
}

usd::UsdAttribute UsdGeomCube::CreateSizeAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomCube cube(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(cube.CreateSizeAttr()));
}

// Sphere specific
usd::UsdAttribute UsdGeomSphere::GetRadiusAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomSphere sphere(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(sphere.GetRadiusAttr()));
}

usd::UsdAttribute UsdGeomSphere::CreateRadiusAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdGeomSphere sphere(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(sphere.CreateRadiusAttr()));
}

// Macro for cylinder/capsule/cone common attributes
#define AXIS_GEOM_ATTRS(ClassName, PxrClass) \
usd::UsdAttribute ClassName::GetRadiusAttr() const { \
    usd::UsdPrim prim = GetPrim(); \
    if (!prim.IsValid()) return usd::UsdAttribute(); \
    PxrClass geom(*asPrim(prim.GetImpl())); \
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(geom.GetRadiusAttr())); \
} \
usd::UsdAttribute ClassName::CreateRadiusAttr() { \
    usd::UsdPrim prim = GetPrim(); \
    if (!prim.IsValid()) return usd::UsdAttribute(); \
    PxrClass geom(*asPrim(prim.GetImpl())); \
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(geom.CreateRadiusAttr())); \
} \
usd::UsdAttribute ClassName::GetHeightAttr() const { \
    usd::UsdPrim prim = GetPrim(); \
    if (!prim.IsValid()) return usd::UsdAttribute(); \
    PxrClass geom(*asPrim(prim.GetImpl())); \
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(geom.GetHeightAttr())); \
} \
usd::UsdAttribute ClassName::CreateHeightAttr() { \
    usd::UsdPrim prim = GetPrim(); \
    if (!prim.IsValid()) return usd::UsdAttribute(); \
    PxrClass geom(*asPrim(prim.GetImpl())); \
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(geom.CreateHeightAttr())); \
} \
usd::UsdAttribute ClassName::GetAxisAttr() const { \
    usd::UsdPrim prim = GetPrim(); \
    if (!prim.IsValid()) return usd::UsdAttribute(); \
    PxrClass geom(*asPrim(prim.GetImpl())); \
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(geom.GetAxisAttr())); \
} \
usd::UsdAttribute ClassName::CreateAxisAttr() { \
    usd::UsdPrim prim = GetPrim(); \
    if (!prim.IsValid()) return usd::UsdAttribute(); \
    PxrClass geom(*asPrim(prim.GetImpl())); \
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(geom.CreateAxisAttr())); \
}

AXIS_GEOM_ATTRS(UsdGeomCylinder, pxr::UsdGeomCylinder)
AXIS_GEOM_ATTRS(UsdGeomCapsule, pxr::UsdGeomCapsule)
AXIS_GEOM_ATTRS(UsdGeomCone, pxr::UsdGeomCone)

#undef AXIS_GEOM_ATTRS

// ============================================================================
// UsdGeomPrimvar implementation
// ============================================================================

static inline pxr::UsdGeomPrimvar* asPrimvar(void* impl) {
    return static_cast<pxr::UsdGeomPrimvar*>(impl);
}

UsdGeomPrimvar::UsdGeomPrimvar() : impl_(new pxr::UsdGeomPrimvar()) {}

UsdGeomPrimvar::UsdGeomPrimvar(const UsdGeomPrimvar& other)
    : impl_(new pxr::UsdGeomPrimvar(*asPrimvar(other.impl_))) {}

UsdGeomPrimvar::UsdGeomPrimvar(UsdGeomPrimvar&& other) noexcept : impl_(other.impl_) {
    other.impl_ = nullptr;
}

UsdGeomPrimvar::~UsdGeomPrimvar() {
    delete asPrimvar(impl_);
}

UsdGeomPrimvar& UsdGeomPrimvar::operator=(const UsdGeomPrimvar& other) {
    if (this != &other) {
        delete asPrimvar(impl_);
        impl_ = new pxr::UsdGeomPrimvar(*asPrimvar(other.impl_));
    }
    return *this;
}

UsdGeomPrimvar& UsdGeomPrimvar::operator=(UsdGeomPrimvar&& other) noexcept {
    if (this != &other) {
        delete asPrimvar(impl_);
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

bool UsdGeomPrimvar::IsValid() const {
    return impl_ && asPrimvar(impl_)->IsDefined();
}

usd::UsdAttribute UsdGeomPrimvar::GetAttr() const {
    if (!IsValid()) return usd::UsdAttribute();
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(asPrimvar(impl_)->GetAttr()));
}

tf::TfToken UsdGeomPrimvar::GetPrimvarName() const {
    if (!IsValid()) return tf::TfToken();
    return tf::TfToken::FromImpl(new pxr::TfToken(asPrimvar(impl_)->GetPrimvarName()));
}

tf::TfToken UsdGeomPrimvar::GetInterpolation() const {
    if (!IsValid()) return tf::TfToken();
    return tf::TfToken::FromImpl(new pxr::TfToken(asPrimvar(impl_)->GetInterpolation()));
}

bool UsdGeomPrimvar::SetInterpolation(const tf::TfToken& interpolation) {
    if (!IsValid()) return false;
    return asPrimvar(impl_)->SetInterpolation(*static_cast<pxr::TfToken*>(interpolation.GetImpl()));
}

int UsdGeomPrimvar::GetElementSize() const {
    if (!IsValid()) return 1;
    return asPrimvar(impl_)->GetElementSize();
}

bool UsdGeomPrimvar::SetElementSize(int size) {
    if (!IsValid()) return false;
    return asPrimvar(impl_)->SetElementSize(size);
}

bool UsdGeomPrimvar::IsIndexed() const {
    return IsValid() && asPrimvar(impl_)->IsIndexed();
}

bool UsdGeomPrimvar::Get(vt::VtValue* value, usd::UsdTimeCode time) const {
    if (!IsValid() || !value) return false;
    pxr::VtValue pxrValue;
    bool result = asPrimvar(impl_)->Get(&pxrValue, toPxrTimeCode(time));
    if (result) {
        *value = vt::VtValue::FromImpl(new pxr::VtValue(pxrValue));
    }
    return result;
}

bool UsdGeomPrimvar::Set(const vt::VtValue& value, usd::UsdTimeCode time) {
    if (!IsValid()) return false;
    pxr::VtValue* pxrValue = static_cast<pxr::VtValue*>(value.GetImpl());
    return asPrimvar(impl_)->Set(*pxrValue, toPxrTimeCode(time));
}

UsdGeomPrimvar UsdGeomPrimvar::FromImpl(void* impl) {
    UsdGeomPrimvar pv;
    delete asPrimvar(pv.impl_);
    pv.impl_ = impl;
    return pv;
}

// ============================================================================
// UsdGeomPrimvarsAPI implementation
// ============================================================================

UsdGeomPrimvarsAPI::UsdGeomPrimvarsAPI(const usd::UsdPrim& prim) : UsdAPISchemaBase(prim) {}

UsdGeomPrimvarsAPI::UsdGeomPrimvarsAPI(const usd::UsdSchemaBase& schemaObj) : UsdAPISchemaBase(schemaObj) {}

UsdGeomPrimvarsAPI::~UsdGeomPrimvarsAPI() {}

UsdGeomPrimvarsAPI UsdGeomPrimvarsAPI::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    return UsdGeomPrimvarsAPI(stage.GetPrimAtPath(path));
}

UsdGeomPrimvar UsdGeomPrimvarsAPI::CreatePrimvar(const tf::TfToken& name,
                                                   const std::string& typeName,
                                                   const tf::TfToken& interpolation) {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdGeomPrimvar();

    pxr::UsdGeomPrimvarsAPI api(*asPrim(prim.GetImpl()));
    pxr::TfToken* pxrName = static_cast<pxr::TfToken*>(name.GetImpl());
    pxr::SdfValueTypeName valueType = pxr::SdfSchema::GetInstance().FindType(typeName);

    pxr::TfToken interp;
    if (strlen(interpolation.GetText()) > 0) {
        interp = *static_cast<pxr::TfToken*>(interpolation.GetImpl());
    }

    return UsdGeomPrimvar::FromImpl(new pxr::UsdGeomPrimvar(
        api.CreatePrimvar(*pxrName, valueType, interp)));
}

UsdGeomPrimvar UsdGeomPrimvarsAPI::GetPrimvar(const tf::TfToken& name) const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdGeomPrimvar();

    pxr::UsdGeomPrimvarsAPI api(*asPrim(prim.GetImpl()));
    pxr::TfToken* pxrName = static_cast<pxr::TfToken*>(name.GetImpl());

    return UsdGeomPrimvar::FromImpl(new pxr::UsdGeomPrimvar(api.GetPrimvar(*pxrName)));
}

std::vector<UsdGeomPrimvar> UsdGeomPrimvarsAPI::GetPrimvars() const {
    std::vector<UsdGeomPrimvar> result;
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return result;

    pxr::UsdGeomPrimvarsAPI api(*asPrim(prim.GetImpl()));
    for (const auto& pv : api.GetPrimvars()) {
        result.push_back(UsdGeomPrimvar::FromImpl(new pxr::UsdGeomPrimvar(pv)));
    }

    return result;
}

std::vector<UsdGeomPrimvar> UsdGeomPrimvarsAPI::GetAuthoredPrimvars() const {
    std::vector<UsdGeomPrimvar> result;
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return result;

    pxr::UsdGeomPrimvarsAPI api(*asPrim(prim.GetImpl()));
    for (const auto& pv : api.GetAuthoredPrimvars()) {
        result.push_back(UsdGeomPrimvar::FromImpl(new pxr::UsdGeomPrimvar(pv)));
    }

    return result;
}

bool UsdGeomPrimvarsAPI::HasPrimvar(const tf::TfToken& name) const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return false;

    pxr::UsdGeomPrimvarsAPI api(*asPrim(prim.GetImpl()));
    pxr::TfToken* pxrName = static_cast<pxr::TfToken*>(name.GetImpl());

    return api.HasPrimvar(*pxrName);
}

bool UsdGeomPrimvarsAPI::RemovePrimvar(const tf::TfToken& name) {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return false;

    pxr::UsdGeomPrimvarsAPI api(*asPrim(prim.GetImpl()));
    pxr::TfToken* pxrName = static_cast<pxr::TfToken*>(name.GetImpl());

    return api.RemovePrimvar(*pxrName);
}

usd::UsdSchemaKind UsdGeomPrimvarsAPI::_GetSchemaKind() const {
    return usd::UsdSchemaKind::SingleApplyAPI;
}

// ============================================================================
// Token Constants implementation
// ============================================================================

namespace UsdGeomTokens {

#define TOKEN_IMPL(name, value) \
tf::TfToken name() { \
    static tf::TfToken token = tf::TfToken::FromImpl(new pxr::TfToken(value)); \
    return token; \
}

// Visibility
TOKEN_IMPL(inherited, "inherited")
TOKEN_IMPL(invisible, "invisible")

// Purpose
TOKEN_IMPL(default_, "default")
TOKEN_IMPL(render, "render")
TOKEN_IMPL(proxy, "proxy")
TOKEN_IMPL(guide, "guide")

// Orientation
TOKEN_IMPL(rightHanded, "rightHanded")
TOKEN_IMPL(leftHanded, "leftHanded")

// Subdivision schemes
TOKEN_IMPL(catmullClark, "catmullClark")
TOKEN_IMPL(loop, "loop")
TOKEN_IMPL(bilinear, "bilinear")
TOKEN_IMPL(none, "none")

// Interpolation
TOKEN_IMPL(constant, "constant")
TOKEN_IMPL(uniform, "uniform")
TOKEN_IMPL(varying, "varying")
TOKEN_IMPL(vertex, "vertex")
TOKEN_IMPL(faceVarying, "faceVarying")

// Camera
TOKEN_IMPL(perspective, "perspective")
TOKEN_IMPL(orthographic, "orthographic")

// Axis
TOKEN_IMPL(x, "X")
TOKEN_IMPL(y, "Y")
TOKEN_IMPL(z, "Z")

// Basis curve types
TOKEN_IMPL(linear, "linear")
TOKEN_IMPL(cubic, "cubic")

// Basis curve bases
TOKEN_IMPL(bezier, "bezier")
TOKEN_IMPL(bspline, "bspline")
TOKEN_IMPL(catmullRom, "catmullRom")

// Wrap modes
TOKEN_IMPL(nonperiodic, "nonperiodic")
TOKEN_IMPL(periodic, "periodic")
TOKEN_IMPL(pinned, "pinned")

#undef TOKEN_IMPL

} // namespace UsdGeomTokens

} // namespace usdGeom
} // namespace cxxfacade
