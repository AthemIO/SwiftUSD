// UsdLux.cpp - Lighting Schemas implementation
// This file contains the real USD calls for the UsdLux facade

#include "../../include/usdLux/UsdLux.hpp"

// Pixar USD includes - ONLY in .cpp files!
#include <pxr/usd/usdLux/lightAPI.h>
#include <pxr/usd/usdLux/meshLightAPI.h>
#include <pxr/usd/usdLux/volumeLightAPI.h>
#include <pxr/usd/usdLux/listAPI.h>
#include <pxr/usd/usdLux/shadowAPI.h>
#include <pxr/usd/usdLux/shapingAPI.h>
#include <pxr/usd/usdLux/lightFilter.h>
#include <pxr/usd/usdLux/distantLight.h>
#include <pxr/usd/usdLux/domeLight.h>
#include <pxr/usd/usdLux/rectLight.h>
#include <pxr/usd/usdLux/sphereLight.h>
#include <pxr/usd/usdLux/cylinderLight.h>
#include <pxr/usd/usdLux/diskLight.h>
#include <pxr/usd/usdLux/tokens.h>
#include <pxr/usd/usdLux/blackbody.h>
#include <pxr/usd/usdShade/connectableAPI.h>

PXR_NAMESPACE_USING_DIRECTIVE

namespace cxxfacade {
namespace usdLux {

// ============================================================================
// Helper functions for conversion
// ============================================================================

namespace {

inline UsdPrim GetUsdPrim(const usd::UsdPrim& prim) {
    return *reinterpret_cast<const UsdPrim*>(prim.GetImpl());
}

inline UsdStagePtr GetUsdStagePtr(const usd::UsdStage& stage) {
    return *reinterpret_cast<const UsdStagePtr*>(stage.GetImpl());
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

} // anonymous namespace

// ============================================================================
// UsdLuxLightAPI Implementation
// ============================================================================

UsdLuxLightAPI::UsdLuxLightAPI(const usd::UsdPrim& prim)
    : usd::UsdAPISchemaBase(prim) {}

UsdLuxLightAPI::UsdLuxLightAPI(const usd::UsdSchemaBase& schemaObj)
    : usd::UsdAPISchemaBase(schemaObj) {}

UsdLuxLightAPI::~UsdLuxLightAPI() = default;

UsdLuxLightAPI UsdLuxLightAPI::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    UsdStagePtr stagePtr = GetUsdStagePtr(stage);
    if (!stagePtr) return UsdLuxLightAPI();
    UsdPrim prim = stagePtr->GetPrimAtPath(GetSdfPath(path));
    return UsdLuxLightAPI(WrapPrim(prim));
}

UsdLuxLightAPI UsdLuxLightAPI::Apply(const usd::UsdPrim& prim) {
    UsdPrim usdPrim = GetUsdPrim(prim);
    auto applied = PXR_NS::UsdLuxLightAPI::Apply(usdPrim);
    if (!applied) return UsdLuxLightAPI();
    return UsdLuxLightAPI(WrapPrim(applied.GetPrim()));
}

bool UsdLuxLightAPI::CanApply(const usd::UsdPrim& prim, std::string* whyNot) {
    UsdPrim usdPrim = GetUsdPrim(prim);
    return PXR_NS::UsdLuxLightAPI::CanApply(usdPrim, whyNot);
}

usd::UsdAttribute UsdLuxLightAPI::GetShaderIdAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxLightAPI api(usdPrim);
    return WrapAttribute(api.GetShaderIdAttr());
}

usd::UsdAttribute UsdLuxLightAPI::CreateShaderIdAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxLightAPI api(usdPrim);
    return WrapAttribute(api.CreateShaderIdAttr());
}

usd::UsdAttribute UsdLuxLightAPI::GetIntensityAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxLightAPI api(usdPrim);
    return WrapAttribute(api.GetIntensityAttr());
}

usd::UsdAttribute UsdLuxLightAPI::CreateIntensityAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxLightAPI api(usdPrim);
    return WrapAttribute(api.CreateIntensityAttr());
}

usd::UsdAttribute UsdLuxLightAPI::GetExposureAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxLightAPI api(usdPrim);
    return WrapAttribute(api.GetExposureAttr());
}

usd::UsdAttribute UsdLuxLightAPI::CreateExposureAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxLightAPI api(usdPrim);
    return WrapAttribute(api.CreateExposureAttr());
}

usd::UsdAttribute UsdLuxLightAPI::GetDiffuseAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxLightAPI api(usdPrim);
    return WrapAttribute(api.GetDiffuseAttr());
}

usd::UsdAttribute UsdLuxLightAPI::CreateDiffuseAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxLightAPI api(usdPrim);
    return WrapAttribute(api.CreateDiffuseAttr());
}

usd::UsdAttribute UsdLuxLightAPI::GetSpecularAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxLightAPI api(usdPrim);
    return WrapAttribute(api.GetSpecularAttr());
}

usd::UsdAttribute UsdLuxLightAPI::CreateSpecularAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxLightAPI api(usdPrim);
    return WrapAttribute(api.CreateSpecularAttr());
}

usd::UsdAttribute UsdLuxLightAPI::GetNormalizeAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxLightAPI api(usdPrim);
    return WrapAttribute(api.GetNormalizeAttr());
}

usd::UsdAttribute UsdLuxLightAPI::CreateNormalizeAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxLightAPI api(usdPrim);
    return WrapAttribute(api.CreateNormalizeAttr());
}

