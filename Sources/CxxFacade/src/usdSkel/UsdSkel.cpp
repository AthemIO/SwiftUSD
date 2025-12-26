// UsdSkel.cpp - Skeletal Animation Schemas implementation
// This file contains the real USD calls for the UsdSkel facade

#include "../../include/usdSkel/UsdSkel.hpp"

// Pixar USD includes - ONLY in .cpp files!
#include <pxr/usd/usdSkel/skeleton.h>
#include <pxr/usd/usdSkel/root.h>
#include <pxr/usd/usdSkel/animation.h>
#include <pxr/usd/usdSkel/bindingAPI.h>
#include <pxr/usd/usdSkel/blendShape.h>
#include <pxr/usd/usdSkel/topology.h>
#include <pxr/usd/usdSkel/cache.h>
#include <pxr/usd/usdSkel/animQuery.h>
#include <pxr/usd/usdSkel/skeletonQuery.h>
#include <pxr/usd/usdSkel/skinningQuery.h>
#include <pxr/usd/usdSkel/tokens.h>
#include <pxr/usd/usdSkel/utils.h>
#include <pxr/usd/usdGeom/primvar.h>

PXR_NAMESPACE_USING_DIRECTIVE

namespace cxxfacade {
namespace usdSkel {

// ============================================================================
// Helper functions for conversion
// ============================================================================

namespace {

inline UsdPrim GetUsdPrim(const usd::UsdPrim& prim) {
    return *reinterpret_cast<const UsdPrim*>(prim.GetImpl());
}

inline UsdStageRefPtr GetUsdStagePtr(const usd::UsdStage& stage) {
    // UsdStage uses a StageHandle struct internally with a ptr member
    struct StageHandle { UsdStageRefPtr ptr; };
    StageHandle* handle = static_cast<StageHandle*>(stage.GetImpl());
    return handle ? handle->ptr : UsdStageRefPtr();
}

inline SdfPath GetSdfPath(const sdf::SdfPath& path) {
    return *reinterpret_cast<const SdfPath*>(path.GetImpl());
}

inline TfToken GetTfToken(const tf::TfToken& token) {
    return *reinterpret_cast<const TfToken*>(token.GetImpl());
}

inline usd::UsdAttribute WrapAttribute(const UsdAttribute& attr) {
    return usd::UsdAttribute::FromImpl(new UsdAttribute(attr));
}

inline usd::UsdRelationship WrapRelationship(const UsdRelationship& rel) {
    return usd::UsdRelationship::FromImpl(new UsdRelationship(rel));
}

inline usd::UsdPrim WrapPrim(const UsdPrim& prim) {
    return usd::UsdPrim::FromImpl(new UsdPrim(prim));
}

inline tf::TfToken WrapToken(const TfToken& token) {
    return tf::TfToken(token.GetString());
}

inline gf::Matrix4d WrapMatrix(const GfMatrix4d& mat) {
    return gf::Matrix4d(
        mat[0][0], mat[0][1], mat[0][2], mat[0][3],
        mat[1][0], mat[1][1], mat[1][2], mat[1][3],
        mat[2][0], mat[2][1], mat[2][2], mat[2][3],
        mat[3][0], mat[3][1], mat[3][2], mat[3][3]
    );
}

inline GfMatrix4d GetGfMatrix(const gf::Matrix4d& mat) {
    GfMatrix4d result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result[i][j] = mat.data[i][j];
        }
    }
    return result;
}

} // anonymous namespace

// ============================================================================
// UsdSkelTopology Implementation
// ============================================================================

UsdSkelTopology::UsdSkelTopology() : impl_(nullptr) {}

UsdSkelTopology::UsdSkelTopology(const UsdSkelTopology& other) : impl_(nullptr) {
    if (other.impl_) {
        impl_ = new PXR_NS::UsdSkelTopology(*static_cast<PXR_NS::UsdSkelTopology*>(other.impl_));
    }
}

UsdSkelTopology::UsdSkelTopology(UsdSkelTopology&& other) noexcept : impl_(other.impl_) {
    other.impl_ = nullptr;
}

UsdSkelTopology::~UsdSkelTopology() {
    delete static_cast<PXR_NS::UsdSkelTopology*>(impl_);
}

UsdSkelTopology& UsdSkelTopology::operator=(const UsdSkelTopology& other) {
    if (this != &other) {
        delete static_cast<PXR_NS::UsdSkelTopology*>(impl_);
        impl_ = nullptr;
        if (other.impl_) {
            impl_ = new PXR_NS::UsdSkelTopology(*static_cast<PXR_NS::UsdSkelTopology*>(other.impl_));
        }
    }
    return *this;
}

UsdSkelTopology& UsdSkelTopology::operator=(UsdSkelTopology&& other) noexcept {
    if (this != &other) {
        delete static_cast<PXR_NS::UsdSkelTopology*>(impl_);
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

UsdSkelTopology::UsdSkelTopology(const std::vector<tf::TfToken>& jointPaths) {
    VtTokenArray tokens;
    tokens.reserve(jointPaths.size());
    for (const auto& path : jointPaths) {
        tokens.push_back(GetTfToken(path));
    }
    impl_ = new PXR_NS::UsdSkelTopology(TfSpan<const TfToken>(tokens.data(), tokens.size()));
}

UsdSkelTopology::UsdSkelTopology(const std::vector<int>& parentIndices) {
    VtIntArray indices(parentIndices.begin(), parentIndices.end());
    impl_ = new PXR_NS::UsdSkelTopology(indices);
}

bool UsdSkelTopology::IsValid() const {
    return impl_ != nullptr && static_cast<PXR_NS::UsdSkelTopology*>(impl_)->GetNumJoints() > 0;
}

bool UsdSkelTopology::Validate(std::string* reason) const {
    if (!impl_) return false;
    return static_cast<PXR_NS::UsdSkelTopology*>(impl_)->Validate(reason);
}

std::vector<int> UsdSkelTopology::GetParentIndices() const {
    if (!impl_) return {};
    const VtIntArray& indices = static_cast<PXR_NS::UsdSkelTopology*>(impl_)->GetParentIndices();
    return std::vector<int>(indices.begin(), indices.end());
}

size_t UsdSkelTopology::GetNumJoints() const {
    if (!impl_) return 0;
    return static_cast<PXR_NS::UsdSkelTopology*>(impl_)->GetNumJoints();
}

int UsdSkelTopology::GetParent(size_t index) const {
    if (!impl_) return -1;
    return static_cast<PXR_NS::UsdSkelTopology*>(impl_)->GetParent(index);
}

bool UsdSkelTopology::IsRoot(size_t index) const {
    if (!impl_) return true;
    return static_cast<PXR_NS::UsdSkelTopology*>(impl_)->IsRoot(index);
}

bool UsdSkelTopology::operator==(const UsdSkelTopology& other) const {
    if (!impl_ && !other.impl_) return true;
    if (!impl_ || !other.impl_) return false;
    return *static_cast<PXR_NS::UsdSkelTopology*>(impl_) ==
           *static_cast<PXR_NS::UsdSkelTopology*>(other.impl_);
}

bool UsdSkelTopology::operator!=(const UsdSkelTopology& other) const {
    return !(*this == other);
}

UsdSkelTopology UsdSkelTopology::FromImpl(void* impl) {
    UsdSkelTopology result;
    result.impl_ = impl;
    return result;
}

// ============================================================================
// UsdSkelSkeleton Implementation
// ============================================================================

UsdSkelSkeleton::UsdSkelSkeleton(const usd::UsdPrim& prim)
    : usdGeom::UsdGeomBoundable(prim) {}

UsdSkelSkeleton::UsdSkelSkeleton(const usd::UsdSchemaBase& schemaObj)
    : usdGeom::UsdGeomBoundable(schemaObj) {}

UsdSkelSkeleton::~UsdSkelSkeleton() = default;

UsdSkelSkeleton UsdSkelSkeleton::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    UsdStagePtr stagePtr = GetUsdStagePtr(stage);
    if (!stagePtr) return UsdSkelSkeleton();
    UsdPrim prim = stagePtr->GetPrimAtPath(GetSdfPath(path));
    return UsdSkelSkeleton(WrapPrim(prim));
}

UsdSkelSkeleton UsdSkelSkeleton::Define(usd::UsdStage& stage, const sdf::SdfPath& path) {
    usd::UsdPrim prim = stage.DefinePrim(path, tf::TfToken("Skeleton"));
    return UsdSkelSkeleton(prim);
}

usd::UsdAttribute UsdSkelSkeleton::GetJointsAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelSkeleton skel(usdPrim);
    return WrapAttribute(skel.GetJointsAttr());
}

