//
// Copyright 2025 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#ifndef PXR_EXEC_EXEC_SWIFT_BRIDGE_H
#define PXR_EXEC_EXEC_SWIFT_BRIDGE_H

/// \file Exec/swiftBridge.h
/// Swift-compatible bridge functions for Exec builtin computation tokens.
///
/// TfStaticData<> uses operator->() for access, which Swift C++ interop cannot
/// handle. These functions provide simple accessors that Swift can call.
///
/// The builtin computation tokens are essential for production use:
/// - computeTime: For time-dependent computations (returns EfTime)
/// - computeValue: For attribute value extraction (returns attribute's value type)

#include "pxr/pxr.h"

#include "Exec/api.h"
#include "Tf/token.h"
#include "Vt/value.h"
#include "Gf/matrix4d.h"
#include "Gf/matrix4f.h"
#include "Gf/vec2d.h"
#include "Gf/vec2f.h"
#include "Gf/vec3d.h"
#include "Gf/vec3f.h"
#include "Gf/vec4d.h"
#include "Gf/vec4f.h"
#include "Gf/quatd.h"
#include "Gf/quatf.h"

PXR_NAMESPACE_OPEN_SCOPE

/// @{
/// \name Exec Builtin Computation Token Swift Bridge Functions
///
/// Functions to access builtin computation tokens from Swift.
/// These work around Swift C++ interop limitations with TfStaticData.

/// Returns the computeTime token.
///
/// This token identifies the builtin computation that returns the current
/// time on the stage as an EfTime value. The computation provider must be
/// the stage.
///
/// Example usage from Swift:
/// ```swift
/// let timeKey = ExecUsd.ValueKey(
///     prim: stagePrim,
///     computation: Exec.Tokens.computeTime
/// )
/// ```
EXEC_API
TfToken Exec_Swift_GetComputeTimeToken();

/// Returns the computeValue token.
///
/// This token identifies the builtin computation that returns an attribute's
/// value. The return type matches the attribute's scalar value type.
/// The computation provider must be an attribute.
///
/// Example usage from Swift:
/// ```swift
/// let valueKey = ExecUsd.ValueKey(
///     attribute: myAttribute,
///     computation: Exec.Tokens.computeValue
/// )
/// ```
EXEC_API
TfToken Exec_Swift_GetComputeValueToken();

/// Returns all builtin computation tokens as a vector.
///
/// This provides access to all builtin computation tokens for iteration
/// or validation purposes.
EXEC_API
std::vector<TfToken> Exec_Swift_GetAllBuiltinComputationTokens();

/// @}

/// @{
/// \name Custom Computation Registration Swift Bridge Types
///
/// Types and functions for registering custom computations from Swift.
/// These bridge the gap between Swift closures and C++ ExecCallbackFn.

/// Opaque handle for a computation context.
/// This wraps VdfContext and provides access to inputs and outputs.
typedef void* Exec_Swift_ContextHandle;

/// C-style callback type for Swift computation implementations.
/// Parameters:
/// - context: Handle for accessing inputs and setting outputs
/// - userContext: User-provided closure context (passed through unchanged)
typedef void (*Exec_Swift_ComputationCallback)(
    Exec_Swift_ContextHandle context,
    void* userContext);

/// Dynamic traversal type for finding input providers.
/// Maps to ExecProviderResolution::DynamicTraversal enum.
enum Exec_Swift_DynamicTraversal {
    /// The localTraversal path directly indicates the provider.
    Exec_Swift_DynamicTraversal_Local = 0,
    /// Find providers by traversing relationship targets.
    Exec_Swift_DynamicTraversal_RelationshipTargetedObjects = 1,
    /// Find providers by traversing attribute connections.
    Exec_Swift_DynamicTraversal_ConnectionTargetedObjects = 2,
    /// Find the provider by traversing upward in namespace.
    Exec_Swift_DynamicTraversal_NamespaceAncestor = 3
};

/// Input specification for a custom computation.
/// Specifies how to find an input value for the computation.
struct Exec_Swift_InputSpec {
    /// The name used to access this input in the callback.
    const char* inputName;

    /// The computation name to request on the provider.
    const char* computationName;

    /// A token for distinguishing computations with the same name (usually empty).
    const char* disambiguatingId;

    /// The type name of the expected result (e.g., "double", "GfMatrix4d").
    const char* resultTypeName;

    /// A path relative to the owner describing initial traversal to provider.
    /// For attribute values, this is the attribute name (e.g., ".radius").
    /// For namespace ancestor, this can be empty.
    const char* localTraversalPath;

    /// How to dynamically traverse to find the provider.
    Exec_Swift_DynamicTraversal dynamicTraversal;

    /// Whether to fall back to dispatched computations if local not found.
    bool fallsBackToDispatched;

    /// Whether the input is optional (won't error if not found).
    bool optional;
};

/// Computation specification for registration.
struct Exec_Swift_ComputationSpec {
    /// The schema type name (e.g., "UsdGeomXformable", "UsdGeomSphere").
    const char* schemaTypeName;

    /// The computation name (e.g., "computeMyValue").
    const char* computationName;

    /// The result type name (e.g., "double", "GfMatrix4d").
    const char* resultTypeName;

    /// The Swift callback function.
    Exec_Swift_ComputationCallback callback;

    /// User context passed to callback.
    void* callbackContext;

    /// Array of input specifications.
    const Exec_Swift_InputSpec* inputs;

    /// Number of inputs in the array.
    size_t inputCount;

    /// True for prim computation, false for attribute computation.
    bool isPrimComputation;