usd::UsdAttribute UsdLuxLightAPI::GetColorAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxLightAPI api(usdPrim);
    return WrapAttribute(api.GetColorAttr());
}

usd::UsdAttribute UsdLuxLightAPI::CreateColorAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxLightAPI api(usdPrim);
    return WrapAttribute(api.CreateColorAttr());
}

usd::UsdAttribute UsdLuxLightAPI::GetEnableColorTemperatureAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxLightAPI api(usdPrim);
    return WrapAttribute(api.GetEnableColorTemperatureAttr());
}

usd::UsdAttribute UsdLuxLightAPI::CreateEnableColorTemperatureAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxLightAPI api(usdPrim);
    return WrapAttribute(api.CreateEnableColorTemperatureAttr());
}

usd::UsdAttribute UsdLuxLightAPI::GetColorTemperatureAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxLightAPI api(usdPrim);
    return WrapAttribute(api.GetColorTemperatureAttr());
}

usd::UsdAttribute UsdLuxLightAPI::CreateColorTemperatureAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxLightAPI api(usdPrim);
    return WrapAttribute(api.CreateColorTemperatureAttr());
}

usd::UsdAttribute UsdLuxLightAPI::GetMaterialSyncModeAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxLightAPI api(usdPrim);
    return WrapAttribute(api.GetMaterialSyncModeAttr());
}

usd::UsdAttribute UsdLuxLightAPI::CreateMaterialSyncModeAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxLightAPI api(usdPrim);
    return WrapAttribute(api.CreateMaterialSyncModeAttr());
}

usd::UsdRelationship UsdLuxLightAPI::GetFiltersRel() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxLightAPI api(usdPrim);
    return WrapRelationship(api.GetFiltersRel());
}

usd::UsdRelationship UsdLuxLightAPI::CreateFiltersRel() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxLightAPI api(usdPrim);
    return WrapRelationship(api.CreateFiltersRel());
}

usdShade::UsdShadeConnectableAPI UsdLuxLightAPI::ConnectableAPI() const {
    return usdShade::UsdShadeConnectableAPI(GetPrim());
}

usdShade::UsdShadeInput UsdLuxLightAPI::CreateInput(const tf::TfToken& name, const std::string& typeName) {
    return ConnectableAPI().CreateInput(name, typeName);
}

usdShade::UsdShadeInput UsdLuxLightAPI::GetInput(const tf::TfToken& name) const {
    return ConnectableAPI().GetInput(name);
}

std::vector<usdShade::UsdShadeInput> UsdLuxLightAPI::GetInputs(bool onlyAuthored) const {
    return ConnectableAPI().GetInputs(onlyAuthored);
}

usdShade::UsdShadeOutput UsdLuxLightAPI::CreateOutput(const tf::TfToken& name, const std::string& typeName) {
    return ConnectableAPI().CreateOutput(name, typeName);
}

usdShade::UsdShadeOutput UsdLuxLightAPI::GetOutput(const tf::TfToken& name) const {
    return ConnectableAPI().GetOutput(name);
}

std::vector<usdShade::UsdShadeOutput> UsdLuxLightAPI::GetOutputs(bool onlyAuthored) const {
    return ConnectableAPI().GetOutputs(onlyAuthored);
}

usd::UsdSchemaKind UsdLuxLightAPI::_GetSchemaKind() const {
    return usd::UsdSchemaKind::SingleApplyAPI;
}

// ============================================================================
// UsdLuxMeshLightAPI Implementation
// ============================================================================

UsdLuxMeshLightAPI::UsdLuxMeshLightAPI(const usd::UsdPrim& prim)
    : usd::UsdAPISchemaBase(prim) {}

UsdLuxMeshLightAPI::UsdLuxMeshLightAPI(const usd::UsdSchemaBase& schemaObj)
    : usd::UsdAPISchemaBase(schemaObj) {}

UsdLuxMeshLightAPI::~UsdLuxMeshLightAPI() = default;

UsdLuxMeshLightAPI UsdLuxMeshLightAPI::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    UsdStagePtr stagePtr = GetUsdStagePtr(stage);
    if (!stagePtr) return UsdLuxMeshLightAPI();
    UsdPrim prim = stagePtr->GetPrimAtPath(GetSdfPath(path));
    return UsdLuxMeshLightAPI(WrapPrim(prim));
}

UsdLuxMeshLightAPI UsdLuxMeshLightAPI::Apply(const usd::UsdPrim& prim) {
    UsdPrim usdPrim = GetUsdPrim(prim);
    auto applied = PXR_NS::UsdLuxMeshLightAPI::Apply(usdPrim);
    if (!applied) return UsdLuxMeshLightAPI();
    return UsdLuxMeshLightAPI(WrapPrim(applied.GetPrim()));
}

bool UsdLuxMeshLightAPI::CanApply(const usd::UsdPrim& prim, std::string* whyNot) {
    UsdPrim usdPrim = GetUsdPrim(prim);
    return PXR_NS::UsdLuxMeshLightAPI::CanApply(usdPrim, whyNot);
}

usd::UsdSchemaKind UsdLuxMeshLightAPI::_GetSchemaKind() const {
    return usd::UsdSchemaKind::SingleApplyAPI;
}

// ============================================================================
// UsdLuxVolumeLightAPI Implementation
// ============================================================================

UsdLuxVolumeLightAPI::UsdLuxVolumeLightAPI(const usd::UsdPrim& prim)
    : usd::UsdAPISchemaBase(prim) {}