usd::UsdAttribute UsdSkelSkeleton::CreateJointsAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelSkeleton skel(usdPrim);
    return WrapAttribute(skel.CreateJointsAttr());
}

usd::UsdAttribute UsdSkelSkeleton::GetJointNamesAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelSkeleton skel(usdPrim);
    return WrapAttribute(skel.GetJointNamesAttr());
}

usd::UsdAttribute UsdSkelSkeleton::CreateJointNamesAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelSkeleton skel(usdPrim);
    return WrapAttribute(skel.CreateJointNamesAttr());
}

usd::UsdAttribute UsdSkelSkeleton::GetBindTransformsAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelSkeleton skel(usdPrim);
    return WrapAttribute(skel.GetBindTransformsAttr());
}

usd::UsdAttribute UsdSkelSkeleton::CreateBindTransformsAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelSkeleton skel(usdPrim);
    return WrapAttribute(skel.CreateBindTransformsAttr());
}

usd::UsdAttribute UsdSkelSkeleton::GetRestTransformsAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelSkeleton skel(usdPrim);
    return WrapAttribute(skel.GetRestTransformsAttr());
}

usd::UsdAttribute UsdSkelSkeleton::CreateRestTransformsAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelSkeleton skel(usdPrim);
    return WrapAttribute(skel.CreateRestTransformsAttr());
}

usd::UsdSchemaKind UsdSkelSkeleton::_GetSchemaKind() const {
    return usd::UsdSchemaKind::ConcreteTyped;
}

// ============================================================================
// UsdSkelRoot Implementation
// ============================================================================

UsdSkelRoot::UsdSkelRoot(const usd::UsdPrim& prim)
    : usdGeom::UsdGeomBoundable(prim) {}

UsdSkelRoot::UsdSkelRoot(const usd::UsdSchemaBase& schemaObj)
    : usdGeom::UsdGeomBoundable(schemaObj) {}

UsdSkelRoot::~UsdSkelRoot() = default;

UsdSkelRoot UsdSkelRoot::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    UsdStagePtr stagePtr = GetUsdStagePtr(stage);
    if (!stagePtr) return UsdSkelRoot();
    UsdPrim prim = stagePtr->GetPrimAtPath(GetSdfPath(path));
    return UsdSkelRoot(WrapPrim(prim));
}

UsdSkelRoot UsdSkelRoot::Define(usd::UsdStage& stage, const sdf::SdfPath& path) {
    usd::UsdPrim prim = stage.DefinePrim(path, tf::TfToken("SkelRoot"));
    return UsdSkelRoot(prim);
}

UsdSkelRoot UsdSkelRoot::Find(const usd::UsdPrim& prim) {
    UsdPrim usdPrim = GetUsdPrim(prim);
    PXR_NS::UsdSkelRoot root = PXR_NS::UsdSkelRoot::Find(usdPrim);
    if (!root) return UsdSkelRoot();
    return UsdSkelRoot(WrapPrim(root.GetPrim()));
}

usd::UsdSchemaKind UsdSkelRoot::_GetSchemaKind() const {
    return usd::UsdSchemaKind::ConcreteTyped;
}

// ============================================================================
// UsdSkelAnimation Implementation
// ============================================================================

UsdSkelAnimation::UsdSkelAnimation(const usd::UsdPrim& prim)
    : usd::UsdTyped(prim) {}

UsdSkelAnimation::UsdSkelAnimation(const usd::UsdSchemaBase& schemaObj)
    : usd::UsdTyped(schemaObj) {}

UsdSkelAnimation::~UsdSkelAnimation() = default;

UsdSkelAnimation UsdSkelAnimation::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    UsdStagePtr stagePtr = GetUsdStagePtr(stage);
    if (!stagePtr) return UsdSkelAnimation();
    UsdPrim prim = stagePtr->GetPrimAtPath(GetSdfPath(path));
    return UsdSkelAnimation(WrapPrim(prim));
}

UsdSkelAnimation UsdSkelAnimation::Define(usd::UsdStage& stage, const sdf::SdfPath& path) {
    usd::UsdPrim prim = stage.DefinePrim(path, tf::TfToken("SkelAnimation"));
    return UsdSkelAnimation(prim);
}

usd::UsdAttribute UsdSkelAnimation::GetJointsAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelAnimation anim(usdPrim);
    return WrapAttribute(anim.GetJointsAttr());
}

usd::UsdAttribute UsdSkelAnimation::CreateJointsAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelAnimation anim(usdPrim);
    return WrapAttribute(anim.CreateJointsAttr());
}

usd::UsdAttribute UsdSkelAnimation::GetTranslationsAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelAnimation anim(usdPrim);
    return WrapAttribute(anim.GetTranslationsAttr());
}

usd::UsdAttribute UsdSkelAnimation::CreateTranslationsAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelAnimation anim(usdPrim);
    return WrapAttribute(anim.CreateTranslationsAttr());
}

usd::UsdAttribute UsdSkelAnimation::GetRotationsAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelAnimation anim(usdPrim);
    return WrapAttribute(anim.GetRotationsAttr());
}

usd::UsdAttribute UsdSkelAnimation::CreateRotationsAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelAnimation anim(usdPrim);
    return WrapAttribute(anim.CreateRotationsAttr());
}

usd::UsdAttribute UsdSkelAnimation::GetScalesAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelAnimation anim(usdPrim);
    return WrapAttribute(anim.GetScalesAttr());
}

usd::UsdAttribute UsdSkelAnimation::CreateScalesAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelAnimation anim(usdPrim);
    return WrapAttribute(anim.CreateScalesAttr());
}

