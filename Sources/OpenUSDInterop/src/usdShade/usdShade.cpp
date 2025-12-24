// usdShade.cpp - UsdShade module C wrapper implementation
// Mirrors: pxr/usd/usdShade/*.h

#include "../../include/usdShade/usdShade.h"

// Check if we should use full USD library
#if defined(USE_PIXAR_USD) && USE_PIXAR_USD
    #define USD_USE_FULL 1
    #include <pxr/usd/usdShade/material.h>
    #include <pxr/usd/usdShade/shader.h>
    #include <pxr/usd/usdShade/nodeGraph.h>
    #include <pxr/usd/usdShade/input.h>
    #include <pxr/usd/usdShade/output.h>
    #include <pxr/usd/usdShade/connectableAPI.h>
    #include <pxr/usd/usdShade/materialBindingAPI.h>
    #include <pxr/usd/usdShade/utils.h>
    #include <pxr/usd/usd/stage.h>
    #include <pxr/usd/usd/prim.h>
    #include <pxr/usd/usd/attribute.h>
    #include <pxr/usd/sdf/path.h>
    #include <pxr/usd/sdf/assetPath.h>
    #include <pxr/base/tf/token.h>
    #include <pxr/base/tf/stringUtils.h>
    #include <pxr/base/vt/value.h>
    #include <pxr/base/gf/vec3f.h>
    PXR_NAMESPACE_USING_DIRECTIVE
#else
    #define USD_USE_FULL 0
#endif

#include <atomic>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>

// Forward declare opaque types from other modules so we can use their members
// These must match the definitions in their respective source files

struct TfTokenOpaque {
#if USD_USE_FULL
    TfToken token;
#else
    std::string text;
    size_t hash;
#endif
    std::atomic<int> refCount;
};

struct SdfPathOpaque {
#if USD_USE_FULL
    SdfPath path;
#else
    std::string pathString;
#endif
    std::atomic<int> refCount;
};

struct SdfAssetPathOpaque {
#if USD_USE_FULL
    SdfAssetPath assetPath;
#else
    std::string path;
    std::string resolvedPath;
#endif
    std::atomic<int> refCount;
};

struct UsdStageOpaque {
#if USD_USE_FULL
    UsdStageRefPtr stage;
#else
    std::string identifier;
    double startTimeCode;
    double endTimeCode;
    double timeCodesPerSecond;
    double framesPerSecond;
#endif
    std::atomic<int> refCount;
};

struct UsdPrimOpaque {
#if USD_USE_FULL
    UsdPrim prim;
#else
    std::string path;
    std::string name;
    std::string typeName;
    bool isActive;
    bool isDefined;
#endif
    std::atomic<int> refCount;
};

struct UsdAttributeOpaque {
#if USD_USE_FULL
    UsdAttribute attr;
#else
    std::string name;
    std::string typeName;
    std::string path;
#endif
    std::atomic<int> refCount;
};

// ============================================================================
// MARK: - Wrapper Structures
// ============================================================================

struct UsdShadeMaterialOpaque {
#if USD_USE_FULL
    UsdShadeMaterial material;
#else
    std::string path;
#endif
    std::atomic<int> refCount;

    UsdShadeMaterialOpaque() : refCount(1) {}

#if USD_USE_FULL
    explicit UsdShadeMaterialOpaque(const UsdShadeMaterial& m)
        : material(m), refCount(1) {}
#endif
};

struct UsdShadeShaderOpaque {
#if USD_USE_FULL
    UsdShadeShader shader;
#else
    std::string path;
    std::string shaderId;
#endif
    std::atomic<int> refCount;

    UsdShadeShaderOpaque() : refCount(1) {}

#if USD_USE_FULL
    explicit UsdShadeShaderOpaque(const UsdShadeShader& s)
        : shader(s), refCount(1) {}
#endif
};

struct UsdShadeNodeGraphOpaque {
#if USD_USE_FULL
    UsdShadeNodeGraph nodeGraph;
#else
    std::string path;
#endif
    std::atomic<int> refCount;

    UsdShadeNodeGraphOpaque() : refCount(1) {}

#if USD_USE_FULL
    explicit UsdShadeNodeGraphOpaque(const UsdShadeNodeGraph& ng)
        : nodeGraph(ng), refCount(1) {}
#endif
};

struct UsdShadeInputOpaque {
#if USD_USE_FULL
    UsdShadeInput input;
#else
    std::string name;
    std::string typeName;
#endif
    std::atomic<int> refCount;

    UsdShadeInputOpaque() : refCount(1) {}

#if USD_USE_FULL
    explicit UsdShadeInputOpaque(const UsdShadeInput& i)
        : input(i), refCount(1) {}
#endif
};

struct UsdShadeOutputOpaque {
#if USD_USE_FULL
    UsdShadeOutput output;
#else
    std::string name;
    std::string typeName;
#endif
    std::atomic<int> refCount;

    UsdShadeOutputOpaque() : refCount(1) {}

#if USD_USE_FULL
    explicit UsdShadeOutputOpaque(const UsdShadeOutput& o)
        : output(o), refCount(1) {}
#endif
};

struct UsdShadeConnectableAPIOpaque {
#if USD_USE_FULL
    UsdShadeConnectableAPI api;
#else
    std::string path;
#endif
    std::atomic<int> refCount;

    UsdShadeConnectableAPIOpaque() : refCount(1) {}

#if USD_USE_FULL
    explicit UsdShadeConnectableAPIOpaque(const UsdShadeConnectableAPI& a)
        : api(a), refCount(1) {}
#endif
};

struct UsdShadeMaterialBindingAPIOpaque {
#if USD_USE_FULL
    UsdShadeMaterialBindingAPI api;
#else
    std::string path;
#endif
    std::atomic<int> refCount;

    UsdShadeMaterialBindingAPIOpaque() : refCount(1) {}

#if USD_USE_FULL
    explicit UsdShadeMaterialBindingAPIOpaque(const UsdShadeMaterialBindingAPI& a)
        : api(a), refCount(1) {}
#endif
};

// ============================================================================
// MARK: - Helper Macros
// ============================================================================

#define CATCH_AND_RETURN(defaultValue) \
    catch (...) { return defaultValue; }

#define CATCH_AND_RETURN_RESULT \
    catch (...) { return USD_RESULT_ERROR; }

// ============================================================================
// MARK: - Helper Functions
// ============================================================================

#if USD_USE_FULL
static TfToken GetMaterialPurposeToken(UsdShadeMaterialPurpose purpose) {
    switch (purpose) {
        case USDSHADE_PURPOSE_ALL:
            return UsdShadeTokens->allPurpose;
        case USDSHADE_PURPOSE_PREVIEW:
            return UsdShadeTokens->preview;
        case USDSHADE_PURPOSE_FULL:
            return UsdShadeTokens->full;
        default:
            return UsdShadeTokens->allPurpose;
    }
}

static UsdShadeConnectionModification GetConnectionMod(UsdShadeConnectionModification mod) {
    switch (mod) {
        case USDSHADE_CONNECTION_REPLACE:
            return UsdShadeConnectionModification::Replace;
        case USDSHADE_CONNECTION_PREPEND:
            return UsdShadeConnectionModification::Prepend;
        case USDSHADE_CONNECTION_APPEND:
            return UsdShadeConnectionModification::Append;
        default:
            return UsdShadeConnectionModification::Replace;
    }
}

static UsdShadeAttributeType GetAttrType(UsdShadeAttributeType type) {
    return type;
}

static UsdTimeCode ToTimeCode(UsdTimeCode tc) {
    if (tc.isDefault) {
        return UsdTimeCode::Default();
    }
    return UsdTimeCode(tc.time);
}
#endif

// ============================================================================
// MARK: - UsdShadeMaterial Implementation
// ============================================================================

