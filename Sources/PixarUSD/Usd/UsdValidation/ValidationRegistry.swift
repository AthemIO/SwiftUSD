/* ----------------------------------------------------------------
 * :: :  M  E  T  A  V  E  R  S  E  :                            ::
 * ----------------------------------------------------------------
 * Licensed under the terms set forth in the LICENSE.txt file, this
 * file is available at https://openusd.org/license.
 *
 *                                        Copyright (C) 2016 Pixar.
 *         Copyright (C) 2024 Wabi Foundation. All Rights Reserved.
 * ----------------------------------------------------------------
 *  . x x x . o o o . x x x . : : : .    o  x  o    . : : : .
 * ---------------------------------------------------------------- */

import Sdf
import Tf
import Usd
import UsdValidation

// MARK: - UsdValidation.Registry

public extension UsdValidation
{
  /**
   * # UsdValidation.Registry
   *
   * **Central Registry for USD Validators**
   *
   * ## Overview
   *
   * The Registry is a singleton that manages all registered validators.
   * It provides methods to query validators by name, keyword, plugin, or
   * schema type. The registry automatically discovers validators from
   * plugins when needed.
   *
   * ## Usage
   *
   * ```swift
   * // Check if a validator exists
   * if UsdValidation.Registry.hasValidator(named: "usdValidation:CompositionErrorTest") {
   *     print("Validator found!")
   * }
   *
   * // Get all validator metadata
   * let allMetadata = UsdValidation.Registry.getAllValidatorMetadata()
   *
   * // Get validators for a specific keyword
   * let coreValidators = UsdValidation.Registry.getValidatorMetadata(
   *     forKeyword: Tf.Token("UsdCoreValidators")
   * )
   * ```
   *
   * ## Thread Safety
   *
   * The registry is thread-safe. Multiple threads can query the registry
   * concurrently. Registration operations are also thread-safe.
   */
  enum Registry
  {
    // MARK: - Validator Queries

    /**
     * Check if a validator with the given name is registered.
     *
     * - Parameter name: The validator name (e.g., "usdValidation:CompositionErrorTest").
     * - Returns: True if the validator exists.
     */
    public static func hasValidator(named name: Tf.Token) -> Bool
    {
      Pixar.UsdValidation_Swift_HasValidator(name)
    }

    /**
     * Check if a validator with the given name is registered.
     *
     * - Parameter name: The validator name as a string.
     * - Returns: True if the validator exists.
     */
    public static func hasValidator(named name: String) -> Bool
    {
      hasValidator(named: Tf.Token(name))
    }

    /**
     * Check if a validator suite with the given name is registered.
     *
     * - Parameter name: The suite name.
     * - Returns: True if the suite exists.
     */
    public static func hasValidatorSuite(named name: Tf.Token) -> Bool
    {
      Pixar.UsdValidation_Swift_HasValidatorSuite(name)
    }

    // MARK: - Metadata Queries

    /**
     * Get metadata for a specific validator by name.
     *
     * - Parameter name: The validator name.
     * - Returns: The validator metadata, or nil if not found.
     */
    public static func getValidatorMetadata(named name: Tf.Token) -> UsdValidation.ValidatorMetadata?
    {
      var metadata = UsdValidation.ValidatorMetadata()
      if Pixar.UsdValidation_Swift_GetValidatorMetadata(name, &metadata)
      {
        return metadata
      }
      return nil
    }

    /**
     * Get metadata for a specific validator by name.
     *
     * - Parameter name: The validator name as a string.
     * - Returns: The validator metadata, or nil if not found.
     */
    public static func getValidatorMetadata(named name: String) -> UsdValidation.ValidatorMetadata?
    {
      getValidatorMetadata(named: Tf.Token(name))
    }

