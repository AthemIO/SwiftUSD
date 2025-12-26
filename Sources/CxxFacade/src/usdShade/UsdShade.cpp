// UsdShade.cpp - Shading Schemas implementation
#include "../../include/usdShade/UsdShade.hpp"

// Pixar USD includes - ONLY in .cpp files!
#include <pxr/usd/usdShade/input.h>
#include <pxr/usd/usdShade/output.h>
#include <pxr/usd/usdShade/connectableAPI.h>
#include <pxr/usd/usdShade/shader.h>
#include <pxr/usd/usdShade/nodeGraph.h>
#include <pxr/usd/usdShade/material.h>
#include <pxr/usd/usdShade/materialBindingAPI.h>
#include <pxr/usd/usdShade/tokens.h>
#include <pxr/usd/sdf/schema.h>
#include <pxr/base/tf/token.h>
#include <pxr/base/vt/value.h>
#include <cstring>

PXR_NAMESPACE_USING_DIRECTIVE

namespace cxxfacade {
namespace usdShade {

// ============================================================================
// Helper Functions
// ============================================================================

static inline pxr::UsdTimeCode toPxrTimeCode(const usd::UsdTimeCode& tc) {
    if (tc.IsDefault()) {
        return pxr::UsdTimeCode::Default();
    }
    return pxr::UsdTimeCode(tc.GetValue());
}

// Helper to get pxr::UsdPrim from our wrapper
static inline pxr::UsdPrim* asPrim(void* impl) {
    return static_cast<pxr::UsdPrim*>(impl);
}

// Helper to get pxr::UsdAttribute from our wrapper
static inline pxr::UsdAttribute* asAttr(void* impl) {
    return static_cast<pxr::UsdAttribute*>(impl);
}

// Helper to get pxr::UsdRelationship from our wrapper
static inline pxr::UsdRelationship* asRel(void* impl) {
    return static_cast<pxr::UsdRelationship*>(impl);
}

// Helper to get pxr::TfToken from our wrapper
static inline pxr::TfToken toPxrToken(const tf::TfToken& tok) {
    if (tok.GetImpl()) {
        return *static_cast<pxr::TfToken*>(tok.GetImpl());
    }
    return pxr::TfToken();
}

// Helper to get pxr::SdfPath from our wrapper
static inline pxr::SdfPath toPxrPath(const sdf::SdfPath& path) {
    if (path.GetImpl()) {
        return *static_cast<pxr::SdfPath*>(path.GetImpl());
    }
    return pxr::SdfPath();
}

// Helper to get pxr::VtValue from our wrapper
static inline pxr::VtValue toPxrValue(const vt::VtValue& val) {
    if (val.GetImpl()) {
        return *static_cast<pxr::VtValue*>(val.GetImpl());
    }
    return pxr::VtValue();
}

// ============================================================================
// UsdShadeInput implementation
// ============================================================================

static inline pxr::UsdShadeInput* asInput(void* impl) {
    return static_cast<pxr::UsdShadeInput*>(impl);
}

UsdShadeInput::UsdShadeInput() : impl_(new pxr::UsdShadeInput()) {}

UsdShadeInput::UsdShadeInput(const UsdShadeInput& other)
    : impl_(new pxr::UsdShadeInput(*asInput(other.impl_))) {}

UsdShadeInput::UsdShadeInput(UsdShadeInput&& other) noexcept : impl_(other.impl_) {
    other.impl_ = nullptr;
}

UsdShadeInput::~UsdShadeInput() {
    delete asInput(impl_);
}

UsdShadeInput& UsdShadeInput::operator=(const UsdShadeInput& other) {
    if (this != &other) {
        delete asInput(impl_);
        impl_ = new pxr::UsdShadeInput(*asInput(other.impl_));
    }
    return *this;
}

UsdShadeInput& UsdShadeInput::operator=(UsdShadeInput&& other) noexcept {
    if (this != &other) {
        delete asInput(impl_);
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

bool UsdShadeInput::IsValid() const {
    return impl_ && asInput(impl_)->IsDefined();
}

tf::TfToken UsdShadeInput::GetFullName() const {
    if (!IsValid()) return tf::TfToken();
    return tf::TfToken::FromImpl(new pxr::TfToken(asInput(impl_)->GetFullName()));
}

tf::TfToken UsdShadeInput::GetBaseName() const {
    if (!IsValid()) return tf::TfToken();
    return tf::TfToken::FromImpl(new pxr::TfToken(asInput(impl_)->GetBaseName()));
}

std::string UsdShadeInput::GetTypeName() const {
    if (!IsValid()) return "";
    return asInput(impl_)->GetTypeName().GetAsToken().GetString();
}

usd::UsdPrim UsdShadeInput::GetPrim() const {
    if (!IsValid()) return usd::UsdPrim();
    return usd::UsdPrim::FromImpl(new pxr::UsdPrim(asInput(impl_)->GetPrim()));
}

usd::UsdAttribute UsdShadeInput::GetAttr() const {
    if (!IsValid()) return usd::UsdAttribute();
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(asInput(impl_)->GetAttr()));
}

bool UsdShadeInput::Get(vt::VtValue* value, usd::UsdTimeCode time) const {
    if (!IsValid() || !value) return false;
    pxr::VtValue pxrVal;
    bool success = asInput(impl_)->Get(&pxrVal, toPxrTimeCode(time));
    if (success) {
        *value = vt::VtValue::FromImpl(new pxr::VtValue(pxrVal));
    }
    return success;
}

bool UsdShadeInput::Set(const vt::VtValue& value, usd::UsdTimeCode time) {
    if (!IsValid()) return false;
    return asInput(impl_)->Set(toPxrValue(value), toPxrTimeCode(time));
}

bool UsdShadeInput::SetRenderType(const tf::TfToken& renderType) {
    if (!IsValid()) return false;
    return asInput(impl_)->SetRenderType(toPxrToken(renderType));
}

tf::TfToken UsdShadeInput::GetRenderType() const {
    if (!IsValid()) return tf::TfToken();
    return tf::TfToken::FromImpl(new pxr::TfToken(asInput(impl_)->GetRenderType()));
}

bool UsdShadeInput::HasRenderType() const {
    return IsValid() && asInput(impl_)->HasRenderType();
}

bool UsdShadeInput::SetConnectability(const tf::TfToken& connectability) {
    if (!IsValid()) return false;
    return asInput(impl_)->SetConnectability(toPxrToken(connectability));
}

tf::TfToken UsdShadeInput::GetConnectability() const {
    if (!IsValid()) return tf::TfToken();
    return tf::TfToken::FromImpl(new pxr::TfToken(asInput(impl_)->GetConnectability()));
}

bool UsdShadeInput::ClearConnectability() const {
    if (!IsValid()) return false;
    return asInput(impl_)->ClearConnectability();
}

bool UsdShadeInput::CanConnect(const UsdShadeInput& source) const {
    if (!IsValid() || !source.IsValid()) return false;
    return pxr::UsdShadeConnectableAPI::CanConnect(*asInput(impl_), *asInput(source.impl_));
}

bool UsdShadeInput::CanConnect(const UsdShadeOutput& source) const {
    if (!IsValid() || !source.IsValid()) return false;
    return pxr::UsdShadeConnectableAPI::CanConnect(*asInput(impl_),
        *static_cast<pxr::UsdShadeOutput*>(source.GetImpl()));
}

bool UsdShadeInput::ConnectToSource(const sdf::SdfPath& sourcePath) const {
    if (!IsValid()) return false;
    return pxr::UsdShadeConnectableAPI::ConnectToSource(*asInput(impl_), toPxrPath(sourcePath));
}

bool UsdShadeInput::ConnectToSource(const UsdShadeInput& sourceInput) const {
    if (!IsValid() || !sourceInput.IsValid()) return false;
    return pxr::UsdShadeConnectableAPI::ConnectToSource(*asInput(impl_), *asInput(sourceInput.impl_));
}

bool UsdShadeInput::ConnectToSource(const UsdShadeOutput& sourceOutput) const {
    if (!IsValid() || !sourceOutput.IsValid()) return false;
    return pxr::UsdShadeConnectableAPI::ConnectToSource(*asInput(impl_),
        *static_cast<pxr::UsdShadeOutput*>(sourceOutput.GetImpl()));
}

bool UsdShadeInput::HasConnectedSource() const {
    return IsValid() && asInput(impl_)->HasConnectedSource();
}

bool UsdShadeInput::DisconnectSource() const {
    if (!IsValid()) return false;
    return pxr::UsdShadeConnectableAPI::DisconnectSource(*asInput(impl_));
}

bool UsdShadeInput::ClearSources() const {
    if (!IsValid()) return false;
    return pxr::UsdShadeConnectableAPI::ClearSources(*asInput(impl_));
}

bool UsdShadeInput::SetDocumentation(const std::string& docs) {
    if (!IsValid()) return false;
    return asInput(impl_)->SetDocumentation(docs);
}

std::string UsdShadeInput::GetDocumentation() const {
    if (!IsValid()) return "";
    return asInput(impl_)->GetDocumentation();
}

bool UsdShadeInput::SetDisplayGroup(const std::string& displayGroup) {
    if (!IsValid()) return false;
    return asInput(impl_)->SetDisplayGroup(displayGroup);
}

std::string UsdShadeInput::GetDisplayGroup() const {
    if (!IsValid()) return "";
    return asInput(impl_)->GetDisplayGroup();
}

bool UsdShadeInput::operator==(const UsdShadeInput& other) const {
    if (!IsValid() || !other.IsValid()) return false;
    return *asInput(impl_) == *asInput(other.impl_);
}

UsdShadeInput UsdShadeInput::FromImpl(void* impl) {
    UsdShadeInput input;
    delete asInput(input.impl_);
    input.impl_ = impl;
    return input;
}

// ============================================================================
// UsdShadeOutput implementation
// ============================================================================

static inline pxr::UsdShadeOutput* asOutput(void* impl) {
    return static_cast<pxr::UsdShadeOutput*>(impl);
}

UsdShadeOutput::UsdShadeOutput() : impl_(new pxr::UsdShadeOutput()) {}

UsdShadeOutput::UsdShadeOutput(const UsdShadeOutput& other)
    : impl_(new pxr::UsdShadeOutput(*asOutput(other.impl_))) {}

UsdShadeOutput::UsdShadeOutput(UsdShadeOutput&& other) noexcept : impl_(other.impl_) {
    other.impl_ = nullptr;
}

UsdShadeOutput::~UsdShadeOutput() {
    delete asOutput(impl_);
}

UsdShadeOutput& UsdShadeOutput::operator=(const UsdShadeOutput& other) {
    if (this != &other) {
        delete asOutput(impl_);
        impl_ = new pxr::UsdShadeOutput(*asOutput(other.impl_));
    }
    return *this;
}

UsdShadeOutput& UsdShadeOutput::operator=(UsdShadeOutput&& other) noexcept {
    if (this != &other) {
        delete asOutput(impl_);
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

bool UsdShadeOutput::IsValid() const {
    return impl_ && asOutput(impl_)->IsDefined();
}

tf::TfToken UsdShadeOutput::GetFullName() const {
    if (!IsValid()) return tf::TfToken();
    return tf::TfToken::FromImpl(new pxr::TfToken(asOutput(impl_)->GetFullName()));
}

tf::TfToken UsdShadeOutput::GetBaseName() const {
    if (!IsValid()) return tf::TfToken();
    return tf::TfToken::FromImpl(new pxr::TfToken(asOutput(impl_)->GetBaseName()));
}

std::string UsdShadeOutput::GetTypeName() const {
    if (!IsValid()) return "";
    return asOutput(impl_)->GetTypeName().GetAsToken().GetString();
}

usd::UsdPrim UsdShadeOutput::GetPrim() const {
    if (!IsValid()) return usd::UsdPrim();
    return usd::UsdPrim::FromImpl(new pxr::UsdPrim(asOutput(impl_)->GetPrim()));
}

usd::UsdAttribute UsdShadeOutput::GetAttr() const {
    if (!IsValid()) return usd::UsdAttribute();
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(asOutput(impl_)->GetAttr()));
}

bool UsdShadeOutput::Set(const vt::VtValue& value, usd::UsdTimeCode time) {
    if (!IsValid()) return false;
    return asOutput(impl_)->Set(toPxrValue(value), toPxrTimeCode(time));
}

bool UsdShadeOutput::SetRenderType(const tf::TfToken& renderType) {
    if (!IsValid()) return false;
    return asOutput(impl_)->SetRenderType(toPxrToken(renderType));
}

tf::TfToken UsdShadeOutput::GetRenderType() const {
    if (!IsValid()) return tf::TfToken();
    return tf::TfToken::FromImpl(new pxr::TfToken(asOutput(impl_)->GetRenderType()));
}

bool UsdShadeOutput::HasRenderType() const {
    return IsValid() && asOutput(impl_)->HasRenderType();
}

bool UsdShadeOutput::CanConnect(const UsdShadeInput& source) const {
    if (!IsValid() || !source.IsValid()) return false;
    return pxr::UsdShadeConnectableAPI::CanConnect(*asOutput(impl_), *asInput(source.GetImpl()));
}

bool UsdShadeOutput::CanConnect(const UsdShadeOutput& source) const {
    if (!IsValid() || !source.IsValid()) return false;
    return pxr::UsdShadeConnectableAPI::CanConnect(*asOutput(impl_), *asOutput(source.impl_));
}

bool UsdShadeOutput::ConnectToSource(const sdf::SdfPath& sourcePath) const {
    if (!IsValid()) return false;
    return pxr::UsdShadeConnectableAPI::ConnectToSource(*asOutput(impl_), toPxrPath(sourcePath));
}

bool UsdShadeOutput::ConnectToSource(const UsdShadeInput& sourceInput) const {
    if (!IsValid() || !sourceInput.IsValid()) return false;
    return pxr::UsdShadeConnectableAPI::ConnectToSource(*asOutput(impl_), *asInput(sourceInput.GetImpl()));
}

bool UsdShadeOutput::ConnectToSource(const UsdShadeOutput& sourceOutput) const {
    if (!IsValid() || !sourceOutput.IsValid()) return false;
    return pxr::UsdShadeConnectableAPI::ConnectToSource(*asOutput(impl_), *asOutput(sourceOutput.impl_));
}

bool UsdShadeOutput::HasConnectedSource() const {
    return IsValid() && asOutput(impl_)->HasConnectedSource();
}

bool UsdShadeOutput::DisconnectSource() const {
    if (!IsValid()) return false;
    return pxr::UsdShadeConnectableAPI::DisconnectSource(*asOutput(impl_));
}

bool UsdShadeOutput::ClearSources() const {
    if (!IsValid()) return false;
    return pxr::UsdShadeConnectableAPI::ClearSources(*asOutput(impl_));
}

bool UsdShadeOutput::operator==(const UsdShadeOutput& other) const {
    if (!IsValid() || !other.IsValid()) return false;
    return *asOutput(impl_) == *asOutput(other.impl_);
}

UsdShadeOutput UsdShadeOutput::FromImpl(void* impl) {
    UsdShadeOutput output;
    delete asOutput(output.impl_);
    output.impl_ = impl;
    return output;
}

// ============================================================================
// UsdShadeConnectableAPI implementation
// ============================================================================

UsdShadeConnectableAPI::UsdShadeConnectableAPI(const usd::UsdPrim& prim) : UsdAPISchemaBase(prim) {}

UsdShadeConnectableAPI::UsdShadeConnectableAPI(const usd::UsdSchemaBase& schemaObj) : UsdAPISchemaBase(schemaObj) {}

UsdShadeConnectableAPI::~UsdShadeConnectableAPI() {}

UsdShadeConnectableAPI UsdShadeConnectableAPI::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    return UsdShadeConnectableAPI(stage.GetPrimAtPath(path));
}

bool UsdShadeConnectableAPI::IsContainer() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return false;
    pxr::UsdShadeConnectableAPI api(*asPrim(prim.GetImpl()));
    return api.IsContainer();
}

bool UsdShadeConnectableAPI::RequiresEncapsulation() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return true;
    pxr::UsdShadeConnectableAPI api(*asPrim(prim.GetImpl()));
    return api.RequiresEncapsulation();
}

UsdShadeInput UsdShadeConnectableAPI::CreateInput(const tf::TfToken& name, const std::string& typeName) {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdShadeInput();
    pxr::UsdShadeConnectableAPI api(*asPrim(prim.GetImpl()));
    pxr::SdfValueTypeName valueType = pxr::SdfSchema::GetInstance().FindType(typeName);
    return UsdShadeInput::FromImpl(new pxr::UsdShadeInput(api.CreateInput(toPxrToken(name), valueType)));
}

UsdShadeInput UsdShadeConnectableAPI::GetInput(const tf::TfToken& name) const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdShadeInput();
    pxr::UsdShadeConnectableAPI api(*asPrim(prim.GetImpl()));
    return UsdShadeInput::FromImpl(new pxr::UsdShadeInput(api.GetInput(toPxrToken(name))));
}

