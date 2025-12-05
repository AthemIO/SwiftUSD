/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import UsdLux

public typealias UsdLuxSphereLight = Pixar.UsdLuxSphereLight

public extension UsdLux
{
  typealias SphereLight = UsdLuxSphereLight
}

@Xformable
extension UsdLux.SphereLight: GeomXformable
{
  @discardableResult
  public static func define(_ stage: Usd.StageRefPtr, path: Sdf.Path) -> UsdLux.SphereLight
  {
    UsdLux.SphereLight.Define(stage.pointee.getPtr(), path)
  }

  @discardableResult
  public static func define(_ stage: Usd.StageRefPtr, path: String) -> UsdLux.SphereLight
  {
    UsdLux.SphereLight.define(stage, path: .init(path))
  }
}
