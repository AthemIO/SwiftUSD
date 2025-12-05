/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import UsdLux

public typealias UsdLuxMeshLightAPI = Pixar.UsdLuxMeshLightAPI

public extension UsdLux
{
  typealias MeshLightAPI = UsdLuxMeshLightAPI
}

extension UsdLux.MeshLightAPI
{
  @discardableResult
  static func apply(_ prim: Usd.Prim) -> UsdLux.MeshLightAPI
  {
    UsdLux.MeshLightAPI.Apply(prim)
  }
}