    /// Attribute name (only used if isPrimComputation is false).
    const char* attributeName;
};

/// @}

/// @{
/// \name Custom Computation Registration Functions

/// Registers a custom computation with the Exec definition registry.
///
/// This function should be called after Pixar.Bundler.shared.setup(.resources)
/// but before creating any ExecUsd.System instances.
///
/// Returns true on success, false on failure (invalid schema type, etc.).
EXEC_API
bool Exec_Swift_RegisterComputation(const Exec_Swift_ComputationSpec* spec);

/// @}

/// @{
/// \name Computation Context Access Functions
///
/// Functions called from within Swift computation callbacks to access
/// input values and set the output.

/// Checks if an input has a value.
/// Returns true if the input named inputName has a valid value.
EXEC_API
bool Exec_Swift_Context_HasInputValue(
    Exec_Swift_ContextHandle context,
    const char* inputName);

/// Gets an input value as VtValue.
/// Returns an empty VtValue if the input doesn't exist.
EXEC_API
VtValue Exec_Swift_Context_GetInputValue(
    Exec_Swift_ContextHandle context,
    const char* inputName);

/// Sets the output value from a VtValue.
EXEC_API
void Exec_Swift_Context_SetOutput(
    Exec_Swift_ContextHandle context,
    const VtValue& value);

// Typed input getters (avoid VtValue overhead for common types)

EXEC_API
double Exec_Swift_Context_GetInputDouble(
    Exec_Swift_ContextHandle context,
    const char* inputName);

EXEC_API
float Exec_Swift_Context_GetInputFloat(
    Exec_Swift_ContextHandle context,
    const char* inputName);

EXEC_API
int Exec_Swift_Context_GetInputInt(
    Exec_Swift_ContextHandle context,
    const char* inputName);

EXEC_API
bool Exec_Swift_Context_GetInputBool(
    Exec_Swift_ContextHandle context,
    const char* inputName);

EXEC_API
GfMatrix4d Exec_Swift_Context_GetInputMatrix4d(
    Exec_Swift_ContextHandle context,
    const char* inputName);

EXEC_API
GfMatrix4f Exec_Swift_Context_GetInputMatrix4f(
    Exec_Swift_ContextHandle context,
    const char* inputName);

EXEC_API
GfVec2d Exec_Swift_Context_GetInputVec2d(
    Exec_Swift_ContextHandle context,
    const char* inputName);

EXEC_API
GfVec2f Exec_Swift_Context_GetInputVec2f(
    Exec_Swift_ContextHandle context,
    const char* inputName);

EXEC_API
GfVec3d Exec_Swift_Context_GetInputVec3d(
    Exec_Swift_ContextHandle context,
    const char* inputName);

EXEC_API
GfVec3f Exec_Swift_Context_GetInputVec3f(
    Exec_Swift_ContextHandle context,
    const char* inputName);

EXEC_API
GfVec4d Exec_Swift_Context_GetInputVec4d(
    Exec_Swift_ContextHandle context,
    const char* inputName);

EXEC_API
GfVec4f Exec_Swift_Context_GetInputVec4f(
    Exec_Swift_ContextHandle context,
    const char* inputName);

EXEC_API
GfQuatd Exec_Swift_Context_GetInputQuatd(
    Exec_Swift_ContextHandle context,
    const char* inputName);

EXEC_API
GfQuatf Exec_Swift_Context_GetInputQuatf(
    Exec_Swift_ContextHandle context,
    const char* inputName);

// Typed output setters (avoid VtValue overhead for common types)

EXEC_API
void Exec_Swift_Context_SetOutputDouble(
    Exec_Swift_ContextHandle context,
    double value);

EXEC_API
void Exec_Swift_Context_SetOutputFloat(
    Exec_Swift_ContextHandle context,
    float value);

EXEC_API
void Exec_Swift_Context_SetOutputInt(
    Exec_Swift_ContextHandle context,
    int value);

EXEC_API
void Exec_Swift_Context_SetOutputBool(
    Exec_Swift_ContextHandle context,
    bool value);

EXEC_API
void Exec_Swift_Context_SetOutputMatrix4d(
    Exec_Swift_ContextHandle context,
    const GfMatrix4d& value);

EXEC_API
void Exec_Swift_Context_SetOutputMatrix4f(
    Exec_Swift_ContextHandle context,
    const GfMatrix4f& value);

EXEC_API
void Exec_Swift_Context_SetOutputVec2d(
    Exec_Swift_ContextHandle context,
    const GfVec2d& value);

EXEC_API
void Exec_Swift_Context_SetOutputVec2f(
    Exec_Swift_ContextHandle context,
    const GfVec2f& value);

EXEC_API
void Exec_Swift_Context_SetOutputVec3d(
    Exec_Swift_ContextHandle context,
    const GfVec3d& value);

EXEC_API
void Exec_Swift_Context_SetOutputVec3f(
    Exec_Swift_ContextHandle context,
    const GfVec3f& value);

EXEC_API
void Exec_Swift_Context_SetOutputVec4d(
    Exec_Swift_ContextHandle context,
    const GfVec4d& value);

EXEC_API
void Exec_Swift_Context_SetOutputVec4f(
    Exec_Swift_ContextHandle context,
    const GfVec4f& value);

EXEC_API
void Exec_Swift_Context_SetOutputQuatd(
    Exec_Swift_ContextHandle context,
    const GfQuatd& value);

EXEC_API
void Exec_Swift_Context_SetOutputQuatf(
    Exec_Swift_ContextHandle context,
    const GfQuatf& value);

/// @}

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXR_EXEC_EXEC_SWIFT_BRIDGE_H
