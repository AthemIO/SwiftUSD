/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import UsdShade

public typealias UsdShadeOutput = Pixar.UsdShadeOutput

public extension UsdShade
{
  typealias Output = UsdShadeOutput
}

public extension UsdShade.Output
{
  @discardableResult
  func connectTo(source: UsdShade.ConnectableAPI,
                 at name: Tf.Token,
                 from sourceType: UsdShade.AttributeType = UsdShade.AttributeType.Output,
                 type: Sdf.ValueTypeName = Sdf.ValueTypeName()) -> Bool
  {
    ConnectToSource(source, name, sourceType, type)
  }

  @discardableResult
  func connectTo(source: UsdShade.ConnectableAPI,
                 at name: UsdShade.Tokens,
                 from sourceType: UsdShade.AttributeType = UsdShade.AttributeType.Output,
                 type: Sdf.ValueTypeName = Sdf.ValueTypeName()) -> Bool
  {
    ConnectToSource(source, name.getToken(), sourceType, type)
  }

  @discardableResult
  func connectTo(source: UsdShade.ConnectableAPI,
                 at name: String,
                 from sourceType: UsdShade.AttributeType = UsdShade.AttributeType.Output,
                 type: Sdf.ValueTypeName = Sdf.ValueTypeName()) -> Bool
  {
    ConnectToSource(source, Tf.Token(name), sourceType, type)
  }
}
