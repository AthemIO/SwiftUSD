// usdShade.h - UsdShade module C wrapper
// Mirrors: pxr/usd/usdShade/*.h (Material, Shader, MaterialBindingAPI, Input, Output, ConnectableAPI)

#ifndef OPENUSDINTEROP_USDSHADE_USDSHADE_H
#define OPENUSDINTEROP_USDSHADE_USDSHADE_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Cross-module dependencies
#include "../base/tf.h"
#include "../base/vt.h"
#include "../usd/sdf.h"
#include "../usd/usd.h"

#ifndef USD_INTEROP_API
#if defined(_WIN32)
    #if defined(OPENUSDINTEROP_EXPORTS)
        #define USD_INTEROP_API __declspec(dllexport)
    #else
        #define USD_INTEROP_API __declspec(dllimport)
    #endif
#else
    #define USD_INTEROP_API __attribute__((visibility("default")))
#endif
#endif

#ifndef USD_RESULT_DEFINED
#define USD_RESULT_DEFINED
typedef enum {
    USD_RESULT_SUCCESS = 0,
    USD_RESULT_ERROR = 1,
    USD_RESULT_INVALID_ARGUMENT = 2,
    USD_RESULT_NOT_FOUND = 3,
    USD_RESULT_ALREADY_EXISTS = 4,
    USD_RESULT_OUT_OF_MEMORY = 5,
    USD_RESULT_IO_ERROR = 6
} UsdResult;
#endif

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// MARK: - Opaque Handle Types
// ============================================================================

typedef struct UsdShadeMaterialOpaque* UsdShadeMaterialRef;
typedef struct UsdShadeShaderOpaque* UsdShadeShaderRef;
typedef struct UsdShadeNodeGraphOpaque* UsdShadeNodeGraphRef;
typedef struct UsdShadeInputOpaque* UsdShadeInputRef;
typedef struct UsdShadeOutputOpaque* UsdShadeOutputRef;
typedef struct UsdShadeConnectableAPIOpaque* UsdShadeConnectableAPIRef;
typedef struct UsdShadeMaterialBindingAPIOpaque* UsdShadeMaterialBindingAPIRef;

// ============================================================================
// MARK: - Enums
// ============================================================================

/// Attribute type (input or output)
typedef enum {
    USDSHADE_ATTR_TYPE_INVALID = 0,
    USDSHADE_ATTR_TYPE_INPUT = 1,
    USDSHADE_ATTR_TYPE_OUTPUT = 2
} UsdShadeAttributeType;

/// Connection modification mode
typedef enum {
    USDSHADE_CONNECTION_REPLACE = 0,  ///< Replace existing connections
    USDSHADE_CONNECTION_PREPEND = 1,  ///< Add to beginning of connection list
    USDSHADE_CONNECTION_APPEND = 2    ///< Add to end of connection list
} UsdShadeConnectionModification;

/// Material binding strength
typedef enum {
    USDSHADE_BINDING_STRENGTH_FALLBACK = 0,   ///< Weak binding, can be overridden
    USDSHADE_BINDING_STRENGTH_STRONGER = 1,   ///< Strong binding, overrides fallback
    USDSHADE_BINDING_STRENGTH_STRONG_THAN_DESCENDANTS = 2 ///< Strongest binding
} UsdShadeBindingStrength;

/// Material purpose for bindings
typedef enum {
    USDSHADE_PURPOSE_ALL = 0,        ///< All-purpose binding
    USDSHADE_PURPOSE_PREVIEW = 1,    ///< Preview render binding
    USDSHADE_PURPOSE_FULL = 2        ///< Full render binding
} UsdShadeMaterialPurpose;

// ============================================================================
// MARK: - UsdShadeMaterial
// ============================================================================

/// Gets a Material schema from a prim at the given path.
/// Caller must release with UsdShadeMaterial_Release.
USD_INTEROP_API UsdShadeMaterialRef UsdShadeMaterial_Get(UsdStageRef stage, SdfPathRef path);

/// Defines a new Material prim at the given path.
/// Caller must release with UsdShadeMaterial_Release.
USD_INTEROP_API UsdShadeMaterialRef UsdShadeMaterial_Define(UsdStageRef stage, SdfPathRef path);

