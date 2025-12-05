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
import Vt

// MARK: - ArWriteMode Swift Enum

/// Top-level typealias for the C++ WriteMode enum.
public typealias ArWriteMode = Pixar.ArResolver.WriteMode

/// Enumeration of write modes for opening assets for write.
public enum ArResolverWriteMode
{
  /// Open asset for in-place updates. If the asset exists, its contents
  /// will not be discarded and writes may overwrite existing data.
  case update

  /// Open asset for replacement. If the asset exists, its contents will
  /// be discarded by the time the ArWritableAsset is destroyed.
  case replace

  /// Convert to C++ WriteMode
  public var cxxValue: ArWriteMode
  {
    switch self
    {
      case .update: return .Update
      case .replace: return .Replace
    }
  }
}

public extension Ar
{
  /// Swift-friendly write mode enum.
  typealias ResolverWriteMode = ArResolverWriteMode
}

// MARK: - ArResolver Extensions

public extension ArResolver
{
  // MARK: - Identifier Creation

  /// Returns an identifier for the asset specified by the given path.
  ///
  /// Identifiers are canonicalized asset paths that may be assigned to a logical
  /// asset to facilitate comparisons and lookups.
  ///
  /// - Parameters:
  ///   - assetPath: The asset path to create an identifier for
  ///   - anchorAssetPath: Optional resolved path to anchor relative paths to
  /// - Returns: A canonicalized identifier string
  func createIdentifier(
    _ assetPath: String,
    anchoredTo anchorAssetPath: ArResolvedPath = ArResolvedPath()
  ) -> String
  {
    String(CreateIdentifier(std.string(assetPath), anchorAssetPath))
  }

  /// Returns an identifier for a new asset specified by the given path.
  ///
  /// - Parameters:
  ///   - assetPath: The asset path for the new asset
  ///   - anchorAssetPath: Optional resolved path to anchor relative paths to
  /// - Returns: A canonicalized identifier string for the new asset
  func createIdentifierForNewAsset(
    _ assetPath: String,
    anchoredTo anchorAssetPath: ArResolvedPath = ArResolvedPath()
  ) -> String
  {
    String(CreateIdentifierForNewAsset(std.string(assetPath), anchorAssetPath))
  }

  // MARK: - Path Resolution

  /// Returns the resolved path for the asset identified by the given path.
  ///
  /// If the asset does not exist, returns an empty ArResolvedPath.
  ///
  /// - Parameter assetPath: The asset path to resolve
  /// - Returns: The resolved path, or empty if asset doesn't exist
  func resolve(_ assetPath: String) -> ArResolvedPath
  {
    Resolve(std.string(assetPath))
  }

  /// Returns the resolved path for a new asset that may be created.
  ///
  /// Note that an asset might or might not already exist at the returned path.
  ///
  /// - Parameter assetPath: The asset path to resolve for new asset creation
  /// - Returns: The resolved path where the new asset can be created
  func resolveForNewAsset(_ assetPath: String) -> ArResolvedPath
  {
    ResolveForNewAsset(std.string(assetPath))
  }

  // MARK: - Context Operations

  /// Binds the given context to this resolver.
  ///
  /// - Note: Clients should generally use `ArResolverContextBinder` instead.
  ///
  /// - Parameters:
  ///   - context: The context to bind
  ///   - bindingData: Output parameter for binding data
  func bindContext(_ context: ArResolverContext, bindingData: inout Vt.Value)
  {
    BindContext(context, &bindingData)
  }

  /// Unbinds the given context from this resolver.
  ///
  /// - Note: Clients should generally use `ArResolverContextBinder` instead.
  ///
  /// - Parameters:
  ///   - context: The context to unbind
  ///   - bindingData: The binding data from the corresponding bind call
  func unbindContext(_ context: ArResolverContext, bindingData: inout Vt.Value)
  {
    UnbindContext(context, &bindingData)
  }

  /// Return an ArResolverContext that may be bound to this resolver
  /// to resolve assets when no other context is explicitly specified.
  ///
  /// - Returns: A default resolver context
  func createDefaultContext() -> ArResolverContext
  {
    CreateDefaultContext()
  }

  /// Return an ArResolverContext that may be bound to this resolver
  /// to resolve the asset located at the given path.
  ///
  /// - Parameter assetPath: The asset path to create context for
  /// - Returns: A resolver context appropriate for the given asset
  func createDefaultContext(forAsset assetPath: String) -> ArResolverContext
  {
    CreateDefaultContextForAsset(std.string(assetPath))
  }

  /// Return an ArResolverContext created from the primary ArResolver
  /// implementation using the given context string.
  ///
  /// - Parameter contextStr: String representation of context
  /// - Returns: A resolver context created from the string
  func createContext(fromString contextStr: String) -> ArResolverContext
  {
    CreateContextFromString(std.string(contextStr))
  }

