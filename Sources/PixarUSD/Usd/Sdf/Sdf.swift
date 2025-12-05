/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Sdf

/**
 * # ``Sdf``
 *
 * **Scene Description Foundations**
 *
 * ## Overview
 *
 * **Sdf** provides the foundations for serializing scene description to a reference text format,
 * or a multitude of plugin-defined formats.  It also provides the primitive abstractions for
 * interacting with scene description, such as ``SdfPath``, ``SdfLayer``, ``SdfPrimSpec``. */
public enum Sdf
{}

// MARK: - OpenUSD 25.11 File Format Utilities (moved from Usd)

public typealias SdfCrateInfo = Pixar.SdfCrateInfo
public typealias SdfZipFile = Pixar.SdfZipFile
public typealias SdfZipFileWriter = Pixar.SdfZipFileWriter

public extension Sdf
{
  /// Introspection class for .usdc crate files.
  typealias CrateInfo = SdfCrateInfo

  /// Class for reading zip files (supports .usdz).
  typealias ZipFile = SdfZipFile

  /// Class for writing zip files (supports .usdz).
  typealias ZipFileWriter = SdfZipFileWriter
}

// MARK: - OpenUSD 25.11 Expression APIs

public typealias SdfBooleanExpression = Pixar.SdfBooleanExpression
public typealias SdfVariableExpression = Pixar.SdfVariableExpression

public extension Sdf
{
  /// Boolean expression for conditional evaluation (new in 25.11).
  typealias BooleanExpression = SdfBooleanExpression

  /// Variable expression for dynamic string substitution (new in 25.11).
  typealias VariableExpression = SdfVariableExpression
}
