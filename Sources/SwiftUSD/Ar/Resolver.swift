// Resolver.swift - ArResolver wrapper (global asset resolver)
// Mirrors: pxr/usd/ar/resolver.h

#if canImport(Darwin)
import Darwin
#elseif canImport(Glibc)
import Glibc
#elseif canImport(WinSDK)
import WinSDK
#endif
import OpenUSDInterop

// MARK: - Resolver

/// The asset resolver interface.
///
/// An asset resolver is responsible for resolving asset information
/// (including the asset's physical path) from a logical path.
///
/// This type provides access to the global resolver instance via
/// static methods. The resolver is configured automatically at runtime.
///
/// Mirrors `pxr::ArResolver` (accessed via `ArGetResolver()`) from the USD C++ API.
public enum Resolver {

    // MARK: - Identifier Operations

    /// Returns an identifier for the asset specified by the given path.
    ///
    /// If `anchorAssetPath` is provided, it is used as the anchor for
    /// relative paths.
    ///
    /// Identifiers are canonicalized asset paths that may be assigned
    /// to a logical asset to facilitate comparisons and lookups.
    ///
    /// - Parameters:
    ///   - assetPath: The asset path to create an identifier for.
    ///   - anchorAssetPath: Optional resolved path to anchor relative paths.
    /// - Returns: The identifier string, or `nil` on failure.
    public static func createIdentifier(
        for assetPath: String,
        anchorAssetPath: ResolvedPath? = nil
    ) -> String? {
        let cStr = Ar_CreateIdentifier(assetPath, anchorAssetPath?.handle)
        guard let cStr = cStr else { return nil }
        defer { Ar_FreeString(cStr) }
        return String(cString: cStr)
    }

    /// Returns an identifier for a new asset at the given path.
    ///
    /// This is similar to `createIdentifier` but is used for assets
    /// that may not exist yet and are being created.
    ///
    /// - Parameters:
    ///   - assetPath: The asset path for the new asset.
    ///   - anchorAssetPath: Optional resolved path to anchor relative paths.
    /// - Returns: The identifier string, or `nil` on failure.
    public static func createIdentifierForNewAsset(
        for assetPath: String,
        anchorAssetPath: ResolvedPath? = nil
    ) -> String? {
        let cStr = Ar_CreateIdentifierForNewAsset(assetPath, anchorAssetPath?.handle)
        guard let cStr = cStr else { return nil }
        defer { Ar_FreeString(cStr) }
        return String(cString: cStr)
    }

    // MARK: - Resolution Operations

    /// Resolves the given asset path to a resolved path.
    ///
    /// - Parameter assetPath: The asset path to resolve.
    /// - Returns: The resolved path, or `nil` if the asset does not exist.
    public static func resolve(_ assetPath: String) -> ResolvedPath? {
        guard let ref = Ar_Resolve(assetPath) else {
            return nil
        }
        return ResolvedPath(handle: ref)
    }

    /// Resolves the given asset path for creating a new asset.
    ///
    /// - Parameter assetPath: The asset path for the new asset.
    /// - Returns: The resolved path where the asset can be created,
    ///   or `nil` if such a path cannot be computed.
    public static func resolveForNewAsset(_ assetPath: String) -> ResolvedPath? {
        guard let ref = Ar_ResolveForNewAsset(assetPath) else {
            return nil
        }
        return ResolvedPath(handle: ref)
    }

    // MARK: - Context Operations

    /// Creates a default resolver context.
    ///
    /// The returned context may be bound to this resolver to resolve
    /// assets when no other context is explicitly specified.
    ///
    /// - Returns: A default resolver context.
    /// - Throws: `ResolverError.contextCreationFailed` if creation fails.
    public static func createDefaultContext() throws -> ResolverContext {
        guard let ref = Ar_CreateDefaultContext() else {
            throw ResolverError.contextCreationFailed("Failed to create default context")
        }
        return ResolverContext(handle: ref)
    }

    /// Creates a default resolver context for the given asset.
    ///
    /// The returned context may be bound to resolve the asset located
    /// at the given path or assets referenced by that asset.
    ///
    /// - Parameter assetPath: The asset path.
    /// - Returns: A default resolver context for the asset.
    /// - Throws: `ResolverError.contextCreationFailed` if creation fails.
    public static func createDefaultContext(for assetPath: String) throws -> ResolverContext {
        guard let ref = Ar_CreateDefaultContextForAsset(assetPath) else {
            throw ResolverError.contextCreationFailed("Failed to create default context for asset")
        }
        return ResolverContext(handle: ref)
    }

    /// Creates a resolver context from a context string.
    ///
    /// - Parameter contextString: The context string.
    /// - Returns: A resolver context.
    /// - Throws: `ResolverError.contextCreationFailed` if creation fails.
    public static func createContext(from contextString: String) throws -> ResolverContext {
        guard let ref = Ar_CreateContextFromString(contextString) else {
            throw ResolverError.contextCreationFailed("Failed to create context from string")
        }
        return ResolverContext(handle: ref)
    }

    /// Creates a resolver context from a context string for a specific URI scheme.
    ///
    /// - Parameters:
    ///   - contextString: The context string.
    ///   - uriScheme: The URI scheme (e.g., "http", "custom"). If `nil` or empty,
    ///     uses the primary resolver.
    /// - Returns: A resolver context.
    /// - Throws: `ResolverError.contextCreationFailed` if creation fails.
    public static func createContext(
        from contextString: String,
        forScheme uriScheme: String?
    ) throws -> ResolverContext {
        guard let ref = Ar_CreateContextFromStringForScheme(uriScheme, contextString) else {
            throw ResolverError.contextCreationFailed("Failed to create context from string for scheme")
        }
        return ResolverContext(handle: ref)
    }

