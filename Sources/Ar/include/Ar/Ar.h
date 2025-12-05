//
// Copyright 2020 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#ifndef PXR_USD_AR_AR_H
#define PXR_USD_AR_AR_H

#define AR_VERSION 2

#endif
#ifndef __PXR_USD_AR_H__
#define __PXR_USD_AR_H__

// Ar - Base types first (order matters)
#include <Ar/api.h>
#include <Ar/timestamp.h>
#include <Ar/resolvedPath.h>
#include <Ar/assetInfo.h>
#include <Ar/asset.h>
#include <Ar/writableAsset.h>
#include <Ar/filesystemAsset.h>
#include <Ar/inMemoryAsset.h>

// Resolver context - must come before resolver and anything that uses context
#include <Ar/resolverContext.h>
#include <Ar/defineResolverContext.h>
#include <Ar/defaultResolverContext.h>

// Resolver types - depends on resolverContext
#include <Ar/resolver.h>
#include <Ar/resolverContextBinder.h>
#include <Ar/resolverScopedCache.h>
#include <Ar/threadLocalScopedCache.h>
#include <Ar/defineResolver.h>
#include <Ar/defaultResolver.h>

// Writable assets (depends on resolver)
#include <Ar/filesystemWritableAsset.h>

// Package resolver
#include <Ar/packageResolver.h>
#include <Ar/packageUtils.h>
#include <Ar/definePackageResolver.h>

// Notifications
#include <Ar/notice.h>

// Swift bridge (must come after all other Ar headers)
#include <Ar/swiftBridge.h>

#endif  // __PXR_USD_AR_H__
