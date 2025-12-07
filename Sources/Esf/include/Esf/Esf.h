/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

#ifndef __PXR_EXEC_ESF_H__
#define __PXR_EXEC_ESF_H__

// Esf (Execution Scene Foundation) umbrella header for Swift module builds
// Provides interfaces for accessing scene description objects

// API
#include <Esf/api.h>

// Core scene objects
#include <Esf/object.h>
#include <Esf/stage.h>
#include <Esf/prim.h>
#include <Esf/property.h>
#include <Esf/attribute.h>
#include <Esf/attributeQuery.h>
#include <Esf/relationship.h>

// Edit tracking
#include <Esf/editReason.h>
#include <Esf/journal.h>

// Configuration
#include <Esf/schemaConfigKey.h>

// Utilities
#include <Esf/fixedSizePolymorphicHolder.h>

// Note: The following headers are excluded from the umbrella because they
// cause Swift C++ interop issues:
// - pch.h: Precompiled header with <cmath> and other problematic includes

#endif  // __PXR_EXEC_ESF_H__
