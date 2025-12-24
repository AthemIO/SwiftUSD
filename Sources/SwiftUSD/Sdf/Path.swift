// Path.swift - SdfPath wrapper
// Mirrors: pxr/usd/sdf/path.h

import OpenUSDInterop

// MARK: - SdfPath

/// A path value used to locate objects in layers or scenegraphs.
///
/// `Path` is used in several ways:
/// - As a storage key for addressing and accessing values held in a layer
/// - As a namespace identity for scenegraph objects
/// - As a way to refer to other scenegraph objects through relative paths
///
/// Path components are separated by `/` for hierarchy and `.` for properties.
/// Examples:
/// - `/World` - absolute prim path
/// - `/World/Mesh` - nested prim path
/// - `/World/Mesh.points` - property path
/// - `Mesh` - relative prim path
/// - `.points` - relative property path
///
/// Mirrors `pxr::SdfPath` from the USD C++ API.
public final class Path: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: SdfPathRef

    // MARK: - Initialization

    /// Creates a path from the given path string.
    ///
    /// - Parameter pathString: The string representation of the path.
    /// - Throws: `PathError.creationFailed` if the path cannot be created.
    public init(_ pathString: String) throws {
        guard let ref = SdfPath_Create(pathString) else {
            throw PathError.creationFailed("Failed to create path from '\(pathString)'")
        }
        self.handle = ref
    }

    /// Creates an empty path.
    ///
    /// - Throws: `PathError.creationFailed` if the path cannot be created.
    public init() throws {
        guard let ref = SdfPath_CreateEmpty() else {
            throw PathError.creationFailed("Failed to create empty path")
        }
        self.handle = ref
    }

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: SdfPathRef) {
        self.handle = handle
    }

    deinit {
        SdfPath_Release(handle)
    }

    // MARK: - Static Factory Methods

    /// The empty path.
    public static var empty: Path {
        get throws {
            try Path()
        }
    }

    /// The absolute root path ("/").
    public static var absoluteRoot: Path {
        get throws {
            guard let ref = SdfPath_AbsoluteRootPath() else {
                throw PathError.creationFailed("Failed to create absolute root path")
            }
            return Path(handle: ref)
        }
    }

    /// The reflexive relative path (".").
    public static var reflexiveRelative: Path {
        get throws {
            guard let ref = SdfPath_ReflexiveRelativePath() else {
                throw PathError.creationFailed("Failed to create reflexive relative path")
            }
            return Path(handle: ref)
        }
    }

    // MARK: - Properties

    /// The string representation of the path.
    public var text: String {
        String(cString: SdfPath_GetText(handle))
    }

    /// The name of the prim, property, or relational attribute.
    public var name: String {
        String(cString: SdfPath_GetName(handle))
    }

    /// The name as a token.
    public var nameToken: Token? {
        guard let ref = SdfPath_GetNameToken(handle) else {
            return nil
        }
        return Token(handle: ref)
    }

    /// The number of path elements.
    public var elementCount: Int {
        Int(SdfPath_GetElementCount(handle))
    }

    /// Returns `true` if this is an empty path.
    public var isEmpty: Bool {
        SdfPath_IsEmpty(handle)
    }

    /// Returns `true` if this path is absolute (starts with "/").
    public var isAbsolutePath: Bool {
        SdfPath_IsAbsolutePath(handle)
    }

    /// Returns `true` if this is the absolute root path ("/").
    public var isAbsoluteRootPath: Bool {
        SdfPath_IsAbsoluteRootPath(handle)
    }

    /// Returns `true` if this path identifies a prim.
    public var isPrimPath: Bool {
        SdfPath_IsPrimPath(handle)
    }

    /// Returns `true` if this path identifies a prim or the absolute root.
    public var isAbsoluteRootOrPrimPath: Bool {
        SdfPath_IsAbsoluteRootOrPrimPath(handle)
    }

    /// Returns `true` if this path identifies a root prim (e.g., "/Foo").
    public var isRootPrimPath: Bool {
        SdfPath_IsRootPrimPath(handle)
    }

    /// Returns `true` if this path identifies a property.
    public var isPropertyPath: Bool {
        SdfPath_IsPropertyPath(handle)
    }

    /// Returns `true` if this path identifies a prim's property.
    public var isPrimPropertyPath: Bool {
        SdfPath_IsPrimPropertyPath(handle)
    }

    /// Returns `true` if this path contains a variant selection.
    public var containsVariantSelection: Bool {
        SdfPath_ContainsPrimVariantSelection(handle)
    }

    // MARK: - Navigation

    /// Returns the parent path.
    ///
    /// - Returns: The parent path, or `nil` if this path has no parent.
    public var parentPath: Path? {
        guard let ref = SdfPath_GetParentPath(handle) else {
            return nil
        }
        return Path(handle: ref)
    }

    /// Returns the prim path (strips property components).
    ///
    /// - Returns: The prim path.
    public var primPath: Path? {
        guard let ref = SdfPath_GetPrimPath(handle) else {
            return nil
        }
        return Path(handle: ref)
    }

    /// Returns the absolute root or prim path.
    ///
    /// - Returns: The absolute root or prim path.
    public var absoluteRootOrPrimPath: Path? {
        guard let ref = SdfPath_GetAbsoluteRootOrPrimPath(handle) else {
            return nil
        }
        return Path(handle: ref)
    }

    // MARK: - Construction

    /// Appends a child element to this path.
    ///
    /// - Parameter childName: The name of the child to append.
    /// - Returns: A new path with the child appended.
    /// - Throws: `PathError.creationFailed` if the operation fails.
    public func appendingChild(_ childName: Token) throws -> Path {
        guard let ref = SdfPath_AppendChild(handle, childName.handle) else {
            throw PathError.creationFailed("Failed to append child '\(childName.text)'")
        }
        return Path(handle: ref)
    }

    /// Appends a child element to this path.
    ///
    /// - Parameter childName: The name of the child to append.
    /// - Returns: A new path with the child appended.
    /// - Throws: `PathError.creationFailed` if the operation fails.
    public func appendingChild(_ childName: String) throws -> Path {
        guard let ref = SdfPath_AppendChildString(handle, childName) else {
            throw PathError.creationFailed("Failed to append child '\(childName)'")
        }
        return Path(handle: ref)
    }

    /// Appends a property element to this path.
    ///
    /// - Parameter propertyName: The name of the property to append.
    /// - Returns: A new path with the property appended.
    /// - Throws: `PathError.creationFailed` if the operation fails.
    public func appendingProperty(_ propertyName: Token) throws -> Path {
        guard let ref = SdfPath_AppendProperty(handle, propertyName.handle) else {
            throw PathError.creationFailed("Failed to append property '\(propertyName.text)'")
        }
        return Path(handle: ref)
    }

    /// Appends a property element to this path.
    ///
    /// - Parameter propertyName: The name of the property to append.
    /// - Returns: A new path with the property appended.
    /// - Throws: `PathError.creationFailed` if the operation fails.
    public func appendingProperty(_ propertyName: String) throws -> Path {
        guard let ref = SdfPath_AppendPropertyString(handle, propertyName) else {
            throw PathError.creationFailed("Failed to append property '\(propertyName)'")
        }
        return Path(handle: ref)
    }

    /// Appends a variant selection to this path.
    ///
    /// - Parameters:
    ///   - variantSet: The name of the variant set.
    ///   - variant: The name of the variant.
    /// - Returns: A new path with the variant selection appended.
    /// - Throws: `PathError.creationFailed` if the operation fails.
    public func appendingVariantSelection(variantSet: String, variant: String) throws -> Path {
        guard let ref = SdfPath_AppendVariantSelection(handle, variantSet, variant) else {
            throw PathError.creationFailed("Failed to append variant selection")
        }
        return Path(handle: ref)
    }

    /// Appends a relative path to this path.
    ///
    /// - Parameter suffix: The path to append.
    /// - Returns: A new path with the suffix appended.
    /// - Throws: `PathError.creationFailed` if the operation fails.
    public func appendingPath(_ suffix: Path) throws -> Path {
        guard let ref = SdfPath_AppendPath(handle, suffix.handle) else {
            throw PathError.creationFailed("Failed to append path")
        }
        return Path(handle: ref)
    }

    /// Replaces a prefix in this path with a new prefix.
    ///
    /// - Parameters:
    ///   - oldPrefix: The prefix to replace.
    ///   - newPrefix: The replacement prefix.
    /// - Returns: A new path with the prefix replaced.
    /// - Throws: `PathError.creationFailed` if the operation fails.
    public func replacingPrefix(_ oldPrefix: Path, with newPrefix: Path) throws -> Path {
        guard let ref = SdfPath_ReplacePrefix(handle, oldPrefix.handle, newPrefix.handle) else {
            throw PathError.creationFailed("Failed to replace prefix")
        }
        return Path(handle: ref)
    }

    /// Returns the common prefix of this path and another path.
    ///
    /// - Parameter other: The other path.
    /// - Returns: The common prefix.
    /// - Throws: `PathError.creationFailed` if the operation fails.
    public func commonPrefix(with other: Path) throws -> Path {
        guard let ref = SdfPath_GetCommonPrefix(handle, other.handle) else {
            throw PathError.creationFailed("Failed to get common prefix")
        }
        return Path(handle: ref)
    }

    /// Makes this path absolute using the given anchor.
    ///
    /// - Parameter anchor: The anchor path (must be absolute).
    /// - Returns: An absolute path.
    /// - Throws: `PathError.creationFailed` if the operation fails.
    public func makeAbsolute(anchor: Path) throws -> Path {
        guard let ref = SdfPath_MakeAbsolutePath(handle, anchor.handle) else {
            throw PathError.creationFailed("Failed to make absolute path")
        }
        return Path(handle: ref)
    }

    /// Makes this path relative to the given anchor.
    ///
    /// - Parameter anchor: The anchor path (must be absolute).
    /// - Returns: A relative path.
    /// - Throws: `PathError.creationFailed` if the operation fails.
    public func makeRelative(anchor: Path) throws -> Path {
        guard let ref = SdfPath_MakeRelativePath(handle, anchor.handle) else {
            throw PathError.creationFailed("Failed to make relative path")
        }
        return Path(handle: ref)
    }

    // MARK: - Query

    /// Returns `true` if this path has the given prefix.
    ///
    /// - Parameter prefix: The prefix to check.
    /// - Returns: `true` if this path has the prefix.
    public func hasPrefix(_ prefix: Path) -> Bool {
        SdfPath_HasPrefix(handle, prefix.handle)
    }

    /// Returns the prefix paths of this path (ancestors from root to this path).
    ///
    /// - Returns: An array of prefix paths from shortest to longest.
    public var prefixes: [Path] {
        let count = SdfPath_GetPrefixCount(handle)
        guard count > 0 else { return [] }

        var handles = [SdfPathRef?](repeating: nil, count: count)
        let actualCount = handles.withUnsafeMutableBufferPointer { buffer in
            SdfPath_GetPrefixes(handle, buffer.baseAddress, count)
        }

        var result = [Path]()
        result.reserveCapacity(actualCount)
        for i in 0..<actualCount {
            if let h = handles[i] {
                result.append(Path(handle: h))
            }
        }
        return result
    }

    // MARK: - Copying

    /// Creates an independent copy of this path.
    ///
    /// - Returns: A new path with the same value.
    /// - Throws: `PathError.creationFailed` if the copy cannot be created.
    public func copy() throws -> Path {
        guard let ref = SdfPath_Copy(handle) else {
            throw PathError.creationFailed("Failed to copy path")
        }
        return Path(handle: ref)
    }

    // MARK: - Validation

    /// Returns `true` if the string is a valid path string.
    ///
    /// - Parameter pathString: The string to validate.
    /// - Returns: `true` if the string is valid.
    public static func isValidPathString(_ pathString: String) -> Bool {
        SdfPath_IsValidPathString(pathString)
    }

    /// Returns `true` if the name is a valid identifier.
    ///
    /// - Parameter name: The name to validate.
    /// - Returns: `true` if the name is valid.
    public static func isValidIdentifier(_ name: String) -> Bool {
        SdfPath_IsValidIdentifier(name)
    }
}

