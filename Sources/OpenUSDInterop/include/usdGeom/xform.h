// xform.h - UsdGeomXform C wrapper
// Mirrors: pxr/usdGeom/xform.h

#ifndef OPENUSDINTEROP_USDGEOM_XFORM_H
#define OPENUSDINTEROP_USDGEOM_XFORM_H

#include "../OpenUSDInterop.h"
#include "../base/gf.h"

#ifdef __cplusplus
extern "C" {
#endif

// MARK: - Xform Definition

/// Defines a new Xform at the given path.
USD_INTEROP_API UsdResult UsdGeomXform_Define(UsdStageRef stage, UsdSdfPathRef path, UsdPrimRef* outPrim);

/// Returns true if the prim is an Xform.
USD_INTEROP_API bool UsdGeomXform_IsXform(UsdPrimRef prim);

// MARK: - Transform Operations

/// XformOp type enum.
typedef enum {
    USD_XFORM_OP_TRANSLATE,
    USD_XFORM_OP_SCALE,
    USD_XFORM_OP_ROTATE_X,
    USD_XFORM_OP_ROTATE_Y,
    USD_XFORM_OP_ROTATE_Z,
    USD_XFORM_OP_ROTATE_XYZ,
    USD_XFORM_OP_ROTATE_XZY,
    USD_XFORM_OP_ROTATE_YXZ,
    USD_XFORM_OP_ROTATE_YZX,
    USD_XFORM_OP_ROTATE_ZXY,
    USD_XFORM_OP_ROTATE_ZYX,
    USD_XFORM_OP_ORIENT,
    USD_XFORM_OP_TRANSFORM
} UsdXformOpType;

/// Opaque handle to an XformOp.
typedef struct UsdXformOp_s* UsdXformOpRef;

/// Adds a translate operation.
USD_INTEROP_API UsdResult UsdGeomXform_AddTranslateOp(UsdPrimRef prim, UsdXformOpRef* outOp);

/// Adds a rotate XYZ operation.
USD_INTEROP_API UsdResult UsdGeomXform_AddRotateXYZOp(UsdPrimRef prim, UsdXformOpRef* outOp);

/// Adds a scale operation.
USD_INTEROP_API UsdResult UsdGeomXform_AddScaleOp(UsdPrimRef prim, UsdXformOpRef* outOp);

/// Adds a 4x4 transform operation.
USD_INTEROP_API UsdResult UsdGeomXform_AddTransformOp(UsdPrimRef prim, UsdXformOpRef* outOp);

/// Gets the ordered xform operations.
USD_INTEROP_API UsdResult UsdGeomXform_GetOrderedXformOps(
    UsdPrimRef prim,
    UsdXformOpRef* outOps,
    size_t* outCount,
    bool* outResetXformStack
);

/// Destroys an xform op handle.
USD_INTEROP_API void UsdXformOp_Destroy(UsdXformOpRef op);

// MARK: - XformOp Value Access

/// Gets the type of the xform op.
USD_INTEROP_API UsdXformOpType UsdXformOp_GetType(UsdXformOpRef op);

/// Gets the underlying attribute.
USD_INTEROP_API UsdResult UsdXformOp_GetAttr(UsdXformOpRef op, UsdAttributeRef* outAttr);

/// Sets a Vec3d value on the op.
USD_INTEROP_API UsdResult UsdXformOp_SetVec3d(UsdXformOpRef op, UsdTimeCode time, UsdGfVec3d value);

/// Gets a Vec3d value from the op.
USD_INTEROP_API UsdResult UsdXformOp_GetVec3d(UsdXformOpRef op, UsdTimeCode time, UsdGfVec3d* outValue);

/// Sets a Matrix4d value on the op.
USD_INTEROP_API UsdResult UsdXformOp_SetMatrix4d(UsdXformOpRef op, UsdTimeCode time, UsdGfMatrix4d value);

/// Gets a Matrix4d value from the op.
USD_INTEROP_API UsdResult UsdXformOp_GetMatrix4d(UsdXformOpRef op, UsdTimeCode time, UsdGfMatrix4d* outValue);

// MARK: - Local Transform

/// Gets the local transformation matrix.
USD_INTEROP_API UsdResult UsdGeomXform_GetLocalTransformation(
    UsdPrimRef prim,
    UsdTimeCode time,
    UsdGfMatrix4d* outMatrix,
    bool* outResetXformStack
);

/// Sets the reset xform stack flag.
USD_INTEROP_API UsdResult UsdGeomXform_SetResetXformStack(UsdPrimRef prim, bool resetXformStack);

/// Gets the reset xform stack flag.
USD_INTEROP_API bool UsdGeomXform_GetResetXformStack(UsdPrimRef prim);

// MARK: - World Transform

/// Computes the world transformation matrix.
USD_INTEROP_API UsdResult UsdGeomXform_ComputeWorldTransform(
    UsdPrimRef prim,
    UsdTimeCode time,
    UsdGfMatrix4d* outMatrix
);

#ifdef __cplusplus
}
#endif

#endif // OPENUSDINTEROP_USDGEOM_XFORM_H