UsdLuxVolumeLightAPI::UsdLuxVolumeLightAPI(const usd::UsdSchemaBase& schemaObj)
    : usd::UsdAPISchemaBase(schemaObj) {}

UsdLuxVolumeLightAPI::~UsdLuxVolumeLightAPI() = default;

UsdLuxVolumeLightAPI UsdLuxVolumeLightAPI::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    UsdStagePtr stagePtr = GetUsdStagePtr(stage);
    if (!stagePtr) return UsdLuxVolumeLightAPI();
    UsdPrim prim = stagePtr->GetPrimAtPath(GetSdfPath(path));
    return UsdLuxVolumeLightAPI(WrapPrim(prim));
}

UsdLuxVolumeLightAPI UsdLuxVolumeLightAPI::Apply(const usd::UsdPrim& prim) {
    UsdPrim usdPrim = GetUsdPrim(prim);
    auto applied = PXR_NS::UsdLuxVolumeLightAPI::Apply(usdPrim);
    if (!applied) return UsdLuxVolumeLightAPI();
    return UsdLuxVolumeLightAPI(WrapPrim(applied.GetPrim()));
}

bool UsdLuxVolumeLightAPI::CanApply(const usd::UsdPrim& prim, std::string* whyNot) {
    UsdPrim usdPrim = GetUsdPrim(prim);
    return PXR_NS::UsdLuxVolumeLightAPI::CanApply(usdPrim, whyNot);
}

usd::UsdSchemaKind UsdLuxVolumeLightAPI::_GetSchemaKind() const {
    return usd::UsdSchemaKind::SingleApplyAPI;
}

// ============================================================================
// UsdLuxListAPI Implementation
// ============================================================================

UsdLuxListAPI::UsdLuxListAPI(const usd::UsdPrim& prim)
    : usd::UsdAPISchemaBase(prim) {}

UsdLuxListAPI::UsdLuxListAPI(const usd::UsdSchemaBase& schemaObj)
    : usd::UsdAPISchemaBase(schemaObj) {}

UsdLuxListAPI::~UsdLuxListAPI() = default;

UsdLuxListAPI UsdLuxListAPI::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    UsdStagePtr stagePtr = GetUsdStagePtr(stage);
    if (!stagePtr) return UsdLuxListAPI();
    UsdPrim prim = stagePtr->GetPrimAtPath(GetSdfPath(path));
    return UsdLuxListAPI(WrapPrim(prim));
}

UsdLuxListAPI UsdLuxListAPI::Apply(const usd::UsdPrim& prim) {
    UsdPrim usdPrim = GetUsdPrim(prim);
    auto applied = PXR_NS::UsdLuxListAPI::Apply(usdPrim);
    if (!applied) return UsdLuxListAPI();
    return UsdLuxListAPI(WrapPrim(applied.GetPrim()));
}

bool UsdLuxListAPI::CanApply(const usd::UsdPrim& prim, std::string* whyNot) {
    UsdPrim usdPrim = GetUsdPrim(prim);
    return PXR_NS::UsdLuxListAPI::CanApply(usdPrim, whyNot);
}

usd::UsdRelationship UsdLuxListAPI::GetLightListRel() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxListAPI api(usdPrim);
    return WrapRelationship(api.GetLightListRel());
}

usd::UsdRelationship UsdLuxListAPI::CreateLightListRel() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxListAPI api(usdPrim);
    return WrapRelationship(api.CreateLightListRel());
}

usd::UsdAttribute UsdLuxListAPI::GetLightListCacheBehaviorAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxListAPI api(usdPrim);
    return WrapAttribute(api.GetLightListCacheBehaviorAttr());
}

usd::UsdAttribute UsdLuxListAPI::CreateLightListCacheBehaviorAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxListAPI api(usdPrim);
    return WrapAttribute(api.CreateLightListCacheBehaviorAttr());
}

std::vector<sdf::SdfPath> UsdLuxListAPI::ComputeLightList(UsdLuxListAPIComputeMode mode) const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxListAPI api(usdPrim);

    PXR_NS::UsdLuxListAPI::ComputeMode usdMode =
        mode == UsdLuxListAPIComputeMode::ComputeModeConsultModelHierarchyCache
            ? PXR_NS::UsdLuxListAPI::ComputeModeConsultModelHierarchyCache
            : PXR_NS::UsdLuxListAPI::ComputeModeIgnoreCache;

    SdfPathSet pathSet = api.ComputeLightList(usdMode);
    std::vector<sdf::SdfPath> result;
    result.reserve(pathSet.size());
    for (const auto& path : pathSet) {
        result.push_back(sdf::SdfPath::FromImpl(new SdfPath(path)));
    }
    return result;
}

void UsdLuxListAPI::StoreLightList(const std::vector<sdf::SdfPath>& paths) const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxListAPI api(usdPrim);

    SdfPathSet pathSet;
    for (const auto& path : paths) {
        pathSet.insert(GetSdfPath(path));
    }
    api.StoreLightList(pathSet);
}

void UsdLuxListAPI::InvalidateLightListCache() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxListAPI api(usdPrim);
    api.InvalidateLightList();
}

usd::UsdSchemaKind UsdLuxListAPI::_GetSchemaKind() const {
    return usd::UsdSchemaKind::SingleApplyAPI;
}

// ============================================================================
// UsdLuxShadowAPI Implementation
// ============================================================================

UsdLuxShadowAPI::UsdLuxShadowAPI(const usd::UsdPrim& prim)
    : usd::UsdAPISchemaBase(prim) {}

