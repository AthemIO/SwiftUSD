//
// Copyright 2025 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//

#include "Exec/swiftBridge.h"

// Include private Exec headers for registration
#include "Exec/definitionRegistry.h"
#include "Exec/inputKey.h"
#include "Exec/providerResolution.h"
#include "Exec/types.h"

// Include Vdf for context access
#include "Vdf/context.h"
#include "Vdf/executionTypeRegistry.h"

// Include other necessary headers
#include "Tf/type.h"
#include "Tf/token.h"
#include "Tf/errorMark.h"
#include "Sdf/path.h"

#include <memory>
#include <string>

PXR_NAMESPACE_OPEN_SCOPE

// ---------------------------------------------------------------------------
// Helper: Type name to TfType lookup
// ---------------------------------------------------------------------------

namespace {

// Maps common type names to TfType
TfType _LookupTypeByName(const char* typeName) {
    if (!typeName || typeName[0] == '\0') {
        return TfType();
    }

    const std::string name(typeName);

    // Try direct lookup first
    TfType type = TfType::FindByName(name);
    if (!type.IsUnknown()) {
        return type;
    }

    // Handle common aliases
    if (name == "double") {
        return TfType::Find<double>();
    }
    if (name == "float") {
        return TfType::Find<float>();
    }
    if (name == "int") {
        return TfType::Find<int>();
    }
    if (name == "bool") {
        return TfType::Find<bool>();
    }
    if (name == "GfMatrix4d" || name == "matrix4d") {
        return TfType::Find<GfMatrix4d>();
    }
    if (name == "GfMatrix4f" || name == "matrix4f") {
        return TfType::Find<GfMatrix4f>();
    }
    if (name == "GfVec2d" || name == "vec2d") {
        return TfType::Find<GfVec2d>();
    }
    if (name == "GfVec2f" || name == "vec2f") {
        return TfType::Find<GfVec2f>();
    }
    if (name == "GfVec3d" || name == "vec3d") {
        return TfType::Find<GfVec3d>();
    }
    if (name == "GfVec3f" || name == "vec3f") {
        return TfType::Find<GfVec3f>();
    }
    if (name == "GfVec4d" || name == "vec4d") {
        return TfType::Find<GfVec4d>();
    }
    if (name == "GfVec4f" || name == "vec4f") {
        return TfType::Find<GfVec4f>();
    }
    if (name == "GfQuatd" || name == "quatd") {
        return TfType::Find<GfQuatd>();
    }
    if (name == "GfQuatf" || name == "quatf") {
        return TfType::Find<GfQuatf>();
    }

    return TfType();
}

// Converts Exec_Swift_DynamicTraversal to ExecProviderResolution::DynamicTraversal
ExecProviderResolution::DynamicTraversal _ConvertDynamicTraversal(
    Exec_Swift_DynamicTraversal traversal)
{
    switch (traversal) {
        case Exec_Swift_DynamicTraversal_Local:
            return ExecProviderResolution::DynamicTraversal::Local;
        case Exec_Swift_DynamicTraversal_RelationshipTargetedObjects:
            return ExecProviderResolution::DynamicTraversal::RelationshipTargetedObjects;
        case Exec_Swift_DynamicTraversal_ConnectionTargetedObjects:
            return ExecProviderResolution::DynamicTraversal::ConnectionTargetedObjects;
        case Exec_Swift_DynamicTraversal_NamespaceAncestor:
            return ExecProviderResolution::DynamicTraversal::NamespaceAncestor;
        default:
            return ExecProviderResolution::DynamicTraversal::Local;
    }
}

// Structure to hold Swift callback and context
struct _SwiftCallbackContext {
    Exec_Swift_ComputationCallback callback;
    void* userContext;
};

} // anonymous namespace

// ---------------------------------------------------------------------------
// Computation Registration
// ---------------------------------------------------------------------------

