// ResolverContext.swift - ArResolverContext wrapper
// Mirrors: pxr/usd/ar/resolverContext.h

import OpenUSDInterop

// MARK: - ResolverContext

/// An asset resolver context allows clients to provide additional data
/// to the resolver for use during resolution.
///
/// Clients may provide context data via context objects of their own.
/// A `ResolverContext` is a wrapper around these objects that allows them
/// to be treated as a single type.
///
/// Mirrors `pxr::ArResolverContext` from the USD C++ API.
public final class ResolverContext: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: ArResolverContextRef

    // MARK: - Initialization

    /// Creates an empty resolver context.
    ///
    /// - Throws: `ResolverContextError.creationFailed` if the context cannot be created.
    public init() throws {
        guard let ref = ArResolverContext_CreateEmpty() else {
            throw ResolverContextError.creationFailed("Failed to create empty resolver context")
        }
        self.handle = ref
    }

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: ArResolverContextRef) {
        self.handle = handle
    }

    deinit {
        ArResolverContext_Release(handle)
    }

    // MARK: - Static Factory Methods

    /// An empty resolver context.
    public static var empty: ResolverContext {
        get throws {
            try ResolverContext()
        }
    }

    // MARK: - Properties

    /// Returns `true` if this context is empty.
    public var isEmpty: Bool {
        ArResolverContext_IsEmpty(handle)
    }

    /// Returns a debug string representation of this context.
    public var debugString: String {
        guard let cStr = ArResolverContext_GetDebugString(handle) else {
            return ""
        }
        defer { ArResolverContext_FreeString(cStr) }
        return String(cString: cStr)
    }

    // MARK: - Copying

    /// Creates an independent copy of this resolver context.
    ///
    /// - Returns: A new resolver context with the same value.
    /// - Throws: `ResolverContextError.creationFailed` if the copy cannot be created.
    public func copy() throws -> ResolverContext {
        guard let ref = ArResolverContext_Copy(handle) else {
            throw ResolverContextError.creationFailed("Failed to copy resolver context")
        }
        return ResolverContext(handle: ref)
    }
}

// MARK: - Equatable

extension ResolverContext: Equatable {

    public static func == (lhs: ResolverContext, rhs: ResolverContext) -> Bool {
        ArResolverContext_Equal(lhs.handle, rhs.handle)
    }
}

// MARK: - Comparable

extension ResolverContext: Comparable {

    public static func < (lhs: ResolverContext, rhs: ResolverContext) -> Bool {
        ArResolverContext_Compare(lhs.handle, rhs.handle) < 0
    }
}

// MARK: - Hashable

extension ResolverContext: Hashable {

    public func hash(into hasher: inout Hasher) {
        hasher.combine(ArResolverContext_Hash(handle))
    }
}

// MARK: - CustomStringConvertible

extension ResolverContext: CustomStringConvertible {

    public var description: String {
        debugString.isEmpty ? "ResolverContext(empty)" : debugString
    }
}

// MARK: - CustomDebugStringConvertible

extension ResolverContext: CustomDebugStringConvertible {

    public var debugDescription: String {
        "ResolverContext(\(debugString))"
    }
}

// MARK: - ResolverContextError

/// Errors that can occur when working with resolver contexts.
public enum ResolverContextError: Error, CustomStringConvertible {

    /// Resolver context creation failed.
    case creationFailed(String)

    public var description: String {
        switch self {
        case .creationFailed(let message):
            return "Resolver context creation failed: \(message)"
        }
    }
}
