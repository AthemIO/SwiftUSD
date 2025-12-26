// Stage.cpp - UsdStage and UsdPrim implementation
// This file includes real USD headers - NOT visible to Swift

#include "../include/Stage.hpp"
#include "../include/Prim.hpp"

#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usd/primRange.h>
#include <pxr/usd/usd/attribute.h>
#include <pxr/usd/sdf/path.h>
#include <pxr/base/tf/token.h>
#include <pxr/base/gf/vec3f.h>
#include <pxr/base/gf/vec3d.h>
#include <pxr/base/gf/matrix4d.h>
#include <pxr/base/vt/array.h>
#include <pxr/usd/usdGeom/xformable.h>
#include <pxr/usd/usdGeom/boundable.h>
#include <pxr/usd/usdGeom/bboxCache.h>

PXR_NAMESPACE_USING_DIRECTIVE

namespace pixarusd {

// ============================================================================
// MARK: - Implementation Structs
// ============================================================================

namespace detail {

struct StageImpl {
    UsdStageRefPtr usdStage;

    explicit StageImpl(UsdStageRefPtr stage) : usdStage(std::move(stage)) {}
};

struct PrimImpl {
    UsdPrim usdPrim;
    Stage* owningStage;  // Prevent stage from being released while prim exists

    PrimImpl(UsdPrim prim, Stage* stage)
        : usdPrim(std::move(prim))
        , owningStage(stage)
    {
        if (owningStage) {
            retainStage(owningStage);
        }
    }