bool
Exec_Swift_RegisterComputation(const Exec_Swift_ComputationSpec* spec)
{
    if (!spec) {
        TF_CODING_ERROR("Null computation spec");
        return false;
    }

    if (!spec->schemaTypeName || !spec->computationName || !spec->resultTypeName) {
        TF_CODING_ERROR("Missing required fields in computation spec");
        return false;
    }

    if (!spec->callback) {
        TF_CODING_ERROR("Null callback in computation spec");
        return false;
    }

    // 1. Look up schema TfType
    TfType schemaType = TfType::FindByName(spec->schemaTypeName);
    if (schemaType.IsUnknown()) {
        TF_CODING_ERROR("Unknown schema type: %s", spec->schemaTypeName);
        return false;
    }

    // 2. Look up result TfType
    TfType resultType = _LookupTypeByName(spec->resultTypeName);
    if (resultType.IsUnknown()) {
        TF_CODING_ERROR("Unknown result type: %s", spec->resultTypeName);
        return false;
    }

    // 3. Build input keys
    auto inputKeys = Exec_InputKeyVector::MakeShared();
    for (size_t i = 0; i < spec->inputCount; ++i) {
        const Exec_Swift_InputSpec& input = spec->inputs[i];

        if (!input.inputName || !input.computationName || !input.resultTypeName) {
            TF_CODING_ERROR("Missing required fields in input spec %zu", i);
            return false;
        }

        TfType inputResultType = _LookupTypeByName(input.resultTypeName);
        if (inputResultType.IsUnknown()) {
            TF_CODING_ERROR("Unknown input result type: %s", input.resultTypeName);
            return false;
        }

        Exec_InputKey key;
        key.inputName = TfToken(input.inputName);
        key.computationName = TfToken(input.computationName);
        key.disambiguatingId = input.disambiguatingId ? TfToken(input.disambiguatingId) : TfToken();
        key.resultType = inputResultType;
        key.providerResolution.localTraversal =
            input.localTraversalPath ? SdfPath(input.localTraversalPath) : SdfPath();
        key.providerResolution.dynamicTraversal =
            _ConvertDynamicTraversal(input.dynamicTraversal);
        key.fallsBackToDispatched = input.fallsBackToDispatched;
        key.optional = input.optional;

        inputKeys->Get().push_back(std::move(key));
    }

    // 4. Create the callback wrapper
    // Store Swift callback and context in a shared_ptr to ensure lifetime
    auto swiftContext = std::make_shared<_SwiftCallbackContext>();
    swiftContext->callback = spec->callback;
    swiftContext->userContext = spec->callbackContext;

    // Create the ExecCallbackFn that wraps the Swift callback
    ExecCallbackFn callback = [swiftContext](const VdfContext& ctx) {
        // Pass VdfContext as opaque pointer to Swift
        // We cast away const because the context handle is used for both
        // reading and writing through separate functions
        swiftContext->callback(
            const_cast<VdfContext*>(&ctx),
            swiftContext->userContext);
    };

    // 5. Register with definition registry
    Exec_DefinitionRegistry& registry =
        Exec_DefinitionRegistry::RegistrationAccess::_GetInstanceForRegistration();

    TfErrorMark mark;

    if (spec->isPrimComputation) {
        registry.RegisterPrimComputation(
            schemaType,
            TfToken(spec->computationName),
            resultType,
            std::move(callback),
            std::move(inputKeys),
            nullptr);  // No dispatch schemas for now
    } else {
        if (!spec->attributeName) {
            TF_CODING_ERROR("Attribute computation requires attributeName");
            return false;
        }
        registry.RegisterAttributeComputation(
            TfToken(spec->attributeName),
            schemaType,
            TfToken(spec->computationName),
            resultType,
            std::move(callback),
            std::move(inputKeys),
            nullptr);  // No dispatch schemas for now
    }

    // Mark registration complete for the schema type
    registry.SetComputationRegistrationComplete(schemaType);

    return mark.IsClean();
}

// ---------------------------------------------------------------------------
// Context Access: Has Input
// ---------------------------------------------------------------------------

