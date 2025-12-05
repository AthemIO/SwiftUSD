/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import CxxStdlib
import UsdMtlx

public extension UsdMtlx
{
  // MARK: - Search Paths

  /// Return the MaterialX standard library paths.
  ///
  /// All standard library files (and only standard library files)
  /// should be found on these paths.
  static func standardLibraryPaths() -> [String]
  {
    // Returns const SdrStringVec& (reference), access via pointee
    let vec = Pixar.UsdMtlxStandardLibraryPaths().pointee
    var result: [String] = []
    for i in 0 ..< vec.size()
    {
      result.append(String(vec[i]))
    }
    return result
  }

  /// Return the paths to directories containing custom MaterialX files.
  ///
  /// These paths are set via the `PXR_MTLX_PLUGIN_SEARCH_PATHS`
  /// environment variable.
  static func customSearchPaths() -> [String]
  {
    // Returns const SdrStringVec& (reference), access via pointee
    let vec = Pixar.UsdMtlxCustomSearchPaths().pointee
    var result: [String] = []
    for i in 0 ..< vec.size()
    {
      result.append(String(vec[i]))
    }
    return result
  }

  /// Return the complete MaterialX search paths.
  ///
  /// In order, this includes:
  /// - Directories containing custom MaterialX files set in the env var
  ///   `PXR_MTLX_PLUGIN_SEARCH_PATHS`
  /// - Standard library paths set in the env var `PXR_MTLX_STDLIB_SEARCH_PATHS`
  /// - Path to the MaterialX standard library discovered at build time
  static func searchPaths() -> [String]
  {
    // Returns const SdrStringVec& (reference), access via pointee
    let vec = Pixar.UsdMtlxSearchPaths().pointee
    var result: [String] = []
    for i in 0 ..< vec.size()
    {
      result.append(String(vec[i]))
    }
    return result
  }

  /// Return the MaterialX standard file extensions.
  ///
  /// Typically returns `.mtlx`.
  static func standardFileExtensions() -> [String]
  {
    // Returns SdrStringVec by value, can iterate directly
    let vec = Pixar.UsdMtlxStandardFileExtensions()
    var result: [String] = []
    for i in 0 ..< vec.size()
    {
      result.append(String(vec[i]))
    }
    return result
  }

  // MARK: - Type Conversion

  /// Convert a (standard) MaterialX type name to USD type info.
  ///
  /// - Parameter mtlxTypeName: The MaterialX type name (e.g., "color3", "float", "vector3")
  /// - Returns: Type information including the closest USD type and array size
  @discardableResult
  static func getUsdType(_ mtlxTypeName: String) -> UsdMtlxUsdTypeInfo
  {
    Pixar.UsdMtlxGetUsdType(std.string(mtlxTypeName))
  }

  // MARK: - String Utilities

  /// Split a MaterialX string array into a vector of strings.
  ///
  /// The MaterialX specification says:
  /// > Individual string values within stringarrays may not contain
  /// > commas or semicolons, and any leading and trailing whitespace
  /// > characters in them is ignored.
  ///
  /// These restrictions do not apply to the string type.
  ///
  /// - Parameter s: The packed stringarray per MaterialX spec
  /// - Returns: Array of individual strings
  static func splitStringArray(_ s: String) -> [String]
  {
    // Returns std::vector<std::string> by value
    let vec = Pixar.UsdMtlxSplitStringArray(std.string(s))
    var result: [String] = []
    for i in 0 ..< vec.size()
    {
      result.append(String(vec[i]))
    }
    return result
  }
}
