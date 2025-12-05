/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import UsdShade

public typealias UsdShadeMaterial = Pixar.UsdShadeMaterial

public extension UsdShade
{
  typealias Material = UsdShadeMaterial
}

public extension UsdShade.Material
{
  @discardableResult
  static func define(_ stage: Usd.StageRefPtr, path: Sdf.Path) -> UsdShade.Material
  {
    UsdShade.Material.Define(stage.pointee.getPtr(), path)
  }

  @discardableResult
  static func define(_ stage: Usd.StageRefPtr, path: String) -> UsdShade.Material
  {
    UsdShade.Material.define(stage, path: .init(path))
  }

  @discardableResult
  func createSurfaceOutput(renderContext: Tf.Token) -> UsdShade.Output
  {
    CreateSurfaceOutput(renderContext)
  }

  @discardableResult
  func createSurfaceOutput(renderContext: UsdShade.Tokens = .universalRenderContext) -> UsdShade.Output
  {
    createSurfaceOutput(renderContext: renderContext.getToken())
  }
}