bool
Exec_Swift_Context_HasInputValue(
    Exec_Swift_ContextHandle context,
    const char* inputName)
{
    if (!context || !inputName) {
        return false;
    }

    const VdfContext* ctx = static_cast<const VdfContext*>(context);
    TfToken name(inputName);

    // We need to check for various types since HasInputValue is templated
    // Try common types - this is a limitation of the bridge
    if (ctx->HasInputValue<double>(name)) return true;
    if (ctx->HasInputValue<float>(name)) return true;
    if (ctx->HasInputValue<int>(name)) return true;
    if (ctx->HasInputValue<bool>(name)) return true;
    if (ctx->HasInputValue<GfMatrix4d>(name)) return true;
    if (ctx->HasInputValue<GfMatrix4f>(name)) return true;
    if (ctx->HasInputValue<GfVec2d>(name)) return true;
    if (ctx->HasInputValue<GfVec2f>(name)) return true;
    if (ctx->HasInputValue<GfVec3d>(name)) return true;
    if (ctx->HasInputValue<GfVec3f>(name)) return true;
    if (ctx->HasInputValue<GfVec4d>(name)) return true;
    if (ctx->HasInputValue<GfVec4f>(name)) return true;
    if (ctx->HasInputValue<GfQuatd>(name)) return true;
    if (ctx->HasInputValue<GfQuatf>(name)) return true;

    return false;
}

// ---------------------------------------------------------------------------
// Context Access: Get Input (VtValue)
// ---------------------------------------------------------------------------

VtValue
Exec_Swift_Context_GetInputValue(
    Exec_Swift_ContextHandle context,
    const char* inputName)
{
    if (!context || !inputName) {
        return VtValue();
    }

    const VdfContext* ctx = static_cast<const VdfContext*>(context);
    TfToken name(inputName);

    // Try each type and wrap in VtValue if found
    // This is inefficient but necessary without runtime type info
    if (const double* v = ctx->GetInputValuePtr<double>(name)) {
        return VtValue(*v);
    }
    if (const float* v = ctx->GetInputValuePtr<float>(name)) {
        return VtValue(*v);
    }
    if (const int* v = ctx->GetInputValuePtr<int>(name)) {
        return VtValue(*v);
    }
    if (const bool* v = ctx->GetInputValuePtr<bool>(name)) {
        return VtValue(*v);
    }
    if (const GfMatrix4d* v = ctx->GetInputValuePtr<GfMatrix4d>(name)) {
        return VtValue(*v);
    }
    if (const GfMatrix4f* v = ctx->GetInputValuePtr<GfMatrix4f>(name)) {
        return VtValue(*v);
    }
    if (const GfVec2d* v = ctx->GetInputValuePtr<GfVec2d>(name)) {
        return VtValue(*v);
    }
    if (const GfVec2f* v = ctx->GetInputValuePtr<GfVec2f>(name)) {
        return VtValue(*v);
    }
    if (const GfVec3d* v = ctx->GetInputValuePtr<GfVec3d>(name)) {
        return VtValue(*v);
    }
    if (const GfVec3f* v = ctx->GetInputValuePtr<GfVec3f>(name)) {
        return VtValue(*v);
    }
    if (const GfVec4d* v = ctx->GetInputValuePtr<GfVec4d>(name)) {
        return VtValue(*v);
    }
    if (const GfVec4f* v = ctx->GetInputValuePtr<GfVec4f>(name)) {
        return VtValue(*v);
    }
    if (const GfQuatd* v = ctx->GetInputValuePtr<GfQuatd>(name)) {
        return VtValue(*v);
    }
    if (const GfQuatf* v = ctx->GetInputValuePtr<GfQuatf>(name)) {
        return VtValue(*v);
    }

    return VtValue();
}

// ---------------------------------------------------------------------------
// Context Access: Set Output (VtValue)
// ---------------------------------------------------------------------------

