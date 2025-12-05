/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import UsdGeom

public typealias UsdGeomXform = Pixar.UsdGeomXform

public extension UsdGeom
{
  typealias Xform = UsdGeomXform
}

@Xformable
extension UsdGeom.Xform: GeomXformable
{
  @discardableResult
  public static func define(_ stage: Usd.StageRefPtr, path: Sdf.Path) -> UsdGeom.Xform
  {
    UsdGeom.Xform.Define(stage.pointee.getPtr(), path)
  }

  @discardableResult
  public static func define(_ stage: Usd.StageRefPtr, path: String) -> UsdGeom.Xform
  {
    UsdGeom.Xform.define(stage, path: .init(path))
  }
}
