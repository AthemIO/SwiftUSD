/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import UsdPhysics

public extension UsdPhysics.MassAPI
{
  // MARK: - Apply Methods

  /// Applies the MassAPI schema to the given prim.
  ///
  /// - Parameter prim: The prim to apply the schema to.
  /// - Returns: A valid UsdPhysicsMassAPI if successful.
  @discardableResult
  static func apply(_ prim: Usd.Prim) -> UsdPhysics.MassAPI
  {
    UsdPhysics.MassAPI.Apply(prim)
  }

  // MARK: - Convenience Apply Methods for Geometry Types

  @discardableResult
  static func apply(_ prim: UsdGeomSphere) -> UsdPhysics.MassAPI
  {
    UsdPhysics.MassAPI.Apply(prim.GetPrim())
  }

  @discardableResult
  static func apply(_ prim: UsdGeomCube) -> UsdPhysics.MassAPI
  {
    UsdPhysics.MassAPI.Apply(prim.GetPrim())
  }

  @discardableResult
  static func apply(_ prim: UsdGeomCapsule) -> UsdPhysics.MassAPI
  {
    UsdPhysics.MassAPI.Apply(prim.GetPrim())
  }

  @discardableResult
  static func apply(_ prim: UsdGeomCylinder) -> UsdPhysics.MassAPI
  {
    UsdPhysics.MassAPI.Apply(prim.GetPrim())
  }

  @discardableResult
  static func apply(_ prim: UsdGeomCone) -> UsdPhysics.MassAPI
  {
    UsdPhysics.MassAPI.Apply(prim.GetPrim())
  }

  @discardableResult
  static func apply(_ prim: UsdGeomMesh) -> UsdPhysics.MassAPI
  {
    UsdPhysics.MassAPI.Apply(prim.GetPrim())
  }

  @discardableResult
  static func apply(_ prim: UsdGeomXform) -> UsdPhysics.MassAPI
  {
    UsdPhysics.MassAPI.Apply(prim.GetPrim())
  }
}