std::vector<UsdShadeInput> UsdShadeConnectableAPI::GetInputs(bool onlyAuthored) const {
    std::vector<UsdShadeInput> result;
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return result;
    pxr::UsdShadeConnectableAPI api(*asPrim(prim.GetImpl()));
    std::vector<pxr::UsdShadeInput> pxrInputs = api.GetInputs(onlyAuthored);
    result.reserve(pxrInputs.size());
    for (const auto& inp : pxrInputs) {
        result.push_back(UsdShadeInput::FromImpl(new pxr::UsdShadeInput(inp)));
    }
    return result;
}

UsdShadeOutput UsdShadeConnectableAPI::CreateOutput(const tf::TfToken& name, const std::string& typeName) {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdShadeOutput();
    pxr::UsdShadeConnectableAPI api(*asPrim(prim.GetImpl()));
    pxr::SdfValueTypeName valueType = pxr::SdfSchema::GetInstance().FindType(typeName);
    return UsdShadeOutput::FromImpl(new pxr::UsdShadeOutput(api.CreateOutput(toPxrToken(name), valueType)));
}

UsdShadeOutput UsdShadeConnectableAPI::GetOutput(const tf::TfToken& name) const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdShadeOutput();
    pxr::UsdShadeConnectableAPI api(*asPrim(prim.GetImpl()));
    return UsdShadeOutput::FromImpl(new pxr::UsdShadeOutput(api.GetOutput(toPxrToken(name))));
}

