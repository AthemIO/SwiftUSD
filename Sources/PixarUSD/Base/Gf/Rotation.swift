/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Gf

/* note: the typealiases are documented the same way twice,
 * keep it like this so that sourcekit shows documentation
 * regardless of which typealias a user might use in their
 * code. */

/**
 * # GfRotation
 *
 * Basic type for a 3-space rotation specification.
 */
public typealias GfRotation = Pixar.GfRotation

public extension Gf
{
  /**
   * # GfRotation
   *
   * Basic type for a 3-space rotation specification.
   */
  typealias Rotation = GfRotation
}

public extension Gf.Rotation
{
  static func * (lhs: Gf.Rotation, rhs: Gf.Rotation) -> Gf.Rotation
  {
    var result = lhs
    result *= rhs
    return result
  }

  static func * (rotation: Gf.Rotation, scale: Double) -> Gf.Rotation
  {
    var result = rotation
    result *= scale
    return result
  }
}
