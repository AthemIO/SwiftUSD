/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import UsdLux

public typealias UsdLuxDiskLight = Pixar.UsdLuxDiskLight

public extension UsdLux
{
  typealias DiskLight = UsdLuxDiskLight
}

@Xformable
extension UsdLux.DiskLight: GeomXformable
{
  @discardableResult
  public static func define(_ stage: UsdStageRefPtr, path: Sdf.Path) -> UsdLux.DiskLight
  {
    UsdLux.DiskLight.Define(stage.pointee.getPtr(), path)
  }

  @discardableResult
  public static func define(_ stage: UsdStageRefPtr, path: String) -> UsdLux.DiskLight
  {
    UsdLux.DiskLight.define(stage, path: .init(path))
  }
}
