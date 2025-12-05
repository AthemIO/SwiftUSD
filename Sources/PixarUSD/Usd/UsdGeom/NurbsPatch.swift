/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import UsdGeom

public typealias UsdGeomNurbsPatch = Pixar.UsdGeomNurbsPatch

public extension UsdGeom
{
  typealias NurbsPatch = UsdGeomNurbsPatch
}

public extension UsdGeom.NurbsPatch
{
  @discardableResult
  static func define(_ stage: Usd.StageRefPtr, path: Sdf.Path) -> UsdGeom.NurbsPatch
  {
    UsdGeom.NurbsPatch.Define(stage.pointee.getPtr(), path)
  }

  @discardableResult
  static func define(_ stage: Usd.StageRefPtr, path: String) -> UsdGeom.NurbsPatch
  {
    UsdGeom.NurbsPatch.define(stage, path: .init(path))
  }
}