// MARK: - Equatable

extension Path: Equatable {

    public static func == (lhs: Path, rhs: Path) -> Bool {
        SdfPath_Equal(lhs.handle, rhs.handle)
    }
}

// MARK: - Comparable

extension Path: Comparable {

    public static func < (lhs: Path, rhs: Path) -> Bool {
        SdfPath_Compare(lhs.handle, rhs.handle) < 0
    }
}

// MARK: - Hashable

extension Path: Hashable {

    public func hash(into hasher: inout Hasher) {
        hasher.combine(SdfPath_Hash(handle))
    }
}

// MARK: - CustomStringConvertible

extension Path: CustomStringConvertible {

    public var description: String {
        text
    }
}

// MARK: - CustomDebugStringConvertible

extension Path: CustomDebugStringConvertible {

    public var debugDescription: String {
        "Path(\"\(text)\")"
    }
}

// MARK: - ExpressibleByStringLiteral

extension Path: ExpressibleByStringLiteral {

    public convenience init(stringLiteral value: String) {
        if let ref = SdfPath_Create(value) {
            self.init(handle: ref)
        } else if let emptyRef = SdfPath_CreateEmpty() {
            self.init(handle: emptyRef)
        } else {
            fatalError("Failed to create path from string literal '\(value)'")
        }
    }
}

// MARK: - PathError

/// Errors that can occur when working with paths.
public enum PathError: Error, CustomStringConvertible {

    /// Path creation failed.
    case creationFailed(String)

    /// An invalid argument was provided.
    case invalidArgument(String)

    public var description: String {
        switch self {
        case .creationFailed(let message):
            return "Path creation failed: \(message)"
        case .invalidArgument(let message):
            return "Invalid argument: \(message)"
        }
    }
}

// MARK: - Codable

extension Path: Codable {

    public convenience init(from decoder: Decoder) throws {
        let container = try decoder.singleValueContainer()
        let text = try container.decode(String.self)
        try self.init(text)
    }

    public func encode(to encoder: Encoder) throws {
        var container = encoder.singleValueContainer()
        try container.encode(text)
    }
}
