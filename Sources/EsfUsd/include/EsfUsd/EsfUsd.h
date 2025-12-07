/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

#ifndef __PXR_EXEC_ESFUSD_H__
#define __PXR_EXEC_ESFUSD_H__

// EsfUsd (Execution Scene Foundation for USD) umbrella header for Swift module builds
// Provides interfaces for accessing UsdStage scene objects

// API
#include <EsfUsd/api.h>

// Scene adapter (primary public interface)
#include <EsfUsd/sceneAdapter.h>

// USD scene object implementations
#include <EsfUsd/stage.h>
#include <EsfUsd/prim.h>
#include <EsfUsd/object.h>
#include <EsfUsd/property.h>
#include <EsfUsd/attribute.h>
#include <EsfUsd/attributeQuery.h>
#include <EsfUsd/relationship.h>

// Note: The following headers are excluded from the umbrella because they
// cause Swift C++ interop issues:
// - pch.h: Precompiled header with <cmath> and other problematic includes

#endif  // __PXR_EXEC_ESFUSD_H__
