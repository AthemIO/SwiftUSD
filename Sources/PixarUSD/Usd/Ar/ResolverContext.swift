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

import Ar
import CxxStdlib

public extension ArResolverContext
{
  // MARK: - Factory Methods

  /// Create an empty resolver context.
  static func empty() -> ArResolverContext
  {
    ArResolverContext()
  }

  // MARK: - Properties

  /// Returns whether this resolver context is empty.
  var isEmpty: Bool
  {
    IsEmpty()
  }

  /// Returns a debug string representing the contained context objects.
  var debugDescription: String
  {
    String(GetDebugString())
  }
}

// MARK: - CustomStringConvertible

extension ArResolverContext: CustomStringConvertible
{
  public var description: String
  {
    debugDescription
  }
}
