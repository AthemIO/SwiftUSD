// material.h - UsdShadeMaterial C wrapper
// Mirrors: pxr/usdShade/material.h

#ifndef OPENUSDINTEROP_USDSHADE_MATERIAL_H
#define OPENUSDINTEROP_USDSHADE_MATERIAL_H

#include "../OpenUSDInterop.h"

#ifdef __cplusplus
extern "C" {
#endif

// MARK: - Opaque Handle Types

/// Opaque handle to a UsdShadeOutput.
typedef struct UsdShadeOutput_s* UsdShadeOutputRef;

/// Opaque handle to a UsdShadeInput.
typedef struct UsdShadeInput_s* UsdShadeInputRef;

// MARK: - Material Definition

/// Defines a new material at the given path.
USD_INTEROP_API UsdResult UsdShadeMaterial_Define(UsdStageRef stage, UsdSdfPathRef path, UsdPrimRef* outPrim);

/// Returns true if the prim is a material.
USD_INTEROP_API bool UsdShadeMaterial_IsMaterial(UsdPrimRef prim);

// MARK: - Material Outputs

/// Gets the surface output.
USD_INTEROP_API UsdResult UsdShadeMaterial_GetSurfaceOutput(UsdPrimRef prim, UsdShadeOutputRef* outOutput);

/// Creates the surface output.
USD_INTEROP_API UsdResult UsdShadeMaterial_CreateSurfaceOutput(UsdPrimRef prim, UsdShadeOutputRef* outOutput);

/// Gets the displacement output.
USD_INTEROP_API UsdResult UsdShadeMaterial_GetDisplacementOutput(UsdPrimRef prim, UsdShadeOutputRef* outOutput);

/// Creates the displacement output.
USD_INTEROP_API UsdResult UsdShadeMaterial_CreateDisplacementOutput(UsdPrimRef prim, UsdShadeOutputRef* outOutput);

/// Gets the volume output.
USD_INTEROP_API UsdResult UsdShadeMaterial_GetVolumeOutput(UsdPrimRef prim, UsdShadeOutputRef* outOutput);

/// Creates the volume output.
USD_INTEROP_API UsdResult UsdShadeMaterial_CreateVolumeOutput(UsdPrimRef prim, UsdShadeOutputRef* outOutput);

// MARK: - Material Binding

/// Binds a material to a prim.
USD_INTEROP_API UsdResult UsdShadeMaterial_Bind(UsdPrimRef material, UsdPrimRef target);

/// Unbinds any material from a prim.
USD_INTEROP_API UsdResult UsdShadeMaterial_Unbind(UsdPrimRef prim);

/// Gets the bound material for a prim.
USD_INTEROP_API UsdResult UsdShadeMaterial_GetBoundMaterial(UsdPrimRef prim, UsdPrimRef* outMaterial);

// MARK: - Shader Definition

/// Defines a new shader at the given path.
USD_INTEROP_API UsdResult UsdShadeShader_Define(UsdStageRef stage, UsdSdfPathRef path, UsdPrimRef* outPrim);

/// Returns true if the prim is a shader.
USD_INTEROP_API bool UsdShadeShader_IsShader(UsdPrimRef prim);

/// Gets the shader ID.
USD_INTEROP_API UsdResult UsdShadeShader_GetShaderId(UsdPrimRef prim, UsdTfTokenRef* outId);

/// Sets the shader ID.
USD_INTEROP_API UsdResult UsdShadeShader_SetShaderId(UsdPrimRef prim, UsdTfTokenRef id);

// MARK: - Shader Inputs

/// Creates an input on a shader.
USD_INTEROP_API UsdResult UsdShadeShader_CreateInput(
    UsdPrimRef prim,
    UsdTfTokenRef name,
    UsdTfTokenRef typeName,
    UsdShadeInputRef* outInput
);

/// Gets an input by name.
USD_INTEROP_API UsdResult UsdShadeShader_GetInput(UsdPrimRef prim, UsdTfTokenRef name, UsdShadeInputRef* outInput);

// MARK: - Shader Outputs

/// Creates an output on a shader.
USD_INTEROP_API UsdResult UsdShadeShader_CreateOutput(
    UsdPrimRef prim,
    UsdTfTokenRef name,
    UsdTfTokenRef typeName,
    UsdShadeOutputRef* outOutput
);

/// Gets an output by name.
USD_INTEROP_API UsdResult UsdShadeShader_GetOutput(UsdPrimRef prim, UsdTfTokenRef name, UsdShadeOutputRef* outOutput);

// MARK: - Input/Output Handle Management

/// Destroys an input handle.
USD_INTEROP_API void UsdShadeInput_Destroy(UsdShadeInputRef input);

/// Destroys an output handle.
USD_INTEROP_API void UsdShadeOutput_Destroy(UsdShadeOutputRef output);

/// Returns true if the input is valid.
USD_INTEROP_API bool UsdShadeInput_IsValid(UsdShadeInputRef input);

/// Returns true if the output is valid.
USD_INTEROP_API bool UsdShadeOutput_IsValid(UsdShadeOutputRef output);

// MARK: - Connections

/// Connects an input to a source output.
USD_INTEROP_API UsdResult UsdShadeInput_ConnectToOutput(UsdShadeInputRef input, UsdShadeOutputRef source);

/// Connects an output to a source output.
USD_INTEROP_API UsdResult UsdShadeOutput_ConnectToOutput(UsdShadeOutputRef output, UsdShadeOutputRef source);

#ifdef __cplusplus
}
#endif

#endif // OPENUSDINTEROP_USDSHADE_MATERIAL_H
