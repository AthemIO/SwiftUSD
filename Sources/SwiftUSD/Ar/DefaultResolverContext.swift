// DefaultResolverContext.swift - ArDefaultResolverContext wrapper
// Mirrors: pxr/usd/ar/defaultResolverContext.h

#if canImport(Darwin)
import Darwin
#elseif canImport(Glibc)
import Glibc
#elseif canImport(WinSDK)
import WinSDK
#endif
import OpenUSDInterop

// MARK: - DefaultResolverContext

/// Resolver context object that specifies a search path to use during
/// asset resolution.
///
/// This object is intended for use with the default `ArDefaultResolver`
/// asset resolution implementation. When bound, the search paths specified
/// in this context will be used to resolve relative asset paths.
///
/// Example usage:
/// ```swift
/// let context = try DefaultResolverContext(searchPaths: ["/Local/Models", "/Installed/Models"])
/// // Bind the context and resolve assets...
/// ```
///
/// Mirrors `pxr::ArDefaultResolverContext` from the USD C++ API.
public final class DefaultResolverContext: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: ArDefaultResolverContextRef

    // MARK: - Initialization

    /// Creates a context with no search paths.
    ///
    /// - Throws: `DefaultResolverContextError.creationFailed` if the context cannot be created.
    public init() throws {
        guard let ref = ArDefaultResolverContext_Create() else {
            throw DefaultResolverContextError.creationFailed("Failed to create default resolver context")
        }
        self.handle = ref
    }

    /// Creates a context with the given search paths.
    ///
    /// Elements in search paths should be absolute paths. If they are not,
    /// they will be anchored to the current working directory.
    ///
    /// - Parameter searchPaths: The search paths to use for resolution.
    /// - Throws: `DefaultResolverContextError.creationFailed` if the context cannot be created.
    public init(searchPaths: [String]) throws {
        let cStrings: [UnsafePointer<CChar>?] = searchPaths.map { $0.withCString { UnsafePointer(strdup($0)) } }
        defer {
            for cStr in cStrings {
                if let ptr = cStr {
                    free(UnsafeMutablePointer(mutating: ptr))
                }
            }
        }

        guard let ref = cStrings.withUnsafeBufferPointer({ buffer in
            ArDefaultResolverContext_CreateWithSearchPaths(buffer.baseAddress, searchPaths.count)
        }) else {
            throw DefaultResolverContextError.creationFailed("Failed to create default resolver context with search paths")
        }
        self.handle = ref
    }

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: ArDefaultResolverContextRef) {
        self.handle = handle
    }

    deinit {
        ArDefaultResolverContext_Release(handle)
    }

    // MARK: - Static Factory Methods

    /// An empty default resolver context (no search paths).
    public static var empty: DefaultResolverContext {
        get throws {
            try DefaultResolverContext()
        }
    }

    // MARK: - Properties

    /// The search paths used by this context.
    public var searchPaths: [String] {
        let count = ArDefaultResolverContext_GetSearchPathCount(handle)
        guard count > 0 else { return [] }

        var cStrings = [UnsafeMutablePointer<CChar>?](repeating: nil, count: count)
        let actualCount = cStrings.withUnsafeMutableBufferPointer { buffer in
            ArDefaultResolverContext_GetSearchPaths(handle, buffer.baseAddress, count)
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

    /// Returns a string representation for debugging.
    public var asString: String {
        guard let cStr = ArDefaultResolverContext_GetAsString(handle) else {
            return ""
        }
        defer { ArDefaultResolverContext_FreeString(cStr) }
        return String(cString: cStr)
    }

    // MARK: - Conversion

    /// Converts this default resolver context to a generic resolver context.
    ///
    /// - Returns: A `ResolverContext` containing this default resolver context.
    /// - Throws: `DefaultResolverContextError.creationFailed` if conversion fails.
    public func toResolverContext() throws -> ResolverContext {
        guard let ref = ArDefaultResolverContext_ToResolverContext(handle) else {
            throw DefaultResolverContextError.creationFailed("Failed to convert to resolver context")
        }
        return ResolverContext(handle: ref)
    }

    // MARK: - Copying

    /// Creates an independent copy of this context.
    ///
    /// - Returns: A new context with the same search paths.
    /// - Throws: `DefaultResolverContextError.creationFailed` if the copy cannot be created.
    public func copy() throws -> DefaultResolverContext {
        guard let ref = ArDefaultResolverContext_Copy(handle) else {
            throw DefaultResolverContextError.creationFailed("Failed to copy default resolver context")
        }
        return DefaultResolverContext(handle: ref)
    }
}

// MARK: - Equatable

extension DefaultResolverContext: Equatable {

    public static func == (lhs: DefaultResolverContext, rhs: DefaultResolverContext) -> Bool {
        ArDefaultResolverContext_Equal(lhs.handle, rhs.handle)
    }
}

// MARK: - Comparable

extension DefaultResolverContext: Comparable {

    public static func < (lhs: DefaultResolverContext, rhs: DefaultResolverContext) -> Bool {
        ArDefaultResolverContext_Compare(lhs.handle, rhs.handle) < 0
    }
}

// MARK: - Hashable

extension DefaultResolverContext: Hashable {

    public func hash(into hasher: inout Hasher) {
        hasher.combine(ArDefaultResolverContext_Hash(handle))
    }
}

// MARK: - CustomStringConvertible

extension DefaultResolverContext: CustomStringConvertible {

    public var description: String {
        asString.isEmpty ? "DefaultResolverContext(empty)" : asString
    }
}

// MARK: - CustomDebugStringConvertible

extension DefaultResolverContext: CustomDebugStringConvertible {

    public var debugDescription: String {
        "DefaultResolverContext(searchPaths: \(searchPaths))"
    }
}

// MARK: - DefaultResolverContextError

/// Errors that can occur when working with default resolver contexts.
public enum DefaultResolverContextError: Error, CustomStringConvertible {

    /// Context creation failed.
    case creationFailed(String)

    public var description: String {
        switch self {
        case .creationFailed(let message):
            return "Default resolver context creation failed: \(message)"
        }
    }
}