/// Creates a Material schema from an existing prim.
/// Caller must release with UsdShadeMaterial_Release.
USD_INTEROP_API UsdShadeMaterialRef UsdShadeMaterial_FromPrim(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdShadeMaterialRef UsdShadeMaterial_Retain(UsdShadeMaterialRef material);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdShadeMaterial_Release(UsdShadeMaterialRef material);

/// Returns true if the material is valid.
USD_INTEROP_API bool UsdShadeMaterial_IsValid(UsdShadeMaterialRef material);

/// Gets the underlying prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdShadeMaterial_GetPrim(UsdShadeMaterialRef material);

/// Gets the path of the material.
/// Caller must release with SdfPath_Release.
USD_INTEROP_API SdfPathRef UsdShadeMaterial_GetPath(UsdShadeMaterialRef material);

// MARK: - Material Surface Output

/// Creates the surface output for the given render context.
/// renderContext can be NULL for universal context.
/// Caller must release with UsdShadeOutput_Release.
USD_INTEROP_API UsdShadeOutputRef UsdShadeMaterial_CreateSurfaceOutput(
    UsdShadeMaterialRef material,
    TfTokenRef renderContext
);

/// Gets the surface output for the given render context.
/// renderContext can be NULL for universal context.
/// Caller must release with UsdShadeOutput_Release.
USD_INTEROP_API UsdShadeOutputRef UsdShadeMaterial_GetSurfaceOutput(
    UsdShadeMaterialRef material,
    TfTokenRef renderContext
);

/// Computes the surface source shader.
/// Caller must release with UsdShadeShader_Release.
USD_INTEROP_API UsdShadeShaderRef UsdShadeMaterial_ComputeSurfaceSource(
    UsdShadeMaterialRef material,
    TfTokenRef renderContext,
    TfTokenRef* outSourceName,
    UsdShadeAttributeType* outSourceType
);

// MARK: - Material Displacement Output

/// Creates the displacement output for the given render context.
/// Caller must release with UsdShadeOutput_Release.
USD_INTEROP_API UsdShadeOutputRef UsdShadeMaterial_CreateDisplacementOutput(
    UsdShadeMaterialRef material,
    TfTokenRef renderContext
);

/// Gets the displacement output for the given render context.
/// Caller must release with UsdShadeOutput_Release.
USD_INTEROP_API UsdShadeOutputRef UsdShadeMaterial_GetDisplacementOutput(
    UsdShadeMaterialRef material,
    TfTokenRef renderContext
);

/// Computes the displacement source shader.
/// Caller must release with UsdShadeShader_Release.
USD_INTEROP_API UsdShadeShaderRef UsdShadeMaterial_ComputeDisplacementSource(
    UsdShadeMaterialRef material,
    TfTokenRef renderContext,
    TfTokenRef* outSourceName,
    UsdShadeAttributeType* outSourceType
);

// MARK: - Material Volume Output

/// Creates the volume output for the given render context.
/// Caller must release with UsdShadeOutput_Release.
USD_INTEROP_API UsdShadeOutputRef UsdShadeMaterial_CreateVolumeOutput(
    UsdShadeMaterialRef material,
    TfTokenRef renderContext
);

/// Gets the volume output for the given render context.
/// Caller must release with UsdShadeOutput_Release.
USD_INTEROP_API UsdShadeOutputRef UsdShadeMaterial_GetVolumeOutput(
    UsdShadeMaterialRef material,
    TfTokenRef renderContext
);

// MARK: - Base Material (Specialization)

/// Gets the base material.
/// Caller must release with UsdShadeMaterial_Release.
USD_INTEROP_API UsdShadeMaterialRef UsdShadeMaterial_GetBaseMaterial(UsdShadeMaterialRef material);

/// Gets the base material path.
/// Caller must release with SdfPath_Release.
USD_INTEROP_API SdfPathRef UsdShadeMaterial_GetBaseMaterialPath(UsdShadeMaterialRef material);

/// Sets the base material.
USD_INTEROP_API UsdResult UsdShadeMaterial_SetBaseMaterial(
    UsdShadeMaterialRef material,
    UsdShadeMaterialRef baseMaterial
);

/// Sets the base material by path.
USD_INTEROP_API UsdResult UsdShadeMaterial_SetBaseMaterialPath(
    UsdShadeMaterialRef material,
    SdfPathRef basePath
);

/// Clears the base material.
USD_INTEROP_API UsdResult UsdShadeMaterial_ClearBaseMaterial(UsdShadeMaterialRef material);

/// Returns true if the material has a base material.
USD_INTEROP_API bool UsdShadeMaterial_HasBaseMaterial(UsdShadeMaterialRef material);

// MARK: - Material Interface (Inputs/Outputs)

/// Creates an input on the material.
/// Caller must release with UsdShadeInput_Release.
USD_INTEROP_API UsdShadeInputRef UsdShadeMaterial_CreateInput(
    UsdShadeMaterialRef material,
    TfTokenRef name,
    TfTokenRef typeName
);

/// Gets an input by name.
/// Caller must release with UsdShadeInput_Release.
USD_INTEROP_API UsdShadeInputRef UsdShadeMaterial_GetInput(
    UsdShadeMaterialRef material,
    TfTokenRef name
);

/// Gets the count of inputs.
USD_INTEROP_API size_t UsdShadeMaterial_GetInputCount(
    UsdShadeMaterialRef material,
    bool onlyAuthored
);

/// Gets all inputs.
/// outInputs must have space for maxCount inputs.
/// Caller must release each with UsdShadeInput_Release.
USD_INTEROP_API size_t UsdShadeMaterial_GetInputs(
    UsdShadeMaterialRef material,
    UsdShadeInputRef* outInputs,
    size_t maxCount,
    bool onlyAuthored
);

/// Creates an output on the material.
/// Caller must release with UsdShadeOutput_Release.
USD_INTEROP_API UsdShadeOutputRef UsdShadeMaterial_CreateOutput(
    UsdShadeMaterialRef material,
    TfTokenRef name,
    TfTokenRef typeName
);

/// Gets an output by name.
/// Caller must release with UsdShadeOutput_Release.
USD_INTEROP_API UsdShadeOutputRef UsdShadeMaterial_GetOutput(
    UsdShadeMaterialRef material,
    TfTokenRef name
);

/// Gets the count of outputs.
USD_INTEROP_API size_t UsdShadeMaterial_GetOutputCount(
    UsdShadeMaterialRef material,
    bool onlyAuthored
);

/// Gets all outputs.
/// outOutputs must have space for maxCount outputs.
/// Caller must release each with UsdShadeOutput_Release.
USD_INTEROP_API size_t UsdShadeMaterial_GetOutputs(
    UsdShadeMaterialRef material,
    UsdShadeOutputRef* outOutputs,
    size_t maxCount,
    bool onlyAuthored
);

// ============================================================================
// MARK: - UsdShadeShader
// ============================================================================

/// Gets a Shader schema from a prim at the given path.
/// Caller must release with UsdShadeShader_Release.
USD_INTEROP_API UsdShadeShaderRef UsdShadeShader_Get(UsdStageRef stage, SdfPathRef path);

/// Defines a new Shader prim at the given path.
/// Caller must release with UsdShadeShader_Release.
USD_INTEROP_API UsdShadeShaderRef UsdShadeShader_Define(UsdStageRef stage, SdfPathRef path);

/// Creates a Shader schema from an existing prim.
/// Caller must release with UsdShadeShader_Release.
USD_INTEROP_API UsdShadeShaderRef UsdShadeShader_FromPrim(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdShadeShaderRef UsdShadeShader_Retain(UsdShadeShaderRef shader);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdShadeShader_Release(UsdShadeShaderRef shader);

/// Returns true if the shader is valid.
USD_INTEROP_API bool UsdShadeShader_IsValid(UsdShadeShaderRef shader);

/// Gets the underlying prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdShadeShader_GetPrim(UsdShadeShaderRef shader);

/// Gets the path of the shader.
/// Caller must release with SdfPath_Release.
USD_INTEROP_API SdfPathRef UsdShadeShader_GetPath(UsdShadeShaderRef shader);

// MARK: - Shader ID

/// Gets the shader ID.
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef UsdShadeShader_GetShaderId(UsdShadeShaderRef shader);

/// Sets the shader ID.
USD_INTEROP_API UsdResult UsdShadeShader_SetShaderId(UsdShadeShaderRef shader, TfTokenRef id);

/// Gets the id attribute.
/// Caller must release with UsdAttribute_Release.
USD_INTEROP_API UsdAttributeRef UsdShadeShader_GetIdAttr(UsdShadeShaderRef shader);

/// Creates the id attribute.
/// Caller must release with UsdAttribute_Release.
USD_INTEROP_API UsdAttributeRef UsdShadeShader_CreateIdAttr(UsdShadeShaderRef shader, TfTokenRef defaultValue);

// MARK: - Shader Source Asset

/// Gets the source asset path.
/// Caller must release with SdfAssetPath_Release.
USD_INTEROP_API UsdResult UsdShadeShader_GetSourceAsset(
    UsdShadeShaderRef shader,
    SdfAssetPathRef* outPath,
    TfTokenRef sourceType
);

/// Sets the source asset path.
USD_INTEROP_API UsdResult UsdShadeShader_SetSourceAsset(
    UsdShadeShaderRef shader,
    SdfAssetPathRef path,
    TfTokenRef sourceType
);

/// Gets the source asset sub-identifier.
/// Caller must release with TfToken_Release.
USD_INTEROP_API UsdResult UsdShadeShader_GetSourceAssetSubIdentifier(
    UsdShadeShaderRef shader,
    TfTokenRef* outSubId,
    TfTokenRef sourceType
);

/// Sets the source asset sub-identifier.
USD_INTEROP_API UsdResult UsdShadeShader_SetSourceAssetSubIdentifier(
    UsdShadeShaderRef shader,
    TfTokenRef subId,
    TfTokenRef sourceType
);

// MARK: - Shader Source Code

/// Gets the source code.
/// Caller must free with UsdShadeShader_FreeString.
USD_INTEROP_API UsdResult UsdShadeShader_GetSourceCode(
    UsdShadeShaderRef shader,
    char** outCode,
    TfTokenRef sourceType
);

/// Sets the source code.
USD_INTEROP_API UsdResult UsdShadeShader_SetSourceCode(
    UsdShadeShaderRef shader,
    const char* code,
    TfTokenRef sourceType
);

/// Frees a string allocated by UsdShadeShader functions.
USD_INTEROP_API void UsdShadeShader_FreeString(char* str);

// MARK: - Shader Inputs/Outputs

/// Creates an input on the shader.
/// Caller must release with UsdShadeInput_Release.
USD_INTEROP_API UsdShadeInputRef UsdShadeShader_CreateInput(
    UsdShadeShaderRef shader,
    TfTokenRef name,
    TfTokenRef typeName
);

/// Gets an input by name.
/// Caller must release with UsdShadeInput_Release.
USD_INTEROP_API UsdShadeInputRef UsdShadeShader_GetInput(
    UsdShadeShaderRef shader,
    TfTokenRef name
);

/// Gets the count of inputs.
USD_INTEROP_API size_t UsdShadeShader_GetInputCount(
    UsdShadeShaderRef shader,
    bool onlyAuthored
);

/// Gets all inputs.
/// outInputs must have space for maxCount inputs.
/// Caller must release each with UsdShadeInput_Release.
USD_INTEROP_API size_t UsdShadeShader_GetInputs(
    UsdShadeShaderRef shader,
    UsdShadeInputRef* outInputs,
    size_t maxCount,
    bool onlyAuthored
);

/// Creates an output on the shader.
/// Caller must release with UsdShadeOutput_Release.
USD_INTEROP_API UsdShadeOutputRef UsdShadeShader_CreateOutput(
    UsdShadeShaderRef shader,
    TfTokenRef name,
    TfTokenRef typeName
);

/// Gets an output by name.
/// Caller must release with UsdShadeOutput_Release.
USD_INTEROP_API UsdShadeOutputRef UsdShadeShader_GetOutput(
    UsdShadeShaderRef shader,
    TfTokenRef name
);

/// Gets the count of outputs.
USD_INTEROP_API size_t UsdShadeShader_GetOutputCount(
    UsdShadeShaderRef shader,
    bool onlyAuthored
);

/// Gets all outputs.
/// outOutputs must have space for maxCount outputs.
/// Caller must release each with UsdShadeOutput_Release.
USD_INTEROP_API size_t UsdShadeShader_GetOutputs(
    UsdShadeShaderRef shader,
    UsdShadeOutputRef* outOutputs,
    size_t maxCount,
    bool onlyAuthored
);

// ============================================================================
// MARK: - UsdShadeNodeGraph
// ============================================================================

/// Gets a NodeGraph schema from a prim at the given path.
/// Caller must release with UsdShadeNodeGraph_Release.
USD_INTEROP_API UsdShadeNodeGraphRef UsdShadeNodeGraph_Get(UsdStageRef stage, SdfPathRef path);

/// Defines a new NodeGraph prim at the given path.
/// Caller must release with UsdShadeNodeGraph_Release.
USD_INTEROP_API UsdShadeNodeGraphRef UsdShadeNodeGraph_Define(UsdStageRef stage, SdfPathRef path);

/// Creates a NodeGraph schema from an existing prim.
/// Caller must release with UsdShadeNodeGraph_Release.
USD_INTEROP_API UsdShadeNodeGraphRef UsdShadeNodeGraph_FromPrim(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdShadeNodeGraphRef UsdShadeNodeGraph_Retain(UsdShadeNodeGraphRef nodeGraph);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdShadeNodeGraph_Release(UsdShadeNodeGraphRef nodeGraph);

/// Returns true if the node graph is valid.
USD_INTEROP_API bool UsdShadeNodeGraph_IsValid(UsdShadeNodeGraphRef nodeGraph);

/// Gets the underlying prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdShadeNodeGraph_GetPrim(UsdShadeNodeGraphRef nodeGraph);

/// Gets the path of the node graph.
/// Caller must release with SdfPath_Release.
USD_INTEROP_API SdfPathRef UsdShadeNodeGraph_GetPath(UsdShadeNodeGraphRef nodeGraph);

// MARK: - NodeGraph Inputs/Outputs

/// Creates an input on the node graph.
/// Caller must release with UsdShadeInput_Release.
USD_INTEROP_API UsdShadeInputRef UsdShadeNodeGraph_CreateInput(
    UsdShadeNodeGraphRef nodeGraph,
    TfTokenRef name,
    TfTokenRef typeName
);

/// Gets an input by name.
/// Caller must release with UsdShadeInput_Release.
USD_INTEROP_API UsdShadeInputRef UsdShadeNodeGraph_GetInput(
    UsdShadeNodeGraphRef nodeGraph,
    TfTokenRef name
);

/// Gets the count of inputs.
USD_INTEROP_API size_t UsdShadeNodeGraph_GetInputCount(
    UsdShadeNodeGraphRef nodeGraph,
    bool onlyAuthored
);

/// Gets all inputs.
/// outInputs must have space for maxCount inputs.
/// Caller must release each with UsdShadeInput_Release.
USD_INTEROP_API size_t UsdShadeNodeGraph_GetInputs(
    UsdShadeNodeGraphRef nodeGraph,
    UsdShadeInputRef* outInputs,
    size_t maxCount,
    bool onlyAuthored
);

/// Creates an output on the node graph.
/// Caller must release with UsdShadeOutput_Release.
USD_INTEROP_API UsdShadeOutputRef UsdShadeNodeGraph_CreateOutput(
    UsdShadeNodeGraphRef nodeGraph,
    TfTokenRef name,
    TfTokenRef typeName
);

/// Gets an output by name.
/// Caller must release with UsdShadeOutput_Release.
USD_INTEROP_API UsdShadeOutputRef UsdShadeNodeGraph_GetOutput(
    UsdShadeNodeGraphRef nodeGraph,
    TfTokenRef name
);

/// Gets the count of outputs.
USD_INTEROP_API size_t UsdShadeNodeGraph_GetOutputCount(
    UsdShadeNodeGraphRef nodeGraph,
    bool onlyAuthored
);

/// Gets all outputs.
/// outOutputs must have space for maxCount outputs.
/// Caller must release each with UsdShadeOutput_Release.
USD_INTEROP_API size_t UsdShadeNodeGraph_GetOutputs(
    UsdShadeNodeGraphRef nodeGraph,
    UsdShadeOutputRef* outOutputs,
    size_t maxCount,
    bool onlyAuthored
);

// MARK: - NodeGraph Computations

/// Computes outputs from interface inputs.
USD_INTEROP_API UsdResult UsdShadeNodeGraph_ComputeInterfaceInputConsumersMap(
    UsdShadeNodeGraphRef nodeGraph,
    bool computeTransitiveConsumers
);

// ============================================================================
// MARK: - UsdShadeInput
// ============================================================================

/// Creates an input from an attribute.
/// Caller must release with UsdShadeInput_Release.
USD_INTEROP_API UsdShadeInputRef UsdShadeInput_FromAttribute(UsdAttributeRef attr);

/// Increments reference count.
USD_INTEROP_API UsdShadeInputRef UsdShadeInput_Retain(UsdShadeInputRef input);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdShadeInput_Release(UsdShadeInputRef input);

/// Returns true if the input is valid.
USD_INTEROP_API bool UsdShadeInput_IsValid(UsdShadeInputRef input);

/// Returns true if the input is defined.
USD_INTEROP_API bool UsdShadeInput_IsDefined(UsdShadeInputRef input);

/// Gets the full name (with inputs: prefix).
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef UsdShadeInput_GetFullName(UsdShadeInputRef input);

/// Gets the base name (without inputs: prefix).
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef UsdShadeInput_GetBaseName(UsdShadeInputRef input);

/// Gets the type name.
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef UsdShadeInput_GetTypeName(UsdShadeInputRef input);

/// Gets the owning prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdShadeInput_GetPrim(UsdShadeInputRef input);

/// Gets the underlying attribute.
/// Caller must release with UsdAttribute_Release.
USD_INTEROP_API UsdAttributeRef UsdShadeInput_GetAttr(UsdShadeInputRef input);

// MARK: - Input Value Access

/// Gets a bool value.
USD_INTEROP_API UsdResult UsdShadeInput_GetBool(UsdShadeInputRef input, UsdTimeCode time, bool* value);

/// Sets a bool value.
USD_INTEROP_API UsdResult UsdShadeInput_SetBool(UsdShadeInputRef input, UsdTimeCode time, bool value);

/// Gets an int value.
USD_INTEROP_API UsdResult UsdShadeInput_GetInt(UsdShadeInputRef input, UsdTimeCode time, int* value);

/// Sets an int value.
USD_INTEROP_API UsdResult UsdShadeInput_SetInt(UsdShadeInputRef input, UsdTimeCode time, int value);

/// Gets a float value.
USD_INTEROP_API UsdResult UsdShadeInput_GetFloat(UsdShadeInputRef input, UsdTimeCode time, float* value);

/// Sets a float value.
USD_INTEROP_API UsdResult UsdShadeInput_SetFloat(UsdShadeInputRef input, UsdTimeCode time, float value);

/// Gets a Vec3f value.
USD_INTEROP_API UsdResult UsdShadeInput_GetVec3f(UsdShadeInputRef input, UsdTimeCode time, float* x, float* y, float* z);

/// Sets a Vec3f value.
USD_INTEROP_API UsdResult UsdShadeInput_SetVec3f(UsdShadeInputRef input, UsdTimeCode time, float x, float y, float z);

/// Gets a string value.
/// Caller must free with UsdShadeInput_FreeString.
USD_INTEROP_API UsdResult UsdShadeInput_GetString(UsdShadeInputRef input, UsdTimeCode time, char** value);

/// Sets a string value.
USD_INTEROP_API UsdResult UsdShadeInput_SetString(UsdShadeInputRef input, UsdTimeCode time, const char* value);

/// Gets a token value.
/// Caller must release with TfToken_Release.
USD_INTEROP_API UsdResult UsdShadeInput_GetToken(UsdShadeInputRef input, UsdTimeCode time, TfTokenRef* value);

/// Sets a token value.
USD_INTEROP_API UsdResult UsdShadeInput_SetToken(UsdShadeInputRef input, UsdTimeCode time, TfTokenRef value);

/// Gets an asset path value.
/// Caller must release with SdfAssetPath_Release.
USD_INTEROP_API UsdResult UsdShadeInput_GetAssetPath(UsdShadeInputRef input, UsdTimeCode time, SdfAssetPathRef* value);

/// Sets an asset path value.
USD_INTEROP_API UsdResult UsdShadeInput_SetAssetPath(UsdShadeInputRef input, UsdTimeCode time, SdfAssetPathRef value);

/// Frees a string allocated by UsdShadeInput functions.
USD_INTEROP_API void UsdShadeInput_FreeString(char* str);

// MARK: - Input Render Type

/// Gets the render type.
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef UsdShadeInput_GetRenderType(UsdShadeInputRef input);

/// Sets the render type.
USD_INTEROP_API UsdResult UsdShadeInput_SetRenderType(UsdShadeInputRef input, TfTokenRef renderType);

/// Returns true if the input has a render type.
USD_INTEROP_API bool UsdShadeInput_HasRenderType(UsdShadeInputRef input);

// MARK: - Input Connectability

/// Gets the connectability.
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef UsdShadeInput_GetConnectability(UsdShadeInputRef input);

/// Sets the connectability ("full" or "interfaceOnly").
USD_INTEROP_API UsdResult UsdShadeInput_SetConnectability(UsdShadeInputRef input, TfTokenRef connectability);

/// Clears the connectability.
USD_INTEROP_API UsdResult UsdShadeInput_ClearConnectability(UsdShadeInputRef input);

// MARK: - Input Connections

/// Returns true if the input can connect to the given source.
USD_INTEROP_API bool UsdShadeInput_CanConnect(UsdShadeInputRef input, UsdShadeOutputRef source);

/// Connects the input to a source output.
USD_INTEROP_API UsdResult UsdShadeInput_ConnectToOutput(
    UsdShadeInputRef input,
    UsdShadeOutputRef source,
    UsdShadeConnectionModification mod
);

/// Connects the input to a source input.
USD_INTEROP_API UsdResult UsdShadeInput_ConnectToInput(
    UsdShadeInputRef input,
    UsdShadeInputRef source,
    UsdShadeConnectionModification mod
);

/// Connects the input to a source by path.
USD_INTEROP_API UsdResult UsdShadeInput_ConnectToPath(
    UsdShadeInputRef input,
    SdfPathRef sourcePath,
    UsdShadeConnectionModification mod
);

/// Returns true if the input has a connected source.
USD_INTEROP_API bool UsdShadeInput_HasConnectedSource(UsdShadeInputRef input);

/// Returns true if the input's connection is from a base material.
USD_INTEROP_API bool UsdShadeInput_IsSourceConnectionFromBaseMaterial(UsdShadeInputRef input);

/// Gets the count of connected sources.
USD_INTEROP_API size_t UsdShadeInput_GetConnectedSourceCount(UsdShadeInputRef input);

/// Gets the connected source paths.
/// outPaths must have space for maxCount paths.
/// Caller must release each with SdfPath_Release.
USD_INTEROP_API size_t UsdShadeInput_GetConnectedSources(
    UsdShadeInputRef input,
    SdfPathRef* outPaths,
    size_t maxCount
);

/// Disconnects a specific source.
USD_INTEROP_API UsdResult UsdShadeInput_DisconnectSource(UsdShadeInputRef input, SdfPathRef sourcePath);

/// Clears all sources.
USD_INTEROP_API UsdResult UsdShadeInput_ClearSources(UsdShadeInputRef input);

// MARK: - Input Documentation

/// Gets the documentation.
/// Caller must free with UsdShadeInput_FreeString.
USD_INTEROP_API char* UsdShadeInput_GetDocumentation(UsdShadeInputRef input);

/// Sets the documentation.
USD_INTEROP_API UsdResult UsdShadeInput_SetDocumentation(UsdShadeInputRef input, const char* docs);

/// Gets the display group.
/// Caller must free with UsdShadeInput_FreeString.
USD_INTEROP_API char* UsdShadeInput_GetDisplayGroup(UsdShadeInputRef input);

/// Sets the display group.
USD_INTEROP_API UsdResult UsdShadeInput_SetDisplayGroup(UsdShadeInputRef input, const char* displayGroup);

// ============================================================================
// MARK: - UsdShadeOutput
// ============================================================================

/// Creates an output from an attribute.
/// Caller must release with UsdShadeOutput_Release.
USD_INTEROP_API UsdShadeOutputRef UsdShadeOutput_FromAttribute(UsdAttributeRef attr);

/// Increments reference count.
USD_INTEROP_API UsdShadeOutputRef UsdShadeOutput_Retain(UsdShadeOutputRef output);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdShadeOutput_Release(UsdShadeOutputRef output);

/// Returns true if the output is valid.
USD_INTEROP_API bool UsdShadeOutput_IsValid(UsdShadeOutputRef output);

/// Returns true if the output is defined.
USD_INTEROP_API bool UsdShadeOutput_IsDefined(UsdShadeOutputRef output);

/// Gets the full name (with outputs: prefix).
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef UsdShadeOutput_GetFullName(UsdShadeOutputRef output);

/// Gets the base name (without outputs: prefix).
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef UsdShadeOutput_GetBaseName(UsdShadeOutputRef output);

/// Gets the type name.
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef UsdShadeOutput_GetTypeName(UsdShadeOutputRef output);

/// Gets the owning prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdShadeOutput_GetPrim(UsdShadeOutputRef output);

/// Gets the underlying attribute.
/// Caller must release with UsdAttribute_Release.
USD_INTEROP_API UsdAttributeRef UsdShadeOutput_GetAttr(UsdShadeOutputRef output);

// MARK: - Output Render Type

/// Gets the render type.
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef UsdShadeOutput_GetRenderType(UsdShadeOutputRef output);

/// Sets the render type.
USD_INTEROP_API UsdResult UsdShadeOutput_SetRenderType(UsdShadeOutputRef output, TfTokenRef renderType);

/// Returns true if the output has a render type.
USD_INTEROP_API bool UsdShadeOutput_HasRenderType(UsdShadeOutputRef output);

// MARK: - Output Connections (for NodeGraphs)

/// Returns true if the output can connect to the given source.
USD_INTEROP_API bool UsdShadeOutput_CanConnect(UsdShadeOutputRef output, UsdShadeOutputRef source);

/// Connects the output to a source output.
USD_INTEROP_API UsdResult UsdShadeOutput_ConnectToOutput(
    UsdShadeOutputRef output,
    UsdShadeOutputRef source,
    UsdShadeConnectionModification mod
);

/// Connects the output to a source input.
USD_INTEROP_API UsdResult UsdShadeOutput_ConnectToInput(
    UsdShadeOutputRef output,
    UsdShadeInputRef source,
    UsdShadeConnectionModification mod
);

/// Connects the output to a source by path.
USD_INTEROP_API UsdResult UsdShadeOutput_ConnectToPath(
    UsdShadeOutputRef output,
    SdfPathRef sourcePath,
    UsdShadeConnectionModification mod
);

/// Returns true if the output has a connected source.
USD_INTEROP_API bool UsdShadeOutput_HasConnectedSource(UsdShadeOutputRef output);

/// Returns true if the output's connection is from a base material.
USD_INTEROP_API bool UsdShadeOutput_IsSourceConnectionFromBaseMaterial(UsdShadeOutputRef output);

/// Gets the count of connected sources.
USD_INTEROP_API size_t UsdShadeOutput_GetConnectedSourceCount(UsdShadeOutputRef output);

/// Gets the connected source paths.
/// outPaths must have space for maxCount paths.
/// Caller must release each with SdfPath_Release.
USD_INTEROP_API size_t UsdShadeOutput_GetConnectedSources(
    UsdShadeOutputRef output,
    SdfPathRef* outPaths,
    size_t maxCount
);

/// Disconnects a specific source.
USD_INTEROP_API UsdResult UsdShadeOutput_DisconnectSource(UsdShadeOutputRef output, SdfPathRef sourcePath);

/// Clears all sources.
USD_INTEROP_API UsdResult UsdShadeOutput_ClearSources(UsdShadeOutputRef output);

// ============================================================================
// MARK: - UsdShadeConnectableAPI
// ============================================================================

/// Gets a ConnectableAPI schema from a prim.
/// Caller must release with UsdShadeConnectableAPI_Release.
USD_INTEROP_API UsdShadeConnectableAPIRef UsdShadeConnectableAPI_Get(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdShadeConnectableAPIRef UsdShadeConnectableAPI_Retain(UsdShadeConnectableAPIRef api);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdShadeConnectableAPI_Release(UsdShadeConnectableAPIRef api);

/// Returns true if the API is valid.
USD_INTEROP_API bool UsdShadeConnectableAPI_IsValid(UsdShadeConnectableAPIRef api);

/// Returns true if the prim is a container (NodeGraph/Material).
USD_INTEROP_API bool UsdShadeConnectableAPI_IsContainer(UsdShadeConnectableAPIRef api);

/// Returns true if the prim requires encapsulation.
USD_INTEROP_API bool UsdShadeConnectableAPI_RequiresEncapsulation(UsdShadeConnectableAPIRef api);

/// Gets the underlying prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdShadeConnectableAPI_GetPrim(UsdShadeConnectableAPIRef api);

// MARK: - ConnectableAPI Static Connection Utilities

/// Checks if an input can connect to a source (static).
USD_INTEROP_API bool UsdShadeConnectableAPI_CanConnectInput(
    UsdShadeInputRef input,
    UsdShadeOutputRef source
);

/// Checks if an output can connect to a source (static).
USD_INTEROP_API bool UsdShadeConnectableAPI_CanConnectOutput(
    UsdShadeOutputRef output,
    UsdShadeOutputRef source
);

// ============================================================================
// MARK: - UsdShadeMaterialBindingAPI
// ============================================================================

/// Applies the MaterialBindingAPI to a prim.
/// Caller must release with UsdShadeMaterialBindingAPI_Release.
USD_INTEROP_API UsdShadeMaterialBindingAPIRef UsdShadeMaterialBindingAPI_Apply(UsdPrimRef prim);

/// Gets the MaterialBindingAPI from a prim if already applied.
/// Caller must release with UsdShadeMaterialBindingAPI_Release.
USD_INTEROP_API UsdShadeMaterialBindingAPIRef UsdShadeMaterialBindingAPI_Get(UsdPrimRef prim);

/// Increments reference count.
USD_INTEROP_API UsdShadeMaterialBindingAPIRef UsdShadeMaterialBindingAPI_Retain(UsdShadeMaterialBindingAPIRef api);

/// Decrements reference count and releases if zero.
USD_INTEROP_API void UsdShadeMaterialBindingAPI_Release(UsdShadeMaterialBindingAPIRef api);

/// Returns true if the API is valid.
USD_INTEROP_API bool UsdShadeMaterialBindingAPI_IsValid(UsdShadeMaterialBindingAPIRef api);

/// Gets the underlying prim.
/// Caller must release with UsdPrim_Release.
USD_INTEROP_API UsdPrimRef UsdShadeMaterialBindingAPI_GetPrim(UsdShadeMaterialBindingAPIRef api);

// MARK: - Direct Material Binding

/// Binds a material directly to this prim.
USD_INTEROP_API UsdResult UsdShadeMaterialBindingAPI_Bind(
    UsdShadeMaterialBindingAPIRef api,
    UsdShadeMaterialRef material,
    UsdShadeBindingStrength strength,
    UsdShadeMaterialPurpose purpose
);

/// Gets the directly bound material.
/// Caller must release with UsdShadeMaterial_Release.
USD_INTEROP_API UsdShadeMaterialRef UsdShadeMaterialBindingAPI_GetDirectlyBoundMaterial(
    UsdShadeMaterialBindingAPIRef api,
    UsdShadeMaterialPurpose purpose
);

/// Gets the path of the directly bound material.
/// Caller must release with SdfPath_Release.
USD_INTEROP_API SdfPathRef UsdShadeMaterialBindingAPI_GetDirectBindingPath(
    UsdShadeMaterialBindingAPIRef api,
    UsdShadeMaterialPurpose purpose
);

/// Unbinds the direct material binding.
USD_INTEROP_API UsdResult UsdShadeMaterialBindingAPI_UnbindDirectBinding(
    UsdShadeMaterialBindingAPIRef api,
    UsdShadeMaterialPurpose purpose
);

// MARK: - Computed Binding

/// Computes the bound material considering inheritance.
/// Caller must release with UsdShadeMaterial_Release.
USD_INTEROP_API UsdShadeMaterialRef UsdShadeMaterialBindingAPI_ComputeBoundMaterial(
    UsdShadeMaterialBindingAPIRef api,
    UsdShadeMaterialPurpose purpose
);

/// Computes the bound material path considering inheritance.
/// Caller must release with SdfPath_Release.
USD_INTEROP_API SdfPathRef UsdShadeMaterialBindingAPI_ComputeBoundMaterialPath(
    UsdShadeMaterialBindingAPIRef api,
    UsdShadeMaterialPurpose purpose
);

// MARK: - Binding Utilities

/// Gets the material binding strength of the direct binding relationship.
USD_INTEROP_API UsdShadeBindingStrength UsdShadeMaterialBindingAPI_GetDirectBindingStrength(
    UsdShadeMaterialBindingAPIRef api,
    UsdShadeMaterialPurpose purpose
);

/// Sets the material binding strength of the direct binding relationship.
USD_INTEROP_API UsdResult UsdShadeMaterialBindingAPI_SetDirectBindingStrength(
    UsdShadeMaterialBindingAPIRef api,
    UsdShadeMaterialPurpose purpose,
    UsdShadeBindingStrength strength
);

// ============================================================================
// MARK: - Utility Functions
// ============================================================================

/// Gets the input name prefix ("inputs:").
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef UsdShadeUtils_GetInputPrefix(void);

/// Gets the output name prefix ("outputs:").
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef UsdShadeUtils_GetOutputPrefix(void);

/// Returns true if the attribute name has the input prefix.
USD_INTEROP_API bool UsdShadeUtils_IsInputName(const char* name);

/// Returns true if the attribute name has the output prefix.
USD_INTEROP_API bool UsdShadeUtils_IsOutputName(const char* name);

/// Gets the base name from a full input/output name.
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef UsdShadeUtils_GetBaseNameFromFullName(const char* fullName);

/// Gets the attribute type from a full name.
USD_INTEROP_API UsdShadeAttributeType UsdShadeUtils_GetTypeFromFullName(const char* fullName);

/// Creates a full input name from a base name.
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef UsdShadeUtils_CreateInputName(TfTokenRef baseName);

/// Creates a full output name from a base name.
/// Caller must release with TfToken_Release.
USD_INTEROP_API TfTokenRef UsdShadeUtils_CreateOutputName(TfTokenRef baseName);

#ifdef __cplusplus
}
#endif

#endif // OPENUSDINTEROP_USDSHADE_USDSHADE_H