std::vector<UsdShadeOutput> UsdShadeConnectableAPI::GetOutputs(bool onlyAuthored) const {
    std::vector<UsdShadeOutput> result;
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return result;
    pxr::UsdShadeConnectableAPI api(*asPrim(prim.GetImpl()));
    std::vector<pxr::UsdShadeOutput> pxrOutputs = api.GetOutputs(onlyAuthored);
    result.reserve(pxrOutputs.size());
    for (const auto& out : pxrOutputs) {
        result.push_back(UsdShadeOutput::FromImpl(new pxr::UsdShadeOutput(out)));
    }
    return result;
}

bool UsdShadeConnectableAPI::CanConnect(const UsdShadeInput& input, const usd::UsdAttribute& source) {
    if (!input.IsValid() || !source.IsValid()) return false;
    return pxr::UsdShadeConnectableAPI::CanConnect(*asInput(input.GetImpl()), *asAttr(source.GetImpl()));
}

bool UsdShadeConnectableAPI::CanConnect(const UsdShadeOutput& output, const usd::UsdAttribute& source) {
    if (!output.IsValid() || !source.IsValid()) return false;
    return pxr::UsdShadeConnectableAPI::CanConnect(*asOutput(output.GetImpl()), *asAttr(source.GetImpl()));
}

bool UsdShadeConnectableAPI::ConnectToSource(const usd::UsdAttribute& attr, const sdf::SdfPath& sourcePath) {
    if (!attr.IsValid()) return false;
    return pxr::UsdShadeConnectableAPI::ConnectToSource(*asAttr(attr.GetImpl()), toPxrPath(sourcePath));
}

bool UsdShadeConnectableAPI::HasConnectedSource(const usd::UsdAttribute& attr) {
    if (!attr.IsValid()) return false;
    return pxr::UsdShadeConnectableAPI::HasConnectedSource(*asAttr(attr.GetImpl()));
}

bool UsdShadeConnectableAPI::DisconnectSource(const usd::UsdAttribute& attr) {
    if (!attr.IsValid()) return false;
    return pxr::UsdShadeConnectableAPI::DisconnectSource(*asAttr(attr.GetImpl()));
}

bool UsdShadeConnectableAPI::ClearSources(const usd::UsdAttribute& attr) {
    if (!attr.IsValid()) return false;
    return pxr::UsdShadeConnectableAPI::ClearSources(*asAttr(attr.GetImpl()));
}

usd::UsdSchemaKind UsdShadeConnectableAPI::_GetSchemaKind() const {
    return usd::UsdSchemaKind::NonAppliedAPI;
}

// ============================================================================
// UsdShadeShader implementation
// ============================================================================

UsdShadeShader::UsdShadeShader(const usd::UsdPrim& prim) : UsdTyped(prim) {}

UsdShadeShader::UsdShadeShader(const usd::UsdSchemaBase& schemaObj) : UsdTyped(schemaObj) {}

UsdShadeShader::~UsdShadeShader() {}

UsdShadeShader UsdShadeShader::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    return UsdShadeShader(stage.GetPrimAtPath(path));
}

UsdShadeShader UsdShadeShader::Define(usd::UsdStage& stage, const sdf::SdfPath& path) {
    pxr::UsdStageRefPtr pxrStage = *static_cast<pxr::UsdStageRefPtr*>(stage.GetImpl());
    pxr::UsdShadeShader shader = pxr::UsdShadeShader::Define(pxrStage, toPxrPath(path));
    return UsdShadeShader(usd::UsdPrim::FromImpl(new pxr::UsdPrim(shader.GetPrim())));
}

UsdShadeConnectableAPI UsdShadeShader::ConnectableAPI() const {
    return UsdShadeConnectableAPI(GetPrim());
}

