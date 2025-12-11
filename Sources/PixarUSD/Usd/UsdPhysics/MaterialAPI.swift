/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import UsdPhysics

public extension UsdPhysics.MaterialAPI
{
  // MARK: - Apply Methods

  /// Applies the MaterialAPI schema to the given prim.
  ///
  /// - Parameter prim: The prim to apply the schema to.
  /// - Returns: A valid UsdPhysicsMaterialAPI if successful.
  @discardableResult
  static func apply(_ prim: Usd.Prim) -> UsdPhysics.MaterialAPI
  {
    UsdPhysics.MaterialAPI.Apply(prim)
  }

  // MARK: - Convenience Apply Methods

  @discardableResult
  static func apply(_ material: UsdShade.Material) -> UsdPhysics.MaterialAPI
  {
    UsdPhysics.MaterialAPI.Apply(material.GetPrim())
  }
}
