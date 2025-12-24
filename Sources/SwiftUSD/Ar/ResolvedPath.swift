// ResolvedPath.swift - ArResolvedPath wrapper
// Mirrors: pxr/usd/ar/resolvedPath.h

import OpenUSDInterop

// MARK: - ResolvedPath

/// Represents a resolved asset path.
///
/// A resolved path is the result of asset resolution - it represents
/// the actual location where an asset's data can be accessed.
///
/// Mirrors `pxr::ArResolvedPath` from the USD C++ API.
public final class ResolvedPath: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: ArResolvedPathRef

    // MARK: - Initialization

    /// Creates a resolved path from the given path string.
    ///
    /// - Parameter pathString: The resolved path string.
    /// - Throws: `ResolvedPathError.creationFailed` if the path cannot be created.
    public init(_ pathString: String) throws {
        guard let ref = ArResolvedPath_Create(pathString) else {
            throw ResolvedPathError.creationFailed("Failed to create resolved path from '\(pathString)'")
        }
        self.handle = ref
    }

    /// Creates an empty resolved path.
    ///
    /// - Throws: `ResolvedPathError.creationFailed` if the path cannot be created.
    public init() throws {
        guard let ref = ArResolvedPath_CreateEmpty() else {
            throw ResolvedPathError.creationFailed("Failed to create empty resolved path")
        }
        self.handle = ref
    }

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: ArResolvedPathRef) {
        self.handle = handle
    }

    deinit {
        ArResolvedPath_Release(handle)
    }

    // MARK: - Static Factory Methods

    /// An empty resolved path.
    public static var empty: ResolvedPath {
        get throws {
            try ResolvedPath()
        }
    }

    // MARK: - Properties

    /// The resolved path string.
    public var pathString: String {
        String(cString: ArResolvedPath_GetPathString(handle))
    }

    /// Returns `true` if this is an empty resolved path.
    public var isEmpty: Bool {
        ArResolvedPath_IsEmpty(handle)
    }

    /// Returns `true` if this resolved path is valid (non-empty).
    public var isValid: Bool {
        !isEmpty
    }

    // MARK: - Copying

    /// Creates an independent copy of this resolved path.
    ///
    /// - Returns: A new resolved path with the same value.
    /// - Throws: `ResolvedPathError.creationFailed` if the copy cannot be created.
    public func copy() throws -> ResolvedPath {
        guard let ref = ArResolvedPath_Copy(handle) else {
            throw ResolvedPathError.creationFailed("Failed to copy resolved path")
        }
        return ResolvedPath(handle: ref)
    }
}

// MARK: - Equatable

extension ResolvedPath: Equatable {

    public static func == (lhs: ResolvedPath, rhs: ResolvedPath) -> Bool {
        ArResolvedPath_Equal(lhs.handle, rhs.handle)
    }
}

// MARK: - Comparable

extension ResolvedPath: Comparable {

    public static func < (lhs: ResolvedPath, rhs: ResolvedPath) -> Bool {
        ArResolvedPath_Compare(lhs.handle, rhs.handle) < 0
    }
}

// MARK: - Hashable

extension ResolvedPath: Hashable {

    public func hash(into hasher: inout Hasher) {
        hasher.combine(ArResolvedPath_Hash(handle))
    }
}

// MARK: - CustomStringConvertible

extension ResolvedPath: CustomStringConvertible {

    public var description: String {
        pathString
    }
}

// MARK: - CustomDebugStringConvertible

extension ResolvedPath: CustomDebugStringConvertible {

    public var debugDescription: String {
        "ResolvedPath(\"\(pathString)\")"
    }
}

// MARK: - ExpressibleByStringLiteral

extension ResolvedPath: ExpressibleByStringLiteral {

    public convenience init(stringLiteral value: String) {
        if let ref = ArResolvedPath_Create(value) {
            self.init(handle: ref)
        } else if let emptyRef = ArResolvedPath_CreateEmpty() {
            self.init(handle: emptyRef)
        } else {
            fatalError("Failed to create resolved path from string literal '\(value)'")
        }
    }
}

// MARK: - ResolvedPathError

/// Errors that can occur when working with resolved paths.
public enum ResolvedPathError: Error, CustomStringConvertible {

    /// Resolved path creation failed.
    case creationFailed(String)

    public var description: String {
        switch self {
        case .creationFailed(let message):
            return "Resolved path creation failed: \(message)"
        }
    }
}

// MARK: - Codable

extension ResolvedPath: Codable {

    public convenience init(from decoder: Decoder) throws {
        let container = try decoder.singleValueContainer()
        let path = try container.decode(String.self)
        try self.init(path)
    }

    public func encode(to encoder: Encoder) throws {
        var container = encoder.singleValueContainer()
        try container.encode(pathString)
    }
}
