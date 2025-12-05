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

// MARK: - Core Type Aliases

public typealias ArResolver = Pixar.ArResolver
public typealias ArDefaultResolver = Pixar.ArDefaultResolver
public typealias ArResolvedPath = Pixar.ArResolvedPath
public typealias ArResolverContext = Pixar.ArResolverContext
public typealias ArResolverContextBinder = Pixar.ArResolverContextBinder
public typealias ArTimestamp = Pixar.ArTimestamp
public typealias ArAssetInfo = Pixar.ArAssetInfo
public typealias ArDefaultResolverContext = Pixar.ArDefaultResolverContext

// Asset types - now exported with SWIFT_IMMORTAL_REFERENCE annotations
public typealias ArAsset = Pixar.ArAsset
public typealias ArWritableAsset = Pixar.ArWritableAsset
public typealias ArFilesystemAsset = Pixar.ArFilesystemAsset
public typealias ArInMemoryAsset = Pixar.ArInMemoryAsset
public typealias ArFilesystemWritableAsset = Pixar.ArFilesystemWritableAsset

// Note: ArResolverScopedCache is an RAII type that cannot be directly exported to Swift.
// Use the ArScopedCache Swift wrapper class instead, or the Ar.withCacheScope() function.

/**
 * # ``Ar``
 *
 * **Asset Resolution**
 *
 * ## Overview
 *
 * **Ar** is the **asset resolution** library, and is responsible for querying, reading, and
 * writing asset data. It provides several interfaces that allow **USD** to access
 * an asset without knowing how that asset is physically stored.
 *
 * ## Key Types
 *
 * - ``Resolver``: The main interface for resolving asset paths
 * - ``ResolvedPath``: A resolved asset path value type
 * - ``ResolverContext``: Context for controlling resolution behavior
 * - ``Asset``: Interface for reading asset data
 * - ``WritableAsset``: Interface for writing asset data
 * - ``Timestamp``: Asset modification timestamp
 * - ``AssetInfo``: Metadata about a resolved asset
 */
public enum Ar
{
  public typealias Resolver = ArResolver
  public typealias DefaultResolver = ArDefaultResolver
  public typealias ResolvedPath = ArResolvedPath
  public typealias ResolverContext = ArResolverContext
  public typealias ResolverContextBinder = ArResolverContextBinder
  public typealias Timestamp = ArTimestamp
  public typealias AssetInfo = ArAssetInfo
  public typealias DefaultResolverContext = ArDefaultResolverContext

  // Asset types - now exported with SWIFT_IMMORTAL_REFERENCE annotations
  public typealias Asset = ArAsset
  public typealias WritableAsset = ArWritableAsset
  public typealias FilesystemAsset = ArFilesystemAsset
  public typealias InMemoryAsset = ArInMemoryAsset
  public typealias FilesystemWritableAsset = ArFilesystemWritableAsset

  // Note: ArResolverScopedCache is an RAII type - use ArScopedCache Swift wrapper instead
}

// MARK: - Global Functions

public extension Ar
{
  /// Returns the configured asset resolver.
  ///
  /// When first called, this function will determine the ArResolver subclass
  /// to use for asset resolution. The resolved ArResolver subclass will be cached
  /// and used to service function calls made on the returned resolver.
  ///
  /// - Returns: Reference to the configured asset resolver
  static func getResolver() -> ArResolver
  {
    Pixar.ArGetResolver()
  }

  /// Set the preferred ArResolver subclass used by `getResolver()`.
  ///
  /// Consumers may override ArGetResolver's plugin resolver discovery and
  /// force the use of a specific resolver subclass by calling this
  /// function with the typename of the implementation to use.
  ///
  /// This must be called before the first call to `getResolver()`.
  ///
  /// - Parameter resolverTypeName: The type name of the resolver to use
  static func setPreferredResolver(_ resolverTypeName: String)
  {
    Pixar.ArSetPreferredResolver(std.string(resolverTypeName))
  }

  /// Returns the underlying ArResolver instance used by `getResolver()`.
  ///
  /// This function returns the instance of the ArResolver subclass used by
  /// ArGetResolver and can be dynamic_cast to that type.
  ///
  /// - Warning: This function should typically not be used by consumers except
  ///   in very specific cases.
  static func getUnderlyingResolver() -> ArResolver
  {
    Pixar.ArGetUnderlyingResolver()
  }
}
