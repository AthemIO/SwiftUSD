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

#ifndef __PXR_BASE_TF_H__
#define __PXR_BASE_TF_H__

// Tf umbrella header for Swift module builds
// Note: Include order is critical for proper template instantiation.

// Core foundational types (tfImpl contains TfAbs, TfMin, TfMax, TfDeleter, etc.)
#include <Tf/tfImpl.h>

// Core API and utilities
#include <Tf/api.h>
#include <Tf/hash.h>
#include <Tf/token.h>
#include <Tf/staticTokens.h>
#include <Tf/smallVector.h>
#include <Tf/span.h>
#include <Tf/bits.h>
#include <Tf/meta.h>
#include <Tf/iterator.h>
#include <Tf/stringUtils.h>
#include <Tf/atomicOfstreamWrapper.h>
#include <Tf/atomicRenameUtil.h>
#include <Tf/spinRWMutex.h>
#include <Tf/spinMutex.h>
#include <Tf/bigRWMutex.h>
#include <Tf/cxxCast.h>
#include <Tf/exception.h>

// Utility types
#include <Tf/bitUtils.h>
#include <Tf/compressedBits.h>
#include <Tf/callContext.h>
#include <Tf/dl.h>
#include <Tf/enum.h>
#include <Tf/envSetting.h>
#include <Tf/error.h>
#include <Tf/expiryNotifier.h>
#include <Tf/fileUtils.h>
#include <Tf/functionRef.h>
#include <Tf/functionTraits.h>
#include <Tf/getenv.h>
#include <Tf/setenv.h>

// Hash containers (must come before denseHashMap/denseHashSet)
#include <Tf/hashmap.h>
#include <Tf/hashset.h>
#include <Tf/denseHashMap.h>
#include <Tf/denseHashSet.h>

// Robin map hash containers (provides pxr_tsl::robin_map used by Sdf predicate library)
#include <Tf/pxrTslRobinMap/robin_map.h>
#include <Tf/pxrTslRobinMap/robin_set.h>

// Reference counting and smart pointer infrastructure
// Order is important: refBase -> refPtr -> weakBase -> weakPtrFacade -> weakPtr -> anyWeakPtr
#include <Tf/refBase.h>
#include <Tf/refCount.h>
#include <Tf/delegatedCountPtr.h>
#include <Tf/refPtrTracker.h>
#include <Tf/refPtr.h>
#include <Tf/weakBase.h>
#include <Tf/weakPtrFacade.h>
#include <Tf/weakPtr.h>
#include <Tf/declarePtrs.h>
#include <Tf/nullPtr.h>

// anyWeakPtr depends on weakPtr.h being fully processed, but must come before notice.h
#include <Tf/anyWeakPtr.h>

// anyUniquePtr for type-erased unique pointer storage
#include <Tf/anyUniquePtr.h>

// Type system and registry
#include <Tf/registryManager.h>
#include <Tf/type.h>
#include <Tf/typeFunctions.h>
#include <Tf/typeInfoMap.h>
#include <Tf/typeNotice.h>
#include <Tf/type_Impl.h>
#include <Tf/singleton.h>
// Note: instantiateSingleton.h is intentionally excluded - it can only be included
// once per .cpp file and is meant for singleton instantiation in implementation files
#include <Tf/instantiateType.h>

// Static data
#include <Tf/staticData.h>
#include <Tf/stl.h>

// Scoped utilities
#include <Tf/scoped.h>
#include <Tf/scopeDescription.h>

// Diagnostics
#include <Tf/debug.h>
#include <Tf/debugCodes.h>
#include <Tf/debugNotice.h>
#include <Tf/diagnosticBase.h>
#include <Tf/diagnostic.h>
#include <Tf/diagnosticHelper.h>
#include <Tf/diagnosticLite.h>
#include <Tf/diagnosticMgr.h>
#include <Tf/errorMark.h>
#include <Tf/errorTransport.h>
#include <Tf/mallocTag.h>
#include <Tf/status.h>
#include <Tf/warning.h>
#include <Tf/stackTrace.h>

// Output utilities
#include <Tf/ostreamMethods.h>
#include <Tf/fastCompression.h>
#include <Tf/safeOutputFile.h>
#include <Tf/safeTypeCompare.h>

// Notification system (depends on anyWeakPtr, weakPtr, type, diagnostic, mallocTag)
#include <Tf/notice.h>
#include <Tf/noticeRegistry.h>

// Pattern matching and paths
#include <Tf/pathUtils.h>
#include <Tf/patternMatcher.h>

// Preprocessor utilities
#include <Tf/preprocessorUtils.h>
#include <Tf/preprocessorUtilsLite.h>

// Other utilities
#include <Tf/pointerAndBits.h>
#include <Tf/regTest.h>
#include <Tf/stopwatch.h>
#include <Tf/unicodeUtils.h>

// Python tracing - provides TfPyTraceFnId (stub typedef when Python is disabled)
#include <Tf/pyTracing.h>

// Python-related headers - only include when Python support is enabled
#if PXR_PYTHON_SUPPORT_ENABLED
#include <Tf/pyObjWrapper.h>
#include <Tf/pyLock.h>
#endif // PXR_PYTHON_SUPPORT_ENABLED

#endif  // __PXR_BASE_TF_H__
