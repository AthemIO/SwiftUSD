/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

#ifndef __PXR_EXEC_EF_H__
#define __PXR_EXEC_EF_H__

// Ef (Execution Foundation) umbrella header for Swift module builds
// Built on top of Vdf, provides VdfNode types and executor interfaces

// API and core types
#include <Ef/api.h>

// Time handling
#include <Ef/time.h>
#include <Ef/timeInterval.h>
#include <Ef/timeInputNode.h>

// Leaf nodes
#include <Ef/leafNode.h>
#include <Ef/leafNodeCache.h>
#include <Ef/leafNodeIndexer.h>

// Executors
#include <Ef/executor.h>
#include <Ef/subExecutor.h>
#include <Ef/maskedSubExecutor.h>

// Page cache execution
#include <Ef/pageCache.h>
#include <Ef/pageCacheBasedExecutor.h>
#include <Ef/pageCacheCommitRequest.h>
#include <Ef/pageCacheExecutor.h>
#include <Ef/pageCacheStorage.h>
#include <Ef/pageCacheSubExecutor.h>

// Caching and dependencies
#include <Ef/dependencyCache.h>
#include <Ef/inputValueBlock.h>
#include <Ef/outputValueCache.h>
#include <Ef/loftedOutputSet.h>

// Utilities
#include <Ef/firstValidInputValue.h>
#include <Ef/vectorKey.h>

#endif  // __PXR_EXEC_EF_H__
