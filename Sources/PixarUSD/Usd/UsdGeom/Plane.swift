/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import UsdGeom

public typealias UsdGeomPlane = Pixar.UsdGeomPlane

public extension UsdGeom
{
  typealias Plane = UsdGeomPlane
}

@Xformable
extension UsdGeom.Plane: GeomXformable
{
  @discardableResult
  public static func define(_ stage: Usd.StageRefPtr, path: Sdf.Path) -> UsdGeom.Plane
  {
    UsdGeom.Plane.Define(stage.pointee.getPtr(), path)
  }

  @discardableResult
  public static func define(_ stage: Usd.StageRefPtr, path: String) -> UsdGeom.Plane
  {
    UsdGeom.Plane.define(stage, path: .init(path))
  }
}