UsdLuxShadowAPI::UsdLuxShadowAPI(const usd::UsdSchemaBase& schemaObj)
    : usd::UsdAPISchemaBase(schemaObj) {}

UsdLuxShadowAPI::~UsdLuxShadowAPI() = default;

UsdLuxShadowAPI UsdLuxShadowAPI::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    UsdStagePtr stagePtr = GetUsdStagePtr(stage);
    if (!stagePtr) return UsdLuxShadowAPI();
    UsdPrim prim = stagePtr->GetPrimAtPath(GetSdfPath(path));
    return UsdLuxShadowAPI(WrapPrim(prim));
}

UsdLuxShadowAPI UsdLuxShadowAPI::Apply(const usd::UsdPrim& prim) {
    UsdPrim usdPrim = GetUsdPrim(prim);
    auto applied = PXR_NS::UsdLuxShadowAPI::Apply(usdPrim);
    if (!applied) return UsdLuxShadowAPI();
    return UsdLuxShadowAPI(WrapPrim(applied.GetPrim()));
}

bool UsdLuxShadowAPI::CanApply(const usd::UsdPrim& prim, std::string* whyNot) {
    UsdPrim usdPrim = GetUsdPrim(prim);
    return PXR_NS::UsdLuxShadowAPI::CanApply(usdPrim, whyNot);
}

usd::UsdAttribute UsdLuxShadowAPI::GetShadowEnableAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxShadowAPI api(usdPrim);
    return WrapAttribute(api.GetShadowEnableAttr());
}

usd::UsdAttribute UsdLuxShadowAPI::CreateShadowEnableAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxShadowAPI api(usdPrim);
    return WrapAttribute(api.CreateShadowEnableAttr());
}

usd::UsdAttribute UsdLuxShadowAPI::GetShadowColorAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxShadowAPI api(usdPrim);
    return WrapAttribute(api.GetShadowColorAttr());
}

usd::UsdAttribute UsdLuxShadowAPI::CreateShadowColorAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxShadowAPI api(usdPrim);
    return WrapAttribute(api.CreateShadowColorAttr());
}

usd::UsdAttribute UsdLuxShadowAPI::GetShadowDistanceAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxShadowAPI api(usdPrim);
    return WrapAttribute(api.GetShadowDistanceAttr());
}

usd::UsdAttribute UsdLuxShadowAPI::CreateShadowDistanceAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxShadowAPI api(usdPrim);
    return WrapAttribute(api.CreateShadowDistanceAttr());
}

usd::UsdAttribute UsdLuxShadowAPI::GetShadowFalloffAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxShadowAPI api(usdPrim);
    return WrapAttribute(api.GetShadowFalloffAttr());
}

usd::UsdAttribute UsdLuxShadowAPI::CreateShadowFalloffAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxShadowAPI api(usdPrim);
    return WrapAttribute(api.CreateShadowFalloffAttr());
}

usd::UsdAttribute UsdLuxShadowAPI::GetShadowFalloffGammaAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxShadowAPI api(usdPrim);
    return WrapAttribute(api.GetShadowFalloffGammaAttr());
}

usd::UsdAttribute UsdLuxShadowAPI::CreateShadowFalloffGammaAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxShadowAPI api(usdPrim);
    return WrapAttribute(api.CreateShadowFalloffGammaAttr());
}

usdShade::UsdShadeConnectableAPI UsdLuxShadowAPI::ConnectableAPI() const {
    return usdShade::UsdShadeConnectableAPI(GetPrim());
}

usd::UsdSchemaKind UsdLuxShadowAPI::_GetSchemaKind() const {
    return usd::UsdSchemaKind::SingleApplyAPI;
}

// ============================================================================
// UsdLuxShapingAPI Implementation
// ============================================================================

UsdLuxShapingAPI::UsdLuxShapingAPI(const usd::UsdPrim& prim)
    : usd::UsdAPISchemaBase(prim) {}

UsdLuxShapingAPI::UsdLuxShapingAPI(const usd::UsdSchemaBase& schemaObj)
    : usd::UsdAPISchemaBase(schemaObj) {}

UsdLuxShapingAPI::~UsdLuxShapingAPI() = default;

UsdLuxShapingAPI UsdLuxShapingAPI::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    UsdStagePtr stagePtr = GetUsdStagePtr(stage);
    if (!stagePtr) return UsdLuxShapingAPI();
    UsdPrim prim = stagePtr->GetPrimAtPath(GetSdfPath(path));
    return UsdLuxShapingAPI(WrapPrim(prim));
}

UsdLuxShapingAPI UsdLuxShapingAPI::Apply(const usd::UsdPrim& prim) {
    UsdPrim usdPrim = GetUsdPrim(prim);
    auto applied = PXR_NS::UsdLuxShapingAPI::Apply(usdPrim);
    if (!applied) return UsdLuxShapingAPI();
    return UsdLuxShapingAPI(WrapPrim(applied.GetPrim()));
}

bool UsdLuxShapingAPI::CanApply(const usd::UsdPrim& prim, std::string* whyNot) {
    UsdPrim usdPrim = GetUsdPrim(prim);
    return PXR_NS::UsdLuxShapingAPI::CanApply(usdPrim, whyNot);
}

usd::UsdAttribute UsdLuxShapingAPI::GetShapingFocusAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxShapingAPI api(usdPrim);
    return WrapAttribute(api.GetShapingFocusAttr());
}

usd::UsdAttribute UsdLuxShapingAPI::CreateShapingFocusAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxShapingAPI api(usdPrim);
    return WrapAttribute(api.CreateShapingFocusAttr());
}