usd::UsdAttribute UsdSkelAnimation::GetBlendShapesAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelAnimation anim(usdPrim);
    return WrapAttribute(anim.GetBlendShapesAttr());
}

usd::UsdAttribute UsdSkelAnimation::CreateBlendShapesAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelAnimation anim(usdPrim);
    return WrapAttribute(anim.CreateBlendShapesAttr());
}

usd::UsdAttribute UsdSkelAnimation::GetBlendShapeWeightsAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelAnimation anim(usdPrim);
    return WrapAttribute(anim.GetBlendShapeWeightsAttr());
}

usd::UsdAttribute UsdSkelAnimation::CreateBlendShapeWeightsAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelAnimation anim(usdPrim);
    return WrapAttribute(anim.CreateBlendShapeWeightsAttr());
}

bool UsdSkelAnimation::GetTransforms(std::vector<gf::Matrix4d>* xforms, usd::UsdTimeCode time) const {
    if (!xforms) return false;
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelAnimation anim(usdPrim);

    VtMatrix4dArray usdXforms;
    UsdTimeCode usdTime = time.IsDefault() ? UsdTimeCode::Default() : UsdTimeCode(time.GetValue());
    if (!anim.GetTransforms(&usdXforms, usdTime)) return false;

    xforms->clear();
    xforms->reserve(usdXforms.size());
    for (const auto& mat : usdXforms) {
        xforms->push_back(WrapMatrix(mat));
    }
    return true;
}

bool UsdSkelAnimation::SetTransforms(const std::vector<gf::Matrix4d>& xforms, usd::UsdTimeCode time) {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelAnimation anim(usdPrim);

    VtMatrix4dArray usdXforms;
    usdXforms.reserve(xforms.size());
    for (const auto& mat : xforms) {
        usdXforms.push_back(GetGfMatrix(mat));
    }

    UsdTimeCode usdTime = time.IsDefault() ? UsdTimeCode::Default() : UsdTimeCode(time.GetValue());
    return anim.SetTransforms(usdXforms, usdTime);
}

usd::UsdSchemaKind UsdSkelAnimation::_GetSchemaKind() const {
    return usd::UsdSchemaKind::ConcreteTyped;
}

// ============================================================================
// UsdSkelBindingAPI Implementation
// ============================================================================

UsdSkelBindingAPI::UsdSkelBindingAPI(const usd::UsdPrim& prim)
    : usd::UsdAPISchemaBase(prim) {}

UsdSkelBindingAPI::UsdSkelBindingAPI(const usd::UsdSchemaBase& schemaObj)
    : usd::UsdAPISchemaBase(schemaObj) {}

UsdSkelBindingAPI::~UsdSkelBindingAPI() = default;

UsdSkelBindingAPI UsdSkelBindingAPI::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    UsdStagePtr stagePtr = GetUsdStagePtr(stage);
    if (!stagePtr) return UsdSkelBindingAPI();
    UsdPrim prim = stagePtr->GetPrimAtPath(GetSdfPath(path));
    return UsdSkelBindingAPI(WrapPrim(prim));
}

UsdSkelBindingAPI UsdSkelBindingAPI::Apply(const usd::UsdPrim& prim) {
    UsdPrim usdPrim = GetUsdPrim(prim);
    auto applied = PXR_NS::UsdSkelBindingAPI::Apply(usdPrim);
    if (!applied) return UsdSkelBindingAPI();
    return UsdSkelBindingAPI(WrapPrim(applied.GetPrim()));
}

bool UsdSkelBindingAPI::CanApply(const usd::UsdPrim& prim, std::string* whyNot) {
    UsdPrim usdPrim = GetUsdPrim(prim);
    return PXR_NS::UsdSkelBindingAPI::CanApply(usdPrim, whyNot);
}

usd::UsdAttribute UsdSkelBindingAPI::GetSkinningMethodAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelBindingAPI api(usdPrim);
    return WrapAttribute(api.GetSkinningMethodAttr());
}

usd::UsdAttribute UsdSkelBindingAPI::CreateSkinningMethodAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelBindingAPI api(usdPrim);
    return WrapAttribute(api.CreateSkinningMethodAttr());
}

usd::UsdAttribute UsdSkelBindingAPI::GetGeomBindTransformAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelBindingAPI api(usdPrim);
    return WrapAttribute(api.GetGeomBindTransformAttr());
}

usd::UsdAttribute UsdSkelBindingAPI::CreateGeomBindTransformAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelBindingAPI api(usdPrim);
    return WrapAttribute(api.CreateGeomBindTransformAttr());
}

usd::UsdAttribute UsdSkelBindingAPI::GetJointsAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelBindingAPI api(usdPrim);
    return WrapAttribute(api.GetJointsAttr());
}

usd::UsdAttribute UsdSkelBindingAPI::CreateJointsAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelBindingAPI api(usdPrim);
    return WrapAttribute(api.CreateJointsAttr());
}

usd::UsdAttribute UsdSkelBindingAPI::GetJointIndicesAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelBindingAPI api(usdPrim);
    return WrapAttribute(api.GetJointIndicesAttr());
}

usd::UsdAttribute UsdSkelBindingAPI::CreateJointIndicesAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelBindingAPI api(usdPrim);
    return WrapAttribute(api.CreateJointIndicesAttr());
}

usd::UsdAttribute UsdSkelBindingAPI::GetJointWeightsAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelBindingAPI api(usdPrim);
    return WrapAttribute(api.GetJointWeightsAttr());
}

usd::UsdAttribute UsdSkelBindingAPI::CreateJointWeightsAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelBindingAPI api(usdPrim);
    return WrapAttribute(api.CreateJointWeightsAttr());
}

usd::UsdAttribute UsdSkelBindingAPI::GetBlendShapesAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelBindingAPI api(usdPrim);
    return WrapAttribute(api.GetBlendShapesAttr());
}

usd::UsdAttribute UsdSkelBindingAPI::CreateBlendShapesAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelBindingAPI api(usdPrim);
    return WrapAttribute(api.CreateBlendShapesAttr());
}

usd::UsdRelationship UsdSkelBindingAPI::GetAnimationSourceRel() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelBindingAPI api(usdPrim);
    return WrapRelationship(api.GetAnimationSourceRel());
}

usd::UsdRelationship UsdSkelBindingAPI::CreateAnimationSourceRel() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelBindingAPI api(usdPrim);
    return WrapRelationship(api.CreateAnimationSourceRel());
}

usd::UsdRelationship UsdSkelBindingAPI::GetSkeletonRel() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelBindingAPI api(usdPrim);
    return WrapRelationship(api.GetSkeletonRel());
}

usd::UsdRelationship UsdSkelBindingAPI::CreateSkeletonRel() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelBindingAPI api(usdPrim);
    return WrapRelationship(api.CreateSkeletonRel());
}

usd::UsdRelationship UsdSkelBindingAPI::GetBlendShapeTargetsRel() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelBindingAPI api(usdPrim);
    return WrapRelationship(api.GetBlendShapeTargetsRel());
}

usd::UsdRelationship UsdSkelBindingAPI::CreateBlendShapeTargetsRel() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelBindingAPI api(usdPrim);
    return WrapRelationship(api.CreateBlendShapeTargetsRel());
}

