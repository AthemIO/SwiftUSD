/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import UsdGeom

public typealias UsdGeomPrimvarsAPI = Pixar.UsdGeomPrimvarsAPI
public typealias UsdGeomPrimvar = Pixar.UsdGeomPrimvar

public extension UsdGeom
{
  typealias PrimvarsAPI = UsdGeomPrimvarsAPI
  typealias Primvar = UsdGeomPrimvar
}

public extension UsdGeom.PrimvarsAPI
{
  /// Create a PrimvarsAPI wrapper for the given prim
  @inline(__always)
  static func apply(_ prim: Usd.Prim) -> UsdGeom.PrimvarsAPI
  {
    Pixar.UsdGeomPrimvarsAPI(prim)
  }

  /// Create a primvar with the given name and type
  @discardableResult
  func createPrimvar(
    _ name: Tf.Token,
    typeName: Sdf.ValueTypeName,
    interpolation: Tf.Token = Tf.Token(),
    elementSize: Int32 = -1
  ) -> UsdGeom.Primvar
  {
    CreatePrimvar(name, typeName, interpolation, elementSize)
  }

  /// Create a primvar with the given name (as string) and type
  @discardableResult
  func createPrimvar(
    _ name: String,
    typeName: Sdf.ValueTypeName,
    interpolation: Tf.Token = Tf.Token(),
    elementSize: Int32 = -1
  ) -> UsdGeom.Primvar
  {
    createPrimvar(Tf.Token(name), typeName: typeName, interpolation: interpolation, elementSize: elementSize)
  }

  /// Get a primvar by name
  func getPrimvar(_ name: Tf.Token) -> UsdGeom.Primvar
  {
    GetPrimvar(name)
  }

  /// Get a primvar by name (as string)
  func getPrimvar(_ name: String) -> UsdGeom.Primvar
  {
    getPrimvar(Tf.Token(name))
  }

  /// Check if a primvar exists
  func hasPrimvar(_ name: Tf.Token) -> Bool
  {
    HasPrimvar(name)
  }

  /// Check if a primvar exists (string version)
  func hasPrimvar(_ name: String) -> Bool
  {
    hasPrimvar(Tf.Token(name))
  }
}

public extension UsdGeom.Primvar
{
  /// Set an integer value on the primvar
  @discardableResult
  func set(_ value: Int, time: UsdTimeCode = UsdTimeCode.Default()) -> Bool
  {
    Set(Int32(value), time)
  }

  /// Set a float value on the primvar
  @discardableResult
  func set(_ value: Float, time: UsdTimeCode = UsdTimeCode.Default()) -> Bool
  {
    Set(value, time)
  }

  /// Set a double value on the primvar
  @discardableResult
  func set(_ value: Double, time: UsdTimeCode = UsdTimeCode.Default()) -> Bool
  {
    Set(value, time)
  }

  /// Set a VtValue on the primvar
  @discardableResult
  func set(_ value: VtValue, time: UsdTimeCode = UsdTimeCode.Default()) -> Bool
  {
    Set(value, time)
  }
}
