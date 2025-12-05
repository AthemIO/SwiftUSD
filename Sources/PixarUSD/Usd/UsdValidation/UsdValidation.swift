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
 * It allows developers to write custom validators that check USD layers, stages,
 * and prims for compliance with various rules and best practices.
 *
 * The validation framework consists of:
 * - ``Validator`` - Individual validation rules that check specific aspects of USD content
 * - ``ValidatorSuite`` - Collections of related validators
 * - ``Context`` - Execution context for running validators
 * - ``Registry`` - Central registry for discovering and managing validators
 * - ``Error`` - Results from validation with severity levels and site information
 *
 * ## Limitations
 *
 * Some C++ types in UsdValidation use `std::function` and `std::variant` which
 * are not fully compatible with Swift C++ interop. The following limitations apply:
 *
 * - Creating custom validators from Swift is not currently supported
 * - The `UsdValidationRegistry` singleton may not be directly accessible
 * - Validator task functions cannot be called directly from Swift
 *
 * However, you can still:
 * - Query validator metadata
 * - Access validation errors and their properties
 * - Use validation error sites to locate issues
 *
 * For full validation support, consider using the C++ API directly or
 * via Python bindings.
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
