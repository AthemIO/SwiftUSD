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

/**
 * # ``Usd``
 *
 * **Universal Scene Description (Core)**
 *
 * ## Overview
 *
 * **Usd** is the **universal scene description** library, it provides a
 * high-level API for reading, authoring, and layering scene description. */
public enum Usd
{}

// MARK: - OpenUSD 25.11 Validation Framework

public typealias UsdValidator = Pixar.UsdValidator
public typealias UsdValidationError = Pixar.UsdValidationError
public typealias UsdValidationRegistry = Pixar.UsdValidationRegistry

public extension Usd
{
  /// Validator for checking USD scene compliance.
  typealias Validator = UsdValidator

  /// Error type returned by validation operations.
  typealias ValidationError = UsdValidationError

  /// Registry for managing USD validators.
  typealias ValidationRegistry = UsdValidationRegistry
}

// MARK: - OpenUSD 25.11 Namespace Editor

public typealias UsdNamespaceEditor = Pixar.UsdNamespaceEditor

public extension Usd
{
  /// Editor for batch namespace operations on USD stages (new in 25.11).
  typealias NamespaceEditor = UsdNamespaceEditor
}

// MARK: - OpenUSD 25.11 Color Space API

public typealias UsdColorSpaceAPI = Pixar.UsdColorSpaceAPI
public typealias UsdColorSpaceDefinitionAPI = Pixar.UsdColorSpaceDefinitionAPI

public extension Usd
{
  /// API schema for color space metadata on prims.
  typealias ColorSpaceAPI = UsdColorSpaceAPI

  /// API schema for defining custom color spaces.
  typealias ColorSpaceDefinitionAPI = UsdColorSpaceDefinitionAPI
}

// MARK: - OpenUSD 25.11 Attribute Limits

public typealias UsdAttributeLimits = Pixar.UsdAttributeLimits

public extension Usd
{
  /// Utilities for attribute value limits and clamping.
  typealias AttributeLimits = UsdAttributeLimits
}
