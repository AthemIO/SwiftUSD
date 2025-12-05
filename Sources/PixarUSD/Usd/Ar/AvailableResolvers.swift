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

public extension Ar
{
  /// Returns a list of all available ArResolver subclass type names.
  ///
  /// This includes both the built-in default resolver and any plugin resolvers.
  ///
  /// - Returns: Array of resolver type name strings
  static func getAllResolvers() -> [String]
  {
    // 1. Get the base type for ArResolver
    let base = Pixar.TfType.FindByName("ArResolver")

    guard
      // 2. Verify the base type is valid
      let all = base.pointee.IsUnknown() == false
        // 3. Get all types that derive from the base type
        ? Pixar.TfType.GetDirectlyDerivedTypes(base.pointee) : nil,
      // 4. Ensure the list is not empty
      all().empty() == false
    else { return [] }

    // Convert C++ vector to Swift array
    var result: [String] = []
    let vec = all()
    for i in 0 ..< vec.size()
    {
      result.append(String(vec[i].GetTypeName().pointee))
    }
    return result
  }

  /// Returns list of TfType names for available ArResolver subclasses.
  ///
  /// This is the list used to determine the resolver implementation
  /// returned by `Ar.getResolver()`.
  ///
  /// - Warning: This function is not safe to call concurrently with itself
  ///   or `ArCreateResolver`.
  ///
  /// - Returns: Array of TfType names for available resolvers
  static func getAvailableResolverTypes() -> [String]
  {
    let cxxTypes = Pixar.ArGetAvailableResolvers()
    var result: [String] = []
    for i in 0 ..< cxxTypes.size()
    {
      result.append(String(cxxTypes[i].GetTypeName().pointee))
    }
    return result
  }

  /// Returns list of all registered URI schemes for resolvers.
  ///
  /// Schemes are returned in all lower-case and in alphabetically sorted order.
  ///
  /// - Returns: Array of registered URI scheme strings
  static func getRegisteredURISchemes() -> [String]
  {
    let cxxSchemes = Pixar.ArGetRegisteredURISchemes()
    var result: [String] = []
    for i in 0 ..< cxxSchemes.pointee.size()
    {
      result.append(String(cxxSchemes.pointee[i]))
    }
    return result
  }
}
