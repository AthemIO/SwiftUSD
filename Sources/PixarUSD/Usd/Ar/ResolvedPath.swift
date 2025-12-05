/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Ar
import CxxStdlib
import Foundation

public extension ArResolvedPath
{
  // MARK: - Constructors

  /// Construct a resolved path from a Swift string.
  ///
  /// - Parameter resolvedPath: The resolved path string
  init(_ resolvedPath: String)
  {
    self.init(std.string(resolvedPath))
  }

  /// Construct an empty resolved path.
  static func empty() -> ArResolvedPath
  {
    ArResolvedPath()
  }

  // MARK: - Path String Access

  /// Internal helper to safely copy the path string.
  private borrowing func GetPathStringCopy() -> std.string
  {
    __GetPathStringUnsafe().pointee
  }

  /// The resolved path as a Swift String.
  var path: String
  {
    String(GetPathStringCopy())
  }

  /// The resolved path as a Swift String (alias for `path`).
  var pathString: String
  {
    path
  }

  // MARK: - State Queries

  /// Returns true if this resolved path is empty.
  var isEmpty: Bool
  {
    IsEmpty()
  }

  /// Returns true if this resolved path is non-empty (valid).
  var isValid: Bool
  {
    !IsEmpty()
  }

  // MARK: - Hashing

  /// Return hash value for this resolved path.
  var hash: Int
  {
    Int(GetHash())
  }
}

// MARK: - CustomStringConvertible

extension ArResolvedPath: CustomStringConvertible
{
  public var description: String
  {
    path
  }
}

// MARK: - Equatable Support

public extension ArResolvedPath
{
  /// Compare this resolved path to a Swift string.
  static func == (lhs: ArResolvedPath, rhs: String) -> Bool
  {
    lhs.path == rhs
  }

  static func != (lhs: ArResolvedPath, rhs: String) -> Bool
  {
    lhs.path != rhs
  }
}
