// Sdf.swift - Scene Description Foundation - Idiomatic Swift API
//
// This module provides Swift wrappers around CxxFacade's Sdf types,
// including paths, layers, asset paths, and layer offsets.
//
// The Sdf module is the foundation for scene description in USD,
// providing the core types for identifying objects (paths) and
// managing layer composition.

import CxxFacade

// MARK: - Type Aliases

/// SdfPath - Scene description path for identifying prims and properties.
///
/// SdfPath is used to specify a prim, property, or other element in a
/// scene graph. Paths are hierarchical and use forward slashes as separators.
///
/// ## Usage
/// ```swift
/// // Create paths
/// let rootPath = SdfPath("/")
/// let worldPath = SdfPath("/World")
/// let meshPath = SdfPath("/World/Mesh")
///
/// // Build paths hierarchically
/// let childPath = worldPath.AppendChild(TfToken("Child"))
///
/// // Property paths
/// let propertyPath = meshPath.AppendProperty(TfToken("visibility"))
/// ```
public typealias SdfPath = cxxfacade.sdf.SdfPath

/// SdfAssetPath - Asset path for referencing external files.
///
/// SdfAssetPath represents a path to an external asset (like a texture or
/// reference file). It can contain both the authored path and the resolved
/// path after asset resolution.
///
/// ## Usage
/// ```swift
/// // Create an asset path
/// let texturePath = SdfAssetPath("textures/diffuse.png")
///
/// // With resolved path
/// let resolvedPath = SdfAssetPath(
///     "textures/diffuse.png",
///     "/absolute/path/to/textures/diffuse.png"
/// )
/// ```
public typealias SdfAssetPath = cxxfacade.sdf.SdfAssetPath

/// SdfLayer - Scene description layer (USD file).
///
/// SdfLayer represents a single layer in the USD composition stack.
/// Each layer is typically backed by a file (usda, usdc, usd) and
/// contains scene description that can be composed with other layers.
///
/// ## Usage
/// ```swift
/// // Create an anonymous layer
/// let layer = SdfLayer.CreateAnonymous()
///
/// // Open an existing layer
/// let existingLayer = SdfLayer.FindOrOpen("scene.usda")
///
/// // Create a new layer at a path
/// let newLayer = SdfLayer.CreateNew("output.usda")
/// ```
public typealias SdfLayer = cxxfacade.sdf.SdfLayer

/// SdfLayerOffset - Time offset and scale between layers.
///
/// SdfLayerOffset is an affine transform (scale and translate) for time values.
/// It's used to remap animation timing when composing layers.
///
/// ## Usage
/// ```swift
/// // Create an identity offset (no change)
/// let identity = SdfLayerOffset()
///
/// // Create an offset that doubles time and shifts by 24 frames
/// var offset = SdfLayerOffset(24.0, 2.0)
///
/// // Apply offset to a time value
/// let remappedTime = offset * 10.0  // = 24 + 2*10 = 44
///
/// // Get the inverse transformation
/// let inverse = offset.GetInverse()
/// ```
public typealias SdfLayerOffset = cxxfacade.sdf.SdfLayerOffset

// MARK: - SdfPath Extensions

extension SdfPath: CustomStringConvertible {
    /// A textual representation of the path.
    public var description: String {
        return String(self.GetString())
    }
}

extension SdfPath: CustomDebugStringConvertible {
    /// A debug textual representation of the path.
    public var debugDescription: String {
        if self.IsEmpty() {
            return "SdfPath(empty)"
        }
        return "SdfPath(\"\(String(self.GetString()))\")"
    }
}

extension SdfPath: Hashable {
    /// Hashes the essential components of this value.
    public func hash(into hasher: inout Hasher) {
        hasher.combine(String(self.GetString()))
    }
}

extension SdfPath: ExpressibleByStringLiteral {
    /// Creates a path from a string literal.
    ///
    /// This allows you to write:
    /// ```swift
    /// let path: SdfPath = "/World/Mesh"
    /// ```
    public init(stringLiteral value: String) {
        self.init(value)
    }
}

