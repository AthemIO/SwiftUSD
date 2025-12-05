/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import UsdUI

/**
 * # ``UsdUI``
 *
 * **USD User Interface Schema**
 *
 * ## Overview
 *
 * **UsdUI** provides schemas for representing UI-related information in USD,
 * including node graph layouts and the new UI hints system (25.11). */
public enum UsdUI
{}

// MARK: - OpenUSD 25.11 UI Hints System

public typealias UsdUIObjectHints = Pixar.UsdUIObjectHints
public typealias UsdUIPrimHints = Pixar.UsdUIPrimHints
public typealias UsdUIPropertyHints = Pixar.UsdUIPropertyHints
public typealias UsdUIAttributeHints = Pixar.UsdUIAttributeHints

public extension UsdUI
{
  /// Base class for UI hints on any USD object.
  typealias ObjectHints = UsdUIObjectHints

  /// UI hints specific to USD prims.
  typealias PrimHints = UsdUIPrimHints

  /// UI hints specific to USD properties.
  typealias PropertyHints = UsdUIPropertyHints

  /// UI hints specific to USD attributes.
  typealias AttributeHints = UsdUIAttributeHints
}
