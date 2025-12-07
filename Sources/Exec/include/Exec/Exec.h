/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

#ifndef __PXR_EXEC_EXEC_H__
#define __PXR_EXEC_EXEC_H__

// Exec (Execution System Core) umbrella header for Swift module builds
//
// MINIMAL UMBRELLA: This module contains types with std::unique_ptr members
// that crash the Swift compiler. Only the absolute minimum is exposed here.
// Use bridge functions in ExecUsd/swiftBridge.h for Swift access.

#include <Exec/api.h>

// Note: Most headers in this module are excluded because they contain
// std::unique_ptr members or complex templates that crash Swift C++ interop.
// The public API is accessed via bridge functions.

#endif  // __PXR_EXEC_EXEC_H__