  /// Return an ArResolverContext for a specific URI scheme.
  ///
  /// - Parameters:
  ///   - uriScheme: The URI scheme (empty for primary resolver)
  ///   - contextStr: String representation of context
  /// - Returns: A resolver context for the given scheme
  func createContext(
    forScheme uriScheme: String,
    fromString contextStr: String
  ) -> ArResolverContext
  {
    CreateContextFromString(std.string(uriScheme), std.string(contextStr))
  }

  /// Refresh any caches associated with the given context.
  ///
  /// - Warning: Avoid calling this on contexts that are currently bound.
  ///
  /// - Parameter context: The context to refresh
  func refreshContext(_ context: ArResolverContext)
  {
    RefreshContext(context)
  }

  /// Returns the asset resolver context currently bound in this thread.
  ///
  /// - Returns: The currently bound context
  func getCurrentContext() -> ArResolverContext
  {
    GetCurrentContext()
  }

  /// Returns true if the given path is context-dependent.
  ///
  /// A context-dependent path may resolve differently depending on the bound context.
  ///
  /// - Parameter assetPath: The path to check
  /// - Returns: True if the path is context-dependent
  func isContextDependentPath(_ assetPath: String) -> Bool
  {
    IsContextDependentPath(std.string(assetPath))
  }

  // MARK: - File/Asset Operations

  /// Returns the file extension for the given asset path.
  ///
  /// The returned extension does not include a "." at the beginning.
  ///
  /// - Parameter assetPath: The asset path
  /// - Returns: The file extension without leading dot
  func getExtension(_ assetPath: String) -> String
  {
    String(GetExtension(std.string(assetPath)))
  }

  /// Returns asset info populated with metadata about the asset.
  ///
  /// - Parameters:
  ///   - assetPath: The asset path
  ///   - resolvedPath: The resolved path for the asset
  /// - Returns: Asset info structure with metadata
  func getAssetInfo(
    _ assetPath: String,
    resolvedPath: ArResolvedPath
  ) -> ArAssetInfo
  {
    GetAssetInfo(std.string(assetPath), resolvedPath)
  }

  /// Returns the modification timestamp for the asset.
  ///
  /// - Parameters:
  ///   - assetPath: The asset path
  ///   - resolvedPath: The resolved path for the asset
  /// - Returns: The modification timestamp, or invalid if unavailable
  func getModificationTimestamp(
    _ assetPath: String,
    resolvedPath: ArResolvedPath
  ) -> ArTimestamp
  {
    GetModificationTimestamp(std.string(assetPath), resolvedPath)
  }

  // Note: openAsset() and openAssetForWrite() are not wrapped because
  // ArAsset and ArWritableAsset are abstract C++ classes that are not
  // exported to Swift. These would require SWIFT_IMMORTAL_REFERENCE
  // annotations in the C++ headers to be accessible from Swift.

  /// Returns true if an asset may be written to the given path.
  ///
  /// - Parameter resolvedPath: The resolved path to check
  /// - Returns: True if writing is allowed
  func canWriteAssetToPath(_ resolvedPath: ArResolvedPath) -> Bool
  {
    CanWriteAssetToPath(resolvedPath, nil)
  }

  /// Returns true if an asset may be written to the given path.
  ///
  /// - Parameters:
  ///   - resolvedPath: The resolved path to check
  ///   - reason: Output parameter for the reason if writing is not allowed
  /// - Returns: True if writing is allowed
  func canWriteAssetToPath(_ resolvedPath: ArResolvedPath, reason: inout String) -> Bool
  {
    var cxxReason = std.string()
    let result = CanWriteAssetToPath(resolvedPath, &cxxReason)
    reason = String(cxxReason)
    return result
  }

  // MARK: - Cache Scope Operations

  /// Mark the start of a resolution caching scope.
  ///
  /// - Note: Clients should generally use `ArResolverScopedCache` instead.
  ///
  /// - Parameter cacheScopeData: Data for the cache scope
  func beginCacheScope(_ cacheScopeData: inout Vt.Value)
  {
    BeginCacheScope(&cacheScopeData)
  }

  /// Mark the end of a resolution caching scope.
  ///
  /// - Note: Clients should generally use `ArResolverScopedCache` instead.
  ///
  /// - Parameter cacheScopeData: Data from the corresponding begin call
  func endCacheScope(_ cacheScopeData: inout Vt.Value)
  {
    EndCacheScope(&cacheScopeData)
  }

  // MARK: - Deprecated APIs

  /// Returns true if the given path is a repository path.
  @available(*, deprecated, message: "This API is deprecated in Ar 2.0")
  func isRepositoryPath(_ path: String) -> Bool
  {
    IsRepositoryPath(std.string(path))
  }
}
