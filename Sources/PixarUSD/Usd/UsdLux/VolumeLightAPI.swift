/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import UsdLux

public typealias UsdLuxVolumeLightAPI = Pixar.UsdLuxVolumeLightAPI

public extension UsdLux
{
  typealias VolumeLightAPI = UsdLuxVolumeLightAPI
}

public extension UsdLux.VolumeLightAPI
{
  @discardableResult
  static func apply(_ prim: Usd.Prim) -> UsdLux.VolumeLightAPI
  {
    UsdLux.VolumeLightAPI.Apply(prim)
  }
}
