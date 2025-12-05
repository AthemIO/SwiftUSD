/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import ExecGeom

/**
 * # ``ExecGeom``
 *
 * **Execution for UsdGeom**
 *
 * ## Overview
 *
 * **ExecGeom** is the **Execution for UsdGeom** library. It contains
 * registrations for computations that compute results based on data
 * represented using UsdGeom schemas.
 *
 * ## Available Computations
 *
 * ### Xformable Computations
 * - `computeLocalToWorldTransform`: Computes the local-to-world transformation matrix
 *
 * ## Example Usage
 *
 * ```swift
 * // Create a value key for computing the world transform of a prim
 * let valueKey = ExecUsd.ValueKey(
 *     prim: xformPrim,
 *     computation: ExecGeom.Xformable.computeLocalToWorldTransform
 * )
 *
 * // Build and compute the request
 * var request = system.buildRequest(valueKeys: [valueKey])
 * let cacheView = system.compute(request: &request)
 *
 * // Extract the computed GfMatrix4d
 * let transform = cacheView.get(index: 0)
 * ```
 */
public enum ExecGeom {}

// MARK: - ExecGeom.Xformable

public extension ExecGeom
{
  /**
   * # ``ExecGeom/Xformable``
   *
   * Tokens and computations for UsdGeomXformable prims.
   *
   * ## Overview
   *
   * The `Xformable` namespace provides access to computation tokens
   * that can be used with `ExecUsd.ValueKey` to request geometric
   * transformation computations.
   */
  enum Xformable
  {
    /**
     * The computation token for computing local-to-world transformation.
     *
     * When used with `ExecUsd.ValueKey`, this computation returns a
     * `GfMatrix4d` representing the full transformation from the prim's
     * local coordinate space to world space.
     *
     * ## Example
     *
     * ```swift
     * let valueKey = ExecUsd.ValueKey(
     *     prim: myXformPrim,
     *     computation: ExecGeom.Xformable.computeLocalToWorldTransform
     * )
     * ```
     */
    public static var computeLocalToWorldTransform: Pixar.TfToken
    {
      Pixar.ExecGeom_Swift_GetComputeLocalToWorldTransformToken()
    }
  }
}

// MARK: - Direct Token Access

public extension ExecGeom
{
  /**
   * # ``ExecGeom/Tokens``
   *
   * Direct access to all ExecGeom tokens.
   *
   * ## Overview
   *
   * Provides direct access to the underlying C++ token static instances
   * for cases where you need the raw token values.
   */
  enum Tokens
  {
    /// The token for the computeLocalToWorldTransform computation.
    public static var computeLocalToWorldTransform: Pixar.TfToken
    {
      Pixar.ExecGeom_Swift_GetComputeLocalToWorldTransformToken()
    }
  }
}