usd::UsdAttribute UsdLuxShapingAPI::GetShapingFocusTintAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxShapingAPI api(usdPrim);
    return WrapAttribute(api.GetShapingFocusTintAttr());
}

usd::UsdAttribute UsdLuxShapingAPI::CreateShapingFocusTintAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxShapingAPI api(usdPrim);
    return WrapAttribute(api.CreateShapingFocusTintAttr());
}

usd::UsdAttribute UsdLuxShapingAPI::GetShapingConeAngleAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxShapingAPI api(usdPrim);
    return WrapAttribute(api.GetShapingConeAngleAttr());
}

usd::UsdAttribute UsdLuxShapingAPI::CreateShapingConeAngleAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxShapingAPI api(usdPrim);
    return WrapAttribute(api.CreateShapingConeAngleAttr());
}

usd::UsdAttribute UsdLuxShapingAPI::GetShapingConeSoftnessAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxShapingAPI api(usdPrim);
    return WrapAttribute(api.GetShapingConeSoftnessAttr());
}

usd::UsdAttribute UsdLuxShapingAPI::CreateShapingConeSoftnessAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxShapingAPI api(usdPrim);
    return WrapAttribute(api.CreateShapingConeSoftnessAttr());
}

usd::UsdAttribute UsdLuxShapingAPI::GetShapingIesFileAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxShapingAPI api(usdPrim);
    return WrapAttribute(api.GetShapingIesFileAttr());
}

usd::UsdAttribute UsdLuxShapingAPI::CreateShapingIesFileAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxShapingAPI api(usdPrim);
    return WrapAttribute(api.CreateShapingIesFileAttr());
}

usd::UsdAttribute UsdLuxShapingAPI::GetShapingIesAngleScaleAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxShapingAPI api(usdPrim);
    return WrapAttribute(api.GetShapingIesAngleScaleAttr());
}

usd::UsdAttribute UsdLuxShapingAPI::CreateShapingIesAngleScaleAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxShapingAPI api(usdPrim);
    return WrapAttribute(api.CreateShapingIesAngleScaleAttr());
}

usd::UsdAttribute UsdLuxShapingAPI::GetShapingIesNormalizeAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxShapingAPI api(usdPrim);
    return WrapAttribute(api.GetShapingIesNormalizeAttr());
}

usd::UsdAttribute UsdLuxShapingAPI::CreateShapingIesNormalizeAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxShapingAPI api(usdPrim);
    return WrapAttribute(api.CreateShapingIesNormalizeAttr());
}

usdShade::UsdShadeConnectableAPI UsdLuxShapingAPI::ConnectableAPI() const {
    return usdShade::UsdShadeConnectableAPI(GetPrim());
}

usd::UsdSchemaKind UsdLuxShapingAPI::_GetSchemaKind() const {
    return usd::UsdSchemaKind::SingleApplyAPI;
}

// ============================================================================
// UsdLuxLightFilter Implementation
// ============================================================================

UsdLuxLightFilter::UsdLuxLightFilter(const usd::UsdPrim& prim)
    : usd::UsdTyped(prim) {}

UsdLuxLightFilter::UsdLuxLightFilter(const usd::UsdSchemaBase& schemaObj)
    : usd::UsdTyped(schemaObj) {}

UsdLuxLightFilter::~UsdLuxLightFilter() = default;

UsdLuxLightFilter UsdLuxLightFilter::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    UsdStagePtr stagePtr = GetUsdStagePtr(stage);
    if (!stagePtr) return UsdLuxLightFilter();
    UsdPrim prim = stagePtr->GetPrimAtPath(GetSdfPath(path));
    return UsdLuxLightFilter(WrapPrim(prim));
}

UsdLuxLightFilter UsdLuxLightFilter::Define(usd::UsdStage& stage, const sdf::SdfPath& path) {
    usd::UsdPrim prim = stage.DefinePrim(path, tf::TfToken::FromImpl(new pxr::TfToken("LightFilter")));
    return UsdLuxLightFilter(prim);
}

usd::UsdAttribute UsdLuxLightFilter::GetShaderIdAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxLightFilter filter(usdPrim);
    return WrapAttribute(filter.GetShaderIdAttr());
}

usd::UsdAttribute UsdLuxLightFilter::CreateShaderIdAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxLightFilter filter(usdPrim);
    return WrapAttribute(filter.CreateShaderIdAttr());
}

usdShade::UsdShadeConnectableAPI UsdLuxLightFilter::ConnectableAPI() const {
    return usdShade::UsdShadeConnectableAPI(GetPrim());
}

usd::UsdSchemaKind UsdLuxLightFilter::_GetSchemaKind() const {
    return usd::UsdSchemaKind::ConcreteTyped;
}

// ============================================================================
// UsdLuxDistantLight Implementation
// ============================================================================

UsdLuxDistantLight::UsdLuxDistantLight(const usd::UsdPrim& prim)
    : usdGeom::UsdGeomXformable(prim) {}

UsdLuxDistantLight::UsdLuxDistantLight(const usd::UsdSchemaBase& schemaObj)
    : usdGeom::UsdGeomXformable(schemaObj) {}

UsdLuxDistantLight::~UsdLuxDistantLight() = default;

UsdLuxDistantLight UsdLuxDistantLight::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    UsdStagePtr stagePtr = GetUsdStagePtr(stage);
    if (!stagePtr) return UsdLuxDistantLight();
    UsdPrim prim = stagePtr->GetPrimAtPath(GetSdfPath(path));
    return UsdLuxDistantLight(WrapPrim(prim));
}

