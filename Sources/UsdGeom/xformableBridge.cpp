// SPDX-License-Identifier: Apache-2.0
// Swift C++ Interop Bridge for UsdGeomXformable

#include "UsdGeom/xformableBridge.h"

PXR_NAMESPACE_OPEN_SCOPE

UsdPrim UsdGeomXformable_GetPrim(const UsdGeomXformable& xformable)
{
    return xformable.GetPrim();
}

UsdGeomXformOp UsdGeomXformable_AddXformOp(
    const UsdPrim& prim,
    UsdGeomXformOp::Type opType,
    UsdGeomXformOp::Precision precision,
    const TfToken& opSuffix,
    bool isInverseOp)
{
    UsdGeomXformable xformable(prim);
    return xformable.AddXformOp(opType, precision, opSuffix, isInverseOp);
}

UsdGeomXformOp UsdGeomXformable_AddTranslateOp(
    const UsdPrim& prim,
    UsdGeomXformOp::Precision precision,
    const TfToken& opSuffix,
    bool isInverseOp)
{
    UsdGeomXformable xformable(prim);
    return xformable.AddTranslateOp(precision, opSuffix, isInverseOp);
}

UsdGeomXformOp UsdGeomXformable_AddScaleOp(
    const UsdPrim& prim,
    UsdGeomXformOp::Precision precision,
    const TfToken& opSuffix,
    bool isInverseOp)
{
    UsdGeomXformable xformable(prim);
    return xformable.AddScaleOp(precision, opSuffix, isInverseOp);
}

UsdGeomXformOp UsdGeomXformable_AddRotateXOp(
    const UsdPrim& prim,
    UsdGeomXformOp::Precision precision,
    const TfToken& opSuffix,
    bool isInverseOp)
{
    UsdGeomXformable xformable(prim);
    return xformable.AddRotateXOp(precision, opSuffix, isInverseOp);
}

UsdGeomXformOp UsdGeomXformable_AddRotateYOp(
    const UsdPrim& prim,
    UsdGeomXformOp::Precision precision,
    const TfToken& opSuffix,
    bool isInverseOp)
{
    UsdGeomXformable xformable(prim);
    return xformable.AddRotateYOp(precision, opSuffix, isInverseOp);
}

UsdGeomXformOp UsdGeomXformable_AddRotateZOp(
    const UsdPrim& prim,
    UsdGeomXformOp::Precision precision,
    const TfToken& opSuffix,
    bool isInverseOp)
{
    UsdGeomXformable xformable(prim);
    return xformable.AddRotateZOp(precision, opSuffix, isInverseOp);
}

UsdGeomXformOp UsdGeomXformable_AddRotateXYZOp(
    const UsdPrim& prim,
    UsdGeomXformOp::Precision precision,
    const TfToken& opSuffix,
    bool isInverseOp)
{
    UsdGeomXformable xformable(prim);
    return xformable.AddRotateXYZOp(precision, opSuffix, isInverseOp);
}

PXR_NAMESPACE_CLOSE_SCOPE