    ~PrimImpl() {
        if (owningStage) {
            releaseStage(owningStage);
        }
    }
};

} // namespace detail

// ============================================================================
// MARK: - Stage Implementation
// ============================================================================

Stage::Stage(detail::StageImpl* impl) : impl_(impl) {}

Stage::~Stage() {
    delete impl_;
}

Stage* Stage::Open(const std::string& filePath) {
    UsdStageRefPtr usdStage = UsdStage::Open(filePath);
    if (!usdStage) {
        return nullptr;
    }
    return new Stage(new detail::StageImpl(usdStage));
}

Stage* Stage::CreateNew(const std::string& filePath) {
    UsdStageRefPtr usdStage = UsdStage::CreateNew(filePath);
    if (!usdStage) {
        return nullptr;
    }
    return new Stage(new detail::StageImpl(usdStage));
}

Stage* Stage::CreateInMemory() {
    UsdStageRefPtr usdStage = UsdStage::CreateInMemory();
    return new Stage(new detail::StageImpl(usdStage));
}

bool Stage::Save() const {
    impl_->usdStage->Save();
    return true;
}

bool Stage::Export(const std::string& filePath) const {
    return impl_->usdStage->Export(filePath);
}

std::string Stage::ExportToString() const {
    std::string result;
    impl_->usdStage->ExportToString(&result);
    return result;
}

Prim* Stage::GetPseudoRoot() const {
    UsdPrim root = impl_->usdStage->GetPseudoRoot();
    return new Prim(new detail::PrimImpl(root, const_cast<Stage*>(this)));
}

Prim* Stage::GetPrimAtPath(const std::string& path) const {
    SdfPath sdfPath(path);
    UsdPrim prim = impl_->usdStage->GetPrimAtPath(sdfPath);
    if (!prim.IsValid()) {
        return nullptr;
    }
    return new Prim(new detail::PrimImpl(prim, const_cast<Stage*>(this)));
}

Prim* Stage::DefinePrim(const std::string& path, const std::string& typeName) {
    SdfPath sdfPath(path);
    TfToken typeToken(typeName);
    UsdPrim prim = impl_->usdStage->DefinePrim(sdfPath, typeToken);
    if (!prim.IsValid()) {
        return nullptr;
    }
    return new Prim(new detail::PrimImpl(prim, this));
}

std::vector<Prim*> Stage::Traverse() const {
    std::vector<Prim*> result;
    for (const UsdPrim& prim : impl_->usdStage->Traverse()) {
        result.push_back(new Prim(new detail::PrimImpl(prim, const_cast<Stage*>(this))));
    }
    return result;
}

double Stage::GetStartTimeCode() const {
    return impl_->usdStage->GetStartTimeCode();
}

double Stage::GetEndTimeCode() const {
    return impl_->usdStage->GetEndTimeCode();
}

void Stage::SetStartTimeCode(double time) {
    impl_->usdStage->SetStartTimeCode(time);
}

void Stage::SetEndTimeCode(double time) {
    impl_->usdStage->SetEndTimeCode(time);
}

double Stage::GetFramesPerSecond() const {
    return impl_->usdStage->GetFramesPerSecond();
}

void Stage::SetFramesPerSecond(double fps) {
    impl_->usdStage->SetFramesPerSecond(fps);
}

void* Stage::GetRawStagePtr() const {
    return impl_->usdStage.operator->();
}

// ============================================================================
// MARK: - Prim Implementation
// ============================================================================

Prim::Prim(detail::PrimImpl* impl) : impl_(impl) {}

Prim::~Prim() {
    delete impl_;
}

bool Prim::IsValid() const {
    return impl_ && impl_->usdPrim.IsValid();
}

std::string Prim::GetName() const {
    return impl_->usdPrim.GetName().GetString();
}

std::string Prim::GetPath() const {
    return impl_->usdPrim.GetPath().GetAsString();
}

std::string Prim::GetTypeName() const {
    return impl_->usdPrim.GetTypeName().GetString();
}

bool Prim::IsA(const std::string& schemaType) const {
    // Check type name directly
    return impl_->usdPrim.GetTypeName() == TfToken(schemaType);
}

bool Prim::HasAPI(const std::string& apiSchemaType) const {
    return impl_->usdPrim.HasAPI(TfToken(apiSchemaType));
}

Prim* Prim::GetParent() const {
    UsdPrim parent = impl_->usdPrim.GetParent();
    if (!parent.IsValid()) {
        return nullptr;
    }
    return new Prim(new detail::PrimImpl(parent, impl_->owningStage));
}

std::vector<Prim*> Prim::GetChildren() const {
    std::vector<Prim*> result;
    for (const UsdPrim& child : impl_->usdPrim.GetChildren()) {
        result.push_back(new Prim(new detail::PrimImpl(child, impl_->owningStage)));
    }
    return result;
}

Prim* Prim::GetChild(const std::string& name) const {
    UsdPrim child = impl_->usdPrim.GetChild(TfToken(name));
    if (!child.IsValid()) {
        return nullptr;
    }
    return new Prim(new detail::PrimImpl(child, impl_->owningStage));
}

bool Prim::HasAttribute(const std::string& name) const {
    return impl_->usdPrim.HasAttribute(TfToken(name));
}

std::vector<std::string> Prim::GetAttributeNames() const {
    std::vector<std::string> result;
    for (const UsdAttribute& attr : impl_->usdPrim.GetAttributes()) {
        result.push_back(attr.GetName().GetString());
    }
    return result;
}

// Typed getters
bool Prim::GetBool(const std::string& name, TimeCode time) const {
    UsdAttribute attr = impl_->usdPrim.GetAttribute(TfToken(name));
    if (!attr) return false;
    bool value = false;
    attr.Get(&value, time.value);
    return value;
}

int32_t Prim::GetInt(const std::string& name, TimeCode time) const {
    UsdAttribute attr = impl_->usdPrim.GetAttribute(TfToken(name));
    if (!attr) return 0;
    int value = 0;
    attr.Get(&value, time.value);
    return value;
}

float Prim::GetFloat(const std::string& name, TimeCode time) const {
    UsdAttribute attr = impl_->usdPrim.GetAttribute(TfToken(name));
    if (!attr) return 0.0f;
    float value = 0.0f;
    attr.Get(&value, time.value);
    return value;
}

double Prim::GetDouble(const std::string& name, TimeCode time) const {
    UsdAttribute attr = impl_->usdPrim.GetAttribute(TfToken(name));
    if (!attr) return 0.0;
    double value = 0.0;
    attr.Get(&value, time.value);
    return value;
}

std::string Prim::GetString(const std::string& name, TimeCode time) const {
    UsdAttribute attr = impl_->usdPrim.GetAttribute(TfToken(name));
    if (!attr) return "";
    std::string value;
    attr.Get(&value, time.value);
    return value;
}

Vec3f Prim::GetVec3f(const std::string& name, TimeCode time) const {
    UsdAttribute attr = impl_->usdPrim.GetAttribute(TfToken(name));
    if (!attr) return Vec3f{};
    GfVec3f value;
    if (!attr.Get(&value, time.value)) return Vec3f{};
    return Vec3f{value[0], value[1], value[2]};
}

Vec3d Prim::GetVec3d(const std::string& name, TimeCode time) const {
    UsdAttribute attr = impl_->usdPrim.GetAttribute(TfToken(name));
    if (!attr) return Vec3d{};
    GfVec3d value;
    if (!attr.Get(&value, time.value)) return Vec3d{};
    return Vec3d{value[0], value[1], value[2]};
}

Matrix4d Prim::GetMatrix4d(const std::string& name, TimeCode time) const {
    UsdAttribute attr = impl_->usdPrim.GetAttribute(TfToken(name));
    if (!attr) return Matrix4d{};
    GfMatrix4d value;
    if (!attr.Get(&value, time.value)) return Matrix4d{};

    Matrix4d result;
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            result.m[row][col] = value[row][col];
        }
    }
    return result;
}

