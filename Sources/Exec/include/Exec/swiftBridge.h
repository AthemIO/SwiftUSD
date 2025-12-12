//
// Copyright 2025 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#ifndef PXR_EXEC_EXEC_SWIFT_BRIDGE_H
#define PXR_EXEC_EXEC_SWIFT_BRIDGE_H

/// \file Exec/swiftBridge.h
/// Swift-compatible bridge functions for Exec builtin computation tokens.
///
/// TfStaticData<> uses operator->() for access, which Swift C++ interop cannot
/// handle. These functions provide simple accessors that Swift can call.
///
/// The builtin computation tokens are essential for production use:
/// - computeTime: For time-dependent computations (returns EfTime)
/// - computeValue: For attribute value extraction (returns attribute's value type)

#include "pxr/pxr.h"

#include "Exec/api.h"
#include "Tf/token.h"

PXR_NAMESPACE_OPEN_SCOPE

/// @{
/// \name Exec Builtin Computation Token Swift Bridge Functions
///
/// Functions to access builtin computation tokens from Swift.
/// These work around Swift C++ interop limitations with TfStaticData.

/// Returns the computeTime token.
///
/// This token identifies the builtin computation that returns the current
/// time on the stage as an EfTime value. The computation provider must be
/// the stage.
///
/// Example usage from Swift:
/// ```swift
/// let timeKey = ExecUsd.ValueKey(
///     prim: stagePrim,
///     computation: Exec.Tokens.computeTime
/// )
/// ```
EXEC_API
TfToken Exec_Swift_GetComputeTimeToken();

/// Returns the computeValue token.
///
/// This token identifies the builtin computation that returns an attribute's
/// value. The return type matches the attribute's scalar value type.
/// The computation provider must be an attribute.
///
/// Example usage from Swift:
/// ```swift
/// let valueKey = ExecUsd.ValueKey(
///     attribute: myAttribute,
///     computation: Exec.Tokens.computeValue
/// )
/// ```
EXEC_API
TfToken Exec_Swift_GetComputeValueToken();

/// Returns all builtin computation tokens as a vector.
///
/// This provides access to all builtin computation tokens for iteration
/// or validation purposes.
EXEC_API
std::vector<TfToken> Exec_Swift_GetAllBuiltinComputationTokens();

/// @}

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXR_EXEC_EXEC_SWIFT_BRIDGE_H