usd::UsdAttribute UsdShadeShader::GetImplementationSourceAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdShadeShader shader(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(shader.GetImplementationSourceAttr()));
}

usd::UsdAttribute UsdShadeShader::GetIdAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdShadeShader shader(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(shader.GetIdAttr()));
}

usd::UsdAttribute UsdShadeShader::CreateIdAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdShadeShader shader(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(shader.CreateIdAttr()));
}

tf::TfToken UsdShadeShader::GetImplementationSource() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return tf::TfToken();
    pxr::UsdShadeShader shader(*asPrim(prim.GetImpl()));
    return tf::TfToken::FromImpl(new pxr::TfToken(shader.GetImplementationSource()));
}

bool UsdShadeShader::SetShaderId(const tf::TfToken& id) {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return false;
    pxr::UsdShadeShader shader(*asPrim(prim.GetImpl()));
    return shader.SetShaderId(toPxrToken(id));
}

bool UsdShadeShader::GetShaderId(tf::TfToken* id) const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid() || !id) return false;
    pxr::UsdShadeShader shader(*asPrim(prim.GetImpl()));
    pxr::TfToken pxrId;
    bool success = shader.GetShaderId(&pxrId);
    if (success) {
        *id = tf::TfToken::FromImpl(new pxr::TfToken(pxrId));
    }
    return success;
}

bool UsdShadeShader::SetSourceAsset(const sdf::SdfAssetPath& sourceAsset, const tf::TfToken& sourceType) {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return false;
    pxr::UsdShadeShader shader(*asPrim(prim.GetImpl()));
    pxr::SdfAssetPath pxrPath(sourceAsset.GetAssetPath(), sourceAsset.GetResolvedPath());
    pxr::TfToken pxrSourceType = strlen(sourceType.GetText()) > 0 ? toPxrToken(sourceType) : pxr::UsdShadeTokens->universalSourceType;
    return shader.SetSourceAsset(pxrPath, pxrSourceType);
}

bool UsdShadeShader::GetSourceAsset(sdf::SdfAssetPath* sourceAsset, const tf::TfToken& sourceType) const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid() || !sourceAsset) return false;
    pxr::UsdShadeShader shader(*asPrim(prim.GetImpl()));
    pxr::SdfAssetPath pxrPath;
    pxr::TfToken pxrSourceType = strlen(sourceType.GetText()) > 0 ? toPxrToken(sourceType) : pxr::UsdShadeTokens->universalSourceType;
    bool success = shader.GetSourceAsset(&pxrPath, pxrSourceType);
    if (success) {
        *sourceAsset = sdf::SdfAssetPath(pxrPath.GetAssetPath(), pxrPath.GetResolvedPath());
    }
    return success;
}

bool UsdShadeShader::SetSourceAssetSubIdentifier(const tf::TfToken& subIdentifier, const tf::TfToken& sourceType) {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return false;
    pxr::UsdShadeShader shader(*asPrim(prim.GetImpl()));
    pxr::TfToken pxrSourceType = strlen(sourceType.GetText()) > 0 ? toPxrToken(sourceType) : pxr::UsdShadeTokens->universalSourceType;
    return shader.SetSourceAssetSubIdentifier(toPxrToken(subIdentifier), pxrSourceType);
}

bool UsdShadeShader::GetSourceAssetSubIdentifier(tf::TfToken* subIdentifier, const tf::TfToken& sourceType) const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid() || !subIdentifier) return false;
    pxr::UsdShadeShader shader(*asPrim(prim.GetImpl()));
    pxr::TfToken pxrSubId;
    pxr::TfToken pxrSourceType = strlen(sourceType.GetText()) > 0 ? toPxrToken(sourceType) : pxr::UsdShadeTokens->universalSourceType;
    bool success = shader.GetSourceAssetSubIdentifier(&pxrSubId, pxrSourceType);
    if (success) {
        *subIdentifier = tf::TfToken::FromImpl(new pxr::TfToken(pxrSubId));
    }
    return success;
}

bool UsdShadeShader::SetSourceCode(const std::string& sourceCode, const tf::TfToken& sourceType) {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return false;
    pxr::UsdShadeShader shader(*asPrim(prim.GetImpl()));
    pxr::TfToken pxrSourceType = strlen(sourceType.GetText()) > 0 ? toPxrToken(sourceType) : pxr::UsdShadeTokens->universalSourceType;
    return shader.SetSourceCode(sourceCode, pxrSourceType);
}

bool UsdShadeShader::GetSourceCode(std::string* sourceCode, const tf::TfToken& sourceType) const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid() || !sourceCode) return false;
    pxr::UsdShadeShader shader(*asPrim(prim.GetImpl()));
    pxr::TfToken pxrSourceType = strlen(sourceType.GetText()) > 0 ? toPxrToken(sourceType) : pxr::UsdShadeTokens->universalSourceType;
    return shader.GetSourceCode(sourceCode, pxrSourceType);
}

std::vector<std::string> UsdShadeShader::GetSourceTypes() const {
    std::vector<std::string> result;
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return result;
    pxr::UsdShadeShader shader(*asPrim(prim.GetImpl()));
    std::vector<std::string> types = shader.GetSourceTypes();
    return types;
}

UsdShadeInput UsdShadeShader::CreateInput(const tf::TfToken& name, const std::string& typeName) {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdShadeInput();
    pxr::UsdShadeShader shader(*asPrim(prim.GetImpl()));
    pxr::SdfValueTypeName valueType = pxr::SdfSchema::GetInstance().FindType(typeName);
    return UsdShadeInput::FromImpl(new pxr::UsdShadeInput(shader.CreateInput(toPxrToken(name), valueType)));
}

UsdShadeInput UsdShadeShader::GetInput(const tf::TfToken& name) const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdShadeInput();
    pxr::UsdShadeShader shader(*asPrim(prim.GetImpl()));
    return UsdShadeInput::FromImpl(new pxr::UsdShadeInput(shader.GetInput(toPxrToken(name))));
}

std::vector<UsdShadeInput> UsdShadeShader::GetInputs(bool onlyAuthored) const {
    std::vector<UsdShadeInput> result;
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return result;
    pxr::UsdShadeShader shader(*asPrim(prim.GetImpl()));
    std::vector<pxr::UsdShadeInput> pxrInputs = shader.GetInputs(onlyAuthored);
    result.reserve(pxrInputs.size());
    for (const auto& inp : pxrInputs) {
        result.push_back(UsdShadeInput::FromImpl(new pxr::UsdShadeInput(inp)));
    }
    return result;
}

UsdShadeOutput UsdShadeShader::CreateOutput(const tf::TfToken& name, const std::string& typeName) {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdShadeOutput();
    pxr::UsdShadeShader shader(*asPrim(prim.GetImpl()));
    pxr::SdfValueTypeName valueType = pxr::SdfSchema::GetInstance().FindType(typeName);
    return UsdShadeOutput::FromImpl(new pxr::UsdShadeOutput(shader.CreateOutput(toPxrToken(name), valueType)));
}

UsdShadeOutput UsdShadeShader::GetOutput(const tf::TfToken& name) const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdShadeOutput();
    pxr::UsdShadeShader shader(*asPrim(prim.GetImpl()));
    return UsdShadeOutput::FromImpl(new pxr::UsdShadeOutput(shader.GetOutput(toPxrToken(name))));
}