// Array getters
std::vector<Vec3f> Prim::GetVec3fArray(const std::string& name, TimeCode time) const {
    UsdAttribute attr = impl_->usdPrim.GetAttribute(TfToken(name));
    if (!attr) return {};

    VtArray<GfVec3f> vtArray;
    if (!attr.Get(&vtArray, time.value)) return {};

    std::vector<Vec3f> result;
    result.reserve(vtArray.size());
    for (const auto& v : vtArray) {
        result.push_back(Vec3f{v[0], v[1], v[2]});
    }
    return result;
}

std::vector<Vec3d> Prim::GetVec3dArray(const std::string& name, TimeCode time) const {
    UsdAttribute attr = impl_->usdPrim.GetAttribute(TfToken(name));
    if (!attr) return {};

    VtArray<GfVec3d> vtArray;
    if (!attr.Get(&vtArray, time.value)) return {};

    std::vector<Vec3d> result;
    result.reserve(vtArray.size());
    for (const auto& v : vtArray) {
        result.push_back(Vec3d{v[0], v[1], v[2]});
    }
    return result;
}

std::vector<int32_t> Prim::GetIntArray(const std::string& name, TimeCode time) const {
    UsdAttribute attr = impl_->usdPrim.GetAttribute(TfToken(name));
    if (!attr) return {};

    VtArray<int> vtArray;
    if (!attr.Get(&vtArray, time.value)) return {};

    return std::vector<int32_t>(vtArray.begin(), vtArray.end());
}

std::vector<float> Prim::GetFloatArray(const std::string& name, TimeCode time) const {
    UsdAttribute attr = impl_->usdPrim.GetAttribute(TfToken(name));
    if (!attr) return {};

    VtArray<float> vtArray;
    if (!attr.Get(&vtArray, time.value)) return {};

    return std::vector<float>(vtArray.begin(), vtArray.end());
}

// Typed setters
bool Prim::SetBool(const std::string& name, bool value, TimeCode time) {
    UsdAttribute attr = impl_->usdPrim.GetAttribute(TfToken(name));
    if (!attr) return false;
    return attr.Set(value, time.value);
}

bool Prim::SetInt(const std::string& name, int32_t value, TimeCode time) {
    UsdAttribute attr = impl_->usdPrim.GetAttribute(TfToken(name));
    if (!attr) return false;
    return attr.Set(static_cast<int>(value), time.value);
}

bool Prim::SetFloat(const std::string& name, float value, TimeCode time) {
    UsdAttribute attr = impl_->usdPrim.GetAttribute(TfToken(name));
    if (!attr) return false;
    return attr.Set(value, time.value);
}

bool Prim::SetDouble(const std::string& name, double value, TimeCode time) {
    UsdAttribute attr = impl_->usdPrim.GetAttribute(TfToken(name));
    if (!attr) return false;
    return attr.Set(value, time.value);
}

