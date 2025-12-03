/* ----------------------------------------------------------------
 * :: :  M  E  T  A  V  E  R  S  E  :                            ::
 * ----------------------------------------------------------------
 * This software is Licensed under the terms of the Apache License,
 * version 2.0 (the "Apache License") with the following additional
 * modification; you may not use this file except within compliance
 * of the Apache License and the following modification made to it.
 * Section 6. Trademarks. is deleted and replaced with:
 *
 * Trademarks. This License does not grant permission to use any of
 * its trade names, trademarks, service marks, or the product names
 * of this Licensor or its affiliates, except as required to comply
 * with Section 4(c.) of this License, and to reproduce the content
 * of the NOTICE file.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND without even an
 * implied warranty of MERCHANTABILITY, or FITNESS FOR A PARTICULAR
 * PURPOSE. See the Apache License for more details.
 *
 * You should have received a copy for this software license of the
 * Apache License along with this program; or, if not, please write
 * to the Free Software Foundation Inc., with the following address
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *         Copyright (C) 2024 Wabi Foundation. All Rights Reserved.
 * ----------------------------------------------------------------
 *  . x x x . o o o . x x x . : : : .    o  x  o    . : : : .
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
