/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import UsdMtlx

/**
 * # ``UsdMtlx``
 *
 * **USD MaterialX Integration**
 *
 * ## Overview
 *
 * **UsdMtlx** provides utilities for translating MaterialX documents
 * to USD scene description. MaterialX is an open standard for representing
 * rich material and look-development content in computer graphics.
 *
 * ## Key Features
 *
 * - Convert MaterialX documents to USD shading networks
 * - Query MaterialX search paths and type mappings
 * - Access MaterialX standard library definitions
 */
public enum UsdMtlx
{}

// MARK: - Type Conversion Utilities

public typealias UsdMtlxUsdTypeInfo = Pixar.UsdMtlxUsdTypeInfo

public extension UsdMtlx
{
  typealias UsdTypeInfo = UsdMtlxUsdTypeInfo
}
