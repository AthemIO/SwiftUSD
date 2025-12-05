/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import UsdLux

public typealias UsdLuxLightAPI = Pixar.UsdLuxLightAPI

public extension UsdLux
{
  typealias LuxLightAPI = UsdLuxLightAPI
}

public extension UsdLux.LuxLightAPI
{
  @discardableResult
  static func apply(_ prim: Usd.Prim) -> UsdLux.LuxLightAPI
  {
    UsdLux.LuxLightAPI.Apply(prim)
  }
}
