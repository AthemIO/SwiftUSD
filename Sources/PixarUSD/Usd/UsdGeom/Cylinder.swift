/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import UsdGeom

public typealias UsdGeomCylinder = Pixar.UsdGeomCylinder

public extension UsdGeom
{
  typealias Cylinder = UsdGeomCylinder
}

@Xformable
extension UsdGeom.Cylinder: GeomXformable
{
  @discardableResult
  public static func define(_ stage: Usd.StageRefPtr, path: Sdf.Path) -> UsdGeom.Cylinder
  {
    UsdGeom.Cylinder.Define(stage.pointee.getPtr(), path)
  }

  @discardableResult
  public static func define(_ stage: Usd.StageRefPtr, path: String) -> UsdGeom.Cylinder
  {
    UsdGeom.Cylinder.define(stage, path: .init(path))
  }
}
