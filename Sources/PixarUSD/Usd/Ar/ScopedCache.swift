/* ----------------------------------------------------------------
 *  A T H E M
 * ----------------------------------------------------------------
 *  Copyright (C) 2016 Pixar.
 *  Copyright (C) 2025 Afloat Technologies. All Rights Reserved.
 *  Licensed under https://openusd.org/license
 * ---------------------------------------------------------------- */

import Ar

/// Swift-friendly wrapper for ArResolverScopedCache.
///
/// This class provides RAII-like cache scope management in Swift.
/// The cache scope is active from creation until `end()` is called
/// or the object is deinitialized.
///
/// Example:
/// ```swift
/// let cache = Ar.ScopedCache()
/// defer { cache.end() }
///
/// // Resolver calls here benefit from caching
/// let path1 = resolver.resolve("asset1.usd")
/// let path2 = resolver.resolve("asset2.usd")
/// ```
public final class ArScopedCache
{
  private var handle: UnsafeMutableRawPointer?
  private var hasEnded: Bool = false

  /// Begin a new resolver cache scope.
  public init()
  {
    handle = Pixar.ArSwift_BeginCacheScope()
  }

  /// Begin a cache scope that shares data with a parent scope.
  ///
  /// - Parameter parent: The parent cache scope
  public init(sharingWith parent: ArScopedCache)
  {
    handle = Pixar.ArSwift_BeginCacheScopeWithParent(parent.handle)
  }

  deinit
  {
    end()
  }

  /// End the cache scope.
  ///
  /// This is called automatically on deinit, but can be called
  /// earlier if you need to end the scope before the object goes
  /// out of scope.
  public func end()
  {
    guard !hasEnded, let h = handle else { return }
    Pixar.ArSwift_EndCacheScope(h)
    handle = nil
    hasEnded = true
  }

  /// Execute a closure within this cache scope.
  ///
  /// The cache scope is guaranteed to remain active for the
  /// duration of the closure.
  ///
  /// - Parameter body: The closure to execute
  /// - Returns: The result of the closure
  @discardableResult
  public func withScope<T>(_ body: () throws -> T) rethrows -> T
  {
    defer { end() }
    return try body()
  }
}

// MARK: - Ar Namespace Extension

public extension Ar
{
  typealias ScopedCache = ArScopedCache

  /// Execute a closure within a resolver cache scope.
  ///
  /// Example:
  /// ```swift
  /// Ar.withCacheScope {
  ///     let resolver = Ar.getResolver()
  ///     let path1 = resolver.resolve("asset1.usd")
  ///     let path2 = resolver.resolve("asset2.usd")
  ///     // Both resolutions benefit from caching
  /// }
  /// ```
  ///
  /// - Parameter body: The closure to execute
  /// - Returns: The result of the closure
  @discardableResult
  static func withCacheScope<T>(_ body: () throws -> T) rethrows -> T
  {
    let cache = ScopedCache()
    return try cache.withScope(body)
  }
}
