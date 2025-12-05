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
import Plug

public extension ArDefaultResolver
{
  /// Set the default search path that will be used during asset resolution.
  ///
  /// This function is not thread-safe and should not be called concurrently
  /// with any other ArResolver operations.
  ///
  /// - Parameter searchPath: Array of search path strings (should be absolute paths)
  static func setDefaultSearchPath(_ searchPath: [String])
  {
    var cxxPaths = ArStringVector()
    for path in searchPath
    {
      cxxPaths.push_back(std.string(path))
    }
    ArDefaultResolver.SetDefaultSearchPath(cxxPaths)
  }

  /// Set the default search path that will be used during asset resolution.
  ///
  /// This function is not thread-safe and should not be called concurrently
  /// with any other ArResolver operations.
  ///
  /// - Parameter searchPaths: Variadic list of search path strings
  static func setDefaultSearchPath(_ searchPaths: String...)
  {
    var cxxPaths = ArStringVector()
    for path in searchPaths
    {
      cxxPaths.push_back(std.string(path))
    }
    ArDefaultResolver.SetDefaultSearchPath(cxxPaths)
  }
}

// MARK: - Ar.DefaultResolver Helpers

public extension Ar
{
  /// Set the default search path for the default resolver.
  ///
  /// This is a convenience function for `ArDefaultResolver.setDefaultSearchPath()`.
  ///
  /// - Parameter searchPath: Array of search path strings
  static func setDefaultSearchPath(_ searchPath: [String])
  {
    ArDefaultResolver.setDefaultSearchPath(searchPath)
  }

  /// Set the default search path for the default resolver.
  ///
  /// - Parameter searchPaths: Variadic list of search path strings
  static func setDefaultSearchPath(_ searchPaths: String...)
  {
    var cxxPaths = ArStringVector()
    for path in searchPaths
    {
      cxxPaths.push_back(std.string(path))
    }
    ArDefaultResolver.SetDefaultSearchPath(cxxPaths)
  }
}
