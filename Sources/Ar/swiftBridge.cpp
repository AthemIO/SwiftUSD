//
// Copyright 2024 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// @WABI: Swift Bridge Implementation

#include "Ar/swiftBridge.h"
#include "Ar/resolverScopedCache.h"
#include "Ar/filesystemAsset.h"
#include "Ar/resolver.h"

#include <memory>

PXR_NAMESPACE_OPEN_SCOPE

// Internal storage for cache scope objects
namespace {
    struct CacheScopeHandle {
        std::unique_ptr<ArResolverScopedCache> cache;
    };
}

void* ArSwift_BeginCacheScope()
{
    auto* handle = new CacheScopeHandle();
    handle->cache = std::make_unique<ArResolverScopedCache>();
    return handle;
}

void* ArSwift_BeginCacheScopeWithParent(void* parentHandle)
{
    if (!parentHandle) {
        return ArSwift_BeginCacheScope();
    }
    auto* parent = static_cast<CacheScopeHandle*>(parentHandle);
    auto* handle = new CacheScopeHandle();
    handle->cache = std::make_unique<ArResolverScopedCache>(parent->cache.get());
    return handle;
}

void ArSwift_EndCacheScope(void* handle)
{
    if (handle) {
        auto* scopeHandle = static_cast<CacheScopeHandle*>(handle);
        delete scopeHandle;  // Destructor ends the cache scope
    }
}

ArTimestamp ArSwift_GetFilesystemAssetModificationTimestamp(
    const ArResolvedPath& resolvedPath)
{
    return ArFilesystemAsset::GetModificationTimestamp(resolvedPath);
}

PXR_NAMESPACE_CLOSE_SCOPE
