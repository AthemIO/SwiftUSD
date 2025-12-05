/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import UsdLux

public typealias UsdLuxRectLight = Pixar.UsdLuxRectLight

public extension UsdLux
{
  typealias RectLight = UsdLuxRectLight
}

@Xformable
extension UsdLux.RectLight: GeomXformable
{
  @discardableResult
  public static func define(_ stage: Usd.StageRefPtr, path: Sdf.Path) -> UsdLux.RectLight
  {
    UsdLux.RectLight.Define(stage.pointee.getPtr(), path)
  }

  @discardableResult
  public static func define(_ stage: Usd.StageRefPtr, path: String) -> UsdLux.RectLight
  {
    UsdLux.RectLight.define(stage, path: .init(path))
  }
}
