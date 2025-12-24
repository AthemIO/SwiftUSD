// AssetPath.swift - SdfAssetPath wrapper
// Mirrors: pxr/usd/sdf/assetPath.h

import OpenUSDInterop

// MARK: - SdfAssetPath

/// Contains an asset path and optional evaluated and resolved paths.
///
/// When used to author scene description, the authored path is serialized.
/// Asset paths may contain non-control UTF-8 encoded characters.
///
/// Mirrors `pxr::SdfAssetPath` from the USD C++ API.
public final class AssetPath: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: SdfAssetPathRef

    // MARK: - Initialization

    /// Creates an asset path from the given authored path string.
    ///
    /// - Parameter authoredPath: The authored asset path.
    /// - Throws: `AssetPathError.creationFailed` if the asset path cannot be created.
    public init(_ authoredPath: String) throws {
        guard let ref = SdfAssetPath_Create(authoredPath) else {
            throw AssetPathError.creationFailed("Failed to create asset path from '\(authoredPath)'")
        }
        self.handle = ref
    }

    /// Creates an asset path with both authored and resolved paths.
    ///
    /// - Parameters:
    ///   - authoredPath: The authored asset path.
    ///   - resolvedPath: The resolved asset path.
    /// - Throws: `AssetPathError.creationFailed` if the asset path cannot be created.
    public init(authoredPath: String, resolvedPath: String) throws {
        guard let ref = SdfAssetPath_CreateWithResolvedPath(authoredPath, resolvedPath) else {
            throw AssetPathError.creationFailed("Failed to create asset path")
        }
        self.handle = ref
    }

    /// Creates an empty asset path.
    ///
    /// - Throws: `AssetPathError.creationFailed` if the asset path cannot be created.
    public init() throws {
        guard let ref = SdfAssetPath_CreateEmpty() else {
            throw AssetPathError.creationFailed("Failed to create empty asset path")
        }
        self.handle = ref
    }

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: SdfAssetPathRef) {
        self.handle = handle
    }

    deinit {
        SdfAssetPath_Release(handle)
    }

    // MARK: - Static Factory Methods

    /// The empty asset path.
    public static var empty: AssetPath {
        get throws {
            try AssetPath()
        }
    }

    // MARK: - Properties

    /// The asset path as it was authored.
    ///
    /// This value is used for serialization.
    public var authoredPath: String {
        String(cString: SdfAssetPath_GetAuthoredPath(handle))
    }

    /// The asset path (evaluated path if present, otherwise authored).
    ///
    /// This is the path that is passed to asset resolution.
    public var assetPath: String {
        String(cString: SdfAssetPath_GetAssetPath(handle))
    }

    /// The resolved asset path.
    ///
    /// This is the result of asset resolution.
    /// May be empty if resolution has not been performed.
    public var resolvedPath: String {
        String(cString: SdfAssetPath_GetResolvedPath(handle))
    }

    /// Returns `true` if the authored path is empty.
    public var isEmpty: Bool {
        authoredPath.isEmpty
    }

    // MARK: - Copying

    /// Creates an independent copy of this asset path.
    ///
    /// - Returns: A new asset path with the same values.
    /// - Throws: `AssetPathError.creationFailed` if the copy cannot be created.
    public func copy() throws -> AssetPath {
        guard let ref = SdfAssetPath_Copy(handle) else {
            throw AssetPathError.creationFailed("Failed to copy asset path")
        }
        return AssetPath(handle: ref)
    }
}

// MARK: - Equatable

extension AssetPath: Equatable {

    public static func == (lhs: AssetPath, rhs: AssetPath) -> Bool {
        SdfAssetPath_Equal(lhs.handle, rhs.handle)
    }
}

// MARK: - Comparable

extension AssetPath: Comparable {

    public static func < (lhs: AssetPath, rhs: AssetPath) -> Bool {
        SdfAssetPath_Compare(lhs.handle, rhs.handle) < 0
    }
}

// MARK: - Hashable

extension AssetPath: Hashable {

    public func hash(into hasher: inout Hasher) {
        hasher.combine(SdfAssetPath_Hash(handle))
    }
}

// MARK: - CustomStringConvertible

extension AssetPath: CustomStringConvertible {

    public var description: String {
        assetPath
    }
}

// MARK: - CustomDebugStringConvertible

extension AssetPath: CustomDebugStringConvertible {

    public var debugDescription: String {
        if resolvedPath.isEmpty {
            return "AssetPath(\"\(authoredPath)\")"
        } else {
            return "AssetPath(\"\(authoredPath)\" -> \"\(resolvedPath)\")"
        }
    }
}

// MARK: - ExpressibleByStringLiteral

extension AssetPath: ExpressibleByStringLiteral {

    public convenience init(stringLiteral value: String) {
        if let ref = SdfAssetPath_Create(value) {
            self.init(handle: ref)
        } else if let emptyRef = SdfAssetPath_CreateEmpty() {
            self.init(handle: emptyRef)
        } else {
            fatalError("Failed to create asset path from string literal '\(value)'")
        }
    }
}

// MARK: - AssetPathError

/// Errors that can occur when working with asset paths.
public enum AssetPathError: Error, CustomStringConvertible {

    /// Asset path creation failed.
    case creationFailed(String)

    public var description: String {
        switch self {
        case .creationFailed(let message):
            return "Asset path creation failed: \(message)"
        }
    }
}

// MARK: - Codable

extension AssetPath: Codable {

    public convenience init(from decoder: Decoder) throws {
        let container = try decoder.singleValueContainer()
        let text = try container.decode(String.self)
        try self.init(text)
    }

    public func encode(to encoder: Encoder) throws {
        var container = encoder.singleValueContainer()
        try container.encode(authoredPath)
    }
}
