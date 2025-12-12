//
// Copyright 2025 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#ifndef PXR_EXEC_EXEC_USD_SWIFT_BRIDGE_H
#define PXR_EXEC_EXEC_USD_SWIFT_BRIDGE_H

/// \file ExecUsd/swiftBridge.h
/// Swift-compatible bridge functions for ExecUsd types.
///
/// This header provides C-style factory functions and helpers to work around
/// Swift C++ interop limitations with std::unique_ptr members and complex types.
///
/// IMPORTANT: Swift cannot see C++ types that contain std::unique_ptr members
/// or certain complex template types. Therefore, ALL ExecUsd types are exposed
/// as void* to Swift. The implementation casts these back to the correct types.
///
/// This header intentionally avoids including any ExecUsd type headers (system.h,
/// request.h, cacheView.h, valueKey.h) because they crash the Swift compiler.

#include "pxr/pxr.h"

#include "ExecUsd/api.h"

// Safe includes - these types are used in the interface but don't crash Swift
#include "Usd/stage.h"
#include "Usd/prim.h"
#include "Usd/attribute.h"
#include "Usd/timeCode.h"
#include "Tf/token.h"
#include "Vt/value.h"
#include "Gf/matrix4d.h"
#include "Gf/matrix4f.h"
#include "Gf/vec2f.h"
#include "Gf/vec2d.h"
#include "Gf/vec3f.h"
#include "Gf/vec3d.h"
#include "Gf/vec4f.h"
#include "Gf/vec4d.h"
#include "Gf/quatf.h"
#include "Gf/quatd.h"

PXR_NAMESPACE_OPEN_SCOPE

/// @{
/// \name ExecUsdSystem Swift Bridge Functions
///
/// Factory and helper functions for ExecUsdSystem that work with Swift.
/// Swift cannot directly construct types with std::unique_ptr members,
/// so we provide these bridge functions using opaque void* pointers.

/// Creates a new ExecUsdSystem from a UsdStage (const ref version).
/// Returns an opaque pointer that must be destroyed with ExecUsd_Swift_DestroySystem.
EXECUSD_API
void* ExecUsd_Swift_CreateSystem(const UsdStageConstRefPtr &stage);

/// Creates a new ExecUsdSystem from a UsdStage (non-const ref version).
/// Returns an opaque pointer that must be destroyed with ExecUsd_Swift_DestroySystem.
EXECUSD_API
void* ExecUsd_Swift_CreateSystemFromStage(const UsdStageRefPtr &stage);

/// Destroys an ExecUsdSystem created by ExecUsd_Swift_CreateSystem.
/// Takes an opaque void* pointer.
EXECUSD_API
void ExecUsd_Swift_DestroySystem(void *system);

/// Changes the time at which values are computed.
EXECUSD_API
void ExecUsd_Swift_ChangeTime(void *system, double time);

/// Changes the time at which values are computed using UsdTimeCode.
EXECUSD_API
void ExecUsd_Swift_ChangeTimeCode(void *system, UsdTimeCode time);

/// Changes the time at which values are computed using EfTime.
/// This allows specifying spline evaluation flags for advanced time control.
/// The system parameter is void* (actually ExecUsdSystem*).
/// The timeCodeValue is the numeric time value (ignored if isDefault is true).
/// The isDefault flag indicates if this is the default time code.
/// The splineFlags are application-specific spline evaluation flags.
EXECUSD_API
void ExecUsd_Swift_ChangeTimeWithFlags(
    void *system,
    double timeCodeValue,
    bool isDefault,
    uint8_t splineFlags);

/// @}

/// @{
/// \name ExecUsdValueKey Swift Bridge Functions
///
/// Factory functions for creating ExecUsdValueKey instances.
/// Value keys are returned as opaque void* pointers because ExecUsdValueKey
/// uses std::variant which can cause issues in Swift's C++ interop.

/// Creates a value key for an attribute's builtin computeValue computation.
/// Returns an opaque pointer that must be destroyed with ExecUsd_Swift_DestroyValueKey.
EXECUSD_API
void* ExecUsd_Swift_CreateValueKeyFromAttribute(const UsdAttribute &provider);

/// Creates a value key for an attribute computation.
/// Returns an opaque pointer that must be destroyed with ExecUsd_Swift_DestroyValueKey.
EXECUSD_API
void* ExecUsd_Swift_CreateValueKeyFromAttributeAndToken(
    const UsdAttribute &provider,
    const TfToken &computation);

