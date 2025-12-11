/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import UsdGeom

public typealias UsdGeomXformable = Pixar.UsdGeomXformable

public extension UsdGeom
{
  typealias Xformable = UsdGeomXformable
}

@attached(member, names: arbitrary, conformances: GeomXformable)
public macro Xformable() = #externalMacro(module: "PixarMacros", type: "PixarXformableMacro")

public protocol GeomXformable
{
  /// Returns the underlying UsdPrim for this xformable type.
  /// Required to work around Swift C++ interop ambiguous method resolution.
  func xformablePrim() -> Usd.Prim

  func addXformOp(type: UsdGeomXformOp.`Type`,
                  precision: UsdGeomXformOp.Precision,
                  suffix: Tf.Token,
                  invert: Bool) -> UsdGeomXformOp

  func addTranslateOp(precision: UsdGeomXformOp.Precision,
                      suffix: Tf.Token,
                      invert: Bool) -> UsdGeomXformOp

  func addScaleOp(precision: UsdGeomXformOp.Precision,
                  suffix: Tf.Token,
                  invert: Bool) -> UsdGeomXformOp

  func addRotateXOp(precision: UsdGeomXformOp.Precision,
                    suffix: Tf.Token,
                    invert: Bool) -> UsdGeomXformOp

  func addRotateYOp(precision: UsdGeomXformOp.Precision,
                    suffix: Tf.Token,
                    invert: Bool) -> UsdGeomXformOp

  func addRotateZOp(precision: UsdGeomXformOp.Precision,
                    suffix: Tf.Token,
                    invert: Bool) -> UsdGeomXformOp

  func addRotateXYZOp(precision: UsdGeomXformOp.Precision,
                      suffix: Tf.Token,
                      invert: Bool) -> UsdGeomXformOp
}

@Xformable
extension UsdGeomXformable: GeomXformable
{
  public func xformablePrim() -> Usd.Prim
  {
    Pixar.UsdGeomXformable_GetPrim(self)
  }
}