UsdLuxDistantLight UsdLuxDistantLight::Define(usd::UsdStage& stage, const sdf::SdfPath& path) {
    usd::UsdPrim prim = stage.DefinePrim(path, tf::TfToken::FromImpl(new pxr::TfToken("DistantLight")));
    return UsdLuxDistantLight(prim);
}

UsdLuxLightAPI UsdLuxDistantLight::LightAPI() const {
    return UsdLuxLightAPI(GetPrim());
}

usd::UsdAttribute UsdLuxDistantLight::GetAngleAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxDistantLight light(usdPrim);
    return WrapAttribute(light.GetAngleAttr());
}

usd::UsdAttribute UsdLuxDistantLight::CreateAngleAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxDistantLight light(usdPrim);
    return WrapAttribute(light.CreateAngleAttr());
}

usd::UsdSchemaKind UsdLuxDistantLight::_GetSchemaKind() const {
    return usd::UsdSchemaKind::ConcreteTyped;
}

// ============================================================================
// UsdLuxDomeLight Implementation
// ============================================================================

UsdLuxDomeLight::UsdLuxDomeLight(const usd::UsdPrim& prim)
    : usdGeom::UsdGeomXformable(prim) {}

UsdLuxDomeLight::UsdLuxDomeLight(const usd::UsdSchemaBase& schemaObj)
    : usdGeom::UsdGeomXformable(schemaObj) {}

UsdLuxDomeLight::~UsdLuxDomeLight() = default;

UsdLuxDomeLight UsdLuxDomeLight::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    UsdStagePtr stagePtr = GetUsdStagePtr(stage);
    if (!stagePtr) return UsdLuxDomeLight();
    UsdPrim prim = stagePtr->GetPrimAtPath(GetSdfPath(path));
    return UsdLuxDomeLight(WrapPrim(prim));
}

UsdLuxDomeLight UsdLuxDomeLight::Define(usd::UsdStage& stage, const sdf::SdfPath& path) {
    usd::UsdPrim prim = stage.DefinePrim(path, tf::TfToken::FromImpl(new pxr::TfToken("DomeLight")));
    return UsdLuxDomeLight(prim);
}

UsdLuxLightAPI UsdLuxDomeLight::LightAPI() const {
    return UsdLuxLightAPI(GetPrim());
}

usd::UsdAttribute UsdLuxDomeLight::GetTextureFileAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxDomeLight light(usdPrim);
    return WrapAttribute(light.GetTextureFileAttr());
}

usd::UsdAttribute UsdLuxDomeLight::CreateTextureFileAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxDomeLight light(usdPrim);
    return WrapAttribute(light.CreateTextureFileAttr());
}

usd::UsdAttribute UsdLuxDomeLight::GetTextureFormatAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxDomeLight light(usdPrim);
    return WrapAttribute(light.GetTextureFormatAttr());
}

usd::UsdAttribute UsdLuxDomeLight::CreateTextureFormatAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxDomeLight light(usdPrim);
    return WrapAttribute(light.CreateTextureFormatAttr());
}

usd::UsdAttribute UsdLuxDomeLight::GetGuideRadiusAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxDomeLight light(usdPrim);
    return WrapAttribute(light.GetGuideRadiusAttr());
}

usd::UsdAttribute UsdLuxDomeLight::CreateGuideRadiusAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxDomeLight light(usdPrim);
    return WrapAttribute(light.CreateGuideRadiusAttr());
}

usd::UsdRelationship UsdLuxDomeLight::GetPortalsRel() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxDomeLight light(usdPrim);
    return WrapRelationship(light.GetPortalsRel());
}

usd::UsdRelationship UsdLuxDomeLight::CreatePortalsRel() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxDomeLight light(usdPrim);
    return WrapRelationship(light.CreatePortalsRel());
}

void UsdLuxDomeLight::OrientToStageUpAxis() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxDomeLight light(usdPrim);
    light.OrientToStageUpAxis();
}

usd::UsdSchemaKind UsdLuxDomeLight::_GetSchemaKind() const {
    return usd::UsdSchemaKind::ConcreteTyped;
}

// ============================================================================
// UsdLuxRectLight Implementation
// ============================================================================

UsdLuxRectLight::UsdLuxRectLight(const usd::UsdPrim& prim)
    : usdGeom::UsdGeomBoundable(prim) {}

UsdLuxRectLight::UsdLuxRectLight(const usd::UsdSchemaBase& schemaObj)
    : usdGeom::UsdGeomBoundable(schemaObj) {}

UsdLuxRectLight::~UsdLuxRectLight() = default;

UsdLuxRectLight UsdLuxRectLight::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    UsdStagePtr stagePtr = GetUsdStagePtr(stage);
    if (!stagePtr) return UsdLuxRectLight();
    UsdPrim prim = stagePtr->GetPrimAtPath(GetSdfPath(path));
    return UsdLuxRectLight(WrapPrim(prim));
}

UsdLuxRectLight UsdLuxRectLight::Define(usd::UsdStage& stage, const sdf::SdfPath& path) {
    usd::UsdPrim prim = stage.DefinePrim(path, tf::TfToken::FromImpl(new pxr::TfToken("RectLight")));
    return UsdLuxRectLight(prim);
}

UsdLuxLightAPI UsdLuxRectLight::LightAPI() const {
    return UsdLuxLightAPI(GetPrim());
}

usd::UsdAttribute UsdLuxRectLight::GetWidthAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxRectLight light(usdPrim);
    return WrapAttribute(light.GetWidthAttr());
}