/// Creates a value key for a prim computation.
/// Returns an opaque pointer that must be destroyed with ExecUsd_Swift_DestroyValueKey.
EXECUSD_API
void* ExecUsd_Swift_CreateValueKeyFromPrimAndToken(
    const UsdPrim &provider,
    const TfToken &computation);

/// Destroys a value key created by ExecUsd_Swift_CreateValueKey*.
EXECUSD_API
void ExecUsd_Swift_DestroyValueKey(void *valueKey);

/// @}

/// @{
/// \name ExecUsdRequest Swift Bridge Functions
///
/// Bridge functions for ExecUsdRequest. Since ExecUsdRequest is move-only
/// and contains std::unique_ptr, we need special handling with opaque pointers.

/// Opaque handle for ExecUsdRequest storage.
/// This wraps an ExecUsdRequest in a way that Swift can manage.
/// The request field is void* since ExecUsdRequest contains unique_ptr.
struct ExecUsd_Swift_RequestHandle {
    void *request;  // Actually ExecUsdRequest*, but void* for Swift
    bool isValid;
};

/// Builds a request from a system and a list of value keys.
/// Returns a handle that must be destroyed with ExecUsd_Swift_DestroyRequest.
/// The system parameter is void* (actually ExecUsdSystem*).
/// The valueKeyVector parameter is void* (actually std::vector<ExecUsdValueKey>*).
EXECUSD_API
ExecUsd_Swift_RequestHandle ExecUsd_Swift_BuildRequest(
    void *system,
    void *valueKeyVector);

/// Destroys a request handle.
EXECUSD_API
void ExecUsd_Swift_DestroyRequest(ExecUsd_Swift_RequestHandle *handle);

/// Returns true if the request is valid.
EXECUSD_API
bool ExecUsd_Swift_RequestIsValid(const ExecUsd_Swift_RequestHandle *handle);

/// Prepares a request for execution.
/// The system parameter is void* (actually ExecUsdSystem*).
EXECUSD_API
void ExecUsd_Swift_PrepareRequest(
    void *system,
    ExecUsd_Swift_RequestHandle *handle);

/// Computes a request and returns an opaque cache view handle.
/// The system parameter is void* (actually ExecUsdSystem*).
/// The returned cache view handle must not outlive the system or request.
/// Must be destroyed with ExecUsd_Swift_DestroyCacheView.
EXECUSD_API
void* ExecUsd_Swift_Compute(
    void *system,
    ExecUsd_Swift_RequestHandle *handle);

/// @}

/// @{
/// \name ExecUsdCacheView Swift Bridge Functions
///
/// Bridge functions for extracting values from cache views.
/// CacheView is handled as an opaque void* because it contains Exec_CacheView
/// which has dependencies that crash Swift.

/// Destroys a cache view created by ExecUsd_Swift_Compute.
EXECUSD_API
void ExecUsd_Swift_DestroyCacheView(void *cacheView);

/// Gets the computed value at the given index.
/// The cacheView parameter is void* (actually ExecUsdCacheView*).
EXECUSD_API
VtValue ExecUsd_Swift_CacheViewGet(void *cacheView, int index);

/// @}

/// @{
/// \name ExecUsdCacheView Typed Value Extraction
///
/// Typed extraction functions for common value types.
/// These provide direct access to typed values without going through VtValue,
/// improving performance and type safety in Swift code.

/// Gets the computed value as GfMatrix4d (for transforms).
/// Returns identity matrix if not a matrix type or index invalid.
EXECUSD_API
GfMatrix4d ExecUsd_Swift_CacheViewGetMatrix4d(void *cacheView, int index);

/// Gets the computed value as GfMatrix4f.
/// Returns identity matrix if not a matrix type or index invalid.
EXECUSD_API
GfMatrix4f ExecUsd_Swift_CacheViewGetMatrix4f(void *cacheView, int index);

/// Gets the computed value as double.
/// Returns 0.0 if not a double type or index invalid.
EXECUSD_API
double ExecUsd_Swift_CacheViewGetDouble(void *cacheView, int index);

/// Gets the computed value as float.
/// Returns 0.0f if not a float type or index invalid.
EXECUSD_API
float ExecUsd_Swift_CacheViewGetFloat(void *cacheView, int index);

