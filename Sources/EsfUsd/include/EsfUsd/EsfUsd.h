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

#endif  // __PXR_EXEC_ESFUSD_H__
