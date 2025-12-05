/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import UsdLux

public typealias UsdLuxPortalLight = Pixar.UsdLuxPortalLight

public extension UsdLux
{
  typealias PortalLight = UsdLuxPortalLight
}

@Xformable
extension UsdLux.PortalLight: GeomXformable
{
  @discardableResult
  public static func define(_ stage: Usd.StageRefPtr, path: Sdf.Path) -> UsdLux.PortalLight
  {
    UsdLux.PortalLight.Define(stage.pointee.getPtr(), path)
  }

  @discardableResult
  public static func define(_ stage: Usd.StageRefPtr, path: String) -> UsdLux.PortalLight
  {
    UsdLux.PortalLight.define(stage, path: .init(path))
  }
}