void
Exec_Swift_Context_SetOutput(
    Exec_Swift_ContextHandle context,
    const VtValue& value)
{
    if (!context || value.IsEmpty()) {
        return;
    }

    VdfContext* ctx = static_cast<VdfContext*>(context);

    // Dispatch based on held type
    if (value.IsHolding<double>()) {
        ctx->SetOutput(value.UncheckedGet<double>());
    } else if (value.IsHolding<float>()) {
        ctx->SetOutput(value.UncheckedGet<float>());
    } else if (value.IsHolding<int>()) {
        ctx->SetOutput(value.UncheckedGet<int>());
    } else if (value.IsHolding<bool>()) {
        ctx->SetOutput(value.UncheckedGet<bool>());
    } else if (value.IsHolding<GfMatrix4d>()) {
        ctx->SetOutput(value.UncheckedGet<GfMatrix4d>());
    } else if (value.IsHolding<GfMatrix4f>()) {
        ctx->SetOutput(value.UncheckedGet<GfMatrix4f>());
    } else if (value.IsHolding<GfVec2d>()) {
        ctx->SetOutput(value.UncheckedGet<GfVec2d>());
    } else if (value.IsHolding<GfVec2f>()) {
        ctx->SetOutput(value.UncheckedGet<GfVec2f>());
    } else if (value.IsHolding<GfVec3d>()) {
        ctx->SetOutput(value.UncheckedGet<GfVec3d>());
    } else if (value.IsHolding<GfVec3f>()) {
        ctx->SetOutput(value.UncheckedGet<GfVec3f>());
    } else if (value.IsHolding<GfVec4d>()) {
        ctx->SetOutput(value.UncheckedGet<GfVec4d>());
    } else if (value.IsHolding<GfVec4f>()) {
        ctx->SetOutput(value.UncheckedGet<GfVec4f>());
    } else if (value.IsHolding<GfQuatd>()) {
        ctx->SetOutput(value.UncheckedGet<GfQuatd>());
    } else if (value.IsHolding<GfQuatf>()) {
        ctx->SetOutput(value.UncheckedGet<GfQuatf>());
    } else {
        TF_CODING_ERROR("Unsupported output type: %s", value.GetTypeName().c_str());
    }
}

// ---------------------------------------------------------------------------
// Context Access: Typed Input Getters
// ---------------------------------------------------------------------------

double
Exec_Swift_Context_GetInputDouble(
    Exec_Swift_ContextHandle context,
    const char* inputName)
{
    if (!context || !inputName) {
        return 0.0;
    }
    const VdfContext* ctx = static_cast<const VdfContext*>(context);
    const double* v = ctx->GetInputValuePtr<double>(TfToken(inputName));
    return v ? *v : 0.0;
}

float
Exec_Swift_Context_GetInputFloat(
    Exec_Swift_ContextHandle context,
    const char* inputName)
{
    if (!context || !inputName) {
        return 0.0f;
    }
    const VdfContext* ctx = static_cast<const VdfContext*>(context);
    const float* v = ctx->GetInputValuePtr<float>(TfToken(inputName));
    return v ? *v : 0.0f;
}

int
Exec_Swift_Context_GetInputInt(
    Exec_Swift_ContextHandle context,
    const char* inputName)
{
    if (!context || !inputName) {
        return 0;
    }
    const VdfContext* ctx = static_cast<const VdfContext*>(context);
    const int* v = ctx->GetInputValuePtr<int>(TfToken(inputName));
    return v ? *v : 0;
}

bool
Exec_Swift_Context_GetInputBool(
    Exec_Swift_ContextHandle context,
    const char* inputName)
{
    if (!context || !inputName) {
        return false;
    }
    const VdfContext* ctx = static_cast<const VdfContext*>(context);
    const bool* v = ctx->GetInputValuePtr<bool>(TfToken(inputName));
    return v ? *v : false;
}

