/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Ar
import CxxStdlib
import Plug

/// Type alias for std::vector<std::string> using the PlugRegistry typedef.
/// This is the proper way to create string vectors that work with Swift C++ interop.
public typealias ArStringVector = Pixar.PlugRegistry.PlugPathsVector

public extension ArDefaultResolverContext
{
  // MARK: - Initializers

  /// Construct a context with the given search paths.
  ///
  /// Elements should be absolute paths. If they are not,
  /// they will be anchored to the current working directory.
  ///
  /// - Parameter searchPaths: Array of search path strings
  init(searchPaths: [String])
  {
    var cxxPaths = ArStringVector()
    for path in searchPaths
    {
      cxxPaths.push_back(std.string(path))
    }
    self.init(cxxPaths)
  }

  /// Construct a context with variadic search paths.
  ///
  /// - Parameter searchPaths: Variadic list of search path strings
  init(searchPaths: String...)
  {
    var cxxPaths = ArStringVector()
    for path in searchPaths
    {
      cxxPaths.push_back(std.string(path))
    }
    self.init(cxxPaths)
  }

  // MARK: - Properties

  /// Return a string representation of this context for debugging.
  var debugDescription: String
  {
    String(GetAsString())
  }
}

// MARK: - CustomStringConvertible

extension ArDefaultResolverContext: CustomStringConvertible
{
  public var description: String
  {
    debugDescription
  }
}

// MARK: - Ar Namespace Extension

public extension Ar
{
  /// Create a DefaultResolverContext with the given search paths.
  ///
  /// - Parameter searchPaths: Array of search path strings
  /// - Returns: A new ArDefaultResolverContext
  static func makeDefaultResolverContext(searchPaths: [String]) -> ArDefaultResolverContext
  {
    ArDefaultResolverContext(searchPaths: searchPaths)
  }

  /// Create a DefaultResolverContext with variadic search paths.
  ///
  /// - Parameter searchPaths: Variadic list of search path strings
  /// - Returns: A new ArDefaultResolverContext
  static func makeDefaultResolverContext(searchPaths: String...) -> ArDefaultResolverContext
  {
    var cxxPaths = ArStringVector()
    for path in searchPaths
    {
      cxxPaths.push_back(std.string(path))
    }
    return ArDefaultResolverContext(cxxPaths)
  }
}