bool Prim::SetString(const std::string& name, const std::string& value, TimeCode time) {
    UsdAttribute attr = impl_->usdPrim.GetAttribute(TfToken(name));
    if (!attr) return false;
    return attr.Set(value, time.value);
}

bool Prim::SetVec3f(const std::string& name, Vec3f value, TimeCode time) {
    UsdAttribute attr = impl_->usdPrim.GetAttribute(TfToken(name));
    if (!attr) return false;
    return attr.Set(GfVec3f(value.x, value.y, value.z), time.value);
}

bool Prim::SetVec3d(const std::string& name, Vec3d value, TimeCode time) {
    UsdAttribute attr = impl_->usdPrim.GetAttribute(TfToken(name));
    if (!attr) return false;
    return attr.Set(GfVec3d(value.x, value.y, value.z), time.value);
}

// Array setters
bool Prim::SetVec3fArray(const std::string& name, const std::vector<Vec3f>& values, TimeCode time) {
    UsdAttribute attr = impl_->usdPrim.GetAttribute(TfToken(name));
    if (!attr) return false;

    VtArray<GfVec3f> vtArray;
    vtArray.reserve(values.size());
    for (const auto& v : values) {
        vtArray.push_back(GfVec3f(v.x, v.y, v.z));
    }
    return attr.Set(vtArray, time.value);
}

bool Prim::SetIntArray(const std::string& name, const std::vector<int32_t>& values, TimeCode time) {
    UsdAttribute attr = impl_->usdPrim.GetAttribute(TfToken(name));
    if (!attr) return false;

    VtArray<int> vtArray(values.begin(), values.end());
    return attr.Set(vtArray, time.value);
}

bool Prim::SetFloatArray(const std::string& name, const std::vector<float>& values, TimeCode time) {
    UsdAttribute attr = impl_->usdPrim.GetAttribute(TfToken(name));
    if (!attr) return false;

    VtArray<float> vtArray(values.begin(), values.end());
    return attr.Set(vtArray, time.value);
}

// Transform methods
Matrix4d Prim::GetLocalTransform(TimeCode time) const {
    UsdGeomXformable xformable(impl_->usdPrim);
    if (!xformable) return Matrix4d{};

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

Matrix4d Prim::GetWorldTransform(TimeCode time) const {
    UsdGeomXformable xformable(impl_->usdPrim);
    if (!xformable) return Matrix4d{};

    GfMatrix4d xform = xformable.ComputeLocalToWorldTransform(time.value);

    Matrix4d result;
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            result.m[row][col] = xform[row][col];
        }
    }
    return result;
}

BBox3d Prim::ComputeWorldBound(TimeCode time) const {
    // Use default purposes for bounding box calculation
    TfTokenVector purposes = {UsdGeomTokens->default_, UsdGeomTokens->render};

    UsdGeomBBoxCache bboxCache(time.value, purposes);
    GfBBox3d bbox = bboxCache.ComputeWorldBound(impl_->usdPrim);
    GfRange3d range = bbox.ComputeAlignedBox();

    return BBox3d{
        Vec3d{range.GetMin()[0], range.GetMin()[1], range.GetMin()[2]},
        Vec3d{range.GetMax()[0], range.GetMax()[1], range.GetMax()[2]}
    };
}

void* Prim::GetRawPrimPtr() const {
    return const_cast<UsdPrim*>(&impl_->usdPrim);
}

} // namespace pixarusd

// ============================================================================
// MARK: - Global Retain/Release Functions (for Swift interop)
// ============================================================================

void retainStage(pixarusd::Stage* s) {
    if (s) {
        s->refCount_.fetch_add(1, std::memory_order_relaxed);
    }
}

void releaseStage(pixarusd::Stage* s) {
    if (s && s->refCount_.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete s;
    }
}

void retainPrim(pixarusd::Prim* p) {
    if (p) {
        p->refCount_.fetch_add(1, std::memory_order_relaxed);
    }
}

void releasePrim(pixarusd::Prim* p) {
    if (p && p->refCount_.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete p;
    }
}
