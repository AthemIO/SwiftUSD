/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import UsdLux

public typealias UsdLuxDomeLight = Pixar.UsdLuxDomeLight

public extension UsdLux
{
  typealias DomeLight = UsdLuxDomeLight
}

@Xformable
extension UsdLux.DomeLight: GeomXformable
{
  public func xformablePrim() -> Usd.Prim
  {
    let xformable = unsafeBitCast(self, to: UsdGeomXformable.self)
    return Pixar.UsdGeomXformable_GetPrim(xformable)
  }

  @discardableResult
  public static func define(_ stage: Usd.StageRefPtr, path: Sdf.Path) -> UsdLux.DomeLight
  {
    UsdLux.DomeLight.Define(stage.pointee.getPtr(), path)
  }

  @discardableResult
  public static func define(_ stage: Usd.StageRefPtr, path: String) -> UsdLux.DomeLight
  {
    UsdLux.DomeLight.define(stage, path: .init(path))
  }

  public func getTextureFileAttr() -> Usd.Attribute
  {
    GetTextureFileAttr()
  }

  public func createTextureFileAttr(defaultValue: Vt.Value = Vt.Value(), writeSparsely: Bool = false) -> Usd.Attribute
  {
    CreateTextureFileAttr(defaultValue, writeSparsely)
  }
}