usd::UsdAttribute UsdLuxRectLight::CreateWidthAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxRectLight light(usdPrim);
    return WrapAttribute(light.CreateWidthAttr());
}

usd::UsdAttribute UsdLuxRectLight::GetHeightAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxRectLight light(usdPrim);
    return WrapAttribute(light.GetHeightAttr());
}

usd::UsdAttribute UsdLuxRectLight::CreateHeightAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxRectLight light(usdPrim);
    return WrapAttribute(light.CreateHeightAttr());
}

usd::UsdAttribute UsdLuxRectLight::GetTextureFileAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxRectLight light(usdPrim);
    return WrapAttribute(light.GetTextureFileAttr());
}

usd::UsdAttribute UsdLuxRectLight::CreateTextureFileAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxRectLight light(usdPrim);
    return WrapAttribute(light.CreateTextureFileAttr());
}

usd::UsdSchemaKind UsdLuxRectLight::_GetSchemaKind() const {
    return usd::UsdSchemaKind::ConcreteTyped;
}

// ============================================================================
// UsdLuxSphereLight Implementation
// ============================================================================

UsdLuxSphereLight::UsdLuxSphereLight(const usd::UsdPrim& prim)
    : usdGeom::UsdGeomBoundable(prim) {}

UsdLuxSphereLight::UsdLuxSphereLight(const usd::UsdSchemaBase& schemaObj)
    : usdGeom::UsdGeomBoundable(schemaObj) {}

UsdLuxSphereLight::~UsdLuxSphereLight() = default;

UsdLuxSphereLight UsdLuxSphereLight::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    UsdStagePtr stagePtr = GetUsdStagePtr(stage);
    if (!stagePtr) return UsdLuxSphereLight();
    UsdPrim prim = stagePtr->GetPrimAtPath(GetSdfPath(path));
    return UsdLuxSphereLight(WrapPrim(prim));
}

UsdLuxSphereLight UsdLuxSphereLight::Define(usd::UsdStage& stage, const sdf::SdfPath& path) {
    usd::UsdPrim prim = stage.DefinePrim(path, tf::TfToken::FromImpl(new pxr::TfToken("SphereLight")));
    return UsdLuxSphereLight(prim);
}

UsdLuxLightAPI UsdLuxSphereLight::LightAPI() const {
    return UsdLuxLightAPI(GetPrim());
}

usd::UsdAttribute UsdLuxSphereLight::GetRadiusAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxSphereLight light(usdPrim);
    return WrapAttribute(light.GetRadiusAttr());
}

usd::UsdAttribute UsdLuxSphereLight::CreateRadiusAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxSphereLight light(usdPrim);
    return WrapAttribute(light.CreateRadiusAttr());
}

usd::UsdAttribute UsdLuxSphereLight::GetTreatAsPointAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxSphereLight light(usdPrim);
    return WrapAttribute(light.GetTreatAsPointAttr());
}

usd::UsdAttribute UsdLuxSphereLight::CreateTreatAsPointAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxSphereLight light(usdPrim);
    return WrapAttribute(light.CreateTreatAsPointAttr());
}

usd::UsdSchemaKind UsdLuxSphereLight::_GetSchemaKind() const {
    return usd::UsdSchemaKind::ConcreteTyped;
}

// ============================================================================
// UsdLuxCylinderLight Implementation
// ============================================================================

UsdLuxCylinderLight::UsdLuxCylinderLight(const usd::UsdPrim& prim)
    : usdGeom::UsdGeomBoundable(prim) {}

UsdLuxCylinderLight::UsdLuxCylinderLight(const usd::UsdSchemaBase& schemaObj)
    : usdGeom::UsdGeomBoundable(schemaObj) {}

UsdLuxCylinderLight::~UsdLuxCylinderLight() = default;

UsdLuxCylinderLight UsdLuxCylinderLight::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    UsdStagePtr stagePtr = GetUsdStagePtr(stage);
    if (!stagePtr) return UsdLuxCylinderLight();
    UsdPrim prim = stagePtr->GetPrimAtPath(GetSdfPath(path));
    return UsdLuxCylinderLight(WrapPrim(prim));
}

UsdLuxCylinderLight UsdLuxCylinderLight::Define(usd::UsdStage& stage, const sdf::SdfPath& path) {
    usd::UsdPrim prim = stage.DefinePrim(path, tf::TfToken::FromImpl(new pxr::TfToken("CylinderLight")));
    return UsdLuxCylinderLight(prim);
}

UsdLuxLightAPI UsdLuxCylinderLight::LightAPI() const {
    return UsdLuxLightAPI(GetPrim());
}

usd::UsdAttribute UsdLuxCylinderLight::GetLengthAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxCylinderLight light(usdPrim);
    return WrapAttribute(light.GetLengthAttr());
}

usd::UsdAttribute UsdLuxCylinderLight::CreateLengthAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxCylinderLight light(usdPrim);
    return WrapAttribute(light.CreateLengthAttr());
}

usd::UsdAttribute UsdLuxCylinderLight::GetRadiusAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxCylinderLight light(usdPrim);
    return WrapAttribute(light.GetRadiusAttr());
}

usd::UsdAttribute UsdLuxCylinderLight::CreateRadiusAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxCylinderLight light(usdPrim);
    return WrapAttribute(light.CreateRadiusAttr());
}

usd::UsdAttribute UsdLuxCylinderLight::GetTreatAsLineAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxCylinderLight light(usdPrim);
    return WrapAttribute(light.GetTreatAsLineAttr());
}

