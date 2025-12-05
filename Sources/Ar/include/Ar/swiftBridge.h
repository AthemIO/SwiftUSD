//
// Copyright 2024 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// @WABI: Swift Bridge - Provides Swift-compatible wrappers for Ar types that
// cannot be directly imported to Swift due to RAII or other C++ patterns.
//

#ifndef PXR_USD_AR_SWIFT_BRIDGE_H
#define PXR_USD_AR_SWIFT_BRIDGE_H

#include "pxr/pxrns.h"
#include "Ar/api.h"
#include "Ar/resolvedPath.h"
#include "Ar/timestamp.h"
#include "Ar/resolver.h"

PXR_NAMESPACE_OPEN_SCOPE

// ============================================================================
// ArResolverScopedCache Swift Bridge
// ============================================================================

/// Begin a resolver cache scope.
/// Returns an opaque handle that must be passed to ArSwift_EndCacheScope.
/// The cache scope remains active until EndCacheScope is called.
AR_API
void* ArSwift_BeginCacheScope();

/// Begin a resolver cache scope that shares data with a parent scope.
/// The parentHandle must be a valid handle from ArSwift_BeginCacheScope.
AR_API
void* ArSwift_BeginCacheScopeWithParent(void* parentHandle);

/// End a resolver cache scope.
/// The handle must be from a previous call to ArSwift_BeginCacheScope.
/// After this call, the handle is invalid and must not be used.
AR_API
void ArSwift_EndCacheScope(void* handle);

// ============================================================================
// ArFilesystemAsset Swift Bridge Functions
// ============================================================================

/// Get the modification timestamp for a file.
AR_API
ArTimestamp ArSwift_GetFilesystemAssetModificationTimestamp(
    const ArResolvedPath& resolvedPath);

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXR_USD_AR_SWIFT_BRIDGE_H
