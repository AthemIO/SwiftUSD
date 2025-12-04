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

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXR_EXEC_EXEC_USD_SWIFT_BRIDGE_H
