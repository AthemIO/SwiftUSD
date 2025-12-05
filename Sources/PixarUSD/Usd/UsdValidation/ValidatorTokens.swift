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

import Tf
import UsdValidation

// MARK: - Validation Tokens

public extension UsdValidation
{
  /**
   * # UsdValidation.ValidatorNames
   *
   * ## Overview
   *
   * Tokens representing validator names from the usdValidation plugin.
   * Validator names are prefixed with `usdValidation:` to uniquely
   * identify plugin-provided validators.
   */
  enum ValidatorNames: CaseIterable, Sendable
  {
    /// The composition error test validator.
    case compositionErrorTest
    /// The stage metadata checker validator.
    case stageMetadataChecker

    public func getToken() -> Tf.Token
    {
      switch self
      {
        case .compositionErrorTest:
          Tf.Token("usdValidation:CompositionErrorTest")
        case .stageMetadataChecker:
          Tf.Token("usdValidation:StageMetadataChecker")
      }
    }
  }

  /**
   * # UsdValidation.ValidatorKeywords
   *
   * ## Overview
   *
   * Keywords associated with validators in the usdValidation plugin.
   * Clients can use these to query validators by keyword.
   */
  enum ValidatorKeywords: CaseIterable, Sendable
  {
    /// Core USD validators keyword.
    case usdCoreValidators

    public func getToken() -> Tf.Token
    {
      switch self
      {
        case .usdCoreValidators:
          Tf.Token("UsdCoreValidators")
      }
    }
  }

  /**
   * # UsdValidation.ErrorNames
   *
   * ## Overview
   *
   * Tokens representing validation error names.
   * These are used to construct error identifiers.
   */
  enum ErrorNames: CaseIterable, Sendable
  {
    /// Composition error name.
    case compositionError
    /// Missing default prim error name.
    case missingDefaultPrim

    public func getToken() -> Tf.Token
    {
      switch self
      {
        case .compositionError:
          Tf.Token("CompositionError")
        case .missingDefaultPrim:
          Tf.Token("MissingDefaultPrim")
      }
    }
  }
}
