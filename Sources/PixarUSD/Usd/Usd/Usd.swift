/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
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

// Note: For full validation support, see the UsdValidation module.
// The validation framework provides validators for checking USD content
// against various rules and best practices.
//
// Example usage:
//   import UsdValidation
//   let context = UsdValidation.Context(keywords: [Tf.Token("UsdCoreValidators")])
//   let errors = context.validate(stage)

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