    /// Refreshes any caches associated with the given context.
    ///
    /// If doing so would invalidate asset paths that had previously been
    /// resolved, notifications will be sent to inform clients of this.
    ///
    /// - Warning: Avoid calling this on a context that is currently bound.
    ///   Unbind the context before refreshing it.
    ///
    /// - Parameter context: The context to refresh.
    /// - Throws: `ResolverError.operationFailed` if the refresh fails.
    public static func refreshContext(_ context: ResolverContext) throws {
        let result = Ar_RefreshContext(context.handle)
        if result != USD_RESULT_SUCCESS {
            throw ResolverError.operationFailed("Failed to refresh context")
        }
    }

    /// Returns the currently bound resolver context.
    ///
    /// - Returns: The current context, or `nil` on failure.
    public static var currentContext: ResolverContext? {
        guard let ref = Ar_GetCurrentContext() else {
            return nil
        }
        return ResolverContext(handle: ref)
    }

    /// Returns `true` if the given asset path is context-dependent.
    ///
    /// A context-dependent path may result in different resolved paths
    /// depending on what resolver context is bound when `resolve` is called.
    ///
    /// - Parameter assetPath: The asset path to check.
    /// - Returns: `true` if the path is context-dependent.
    public static func isContextDependentPath(_ assetPath: String) -> Bool {
        Ar_IsContextDependentPath(assetPath)
    }

    // MARK: - File/Asset Operations

    /// Returns the file extension for the given asset path.
    ///
    /// The returned extension does not include a "." at the beginning.
    ///
    /// - Parameter assetPath: The asset path.
    /// - Returns: The file extension, or an empty string if none.
    public static func getExtension(for assetPath: String) -> String {
        guard let cStr = Ar_GetExtension(assetPath) else {
            return ""
        }
        defer { Ar_FreeString(cStr) }
        return String(cString: cStr)
    }

    /// Returns the modification timestamp for the given asset.
    ///
    /// - Parameters:
    ///   - assetPath: The asset path.
    ///   - resolvedPath: The resolved path for the asset.
    /// - Returns: The modification timestamp, which may be invalid if
    ///   a timestamp cannot be retrieved.
    public static func getModificationTimestamp(
        for assetPath: String,
        resolvedPath: ResolvedPath
    ) -> Timestamp {
        let cTimestamp = Ar_GetModificationTimestamp(assetPath, resolvedPath.handle)
        return Timestamp(cTimestamp: cTimestamp)
    }

    /// Checks if an asset can be written to the given resolved path.
    ///
    /// - Parameter resolvedPath: The resolved path to check.
    /// - Returns: A tuple containing whether writing is allowed and
    ///   an optional reason if not.
    public static func canWriteAsset(to resolvedPath: ResolvedPath) -> (canWrite: Bool, reason: String?) {
        var whyNot: UnsafeMutablePointer<CChar>? = nil
        let canWrite = Ar_CanWriteAssetToPath(resolvedPath.handle, &whyNot)

        var reason: String? = nil
        if let whyNot = whyNot {
            reason = String(cString: whyNot)
            Ar_FreeString(whyNot)
        }

        return (canWrite, reason)
    }

    // MARK: - URI Scheme Operations

    /// Returns the list of registered URI schemes.
    ///
    /// Schemes are returned in all lower-case and in alphabetically sorted order.
    public static var registeredURISchemes: [String] {
        let count = Ar_GetRegisteredURISchemeCount()
        guard count > 0 else { return [] }

        var cStrings = [UnsafeMutablePointer<CChar>?](repeating: nil, count: count)
        let actualCount = cStrings.withUnsafeMutableBufferPointer { buffer in
            Ar_GetRegisteredURISchemes(buffer.baseAddress, count)
        }

        var result = [String]()
        result.reserveCapacity(actualCount)
        for i in 0..<actualCount {
            if let cStr = cStrings[i] {
                result.append(String(cString: cStr))
                free(cStr)
            }
        }
        return result
    }

    // MARK: - Default Resolver Configuration

    /// Sets the default search path for the default resolver.
    ///
    /// Calling this function will trigger notifications if the search path
    /// differs from the currently set default value.
    ///
    /// - Warning: This function is not thread-safe and should not be called
    ///   concurrently with any other resolver operations.
    ///
    /// - Parameter searchPaths: The search paths to set.
    /// - Throws: `ResolverError.operationFailed` if setting fails.
    public static func setDefaultSearchPath(_ searchPaths: [String]) throws {
        let cStrings: [UnsafePointer<CChar>?] = searchPaths.map { $0.withCString { UnsafePointer(strdup($0)) } }
        defer {
            for cStr in cStrings {
                if let ptr = cStr {
                    free(UnsafeMutablePointer(mutating: ptr))
                }
            }
        }

        let result = cStrings.withUnsafeBufferPointer { buffer in
            Ar_SetDefaultSearchPath(buffer.baseAddress, searchPaths.count)
        }

        if result != USD_RESULT_SUCCESS {
            throw ResolverError.operationFailed("Failed to set default search path")
        }
    }
}

// MARK: - ResolverError

/// Errors that can occur when working with the resolver.
public enum ResolverError: Error, CustomStringConvertible {

    /// Context creation failed.
    case contextCreationFailed(String)

    /// An operation failed.
    case operationFailed(String)

    public var description: String {
        switch self {
        case .contextCreationFailed(let message):
            return "Context creation failed: \(message)"
        case .operationFailed(let message):
            return "Resolver operation failed: \(message)"
        }
    }
}