/// Gets the computed value as int.
/// Returns 0 if not an int type or index invalid.
EXECUSD_API
int ExecUsd_Swift_CacheViewGetInt(void *cacheView, int index);

/// Gets the computed value as bool.
/// Returns false if not a bool type or index invalid.
EXECUSD_API
bool ExecUsd_Swift_CacheViewGetBool(void *cacheView, int index);

/// Gets the computed value as GfVec2f.
/// Returns zero vector if not a vec2f type or index invalid.
EXECUSD_API
GfVec2f ExecUsd_Swift_CacheViewGetVec2f(void *cacheView, int index);

/// Gets the computed value as GfVec2d.
/// Returns zero vector if not a vec2d type or index invalid.
EXECUSD_API
GfVec2d ExecUsd_Swift_CacheViewGetVec2d(void *cacheView, int index);

/// Gets the computed value as GfVec3f.
/// Returns zero vector if not a vec3f type or index invalid.
EXECUSD_API
GfVec3f ExecUsd_Swift_CacheViewGetVec3f(void *cacheView, int index);

/// Gets the computed value as GfVec3d.
/// Returns zero vector if not a vec3d type or index invalid.
EXECUSD_API
GfVec3d ExecUsd_Swift_CacheViewGetVec3d(void *cacheView, int index);

/// Gets the computed value as GfVec4f.
/// Returns zero vector if not a vec4f type or index invalid.
EXECUSD_API
GfVec4f ExecUsd_Swift_CacheViewGetVec4f(void *cacheView, int index);

/// Gets the computed value as GfVec4d.
/// Returns zero vector if not a vec4d type or index invalid.
EXECUSD_API
GfVec4d ExecUsd_Swift_CacheViewGetVec4d(void *cacheView, int index);

/// Gets the computed value as GfQuatf.
/// Returns identity quaternion if not a quatf type or index invalid.
EXECUSD_API
GfQuatf ExecUsd_Swift_CacheViewGetQuatf(void *cacheView, int index);

/// Gets the computed value as GfQuatd.
/// Returns identity quaternion if not a quatd type or index invalid.
EXECUSD_API
GfQuatd ExecUsd_Swift_CacheViewGetQuatd(void *cacheView, int index);

/// @}

/// @{
/// \name ExecUsdCacheView Type Checking
///
/// Functions to check the type of a cached value before extraction.
/// Useful for determining the correct typed getter to call.

/// Returns true if the value at index is a GfMatrix4d.
EXECUSD_API
bool ExecUsd_Swift_CacheViewIsHoldingMatrix4d(void *cacheView, int index);

/// Returns true if the value at index is a GfMatrix4f.
EXECUSD_API
bool ExecUsd_Swift_CacheViewIsHoldingMatrix4f(void *cacheView, int index);

/// Returns true if the value at index is a double.
EXECUSD_API
bool ExecUsd_Swift_CacheViewIsHoldingDouble(void *cacheView, int index);

/// Returns true if the value at index is a float.
EXECUSD_API
bool ExecUsd_Swift_CacheViewIsHoldingFloat(void *cacheView, int index);

/// Returns true if the value at index is an int.
EXECUSD_API
bool ExecUsd_Swift_CacheViewIsHoldingInt(void *cacheView, int index);

/// Returns true if the value at index is a bool.
EXECUSD_API
bool ExecUsd_Swift_CacheViewIsHoldingBool(void *cacheView, int index);

/// Returns true if the value at index is a GfVec3f.
EXECUSD_API
bool ExecUsd_Swift_CacheViewIsHoldingVec3f(void *cacheView, int index);

/// Returns true if the value at index is a GfVec3d.
EXECUSD_API
bool ExecUsd_Swift_CacheViewIsHoldingVec3d(void *cacheView, int index);

/// Returns the type name of the value at the given index.
/// Returns empty string if index invalid or value is empty.
EXECUSD_API
std::string ExecUsd_Swift_CacheViewGetTypeName(void *cacheView, int index);

/// @}

/// @{
/// \name Value Key Vector Helpers
///
/// Helpers for building vectors of value keys from Swift.
/// All pointers are opaque void* to avoid exposing ExecUsdValueKey to Swift.

/// Creates an empty vector of value keys.
/// Returns an opaque pointer that must be destroyed with ExecUsd_Swift_DestroyValueKeyVector.
EXECUSD_API
void* ExecUsd_Swift_CreateValueKeyVector();