extension SdfPath {
    /// Creates a path from a Swift String.
    public init(_ string: String) {
        self.init(std.string(string))
    }

    /// The path as a Swift String.
    public var pathString: String {
        return String(self.GetString())
    }

    /// Whether this path is empty.
    public var isEmpty: Bool {
        return self.IsEmpty()
    }

    /// Whether this is an absolute path (starts with /).
    public var isAbsolute: Bool {
        return self.IsAbsolutePath()
    }

    /// Whether this is a prim path (not a property path).
    public var isPrimPath: Bool {
        return self.IsPrimPath()
    }

    /// Whether this is a property path.
    public var isPropertyPath: Bool {
        return self.IsPropertyPath()
    }

    /// The parent path, or empty path if this is the root.
    public var parent: SdfPath {
        return self.GetParentPath()
    }

    /// The name (last element) of the path.
    public var name: TfToken {
        return self.GetName()
    }

    /// Append a child element to this path.
    public func appending(child: TfToken) -> SdfPath {
        return self.AppendChild(child)
    }

    /// Append a child element to this path using a string.
    public func appending(child: String) -> SdfPath {
        return self.AppendChild(TfToken(child))
    }

    /// Append a property to this path.
    public func appending(property: TfToken) -> SdfPath {
        return self.AppendProperty(property)
    }

    /// Append a property to this path using a string.
    public func appending(property: String) -> SdfPath {
        return self.AppendProperty(TfToken(property))
    }

    /// The absolute root path "/".
    public static var absoluteRoot: SdfPath {
        return SdfPath.AbsoluteRootPath()
    }

    /// An empty path.
    public static var empty: SdfPath {
        return SdfPath.EmptyPath()
    }
}

// MARK: - SdfAssetPath Extensions

extension SdfAssetPath: CustomStringConvertible {
    /// A textual representation of the asset path.
    public var description: String {
        return String(self.GetAssetPath())
    }
}

extension SdfAssetPath: CustomDebugStringConvertible {
    /// A debug textual representation of the asset path.
    public var debugDescription: String {
        let authored = String(self.GetAssetPath())
        let resolved = String(self.GetResolvedPath())
        if resolved.isEmpty {
            return "SdfAssetPath(\"\(authored)\")"
        }
        return "SdfAssetPath(\"\(authored)\" -> \"\(resolved)\")"
    }
}

extension SdfAssetPath: Hashable {
    /// Hashes the essential components of this value.
    public func hash(into hasher: inout Hasher) {
        hasher.combine(String(self.GetAssetPath()))
        hasher.combine(String(self.GetResolvedPath()))
    }
}

extension SdfAssetPath: ExpressibleByStringLiteral {
    /// Creates an asset path from a string literal.
    public init(stringLiteral value: String) {
        self.init(value)
    }
}

extension SdfAssetPath {
    /// Creates an asset path from a Swift String.
    public init(_ path: String) {
        self.init(std.string(path))
    }

    /// Creates an asset path with both authored and resolved paths.
    public init(_ path: String, resolved: String) {
        self.init(std.string(path), std.string(resolved))
    }

    /// The authored asset path as a Swift String.
    public var assetPath: String {
        return String(self.GetAssetPath())
    }

    /// The resolved asset path as a Swift String.
    public var resolvedPath: String {
        return String(self.GetResolvedPath())
    }
}

// MARK: - SdfLayer Extensions

extension SdfLayer: CustomStringConvertible {
    /// A textual representation of the layer.
    public var description: String {
        if !self.IsValid() {
            return "SdfLayer(invalid)"
        }
        return "SdfLayer(\(String(self.GetIdentifier())))"
    }
}

