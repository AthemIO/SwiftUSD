/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import UsdLux

public typealias UsdLuxDistantLight = Pixar.UsdLuxDistantLight

public extension UsdLux
{
  typealias DistantLight = UsdLuxDistantLight
}

@Xformable
extension UsdLux.DistantLight: GeomXformable
{
  public func xformablePrim() -> Usd.Prim
  {
    let xformable = unsafeBitCast(self, to: UsdGeomXformable.self)
    return Pixar.UsdGeomXformable_GetPrim(xformable)
  }

  @discardableResult
  public static func define(_ stage: UsdStageRefPtr, path: Sdf.Path) -> UsdLux.DistantLight
  {
    UsdLux.DistantLight.Define(stage.pointee.getPtr(), path)
  }

  @discardableResult
  public static func define(_ stage: UsdStageRefPtr, path: String) -> UsdLux.DistantLight
  {
    UsdLux.DistantLight.define(stage, path: .init(path))
  }
}