/// Destroys a vector of value keys.
EXECUSD_API
void ExecUsd_Swift_DestroyValueKeyVector(void *vec);

/// Adds a value key to the vector.
/// The vec parameter is void* (actually std::vector<ExecUsdValueKey>*).
/// The key parameter is void* (actually ExecUsdValueKey*).
EXECUSD_API
void ExecUsd_Swift_ValueKeyVectorPush(void *vec, void *key);

/// Returns the size of the vector.
EXECUSD_API
size_t ExecUsd_Swift_ValueKeyVectorSize(void *vec);

/// Clears the vector.
EXECUSD_API
void ExecUsd_Swift_ValueKeyVectorClear(void *vec);

/// @}

/// @{
/// \name ExecSystem Diagnostics Swift Bridge Functions
///
/// Bridge functions for ExecSystem::Diagnostics utility class.
/// Provides diagnostic capabilities for debugging and profiling.

/// Creates a diagnostics object for the given system.
/// Returns an opaque pointer to ExecSystem::Diagnostics.
/// Must be destroyed with ExecUsd_Swift_DestroyDiagnostics.
/// The system parameter is void* (actually ExecUsdSystem*).
EXECUSD_API
void* ExecUsd_Swift_CreateDiagnostics(void *system);

/// Destroys a diagnostics object created by ExecUsd_Swift_CreateDiagnostics.
EXECUSD_API
void ExecUsd_Swift_DestroyDiagnostics(void *diagnostics);

/// Invalidates all internal state of the exec system, resetting it
/// to a state equivalent to when it was first constructed.
/// The diagnostics parameter is void* (actually ExecSystem::Diagnostics*).
EXECUSD_API
void ExecUsd_Swift_Diagnostics_InvalidateAll(void *diagnostics);

/// Produces a DOT graph of the currently compiled exec network
/// and writes its contents to the specified filename.
/// The diagnostics parameter is void* (actually ExecSystem::Diagnostics*).
/// The filename parameter is a null-terminated C string.
EXECUSD_API
void ExecUsd_Swift_Diagnostics_GraphNetwork(void *diagnostics, const char *filename);

/// @}

/// @{
/// \name Invalidation Callback Swift Bridge Functions
///
/// Bridge functions for invalidation callbacks. Swift cannot directly use
/// std::function, so we provide C-style function pointer types with context
/// parameters for closure state management.

/// C-style callback for computed value invalidation.
/// Parameters:
/// - context: User-provided context pointer (passed through unchanged)
/// - indices: Array of indices that became invalid
/// - indexCount: Number of elements in the indices array
/// - timeIntervalMin: Minimum time of the invalid interval (or -inf)
/// - timeIntervalMax: Maximum time of the invalid interval (or +inf)
/// - includesDefaultTime: True if the default time is part of the invalid interval
typedef void (*ExecUsd_Swift_ValueInvalidationCallback)(
    void *context,
    const int *indices,
    size_t indexCount,
    double timeIntervalMin,
    double timeIntervalMax,
    bool includesDefaultTime);

/// C-style callback for time change invalidation.
/// Parameters:
/// - context: User-provided context pointer (passed through unchanged)
/// - indices: Array of indices that became invalid due to time change
/// - indexCount: Number of elements in the indices array
typedef void (*ExecUsd_Swift_TimeChangeCallback)(
    void *context,
    const int *indices,
    size_t indexCount);

/// Builds a request with invalidation callbacks.
/// Returns a handle that must be destroyed with ExecUsd_Swift_DestroyRequest.
/// The system parameter is void* (actually ExecUsdSystem*).
/// The valueKeyVector parameter is void* (actually std::vector<ExecUsdValueKey>*).
/// The valueCallback is called when computed values become invalid.
/// The timeCallback is called when values become invalid due to time changes.
/// The context pointers are passed through to the callbacks unchanged.
/// Pass nullptr for callbacks you don't need.
EXECUSD_API
ExecUsd_Swift_RequestHandle ExecUsd_Swift_BuildRequestWithCallbacks(
    void *system,
    void *valueKeyVector,
    ExecUsd_Swift_ValueInvalidationCallback valueCallback,
    void *valueContext,
    ExecUsd_Swift_TimeChangeCallback timeCallback,
    void *timeContext);

/// @}

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXR_EXEC_EXEC_USD_SWIFT_BRIDGE_H