usdGeom::UsdGeomPrimvar UsdSkelBindingAPI::GetJointIndicesPrimvar() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelBindingAPI api(usdPrim);
    auto primvar = api.GetJointIndicesPrimvar();
    return usdGeom::UsdGeomPrimvar::FromImpl(new UsdGeomPrimvar(primvar));
}

usdGeom::UsdGeomPrimvar UsdSkelBindingAPI::CreateJointIndicesPrimvar(bool constant, int elementSize) {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelBindingAPI api(usdPrim);
    auto primvar = api.CreateJointIndicesPrimvar(constant, elementSize);
    return usdGeom::UsdGeomPrimvar::FromImpl(new UsdGeomPrimvar(primvar));
}

usdGeom::UsdGeomPrimvar UsdSkelBindingAPI::GetJointWeightsPrimvar() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelBindingAPI api(usdPrim);
    auto primvar = api.GetJointWeightsPrimvar();
    return usdGeom::UsdGeomPrimvar::FromImpl(new UsdGeomPrimvar(primvar));
}

usdGeom::UsdGeomPrimvar UsdSkelBindingAPI::CreateJointWeightsPrimvar(bool constant, int elementSize) {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelBindingAPI api(usdPrim);
    auto primvar = api.CreateJointWeightsPrimvar(constant, elementSize);
    return usdGeom::UsdGeomPrimvar::FromImpl(new UsdGeomPrimvar(primvar));
}

bool UsdSkelBindingAPI::SetRigidJointInfluence(int jointIndex, float weight) {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelBindingAPI api(usdPrim);
    return api.SetRigidJointInfluence(jointIndex, weight);
}

bool UsdSkelBindingAPI::GetSkeleton(UsdSkelSkeleton* skel) const {
    if (!skel) return false;
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelBindingAPI api(usdPrim);

    PXR_NS::UsdSkelSkeleton usdSkel;
    if (!api.GetSkeleton(&usdSkel)) return false;
    *skel = UsdSkelSkeleton(WrapPrim(usdSkel.GetPrim()));
    return true;
}

bool UsdSkelBindingAPI::GetAnimationSource(usd::UsdPrim* prim) const {
    if (!prim) return false;
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelBindingAPI api(usdPrim);

    UsdPrim animPrim;
    if (!api.GetAnimationSource(&animPrim)) return false;
    *prim = WrapPrim(animPrim);
    return true;
}

UsdSkelSkeleton UsdSkelBindingAPI::GetInheritedSkeleton() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelBindingAPI api(usdPrim);
    auto skel = api.GetInheritedSkeleton();
    if (!skel) return UsdSkelSkeleton();
    return UsdSkelSkeleton(WrapPrim(skel.GetPrim()));
}

usd::UsdPrim UsdSkelBindingAPI::GetInheritedAnimationSource() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelBindingAPI api(usdPrim);
    auto animPrim = api.GetInheritedAnimationSource();
    return WrapPrim(animPrim);
}

bool UsdSkelBindingAPI::ValidateJointIndices(const std::vector<int>& indices,
                                              size_t numJoints,
                                              std::string* reason) {
    TfSpan<const int> span(indices.data(), indices.size());
    return PXR_NS::UsdSkelBindingAPI::ValidateJointIndices(span, numJoints, reason);
}

usd::UsdSchemaKind UsdSkelBindingAPI::_GetSchemaKind() const {
    return usd::UsdSchemaKind::SingleApplyAPI;
}

// ============================================================================
// UsdSkelBlendShape Implementation
// ============================================================================

UsdSkelBlendShape::UsdSkelBlendShape(const usd::UsdPrim& prim)
    : usd::UsdTyped(prim) {}

UsdSkelBlendShape::UsdSkelBlendShape(const usd::UsdSchemaBase& schemaObj)
    : usd::UsdTyped(schemaObj) {}

UsdSkelBlendShape::~UsdSkelBlendShape() = default;

UsdSkelBlendShape UsdSkelBlendShape::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    UsdStagePtr stagePtr = GetUsdStagePtr(stage);
    if (!stagePtr) return UsdSkelBlendShape();
    UsdPrim prim = stagePtr->GetPrimAtPath(GetSdfPath(path));
    return UsdSkelBlendShape(WrapPrim(prim));
}

UsdSkelBlendShape UsdSkelBlendShape::Define(usd::UsdStage& stage, const sdf::SdfPath& path) {
    usd::UsdPrim prim = stage.DefinePrim(path, tf::TfToken("BlendShape"));
    return UsdSkelBlendShape(prim);
}

usd::UsdAttribute UsdSkelBlendShape::GetOffsetsAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelBlendShape blendShape(usdPrim);
    return WrapAttribute(blendShape.GetOffsetsAttr());
}

usd::UsdAttribute UsdSkelBlendShape::CreateOffsetsAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelBlendShape blendShape(usdPrim);
    return WrapAttribute(blendShape.CreateOffsetsAttr());
}

usd::UsdAttribute UsdSkelBlendShape::GetNormalOffsetsAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelBlendShape blendShape(usdPrim);
    return WrapAttribute(blendShape.GetNormalOffsetsAttr());
}

usd::UsdAttribute UsdSkelBlendShape::CreateNormalOffsetsAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelBlendShape blendShape(usdPrim);
    return WrapAttribute(blendShape.CreateNormalOffsetsAttr());
}

usd::UsdAttribute UsdSkelBlendShape::GetPointIndicesAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelBlendShape blendShape(usdPrim);
    return WrapAttribute(blendShape.GetPointIndicesAttr());
}

usd::UsdAttribute UsdSkelBlendShape::CreatePointIndicesAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelBlendShape blendShape(usdPrim);
    return WrapAttribute(blendShape.CreatePointIndicesAttr());
}

bool UsdSkelBlendShape::HasInbetween(const tf::TfToken& name) const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdSkelBlendShape blendShape(usdPrim);
    return blendShape.HasInbetween(GetTfToken(name));
}

bool UsdSkelBlendShape::ValidatePointIndices(const std::vector<int>& indices,
                                              size_t numPoints,
                                              std::string* reason) {
    TfSpan<const int> span(indices.data(), indices.size());
    return PXR_NS::UsdSkelBlendShape::ValidatePointIndices(span, numPoints, reason);
}

usd::UsdSchemaKind UsdSkelBlendShape::_GetSchemaKind() const {
    return usd::UsdSchemaKind::ConcreteTyped;
}

// ============================================================================
// UsdSkelAnimQuery Implementation
// ============================================================================

UsdSkelAnimQuery::UsdSkelAnimQuery() : impl_(nullptr) {}

UsdSkelAnimQuery::UsdSkelAnimQuery(const UsdSkelAnimQuery& other) : impl_(nullptr) {
    if (other.impl_) {
        impl_ = new PXR_NS::UsdSkelAnimQuery(*static_cast<PXR_NS::UsdSkelAnimQuery*>(other.impl_));
    }
}

UsdSkelAnimQuery::UsdSkelAnimQuery(UsdSkelAnimQuery&& other) noexcept : impl_(other.impl_) {
    other.impl_ = nullptr;
}

