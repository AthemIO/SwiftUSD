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

// MARK: - Package Utilities

public extension Ar
{
  /// Utility functions for working with package-relative asset paths.
  ///
  /// Assets within package assets (like .usdz files) can be addressed via
  /// "package-relative" paths. For example:
  /// - `Model.package[Geom.file]`
  /// - `/path/to/Model.package[a/b/Geom.file]`
  enum Package
  {
    /// Return true if the path is a package-relative path.
    ///
    /// - Parameter path: The path to check
    /// - Returns: True if this is a package-relative path
    public static func isPackageRelativePath(_ path: String) -> Bool
    {
      Pixar.ArIsPackageRelativePath(std.string(path))
    }

    /// Combines multiple paths into a single package-relative path.
    ///
    /// Example:
    /// ```swift
    /// Ar.Package.joinPaths(["a.pack", "b.pack"])
    ///    // => "a.pack[b.pack]"
    ///
    /// Ar.Package.joinPaths(["a.pack", "b.pack", "c.pack"])
    ///    // => "a.pack[b.pack[c.pack]]"
    /// ```
    ///
    /// - Parameter paths: Array of paths to join
    /// - Returns: The combined package-relative path
    public static func joinPaths(_ paths: [String]) -> String
    {
      var cxxPaths = ArStringVector()
      for path in paths
      {
        cxxPaths.push_back(std.string(path))
      }
      return String(Pixar.ArJoinPackageRelativePath(cxxPaths))
    }

    /// Combines two paths into a package-relative path.
    ///
    /// Example:
    /// ```swift
    /// Ar.Package.joinPaths(package: "a.pack", packaged: "b.file")
    ///    // => "a.pack[b.file]"
    /// ```
    ///
    /// - Parameters:
    ///   - packagePath: The outer package path
    ///   - packagedPath: The inner packaged path
    /// - Returns: The combined package-relative path
    public static func joinPaths(
      package packagePath: String,
      packaged packagedPath: String
    ) -> String
    {
      String(
        Pixar.ArJoinPackageRelativePath(
          std.string(packagePath),
          std.string(packagedPath)
        ))
    }

    /// Split a package-relative path at the outermost level.
    ///
    /// Example:
    /// ```swift
    /// Ar.Package.splitOuter("a.pack[b.pack[c.pack]]")
    ///    // => ("a.pack", "b.pack[c.pack]")
    /// ```
    ///
    /// - Parameter path: The package-relative path to split
    /// - Returns: Tuple of (packagePath, packagedPath)
    public static func splitOuter(_ path: String) -> (package: String, packaged: String)
    {
      let result = Pixar.ArSplitPackageRelativePathOuter(std.string(path))
      return (String(result.first), String(result.second))
    }

    /// Split a package-relative path at the innermost level.
    ///
    /// Example:
    /// ```swift
    /// Ar.Package.splitInner("a.pack[b.pack[c.pack]]")
    ///    // => ("a.pack[b.pack]", "c.pack")
    /// ```
    ///
    /// - Parameter path: The package-relative path to split
    /// - Returns: Tuple of (packagePath, packagedPath)
    public static func splitInner(_ path: String) -> (package: String, packaged: String)
    {
      let result = Pixar.ArSplitPackageRelativePathInner(std.string(path))
      return (String(result.first), String(result.second))
    }
  }
}
