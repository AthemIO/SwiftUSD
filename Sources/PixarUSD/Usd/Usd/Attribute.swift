/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import CxxStdlib
import Usd

public typealias UsdAttribute = Pixar.UsdAttribute

public extension Usd
{
  typealias Attribute = UsdAttribute
}

public extension Usd.Attribute
{
  func set(doc: String)
  {
    SetDocumentation(std.string(doc))
  }

  @discardableResult
  func set(_ value: String, time: UsdTimeCode = UsdTimeCode.Default()) -> Bool
  {
    Set(std.string(value), time)
  }

  @discardableResult
  func set(_ value: Sdf.AssetPath, time: UsdTimeCode = UsdTimeCode.Default()) -> Bool
  {
    Set(value, time)
  }

  @discardableResult
  func set(_ value: Bool, time: UsdTimeCode = UsdTimeCode.Default()) -> Bool
  {
    Set(value, time)
  }

  @discardableResult
  func set(_ value: Int, time: UsdTimeCode = UsdTimeCode.Default()) -> Bool
  {
    Set(Int32(value), time)
  }

  @discardableResult
  func set(_ value: Float, time: UsdTimeCode = UsdTimeCode.Default()) -> Bool
  {
    Set(value, time)
  }

  @discardableResult
  func set(_ value: Double, time: UsdTimeCode = UsdTimeCode.Default()) -> Bool
  {
    Set(value, time)
  }

  @discardableResult
  func set(_ value: VtValue, time: UsdTimeCode = UsdTimeCode.Default()) -> Bool
  {
    Set(value, time)
  }

  @discardableResult
  func set(_ value: GfVec3f, time: UsdTimeCode = UsdTimeCode.Default()) -> Bool
  {
    Set(value, time)
  }

  @discardableResult
  func set(_ value: GfVec3d, time: UsdTimeCode = UsdTimeCode.Default()) -> Bool
  {
    Set(value, time)
  }
}