    /**
     * Get all registered validator metadata.
     *
     * Note: This may trigger loading of plugins that provide validators.
     *
     * - Returns: A vector of all validator metadata.
     */
    public static func getAllValidatorMetadata() -> UsdValidation.ValidatorMetadataVector
    {
      Pixar.UsdValidation_Swift_GetAllValidatorMetadata()
    }

    /**
     * Get validator metadata for validators matching a keyword.
     *
     * - Parameter keyword: The keyword to search for.
     * - Returns: A vector of matching validator metadata.
     */
    public static func getValidatorMetadata(forKeyword keyword: Tf.Token) -> UsdValidation.ValidatorMetadataVector
    {
      Pixar.UsdValidation_Swift_GetValidatorMetadataForKeyword(keyword)
    }

    /**
     * Get validator metadata for validators matching any of the keywords.
     *
     * - Parameter keywords: The keywords to search for.
     * - Returns: A vector of matching validator metadata (union of all matches).
     */
    public static func getValidatorMetadata(forKeywords keywords: [Tf.Token]) -> UsdValidation.ValidatorMetadataVector
    {
      var tokenVector = Pixar.TfTokenVector()
      for keyword in keywords
      {
        tokenVector.push_back(keyword)
      }
      return Pixar.UsdValidation_Swift_GetValidatorMetadataForKeywords(tokenVector)
    }

    /**
     * Get validator metadata for validators belonging to a plugin.
     *
     * - Parameter pluginName: The plugin name.
     * - Returns: A vector of matching validator metadata.
     */
    public static func getValidatorMetadata(forPlugin pluginName: Tf.Token) -> UsdValidation.ValidatorMetadataVector
    {
      Pixar.UsdValidation_Swift_GetValidatorMetadataForPlugin(pluginName)
    }

    /**
     * Get validator metadata for validators matching a schema type.
     *
     * - Parameter schemaType: The schema type name.
     * - Returns: A vector of matching validator metadata.
     */
    public static func getValidatorMetadata(forSchemaType schemaType: Tf.Token) -> UsdValidation.ValidatorMetadataVector
    {
      Pixar.UsdValidation_Swift_GetValidatorMetadataForSchemaType(schemaType)
    }
  }
}

// MARK: - UsdValidation.Validator

public extension UsdValidation
{
  /**
   * # UsdValidation.Validator
   *
   * **A Wrapper for Accessing Registered Validators**
   *
   * ## Overview
   *
   * Provides a Swift-friendly interface to validators registered in the
   * UsdValidationRegistry. Validators are owned by the registry and are
   * not directly constructible from Swift.
   *
   * ## Usage
   *
   * ```swift
   * // Get a validator by name
   * if let validator = UsdValidation.Validator.get(named: "usdValidation:CompositionErrorTest") {
   *     // Run validation directly
   *     let errors = validator.validate(stage: myStage)
   * }
   * ```
   */
  struct Validator
  {
    /// Opaque pointer to the underlying UsdValidationValidator (owned by registry).
    /// Note: Raw pointer to C++ validator, lifetime managed by registry.
    private let _validator: UnsafeRawPointer

    /// Private initializer - validators come from the registry.
    private init(_ ptr: UnsafeRawPointer)
    {
      _validator = ptr
    }

    // MARK: - Factory Methods

    /**
     * Get or load a validator by name from the registry.
     *
     * If the validator is not already loaded, this may trigger plugin loading.
     *
     * - Parameter name: The validator name.
     * - Returns: The validator, or nil if not found.
     */
    public static func get(named name: Tf.Token) -> Validator?
    {
      guard let ptr = Pixar.UsdValidation_Swift_GetOrLoadValidatorByName(name)
      else
      {
        return nil
      }
      return Validator(ptr)
    }

    /**
     * Get or load a validator by name from the registry.
     *
     * - Parameter name: The validator name as a string.
     * - Returns: The validator, or nil if not found.
     */
    public static func get(named name: String) -> Validator?
    {
      get(named: Tf.Token(name))
    }

    // MARK: - Properties

