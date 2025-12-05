/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import UsdGeom

public typealias UsdGeomSphere = Pixar.UsdGeomSphere

public extension UsdGeom
{
  typealias Sphere = UsdGeomSphere
}

@Xformable
extension UsdGeom.Sphere: GeomXformable
{
  @discardableResult
  public static func define(_ stage: Usd.StageRefPtr, path: Sdf.Path) -> UsdGeom.Sphere
  {
    UsdGeom.Sphere.Define(stage.pointee.getPtr(), path)
  }

  @discardableResult
  public static func define(_ stage: Usd.StageRefPtr, path: String) -> UsdGeom.Sphere
  {
    UsdGeom.Sphere.define(stage, path: .init(path))
  }
}
