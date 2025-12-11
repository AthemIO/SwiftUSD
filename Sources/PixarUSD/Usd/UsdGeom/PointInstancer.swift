/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import UsdGeom

public typealias UsdGeomPointInstancer = Pixar.UsdGeomPointInstancer

public extension UsdGeom
{
  typealias PointInstancer = UsdGeomPointInstancer
}

@Xformable
extension UsdGeom.PointInstancer: GeomXformable
{
  public func xformablePrim() -> Usd.Prim
  {
    let xformable = unsafeBitCast(self, to: UsdGeomXformable.self)
    return Pixar.UsdGeomXformable_GetPrim(xformable)
  }

  @discardableResult
  public static func define(_ stage: Usd.StageRefPtr, path: Sdf.Path) -> UsdGeom.PointInstancer
  {
    UsdGeom.PointInstancer.Define(stage.pointee.getPtr(), path)
  }

  @discardableResult
  public static func define(_ stage: Usd.StageRefPtr, path: String) -> UsdGeom.PointInstancer
  {
    UsdGeom.PointInstancer.define(stage, path: .init(path))
  }

  /// Get the prototypes relationship - used to target prototype prims
  public func getPrototypesRel() -> Pixar.UsdRelationship
  {
    GetPrototypesRel()
  }

  /// Create the protoIndices attribute (per-instance prototype index)
  @discardableResult
  public func createProtoIndicesAttr() -> Pixar.UsdAttribute
  {
    CreateProtoIndicesAttr(Vt.Value(), false)
  }

  /// Create the positions attribute (per-instance position)
  @discardableResult
  public func createPositionsAttr() -> Pixar.UsdAttribute
  {
    CreatePositionsAttr(Vt.Value(), false)
  }

  /// Create the orientations attribute (per-instance quaternion orientation, half-precision)
  @discardableResult
  public func createOrientationsAttr() -> Pixar.UsdAttribute
  {
    CreateOrientationsAttr(Vt.Value(), false)
  }

  /// Create the orientationsf attribute (per-instance quaternion orientation, full-precision)
  @discardableResult
  public func createOrientationsfAttr() -> Pixar.UsdAttribute
  {
    CreateOrientationsfAttr(Vt.Value(), false)
  }

  /// Create the scales attribute (per-instance scale)
  @discardableResult
  public func createScalesAttr() -> Pixar.UsdAttribute
  {
    CreateScalesAttr(Vt.Value(), false)
  }

  /// Create the velocities attribute (for motion blur)
  @discardableResult
  public func createVelocitiesAttr() -> Pixar.UsdAttribute
  {
    CreateVelocitiesAttr(Vt.Value(), false)
  }

  /// Create the ids attribute (per-instance unique ID)
  @discardableResult
  public func createIdsAttr() -> Pixar.UsdAttribute
  {
    CreateIdsAttr(Vt.Value(), false)
  }
}