extension SdfLayer: CustomDebugStringConvertible {
    /// A debug textual representation of the layer.
    public var debugDescription: String {
        if !self.IsValid() {
            return "SdfLayer(invalid)"
        }
        let id = String(self.GetIdentifier())
        let display = String(self.GetDisplayName())
        let anon = self.IsAnonymous() ? ", anonymous" : ""
        let dirty = self.IsDirty() ? ", dirty" : ""
        return "SdfLayer(\(id), displayName: \(display)\(anon)\(dirty))"
    }
}

extension SdfLayer {
    /// Whether this is a valid layer reference.
    public var isValid: Bool {
        return self.IsValid()
    }

    /// The layer identifier (file path or anonymous ID).
    public var identifier: String {
        return String(self.GetIdentifier())
    }

    /// The layer's display name.
    public var displayName: String {
        return String(self.GetDisplayName())
    }

    /// Whether this is an anonymous layer.
    public var isAnonymous: Bool {
        return self.IsAnonymous()
    }

    /// Whether the layer has been modified.
    public var isDirty: Bool {
        return self.IsDirty()
    }

    /// Export the layer to a file.
    public func export(to filename: String) -> Bool {
        return self.Export(std.string(filename))
    }

    /// Create a new anonymous layer.
    public static func createAnonymous() -> SdfLayer {
        return SdfLayer.CreateAnonymous()
    }

    /// Create a new layer at the given path.
    public static func create(at path: String) -> SdfLayer {
        return SdfLayer.CreateNew(std.string(path))
    }

    /// Find or open an existing layer.
    public static func findOrOpen(_ path: String) -> SdfLayer {
        return SdfLayer.FindOrOpen(std.string(path))
    }
}

// MARK: - SdfLayerOffset Extensions

extension SdfLayerOffset: CustomStringConvertible {
    /// A textual representation of the layer offset.
    public var description: String {
        if self.IsIdentity() {
            return "SdfLayerOffset(identity)"
        }
        return "SdfLayerOffset(offset: \(self.GetOffset()), scale: \(self.GetScale()))"
    }
}

extension SdfLayerOffset: CustomDebugStringConvertible {
    /// A debug textual representation of the layer offset.
    public var debugDescription: String {
        return "SdfLayerOffset(offset: \(self.GetOffset()), scale: \(self.GetScale()), valid: \(self.IsValid()))"
    }
}

extension SdfLayerOffset: Hashable {
    /// Hashes the essential components of this value.
    public func hash(into hasher: inout Hasher) {
        hasher.combine(self.GetHash())
    }
}

extension SdfLayerOffset {
    /// Creates an identity layer offset (no transformation).
    public init() {
        self.init(0.0, 1.0)
    }

    /// Creates a layer offset with the given offset and scale.
    public init(offset: Double, scale: Double = 1.0) {
        self.init(offset, scale)
    }

    /// The time offset value.
    public var offset: Double {
        get { return self.GetOffset() }
        set { self.SetOffset(newValue) }
    }

    /// The time scale factor.
    public var scale: Double {
        get { return self.GetScale() }
        set { self.SetScale(newValue) }
    }

    /// Whether this is an identity transformation (offset=0, scale=1).
    public var isIdentity: Bool {
        return self.IsIdentity()
    }

    /// Whether this offset is valid (finite values).
    public var isValid: Bool {
        return self.IsValid()
    }

    /// The inverse of this offset.
    public var inverse: SdfLayerOffset {
        return self.GetInverse()
    }

    /// An identity layer offset (no transformation).
    public static var identity: SdfLayerOffset {
        return SdfLayerOffset()
    }

    /// Apply this offset to a time value.
    public func apply(to time: Double) -> Double {
        return self * time
    }
}

// MARK: - Common Path Constants

extension SdfPath {
    /// Common path values used throughout USD.
    public enum Common {
        /// The absolute root path "/".
        nonisolated(unsafe) public static let root = SdfPath.absoluteRoot

        /// An empty path.
        nonisolated(unsafe) public static let empty = SdfPath.empty
    }
}
