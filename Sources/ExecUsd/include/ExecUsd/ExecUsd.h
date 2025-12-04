/* ----------------------------------------------------------------
 * :: :  M  E  T  A  V  E  R  S  E  :                            ::
 * ----------------------------------------------------------------
 * Licensed under the terms set forth in the LICENSE.txt file, this
 * file is available at https://openusd.org/license.
 *
 *                                        Copyright (C) 2016 Pixar.
 *         Copyright (C) 2024 Wabi Foundation. All Rights Reserved.
 * ----------------------------------------------------------------
 *  . x x x . o o o . x x x . : : : .    o  x  o    . : : : .
 * ---------------------------------------------------------------- */

#ifndef __PXR_EXEC_EXECUSD_H__
#define __PXR_EXEC_EXECUSD_H__

// ExecUsd (Execution System for USD) umbrella header for Swift module builds
// Primary entry point for OpenExec - built on Exec and EsfUsd
//
// NOTE: This umbrella is intentionally minimal for Swift C++ interop compatibility.
// The main ExecUsdSystem class contains std::unique_ptr members which crash Swift.
// We use bridge functions (swiftBridge.h) to provide Swift access instead.

// API
#include <ExecUsd/api.h>

// Swift bridge functions - provides access to ExecUsd types through opaque
// pointers and factory functions. This is the only safe way to access ExecUsd
// from Swift due to std::unique_ptr members that crash the Swift compiler.
#include <ExecUsd/swiftBridge.h>

// Note: valueKey.h is excluded from the umbrella because it causes Swift
// compiler crashes. Value keys are created via bridge functions instead.

// Note: The following headers are excluded from the umbrella because they
// cause Swift C++ interop issues (contain std::unique_ptr members or
// include headers that do):
// - cacheView.h: Contains Exec_CacheView which pulls in Vdf types that crash Swift
// - system.h: ExecUsdSystem has std::unique_ptr member, also includes Exec/system.h
// - request.h: ExecUsdRequest has std::unique_ptr member
// - requestImpl.h: Internal implementation with std::unique_ptr
// - visitValueKey.h: Uses internal variant access
// - pch.h: Precompiled header with <cmath> and other problematic includes

#endif  // __PXR_EXEC_EXECUSD_H__