std::vector<UsdShadeOutput> UsdShadeShader::GetOutputs(bool onlyAuthored) const {
    std::vector<UsdShadeOutput> result;
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return result;
    pxr::UsdShadeShader shader(*asPrim(prim.GetImpl()));
    std::vector<pxr::UsdShadeOutput> pxrOutputs = shader.GetOutputs(onlyAuthored);
    result.reserve(pxrOutputs.size());
    for (const auto& out : pxrOutputs) {
        result.push_back(UsdShadeOutput::FromImpl(new pxr::UsdShadeOutput(out)));
    }
    return result;
}

usd::UsdSchemaKind UsdShadeShader::_GetSchemaKind() const {
    return usd::UsdSchemaKind::ConcreteTyped;
}

// ============================================================================
// UsdShadeNodeGraph implementation
// ============================================================================

UsdShadeNodeGraph::UsdShadeNodeGraph(const usd::UsdPrim& prim) : UsdTyped(prim) {}

UsdShadeNodeGraph::UsdShadeNodeGraph(const usd::UsdSchemaBase& schemaObj) : UsdTyped(schemaObj) {}

UsdShadeNodeGraph::~UsdShadeNodeGraph() {}

UsdShadeNodeGraph UsdShadeNodeGraph::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    return UsdShadeNodeGraph(stage.GetPrimAtPath(path));
}

UsdShadeNodeGraph UsdShadeNodeGraph::Define(usd::UsdStage& stage, const sdf::SdfPath& path) {
    pxr::UsdStageRefPtr pxrStage = *static_cast<pxr::UsdStageRefPtr*>(stage.GetImpl());
    pxr::UsdShadeNodeGraph nodeGraph = pxr::UsdShadeNodeGraph::Define(pxrStage, toPxrPath(path));
    return UsdShadeNodeGraph(usd::UsdPrim::FromImpl(new pxr::UsdPrim(nodeGraph.GetPrim())));
}

UsdShadeConnectableAPI UsdShadeNodeGraph::ConnectableAPI() const {
    return UsdShadeConnectableAPI(GetPrim());
}

UsdShadeInput UsdShadeNodeGraph::CreateInput(const tf::TfToken& name, const std::string& typeName) {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdShadeInput();
    pxr::UsdShadeNodeGraph nodeGraph(*asPrim(prim.GetImpl()));
    pxr::SdfValueTypeName valueType = pxr::SdfSchema::GetInstance().FindType(typeName);
    return UsdShadeInput::FromImpl(new pxr::UsdShadeInput(nodeGraph.CreateInput(toPxrToken(name), valueType)));
}

UsdShadeInput UsdShadeNodeGraph::GetInput(const tf::TfToken& name) const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdShadeInput();
    pxr::UsdShadeNodeGraph nodeGraph(*asPrim(prim.GetImpl()));
    return UsdShadeInput::FromImpl(new pxr::UsdShadeInput(nodeGraph.GetInput(toPxrToken(name))));
}

std::vector<UsdShadeInput> UsdShadeNodeGraph::GetInputs(bool onlyAuthored) const {
    std::vector<UsdShadeInput> result;
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return result;
    pxr::UsdShadeNodeGraph nodeGraph(*asPrim(prim.GetImpl()));
    std::vector<pxr::UsdShadeInput> pxrInputs = nodeGraph.GetInputs(onlyAuthored);
    result.reserve(pxrInputs.size());
    for (const auto& inp : pxrInputs) {
        result.push_back(UsdShadeInput::FromImpl(new pxr::UsdShadeInput(inp)));
    }
    return result;
}

std::vector<UsdShadeInput> UsdShadeNodeGraph::GetInterfaceInputs() const {
    std::vector<UsdShadeInput> result;
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return result;
    pxr::UsdShadeNodeGraph nodeGraph(*asPrim(prim.GetImpl()));
    std::vector<pxr::UsdShadeInput> pxrInputs = nodeGraph.GetInterfaceInputs();
    result.reserve(pxrInputs.size());
    for (const auto& inp : pxrInputs) {
        result.push_back(UsdShadeInput::FromImpl(new pxr::UsdShadeInput(inp)));
    }
    return result;
}

UsdShadeOutput UsdShadeNodeGraph::CreateOutput(const tf::TfToken& name, const std::string& typeName) {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdShadeOutput();
    pxr::UsdShadeNodeGraph nodeGraph(*asPrim(prim.GetImpl()));
    pxr::SdfValueTypeName valueType = pxr::SdfSchema::GetInstance().FindType(typeName);
    return UsdShadeOutput::FromImpl(new pxr::UsdShadeOutput(nodeGraph.CreateOutput(toPxrToken(name), valueType)));
}

UsdShadeOutput UsdShadeNodeGraph::GetOutput(const tf::TfToken& name) const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdShadeOutput();
    pxr::UsdShadeNodeGraph nodeGraph(*asPrim(prim.GetImpl()));
    return UsdShadeOutput::FromImpl(new pxr::UsdShadeOutput(nodeGraph.GetOutput(toPxrToken(name))));
}

std::vector<UsdShadeOutput> UsdShadeNodeGraph::GetOutputs(bool onlyAuthored) const {
    std::vector<UsdShadeOutput> result;
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return result;
    pxr::UsdShadeNodeGraph nodeGraph(*asPrim(prim.GetImpl()));
    std::vector<pxr::UsdShadeOutput> pxrOutputs = nodeGraph.GetOutputs(onlyAuthored);
    result.reserve(pxrOutputs.size());
    for (const auto& out : pxrOutputs) {
        result.push_back(UsdShadeOutput::FromImpl(new pxr::UsdShadeOutput(out)));
    }
    return result;
}

UsdShadeShader UsdShadeNodeGraph::ComputeOutputSource(const tf::TfToken& outputName,
                                                       tf::TfToken* sourceName,
                                                       UsdShadeAttributeType* sourceType) const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdShadeShader();
    pxr::UsdShadeNodeGraph nodeGraph(*asPrim(prim.GetImpl()));
    pxr::TfToken pxrSourceName;
    pxr::UsdShadeAttributeType pxrSourceType;
    pxr::UsdShadeShader shader = nodeGraph.ComputeOutputSource(toPxrToken(outputName), &pxrSourceName, &pxrSourceType);
    if (sourceName) {
        *sourceName = tf::TfToken::FromImpl(new pxr::TfToken(pxrSourceName));
    }
    if (sourceType) {
        switch (pxrSourceType) {
            case pxr::UsdShadeAttributeType::Input: *sourceType = UsdShadeAttributeType::Input; break;
            case pxr::UsdShadeAttributeType::Output: *sourceType = UsdShadeAttributeType::Output; break;
            default: *sourceType = UsdShadeAttributeType::Invalid; break;
        }
    }
    return UsdShadeShader(usd::UsdPrim::FromImpl(new pxr::UsdPrim(shader.GetPrim())));
}

usd::UsdSchemaKind UsdShadeNodeGraph::_GetSchemaKind() const {
    return usd::UsdSchemaKind::ConcreteTyped;
}

// ============================================================================
// UsdShadeMaterial implementation
// ============================================================================

UsdShadeMaterial::UsdShadeMaterial(const usd::UsdPrim& prim) : UsdShadeNodeGraph(prim) {}

UsdShadeMaterial::UsdShadeMaterial(const usd::UsdSchemaBase& schemaObj) : UsdShadeNodeGraph(schemaObj) {}

UsdShadeMaterial::~UsdShadeMaterial() {}

UsdShadeMaterial UsdShadeMaterial::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    return UsdShadeMaterial(stage.GetPrimAtPath(path));
}