UsdSkelAnimQuery::~UsdSkelAnimQuery() {
    delete static_cast<PXR_NS::UsdSkelAnimQuery*>(impl_);
}

UsdSkelAnimQuery& UsdSkelAnimQuery::operator=(const UsdSkelAnimQuery& other) {
    if (this != &other) {
        delete static_cast<PXR_NS::UsdSkelAnimQuery*>(impl_);
        impl_ = nullptr;
        if (other.impl_) {
            impl_ = new PXR_NS::UsdSkelAnimQuery(*static_cast<PXR_NS::UsdSkelAnimQuery*>(other.impl_));
        }
    }
    return *this;
}

UsdSkelAnimQuery& UsdSkelAnimQuery::operator=(UsdSkelAnimQuery&& other) noexcept {
    if (this != &other) {
        delete static_cast<PXR_NS::UsdSkelAnimQuery*>(impl_);
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

bool UsdSkelAnimQuery::IsValid() const {
    return impl_ && static_cast<PXR_NS::UsdSkelAnimQuery*>(impl_)->IsValid();
}

usd::UsdPrim UsdSkelAnimQuery::GetPrim() const {
    if (!impl_) return usd::UsdPrim();
    return WrapPrim(static_cast<PXR_NS::UsdSkelAnimQuery*>(impl_)->GetPrim());
}

bool UsdSkelAnimQuery::ComputeJointLocalTransforms(std::vector<gf::Matrix4d>* xforms,
                                                    usd::UsdTimeCode time) const {
    if (!impl_ || !xforms) return false;

    VtMatrix4dArray usdXforms;
    UsdTimeCode usdTime = time.IsDefault() ? UsdTimeCode::Default() : UsdTimeCode(time.GetValue());
    if (!static_cast<PXR_NS::UsdSkelAnimQuery*>(impl_)->ComputeJointLocalTransforms(&usdXforms, usdTime)) {
        return false;
    }

    xforms->clear();
    xforms->reserve(usdXforms.size());
    for (const auto& mat : usdXforms) {
        xforms->push_back(WrapMatrix(mat));
    }
    return true;
}

bool UsdSkelAnimQuery::ComputeBlendShapeWeights(std::vector<float>* weights,
                                                 usd::UsdTimeCode time) const {
    if (!impl_ || !weights) return false;

    VtFloatArray usdWeights;
    UsdTimeCode usdTime = time.IsDefault() ? UsdTimeCode::Default() : UsdTimeCode(time.GetValue());
    if (!static_cast<PXR_NS::UsdSkelAnimQuery*>(impl_)->ComputeBlendShapeWeights(&usdWeights, usdTime)) {
        return false;
    }

    weights->assign(usdWeights.begin(), usdWeights.end());
    return true;
}

std::vector<tf::TfToken> UsdSkelAnimQuery::GetJointOrder() const {
    if (!impl_) return {};
    VtTokenArray tokens = static_cast<PXR_NS::UsdSkelAnimQuery*>(impl_)->GetJointOrder();
    std::vector<tf::TfToken> result;
    result.reserve(tokens.size());
    for (const auto& token : tokens) {
        result.push_back(WrapToken(token));
    }
    return result;
}

std::vector<tf::TfToken> UsdSkelAnimQuery::GetBlendShapeOrder() const {
    if (!impl_) return {};
    VtTokenArray tokens = static_cast<PXR_NS::UsdSkelAnimQuery*>(impl_)->GetBlendShapeOrder();
    std::vector<tf::TfToken> result;
    result.reserve(tokens.size());
    for (const auto& token : tokens) {
        result.push_back(WrapToken(token));
    }
    return result;
}

bool UsdSkelAnimQuery::JointTransformsMightBeTimeVarying() const {
    if (!impl_) return false;
    return static_cast<PXR_NS::UsdSkelAnimQuery*>(impl_)->JointTransformsMightBeTimeVarying();
}

bool UsdSkelAnimQuery::BlendShapeWeightsMightBeTimeVarying() const {
    if (!impl_) return false;
    return static_cast<PXR_NS::UsdSkelAnimQuery*>(impl_)->BlendShapeWeightsMightBeTimeVarying();
}

bool UsdSkelAnimQuery::operator==(const UsdSkelAnimQuery& other) const {
    if (!impl_ && !other.impl_) return true;
    if (!impl_ || !other.impl_) return false;
    return *static_cast<PXR_NS::UsdSkelAnimQuery*>(impl_) ==
           *static_cast<PXR_NS::UsdSkelAnimQuery*>(other.impl_);
}

bool UsdSkelAnimQuery::operator!=(const UsdSkelAnimQuery& other) const {
    return !(*this == other);
}

UsdSkelAnimQuery UsdSkelAnimQuery::FromImpl(void* impl) {
    UsdSkelAnimQuery result;
    result.impl_ = impl;
    return result;
}

// ============================================================================
// UsdSkelSkeletonQuery Implementation
// ============================================================================

UsdSkelSkeletonQuery::UsdSkelSkeletonQuery() : impl_(nullptr) {}

UsdSkelSkeletonQuery::UsdSkelSkeletonQuery(const UsdSkelSkeletonQuery& other) : impl_(nullptr) {
    if (other.impl_) {
        impl_ = new PXR_NS::UsdSkelSkeletonQuery(*static_cast<PXR_NS::UsdSkelSkeletonQuery*>(other.impl_));
    }
}

UsdSkelSkeletonQuery::UsdSkelSkeletonQuery(UsdSkelSkeletonQuery&& other) noexcept : impl_(other.impl_) {
    other.impl_ = nullptr;
}

UsdSkelSkeletonQuery::~UsdSkelSkeletonQuery() {
    delete static_cast<PXR_NS::UsdSkelSkeletonQuery*>(impl_);
}

UsdSkelSkeletonQuery& UsdSkelSkeletonQuery::operator=(const UsdSkelSkeletonQuery& other) {
    if (this != &other) {
        delete static_cast<PXR_NS::UsdSkelSkeletonQuery*>(impl_);
        impl_ = nullptr;
        if (other.impl_) {
            impl_ = new PXR_NS::UsdSkelSkeletonQuery(*static_cast<PXR_NS::UsdSkelSkeletonQuery*>(other.impl_));
        }
    }
    return *this;
}

UsdSkelSkeletonQuery& UsdSkelSkeletonQuery::operator=(UsdSkelSkeletonQuery&& other) noexcept {
    if (this != &other) {
        delete static_cast<PXR_NS::UsdSkelSkeletonQuery*>(impl_);
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

bool UsdSkelSkeletonQuery::IsValid() const {
    return impl_ && static_cast<PXR_NS::UsdSkelSkeletonQuery*>(impl_)->IsValid();
}

bool UsdSkelSkeletonQuery::HasBindPose() const {
    if (!impl_) return false;
    return static_cast<PXR_NS::UsdSkelSkeletonQuery*>(impl_)->HasBindPose();
}

bool UsdSkelSkeletonQuery::HasRestPose() const {
    if (!impl_) return false;
    return static_cast<PXR_NS::UsdSkelSkeletonQuery*>(impl_)->HasRestPose();
}

usd::UsdPrim UsdSkelSkeletonQuery::GetPrim() const {
    if (!impl_) return usd::UsdPrim();
    return WrapPrim(static_cast<PXR_NS::UsdSkelSkeletonQuery*>(impl_)->GetPrim());
}

UsdSkelSkeleton UsdSkelSkeletonQuery::GetSkeleton() const {
    if (!impl_) return UsdSkelSkeleton();
    const auto& skel = static_cast<PXR_NS::UsdSkelSkeletonQuery*>(impl_)->GetSkeleton();
    return UsdSkelSkeleton(WrapPrim(skel.GetPrim()));
}

UsdSkelAnimQuery UsdSkelSkeletonQuery::GetAnimQuery() const {
    if (!impl_) return UsdSkelAnimQuery();
    const auto& animQuery = static_cast<PXR_NS::UsdSkelSkeletonQuery*>(impl_)->GetAnimQuery();
    return UsdSkelAnimQuery::FromImpl(new PXR_NS::UsdSkelAnimQuery(animQuery));
}

UsdSkelTopology UsdSkelSkeletonQuery::GetTopology() const {
    if (!impl_) return UsdSkelTopology();
    const auto& topo = static_cast<PXR_NS::UsdSkelSkeletonQuery*>(impl_)->GetTopology();
    return UsdSkelTopology::FromImpl(new PXR_NS::UsdSkelTopology(topo));
}

std::vector<tf::TfToken> UsdSkelSkeletonQuery::GetJointOrder() const {
    if (!impl_) return {};
    VtTokenArray tokens = static_cast<PXR_NS::UsdSkelSkeletonQuery*>(impl_)->GetJointOrder();
    std::vector<tf::TfToken> result;
    result.reserve(tokens.size());
    for (const auto& token : tokens) {
        result.push_back(WrapToken(token));
    }
    return result;
}

bool UsdSkelSkeletonQuery::ComputeJointLocalTransforms(std::vector<gf::Matrix4d>* xforms,
                                                        usd::UsdTimeCode time,
                                                        bool atRest) const {
    if (!impl_ || !xforms) return false;

    VtMatrix4dArray usdXforms;
    UsdTimeCode usdTime = time.IsDefault() ? UsdTimeCode::Default() : UsdTimeCode(time.GetValue());
    if (!static_cast<PXR_NS::UsdSkelSkeletonQuery*>(impl_)->ComputeJointLocalTransforms(&usdXforms, usdTime, atRest)) {
        return false;
    }

    xforms->clear();
    xforms->reserve(usdXforms.size());
    for (const auto& mat : usdXforms) {
        xforms->push_back(WrapMatrix(mat));
    }
    return true;
}

bool UsdSkelSkeletonQuery::ComputeJointSkelTransforms(std::vector<gf::Matrix4d>* xforms,
                                                       usd::UsdTimeCode time,
                                                       bool atRest) const {
    if (!impl_ || !xforms) return false;

    VtMatrix4dArray usdXforms;
    UsdTimeCode usdTime = time.IsDefault() ? UsdTimeCode::Default() : UsdTimeCode(time.GetValue());
    if (!static_cast<PXR_NS::UsdSkelSkeletonQuery*>(impl_)->ComputeJointSkelTransforms(&usdXforms, usdTime, atRest)) {
        return false;
    }

    xforms->clear();
    xforms->reserve(usdXforms.size());
    for (const auto& mat : usdXforms) {
        xforms->push_back(WrapMatrix(mat));
    }
    return true;
}

bool UsdSkelSkeletonQuery::ComputeSkinningTransforms(std::vector<gf::Matrix4d>* xforms,
                                                      usd::UsdTimeCode time) const {
    if (!impl_ || !xforms) return false;

    VtMatrix4dArray usdXforms;
    UsdTimeCode usdTime = time.IsDefault() ? UsdTimeCode::Default() : UsdTimeCode(time.GetValue());
    if (!static_cast<PXR_NS::UsdSkelSkeletonQuery*>(impl_)->ComputeSkinningTransforms(&usdXforms, usdTime)) {
        return false;
    }

    xforms->clear();
    xforms->reserve(usdXforms.size());
    for (const auto& mat : usdXforms) {
        xforms->push_back(WrapMatrix(mat));
    }
    return true;
}

bool UsdSkelSkeletonQuery::GetJointWorldBindTransforms(std::vector<gf::Matrix4d>* xforms) const {
    if (!impl_ || !xforms) return false;

    VtMatrix4dArray usdXforms;
    if (!static_cast<PXR_NS::UsdSkelSkeletonQuery*>(impl_)->GetJointWorldBindTransforms(&usdXforms)) {
        return false;
    }

    xforms->clear();
    xforms->reserve(usdXforms.size());
    for (const auto& mat : usdXforms) {
        xforms->push_back(WrapMatrix(mat));
    }
    return true;
}

bool UsdSkelSkeletonQuery::operator==(const UsdSkelSkeletonQuery& other) const {
    if (!impl_ && !other.impl_) return true;
    if (!impl_ || !other.impl_) return false;
    return *static_cast<PXR_NS::UsdSkelSkeletonQuery*>(impl_) ==
           *static_cast<PXR_NS::UsdSkelSkeletonQuery*>(other.impl_);
}

bool UsdSkelSkeletonQuery::operator!=(const UsdSkelSkeletonQuery& other) const {
    return !(*this == other);
}

UsdSkelSkeletonQuery UsdSkelSkeletonQuery::FromImpl(void* impl) {
    UsdSkelSkeletonQuery result;
    result.impl_ = impl;
    return result;
}

// ============================================================================
// UsdSkelSkinningQuery Implementation
// ============================================================================

UsdSkelSkinningQuery::UsdSkelSkinningQuery() : impl_(nullptr) {}

UsdSkelSkinningQuery::UsdSkelSkinningQuery(const UsdSkelSkinningQuery& other) : impl_(nullptr) {
    if (other.impl_) {
        impl_ = new PXR_NS::UsdSkelSkinningQuery(*static_cast<PXR_NS::UsdSkelSkinningQuery*>(other.impl_));
    }
}

UsdSkelSkinningQuery::UsdSkelSkinningQuery(UsdSkelSkinningQuery&& other) noexcept : impl_(other.impl_) {
    other.impl_ = nullptr;
}

UsdSkelSkinningQuery::~UsdSkelSkinningQuery() {
    delete static_cast<PXR_NS::UsdSkelSkinningQuery*>(impl_);
}

UsdSkelSkinningQuery& UsdSkelSkinningQuery::operator=(const UsdSkelSkinningQuery& other) {
    if (this != &other) {
        delete static_cast<PXR_NS::UsdSkelSkinningQuery*>(impl_);
        impl_ = nullptr;
        if (other.impl_) {
            impl_ = new PXR_NS::UsdSkelSkinningQuery(*static_cast<PXR_NS::UsdSkelSkinningQuery*>(other.impl_));
        }
    }
    return *this;
}

UsdSkelSkinningQuery& UsdSkelSkinningQuery::operator=(UsdSkelSkinningQuery&& other) noexcept {
    if (this != &other) {
        delete static_cast<PXR_NS::UsdSkelSkinningQuery*>(impl_);
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

bool UsdSkelSkinningQuery::IsValid() const {
    return impl_ && static_cast<PXR_NS::UsdSkelSkinningQuery*>(impl_)->IsValid();
}

usd::UsdPrim UsdSkelSkinningQuery::GetPrim() const {
    if (!impl_) return usd::UsdPrim();
    return WrapPrim(static_cast<PXR_NS::UsdSkelSkinningQuery*>(impl_)->GetPrim());
}

bool UsdSkelSkinningQuery::HasBlendShapes() const {
    if (!impl_) return false;
    return static_cast<PXR_NS::UsdSkelSkinningQuery*>(impl_)->HasBlendShapes();
}

bool UsdSkelSkinningQuery::HasJointInfluences() const {
    if (!impl_) return false;
    return static_cast<PXR_NS::UsdSkelSkinningQuery*>(impl_)->HasJointInfluences();
}

int UsdSkelSkinningQuery::GetNumInfluencesPerComponent() const {
    if (!impl_) return 0;
    return static_cast<PXR_NS::UsdSkelSkinningQuery*>(impl_)->GetNumInfluencesPerComponent();
}

tf::TfToken UsdSkelSkinningQuery::GetInterpolation() const {
    if (!impl_) return tf::TfToken();
    return WrapToken(static_cast<PXR_NS::UsdSkelSkinningQuery*>(impl_)->GetInterpolation());
}

bool UsdSkelSkinningQuery::IsRigidlyDeformed() const {
    if (!impl_) return false;
    return static_cast<PXR_NS::UsdSkelSkinningQuery*>(impl_)->IsRigidlyDeformed();
}

tf::TfToken UsdSkelSkinningQuery::GetSkinningMethod() const {
    if (!impl_) return tf::TfToken();
    return WrapToken(static_cast<PXR_NS::UsdSkelSkinningQuery*>(impl_)->GetSkinningMethod());
}

gf::Matrix4d UsdSkelSkinningQuery::GetGeomBindTransform(usd::UsdTimeCode time) const {
    if (!impl_) return gf::Matrix4d::Identity();
    UsdTimeCode usdTime = time.IsDefault() ? UsdTimeCode::Default() : UsdTimeCode(time.GetValue());
    return WrapMatrix(static_cast<PXR_NS::UsdSkelSkinningQuery*>(impl_)->GetGeomBindTransform(usdTime));
}

bool UsdSkelSkinningQuery::ComputeJointInfluences(std::vector<int>* indices,
                                                   std::vector<float>* weights,
                                                   usd::UsdTimeCode time) const {
    if (!impl_ || !indices || !weights) return false;

    VtIntArray usdIndices;
    VtFloatArray usdWeights;
    UsdTimeCode usdTime = time.IsDefault() ? UsdTimeCode::Default() : UsdTimeCode(time.GetValue());

    if (!static_cast<PXR_NS::UsdSkelSkinningQuery*>(impl_)->ComputeJointInfluences(&usdIndices, &usdWeights, usdTime)) {
        return false;
    }

    indices->assign(usdIndices.begin(), usdIndices.end());
    weights->assign(usdWeights.begin(), usdWeights.end());
    return true;
}

bool UsdSkelSkinningQuery::ComputeSkinnedPoints(const std::vector<gf::Matrix4d>& xforms,
                                                 std::vector<gf::Vec3f>* points,
                                                 usd::UsdTimeCode time) const {
    if (!impl_ || !points) return false;

    VtMatrix4dArray usdXforms;
    usdXforms.reserve(xforms.size());
    for (const auto& mat : xforms) {
        usdXforms.push_back(GetGfMatrix(mat));
    }

    VtVec3fArray usdPoints;
    usdPoints.reserve(points->size());
    for (const auto& pt : *points) {
        usdPoints.push_back(GfVec3f(pt.x, pt.y, pt.z));
    }

    UsdTimeCode usdTime = time.IsDefault() ? UsdTimeCode::Default() : UsdTimeCode(time.GetValue());

    if (!static_cast<PXR_NS::UsdSkelSkinningQuery*>(impl_)->ComputeSkinnedPoints(usdXforms, &usdPoints, usdTime)) {
        return false;
    }

    points->clear();
    points->reserve(usdPoints.size());
    for (const auto& pt : usdPoints) {
        points->push_back(gf::Vec3f(pt[0], pt[1], pt[2]));
    }
    return true;
}

UsdSkelSkinningQuery UsdSkelSkinningQuery::FromImpl(void* impl) {
    UsdSkelSkinningQuery result;
    result.impl_ = impl;
    return result;
}

// ============================================================================
// UsdSkelCache Implementation
// ============================================================================

UsdSkelCache::UsdSkelCache() : impl_(new PXR_NS::UsdSkelCache()) {}

UsdSkelCache::UsdSkelCache(const UsdSkelCache& other)
    : impl_(new PXR_NS::UsdSkelCache(*static_cast<PXR_NS::UsdSkelCache*>(other.impl_))) {}

UsdSkelCache::UsdSkelCache(UsdSkelCache&& other) noexcept : impl_(other.impl_) {
    other.impl_ = nullptr;
}

UsdSkelCache::~UsdSkelCache() {
    delete static_cast<PXR_NS::UsdSkelCache*>(impl_);
}

UsdSkelCache& UsdSkelCache::operator=(const UsdSkelCache& other) {
    if (this != &other) {
        delete static_cast<PXR_NS::UsdSkelCache*>(impl_);
        impl_ = new PXR_NS::UsdSkelCache(*static_cast<PXR_NS::UsdSkelCache*>(other.impl_));
    }
    return *this;
}

UsdSkelCache& UsdSkelCache::operator=(UsdSkelCache&& other) noexcept {
    if (this != &other) {
        delete static_cast<PXR_NS::UsdSkelCache*>(impl_);
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

void UsdSkelCache::Clear() {
    if (impl_) {
        static_cast<PXR_NS::UsdSkelCache*>(impl_)->Clear();
    }
}

bool UsdSkelCache::Populate(const UsdSkelRoot& root) {
    if (!impl_) return false;
    UsdPrim rootPrim = GetUsdPrim(root.GetPrim());
    PXR_NS::UsdSkelRoot usdRoot(rootPrim);
    return static_cast<PXR_NS::UsdSkelCache*>(impl_)->Populate(usdRoot, UsdTraverseInstanceProxies());
}

UsdSkelSkeletonQuery UsdSkelCache::GetSkelQuery(const UsdSkelSkeleton& skel) const {
    if (!impl_) return UsdSkelSkeletonQuery();
    UsdPrim skelPrim = GetUsdPrim(skel.GetPrim());
    PXR_NS::UsdSkelSkeleton usdSkel(skelPrim);
    auto query = static_cast<PXR_NS::UsdSkelCache*>(impl_)->GetSkelQuery(usdSkel);
    return UsdSkelSkeletonQuery::FromImpl(new PXR_NS::UsdSkelSkeletonQuery(query));
}

UsdSkelAnimQuery UsdSkelCache::GetAnimQuery(const UsdSkelAnimation& anim) const {
    if (!impl_) return UsdSkelAnimQuery();
    UsdPrim animPrim = GetUsdPrim(anim.GetPrim());
    PXR_NS::UsdSkelAnimation usdAnim(animPrim);
    auto query = static_cast<PXR_NS::UsdSkelCache*>(impl_)->GetAnimQuery(usdAnim);
    return UsdSkelAnimQuery::FromImpl(new PXR_NS::UsdSkelAnimQuery(query));
}

UsdSkelSkinningQuery UsdSkelCache::GetSkinningQuery(const usd::UsdPrim& prim) const {
    if (!impl_) return UsdSkelSkinningQuery();
    UsdPrim usdPrim = GetUsdPrim(prim);
    auto query = static_cast<PXR_NS::UsdSkelCache*>(impl_)->GetSkinningQuery(usdPrim);
    return UsdSkelSkinningQuery::FromImpl(new PXR_NS::UsdSkelSkinningQuery(query));
}

// ============================================================================
// Token Constants Implementation
// ============================================================================

namespace UsdSkelTokens {

tf::TfToken skeleton() {
    static tf::TfToken token(TfToken("Skeleton").GetString());
    return token;
}

tf::TfToken skelRoot() {
    static tf::TfToken token(TfToken("SkelRoot").GetString());
    return token;
}

tf::TfToken skelAnimation() {
    static tf::TfToken token(TfToken("SkelAnimation").GetString());
    return token;
}

tf::TfToken blendShape() {
    static tf::TfToken token(TfToken("BlendShape").GetString());
    return token;
}

tf::TfToken classicLinear() {
    static tf::TfToken token(PXR_NS::UsdSkelTokens->classicLinear.GetString());
    return token;
}

tf::TfToken dualQuaternion() {
    static tf::TfToken token(PXR_NS::UsdSkelTokens->dualQuaternion.GetString());
    return token;
}

tf::TfToken constant() {
    static tf::TfToken token(TfToken("constant").GetString());
    return token;
}

tf::TfToken vertex() {
    static tf::TfToken token(TfToken("vertex").GetString());
    return token;
}

} // namespace UsdSkelTokens

// ============================================================================
// Utility Functions Implementation
// ============================================================================

bool ComputeJointLocalTransforms(const UsdSkelTopology& topology,
                                  const std::vector<gf::Vec3f>& translations,
                                  const std::vector<gf::Quatf>& rotations,
                                  const std::vector<gf::Vec3f>& scales,
                                  std::vector<gf::Matrix4d>* xforms) {
    if (!xforms) return false;

    // Convert to USD types
    VtVec3fArray usdTranslations;
    usdTranslations.reserve(translations.size());
    for (const auto& t : translations) {
        usdTranslations.push_back(GfVec3f(t.x, t.y, t.z));
    }

    VtQuatfArray usdRotations;
    usdRotations.reserve(rotations.size());
    for (const auto& r : rotations) {
        auto imag = r.GetImaginary();
        usdRotations.push_back(GfQuatf(r.GetReal(), imag.x, imag.y, imag.z));
    }

    VtVec3hArray usdScales;
    usdScales.reserve(scales.size());
    for (const auto& s : scales) {
        usdScales.push_back(GfVec3h(s.x, s.y, s.z));
    }

    VtMatrix4dArray usdXforms;
    if (!PXR_NS::UsdSkelMakeTransforms(
            TfSpan<const GfVec3f>(usdTranslations.data(), usdTranslations.size()),
            TfSpan<const GfQuatf>(usdRotations.data(), usdRotations.size()),
            TfSpan<const GfVec3h>(usdScales.data(), usdScales.size()),
            TfSpan<GfMatrix4d>(usdXforms.data(), usdXforms.size()))) {
        return false;
    }

    xforms->clear();
    xforms->reserve(usdXforms.size());
    for (const auto& mat : usdXforms) {
        xforms->push_back(WrapMatrix(mat));
    }
    return true;
}

bool ConcatJointTransforms(const UsdSkelTopology& topology,
                            const std::vector<gf::Matrix4d>& localXforms,
                            std::vector<gf::Matrix4d>* skelXforms) {
    if (!skelXforms || !topology.GetImpl()) return false;

    VtMatrix4dArray usdLocalXforms;
    usdLocalXforms.reserve(localXforms.size());
    for (const auto& mat : localXforms) {
        usdLocalXforms.push_back(GetGfMatrix(mat));
    }

    VtMatrix4dArray usdSkelXforms;
    const auto& topo = *static_cast<PXR_NS::UsdSkelTopology*>(topology.GetImpl());
    if (!PXR_NS::UsdSkelConcatJointTransforms(
            topo,
            TfSpan<const GfMatrix4d>(usdLocalXforms.data(), usdLocalXforms.size()),
            TfSpan<GfMatrix4d>(usdSkelXforms.data(), usdSkelXforms.size()))) {
        return false;
    }

    skelXforms->clear();
    skelXforms->reserve(usdSkelXforms.size());
    for (const auto& mat : usdSkelXforms) {
        skelXforms->push_back(WrapMatrix(mat));
    }
    return true;
}

bool ComputeSkinningTransforms(const std::vector<gf::Matrix4d>& skelXforms,
                                const std::vector<gf::Matrix4d>& bindXforms,
                                std::vector<gf::Matrix4d>* skinningXforms) {
    if (!skinningXforms || skelXforms.size() != bindXforms.size()) return false;

    skinningXforms->clear();
    skinningXforms->reserve(skelXforms.size());
    for (size_t i = 0; i < skelXforms.size(); ++i) {
        // skinningTransform = inverse(bindTransform) * skelTransform
        gf::Matrix4d invBind = bindXforms[i].GetInverse();
        skinningXforms->push_back(invBind * skelXforms[i]);
    }
    return true;
}

bool DeformPointsWithLBS(const std::vector<gf::Matrix4d>& skinningXforms,
                          const std::vector<int>& jointIndices,
                          const std::vector<float>& jointWeights,
                          int numInfluencesPerPoint,
                          std::vector<gf::Vec3f>* points) {
    if (!points || jointIndices.size() != jointWeights.size()) return false;

    VtMatrix4dArray usdXforms;
    usdXforms.reserve(skinningXforms.size());
    for (const auto& mat : skinningXforms) {
        usdXforms.push_back(GetGfMatrix(mat));
    }

    VtVec3fArray usdPoints;
    usdPoints.reserve(points->size());
    for (const auto& pt : *points) {
        usdPoints.push_back(GfVec3f(pt.x, pt.y, pt.z));
    }

    if (!PXR_NS::UsdSkelSkinPointsLBS(
            GfMatrix4d(1),
            TfSpan<const GfMatrix4d>(usdXforms.data(), usdXforms.size()),
            TfSpan<const int>(jointIndices.data(), jointIndices.size()),
            TfSpan<const float>(jointWeights.data(), jointWeights.size()),
            numInfluencesPerPoint,
            TfSpan<GfVec3f>(usdPoints.data(), usdPoints.size()))) {
        return false;
    }

    points->clear();
    points->reserve(usdPoints.size());
    for (const auto& pt : usdPoints) {
        points->push_back(gf::Vec3f(pt[0], pt[1], pt[2]));
    }
    return true;
}

} // namespace usdSkel
} // namespace cxxfacade
