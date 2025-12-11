/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import UsdGeom

public typealias UsdGeomCapsule = Pixar.UsdGeomCapsule

public extension UsdGeom
{
  typealias Capsule = UsdGeomCapsule
}

@Xformable
extension UsdGeom.Capsule: GeomXformable
{
  public func xformablePrim() -> Usd.Prim
  {
    let xformable = unsafeBitCast(self, to: UsdGeomXformable.self)
    return Pixar.UsdGeomXformable_GetPrim(xformable)
  }

  @discardableResult
  public static func define(_ stage: UsdStageRefPtr, path: Sdf.Path) -> UsdGeom.Capsule
  {
    UsdGeom.Capsule.Define(stage.pointee.getPtr(), path)
  }

  @discardableResult
  public static func define(_ stage: UsdStageRefPtr, path: String) -> UsdGeom.Capsule
  {
    UsdGeom.Capsule.define(stage, path: .init(path))
  }
}
