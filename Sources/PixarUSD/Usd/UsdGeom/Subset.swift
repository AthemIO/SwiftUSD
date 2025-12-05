/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import UsdGeom

public typealias UsdGeomSubset = Pixar.UsdGeomSubset

public extension UsdGeom
{
  typealias Subset = UsdGeomSubset
}

public extension UsdGeom.Subset
{
  @discardableResult
  static func define(_ stage: Usd.StageRefPtr, path: Sdf.Path) -> UsdGeom.Subset
  {
    UsdGeom.Subset.Define(stage.pointee.getPtr(), path)
  }

  @discardableResult
  static func define(_ stage: Usd.StageRefPtr, path: String) -> UsdGeom.Subset
  {
    UsdGeom.Subset.define(stage, path: .init(path))
  }
}
