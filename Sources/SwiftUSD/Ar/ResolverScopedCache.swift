// ResolverScopedCache.swift - ArResolverScopedCache wrapper
// Mirrors: pxr/usd/ar/resolverScopedCache.h

import OpenUSDInterop

// MARK: - ResolverScopedCache

/// A scoped cache that enables caching of resolver queries within a scope.
///
/// This class begins caching when created and ends caching when released.
/// The resolver may cache results of path resolution and other queries
/// within the scope of this object, which can improve performance when
/// multiple resolution operations are performed.
///
/// Example usage:
/// ```swift
/// // Enable caching for multiple resolution operations
/// let cache = try ResolverScopedCache()
///
/// // These operations may benefit from caching
/// let resolved1 = Resolver.resolve("asset1.usd")
/// let resolved2 = Resolver.resolve("asset2.usd")
/// let resolved3 = Resolver.resolve("asset1.usd")  // May use cached result
///
/// // Caching ends when cache goes out of scope
/// ```
///
/// - Note: Caching behavior depends on the resolver implementation.
///   Some resolvers may not implement caching.
///
/// Mirrors `pxr::ArResolverScopedCache` from the USD C++ API.
public final class ResolverScopedCache: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    private let handle: ArResolverScopedCacheRef

    // MARK: - Initialization

    /// Creates a scoped cache and begins caching.
    ///
    /// Caching will continue until this object is deallocated.
    ///
    /// - Throws: `ResolverScopedCacheError.creationFailed` if the cache
    ///   cannot be created.
    public init() throws {
        guard let ref = ArResolverScopedCache_Create() else {
            throw ResolverScopedCacheError.creationFailed("Failed to create scoped cache")
        }
        self.handle = ref
    }

    deinit {
        ArResolverScopedCache_Release(handle)
    }

    // MARK: - Convenience Methods

    /// Executes a closure with caching enabled.
    ///
    /// This method provides a convenient way to enable caching for a
    /// specific scope and ensures caching ends when the closure completes.
    ///
    /// Example usage:
    /// ```swift
    /// let results = try ResolverScopedCache.withCaching {
    ///     // Resolution operations here may benefit from caching
    ///     return [
    ///         Resolver.resolve("model1.usd"),
    ///         Resolver.resolve("model2.usd"),
    ///         Resolver.resolve("model1.usd")  // May use cached result
    ///     ]
    /// }
    /// ```
    ///
    /// - Parameter body: The closure to execute with caching enabled.
    /// - Returns: The result of the closure.
    /// - Throws: Rethrows any error from the closure, or
    ///   `ResolverScopedCacheError.creationFailed` if caching cannot be started.
    public static func withCaching<T>(perform body: () throws -> T) throws -> T {
        let cache = try ResolverScopedCache()
        _ = cache // Keep cache alive for the scope
        return try body()
    }

    /// Executes an async closure with caching enabled.
    ///
    /// - Parameter body: The async closure to execute with caching enabled.
    /// - Returns: The result of the closure.
    /// - Throws: Rethrows any error from the closure, or
    ///   `ResolverScopedCacheError.creationFailed` if caching cannot be started.
    @available(macOS 10.15, iOS 13.0, watchOS 6.0, tvOS 13.0, *)
    public static func withCaching<T>(perform body: () async throws -> T) async throws -> T {
        let cache = try ResolverScopedCache()
        _ = cache // Keep cache alive for the scope
        return try await body()
    }
}

// MARK: - ResolverScopedCacheError

/// Errors that can occur when working with resolver scoped caches.
public enum ResolverScopedCacheError: Error, CustomStringConvertible {

    /// Cache creation failed.
    case creationFailed(String)

    public var description: String {
        switch self {
        case .creationFailed(let message):
            return "Resolver scoped cache creation failed: \(message)"
        }
    }
}
