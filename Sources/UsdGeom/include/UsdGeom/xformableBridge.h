// SPDX-License-Identifier: Apache-2.0
// Swift C++ Interop Bridge for UsdGeomXformable
// This bridge resolves ambiguous method overloads in Swift C++ interop

#ifndef INCLUDED_USDGEOM_XFORMABLE_BRIDGE_H
#define INCLUDED_USDGEOM_XFORMABLE_BRIDGE_H

#include "pxr/pxr.h"
#include "UsdGeom/api.h"
#include "UsdGeom/xformable.h"
#include "UsdGeom/xformOp.h"
#include "Usd/prim.h"
#include "Tf/token.h"

PXR_NAMESPACE_OPEN_SCOPE

// All bridge functions take UsdPrim to work around Swift C++ interop
// not recognizing C++ class inheritance hierarchies. The functions
// internally construct UsdGeomXformable from the prim.

/// Bridge function to get UsdPrim from UsdGeomXformable
/// Works around Swift C++ interop ambiguous method resolution for GetPrim()
USDGEOM_API
UsdPrim UsdGeomXformable_GetPrim(const UsdGeomXformable& xformable);

/// Bridge function for UsdGeomXformable::AddXformOp
/// Takes UsdPrim instead of UsdGeomXformable for Swift compatibility
USDGEOM_API
UsdGeomXformOp UsdGeomXformable_AddXformOp(
    const UsdPrim& prim,
    UsdGeomXformOp::Type opType,
    UsdGeomXformOp::Precision precision,
    const TfToken& opSuffix,
    bool isInverseOp);

/// Bridge function for UsdGeomXformable::AddTranslateOp
USDGEOM_API
UsdGeomXformOp UsdGeomXformable_AddTranslateOp(
    const UsdPrim& prim,
    UsdGeomXformOp::Precision precision,
    const TfToken& opSuffix,
    bool isInverseOp);

/// Bridge function for UsdGeomXformable::AddScaleOp
USDGEOM_API
UsdGeomXformOp UsdGeomXformable_AddScaleOp(
    const UsdPrim& prim,
    UsdGeomXformOp::Precision precision,
    const TfToken& opSuffix,
    bool isInverseOp);

/// Bridge function for UsdGeomXformable::AddRotateXOp
USDGEOM_API
UsdGeomXformOp UsdGeomXformable_AddRotateXOp(
    const UsdPrim& prim,
    UsdGeomXformOp::Precision precision,
    const TfToken& opSuffix,
    bool isInverseOp);

/// Bridge function for UsdGeomXformable::AddRotateYOp
USDGEOM_API
UsdGeomXformOp UsdGeomXformable_AddRotateYOp(
    const UsdPrim& prim,
    UsdGeomXformOp::Precision precision,
    const TfToken& opSuffix,
    bool isInverseOp);

/// Bridge function for UsdGeomXformable::AddRotateZOp
USDGEOM_API
UsdGeomXformOp UsdGeomXformable_AddRotateZOp(
    const UsdPrim& prim,
    UsdGeomXformOp::Precision precision,
    const TfToken& opSuffix,
    bool isInverseOp);

/// Bridge function for UsdGeomXformable::AddRotateXYZOp
USDGEOM_API
UsdGeomXformOp UsdGeomXformable_AddRotateXYZOp(
    const UsdPrim& prim,
    UsdGeomXformOp::Precision precision,
    const TfToken& opSuffix,
    bool isInverseOp);

PXR_NAMESPACE_CLOSE_SCOPE

#endif // INCLUDED_USDGEOM_XFORMABLE_BRIDGE_H
