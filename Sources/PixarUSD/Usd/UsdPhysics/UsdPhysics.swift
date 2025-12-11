/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import UsdPhysics

// MARK: - Type Aliases

/// Type alias for the C++ UsdPhysicsRigidBodyAPI class.
/// Applies physics body attributes to any UsdGeomXformable prim and marks
/// that prim to be driven by a simulation.
public typealias UsdPhysicsRigidBodyAPI = Pixar.UsdPhysicsRigidBodyAPI

/// Type alias for the C++ UsdPhysicsMassAPI class.
/// Defines explicit mass properties (mass, density, inertia etc.).
public typealias UsdPhysicsMassAPI = Pixar.UsdPhysicsMassAPI

/// Type alias for the C++ UsdPhysicsMaterialAPI class.
/// Adds simulation material properties to a Material (friction, restitution).
public typealias UsdPhysicsMaterialAPI = Pixar.UsdPhysicsMaterialAPI

/// Type alias for the C++ UsdPhysicsCollisionAPI class.
/// Applies collision attributes to a UsdGeomXformable prim.
public typealias UsdPhysicsCollisionAPI = Pixar.UsdPhysicsCollisionAPI

// MARK: - UsdPhysics Namespace

/// # UsdPhysics
///
/// Swift namespace for USD Physics schema types.
///
/// ## Overview
///
/// UsdPhysics provides API schemas for physics simulation:
/// - `RigidBodyAPI` - Marks a prim as a rigid body for physics simulation
/// - `MassAPI` - Defines mass properties (mass, density, inertia)
/// - `MaterialAPI` - Defines material properties (friction, restitution)
/// - `CollisionAPI` - Marks a prim as a collision shape
///
/// ## Usage
///
/// ```swift
/// // Apply rigid body physics to a prim
/// let rigidBody = UsdPhysics.RigidBodyAPI.apply(prim)
///
/// // Set mass properties
/// let massAPI = UsdPhysics.MassAPI.apply(prim)
/// massAPI.setMass(10.0)
///
/// // Apply collision
/// let collision = UsdPhysics.CollisionAPI.apply(prim)
/// ```
public enum UsdPhysics
{
  /// Shorthand for UsdPhysicsRigidBodyAPI
  public typealias RigidBodyAPI = UsdPhysicsRigidBodyAPI

  /// Shorthand for UsdPhysicsMassAPI
  public typealias MassAPI = UsdPhysicsMassAPI

  /// Shorthand for UsdPhysicsMaterialAPI
  public typealias MaterialAPI = UsdPhysicsMaterialAPI

  /// Shorthand for UsdPhysicsCollisionAPI
  public typealias CollisionAPI = UsdPhysicsCollisionAPI
}