UsdShadeMaterial UsdShadeMaterial::Define(usd::UsdStage& stage, const sdf::SdfPath& path) {
    pxr::UsdStageRefPtr pxrStage = *static_cast<pxr::UsdStageRefPtr*>(stage.GetImpl());
    pxr::UsdShadeMaterial material = pxr::UsdShadeMaterial::Define(pxrStage, toPxrPath(path));
    return UsdShadeMaterial(usd::UsdPrim::FromImpl(new pxr::UsdPrim(material.GetPrim())));
}

usd::UsdAttribute UsdShadeMaterial::GetSurfaceAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdShadeMaterial material(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(material.GetSurfaceAttr()));
}

usd::UsdAttribute UsdShadeMaterial::CreateSurfaceAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdShadeMaterial material(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(material.CreateSurfaceAttr()));
}

UsdShadeOutput UsdShadeMaterial::CreateSurfaceOutput(const tf::TfToken& renderContext) {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdShadeOutput();
    pxr::UsdShadeMaterial material(*asPrim(prim.GetImpl()));
    pxr::TfToken pxrContext = strlen(renderContext.GetText()) > 0 ? toPxrToken(renderContext) : pxr::TfToken();
    return UsdShadeOutput::FromImpl(new pxr::UsdShadeOutput(material.CreateSurfaceOutput(pxrContext)));
}

UsdShadeOutput UsdShadeMaterial::GetSurfaceOutput(const tf::TfToken& renderContext) const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdShadeOutput();
    pxr::UsdShadeMaterial material(*asPrim(prim.GetImpl()));
    pxr::TfToken pxrContext = strlen(renderContext.GetText()) > 0 ? toPxrToken(renderContext) : pxr::TfToken();
    return UsdShadeOutput::FromImpl(new pxr::UsdShadeOutput(material.GetSurfaceOutput(pxrContext)));
}

std::vector<UsdShadeOutput> UsdShadeMaterial::GetSurfaceOutputs() const {
    std::vector<UsdShadeOutput> result;
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return result;
    pxr::UsdShadeMaterial material(*asPrim(prim.GetImpl()));
    std::vector<pxr::UsdShadeOutput> pxrOutputs = material.GetSurfaceOutputs();
    result.reserve(pxrOutputs.size());
    for (const auto& out : pxrOutputs) {
        result.push_back(UsdShadeOutput::FromImpl(new pxr::UsdShadeOutput(out)));
    }
    return result;
}

UsdShadeShader UsdShadeMaterial::ComputeSurfaceSource(const tf::TfToken& renderContext,
                                                       tf::TfToken* sourceName,
                                                       UsdShadeAttributeType* sourceType) const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdShadeShader();
    pxr::UsdShadeMaterial material(*asPrim(prim.GetImpl()));
    pxr::TfToken pxrContext = strlen(renderContext.GetText()) > 0 ? toPxrToken(renderContext) : pxr::TfToken();
    pxr::TfToken pxrSourceName;
    pxr::UsdShadeAttributeType pxrSourceType;
    pxr::UsdShadeShader shader = material.ComputeSurfaceSource(pxrContext, &pxrSourceName, &pxrSourceType);
    if (sourceName) {
        *sourceName = tf::TfToken::FromImpl(new pxr::TfToken(pxrSourceName));
    }
    if (sourceType) {
        switch (pxrSourceType) {
            case pxr::UsdShadeAttributeType::Input: *sourceType = UsdShadeAttributeType::Input; break;
            case pxr::UsdShadeAttributeType::Output: *sourceType = UsdShadeAttributeType::Output; break;
            default: *sourceType = UsdShadeAttributeType::Invalid; break;
        }
    }
    return UsdShadeShader(usd::UsdPrim::FromImpl(new pxr::UsdPrim(shader.GetPrim())));
}

usd::UsdAttribute UsdShadeMaterial::GetDisplacementAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdShadeMaterial material(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(material.GetDisplacementAttr()));
}

usd::UsdAttribute UsdShadeMaterial::CreateDisplacementAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdShadeMaterial material(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(material.CreateDisplacementAttr()));
}

UsdShadeOutput UsdShadeMaterial::CreateDisplacementOutput(const tf::TfToken& renderContext) {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdShadeOutput();
    pxr::UsdShadeMaterial material(*asPrim(prim.GetImpl()));
    pxr::TfToken pxrContext = strlen(renderContext.GetText()) > 0 ? toPxrToken(renderContext) : pxr::TfToken();
    return UsdShadeOutput::FromImpl(new pxr::UsdShadeOutput(material.CreateDisplacementOutput(pxrContext)));
}

UsdShadeOutput UsdShadeMaterial::GetDisplacementOutput(const tf::TfToken& renderContext) const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdShadeOutput();
    pxr::UsdShadeMaterial material(*asPrim(prim.GetImpl()));
    pxr::TfToken pxrContext = strlen(renderContext.GetText()) > 0 ? toPxrToken(renderContext) : pxr::TfToken();
    return UsdShadeOutput::FromImpl(new pxr::UsdShadeOutput(material.GetDisplacementOutput(pxrContext)));
}

std::vector<UsdShadeOutput> UsdShadeMaterial::GetDisplacementOutputs() const {
    std::vector<UsdShadeOutput> result;
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return result;
    pxr::UsdShadeMaterial material(*asPrim(prim.GetImpl()));
    std::vector<pxr::UsdShadeOutput> pxrOutputs = material.GetDisplacementOutputs();
    result.reserve(pxrOutputs.size());
    for (const auto& out : pxrOutputs) {
        result.push_back(UsdShadeOutput::FromImpl(new pxr::UsdShadeOutput(out)));
    }
    return result;
}

UsdShadeShader UsdShadeMaterial::ComputeDisplacementSource(const tf::TfToken& renderContext,
                                                            tf::TfToken* sourceName,
                                                            UsdShadeAttributeType* sourceType) const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdShadeShader();
    pxr::UsdShadeMaterial material(*asPrim(prim.GetImpl()));
    pxr::TfToken pxrContext = strlen(renderContext.GetText()) > 0 ? toPxrToken(renderContext) : pxr::TfToken();
    pxr::TfToken pxrSourceName;
    pxr::UsdShadeAttributeType pxrSourceType;
    pxr::UsdShadeShader shader = material.ComputeDisplacementSource(pxrContext, &pxrSourceName, &pxrSourceType);
    if (sourceName) {
        *sourceName = tf::TfToken::FromImpl(new pxr::TfToken(pxrSourceName));
    }
    if (sourceType) {
        switch (pxrSourceType) {
            case pxr::UsdShadeAttributeType::Input: *sourceType = UsdShadeAttributeType::Input; break;
            case pxr::UsdShadeAttributeType::Output: *sourceType = UsdShadeAttributeType::Output; break;
            default: *sourceType = UsdShadeAttributeType::Invalid; break;
        }
    }
    return UsdShadeShader(usd::UsdPrim::FromImpl(new pxr::UsdPrim(shader.GetPrim())));
}

usd::UsdAttribute UsdShadeMaterial::GetVolumeAttr() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdShadeMaterial material(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(material.GetVolumeAttr()));
}

usd::UsdAttribute UsdShadeMaterial::CreateVolumeAttr() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdAttribute();
    pxr::UsdShadeMaterial material(*asPrim(prim.GetImpl()));
    return usd::UsdAttribute::FromImpl(new pxr::UsdAttribute(material.CreateVolumeAttr()));
}

