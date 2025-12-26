// Usd.cpp - Universal Scene Description implementation
#include "../../include/usd/Usd.hpp"

#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usd/property.h>
#include <pxr/usd/usd/attribute.h>
#include <pxr/usd/usd/relationship.h>
#include <pxr/usd/usd/primRange.h>
#include <pxr/usd/usd/editTarget.h>
#include <pxr/usd/usd/editContext.h>
#include <pxr/usd/usd/schemaBase.h>
#include <pxr/usd/usd/typed.h>
#include <pxr/usd/usd/apiSchemaBase.h>
#include <pxr/usd/usdGeom/tokens.h>
#include <pxr/usd/usdGeom/metrics.h>
#include <pxr/usd/sdf/types.h>

PXR_NAMESPACE_USING_DIRECTIVE

namespace cxxfacade {
namespace usd {

// ============================================================================
// UsdTimeCode implementation
// ============================================================================

UsdTimeCode UsdTimeCode::EarliestTime() {
    return UsdTimeCode(pxr::UsdTimeCode::EarliestTime().GetValue());
}

// Helper to convert our UsdTimeCode to pxr::UsdTimeCode
static inline pxr::UsdTimeCode toPxrTimeCode(const UsdTimeCode& tc) {
    if (tc.IsDefault()) {
        return pxr::UsdTimeCode::Default();
    }
    return pxr::UsdTimeCode(tc.GetValue());
}

// ============================================================================
// UsdAttribute implementation
// ============================================================================

static inline pxr::UsdAttribute* asAttr(void* impl) {
    return static_cast<pxr::UsdAttribute*>(impl);
}

UsdAttribute::UsdAttribute() : impl_(new pxr::UsdAttribute()) {}

UsdAttribute::UsdAttribute(const UsdAttribute& other)
    : impl_(new pxr::UsdAttribute(*asAttr(other.impl_))) {}

UsdAttribute::UsdAttribute(UsdAttribute&& other) noexcept : impl_(other.impl_) {
    other.impl_ = nullptr;
}

UsdAttribute::~UsdAttribute() {
    delete asAttr(impl_);
}

UsdAttribute& UsdAttribute::operator=(const UsdAttribute& other) {
    if (this != &other) {
        delete asAttr(impl_);
        impl_ = new pxr::UsdAttribute(*asAttr(other.impl_));
    }
    return *this;
}

UsdAttribute& UsdAttribute::operator=(UsdAttribute&& other) noexcept {
    if (this != &other) {
        delete asAttr(impl_);
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

bool UsdAttribute::IsValid() const {
    return impl_ && asAttr(impl_)->IsValid();
}

tf::TfToken UsdAttribute::GetName() const {
    if (!IsValid()) return tf::TfToken();
    return tf::TfToken::FromImpl(new pxr::TfToken(asAttr(impl_)->GetName()));
}

sdf::SdfPath UsdAttribute::GetPath() const {
    if (!IsValid()) return sdf::SdfPath();
    return sdf::SdfPath::FromImpl(new pxr::SdfPath(asAttr(impl_)->GetPath()));
}

bool UsdAttribute::Get(vt::VtValue* value, UsdTimeCode time) const {
    if (!IsValid() || !value) return false;
    pxr::VtValue pxrValue;
    bool result = asAttr(impl_)->Get(&pxrValue, toPxrTimeCode(time));
    if (result) {
        *value = vt::VtValue::FromImpl(new pxr::VtValue(pxrValue));
    }
    return result;
}

bool UsdAttribute::Set(const vt::VtValue& value, UsdTimeCode time) {
    if (!IsValid()) return false;
    pxr::VtValue* pxrValue = static_cast<pxr::VtValue*>(value.GetImpl());
    return asAttr(impl_)->Set(*pxrValue, toPxrTimeCode(time));
}

std::string UsdAttribute::GetTypeName() const {
    if (!IsValid()) return "";
    return asAttr(impl_)->GetTypeName().GetAsToken().GetString();
}

bool UsdAttribute::HasValue() const {
    return IsValid() && asAttr(impl_)->HasValue();
}

bool UsdAttribute::HasAuthoredValue() const {
    return IsValid() && asAttr(impl_)->HasAuthoredValue();
}

UsdAttribute UsdAttribute::FromImpl(void* impl) {
    UsdAttribute attr;
    delete asAttr(attr.impl_);
    attr.impl_ = impl;
    return attr;
}

// ============================================================================
// UsdRelationship implementation
// ============================================================================

static inline pxr::UsdRelationship* asRel(void* impl) {
    return static_cast<pxr::UsdRelationship*>(impl);
}

UsdRelationship::UsdRelationship() : impl_(new pxr::UsdRelationship()) {}

UsdRelationship::UsdRelationship(const UsdRelationship& other)
    : impl_(new pxr::UsdRelationship(*asRel(other.impl_))) {}

UsdRelationship::UsdRelationship(UsdRelationship&& other) noexcept : impl_(other.impl_) {
    other.impl_ = nullptr;
}

UsdRelationship::~UsdRelationship() {
    delete asRel(impl_);
}

UsdRelationship& UsdRelationship::operator=(const UsdRelationship& other) {
    if (this != &other) {
        delete asRel(impl_);
        impl_ = new pxr::UsdRelationship(*asRel(other.impl_));
    }
    return *this;
}

UsdRelationship& UsdRelationship::operator=(UsdRelationship&& other) noexcept {
    if (this != &other) {
        delete asRel(impl_);
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

bool UsdRelationship::IsValid() const {
    return impl_ && asRel(impl_)->IsValid();
}

tf::TfToken UsdRelationship::GetName() const {
    if (!IsValid()) return tf::TfToken();
    return tf::TfToken::FromImpl(new pxr::TfToken(asRel(impl_)->GetName()));
}

sdf::SdfPath UsdRelationship::GetPath() const {
    if (!IsValid()) return sdf::SdfPath();
    return sdf::SdfPath::FromImpl(new pxr::SdfPath(asRel(impl_)->GetPath()));
}

bool UsdRelationship::GetTargets(std::vector<sdf::SdfPath>* targets) const {
    if (!IsValid() || !targets) return false;
    pxr::SdfPathVector pxrTargets;
    bool result = asRel(impl_)->GetTargets(&pxrTargets);
    if (result) {
        targets->clear();
        for (const auto& p : pxrTargets) {
            targets->push_back(sdf::SdfPath::FromImpl(new pxr::SdfPath(p)));
        }
    }
    return result;
}

bool UsdRelationship::SetTargets(const std::vector<sdf::SdfPath>& targets) {
    if (!IsValid()) return false;
    pxr::SdfPathVector pxrTargets;
    for (const auto& t : targets) {
        pxrTargets.push_back(*static_cast<pxr::SdfPath*>(t.GetImpl()));
    }
    return asRel(impl_)->SetTargets(pxrTargets);
}

bool UsdRelationship::AddTarget(const sdf::SdfPath& target) {
    if (!IsValid()) return false;
    return asRel(impl_)->AddTarget(*static_cast<pxr::SdfPath*>(target.GetImpl()));
}

UsdRelationship UsdRelationship::FromImpl(void* impl) {
    UsdRelationship rel;
    delete asRel(rel.impl_);
    rel.impl_ = impl;
    return rel;
}

// ============================================================================
// UsdPrim implementation
// ============================================================================

static inline pxr::UsdPrim* asPrim(void* impl) {
    return static_cast<pxr::UsdPrim*>(impl);
}

UsdPrim::UsdPrim() : impl_(new pxr::UsdPrim()) {}

UsdPrim::UsdPrim(const UsdPrim& other)
    : impl_(new pxr::UsdPrim(*asPrim(other.impl_))) {}

UsdPrim::UsdPrim(UsdPrim&& other) noexcept : impl_(other.impl_) {
    other.impl_ = nullptr;
}

UsdPrim::~UsdPrim() {
    delete asPrim(impl_);
}

UsdPrim& UsdPrim::operator=(const UsdPrim& other) {
    if (this != &other) {
        delete asPrim(impl_);
        impl_ = new pxr::UsdPrim(*asPrim(other.impl_));
    }
    return *this;
}

UsdPrim& UsdPrim::operator=(UsdPrim&& other) noexcept {
    if (this != &other) {
        delete asPrim(impl_);
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

bool UsdPrim::IsValid() const {
    return impl_ && asPrim(impl_)->IsValid();
}

bool UsdPrim::IsActive() const {
    return IsValid() && asPrim(impl_)->IsActive();
}

bool UsdPrim::IsDefined() const {
    return IsValid() && asPrim(impl_)->IsDefined();
}

tf::TfToken UsdPrim::GetName() const {
    if (!IsValid()) return tf::TfToken();
    return tf::TfToken::FromImpl(new pxr::TfToken(asPrim(impl_)->GetName()));
}

sdf::SdfPath UsdPrim::GetPath() const {
    if (!IsValid()) return sdf::SdfPath();
    return sdf::SdfPath::FromImpl(new pxr::SdfPath(asPrim(impl_)->GetPath()));
}

tf::TfToken UsdPrim::GetTypeName() const {
    if (!IsValid()) return tf::TfToken();
    return tf::TfToken::FromImpl(new pxr::TfToken(asPrim(impl_)->GetTypeName()));
}

UsdPrim UsdPrim::GetParent() const {
    if (!IsValid()) return UsdPrim();
    return UsdPrim::FromImpl(new pxr::UsdPrim(asPrim(impl_)->GetParent()));
}

UsdStage UsdPrim::GetStage() const {
    if (!IsValid()) return UsdStage();
    pxr::UsdStagePtr stage = asPrim(impl_)->GetStage();
    if (!stage) return UsdStage();

    struct StageHandle {
        pxr::UsdStageRefPtr ptr;
        explicit StageHandle(const pxr::UsdStagePtr& p) : ptr(p) {}
    };
    return UsdStage::FromImpl(new StageHandle(stage));
}

std::vector<UsdPrim> UsdPrim::GetChildren() const {
    std::vector<UsdPrim> result;
    if (!IsValid()) return result;
    for (const auto& child : asPrim(impl_)->GetChildren()) {
        result.push_back(UsdPrim::FromImpl(new pxr::UsdPrim(child)));
    }
    return result;
}

UsdAttribute UsdPrim::GetAttribute(const tf::TfToken& name) const {
    if (!IsValid()) return UsdAttribute();
    pxr::TfToken* pxrToken = static_cast<pxr::TfToken*>(name.GetImpl());
    return UsdAttribute::FromImpl(new pxr::UsdAttribute(asPrim(impl_)->GetAttribute(*pxrToken)));
}

UsdAttribute UsdPrim::CreateAttribute(const tf::TfToken& name,
                                        const std::string& typeName,
                                        bool custom) {
    if (!IsValid()) return UsdAttribute();
    pxr::TfToken* pxrToken = static_cast<pxr::TfToken*>(name.GetImpl());
    pxr::SdfValueTypeName valueType = pxr::SdfSchema::GetInstance().FindType(typeName);
    return UsdAttribute::FromImpl(new pxr::UsdAttribute(
        asPrim(impl_)->CreateAttribute(*pxrToken, valueType, custom)));
}

UsdRelationship UsdPrim::GetRelationship(const tf::TfToken& name) const {
    if (!IsValid()) return UsdRelationship();
    pxr::TfToken* pxrToken = static_cast<pxr::TfToken*>(name.GetImpl());
    return UsdRelationship::FromImpl(new pxr::UsdRelationship(asPrim(impl_)->GetRelationship(*pxrToken)));
}

UsdRelationship UsdPrim::CreateRelationship(const tf::TfToken& name, bool custom) {
    if (!IsValid()) return UsdRelationship();
    pxr::TfToken* pxrToken = static_cast<pxr::TfToken*>(name.GetImpl());
    return UsdRelationship::FromImpl(new pxr::UsdRelationship(
        asPrim(impl_)->CreateRelationship(*pxrToken, custom)));
}

bool UsdPrim::HasAPI(const tf::TfToken& schemaName) const {
    if (!IsValid()) return false;
    pxr::TfToken* pxrToken = static_cast<pxr::TfToken*>(schemaName.GetImpl());
    return asPrim(impl_)->HasAPI(pxr::TfType::FindByName(pxrToken->GetString()));
}

bool UsdPrim::ApplyAPI(const tf::TfToken& schemaName) {
    if (!IsValid()) return false;
    pxr::TfToken* pxrToken = static_cast<pxr::TfToken*>(schemaName.GetImpl());
    return asPrim(impl_)->ApplyAPI(pxr::TfType::FindByName(pxrToken->GetString()));
}

bool UsdPrim::operator==(const UsdPrim& other) const {
    if (!impl_ && !other.impl_) return true;
    if (!impl_ || !other.impl_) return false;
    return *asPrim(impl_) == *asPrim(other.impl_);
}

UsdPrim UsdPrim::FromImpl(void* impl) {
    UsdPrim prim;
    delete asPrim(prim.impl_);
    prim.impl_ = impl;
    return prim;
}

// ============================================================================
// UsdStage implementation
// ============================================================================

struct StageHandle {
    pxr::UsdStageRefPtr ptr;
    StageHandle() = default;
    explicit StageHandle(const pxr::UsdStageRefPtr& p) : ptr(p) {}
};

static inline StageHandle* asStageHandle(void* impl) {
    return static_cast<StageHandle*>(impl);
}

UsdStage::UsdStage() : impl_(new StageHandle()) {}

UsdStage::UsdStage(const UsdStage& other)
    : impl_(new StageHandle(asStageHandle(other.impl_)->ptr)) {}

UsdStage::UsdStage(UsdStage&& other) noexcept : impl_(other.impl_) {
    other.impl_ = nullptr;
}

UsdStage::~UsdStage() {
    delete asStageHandle(impl_);
}

UsdStage& UsdStage::operator=(const UsdStage& other) {
    if (this != &other) {
        delete asStageHandle(impl_);
        impl_ = new StageHandle(asStageHandle(other.impl_)->ptr);
    }
    return *this;
}

UsdStage& UsdStage::operator=(UsdStage&& other) noexcept {
    if (this != &other) {
        delete asStageHandle(impl_);
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

bool UsdStage::IsValid() const {
    return impl_ && asStageHandle(impl_)->ptr;
}

UsdStage UsdStage::CreateInMemory() {
    UsdStage stage;
    asStageHandle(stage.impl_)->ptr = pxr::UsdStage::CreateInMemory();
    return stage;
}

UsdStage UsdStage::CreateNew(const std::string& identifier) {
    UsdStage stage;
    asStageHandle(stage.impl_)->ptr = pxr::UsdStage::CreateNew(identifier);
    return stage;
}

UsdStage UsdStage::Open(const std::string& identifier) {
    UsdStage stage;
    asStageHandle(stage.impl_)->ptr = pxr::UsdStage::Open(identifier);
    return stage;
}

sdf::SdfLayer UsdStage::GetRootLayer() const {
    if (!IsValid()) return sdf::SdfLayer();

    // SdfLayer stores its own SdfLayerHandle internally
    struct SdfLayerHandle {
        pxr::SdfLayerRefPtr ptr;
        SdfLayerHandle() = default;
        explicit SdfLayerHandle(const pxr::SdfLayerRefPtr& p) : ptr(p) {}
    };

    auto* handle = new SdfLayerHandle(asStageHandle(impl_)->ptr->GetRootLayer());
    return sdf::SdfLayer::FromImpl(handle);
}

UsdPrim UsdStage::GetPseudoRoot() const {
    if (!IsValid()) return UsdPrim();
    return UsdPrim::FromImpl(new pxr::UsdPrim(asStageHandle(impl_)->ptr->GetPseudoRoot()));
}

UsdPrim UsdStage::GetDefaultPrim() const {
    if (!IsValid()) return UsdPrim();
    return UsdPrim::FromImpl(new pxr::UsdPrim(asStageHandle(impl_)->ptr->GetDefaultPrim()));
}

bool UsdStage::SetDefaultPrim(const UsdPrim& prim) {
    if (!IsValid() || !prim.IsValid()) return false;
    asStageHandle(impl_)->ptr->SetDefaultPrim(*static_cast<pxr::UsdPrim*>(prim.GetImpl()));
    return true;
}

UsdPrim UsdStage::GetPrimAtPath(const sdf::SdfPath& path) const {
    if (!IsValid()) return UsdPrim();
    pxr::SdfPath* pxrPath = static_cast<pxr::SdfPath*>(path.GetImpl());
    return UsdPrim::FromImpl(new pxr::UsdPrim(asStageHandle(impl_)->ptr->GetPrimAtPath(*pxrPath)));
}

UsdPrim UsdStage::DefinePrim(const sdf::SdfPath& path, const tf::TfToken& typeName) {
    if (!IsValid()) return UsdPrim();
    pxr::SdfPath* pxrPath = static_cast<pxr::SdfPath*>(path.GetImpl());
    pxr::TfToken* pxrToken = static_cast<pxr::TfToken*>(typeName.GetImpl());
    return UsdPrim::FromImpl(new pxr::UsdPrim(asStageHandle(impl_)->ptr->DefinePrim(*pxrPath, *pxrToken)));
}

UsdPrim UsdStage::OverridePrim(const sdf::SdfPath& path) {
    if (!IsValid()) return UsdPrim();
    pxr::SdfPath* pxrPath = static_cast<pxr::SdfPath*>(path.GetImpl());
    return UsdPrim::FromImpl(new pxr::UsdPrim(asStageHandle(impl_)->ptr->OverridePrim(*pxrPath)));
}

bool UsdStage::RemovePrim(const sdf::SdfPath& path) {
    if (!IsValid()) return false;
    pxr::SdfPath* pxrPath = static_cast<pxr::SdfPath*>(path.GetImpl());
    return asStageHandle(impl_)->ptr->RemovePrim(*pxrPath);
}

std::vector<UsdPrim> UsdStage::Traverse() const {
    std::vector<UsdPrim> result;
    if (!IsValid()) return result;
    for (const auto& prim : asStageHandle(impl_)->ptr->Traverse()) {
        result.push_back(UsdPrim::FromImpl(new pxr::UsdPrim(prim)));
    }
    return result;
}

tf::TfToken UsdStage::GetUpAxis() const {
    if (!IsValid()) return tf::TfToken();
    return tf::TfToken::FromImpl(new pxr::TfToken(
        pxr::UsdGeomGetStageUpAxis(asStageHandle(impl_)->ptr)));
}

bool UsdStage::SetUpAxis(const tf::TfToken& axis) {
    if (!IsValid()) return false;
    pxr::TfToken* pxrToken = static_cast<pxr::TfToken*>(axis.GetImpl());
    return pxr::UsdGeomSetStageUpAxis(asStageHandle(impl_)->ptr, *pxrToken);
}

double UsdStage::GetMetersPerUnit() const {
    if (!IsValid()) return 1.0;
    return pxr::UsdGeomGetStageMetersPerUnit(asStageHandle(impl_)->ptr);
}

bool UsdStage::SetMetersPerUnit(double metersPerUnit) {
    if (!IsValid()) return false;
    return pxr::UsdGeomSetStageMetersPerUnit(asStageHandle(impl_)->ptr, metersPerUnit);
}

double UsdStage::GetStartTimeCode() const {
    if (!IsValid()) return 0.0;
    return asStageHandle(impl_)->ptr->GetStartTimeCode();
}

double UsdStage::GetEndTimeCode() const {
    if (!IsValid()) return 0.0;
    return asStageHandle(impl_)->ptr->GetEndTimeCode();
}

void UsdStage::SetStartTimeCode(double startTime) {
    if (IsValid()) {
        asStageHandle(impl_)->ptr->SetStartTimeCode(startTime);
    }
}

void UsdStage::SetEndTimeCode(double endTime) {
    if (IsValid()) {
        asStageHandle(impl_)->ptr->SetEndTimeCode(endTime);
    }
}

bool UsdStage::Save() const {
    if (!IsValid()) return false;
    asStageHandle(impl_)->ptr->Save();
    return true;
}

bool UsdStage::Export(const std::string& filename) const {
    if (!IsValid()) return false;
    return asStageHandle(impl_)->ptr->Export(filename);
}

UsdStage UsdStage::FromImpl(void* impl) {
    UsdStage stage;
    delete asStageHandle(stage.impl_);
    stage.impl_ = impl;
    return stage;
}

// ============================================================================
// UsdEditTarget implementation
// ============================================================================

struct EditTargetHandle {
    pxr::UsdEditTarget target;
    EditTargetHandle() = default;
    explicit EditTargetHandle(const pxr::UsdEditTarget& t) : target(t) {}
};

static inline EditTargetHandle* asEditTarget(void* impl) {
    return static_cast<EditTargetHandle*>(impl);
}

UsdEditTarget::UsdEditTarget() : impl_(new EditTargetHandle()) {}

UsdEditTarget::UsdEditTarget(const sdf::SdfLayer& layer) : impl_(nullptr) {
    struct SdfLayerHandle {
        pxr::SdfLayerRefPtr ptr;
    };
    auto* layerHandle = static_cast<SdfLayerHandle*>(layer.GetImpl());
    if (layerHandle && layerHandle->ptr) {
        impl_ = new EditTargetHandle(pxr::UsdEditTarget(layerHandle->ptr));
    } else {
        impl_ = new EditTargetHandle();
    }
}

UsdEditTarget::UsdEditTarget(const UsdEditTarget& other)
    : impl_(new EditTargetHandle(asEditTarget(other.impl_)->target)) {}

UsdEditTarget::UsdEditTarget(UsdEditTarget&& other) noexcept : impl_(other.impl_) {
    other.impl_ = nullptr;
}

UsdEditTarget::~UsdEditTarget() {
    delete asEditTarget(impl_);
}

UsdEditTarget& UsdEditTarget::operator=(const UsdEditTarget& other) {
    if (this != &other) {
        delete asEditTarget(impl_);
        impl_ = new EditTargetHandle(asEditTarget(other.impl_)->target);
    }
    return *this;
}

UsdEditTarget& UsdEditTarget::operator=(UsdEditTarget&& other) noexcept {
    if (this != &other) {
        delete asEditTarget(impl_);
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

bool UsdEditTarget::IsNull() const {
    return !impl_ || asEditTarget(impl_)->target.IsNull();
}

bool UsdEditTarget::IsValid() const {
    return impl_ && asEditTarget(impl_)->target.IsValid();
}

sdf::SdfLayer UsdEditTarget::GetLayer() const {
    if (!impl_) return sdf::SdfLayer();

    struct SdfLayerHandle {
        pxr::SdfLayerRefPtr ptr;
        SdfLayerHandle() = default;
        explicit SdfLayerHandle(const pxr::SdfLayerHandle& h) : ptr(h) {}
    };

    pxr::SdfLayerHandle layerHandle = asEditTarget(impl_)->target.GetLayer();
    if (!layerHandle) return sdf::SdfLayer();
    return sdf::SdfLayer::FromImpl(new SdfLayerHandle(layerHandle));
}

sdf::SdfPath UsdEditTarget::MapToSpecPath(const sdf::SdfPath& scenePath) const {
    if (!impl_) return sdf::SdfPath();
    pxr::SdfPath* pxrPath = static_cast<pxr::SdfPath*>(scenePath.GetImpl());
    pxr::SdfPath mappedPath = asEditTarget(impl_)->target.MapToSpecPath(*pxrPath);
    return sdf::SdfPath::FromImpl(new pxr::SdfPath(mappedPath));
}

bool UsdEditTarget::operator==(const UsdEditTarget& other) const {
    if (!impl_ && !other.impl_) return true;
    if (!impl_ || !other.impl_) return false;
    return asEditTarget(impl_)->target == asEditTarget(other.impl_)->target;
}

bool UsdEditTarget::operator!=(const UsdEditTarget& other) const {
    return !(*this == other);
}

UsdEditTarget UsdEditTarget::FromImpl(void* impl) {
    UsdEditTarget target;
    delete asEditTarget(target.impl_);
    target.impl_ = impl;
    return target;
}

// ============================================================================
// UsdEditContext implementation
// ============================================================================

struct EditContextHandle {
    pxr::UsdStageRefPtr stage;
    pxr::UsdEditTarget originalTarget;
    bool valid;

    EditContextHandle() : valid(false) {}
};

static inline EditContextHandle* asEditContext(void* impl) {
    return static_cast<EditContextHandle*>(impl);
}

UsdEditContext::UsdEditContext(UsdStage& stage) : impl_(new EditContextHandle()) {
    if (stage.IsValid()) {
        auto* handle = asEditContext(impl_);
        handle->stage = asStageHandle(stage.GetImpl())->ptr;
        handle->originalTarget = handle->stage->GetEditTarget();
        handle->valid = true;
    }
}

UsdEditContext::UsdEditContext(UsdStage& stage, const UsdEditTarget& editTarget)
    : impl_(new EditContextHandle()) {
    if (stage.IsValid()) {
        auto* handle = asEditContext(impl_);
        handle->stage = asStageHandle(stage.GetImpl())->ptr;
        handle->originalTarget = handle->stage->GetEditTarget();
        handle->stage->SetEditTarget(asEditTarget(editTarget.GetImpl())->target);
        handle->valid = true;
    }
}

UsdEditContext::~UsdEditContext() {
    auto* handle = asEditContext(impl_);
    if (handle && handle->valid && handle->stage) {
        handle->stage->SetEditTarget(handle->originalTarget);
    }
    delete handle;
}

// ============================================================================
// UsdProperty implementation
// ============================================================================

static inline pxr::UsdProperty* asProp(void* impl) {
    return static_cast<pxr::UsdProperty*>(impl);
}

UsdProperty::UsdProperty() : impl_(new pxr::UsdProperty()) {}

UsdProperty::UsdProperty(const UsdProperty& other)
    : impl_(new pxr::UsdProperty(*asProp(other.impl_))) {}

UsdProperty::UsdProperty(UsdProperty&& other) noexcept : impl_(other.impl_) {
    other.impl_ = nullptr;
}

UsdProperty::~UsdProperty() {
    delete asProp(impl_);
}

UsdProperty& UsdProperty::operator=(const UsdProperty& other) {
    if (this != &other) {
        delete asProp(impl_);
        impl_ = new pxr::UsdProperty(*asProp(other.impl_));
    }
    return *this;
}

UsdProperty& UsdProperty::operator=(UsdProperty&& other) noexcept {
    if (this != &other) {
        delete asProp(impl_);
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

bool UsdProperty::IsValid() const {
    return impl_ && asProp(impl_)->IsValid();
}

tf::TfToken UsdProperty::GetName() const {
    if (!IsValid()) return tf::TfToken();
    return tf::TfToken::FromImpl(new pxr::TfToken(asProp(impl_)->GetName()));
}

sdf::SdfPath UsdProperty::GetPath() const {
    if (!IsValid()) return sdf::SdfPath();
    return sdf::SdfPath::FromImpl(new pxr::SdfPath(asProp(impl_)->GetPath()));
}

tf::TfToken UsdProperty::GetBaseName() const {
    if (!IsValid()) return tf::TfToken();
    return tf::TfToken::FromImpl(new pxr::TfToken(asProp(impl_)->GetBaseName()));
}

tf::TfToken UsdProperty::GetNamespace() const {
    if (!IsValid()) return tf::TfToken();
    return tf::TfToken::FromImpl(new pxr::TfToken(asProp(impl_)->GetNamespace()));
}

bool UsdProperty::IsCustom() const {
    return IsValid() && asProp(impl_)->IsCustom();
}

bool UsdProperty::IsDefined() const {
    return IsValid() && asProp(impl_)->IsDefined();
}

bool UsdProperty::IsAuthored() const {
    return IsValid() && asProp(impl_)->IsAuthored();
}

std::string UsdProperty::GetDisplayGroup() const {
    if (!IsValid()) return "";
    return asProp(impl_)->GetDisplayGroup();
}

bool UsdProperty::SetDisplayGroup(const std::string& displayGroup) {
    if (!IsValid()) return false;
    return asProp(impl_)->SetDisplayGroup(displayGroup);
}

bool UsdProperty::IsAttribute() const {
    if (!IsValid()) return false;
    return asProp(impl_)->Is<pxr::UsdAttribute>();
}

bool UsdProperty::IsRelationship() const {
    if (!IsValid()) return false;
    return asProp(impl_)->Is<pxr::UsdRelationship>();
}

bool UsdProperty::operator==(const UsdProperty& other) const {
    if (!impl_ && !other.impl_) return true;
    if (!impl_ || !other.impl_) return false;
    return *asProp(impl_) == *asProp(other.impl_);
}

UsdProperty UsdProperty::FromImpl(void* impl) {
    UsdProperty prop;
    delete asProp(prop.impl_);
    prop.impl_ = impl;
    return prop;
}

// ============================================================================
// UsdStage additional methods for EditTarget
// ============================================================================

UsdEditTarget UsdStage::GetEditTarget() const {
    if (!IsValid()) return UsdEditTarget();
    pxr::UsdEditTarget target = asStageHandle(impl_)->ptr->GetEditTarget();
    return UsdEditTarget::FromImpl(new EditTargetHandle(target));
}

void UsdStage::SetEditTarget(const UsdEditTarget& editTarget) {
    if (!IsValid()) return;
    asStageHandle(impl_)->ptr->SetEditTarget(asEditTarget(editTarget.GetImpl())->target);
}

sdf::SdfLayer UsdStage::GetSessionLayer() const {
    if (!IsValid()) return sdf::SdfLayer();

    struct SdfLayerHandle {
        pxr::SdfLayerRefPtr ptr;
        SdfLayerHandle() = default;
        explicit SdfLayerHandle(const pxr::SdfLayerRefPtr& p) : ptr(p) {}
    };

    auto* handle = new SdfLayerHandle(asStageHandle(impl_)->ptr->GetSessionLayer());
    return sdf::SdfLayer::FromImpl(handle);
}

UsdEditTarget UsdStage::GetEditTargetForLocalLayer(const sdf::SdfLayer& layer) const {
    if (!IsValid()) return UsdEditTarget();

    struct SdfLayerHandle {
        pxr::SdfLayerRefPtr ptr;
    };
    auto* layerHandle = static_cast<SdfLayerHandle*>(layer.GetImpl());
    if (!layerHandle || !layerHandle->ptr) return UsdEditTarget();

    pxr::UsdEditTarget target = asStageHandle(impl_)->ptr->GetEditTargetForLocalLayer(layerHandle->ptr);
    return UsdEditTarget::FromImpl(new EditTargetHandle(target));
}

// ============================================================================
// UsdSchemaBase implementation
// ============================================================================

struct SchemaBaseHandle {
    pxr::UsdPrim prim;
    SchemaBaseHandle() = default;
    explicit SchemaBaseHandle(const pxr::UsdPrim& p) : prim(p) {}
};

static inline SchemaBaseHandle* asSchemaBase(void* impl) {
    return static_cast<SchemaBaseHandle*>(impl);
}

UsdSchemaBase::UsdSchemaBase(const UsdPrim& prim) : impl_(nullptr) {
    if (prim.IsValid()) {
        impl_ = new SchemaBaseHandle(*static_cast<pxr::UsdPrim*>(prim.GetImpl()));
    } else {
        impl_ = new SchemaBaseHandle();
    }
}

UsdSchemaBase::UsdSchemaBase(const UsdSchemaBase& otherSchema)
    : impl_(new SchemaBaseHandle(asSchemaBase(otherSchema.impl_)->prim)) {}

UsdSchemaBase::~UsdSchemaBase() {
    delete asSchemaBase(impl_);
}

UsdPrim UsdSchemaBase::GetPrim() const {
    if (!impl_) return UsdPrim();
    return UsdPrim::FromImpl(new pxr::UsdPrim(asSchemaBase(impl_)->prim));
}

sdf::SdfPath UsdSchemaBase::GetPath() const {
    if (!impl_) return sdf::SdfPath();
    return sdf::SdfPath::FromImpl(new pxr::SdfPath(asSchemaBase(impl_)->prim.GetPath()));
}

bool UsdSchemaBase::IsConcrete() const {
    return GetSchemaKind() == UsdSchemaKind::ConcreteTyped;
}

bool UsdSchemaBase::IsTyped() const {
    UsdSchemaKind kind = GetSchemaKind();
    return kind == UsdSchemaKind::ConcreteTyped || kind == UsdSchemaKind::AbstractTyped;
}

bool UsdSchemaBase::IsAPISchema() const {
    UsdSchemaKind kind = GetSchemaKind();
    return kind == UsdSchemaKind::NonAppliedAPI ||
           kind == UsdSchemaKind::SingleApplyAPI ||
           kind == UsdSchemaKind::MultipleApplyAPI;
}

bool UsdSchemaBase::IsAppliedAPISchema() const {
    UsdSchemaKind kind = GetSchemaKind();
    return kind == UsdSchemaKind::SingleApplyAPI ||
           kind == UsdSchemaKind::MultipleApplyAPI;
}

bool UsdSchemaBase::IsMultipleApplyAPISchema() const {
    return GetSchemaKind() == UsdSchemaKind::MultipleApplyAPI;
}

UsdSchemaKind UsdSchemaBase::GetSchemaKind() const {
    return _GetSchemaKind();
}

UsdSchemaBase::operator bool() const {
    return impl_ && asSchemaBase(impl_)->prim.IsValid();
}

UsdSchemaKind UsdSchemaBase::_GetSchemaKind() const {
    return UsdSchemaKind::AbstractBase;
}

UsdSchemaBase UsdSchemaBase::FromImpl(void* impl) {
    UsdSchemaBase schema;
    delete asSchemaBase(schema.impl_);
    schema.impl_ = impl;
    return schema;
}

// ============================================================================
// UsdTyped implementation
// ============================================================================

UsdTyped::UsdTyped(const UsdPrim& prim) : UsdSchemaBase(prim) {}

UsdTyped::UsdTyped(const UsdSchemaBase& schemaObj) : UsdSchemaBase(schemaObj) {}

UsdTyped::~UsdTyped() {}

UsdTyped UsdTyped::Get(const UsdStage& stage, const sdf::SdfPath& path) {
    UsdPrim prim = stage.GetPrimAtPath(path);
    return UsdTyped(prim);
}

UsdSchemaKind UsdTyped::_GetSchemaKind() const {
    return UsdSchemaKind::AbstractBase;
}

// ============================================================================
// UsdAPISchemaBase implementation
// ============================================================================

UsdAPISchemaBase::UsdAPISchemaBase(const UsdPrim& prim) : UsdSchemaBase(prim) {}

UsdAPISchemaBase::UsdAPISchemaBase(const UsdSchemaBase& schemaObj) : UsdSchemaBase(schemaObj) {}

UsdAPISchemaBase::UsdAPISchemaBase(const UsdPrim& prim, const tf::TfToken& instanceName)
    : UsdSchemaBase(prim), instanceName_(instanceName) {}

UsdAPISchemaBase::UsdAPISchemaBase(const UsdSchemaBase& schemaObj, const tf::TfToken& instanceName)
    : UsdSchemaBase(schemaObj), instanceName_(instanceName) {}

UsdAPISchemaBase::~UsdAPISchemaBase() {}

tf::TfToken UsdAPISchemaBase::GetInstanceName() const {
    return instanceName_;
}

UsdSchemaKind UsdAPISchemaBase::_GetSchemaKind() const {
    return UsdSchemaKind::AbstractBase;
}

} // namespace usd
} // namespace cxxfacade
