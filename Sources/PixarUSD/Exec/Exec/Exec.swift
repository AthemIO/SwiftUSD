/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Exec

/**
 * # ``Exec``
 *
 * **Execution System Core**
 *
 * ## Overview
 *
 * **Exec** is the **Execution System Core** library, built on Vdf, Ef, and Esf.
 * It provides facilities for defining computations provided by objects in a scene,
 * ingesting scenes and compiling data flow networks, and evaluating those networks.
 *
 * ## Builtin Computations
 *
 * OpenExec provides several builtin computation tokens accessible via `Exec.Tokens`:
 *
 * - `computeTime`: Returns the current stage time as an `EfTime` value
 * - `computeValue`: Returns an attribute's value (type matches the attribute's scalar type)
 *
 * ## Example Usage
 *
 * ```swift
 * // Get the current time from the stage
 * let timeKey = ExecUsd.ValueKey(
 *     prim: stage.getPseudoRoot(),
 *     computation: Exec.Tokens.computeTime
 * )
 *
 * // Get an attribute's value
 * let valueKey = ExecUsd.ValueKey(
 *     attribute: myAttribute,
 *     computation: Exec.Tokens.computeValue
 * )
 * ```
 */
public enum Exec {}

// MARK: - Builtin Computation Tokens

public extension Exec
{
  /**
   * # ``Exec/Tokens``
   *
   * Builtin computation tokens for the OpenExec execution system.
   *
   * ## Overview
   *
   * These tokens identify builtin computations that can be used with
   * `ExecUsd.ValueKey` to request computed values from the execution system.
   *
   * ## Available Tokens
   *
   * - `computeTime`: Stage time computation (returns `EfTime`)
   * - `computeValue`: Attribute value computation (returns attribute's value type)
   */
  enum Tokens
  {
    /**
     * Token for the builtin stage time computation.
     *
     * This computation returns the current time on the stage as an `EfTime` value.
     * The computation provider must be the stage (e.g., the pseudo-root prim).
     *
     * ## Example
     *
     * ```swift
     * let timeKey = ExecUsd.ValueKey(
     *     prim: stage.getPseudoRoot(),
     *     computation: Exec.Tokens.computeTime
     * )
     * var request = system.buildRequest(valueKeys: [timeKey])
     * let cacheView = system.compute(request: &request)
     * let time = cacheView.get(index: 0)
     * ```
     */
    public static var computeTime: Pixar.TfToken
    {
      Pixar.Exec_Swift_GetComputeTimeToken()
    }

    /**
     * Token for the builtin attribute value computation.
     *
     * This computation returns an attribute's value. The return type matches
     * the attribute's scalar value type. The computation provider must be
     * an attribute.
     *
     * ## Example
     *
     * ```swift
     * let valueKey = ExecUsd.ValueKey(
     *     attribute: radiusAttr,
     *     computation: Exec.Tokens.computeValue
     * )
     * var request = system.buildRequest(valueKeys: [valueKey])
     * let cacheView = system.compute(request: &request)
     * let value = cacheView.get(index: 0)
     * ```
     */
    public static var computeValue: Pixar.TfToken
    {
      Pixar.Exec_Swift_GetComputeValueToken()
    }

    /**
     * Returns all builtin computation tokens.
     *
     * Useful for iteration or validation purposes.
     */
    public static var allTokens: [Pixar.TfToken]
    {
      let cxxVector = Pixar.Exec_Swift_GetAllBuiltinComputationTokens()
      var result: [Pixar.TfToken] = []
      for i in 0 ..< cxxVector.size()
      {
        result.append(cxxVector[i])
      }
      return result
    }
  }
}
