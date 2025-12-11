/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import UsdPhysics

public extension UsdPhysics.CollisionAPI
{
  // MARK: - Apply Methods

  /// Applies the CollisionAPI schema to the given prim.
  ///
  /// - Parameter prim: The prim to apply the schema to.
  /// - Returns: A valid UsdPhysicsCollisionAPI if successful.
  @discardableResult
  static func apply(_ prim: Usd.Prim) -> UsdPhysics.CollisionAPI
  {
    UsdPhysics.CollisionAPI.Apply(prim)
  }

  // MARK: - Convenience Apply Methods for Geometry Types

  @discardableResult
  static func apply(_ prim: UsdGeomSphere) -> UsdPhysics.CollisionAPI
  {
    UsdPhysics.CollisionAPI.Apply(prim.GetPrim())
  }

  @discardableResult
  static func apply(_ prim: UsdGeomCube) -> UsdPhysics.CollisionAPI
  {
    UsdPhysics.CollisionAPI.Apply(prim.GetPrim())
  }

  @discardableResult
  static func apply(_ prim: UsdGeomCapsule) -> UsdPhysics.CollisionAPI
  {
    UsdPhysics.CollisionAPI.Apply(prim.GetPrim())
  }

  @discardableResult
  static func apply(_ prim: UsdGeomCylinder) -> UsdPhysics.CollisionAPI
  {
    UsdPhysics.CollisionAPI.Apply(prim.GetPrim())
  }

  @discardableResult
  static func apply(_ prim: UsdGeomCone) -> UsdPhysics.CollisionAPI
  {
    UsdPhysics.CollisionAPI.Apply(prim.GetPrim())
  }

  @discardableResult
  static func apply(_ prim: UsdGeomMesh) -> UsdPhysics.CollisionAPI
  {
    UsdPhysics.CollisionAPI.Apply(prim.GetPrim())
  }

  @discardableResult
  static func apply(_ prim: UsdGeomXform) -> UsdPhysics.CollisionAPI
  {
    UsdPhysics.CollisionAPI.Apply(prim.GetPrim())
  }

  @discardableResult
  static func apply(_ prim: UsdGeomPlane) -> UsdPhysics.CollisionAPI
  {
    UsdPhysics.CollisionAPI.Apply(prim.GetPrim())
  }
}
