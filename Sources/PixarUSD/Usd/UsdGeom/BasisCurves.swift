/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import UsdGeom

public typealias UsdGeomBasisCurves = Pixar.UsdGeomBasisCurves

public extension UsdGeom
{
  typealias BasisCurves = UsdGeomBasisCurves
}

public extension UsdGeom.BasisCurves
{
  @discardableResult
  static func define(_ stage: Usd.StageRefPtr, path: Sdf.Path) -> UsdGeom.BasisCurves
  {
    UsdGeom.BasisCurves.Define(stage.pointee.getPtr(), path)
  }

  @discardableResult
  static func define(_ stage: Usd.StageRefPtr, path: String) -> UsdGeom.BasisCurves
  {
    UsdGeom.BasisCurves.define(stage, path: .init(path))
  }
}
