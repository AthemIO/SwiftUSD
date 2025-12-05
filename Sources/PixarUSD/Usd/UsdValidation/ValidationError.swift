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
import Usd
import UsdValidation

// MARK: - UsdValidationErrorType Swift Extensions

public extension UsdValidation
{
  /**
   * # UsdValidation.ErrorSeverity
   *
   * Swift-friendly enum for validation error severity levels.
   *
   * ## Overview
   *
   * Provides a Swift enum wrapper around `UsdValidationErrorType` for better
   * Swift API ergonomics and pattern matching.
   *
   * ## Usage
   *
   * The C++ `UsdValidationErrorType` enum has the following values:
   * - `None` (0) - No error
   * - `Error` (1) - An error that should be fixed
   * - `Warn` (2) - A warning that may indicate issues
   * - `Info` (3) - Informational message
   */
  enum ErrorSeverity: Int, Sendable, CaseIterable
  {
    /// No error.
    case none = 0
    /// An error that should be fixed.
    case error = 1
    /// A warning that may indicate issues.
    case warn = 2
    /// Informational message.
    case info = 3

    /// Initialize from C++ UsdValidationErrorType.
    public init(_ cxxType: Pixar.UsdValidationErrorType)
    {
      switch cxxType
      {
        case .None: self = .none
        case .Error: self = .error
        case .Warn: self = .warn
        case .Info: self = .info
        default: self = .none
      }
    }

    /// Convert to C++ UsdValidationErrorType.
    public var cxxType: Pixar.UsdValidationErrorType
    {
      switch self
      {
        case .none: .None
        case .error: .Error
        case .warn: .Warn
        case .info: .Info
      }
    }

    /// Human-readable description of the severity level.
    public var description: String
    {
      switch self
      {
        case .none: "None"
        case .error: "Error"
        case .warn: "Warning"
        case .info: "Info"
      }
    }
  }
}

// Note: UsdValidationError, UsdValidationErrorSite, and related types are
// exported as C++ types but their member functions may not be directly
// callable from Swift due to std::function and std::variant usage in
// dependent types. Use the C++ API directly if you need full access.
//
// The following types are available as typealiases:
// - UsdValidation.Error (UsdValidationError)
// - UsdValidation.ErrorSite (UsdValidationErrorSite)
// - UsdValidation.ErrorVector (UsdValidationErrorVector)
// - UsdValidation.ErrorType (UsdValidationErrorType)
//
// The ErrorSeverity Swift enum above provides a convenient way to work
// with validation error severity levels from Swift.

// MARK: - UsdValidationTimeRange Swift Extensions

public extension UsdValidation.TimeRange
{
  /**
   * Create a time range for validation.
   *
   * The default time range uses the full time interval, which means
   * validators will check all time samples.
   */
  static func full() -> UsdValidation.TimeRange
  {
    Pixar.UsdValidationTimeRange()
  }
}

// MARK: - UsdValidationValidatorMetadata Swift Extensions

public extension UsdValidation.ValidatorMetadata
{
  /// The name of the validator.
  var validatorName: Tf.Token
  {
    name
  }

  /// Documentation string for the validator.
  var documentation: String
  {
    String(doc)
  }

  /// Whether this validator is time-dependent.
  var isTimeDependentValidator: Bool
  {
    isTimeDependent
  }

  /// Whether this represents a validator suite.
  var isSuiteValidator: Bool
  {
    isSuite
  }
}
