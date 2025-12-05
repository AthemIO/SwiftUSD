/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import UsdGeom

public typealias UsdGeomCamera = Pixar.UsdGeomCamera

public extension UsdGeom
{
  typealias Camera = UsdGeomCamera
}

@Xformable
extension UsdGeom.Camera: GeomXformable
{
  @discardableResult
  public static func define(_ stage: Usd.StageRefPtr, path: Sdf.Path) -> UsdGeom.Camera
  {
    UsdGeom.Camera.Define(stage.pointee.getPtr(), path)
  }

  @discardableResult
  public static func define(_ stage: Usd.StageRefPtr, path: String) -> UsdGeom.Camera
  {
    UsdGeom.Camera.define(stage, path: .init(path))
  }
}