UsdShadeOutput UsdShadeMaterial::CreateVolumeOutput(const tf::TfToken& renderContext) {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdShadeOutput();
    pxr::UsdShadeMaterial material(*asPrim(prim.GetImpl()));
    pxr::TfToken pxrContext = strlen(renderContext.GetText()) > 0 ? toPxrToken(renderContext) : pxr::TfToken();
    return UsdShadeOutput::FromImpl(new pxr::UsdShadeOutput(material.CreateVolumeOutput(pxrContext)));
}

UsdShadeOutput UsdShadeMaterial::GetVolumeOutput(const tf::TfToken& renderContext) const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdShadeOutput();
    pxr::UsdShadeMaterial material(*asPrim(prim.GetImpl()));
    pxr::TfToken pxrContext = strlen(renderContext.GetText()) > 0 ? toPxrToken(renderContext) : pxr::TfToken();
    return UsdShadeOutput::FromImpl(new pxr::UsdShadeOutput(material.GetVolumeOutput(pxrContext)));
}

std::vector<UsdShadeOutput> UsdShadeMaterial::GetVolumeOutputs() const {
    std::vector<UsdShadeOutput> result;
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return result;
    pxr::UsdShadeMaterial material(*asPrim(prim.GetImpl()));
    std::vector<pxr::UsdShadeOutput> pxrOutputs = material.GetVolumeOutputs();
    result.reserve(pxrOutputs.size());
    for (const auto& out : pxrOutputs) {
        result.push_back(UsdShadeOutput::FromImpl(new pxr::UsdShadeOutput(out)));
    }
    return result;
}

UsdShadeShader UsdShadeMaterial::ComputeVolumeSource(const tf::TfToken& renderContext,
                                                      tf::TfToken* sourceName,
                                                      UsdShadeAttributeType* sourceType) const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdShadeShader();
    pxr::UsdShadeMaterial material(*asPrim(prim.GetImpl()));
    pxr::TfToken pxrContext = strlen(renderContext.GetText()) > 0 ? toPxrToken(renderContext) : pxr::TfToken();
    pxr::TfToken pxrSourceName;
    pxr::UsdShadeAttributeType pxrSourceType;
    pxr::UsdShadeShader shader = material.ComputeVolumeSource(pxrContext, &pxrSourceName, &pxrSourceType);
    if (sourceName) {
        *sourceName = tf::TfToken::FromImpl(new pxr::TfToken(pxrSourceName));
    }
    if (sourceType) {
        switch (pxrSourceType) {
            case pxr::UsdShadeAttributeType::Input: *sourceType = UsdShadeAttributeType::Input; break;
            case pxr::UsdShadeAttributeType::Output: *sourceType = UsdShadeAttributeType::Output; break;
            default: *sourceType = UsdShadeAttributeType::Invalid; break;
        }
    }
    return UsdShadeShader(usd::UsdPrim::FromImpl(new pxr::UsdPrim(shader.GetPrim())));
}

UsdShadeMaterial UsdShadeMaterial::GetBaseMaterial() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdShadeMaterial();
    pxr::UsdShadeMaterial material(*asPrim(prim.GetImpl()));
    pxr::UsdShadeMaterial base = material.GetBaseMaterial();
    return UsdShadeMaterial(usd::UsdPrim::FromImpl(new pxr::UsdPrim(base.GetPrim())));
}

sdf::SdfPath UsdShadeMaterial::GetBaseMaterialPath() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return sdf::SdfPath();
    pxr::UsdShadeMaterial material(*asPrim(prim.GetImpl()));
    return sdf::SdfPath::FromImpl(new pxr::SdfPath(material.GetBaseMaterialPath()));
}

void UsdShadeMaterial::SetBaseMaterial(const UsdShadeMaterial& baseMaterial) {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return;
    pxr::UsdShadeMaterial material(*asPrim(prim.GetImpl()));
    usd::UsdPrim basePrim = baseMaterial.GetPrim();
    if (basePrim.IsValid()) {
        pxr::UsdShadeMaterial pxrBase(*asPrim(basePrim.GetImpl()));
        material.SetBaseMaterial(pxrBase);
    }
}

void UsdShadeMaterial::SetBaseMaterialPath(const sdf::SdfPath& baseMaterialPath) {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return;
    pxr::UsdShadeMaterial material(*asPrim(prim.GetImpl()));
    material.SetBaseMaterialPath(toPxrPath(baseMaterialPath));
}

void UsdShadeMaterial::ClearBaseMaterial() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return;
    pxr::UsdShadeMaterial material(*asPrim(prim.GetImpl()));
    material.ClearBaseMaterial();
}

bool UsdShadeMaterial::HasBaseMaterial() const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return false;
    pxr::UsdShadeMaterial material(*asPrim(prim.GetImpl()));
    return material.HasBaseMaterial();
}

usd::UsdSchemaKind UsdShadeMaterial::_GetSchemaKind() const {
    return usd::UsdSchemaKind::ConcreteTyped;
}

// ============================================================================
// UsdShadeMaterialBindingAPI implementation
// ============================================================================

UsdShadeMaterialBindingAPI::UsdShadeMaterialBindingAPI(const usd::UsdPrim& prim) : UsdAPISchemaBase(prim) {}

UsdShadeMaterialBindingAPI::UsdShadeMaterialBindingAPI(const usd::UsdSchemaBase& schemaObj) : UsdAPISchemaBase(schemaObj) {}

UsdShadeMaterialBindingAPI::~UsdShadeMaterialBindingAPI() {}

UsdShadeMaterialBindingAPI UsdShadeMaterialBindingAPI::Get(const usd::UsdStage& stage, const sdf::SdfPath& path) {
    return UsdShadeMaterialBindingAPI(stage.GetPrimAtPath(path));
}

UsdShadeMaterialBindingAPI UsdShadeMaterialBindingAPI::Apply(const usd::UsdPrim& prim) {
    if (!prim.IsValid()) return UsdShadeMaterialBindingAPI();
    pxr::UsdShadeMaterialBindingAPI api = pxr::UsdShadeMaterialBindingAPI::Apply(*asPrim(prim.GetImpl()));
    return UsdShadeMaterialBindingAPI(usd::UsdPrim::FromImpl(new pxr::UsdPrim(api.GetPrim())));
}

bool UsdShadeMaterialBindingAPI::CanApply(const usd::UsdPrim& prim, std::string* whyNot) {
    if (!prim.IsValid()) return false;
    return pxr::UsdShadeMaterialBindingAPI::CanApply(*asPrim(prim.GetImpl()), whyNot);
}

usd::UsdRelationship UsdShadeMaterialBindingAPI::GetDirectBindingRel(const tf::TfToken& materialPurpose) const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return usd::UsdRelationship();
    pxr::UsdShadeMaterialBindingAPI api(*asPrim(prim.GetImpl()));
    pxr::TfToken pxrPurpose = strlen(materialPurpose.GetText()) > 0 ? toPxrToken(materialPurpose) : pxr::UsdShadeTokens->allPurpose;
    return usd::UsdRelationship::FromImpl(new pxr::UsdRelationship(api.GetDirectBindingRel(pxrPurpose)));
}

