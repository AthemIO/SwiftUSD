/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Usd
import UsdValidation

/**
 * # ``UsdValidation``
 *
 * **USD Validation Framework**
 *
 * ## Overview
 *
 * **UsdValidation** provides a pluggable validation framework for USD content.
 * It allows developers to run validators that check USD layers, stages,
 * and prims for compliance with various rules and best practices.
 *
 * The validation framework consists of:
 * - ``Context`` - Execution context for running validators on stages, layers, or prims
 * - ``Registry`` - Central registry for discovering and querying validators
 * - ``Validator`` - Individual validation rules (accessed via Registry)
 * - ``ValidatorSuite`` - Collections of related validators
 * - ``Error`` - Results from validation with severity levels and site information
 *
 * ## Usage
 *
 * ```swift
 * // Create a validation context with core USD validators
 * let context = UsdValidation.Context(keywords: [Tf.Token("UsdCoreValidators")])
 *
 * // Validate a stage
 * let errors = context.validate(stage: myStage)
 *
 * // Process errors
 * for i in 0..<errors.size() {
 *     let error = errors[i]
 *     print("[\(error.GetType())]: \(error.GetMessage())")
 * }
 * ```
 *
 * ## Swift Bridge Layer
 *
 * Some C++ types in UsdValidation use `std::function` and `std::variant` which
 * are not directly compatible with Swift C++ interop. SwiftUSD provides a bridge
 * layer (swiftBridge.h) that enables full access to the validation framework.
 *
 * The following features are available:
 * - Create validation contexts from keywords, metadata, or schema types
 * - Run validators on layers, stages, or prims
 * - Query the registry for validators by name, keyword, plugin, or schema type
 * - Access validator metadata and documentation
 * - Run individual validators directly
 *
 * Note: Creating custom validators from Swift is not currently supported.
 * Custom validators must be implemented in C++ and registered via plugins.
 *
 * New in OpenUSD 25.11.
 */
public enum UsdValidation
{}

// MARK: - Global Type Aliases (types that export cleanly to Swift)

/// Error type enum indicating validation error severity.
public typealias UsdValidationErrorType = Pixar.UsdValidationErrorType

/// Error site information indicating where a validation error occurred.
public typealias UsdValidationErrorSite = Pixar.UsdValidationErrorSite

/// A validation error returned by validators.
public typealias UsdValidationError = Pixar.UsdValidationError

/// A vector of validation errors.
public typealias UsdValidationErrorVector = Pixar.UsdValidationErrorVector

/// Metadata describing a validator.
public typealias UsdValidationValidatorMetadata = Pixar.UsdValidationValidatorMetadata

/// A vector of validator metadata.
public typealias UsdValidationValidatorMetadataVector = Pixar.UsdValidationValidatorMetadataVector

/// Time range for time-dependent validation.
public typealias UsdValidationTimeRange = Pixar.UsdValidationTimeRange

// Note: The following types are not exported due to std::function and std::variant usage:
// - UsdValidationValidator (uses std::variant for task functions)
// - UsdValidationValidatorSuite (depends on UsdValidationValidator)
// - UsdValidationContext (uses std::variant and depends on above types)
// - UsdValidationRegistry (TfSingleton pattern and validator management)
// - UsdValidationFixer (uses std::function)

// MARK: - Namespace Type Aliases

public extension UsdValidation
{
  /// Error type enum indicating validation error severity.
  typealias ErrorType = UsdValidationErrorType

  /// Error site information indicating where a validation error occurred.
  typealias ErrorSite = UsdValidationErrorSite

  /// A validation error returned by validators.
  typealias Error = UsdValidationError

  /// A vector of validation errors.
  typealias ErrorVector = UsdValidationErrorVector

  /// Metadata describing a validator.
  typealias ValidatorMetadata = UsdValidationValidatorMetadata

  /// A vector of validator metadata.
  typealias ValidatorMetadataVector = UsdValidationValidatorMetadataVector

  /// Time range for time-dependent validation.
  typealias TimeRange = UsdValidationTimeRange
}