extern "C" {

UsdShadeMaterialRef UsdShadeMaterial_Get(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        if (stage->stage) {
            UsdShadeMaterial material = UsdShadeMaterial::Get(stage->stage, path->path);
            if (material) {
                return new UsdShadeMaterialOpaque(material);
            }
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdShadeMaterialRef UsdShadeMaterial_Define(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        if (stage->stage) {
            UsdShadeMaterial material = UsdShadeMaterial::Define(stage->stage, path->path);
            if (material) {
                return new UsdShadeMaterialOpaque(material);
            }
        }
        return nullptr;
#else
        auto* wrapper = new UsdShadeMaterialOpaque();
        wrapper->path = SdfPath_GetString(path);
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdShadeMaterialRef UsdShadeMaterial_FromPrim(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdShadeMaterial material(prim->prim);
        if (material) {
            return new UsdShadeMaterialOpaque(material);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdShadeMaterialRef UsdShadeMaterial_Retain(UsdShadeMaterialRef material) {
    if (!material) return nullptr;
    material->refCount.fetch_add(1, std::memory_order_relaxed);
    return material;
}

void UsdShadeMaterial_Release(UsdShadeMaterialRef material) {
    if (!material) return;
    if (material->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete material;
    }
}

bool UsdShadeMaterial_IsValid(UsdShadeMaterialRef material) {
    if (!material) return false;
    try {
#if USD_USE_FULL
        return material->material.GetPrim().IsValid();
#else
        return !material->path.empty();
#endif
    } CATCH_AND_RETURN(false)
}

UsdPrimRef UsdShadeMaterial_GetPrim(UsdShadeMaterialRef material) {
    if (!material) return nullptr;
    try {
#if USD_USE_FULL
        UsdPrim prim = material->material.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

SdfPathRef UsdShadeMaterial_GetPath(UsdShadeMaterialRef material) {
    if (!material) return nullptr;
    try {
#if USD_USE_FULL
        SdfPath path = material->material.GetPath();
        return SdfPath_Create(path.GetString().c_str());
#else
        return SdfPath_Create(material->path.c_str());
#endif
    } CATCH_AND_RETURN(nullptr)
}

// MARK: - Material Surface Output

UsdShadeOutputRef UsdShadeMaterial_CreateSurfaceOutput(
    UsdShadeMaterialRef material,
    TfTokenRef renderContext
) {
    if (!material) return nullptr;
    try {
#if USD_USE_FULL
        TfToken context = renderContext ? renderContext->token : TfToken();
        UsdShadeOutput output = material->material.CreateSurfaceOutput(context);
        if (output) {
            return new UsdShadeOutputOpaque(output);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdShadeOutputRef UsdShadeMaterial_GetSurfaceOutput(
    UsdShadeMaterialRef material,
    TfTokenRef renderContext
) {
    if (!material) return nullptr;
    try {
#if USD_USE_FULL
        TfToken context = renderContext ? renderContext->token : TfToken();
        UsdShadeOutput output = material->material.GetSurfaceOutput(context);
        if (output) {
            return new UsdShadeOutputOpaque(output);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdShadeShaderRef UsdShadeMaterial_ComputeSurfaceSource(
    UsdShadeMaterialRef material,
    TfTokenRef renderContext,
    TfTokenRef* outSourceName,
    UsdShadeAttributeType* outSourceType
) {
    if (!material) return nullptr;
    try {
#if USD_USE_FULL
        TfToken context = renderContext ? renderContext->token : TfToken();
        TfToken sourceName;
        UsdShadeAttributeType sourceType;
        UsdShadeShader shader = material->material.ComputeSurfaceSource({context}, &sourceName, &sourceType);
        if (shader) {
            if (outSourceName) {
                *outSourceName = TfToken_Create(sourceName.GetString().c_str());
            }
            if (outSourceType) {
                *outSourceType = (sourceType == UsdShadeAttributeType::Input) ?
                    USDSHADE_ATTR_TYPE_INPUT : USDSHADE_ATTR_TYPE_OUTPUT;
            }
            return new UsdShadeShaderOpaque(shader);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

// MARK: - Material Displacement Output

UsdShadeOutputRef UsdShadeMaterial_CreateDisplacementOutput(
    UsdShadeMaterialRef material,
    TfTokenRef renderContext
) {
    if (!material) return nullptr;
    try {
#if USD_USE_FULL
        TfToken context = renderContext ? renderContext->token : TfToken();
        UsdShadeOutput output = material->material.CreateDisplacementOutput(context);
        if (output) {
            return new UsdShadeOutputOpaque(output);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdShadeOutputRef UsdShadeMaterial_GetDisplacementOutput(
    UsdShadeMaterialRef material,
    TfTokenRef renderContext
) {
    if (!material) return nullptr;
    try {
#if USD_USE_FULL
        TfToken context = renderContext ? renderContext->token : TfToken();
        UsdShadeOutput output = material->material.GetDisplacementOutput(context);
        if (output) {
            return new UsdShadeOutputOpaque(output);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdShadeShaderRef UsdShadeMaterial_ComputeDisplacementSource(
    UsdShadeMaterialRef material,
    TfTokenRef renderContext,
    TfTokenRef* outSourceName,
    UsdShadeAttributeType* outSourceType
) {
    if (!material) return nullptr;
    try {
#if USD_USE_FULL
        TfToken context = renderContext ? renderContext->token : TfToken();
        TfToken sourceName;
        UsdShadeAttributeType sourceType;
        UsdShadeShader shader = material->material.ComputeDisplacementSource({context}, &sourceName, &sourceType);
        if (shader) {
            if (outSourceName) {
                *outSourceName = TfToken_Create(sourceName.GetString().c_str());
            }
            if (outSourceType) {
                *outSourceType = (sourceType == UsdShadeAttributeType::Input) ?
                    USDSHADE_ATTR_TYPE_INPUT : USDSHADE_ATTR_TYPE_OUTPUT;
            }
            return new UsdShadeShaderOpaque(shader);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

// MARK: - Material Volume Output

UsdShadeOutputRef UsdShadeMaterial_CreateVolumeOutput(
    UsdShadeMaterialRef material,
    TfTokenRef renderContext
) {
    if (!material) return nullptr;
    try {
#if USD_USE_FULL
        TfToken context = renderContext ? renderContext->token : TfToken();
        UsdShadeOutput output = material->material.CreateVolumeOutput(context);
        if (output) {
            return new UsdShadeOutputOpaque(output);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdShadeOutputRef UsdShadeMaterial_GetVolumeOutput(
    UsdShadeMaterialRef material,
    TfTokenRef renderContext
) {
    if (!material) return nullptr;
    try {
#if USD_USE_FULL
        TfToken context = renderContext ? renderContext->token : TfToken();
        UsdShadeOutput output = material->material.GetVolumeOutput(context);
        if (output) {
            return new UsdShadeOutputOpaque(output);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

// MARK: - Base Material

UsdShadeMaterialRef UsdShadeMaterial_GetBaseMaterial(UsdShadeMaterialRef material) {
    if (!material) return nullptr;
    try {
#if USD_USE_FULL
        UsdShadeMaterial base = material->material.GetBaseMaterial();
        if (base) {
            return new UsdShadeMaterialOpaque(base);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

SdfPathRef UsdShadeMaterial_GetBaseMaterialPath(UsdShadeMaterialRef material) {
    if (!material) return nullptr;
    try {
#if USD_USE_FULL
        SdfPath path = material->material.GetBaseMaterialPath();
        if (!path.IsEmpty()) {
            return SdfPath_Create(path.GetString().c_str());
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdResult UsdShadeMaterial_SetBaseMaterial(
    UsdShadeMaterialRef material,
    UsdShadeMaterialRef baseMaterial
) {
    if (!material || !baseMaterial) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        material->material.SetBaseMaterial(baseMaterial->material);
        return USD_RESULT_SUCCESS;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdShadeMaterial_SetBaseMaterialPath(
    UsdShadeMaterialRef material,
    SdfPathRef basePath
) {
    if (!material || !basePath) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        material->material.SetBaseMaterialPath(basePath->path);
        return USD_RESULT_SUCCESS;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdShadeMaterial_ClearBaseMaterial(UsdShadeMaterialRef material) {
    if (!material) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        material->material.ClearBaseMaterial();
        return USD_RESULT_SUCCESS;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

bool UsdShadeMaterial_HasBaseMaterial(UsdShadeMaterialRef material) {
    if (!material) return false;
    try {
#if USD_USE_FULL
        return material->material.HasBaseMaterial();
#else
        return false;
#endif
    } CATCH_AND_RETURN(false)
}

// MARK: - Material Inputs

UsdShadeInputRef UsdShadeMaterial_CreateInput(
    UsdShadeMaterialRef material,
    TfTokenRef name,
    TfTokenRef typeName
) {
    if (!material || !name || !typeName) return nullptr;
    try {
#if USD_USE_FULL
        SdfValueTypeName sdfTypeName = SdfSchema::GetInstance().FindType(typeName->token.GetString());
        UsdShadeInput input = material->material.CreateInput(name->token, sdfTypeName);
        if (input) {
            return new UsdShadeInputOpaque(input);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdShadeInputRef UsdShadeMaterial_GetInput(
    UsdShadeMaterialRef material,
    TfTokenRef name
) {
    if (!material || !name) return nullptr;
    try {
#if USD_USE_FULL
        UsdShadeInput input = material->material.GetInput(name->token);
        if (input) {
            return new UsdShadeInputOpaque(input);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

size_t UsdShadeMaterial_GetInputCount(
    UsdShadeMaterialRef material,
    bool onlyAuthored
) {
    if (!material) return 0;
    try {
#if USD_USE_FULL
        std::vector<UsdShadeInput> inputs = material->material.GetInputs(onlyAuthored);
        return inputs.size();
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdShadeMaterial_GetInputs(
    UsdShadeMaterialRef material,
    UsdShadeInputRef* outInputs,
    size_t maxCount,
    bool onlyAuthored
) {
    if (!material || !outInputs || maxCount == 0) return 0;
    try {
#if USD_USE_FULL
        std::vector<UsdShadeInput> inputs = material->material.GetInputs(onlyAuthored);
        size_t count = std::min(inputs.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outInputs[i] = new UsdShadeInputOpaque(inputs[i]);
        }
        return count;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

UsdShadeOutputRef UsdShadeMaterial_CreateOutput(
    UsdShadeMaterialRef material,
    TfTokenRef name,
    TfTokenRef typeName
) {
    if (!material || !name || !typeName) return nullptr;
    try {
#if USD_USE_FULL
        SdfValueTypeName sdfTypeName = SdfSchema::GetInstance().FindType(typeName->token.GetString());
        UsdShadeOutput output = material->material.CreateOutput(name->token, sdfTypeName);
        if (output) {
            return new UsdShadeOutputOpaque(output);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdShadeOutputRef UsdShadeMaterial_GetOutput(
    UsdShadeMaterialRef material,
    TfTokenRef name
) {
    if (!material || !name) return nullptr;
    try {
#if USD_USE_FULL
        UsdShadeOutput output = material->material.GetOutput(name->token);
        if (output) {
            return new UsdShadeOutputOpaque(output);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

size_t UsdShadeMaterial_GetOutputCount(
    UsdShadeMaterialRef material,
    bool onlyAuthored
) {
    if (!material) return 0;
    try {
#if USD_USE_FULL
        std::vector<UsdShadeOutput> outputs = material->material.GetOutputs(onlyAuthored);
        return outputs.size();
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdShadeMaterial_GetOutputs(
    UsdShadeMaterialRef material,
    UsdShadeOutputRef* outOutputs,
    size_t maxCount,
    bool onlyAuthored
) {
    if (!material || !outOutputs || maxCount == 0) return 0;
    try {
#if USD_USE_FULL
        std::vector<UsdShadeOutput> outputs = material->material.GetOutputs(onlyAuthored);
        size_t count = std::min(outputs.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outOutputs[i] = new UsdShadeOutputOpaque(outputs[i]);
        }
        return count;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

// ============================================================================
// MARK: - UsdShadeShader Implementation
// ============================================================================

UsdShadeShaderRef UsdShadeShader_Get(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        if (stage->stage) {
            UsdShadeShader shader = UsdShadeShader::Get(stage->stage, path->path);
            if (shader) {
                return new UsdShadeShaderOpaque(shader);
            }
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdShadeShaderRef UsdShadeShader_Define(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        if (stage->stage) {
            UsdShadeShader shader = UsdShadeShader::Define(stage->stage, path->path);
            if (shader) {
                return new UsdShadeShaderOpaque(shader);
            }
        }
        return nullptr;
#else
        auto* wrapper = new UsdShadeShaderOpaque();
        wrapper->path = SdfPath_GetString(path);
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdShadeShaderRef UsdShadeShader_FromPrim(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdShadeShader shader(prim->prim);
        if (shader) {
            return new UsdShadeShaderOpaque(shader);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdShadeShaderRef UsdShadeShader_Retain(UsdShadeShaderRef shader) {
    if (!shader) return nullptr;
    shader->refCount.fetch_add(1, std::memory_order_relaxed);
    return shader;
}

void UsdShadeShader_Release(UsdShadeShaderRef shader) {
    if (!shader) return;
    if (shader->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete shader;
    }
}

bool UsdShadeShader_IsValid(UsdShadeShaderRef shader) {
    if (!shader) return false;
    try {
#if USD_USE_FULL
        return shader->shader.GetPrim().IsValid();
#else
        return !shader->path.empty();
#endif
    } CATCH_AND_RETURN(false)
}

UsdPrimRef UsdShadeShader_GetPrim(UsdShadeShaderRef shader) {
    if (!shader) return nullptr;
    try {
#if USD_USE_FULL
        UsdPrim prim = shader->shader.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

SdfPathRef UsdShadeShader_GetPath(UsdShadeShaderRef shader) {
    if (!shader) return nullptr;
    try {
#if USD_USE_FULL
        SdfPath path = shader->shader.GetPath();
        return SdfPath_Create(path.GetString().c_str());
#else
        return SdfPath_Create(shader->path.c_str());
#endif
    } CATCH_AND_RETURN(nullptr)
}

// MARK: - Shader ID

TfTokenRef UsdShadeShader_GetShaderId(UsdShadeShaderRef shader) {
    if (!shader) return nullptr;
    try {
#if USD_USE_FULL
        TfToken id;
        if (shader->shader.GetShaderId(&id)) {
            return TfToken_Create(id.GetString().c_str());
        }
        return nullptr;
#else
        return TfToken_Create(shader->shaderId.c_str());
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdResult UsdShadeShader_SetShaderId(UsdShadeShaderRef shader, TfTokenRef id) {
    if (!shader || !id) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (shader->shader.SetShaderId(id->token)) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        shader->shaderId = id->text;
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdAttributeRef UsdShadeShader_GetIdAttr(UsdShadeShaderRef shader) {
    if (!shader) return nullptr;
    try {
#if USD_USE_FULL
        UsdAttribute attr = shader->shader.GetIdAttr();
        if (attr) {
            return new UsdAttributeOpaque(attr);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdAttributeRef UsdShadeShader_CreateIdAttr(UsdShadeShaderRef shader, TfTokenRef defaultValue) {
    if (!shader) return nullptr;
    try {
#if USD_USE_FULL
        VtValue defaultVal;
        if (defaultValue) {
            defaultVal = VtValue(defaultValue->token);
        }
        UsdAttribute attr = shader->shader.CreateIdAttr(defaultVal);
        if (attr) {
            return new UsdAttributeOpaque(attr);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

// MARK: - Shader Source Asset

UsdResult UsdShadeShader_GetSourceAsset(
    UsdShadeShaderRef shader,
    SdfAssetPathRef* outPath,
    TfTokenRef sourceType
) {
    if (!shader || !outPath) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        TfToken type = sourceType ? sourceType->token : TfToken();
        SdfAssetPath assetPath;
        if (shader->shader.GetSourceAsset(&assetPath, type)) {
            *outPath = SdfAssetPath_Create(assetPath.GetAssetPath().c_str());
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_NOT_FOUND;
#else
        return USD_RESULT_NOT_FOUND;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdShadeShader_SetSourceAsset(
    UsdShadeShaderRef shader,
    SdfAssetPathRef path,
    TfTokenRef sourceType
) {
    if (!shader || !path) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        TfToken type = sourceType ? sourceType->token : TfToken();
        if (shader->shader.SetSourceAsset(path->assetPath, type)) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdShadeShader_GetSourceAssetSubIdentifier(
    UsdShadeShaderRef shader,
    TfTokenRef* outSubId,
    TfTokenRef sourceType
) {
    if (!shader || !outSubId) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        TfToken type = sourceType ? sourceType->token : TfToken();
        TfToken subId;
        if (shader->shader.GetSourceAssetSubIdentifier(&subId, type)) {
            *outSubId = TfToken_Create(subId.GetString().c_str());
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_NOT_FOUND;
#else
        return USD_RESULT_NOT_FOUND;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdShadeShader_SetSourceAssetSubIdentifier(
    UsdShadeShaderRef shader,
    TfTokenRef subId,
    TfTokenRef sourceType
) {
    if (!shader || !subId) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        TfToken type = sourceType ? sourceType->token : TfToken();
        if (shader->shader.SetSourceAssetSubIdentifier(subId->token, type)) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

// MARK: - Shader Source Code

UsdResult UsdShadeShader_GetSourceCode(
    UsdShadeShaderRef shader,
    char** outCode,
    TfTokenRef sourceType
) {
    if (!shader || !outCode) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        TfToken type = sourceType ? sourceType->token : TfToken();
        std::string code;
        if (shader->shader.GetSourceCode(&code, type)) {
            *outCode = strdup(code.c_str());
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_NOT_FOUND;
#else
        return USD_RESULT_NOT_FOUND;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdShadeShader_SetSourceCode(
    UsdShadeShaderRef shader,
    const char* code,
    TfTokenRef sourceType
) {
    if (!shader || !code) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        TfToken type = sourceType ? sourceType->token : TfToken();
        if (shader->shader.SetSourceCode(code, type)) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

void UsdShadeShader_FreeString(char* str) {
    if (str) free(str);
}

// MARK: - Shader Inputs/Outputs

UsdShadeInputRef UsdShadeShader_CreateInput(
    UsdShadeShaderRef shader,
    TfTokenRef name,
    TfTokenRef typeName
) {
    if (!shader || !name || !typeName) return nullptr;
    try {
#if USD_USE_FULL
        SdfValueTypeName sdfTypeName = SdfSchema::GetInstance().FindType(typeName->token.GetString());
        UsdShadeInput input = shader->shader.CreateInput(name->token, sdfTypeName);
        if (input) {
            return new UsdShadeInputOpaque(input);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdShadeInputRef UsdShadeShader_GetInput(
    UsdShadeShaderRef shader,
    TfTokenRef name
) {
    if (!shader || !name) return nullptr;
    try {
#if USD_USE_FULL
        UsdShadeInput input = shader->shader.GetInput(name->token);
        if (input) {
            return new UsdShadeInputOpaque(input);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

size_t UsdShadeShader_GetInputCount(
    UsdShadeShaderRef shader,
    bool onlyAuthored
) {
    if (!shader) return 0;
    try {
#if USD_USE_FULL
        std::vector<UsdShadeInput> inputs = shader->shader.GetInputs(onlyAuthored);
        return inputs.size();
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdShadeShader_GetInputs(
    UsdShadeShaderRef shader,
    UsdShadeInputRef* outInputs,
    size_t maxCount,
    bool onlyAuthored
) {
    if (!shader || !outInputs || maxCount == 0) return 0;
    try {
#if USD_USE_FULL
        std::vector<UsdShadeInput> inputs = shader->shader.GetInputs(onlyAuthored);
        size_t count = std::min(inputs.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outInputs[i] = new UsdShadeInputOpaque(inputs[i]);
        }
        return count;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

UsdShadeOutputRef UsdShadeShader_CreateOutput(
    UsdShadeShaderRef shader,
    TfTokenRef name,
    TfTokenRef typeName
) {
    if (!shader || !name || !typeName) return nullptr;
    try {
#if USD_USE_FULL
        SdfValueTypeName sdfTypeName = SdfSchema::GetInstance().FindType(typeName->token.GetString());
        UsdShadeOutput output = shader->shader.CreateOutput(name->token, sdfTypeName);
        if (output) {
            return new UsdShadeOutputOpaque(output);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdShadeOutputRef UsdShadeShader_GetOutput(
    UsdShadeShaderRef shader,
    TfTokenRef name
) {
    if (!shader || !name) return nullptr;
    try {
#if USD_USE_FULL
        UsdShadeOutput output = shader->shader.GetOutput(name->token);
        if (output) {
            return new UsdShadeOutputOpaque(output);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

size_t UsdShadeShader_GetOutputCount(
    UsdShadeShaderRef shader,
    bool onlyAuthored
) {
    if (!shader) return 0;
    try {
#if USD_USE_FULL
        std::vector<UsdShadeOutput> outputs = shader->shader.GetOutputs(onlyAuthored);
        return outputs.size();
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdShadeShader_GetOutputs(
    UsdShadeShaderRef shader,
    UsdShadeOutputRef* outOutputs,
    size_t maxCount,
    bool onlyAuthored
) {
    if (!shader || !outOutputs || maxCount == 0) return 0;
    try {
#if USD_USE_FULL
        std::vector<UsdShadeOutput> outputs = shader->shader.GetOutputs(onlyAuthored);
        size_t count = std::min(outputs.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outOutputs[i] = new UsdShadeOutputOpaque(outputs[i]);
        }
        return count;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

// ============================================================================
// MARK: - UsdShadeNodeGraph Implementation
// ============================================================================

UsdShadeNodeGraphRef UsdShadeNodeGraph_Get(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        if (stage->stage) {
            UsdShadeNodeGraph nodeGraph = UsdShadeNodeGraph::Get(stage->stage, path->path);
            if (nodeGraph) {
                return new UsdShadeNodeGraphOpaque(nodeGraph);
            }
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdShadeNodeGraphRef UsdShadeNodeGraph_Define(UsdStageRef stage, SdfPathRef path) {
    if (!stage || !path) return nullptr;
    try {
#if USD_USE_FULL
        if (stage->stage) {
            UsdShadeNodeGraph nodeGraph = UsdShadeNodeGraph::Define(stage->stage, path->path);
            if (nodeGraph) {
                return new UsdShadeNodeGraphOpaque(nodeGraph);
            }
        }
        return nullptr;
#else
        auto* wrapper = new UsdShadeNodeGraphOpaque();
        wrapper->path = SdfPath_GetString(path);
        return wrapper;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdShadeNodeGraphRef UsdShadeNodeGraph_FromPrim(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdShadeNodeGraph nodeGraph(prim->prim);
        if (nodeGraph) {
            return new UsdShadeNodeGraphOpaque(nodeGraph);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdShadeNodeGraphRef UsdShadeNodeGraph_Retain(UsdShadeNodeGraphRef nodeGraph) {
    if (!nodeGraph) return nullptr;
    nodeGraph->refCount.fetch_add(1, std::memory_order_relaxed);
    return nodeGraph;
}

void UsdShadeNodeGraph_Release(UsdShadeNodeGraphRef nodeGraph) {
    if (!nodeGraph) return;
    if (nodeGraph->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete nodeGraph;
    }
}

bool UsdShadeNodeGraph_IsValid(UsdShadeNodeGraphRef nodeGraph) {
    if (!nodeGraph) return false;
    try {
#if USD_USE_FULL
        return nodeGraph->nodeGraph.GetPrim().IsValid();
#else
        return !nodeGraph->path.empty();
#endif
    } CATCH_AND_RETURN(false)
}

UsdPrimRef UsdShadeNodeGraph_GetPrim(UsdShadeNodeGraphRef nodeGraph) {
    if (!nodeGraph) return nullptr;
    try {
#if USD_USE_FULL
        UsdPrim prim = nodeGraph->nodeGraph.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

SdfPathRef UsdShadeNodeGraph_GetPath(UsdShadeNodeGraphRef nodeGraph) {
    if (!nodeGraph) return nullptr;
    try {
#if USD_USE_FULL
        SdfPath path = nodeGraph->nodeGraph.GetPath();
        return SdfPath_Create(path.GetString().c_str());
#else
        return SdfPath_Create(nodeGraph->path.c_str());
#endif
    } CATCH_AND_RETURN(nullptr)
}

// MARK: - NodeGraph Inputs/Outputs

UsdShadeInputRef UsdShadeNodeGraph_CreateInput(
    UsdShadeNodeGraphRef nodeGraph,
    TfTokenRef name,
    TfTokenRef typeName
) {
    if (!nodeGraph || !name || !typeName) return nullptr;
    try {
#if USD_USE_FULL
        SdfValueTypeName sdfTypeName = SdfSchema::GetInstance().FindType(typeName->token.GetString());
        UsdShadeInput input = nodeGraph->nodeGraph.CreateInput(name->token, sdfTypeName);
        if (input) {
            return new UsdShadeInputOpaque(input);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdShadeInputRef UsdShadeNodeGraph_GetInput(
    UsdShadeNodeGraphRef nodeGraph,
    TfTokenRef name
) {
    if (!nodeGraph || !name) return nullptr;
    try {
#if USD_USE_FULL
        UsdShadeInput input = nodeGraph->nodeGraph.GetInput(name->token);
        if (input) {
            return new UsdShadeInputOpaque(input);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

size_t UsdShadeNodeGraph_GetInputCount(
    UsdShadeNodeGraphRef nodeGraph,
    bool onlyAuthored
) {
    if (!nodeGraph) return 0;
    try {
#if USD_USE_FULL
        std::vector<UsdShadeInput> inputs = nodeGraph->nodeGraph.GetInputs(onlyAuthored);
        return inputs.size();
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdShadeNodeGraph_GetInputs(
    UsdShadeNodeGraphRef nodeGraph,
    UsdShadeInputRef* outInputs,
    size_t maxCount,
    bool onlyAuthored
) {
    if (!nodeGraph || !outInputs || maxCount == 0) return 0;
    try {
#if USD_USE_FULL
        std::vector<UsdShadeInput> inputs = nodeGraph->nodeGraph.GetInputs(onlyAuthored);
        size_t count = std::min(inputs.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outInputs[i] = new UsdShadeInputOpaque(inputs[i]);
        }
        return count;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

UsdShadeOutputRef UsdShadeNodeGraph_CreateOutput(
    UsdShadeNodeGraphRef nodeGraph,
    TfTokenRef name,
    TfTokenRef typeName
) {
    if (!nodeGraph || !name || !typeName) return nullptr;
    try {
#if USD_USE_FULL
        SdfValueTypeName sdfTypeName = SdfSchema::GetInstance().FindType(typeName->token.GetString());
        UsdShadeOutput output = nodeGraph->nodeGraph.CreateOutput(name->token, sdfTypeName);
        if (output) {
            return new UsdShadeOutputOpaque(output);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdShadeOutputRef UsdShadeNodeGraph_GetOutput(
    UsdShadeNodeGraphRef nodeGraph,
    TfTokenRef name
) {
    if (!nodeGraph || !name) return nullptr;
    try {
#if USD_USE_FULL
        UsdShadeOutput output = nodeGraph->nodeGraph.GetOutput(name->token);
        if (output) {
            return new UsdShadeOutputOpaque(output);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

size_t UsdShadeNodeGraph_GetOutputCount(
    UsdShadeNodeGraphRef nodeGraph,
    bool onlyAuthored
) {
    if (!nodeGraph) return 0;
    try {
#if USD_USE_FULL
        std::vector<UsdShadeOutput> outputs = nodeGraph->nodeGraph.GetOutputs(onlyAuthored);
        return outputs.size();
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdShadeNodeGraph_GetOutputs(
    UsdShadeNodeGraphRef nodeGraph,
    UsdShadeOutputRef* outOutputs,
    size_t maxCount,
    bool onlyAuthored
) {
    if (!nodeGraph || !outOutputs || maxCount == 0) return 0;
    try {
#if USD_USE_FULL
        std::vector<UsdShadeOutput> outputs = nodeGraph->nodeGraph.GetOutputs(onlyAuthored);
        size_t count = std::min(outputs.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            outOutputs[i] = new UsdShadeOutputOpaque(outputs[i]);
        }
        return count;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

UsdResult UsdShadeNodeGraph_ComputeInterfaceInputConsumersMap(
    UsdShadeNodeGraphRef nodeGraph,
    bool computeTransitiveConsumers
) {
    if (!nodeGraph) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        nodeGraph->nodeGraph.ComputeInterfaceInputConsumersMap(computeTransitiveConsumers);
        return USD_RESULT_SUCCESS;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

// ============================================================================
// MARK: - UsdShadeInput Implementation
// ============================================================================

UsdShadeInputRef UsdShadeInput_FromAttribute(UsdAttributeRef attr) {
    if (!attr) return nullptr;
    try {
#if USD_USE_FULL
        UsdShadeInput input(attr->attr);
        if (input) {
            return new UsdShadeInputOpaque(input);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdShadeInputRef UsdShadeInput_Retain(UsdShadeInputRef input) {
    if (!input) return nullptr;
    input->refCount.fetch_add(1, std::memory_order_relaxed);
    return input;
}

void UsdShadeInput_Release(UsdShadeInputRef input) {
    if (!input) return;
    if (input->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete input;
    }
}

bool UsdShadeInput_IsValid(UsdShadeInputRef input) {
    if (!input) return false;
    try {
#if USD_USE_FULL
        return bool(input->input);
#else
        return !input->name.empty();
#endif
    } CATCH_AND_RETURN(false)
}

bool UsdShadeInput_IsDefined(UsdShadeInputRef input) {
    if (!input) return false;
    try {
#if USD_USE_FULL
        return input->input.IsDefined();
#else
        return !input->name.empty();
#endif
    } CATCH_AND_RETURN(false)
}

TfTokenRef UsdShadeInput_GetFullName(UsdShadeInputRef input) {
    if (!input) return nullptr;
    try {
#if USD_USE_FULL
        TfToken name = input->input.GetFullName();
        return TfToken_Create(name.GetString().c_str());
#else
        return TfToken_Create(input->name.c_str());
#endif
    } CATCH_AND_RETURN(nullptr)
}

TfTokenRef UsdShadeInput_GetBaseName(UsdShadeInputRef input) {
    if (!input) return nullptr;
    try {
#if USD_USE_FULL
        TfToken name = input->input.GetBaseName();
        return TfToken_Create(name.GetString().c_str());
#else
        return TfToken_Create(input->name.c_str());
#endif
    } CATCH_AND_RETURN(nullptr)
}

TfTokenRef UsdShadeInput_GetTypeName(UsdShadeInputRef input) {
    if (!input) return nullptr;
    try {
#if USD_USE_FULL
        SdfValueTypeName typeName = input->input.GetTypeName();
        return TfToken_Create(typeName.GetAsToken().GetString().c_str());
#else
        return TfToken_Create(input->typeName.c_str());
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdPrimRef UsdShadeInput_GetPrim(UsdShadeInputRef input) {
    if (!input) return nullptr;
    try {
#if USD_USE_FULL
        UsdPrim prim = input->input.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdAttributeRef UsdShadeInput_GetAttr(UsdShadeInputRef input) {
    if (!input) return nullptr;
    try {
#if USD_USE_FULL
        UsdAttribute attr = input->input.GetAttr();
        if (attr) {
            return new UsdAttributeOpaque(attr);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

// MARK: - Input Value Access

UsdResult UsdShadeInput_GetBool(UsdShadeInputRef input, UsdTimeCode time, bool* value) {
    if (!input || !value) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        VtValue vtVal;
        if (input->input.Get(&vtVal, ToTimeCode(time)) && vtVal.IsHolding<bool>()) {
            *value = vtVal.UncheckedGet<bool>();
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_NOT_FOUND;
#else
        return USD_RESULT_NOT_FOUND;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdShadeInput_SetBool(UsdShadeInputRef input, UsdTimeCode time, bool value) {
    if (!input) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (input->input.Set(value, ToTimeCode(time))) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdShadeInput_GetInt(UsdShadeInputRef input, UsdTimeCode time, int* value) {
    if (!input || !value) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        VtValue vtVal;
        if (input->input.Get(&vtVal, ToTimeCode(time)) && vtVal.IsHolding<int>()) {
            *value = vtVal.UncheckedGet<int>();
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_NOT_FOUND;
#else
        return USD_RESULT_NOT_FOUND;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdShadeInput_SetInt(UsdShadeInputRef input, UsdTimeCode time, int value) {
    if (!input) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (input->input.Set(value, ToTimeCode(time))) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdShadeInput_GetFloat(UsdShadeInputRef input, UsdTimeCode time, float* value) {
    if (!input || !value) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        VtValue vtVal;
        if (input->input.Get(&vtVal, ToTimeCode(time)) && vtVal.IsHolding<float>()) {
            *value = vtVal.UncheckedGet<float>();
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_NOT_FOUND;
#else
        return USD_RESULT_NOT_FOUND;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdShadeInput_SetFloat(UsdShadeInputRef input, UsdTimeCode time, float value) {
    if (!input) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (input->input.Set(value, ToTimeCode(time))) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdShadeInput_GetVec3f(UsdShadeInputRef input, UsdTimeCode time, float* x, float* y, float* z) {
    if (!input || !x || !y || !z) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        VtValue vtVal;
        if (input->input.Get(&vtVal, ToTimeCode(time)) && vtVal.IsHolding<GfVec3f>()) {
            GfVec3f vec = vtVal.UncheckedGet<GfVec3f>();
            *x = vec[0];
            *y = vec[1];
            *z = vec[2];
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_NOT_FOUND;
#else
        return USD_RESULT_NOT_FOUND;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdShadeInput_SetVec3f(UsdShadeInputRef input, UsdTimeCode time, float x, float y, float z) {
    if (!input) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (input->input.Set(GfVec3f(x, y, z), ToTimeCode(time))) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdShadeInput_GetString(UsdShadeInputRef input, UsdTimeCode time, char** value) {
    if (!input || !value) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        VtValue vtVal;
        if (input->input.Get(&vtVal, ToTimeCode(time)) && vtVal.IsHolding<std::string>()) {
            *value = strdup(vtVal.UncheckedGet<std::string>().c_str());
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_NOT_FOUND;
#else
        return USD_RESULT_NOT_FOUND;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdShadeInput_SetString(UsdShadeInputRef input, UsdTimeCode time, const char* value) {
    if (!input || !value) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (input->input.Set(std::string(value), ToTimeCode(time))) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdShadeInput_GetToken(UsdShadeInputRef input, UsdTimeCode time, TfTokenRef* value) {
    if (!input || !value) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        VtValue vtVal;
        if (input->input.Get(&vtVal, ToTimeCode(time)) && vtVal.IsHolding<TfToken>()) {
            *value = TfToken_Create(vtVal.UncheckedGet<TfToken>().GetString().c_str());
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_NOT_FOUND;
#else
        return USD_RESULT_NOT_FOUND;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdShadeInput_SetToken(UsdShadeInputRef input, UsdTimeCode time, TfTokenRef value) {
    if (!input || !value) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (input->input.Set(value->token, ToTimeCode(time))) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdShadeInput_GetAssetPath(UsdShadeInputRef input, UsdTimeCode time, SdfAssetPathRef* value) {
    if (!input || !value) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        VtValue vtVal;
        if (input->input.Get(&vtVal, ToTimeCode(time)) && vtVal.IsHolding<SdfAssetPath>()) {
            *value = SdfAssetPath_Create(vtVal.UncheckedGet<SdfAssetPath>().GetAssetPath().c_str());
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_NOT_FOUND;
#else
        return USD_RESULT_NOT_FOUND;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdShadeInput_SetAssetPath(UsdShadeInputRef input, UsdTimeCode time, SdfAssetPathRef value) {
    if (!input || !value) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (input->input.Set(value->assetPath, ToTimeCode(time))) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

void UsdShadeInput_FreeString(char* str) {
    if (str) free(str);
}

// MARK: - Input Render Type

TfTokenRef UsdShadeInput_GetRenderType(UsdShadeInputRef input) {
    if (!input) return nullptr;
    try {
#if USD_USE_FULL
        TfToken renderType = input->input.GetRenderType();
        return TfToken_Create(renderType.GetString().c_str());
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdResult UsdShadeInput_SetRenderType(UsdShadeInputRef input, TfTokenRef renderType) {
    if (!input || !renderType) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (input->input.SetRenderType(renderType->token)) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

bool UsdShadeInput_HasRenderType(UsdShadeInputRef input) {
    if (!input) return false;
    try {
#if USD_USE_FULL
        return input->input.HasRenderType();
#else
        return false;
#endif
    } CATCH_AND_RETURN(false)
}

// MARK: - Input Connectability

TfTokenRef UsdShadeInput_GetConnectability(UsdShadeInputRef input) {
    if (!input) return nullptr;
    try {
#if USD_USE_FULL
        TfToken connectability = input->input.GetConnectability();
        return TfToken_Create(connectability.GetString().c_str());
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdResult UsdShadeInput_SetConnectability(UsdShadeInputRef input, TfTokenRef connectability) {
    if (!input || !connectability) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (input->input.SetConnectability(connectability->token)) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdShadeInput_ClearConnectability(UsdShadeInputRef input) {
    if (!input) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (input->input.ClearConnectability()) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

// MARK: - Input Connections

bool UsdShadeInput_CanConnect(UsdShadeInputRef input, UsdShadeOutputRef source) {
    if (!input || !source) return false;
    try {
#if USD_USE_FULL
        return input->input.CanConnect(source->output);
#else
        return true;
#endif
    } CATCH_AND_RETURN(false)
}

UsdResult UsdShadeInput_ConnectToOutput(
    UsdShadeInputRef input,
    UsdShadeOutputRef source,
    UsdShadeConnectionModification mod
) {
    if (!input || !source) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (input->input.ConnectToSource(source->output, GetConnectionMod(mod))) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdShadeInput_ConnectToInput(
    UsdShadeInputRef input,
    UsdShadeInputRef source,
    UsdShadeConnectionModification mod
) {
    if (!input || !source) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (input->input.ConnectToSource(source->input, GetConnectionMod(mod))) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdShadeInput_ConnectToPath(
    UsdShadeInputRef input,
    SdfPathRef sourcePath,
    UsdShadeConnectionModification mod
) {
    if (!input || !sourcePath) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (input->input.ConnectToSource(sourcePath->path, GetConnectionMod(mod))) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

bool UsdShadeInput_HasConnectedSource(UsdShadeInputRef input) {
    if (!input) return false;
    try {
#if USD_USE_FULL
        return input->input.HasConnectedSource();
#else
        return false;
#endif
    } CATCH_AND_RETURN(false)
}

bool UsdShadeInput_IsSourceConnectionFromBaseMaterial(UsdShadeInputRef input) {
    if (!input) return false;
    try {
#if USD_USE_FULL
        return input->input.IsSourceConnectionFromBaseMaterial();
#else
        return false;
#endif
    } CATCH_AND_RETURN(false)
}

size_t UsdShadeInput_GetConnectedSourceCount(UsdShadeInputRef input) {
    if (!input) return 0;
    try {
#if USD_USE_FULL
        SdfPathVector invalidPaths;
        UsdShadeSourceInfoVector sources = input->input.GetConnectedSources(&invalidPaths);
        return sources.size();
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdShadeInput_GetConnectedSources(
    UsdShadeInputRef input,
    SdfPathRef* outPaths,
    size_t maxCount
) {
    if (!input || !outPaths || maxCount == 0) return 0;
    try {
#if USD_USE_FULL
        SdfPathVector invalidPaths;
        UsdShadeSourceInfoVector sources = input->input.GetConnectedSources(&invalidPaths);
        size_t count = std::min(sources.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            SdfPath path = sources[i].source.GetPath().AppendProperty(sources[i].sourceName);
            outPaths[i] = SdfPath_Create(path.GetString().c_str());
        }
        return count;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

UsdResult UsdShadeInput_DisconnectSource(UsdShadeInputRef input, SdfPathRef sourcePath) {
    if (!input) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        UsdAttribute attr = sourcePath ?
            input->input.GetPrim().GetStage()->GetAttributeAtPath(sourcePath->path) :
            UsdAttribute();
        if (input->input.DisconnectSource(attr)) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdShadeInput_ClearSources(UsdShadeInputRef input) {
    if (!input) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (input->input.ClearSources()) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

// MARK: - Input Documentation

char* UsdShadeInput_GetDocumentation(UsdShadeInputRef input) {
    if (!input) return nullptr;
    try {
#if USD_USE_FULL
        std::string docs = input->input.GetDocumentation();
        return strdup(docs.c_str());
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdResult UsdShadeInput_SetDocumentation(UsdShadeInputRef input, const char* docs) {
    if (!input || !docs) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (input->input.SetDocumentation(docs)) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

char* UsdShadeInput_GetDisplayGroup(UsdShadeInputRef input) {
    if (!input) return nullptr;
    try {
#if USD_USE_FULL
        std::string displayGroup = input->input.GetDisplayGroup();
        return strdup(displayGroup.c_str());
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdResult UsdShadeInput_SetDisplayGroup(UsdShadeInputRef input, const char* displayGroup) {
    if (!input || !displayGroup) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (input->input.SetDisplayGroup(displayGroup)) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

// ============================================================================
// MARK: - UsdShadeOutput Implementation
// ============================================================================

UsdShadeOutputRef UsdShadeOutput_FromAttribute(UsdAttributeRef attr) {
    if (!attr) return nullptr;
    try {
#if USD_USE_FULL
        UsdShadeOutput output(attr->attr);
        if (output) {
            return new UsdShadeOutputOpaque(output);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdShadeOutputRef UsdShadeOutput_Retain(UsdShadeOutputRef output) {
    if (!output) return nullptr;
    output->refCount.fetch_add(1, std::memory_order_relaxed);
    return output;
}

void UsdShadeOutput_Release(UsdShadeOutputRef output) {
    if (!output) return;
    if (output->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete output;
    }
}

bool UsdShadeOutput_IsValid(UsdShadeOutputRef output) {
    if (!output) return false;
    try {
#if USD_USE_FULL
        return bool(output->output);
#else
        return !output->name.empty();
#endif
    } CATCH_AND_RETURN(false)
}

bool UsdShadeOutput_IsDefined(UsdShadeOutputRef output) {
    if (!output) return false;
    try {
#if USD_USE_FULL
        return output->output.IsDefined();
#else
        return !output->name.empty();
#endif
    } CATCH_AND_RETURN(false)
}

TfTokenRef UsdShadeOutput_GetFullName(UsdShadeOutputRef output) {
    if (!output) return nullptr;
    try {
#if USD_USE_FULL
        TfToken name = output->output.GetFullName();
        return TfToken_Create(name.GetString().c_str());
#else
        return TfToken_Create(output->name.c_str());
#endif
    } CATCH_AND_RETURN(nullptr)
}

TfTokenRef UsdShadeOutput_GetBaseName(UsdShadeOutputRef output) {
    if (!output) return nullptr;
    try {
#if USD_USE_FULL
        TfToken name = output->output.GetBaseName();
        return TfToken_Create(name.GetString().c_str());
#else
        return TfToken_Create(output->name.c_str());
#endif
    } CATCH_AND_RETURN(nullptr)
}

TfTokenRef UsdShadeOutput_GetTypeName(UsdShadeOutputRef output) {
    if (!output) return nullptr;
    try {
#if USD_USE_FULL
        SdfValueTypeName typeName = output->output.GetTypeName();
        return TfToken_Create(typeName.GetAsToken().GetString().c_str());
#else
        return TfToken_Create(output->typeName.c_str());
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdPrimRef UsdShadeOutput_GetPrim(UsdShadeOutputRef output) {
    if (!output) return nullptr;
    try {
#if USD_USE_FULL
        UsdPrim prim = output->output.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdAttributeRef UsdShadeOutput_GetAttr(UsdShadeOutputRef output) {
    if (!output) return nullptr;
    try {
#if USD_USE_FULL
        UsdAttribute attr = output->output.GetAttr();
        if (attr) {
            return new UsdAttributeOpaque(attr);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

// MARK: - Output Render Type

TfTokenRef UsdShadeOutput_GetRenderType(UsdShadeOutputRef output) {
    if (!output) return nullptr;
    try {
#if USD_USE_FULL
        TfToken renderType = output->output.GetRenderType();
        return TfToken_Create(renderType.GetString().c_str());
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdResult UsdShadeOutput_SetRenderType(UsdShadeOutputRef output, TfTokenRef renderType) {
    if (!output || !renderType) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (output->output.SetRenderType(renderType->token)) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

bool UsdShadeOutput_HasRenderType(UsdShadeOutputRef output) {
    if (!output) return false;
    try {
#if USD_USE_FULL
        return output->output.HasRenderType();
#else
        return false;
#endif
    } CATCH_AND_RETURN(false)
}

// MARK: - Output Connections

bool UsdShadeOutput_CanConnect(UsdShadeOutputRef output, UsdShadeOutputRef source) {
    if (!output || !source) return false;
    try {
#if USD_USE_FULL
        return output->output.CanConnect(source->output);
#else
        return true;
#endif
    } CATCH_AND_RETURN(false)
}

UsdResult UsdShadeOutput_ConnectToOutput(
    UsdShadeOutputRef output,
    UsdShadeOutputRef source,
    UsdShadeConnectionModification mod
) {
    if (!output || !source) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (output->output.ConnectToSource(source->output, GetConnectionMod(mod))) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdShadeOutput_ConnectToInput(
    UsdShadeOutputRef output,
    UsdShadeInputRef source,
    UsdShadeConnectionModification mod
) {
    if (!output || !source) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (output->output.ConnectToSource(source->input, GetConnectionMod(mod))) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdShadeOutput_ConnectToPath(
    UsdShadeOutputRef output,
    SdfPathRef sourcePath,
    UsdShadeConnectionModification mod
) {
    if (!output || !sourcePath) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (output->output.ConnectToSource(sourcePath->path, GetConnectionMod(mod))) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

bool UsdShadeOutput_HasConnectedSource(UsdShadeOutputRef output) {
    if (!output) return false;
    try {
#if USD_USE_FULL
        return output->output.HasConnectedSource();
#else
        return false;
#endif
    } CATCH_AND_RETURN(false)
}

bool UsdShadeOutput_IsSourceConnectionFromBaseMaterial(UsdShadeOutputRef output) {
    if (!output) return false;
    try {
#if USD_USE_FULL
        return output->output.IsSourceConnectionFromBaseMaterial();
#else
        return false;
#endif
    } CATCH_AND_RETURN(false)
}

size_t UsdShadeOutput_GetConnectedSourceCount(UsdShadeOutputRef output) {
    if (!output) return 0;
    try {
#if USD_USE_FULL
        SdfPathVector invalidPaths;
        UsdShadeSourceInfoVector sources = output->output.GetConnectedSources(&invalidPaths);
        return sources.size();
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

size_t UsdShadeOutput_GetConnectedSources(
    UsdShadeOutputRef output,
    SdfPathRef* outPaths,
    size_t maxCount
) {
    if (!output || !outPaths || maxCount == 0) return 0;
    try {
#if USD_USE_FULL
        SdfPathVector invalidPaths;
        UsdShadeSourceInfoVector sources = output->output.GetConnectedSources(&invalidPaths);
        size_t count = std::min(sources.size(), maxCount);
        for (size_t i = 0; i < count; ++i) {
            SdfPath path = sources[i].source.GetPath().AppendProperty(sources[i].sourceName);
            outPaths[i] = SdfPath_Create(path.GetString().c_str());
        }
        return count;
#else
        return 0;
#endif
    } CATCH_AND_RETURN(0)
}

UsdResult UsdShadeOutput_DisconnectSource(UsdShadeOutputRef output, SdfPathRef sourcePath) {
    if (!output) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        UsdAttribute attr = sourcePath ?
            output->output.GetPrim().GetStage()->GetAttributeAtPath(sourcePath->path) :
            UsdAttribute();
        if (output->output.DisconnectSource(attr)) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdResult UsdShadeOutput_ClearSources(UsdShadeOutputRef output) {
    if (!output) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        if (output->output.ClearSources()) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

// ============================================================================
// MARK: - UsdShadeConnectableAPI Implementation
// ============================================================================

UsdShadeConnectableAPIRef UsdShadeConnectableAPI_Get(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdShadeConnectableAPI api(prim->prim);
        if (api) {
            return new UsdShadeConnectableAPIOpaque(api);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdShadeConnectableAPIRef UsdShadeConnectableAPI_Retain(UsdShadeConnectableAPIRef api) {
    if (!api) return nullptr;
    api->refCount.fetch_add(1, std::memory_order_relaxed);
    return api;
}

void UsdShadeConnectableAPI_Release(UsdShadeConnectableAPIRef api) {
    if (!api) return;
    if (api->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete api;
    }
}

bool UsdShadeConnectableAPI_IsValid(UsdShadeConnectableAPIRef api) {
    if (!api) return false;
    try {
#if USD_USE_FULL
        return bool(api->api);
#else
        return !api->path.empty();
#endif
    } CATCH_AND_RETURN(false)
}

bool UsdShadeConnectableAPI_IsContainer(UsdShadeConnectableAPIRef api) {
    if (!api) return false;
    try {
#if USD_USE_FULL
        return api->api.IsContainer();
#else
        return false;
#endif
    } CATCH_AND_RETURN(false)
}

bool UsdShadeConnectableAPI_RequiresEncapsulation(UsdShadeConnectableAPIRef api) {
    if (!api) return false;
    try {
#if USD_USE_FULL
        return api->api.RequiresEncapsulation();
#else
        return false;
#endif
    } CATCH_AND_RETURN(false)
}

UsdPrimRef UsdShadeConnectableAPI_GetPrim(UsdShadeConnectableAPIRef api) {
    if (!api) return nullptr;
    try {
#if USD_USE_FULL
        UsdPrim prim = api->api.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

bool UsdShadeConnectableAPI_CanConnectInput(
    UsdShadeInputRef input,
    UsdShadeOutputRef source
) {
    if (!input || !source) return false;
    try {
#if USD_USE_FULL
        return UsdShadeConnectableAPI::CanConnect(input->input, source->output);
#else
        return true;
#endif
    } CATCH_AND_RETURN(false)
}

bool UsdShadeConnectableAPI_CanConnectOutput(
    UsdShadeOutputRef output,
    UsdShadeOutputRef source
) {
    if (!output || !source) return false;
    try {
#if USD_USE_FULL
        return UsdShadeConnectableAPI::CanConnect(output->output, source->output);
#else
        return true;
#endif
    } CATCH_AND_RETURN(false)
}

// ============================================================================
// MARK: - UsdShadeMaterialBindingAPI Implementation
// ============================================================================

UsdShadeMaterialBindingAPIRef UsdShadeMaterialBindingAPI_Apply(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdShadeMaterialBindingAPI api = UsdShadeMaterialBindingAPI::Apply(prim->prim);
        if (api) {
            return new UsdShadeMaterialBindingAPIOpaque(api);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdShadeMaterialBindingAPIRef UsdShadeMaterialBindingAPI_Get(UsdPrimRef prim) {
    if (!prim) return nullptr;
    try {
#if USD_USE_FULL
        UsdShadeMaterialBindingAPI api(prim->prim);
        if (api) {
            return new UsdShadeMaterialBindingAPIOpaque(api);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdShadeMaterialBindingAPIRef UsdShadeMaterialBindingAPI_Retain(UsdShadeMaterialBindingAPIRef api) {
    if (!api) return nullptr;
    api->refCount.fetch_add(1, std::memory_order_relaxed);
    return api;
}

void UsdShadeMaterialBindingAPI_Release(UsdShadeMaterialBindingAPIRef api) {
    if (!api) return;
    if (api->refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete api;
    }
}

bool UsdShadeMaterialBindingAPI_IsValid(UsdShadeMaterialBindingAPIRef api) {
    if (!api) return false;
    try {
#if USD_USE_FULL
        return bool(api->api);
#else
        return !api->path.empty();
#endif
    } CATCH_AND_RETURN(false)
}

UsdPrimRef UsdShadeMaterialBindingAPI_GetPrim(UsdShadeMaterialBindingAPIRef api) {
    if (!api) return nullptr;
    try {
#if USD_USE_FULL
        UsdPrim prim = api->api.GetPrim();
        if (prim) {
            return new UsdPrimOpaque(prim);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

// MARK: - Direct Material Binding

UsdResult UsdShadeMaterialBindingAPI_Bind(
    UsdShadeMaterialBindingAPIRef api,
    UsdShadeMaterialRef material,
    UsdShadeBindingStrength strength,
    UsdShadeMaterialPurpose purpose
) {
    if (!api || !material) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        TfToken purposeToken = GetMaterialPurposeToken(purpose);
        UsdShadeMaterialBindingAPI::BindingStrength bindStrength;
        switch (strength) {
            case USDSHADE_BINDING_STRENGTH_FALLBACK:
                bindStrength = UsdShadeMaterialBindingAPI::BindingStrengthFallback;
                break;
            case USDSHADE_BINDING_STRENGTH_STRONGER:
                bindStrength = UsdShadeMaterialBindingAPI::BindingStrengthStronger;
                break;
            case USDSHADE_BINDING_STRENGTH_STRONG_THAN_DESCENDANTS:
                bindStrength = UsdShadeMaterialBindingAPI::BindingStrengthStronger;
                break;
            default:
                bindStrength = UsdShadeMaterialBindingAPI::BindingStrengthFallback;
        }
        if (api->api.Bind(material->material, bindStrength, purposeToken)) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

UsdShadeMaterialRef UsdShadeMaterialBindingAPI_GetDirectlyBoundMaterial(
    UsdShadeMaterialBindingAPIRef api,
    UsdShadeMaterialPurpose purpose
) {
    if (!api) return nullptr;
    try {
#if USD_USE_FULL
        TfToken purposeToken = GetMaterialPurposeToken(purpose);
        UsdShadeMaterialBindingAPI::DirectBinding binding = api->api.GetDirectBinding(purposeToken);
        if (binding.IsBound()) {
            UsdShadeMaterial material = binding.GetMaterial();
            if (material) {
                return new UsdShadeMaterialOpaque(material);
            }
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

SdfPathRef UsdShadeMaterialBindingAPI_GetDirectBindingPath(
    UsdShadeMaterialBindingAPIRef api,
    UsdShadeMaterialPurpose purpose
) {
    if (!api) return nullptr;
    try {
#if USD_USE_FULL
        TfToken purposeToken = GetMaterialPurposeToken(purpose);
        UsdShadeMaterialBindingAPI::DirectBinding binding = api->api.GetDirectBinding(purposeToken);
        if (binding.IsBound()) {
            SdfPath path = binding.GetMaterialPath();
            if (!path.IsEmpty()) {
                return SdfPath_Create(path.GetString().c_str());
            }
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdResult UsdShadeMaterialBindingAPI_UnbindDirectBinding(
    UsdShadeMaterialBindingAPIRef api,
    UsdShadeMaterialPurpose purpose
) {
    if (!api) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        TfToken purposeToken = GetMaterialPurposeToken(purpose);
        if (api->api.UnbindDirectBinding(purposeToken)) {
            return USD_RESULT_SUCCESS;
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

// MARK: - Computed Binding

UsdShadeMaterialRef UsdShadeMaterialBindingAPI_ComputeBoundMaterial(
    UsdShadeMaterialBindingAPIRef api,
    UsdShadeMaterialPurpose purpose
) {
    if (!api) return nullptr;
    try {
#if USD_USE_FULL
        TfToken purposeToken = GetMaterialPurposeToken(purpose);
        UsdShadeMaterial material = api->api.ComputeBoundMaterial(purposeToken);
        if (material) {
            return new UsdShadeMaterialOpaque(material);
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

SdfPathRef UsdShadeMaterialBindingAPI_ComputeBoundMaterialPath(
    UsdShadeMaterialBindingAPIRef api,
    UsdShadeMaterialPurpose purpose
) {
    if (!api) return nullptr;
    try {
#if USD_USE_FULL
        TfToken purposeToken = GetMaterialPurposeToken(purpose);
        UsdShadeMaterial material = api->api.ComputeBoundMaterial(purposeToken);
        if (material) {
            SdfPath path = material.GetPath();
            return SdfPath_Create(path.GetString().c_str());
        }
        return nullptr;
#else
        return nullptr;
#endif
    } CATCH_AND_RETURN(nullptr)
}

// MARK: - Binding Utilities

UsdShadeBindingStrength UsdShadeMaterialBindingAPI_GetDirectBindingStrength(
    UsdShadeMaterialBindingAPIRef api,
    UsdShadeMaterialPurpose purpose
) {
    if (!api) return USDSHADE_BINDING_STRENGTH_FALLBACK;
    try {
#if USD_USE_FULL
        TfToken purposeToken = GetMaterialPurposeToken(purpose);
        UsdRelationship rel = api->api.GetDirectBindingRel(purposeToken);
        if (rel) {
            UsdShadeMaterialBindingAPI::BindingStrength strength =
                UsdShadeMaterialBindingAPI::GetMaterialBindingStrength(rel);
            switch (strength) {
                case UsdShadeMaterialBindingAPI::BindingStrengthFallback:
                    return USDSHADE_BINDING_STRENGTH_FALLBACK;
                case UsdShadeMaterialBindingAPI::BindingStrengthStronger:
                    return USDSHADE_BINDING_STRENGTH_STRONGER;
            }
        }
        return USDSHADE_BINDING_STRENGTH_FALLBACK;
#else
        return USDSHADE_BINDING_STRENGTH_FALLBACK;
#endif
    } CATCH_AND_RETURN(USDSHADE_BINDING_STRENGTH_FALLBACK)
}

UsdResult UsdShadeMaterialBindingAPI_SetDirectBindingStrength(
    UsdShadeMaterialBindingAPIRef api,
    UsdShadeMaterialPurpose purpose,
    UsdShadeBindingStrength strength
) {
    if (!api) return USD_RESULT_INVALID_ARGUMENT;
    try {
#if USD_USE_FULL
        TfToken purposeToken = GetMaterialPurposeToken(purpose);
        UsdRelationship rel = api->api.GetDirectBindingRel(purposeToken);
        if (rel) {
            UsdShadeMaterialBindingAPI::BindingStrength bindStrength;
            switch (strength) {
                case USDSHADE_BINDING_STRENGTH_FALLBACK:
                    bindStrength = UsdShadeMaterialBindingAPI::BindingStrengthFallback;
                    break;
                case USDSHADE_BINDING_STRENGTH_STRONGER:
                case USDSHADE_BINDING_STRENGTH_STRONG_THAN_DESCENDANTS:
                    bindStrength = UsdShadeMaterialBindingAPI::BindingStrengthStronger;
                    break;
                default:
                    bindStrength = UsdShadeMaterialBindingAPI::BindingStrengthFallback;
            }
            if (UsdShadeMaterialBindingAPI::SetMaterialBindingStrength(rel, bindStrength)) {
                return USD_RESULT_SUCCESS;
            }
        }
        return USD_RESULT_ERROR;
#else
        return USD_RESULT_SUCCESS;
#endif
    } CATCH_AND_RETURN_RESULT
}

// ============================================================================
// MARK: - Utility Functions
// ============================================================================

TfTokenRef UsdShadeUtils_GetInputPrefix(void) {
    try {
#if USD_USE_FULL
        TfToken prefix = UsdShadeUtils::GetPrefixForAttributeType(UsdShadeAttributeType::Input);
        return TfToken_Create(prefix.GetString().c_str());
#else
        return TfToken_Create("inputs:");
#endif
    } CATCH_AND_RETURN(nullptr)
}

TfTokenRef UsdShadeUtils_GetOutputPrefix(void) {
    try {
#if USD_USE_FULL
        TfToken prefix = UsdShadeUtils::GetPrefixForAttributeType(UsdShadeAttributeType::Output);
        return TfToken_Create(prefix.GetString().c_str());
#else
        return TfToken_Create("outputs:");
#endif
    } CATCH_AND_RETURN(nullptr)
}

bool UsdShadeUtils_IsInputName(const char* name) {
    if (!name) return false;
    try {
#if USD_USE_FULL
        return TfStringStartsWith(name, "inputs:");
#else
        return strncmp(name, "inputs:", 7) == 0;
#endif
    } CATCH_AND_RETURN(false)
}

bool UsdShadeUtils_IsOutputName(const char* name) {
    if (!name) return false;
    try {
#if USD_USE_FULL
        return TfStringStartsWith(name, "outputs:");
#else
        return strncmp(name, "outputs:", 8) == 0;
#endif
    } CATCH_AND_RETURN(false)
}

TfTokenRef UsdShadeUtils_GetBaseNameFromFullName(const char* fullName) {
    if (!fullName) return nullptr;
    try {
#if USD_USE_FULL
        std::pair<TfToken, UsdShadeAttributeType> result =
            UsdShadeUtils::GetBaseNameAndType(TfToken(fullName));
        return TfToken_Create(result.first.GetString().c_str());
#else
        const char* colonPos = strchr(fullName, ':');
        if (colonPos) {
            return TfToken_Create(colonPos + 1);
        }
        return TfToken_Create(fullName);
#endif
    } CATCH_AND_RETURN(nullptr)
}

UsdShadeAttributeType UsdShadeUtils_GetTypeFromFullName(const char* fullName) {
    if (!fullName) return USDSHADE_ATTR_TYPE_INVALID;
    try {
#if USD_USE_FULL
        std::pair<TfToken, UsdShadeAttributeType> result =
            UsdShadeUtils::GetBaseNameAndType(TfToken(fullName));
        switch (result.second) {
            case UsdShadeAttributeType::Input:
                return USDSHADE_ATTR_TYPE_INPUT;
            case UsdShadeAttributeType::Output:
                return USDSHADE_ATTR_TYPE_OUTPUT;
            default:
                return USDSHADE_ATTR_TYPE_INVALID;
        }
#else
        if (strncmp(fullName, "inputs:", 7) == 0) return USDSHADE_ATTR_TYPE_INPUT;
        if (strncmp(fullName, "outputs:", 8) == 0) return USDSHADE_ATTR_TYPE_OUTPUT;
        return USDSHADE_ATTR_TYPE_INVALID;
#endif
    } CATCH_AND_RETURN(USDSHADE_ATTR_TYPE_INVALID)
}

TfTokenRef UsdShadeUtils_CreateInputName(TfTokenRef baseName) {
    if (!baseName) return nullptr;
    try {
#if USD_USE_FULL
        TfToken fullName = UsdShadeUtils::GetFullName(baseName->token, UsdShadeAttributeType::Input);
        return TfToken_Create(fullName.GetString().c_str());
#else
        std::string fullName = "inputs:" + baseName->text;
        return TfToken_Create(fullName.c_str());
#endif
    } CATCH_AND_RETURN(nullptr)
}

TfTokenRef UsdShadeUtils_CreateOutputName(TfTokenRef baseName) {
    if (!baseName) return nullptr;
    try {
#if USD_USE_FULL
        TfToken fullName = UsdShadeUtils::GetFullName(baseName->token, UsdShadeAttributeType::Output);
        return TfToken_Create(fullName.GetString().c_str());
#else
        std::string fullName = "outputs:" + baseName->text;
        return TfToken_Create(fullName.c_str());
#endif
    } CATCH_AND_RETURN(nullptr)
}

} // extern "C"
