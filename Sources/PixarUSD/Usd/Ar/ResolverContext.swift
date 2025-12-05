/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
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
