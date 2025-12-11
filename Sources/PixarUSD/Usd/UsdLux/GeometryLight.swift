/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import UsdLux

public typealias UsdLuxGeometryLight = Pixar.UsdLuxGeometryLight

public extension UsdLux
{
  typealias GeometryLight = UsdLuxGeometryLight
}

@Xformable
extension UsdLux.GeometryLight: GeomXformable
{
  public func xformablePrim() -> Usd.Prim
  {
    let xformable = unsafeBitCast(self, to: UsdGeomXformable.self)
    return Pixar.UsdGeomXformable_GetPrim(xformable)
  }

  @discardableResult
  public static func define(_ stage: Usd.StageRefPtr, path: Sdf.Path) -> UsdLux.GeometryLight
  {
    UsdLux.GeometryLight.Define(stage.pointee.getPtr(), path)
  }

  @discardableResult
  public static func define(_ stage: Usd.StageRefPtr, path: String) -> UsdLux.GeometryLight
  {
    UsdLux.GeometryLight.define(stage, path: .init(path))
  }
}