usd::UsdAttribute UsdLuxCylinderLight::CreateTreatAsLineAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxCylinderLight light(usdPrim);
    return WrapAttribute(light.CreateTreatAsLineAttr());
}

usd::UsdSchemaKind UsdLuxCylinderLight::_GetSchemaKind() const {
    return usd::UsdSchemaKind::ConcreteTyped;
}

// ============================================================================
// UsdLuxDiskLight Implementation
// ============================================================================

UsdLuxDiskLight::UsdLuxDiskLight(const usd::UsdPrim& prim)
    : usdGeom::UsdGeomBoundable(prim) {}

UsdLuxDiskLight::UsdLuxDiskLight(const usd::UsdSchemaBase& schemaObj)
    : usdGeom::UsdGeomBoundable(schemaObj) {}

UsdLuxDiskLight::~UsdLuxDiskLight() = default;

UsdLuxDiskLight UsdLuxDiskLight::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    UsdStagePtr stagePtr = GetUsdStagePtr(stage);
    if (!stagePtr) return UsdLuxDiskLight();
    UsdPrim prim = stagePtr->GetPrimAtPath(GetSdfPath(path));
    return UsdLuxDiskLight(WrapPrim(prim));
}

UsdLuxDiskLight UsdLuxDiskLight::Define(usd::UsdStage& stage, const sdf::SdfPath& path) {
    usd::UsdPrim prim = stage.DefinePrim(path, tf::TfToken::FromImpl(new pxr::TfToken("DiskLight")));
    return UsdLuxDiskLight(prim);
}

UsdLuxLightAPI UsdLuxDiskLight::LightAPI() const {
    return UsdLuxLightAPI(GetPrim());
}

usd::UsdAttribute UsdLuxDiskLight::GetRadiusAttr() const {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxDiskLight light(usdPrim);
    return WrapAttribute(light.GetRadiusAttr());
}

usd::UsdAttribute UsdLuxDiskLight::CreateRadiusAttr() {
    UsdPrim usdPrim = GetUsdPrim(GetPrim());
    PXR_NS::UsdLuxDiskLight light(usdPrim);
    return WrapAttribute(light.CreateRadiusAttr());
}

usd::UsdSchemaKind UsdLuxDiskLight::_GetSchemaKind() const {
    return usd::UsdSchemaKind::ConcreteTyped;
}

// ============================================================================
// Token Constants Implementation
// ============================================================================

namespace UsdLuxTokens {

tf::TfToken distantLight() {
    static tf::TfToken token(TfToken("DistantLight").GetString());
    return token;
}

tf::TfToken domeLight() {
    static tf::TfToken token(TfToken("DomeLight").GetString());
    return token;
}

tf::TfToken rectLight() {
    static tf::TfToken token(TfToken("RectLight").GetString());
    return token;
}

tf::TfToken sphereLight() {
    static tf::TfToken token(TfToken("SphereLight").GetString());
    return token;
}

tf::TfToken cylinderLight() {
    static tf::TfToken token(TfToken("CylinderLight").GetString());
    return token;
}

tf::TfToken diskLight() {
    static tf::TfToken token(TfToken("DiskLight").GetString());
    return token;
}

tf::TfToken lightFilter() {
    static tf::TfToken token(TfToken("LightFilter").GetString());
    return token;
}

tf::TfToken automatic() {
    static tf::TfToken token(PXR_NS::UsdLuxTokens->automatic.GetString());
    return token;
}

tf::TfToken latlong() {
    static tf::TfToken token(PXR_NS::UsdLuxTokens->latlong.GetString());
    return token;
}

tf::TfToken mirroredBall() {
    static tf::TfToken token(PXR_NS::UsdLuxTokens->mirroredBall.GetString());
    return token;
}

tf::TfToken angular() {
    static tf::TfToken token(PXR_NS::UsdLuxTokens->angular.GetString());
    return token;
}

tf::TfToken cubeMapVerticalCross() {
    static tf::TfToken token(PXR_NS::UsdLuxTokens->cubeMapVerticalCross.GetString());
    return token;
}

tf::TfToken materialGlowTintsLight() {
    static tf::TfToken token(PXR_NS::UsdLuxTokens->materialGlowTintsLight.GetString());
    return token;
}

tf::TfToken independent() {
    static tf::TfToken token(PXR_NS::UsdLuxTokens->independent.GetString());
    return token;
}

tf::TfToken noMaterialResponse() {
    static tf::TfToken token(PXR_NS::UsdLuxTokens->noMaterialResponse.GetString());
    return token;
}

tf::TfToken consumeAndHalt() {
    static tf::TfToken token(PXR_NS::UsdLuxTokens->consumeAndHalt.GetString());
    return token;
}

tf::TfToken consumeAndContinue() {
    static tf::TfToken token(PXR_NS::UsdLuxTokens->consumeAndContinue.GetString());
    return token;
}

tf::TfToken ignoreCache() {
    static tf::TfToken token(TfToken("ignore").GetString());
    return token;
}

} // namespace UsdLuxTokens

// ============================================================================
// Utility Functions Implementation
// ============================================================================

gf::Vec3f BlackbodyTemperatureAsRgb(float kelvin) {
    GfVec3f rgb = PXR_NS::UsdLuxBlackbodyTemperatureAsRgb(kelvin);
    return gf::Vec3f(rgb[0], rgb[1], rgb[2]);
}

} // namespace usdLux
} // namespace cxxfacade
