//
// Copyright 2025 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#ifndef PXR_EXEC_EXEC_GEOM_SWIFT_BRIDGE_H
#define PXR_EXEC_EXEC_GEOM_SWIFT_BRIDGE_H

/// \file ExecGeom/swiftBridge.h
/// Swift-compatible bridge functions for ExecGeom tokens.
///
/// TF_DECLARE_PUBLIC_TOKENS creates TfStaticData<> pointers that use
/// operator->() for access, which Swift C++ interop cannot handle.
/// These inline functions provide simple accessors that Swift can call.

#include "pxr/pxr.h"

#include "ExecGeom/api.h"
#include "ExecGeom/tokens.h"

PXR_NAMESPACE_OPEN_SCOPE

/// @{
/// \name ExecGeom Token Swift Bridge Functions
///
/// Inline functions to access ExecGeom tokens from Swift.
/// These work around Swift C++ interop limitations with TfStaticData.

/// Returns the computeLocalToWorldTransform token.
inline TfToken
ExecGeom_Swift_GetComputeLocalToWorldTransformToken()
{
    return ExecGeomXformableTokens->computeLocalToWorldTransform;
}

/// @}

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXR_EXEC_EXEC_GEOM_SWIFT_BRIDGE_H
