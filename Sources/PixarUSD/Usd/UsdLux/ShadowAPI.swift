/* ----------------------------------------------------------------
 * :: :  M  E  T  A  V  E  R  S  E  :                            ::
 * ----------------------------------------------------------------
 * Licensed under the terms set forth in the LICENSE.txt file, this
 * file is available at https://openusd.org/license.
 *
 *                                        Copyright (C) 2016 Pixar.
 *         Copyright (C) 2024 Wabi Foundation. All Rights Reserved.
 * ----------------------------------------------------------------
 *  . x x x . o o o . x x x . : : : .    o  x  o    . : : : .
 * ---------------------------------------------------------------- */

import UsdLux

public typealias UsdLuxShadowAPI = Pixar.UsdLuxShadowAPI

public extension UsdLux
{
  typealias ShadowAPI = UsdLuxShadowAPI
}

public extension UsdLux.ShadowAPI
{
  /// Applies the ShadowAPI schema to the given prim.
  @discardableResult
  static func apply(_ prim: Usd.Prim) -> UsdLux.ShadowAPI
  {
    UsdLux.ShadowAPI.Apply(prim)
  }

  /// Applies the ShadowAPI schema to the given distant light.
  @discardableResult
  static func apply(_ light: UsdLux.DistantLight) -> UsdLux.ShadowAPI
  {
    UsdLux.ShadowAPI.Apply(light.GetPrim())
  }

  /// Applies the ShadowAPI schema to the given dome light.
  @discardableResult
  static func apply(_ light: UsdLux.DomeLight) -> UsdLux.ShadowAPI
  {
    UsdLux.ShadowAPI.Apply(light.GetPrim())
  }

  /// Applies the ShadowAPI schema to the given sphere light.
  @discardableResult
  static func apply(_ light: UsdLux.SphereLight) -> UsdLux.ShadowAPI
  {
    UsdLux.ShadowAPI.Apply(light.GetPrim())
  }

  /// Applies the ShadowAPI schema to the given rect light.
  @discardableResult
  static func apply(_ light: UsdLux.RectLight) -> UsdLux.ShadowAPI
  {
    UsdLux.ShadowAPI.Apply(light.GetPrim())
  }

  /// Applies the ShadowAPI schema to the given disk light.
  @discardableResult
  static func apply(_ light: UsdLux.DiskLight) -> UsdLux.ShadowAPI
  {
    UsdLux.ShadowAPI.Apply(light.GetPrim())
  }

  /// Applies the ShadowAPI schema to the given cylinder light.
  @discardableResult
  static func apply(_ light: UsdLux.CylinderLight) -> UsdLux.ShadowAPI
  {
    UsdLux.ShadowAPI.Apply(light.GetPrim())
  }

  /// Creates the shadow:enable attribute with the given default value.
  @discardableResult
  func createShadowEnableAttr(_ defaultValue: Bool? = nil) -> Usd.Attribute
  {
    if let value = defaultValue {
      return CreateShadowEnableAttr(Vt.Value(value), false)
    }
    return CreateShadowEnableAttr(Vt.Value(), false)
  }

  /// Gets the shadow:enable attribute.
  func getShadowEnableAttr() -> Usd.Attribute
  {
    GetShadowEnableAttr()
  }

  /// Creates the shadow:color attribute with the given default value.
  @discardableResult
  func createShadowColorAttr(_ defaultValue: GfVec3f? = nil) -> Usd.Attribute
  {
    if let value = defaultValue {
      return CreateShadowColorAttr(Vt.Value(value), false)
    }
    return CreateShadowColorAttr(Vt.Value(), false)
  }

  /// Gets the shadow:color attribute.
  func getShadowColorAttr() -> Usd.Attribute
  {
    GetShadowColorAttr()
  }

  /// Creates the shadow:distance attribute with the given default value.
  @discardableResult
  func createShadowDistanceAttr(_ defaultValue: Float? = nil) -> Usd.Attribute
  {
    if let value = defaultValue {
      return CreateShadowDistanceAttr(Vt.Value(value), false)
    }
    return CreateShadowDistanceAttr(Vt.Value(), false)
  }

  /// Gets the shadow:distance attribute.
  func getShadowDistanceAttr() -> Usd.Attribute
  {
    GetShadowDistanceAttr()
  }

  /// Creates the shadow:falloff attribute with the given default value.
  @discardableResult
  func createShadowFalloffAttr(_ defaultValue: Float? = nil) -> Usd.Attribute
  {
    if let value = defaultValue {
      return CreateShadowFalloffAttr(Vt.Value(value), false)
    }
    return CreateShadowFalloffAttr(Vt.Value(), false)
  }

  /// Gets the shadow:falloff attribute.
  func getShadowFalloffAttr() -> Usd.Attribute
  {
    GetShadowFalloffAttr()
  }

  /// Creates the shadow:falloffGamma attribute with the given default value.
  @discardableResult
  func createShadowFalloffGammaAttr(_ defaultValue: Float? = nil) -> Usd.Attribute
  {
    if let value = defaultValue {
      return CreateShadowFalloffGammaAttr(Vt.Value(value), false)
    }
    return CreateShadowFalloffGammaAttr(Vt.Value(), false)
  }

  /// Gets the shadow:falloffGamma attribute.
  func getShadowFalloffGammaAttr() -> Usd.Attribute
  {
    GetShadowFalloffGammaAttr()
  }
}
