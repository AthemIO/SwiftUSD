/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import UsdLux

public typealias UsdLuxCylinderLight = Pixar.UsdLuxCylinderLight

public extension UsdLux
{
  typealias CylinderLight = UsdLuxCylinderLight
}

@Xformable
extension UsdLux.CylinderLight: GeomXformable
{
  @discardableResult
  public static func define(_ stage: UsdStageRefPtr, path: Sdf.Path) -> UsdLux.CylinderLight
  {
    UsdLux.CylinderLight.Define(stage.pointee.getPtr(), path)
  }

  @discardableResult
  public static func define(_ stage: UsdStageRefPtr, path: String) -> UsdLux.CylinderLight
  {
    UsdLux.CylinderLight.define(stage, path: .init(path))
  }
}