GfMatrix4d
Exec_Swift_Context_GetInputMatrix4d(
    Exec_Swift_ContextHandle context,
    const char* inputName)
{
    if (!context || !inputName) {
        return GfMatrix4d(1.0);
    }
    const VdfContext* ctx = static_cast<const VdfContext*>(context);
    const GfMatrix4d* v = ctx->GetInputValuePtr<GfMatrix4d>(TfToken(inputName));
    return v ? *v : GfMatrix4d(1.0);
}

GfMatrix4f
Exec_Swift_Context_GetInputMatrix4f(
    Exec_Swift_ContextHandle context,
    const char* inputName)
{
    if (!context || !inputName) {
        return GfMatrix4f(1.0f);
    }
    const VdfContext* ctx = static_cast<const VdfContext*>(context);
    const GfMatrix4f* v = ctx->GetInputValuePtr<GfMatrix4f>(TfToken(inputName));
    return v ? *v : GfMatrix4f(1.0f);
}

GfVec2d
Exec_Swift_Context_GetInputVec2d(
    Exec_Swift_ContextHandle context,
    const char* inputName)
{
    if (!context || !inputName) {
        return GfVec2d(0.0);
    }
    const VdfContext* ctx = static_cast<const VdfContext*>(context);
    const GfVec2d* v = ctx->GetInputValuePtr<GfVec2d>(TfToken(inputName));
    return v ? *v : GfVec2d(0.0);
}

GfVec2f
Exec_Swift_Context_GetInputVec2f(
    Exec_Swift_ContextHandle context,
    const char* inputName)
{
    if (!context || !inputName) {
        return GfVec2f(0.0f);
    }
    const VdfContext* ctx = static_cast<const VdfContext*>(context);
    const GfVec2f* v = ctx->GetInputValuePtr<GfVec2f>(TfToken(inputName));
    return v ? *v : GfVec2f(0.0f);
}

GfVec3d
Exec_Swift_Context_GetInputVec3d(
    Exec_Swift_ContextHandle context,
    const char* inputName)
{
    if (!context || !inputName) {
        return GfVec3d(0.0);
    }
    const VdfContext* ctx = static_cast<const VdfContext*>(context);
    const GfVec3d* v = ctx->GetInputValuePtr<GfVec3d>(TfToken(inputName));
    return v ? *v : GfVec3d(0.0);
}

GfVec3f
Exec_Swift_Context_GetInputVec3f(
    Exec_Swift_ContextHandle context,
    const char* inputName)
{
    if (!context || !inputName) {
        return GfVec3f(0.0f);
    }
    const VdfContext* ctx = static_cast<const VdfContext*>(context);
    const GfVec3f* v = ctx->GetInputValuePtr<GfVec3f>(TfToken(inputName));
    return v ? *v : GfVec3f(0.0f);
}

GfVec4d
Exec_Swift_Context_GetInputVec4d(
    Exec_Swift_ContextHandle context,
    const char* inputName)
{
    if (!context || !inputName) {
        return GfVec4d(0.0);
    }
    const VdfContext* ctx = static_cast<const VdfContext*>(context);
    const GfVec4d* v = ctx->GetInputValuePtr<GfVec4d>(TfToken(inputName));
    return v ? *v : GfVec4d(0.0);
}

GfVec4f
Exec_Swift_Context_GetInputVec4f(
    Exec_Swift_ContextHandle context,
    const char* inputName)
{
    if (!context || !inputName) {
        return GfVec4f(0.0f);
    }
    const VdfContext* ctx = static_cast<const VdfContext*>(context);
    const GfVec4f* v = ctx->GetInputValuePtr<GfVec4f>(TfToken(inputName));
    return v ? *v : GfVec4f(0.0f);
}

GfQuatd
Exec_Swift_Context_GetInputQuatd(
    Exec_Swift_ContextHandle context,
    const char* inputName)
{
    if (!context || !inputName) {
        return GfQuatd::GetIdentity();
    }
    const VdfContext* ctx = static_cast<const VdfContext*>(context);
    const GfQuatd* v = ctx->GetInputValuePtr<GfQuatd>(TfToken(inputName));
    return v ? *v : GfQuatd::GetIdentity();
}