bool UsdShadeMaterialBindingAPI::Bind(const UsdShadeMaterial& material,
                                       const tf::TfToken& bindingStrength,
                                       const tf::TfToken& materialPurpose) {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return false;
    usd::UsdPrim materialPrim = material.GetPrim();
    if (!materialPrim.IsValid()) return false;

    pxr::UsdShadeMaterialBindingAPI api(*asPrim(prim.GetImpl()));
    pxr::UsdShadeMaterial pxrMaterial(*asPrim(materialPrim.GetImpl()));
    pxr::TfToken pxrStrength = strlen(bindingStrength.GetText()) > 0 ? toPxrToken(bindingStrength) : pxr::UsdShadeTokens->fallbackStrength;
    pxr::TfToken pxrPurpose = strlen(materialPurpose.GetText()) > 0 ? toPxrToken(materialPurpose) : pxr::UsdShadeTokens->allPurpose;

    return api.Bind(pxrMaterial, pxrStrength, pxrPurpose);
}

bool UsdShadeMaterialBindingAPI::UnbindDirectBinding(const tf::TfToken& materialPurpose) {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return false;
    pxr::UsdShadeMaterialBindingAPI api(*asPrim(prim.GetImpl()));
    pxr::TfToken pxrPurpose = strlen(materialPurpose.GetText()) > 0 ? toPxrToken(materialPurpose) : pxr::UsdShadeTokens->allPurpose;
    return api.UnbindDirectBinding(pxrPurpose);
}

bool UsdShadeMaterialBindingAPI::UnbindAllBindings() {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return false;
    pxr::UsdShadeMaterialBindingAPI api(*asPrim(prim.GetImpl()));
    return api.UnbindAllBindings();
}

UsdShadeMaterial UsdShadeMaterialBindingAPI::ComputeBoundMaterial(const tf::TfToken& materialPurpose,
                                                                    usd::UsdRelationship* bindingRel) const {
    usd::UsdPrim prim = GetPrim();
    if (!prim.IsValid()) return UsdShadeMaterial();
    pxr::UsdShadeMaterialBindingAPI api(*asPrim(prim.GetImpl()));
    pxr::TfToken pxrPurpose = strlen(materialPurpose.GetText()) > 0 ? toPxrToken(materialPurpose) : pxr::UsdShadeTokens->allPurpose;

    pxr::UsdRelationship pxrBindingRel;
    pxr::UsdShadeMaterial pxrMaterial = api.ComputeBoundMaterial(pxrPurpose, &pxrBindingRel);

    if (bindingRel) {
        *bindingRel = usd::UsdRelationship::FromImpl(new pxr::UsdRelationship(pxrBindingRel));
    }

    return UsdShadeMaterial(usd::UsdPrim::FromImpl(new pxr::UsdPrim(pxrMaterial.GetPrim())));
}

tf::TfToken UsdShadeMaterialBindingAPI::GetMaterialBindingStrength(const usd::UsdRelationship& bindingRel) {
    if (!bindingRel.IsValid()) return tf::TfToken();
    return tf::TfToken::FromImpl(new pxr::TfToken(
        pxr::UsdShadeMaterialBindingAPI::GetMaterialBindingStrength(*asRel(bindingRel.GetImpl()))));
}

bool UsdShadeMaterialBindingAPI::SetMaterialBindingStrength(const usd::UsdRelationship& bindingRel,
                                                             const tf::TfToken& bindingStrength) {
    if (!bindingRel.IsValid()) return false;
    return pxr::UsdShadeMaterialBindingAPI::SetMaterialBindingStrength(
        *asRel(bindingRel.GetImpl()), toPxrToken(bindingStrength));
}

std::vector<tf::TfToken> UsdShadeMaterialBindingAPI::GetMaterialPurposes() {
    std::vector<tf::TfToken> result;
    pxr::TfTokenVector pxrPurposes = pxr::UsdShadeMaterialBindingAPI::GetMaterialPurposes();
    result.reserve(pxrPurposes.size());
    for (const auto& tok : pxrPurposes) {
        result.push_back(tf::TfToken::FromImpl(new pxr::TfToken(tok)));
    }
    return result;
}

sdf::SdfPath UsdShadeMaterialBindingAPI::GetResolvedTargetPathFromBindingRel(const usd::UsdRelationship& bindingRel) {
    if (!bindingRel.IsValid()) return sdf::SdfPath();
    return sdf::SdfPath::FromImpl(new pxr::SdfPath(
        pxr::UsdShadeMaterialBindingAPI::GetResolvedTargetPathFromBindingRel(*asRel(bindingRel.GetImpl()))));
}

usd::UsdSchemaKind UsdShadeMaterialBindingAPI::_GetSchemaKind() const {
    return usd::UsdSchemaKind::SingleApplyAPI;
}

// ============================================================================
// Token Constants
// ============================================================================

namespace UsdShadeTokens {

tf::TfToken id() {
    return tf::TfToken::FromImpl(new pxr::TfToken(pxr::UsdShadeTokens->id));
}

tf::TfToken sourceAsset() {
    return tf::TfToken::FromImpl(new pxr::TfToken(pxr::UsdShadeTokens->sourceAsset));
}

tf::TfToken sourceCode() {
    return tf::TfToken::FromImpl(new pxr::TfToken(pxr::UsdShadeTokens->sourceCode));
}

tf::TfToken universalSourceType() {
    return tf::TfToken::FromImpl(new pxr::TfToken(pxr::UsdShadeTokens->universalSourceType));
}

tf::TfToken universalRenderContext() {
    return tf::TfToken::FromImpl(new pxr::TfToken(pxr::UsdShadeTokens->universalRenderContext));
}

tf::TfToken full() {
    return tf::TfToken::FromImpl(new pxr::TfToken(pxr::UsdShadeTokens->full));
}

tf::TfToken interfaceOnly() {
    return tf::TfToken::FromImpl(new pxr::TfToken(pxr::UsdShadeTokens->interfaceOnly));
}

tf::TfToken allPurpose() {
    return tf::TfToken::FromImpl(new pxr::TfToken(pxr::UsdShadeTokens->allPurpose));
}

tf::TfToken preview() {
    return tf::TfToken::FromImpl(new pxr::TfToken(pxr::UsdShadeTokens->preview));
}

tf::TfToken fullPurpose() {
    return tf::TfToken::FromImpl(new pxr::TfToken(pxr::UsdShadeTokens->full));
}

tf::TfToken fallbackStrength() {
    return tf::TfToken::FromImpl(new pxr::TfToken(pxr::UsdShadeTokens->fallbackStrength));
}

tf::TfToken strongerThanDescendants() {
    return tf::TfToken::FromImpl(new pxr::TfToken(pxr::UsdShadeTokens->strongerThanDescendants));
}

tf::TfToken weakerThanDescendants() {
    return tf::TfToken::FromImpl(new pxr::TfToken(pxr::UsdShadeTokens->weakerThanDescendants));
}

tf::TfToken surface() {
    return tf::TfToken::FromImpl(new pxr::TfToken(pxr::UsdShadeTokens->surface));
}

tf::TfToken displacement() {
    return tf::TfToken::FromImpl(new pxr::TfToken(pxr::UsdShadeTokens->displacement));
}

tf::TfToken volume() {
    return tf::TfToken::FromImpl(new pxr::TfToken(pxr::UsdShadeTokens->volume));
}

tf::TfToken inputs() {
    return tf::TfToken::FromImpl(new pxr::TfToken(pxr::UsdShadeTokens->inputs));
}

tf::TfToken outputs() {
    return tf::TfToken::FromImpl(new pxr::TfToken(pxr::UsdShadeTokens->outputs));
}

} // namespace UsdShadeTokens

} // namespace usdShade
} // namespace cxxfacade