    /// The metadata for this validator.
    public var metadata: UsdValidation.ValidatorMetadata
    {
      Pixar.UsdValidation_Swift_GetValidatorMetadataFromValidator(_validator)
    }

    /// The name of this validator.
    public var name: Tf.Token
    {
      metadata.name
    }

    /// The documentation string for this validator.
    public var documentation: String
    {
      String(metadata.doc)
    }

    /// Whether this validator is time-dependent.
    public var isTimeDependent: Bool
    {
      metadata.isTimeDependent
    }

    // MARK: - Validation Methods

    /**
     * Run this validator on a layer.
     *
     * - Parameter layer: The layer to validate.
     * - Returns: A vector of validation errors.
     */
    public func validate(layer: Sdf.LayerHandle) -> UsdValidation.ErrorVector
    {
      Pixar.UsdValidation_Swift_ValidatorValidateLayer(_validator, layer)
    }

    /**
     * Run this validator on a stage.
     *
     * - Parameters:
     *   - stage: The stage to validate.
     *   - timeRange: The time range for validation (defaults to full interval).
     * - Returns: A vector of validation errors.
     */
    public func validate(stage: Usd.StageRefPtr, timeRange: UsdValidation.TimeRange = .full()) -> UsdValidation.ErrorVector
    {
      Pixar.UsdValidation_Swift_ValidatorValidateStage(_validator, stage, timeRange)
    }

    /**
     * Run this validator on a prim.
     *
     * - Parameters:
     *   - prim: The prim to validate.
     *   - timeRange: The time range for validation (defaults to full interval).
     * - Returns: A vector of validation errors.
     */
    public func validate(prim: Usd.Prim, timeRange: UsdValidation.TimeRange = .full()) -> UsdValidation.ErrorVector
    {
      Pixar.UsdValidation_Swift_ValidatorValidatePrim(_validator, prim, timeRange)
    }
  }
}

// MARK: - UsdValidation.ValidatorSuite

public extension UsdValidation
{
  /**
   * # UsdValidation.ValidatorSuite
   *
   * **A Collection of Related Validators**
   *
   * ## Overview
   *
   * Validator suites bundle related validators together. Suites are registered
   * in the UsdValidationRegistry and can be queried by name.
   */
  struct ValidatorSuite
  {
    /// Opaque pointer to the underlying UsdValidationValidatorSuite (owned by registry).
    /// Note: Raw pointer to C++ suite, lifetime managed by registry.
    private let _suite: UnsafeRawPointer

    /// Private initializer - suites come from the registry.
    private init(_ ptr: UnsafeRawPointer)
    {
      _suite = ptr
    }

    // MARK: - Factory Methods

    /**
     * Get or load a validator suite by name from the registry.
     *
     * - Parameter name: The suite name.
     * - Returns: The suite, or nil if not found.
     */
    public static func get(named name: Tf.Token) -> ValidatorSuite?
    {
      guard let ptr = Pixar.UsdValidation_Swift_GetOrLoadValidatorSuiteByName(name)
      else
      {
        return nil
      }
      return ValidatorSuite(ptr)
    }

    /**
     * Get or load a validator suite by name from the registry.
     *
     * - Parameter name: The suite name as a string.
     * - Returns: The suite, or nil if not found.
     */
    public static func get(named name: String) -> ValidatorSuite?
    {
      get(named: Tf.Token(name))
    }

    // MARK: - Properties

    /// The metadata for this suite.
    public var metadata: UsdValidation.ValidatorMetadata
    {
      Pixar.UsdValidation_Swift_GetValidatorSuiteMetadata(_suite)
    }

    /// The name of this suite.
    public var name: Tf.Token
    {
      metadata.name
    }

    /// The number of validators contained in this suite.
    public var validatorCount: Int
    {
      Int(Pixar.UsdValidation_Swift_GetValidatorSuiteValidatorCount(_suite))
    }
  }
}