GfQuatf
Exec_Swift_Context_GetInputQuatf(
    Exec_Swift_ContextHandle context,
    const char* inputName)
{
    if (!context || !inputName) {
        return GfQuatf::GetIdentity();
    }
    const VdfContext* ctx = static_cast<const VdfContext*>(context);
    const GfQuatf* v = ctx->GetInputValuePtr<GfQuatf>(TfToken(inputName));
    return v ? *v : GfQuatf::GetIdentity();
}

// ---------------------------------------------------------------------------
// Context Access: Typed Output Setters
// ---------------------------------------------------------------------------

void
Exec_Swift_Context_SetOutputDouble(
    Exec_Swift_ContextHandle context,
    double value)
{
    if (!context) return;
    static_cast<VdfContext*>(context)->SetOutput(value);
}

void
Exec_Swift_Context_SetOutputFloat(
    Exec_Swift_ContextHandle context,
    float value)
{
    if (!context) return;
    static_cast<VdfContext*>(context)->SetOutput(value);
}

void
Exec_Swift_Context_SetOutputInt(
    Exec_Swift_ContextHandle context,
    int value)
{
    if (!context) return;
    static_cast<VdfContext*>(context)->SetOutput(value);
}

void
Exec_Swift_Context_SetOutputBool(
    Exec_Swift_ContextHandle context,
    bool value)
{
    if (!context) return;
    static_cast<VdfContext*>(context)->SetOutput(value);
}

void
Exec_Swift_Context_SetOutputMatrix4d(
    Exec_Swift_ContextHandle context,
    const GfMatrix4d& value)
{
    if (!context) return;
    static_cast<VdfContext*>(context)->SetOutput(value);
}

void
Exec_Swift_Context_SetOutputMatrix4f(
    Exec_Swift_ContextHandle context,
    const GfMatrix4f& value)
{
    if (!context) return;
    static_cast<VdfContext*>(context)->SetOutput(value);
}

void
Exec_Swift_Context_SetOutputVec2d(
    Exec_Swift_ContextHandle context,
    const GfVec2d& value)
{
    if (!context) return;
    static_cast<VdfContext*>(context)->SetOutput(value);
}

void
Exec_Swift_Context_SetOutputVec2f(
    Exec_Swift_ContextHandle context,
    const GfVec2f& value)
{
    if (!context) return;
    static_cast<VdfContext*>(context)->SetOutput(value);
}

void
Exec_Swift_Context_SetOutputVec3d(
    Exec_Swift_ContextHandle context,
    const GfVec3d& value)
{
    if (!context) return;
    static_cast<VdfContext*>(context)->SetOutput(value);
}

void
Exec_Swift_Context_SetOutputVec3f(
    Exec_Swift_ContextHandle context,
    const GfVec3f& value)
{
    if (!context) return;
    static_cast<VdfContext*>(context)->SetOutput(value);
}

void
Exec_Swift_Context_SetOutputVec4d(
    Exec_Swift_ContextHandle context,
    const GfVec4d& value)
{
    if (!context) return;
    static_cast<VdfContext*>(context)->SetOutput(value);
}

void
Exec_Swift_Context_SetOutputVec4f(
    Exec_Swift_ContextHandle context,
    const GfVec4f& value)
{
    if (!context) return;
    static_cast<VdfContext*>(context)->SetOutput(value);
}

void
Exec_Swift_Context_SetOutputQuatd(
    Exec_Swift_ContextHandle context,
    const GfQuatd& value)
{
    if (!context) return;
    static_cast<VdfContext*>(context)->SetOutput(value);
}

void
Exec_Swift_Context_SetOutputQuatf(
    Exec_Swift_ContextHandle context,
    const GfQuatf& value)
{
    if (!context) return;
    static_cast<VdfContext*>(context)->SetOutput(value);
}

PXR_NAMESPACE_CLOSE_SCOPE
