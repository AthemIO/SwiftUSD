/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import UsdGeom

public typealias UsdGeomNurbsCurves = Pixar.UsdGeomNurbsCurves

public extension UsdGeom
{
  typealias NurbsCurves = UsdGeomNurbsCurves
}

public extension UsdGeom.NurbsCurves
{
  @discardableResult
  static func define(_ stage: Usd.StageRefPtr, path: Sdf.Path) -> UsdGeom.NurbsCurves
  {
    UsdGeom.NurbsCurves.Define(stage.pointee.getPtr(), path)
  }

  @discardableResult
  static func define(_ stage: Usd.StageRefPtr, path: String) -> UsdGeom.NurbsCurves
  {
    UsdGeom.NurbsCurves.define(stage, path: .init(path))
  }
}
