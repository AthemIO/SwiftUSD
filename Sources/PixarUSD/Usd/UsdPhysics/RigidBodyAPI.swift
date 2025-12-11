/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import UsdPhysics

public extension UsdPhysics.RigidBodyAPI
{
  // MARK: - Apply Methods

  /// Applies the RigidBodyAPI schema to the given prim.
  ///
  /// - Parameter prim: The prim to apply the schema to.
  /// - Returns: A valid UsdPhysicsRigidBodyAPI if successful.
  @discardableResult
  static func apply(_ prim: Usd.Prim) -> UsdPhysics.RigidBodyAPI
  {
    UsdPhysics.RigidBodyAPI.Apply(prim)
  }

  // MARK: - Convenience Apply Methods for Geometry Types

  @discardableResult
  static func apply(_ prim: UsdGeomSphere) -> UsdPhysics.RigidBodyAPI
  {
    UsdPhysics.RigidBodyAPI.Apply(prim.GetPrim())
  }

  @discardableResult
  static func apply(_ prim: UsdGeomCube) -> UsdPhysics.RigidBodyAPI
  {
    UsdPhysics.RigidBodyAPI.Apply(prim.GetPrim())
  }

  @discardableResult
  static func apply(_ prim: UsdGeomCapsule) -> UsdPhysics.RigidBodyAPI
  {
    UsdPhysics.RigidBodyAPI.Apply(prim.GetPrim())
  }

  @discardableResult
  static func apply(_ prim: UsdGeomCylinder) -> UsdPhysics.RigidBodyAPI
  {
    UsdPhysics.RigidBodyAPI.Apply(prim.GetPrim())
  }

  @discardableResult
  static func apply(_ prim: UsdGeomCone) -> UsdPhysics.RigidBodyAPI
  {
    UsdPhysics.RigidBodyAPI.Apply(prim.GetPrim())
  }

  @discardableResult
  static func apply(_ prim: UsdGeomMesh) -> UsdPhysics.RigidBodyAPI
  {
    UsdPhysics.RigidBodyAPI.Apply(prim.GetPrim())
  }

  @discardableResult
  static func apply(_ prim: UsdGeomXform) -> UsdPhysics.RigidBodyAPI
  {
    UsdPhysics.RigidBodyAPI.Apply(prim.GetPrim())
  }
}
