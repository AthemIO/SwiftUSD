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
  public func xformablePrim() -> Usd.Prim
  {
    // Use unsafeBitCast to work around Swift C++ interop not recognizing
    // that UsdLuxSphereLight inherits from UsdGeomXformable
    let xformable = unsafeBitCast(self, to: UsdGeomXformable.self)
    return Pixar.UsdGeomXformable_GetPrim(xformable)
  }

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
