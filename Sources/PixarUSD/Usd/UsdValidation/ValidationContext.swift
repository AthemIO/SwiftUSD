/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Sdf
import Tf
import Usd
import UsdValidation

// MARK: - UsdValidation.Context

public extension UsdValidation
{
  /**
   * # UsdValidation.Context
   *
   * **Validation Context for Running Validators**
   *
   * ## Overview
   *
   * A context manages a set of validators and provides methods to run them
   * against USD layers, stages, or prims. The context can be created from:
   * - Keywords (e.g., "UsdCoreValidators")
   * - Validator metadata
   *
   * ## Usage
   *
   * ```swift
   * // Create a context from keywords
   * let context = UsdValidation.Context(keywords: [Tf.Token("UsdCoreValidators")])
   *
   * // Validate a stage
   * let errors = context.validate(stage: myStage)
   *
   * // Check for errors
   * for error in errors {
   *     print("Validation error: \(error.GetMessage())")
   * }
   * ```
   *
   * ## Thread Safety
   *
   * Validation operations run validators in parallel. The context is safe
   * to use from multiple threads, but the stage/layer being validated should
   * not be modified during validation.
   */
  final class Context: @unchecked Sendable
  {
    /// Opaque pointer to the underlying UsdValidationContext.
    private var _context: UnsafeMutableRawPointer?

    /// Whether to include validators from ancestor schema types.
    public let includeAllAncestors: Bool

    // MARK: - Initialization

    /**
     * Create a validation context from keywords.
     *
     * Keywords are used to select validators. For example, "UsdCoreValidators"
     * selects all core USD validators.
     *
     * - Parameters:
     *   - keywords: Tokens identifying validator groups.
     *   - includeAllAncestors: If true, include validators from ancestor
     *     TfTypes for any schema type validators found.
     */
    public init(keywords: [Tf.Token], includeAllAncestors: Bool = true)
    {
      self.includeAllAncestors = includeAllAncestors

      // Convert Swift array to TfTokenVector
      var tokenVector = Pixar.TfTokenVector()
      for keyword in keywords
      {
        tokenVector.push_back(keyword)
      }

      _context = Pixar.UsdValidation_Swift_CreateContextFromKeywords(
        tokenVector,
        includeAllAncestors
      )
    }

    /**
     * Create a validation context from validator metadata.
     *
     * - Parameters:
     *   - metadata: Vector of validator metadata to select validators.
     *   - includeAllAncestors: If true, include validators from ancestor
     *     TfTypes for any schema type validators found.
     */
    public init(metadata: UsdValidation.ValidatorMetadataVector, includeAllAncestors: Bool = true)
    {
      self.includeAllAncestors = includeAllAncestors
      _context = Pixar.UsdValidation_Swift_CreateContextFromMetadata(
        metadata,
        includeAllAncestors
      )
    }

    deinit
    {
      if let context = _context
      {
        Pixar.UsdValidation_Swift_DestroyContext(context)
      }
    }

    // MARK: - Validation Methods

    /**
     * Validate a layer using this context's validators.
     *
     * Only layer validators in the selected group will be run.
     *
     * - Parameter layer: The layer to validate.
     * - Returns: A vector of validation errors found.
     */
    public func validate(layer: Sdf.LayerHandle) -> UsdValidation.ErrorVector
    {
      guard let context = _context
      else
      {
        return UsdValidation.ErrorVector()
      }
      return Pixar.UsdValidation_Swift_ValidateLayer(context, layer)
    }

    /**
     * Validate a stage using this context's validators.
     *
     * This runs:
     * - Layer validators on all used layers
     * - Stage validators on the stage
     * - Prim validators on all prims (using instance proxy traversal)
     *
     * - Parameter stage: The stage to validate.
     * - Returns: A vector of validation errors found.
     */
    public func validate(stage: Usd.StageRefPtr) -> UsdValidation.ErrorVector
    {
      guard let context = _context
      else
      {
        return UsdValidation.ErrorVector()
      }
      return Pixar.UsdValidation_Swift_ValidateStage(context, stage)
    }

    /**
     * Validate a stage with explicit time range.
     *
     * - Parameters:
     *   - stage: The stage to validate.
     *   - timeRange: The time range for time-dependent validation.
     * - Returns: A vector of validation errors found.
     */
    public func validate(stage: Usd.StageRefPtr, timeRange: UsdValidation.TimeRange) -> UsdValidation.ErrorVector
    {
      guard let context = _context
      else
      {
        return UsdValidation.ErrorVector()
      }
      return Pixar.UsdValidation_Swift_ValidateStageWithTimeRange(context, stage, timeRange)
    }

    /**
     * Validate a collection of prims.
     *
     * - Parameters:
     *   - prims: The prims to validate.
     *   - timeRange: The time range for time-dependent validation (defaults to full interval).
     * - Returns: A vector of validation errors found.
     */
    public func validate(prims: [Usd.Prim], timeRange: UsdValidation.TimeRange = .full()) -> UsdValidation.ErrorVector
    {
      guard let context = _context
      else
      {
        return UsdValidation.ErrorVector()
      }

      // Convert Swift array to std::vector<UsdPrim>
      var primVector = Pixar.UsdPrimVector()
      for prim in prims
      {
        primVector.push_back(prim)
      }

      return Pixar.UsdValidation_Swift_ValidatePrims(context, primVector, timeRange)
    }

    // MARK: - State

    /// Returns true if this context was successfully created.
    public var isValid: Bool
    {
      _context != nil
    }
  }
}
