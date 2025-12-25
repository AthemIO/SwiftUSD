// SdfCxx.swift - Swift extensions for Sdf C++ types
// Provides Swift-idiomatic API for swiftusd::Path, swiftusd::AssetPath, and swiftusd::LayerRef

import USDCxx

// MARK: - Type Aliases for C++ Interop Types

/// Type alias for the C++ Path type
public typealias CxxPath = swiftusd.Path

/// Type alias for the C++ AssetPath type
public typealias CxxAssetPath = swiftusd.AssetPath

/// Type alias for the C++ LayerRef type
public typealias CxxLayerRef = swiftusd.LayerRef

/// Type alias for the C++ Token type
public typealias CxxToken = swiftusd.Token

// MARK: - Path Extensions

extension swiftusd.Path: CustomStringConvertible {
    public var description: String {
        String(cString: self.GetText())
    }
}

extension swiftusd.Path: CustomDebugStringConvertible {
    public var debugDescription: String {
        "Path(\"\(self.description)\")"
    }
}

extension swiftusd.Path: Hashable {
    public func hash(into hasher: inout Hasher) {
        hasher.combine(self.GetHash())
    }
}

extension swiftusd.Path: ExpressibleByStringLiteral {
    public init(stringLiteral value: String) {
        self.init(value)
    }
}

extension swiftusd.Path {
    /// The string representation of the path
    public var text: String {
        String(cString: self.GetText())
    }

    /// The name of the last component
    public var name: String {
        String(cString: self.GetName())
    }

    /// Whether the path is empty
    public var isEmpty: Bool {
        self.IsEmpty()
    }

    /// Whether this is an absolute path (starts with '/')
    public var isAbsolute: Bool {
        self.IsAbsolutePath()
    }

    /// Whether this is a prim path (no property component)
    public var isPrim: Bool {
        self.IsPrimPath()
    }

    /// Whether this is a property path (has property component)
    public var isProperty: Bool {
        self.IsPropertyPath()
    }

    /// Creates a new path by appending a child with the given name
    public func appending(child name: String) -> swiftusd.Path {
        self.AppendChild(name)
    }

    /// Creates a new path by appending a property with the given name
    public func appending(property name: String) -> swiftusd.Path {
        self.AppendProperty(name)
    }

    /// Creates a new path by appending another path
    public func appending(path suffix: swiftusd.Path) -> swiftusd.Path {
        self.AppendPath(suffix)
    }

    /// The parent path
    public var parent: swiftusd.Path {
        self.GetParentPath()
    }

    /// The prim path (strips any property component)
    public var primPath: swiftusd.Path {
        self.GetPrimPath()
    }

    /// Whether this path has the given prefix
    public func hasPrefix(_ prefix: swiftusd.Path) -> Bool {
        self.HasPrefix(prefix)
    }
}

// MARK: - AssetPath Extensions

extension swiftusd.AssetPath: CustomStringConvertible {
    public var description: String {
        String(cString: self.GetAssetPath())
    }
}

extension swiftusd.AssetPath: CustomDebugStringConvertible {
    public var debugDescription: String {
        "AssetPath(\"\(self.description)\")"
    }
}

extension swiftusd.AssetPath: Hashable {
    public func hash(into hasher: inout Hasher) {
        hasher.combine(self.GetHash())
    }
}

extension swiftusd.AssetPath: ExpressibleByStringLiteral {
    public init(stringLiteral value: String) {
        self.init(value)
    }
}

extension swiftusd.AssetPath {
    /// The asset path string
    public var path: String {
        String(cString: self.GetAssetPath())
    }

    /// The resolved path string (may be empty if not resolved)
    public var resolvedPath: String {
        String(cString: self.GetResolvedPath())
    }

    /// Whether the asset path is empty
    public var isEmpty: Bool {
        self.IsEmpty()
    }
}

// MARK: - LayerRef Extensions

extension swiftusd.LayerRef {
    /// Creates a new layer with the given identifier
    public static func createNew(_ identifier: String) -> swiftusd.LayerRef? {
        swiftusd.LayerRef.CreateNew(identifier)
    }

    /// Creates a new anonymous layer with optional tag
    public static func createAnonymous(tag: String? = nil) -> swiftusd.LayerRef? {
        if let tag = tag {
            return swiftusd.LayerRef.CreateAnonymous(tag)
        }
        return swiftusd.LayerRef.CreateAnonymous(nil)
    }

    /// Finds or opens a layer with the given identifier
    public static func findOrOpen(_ identifier: String) -> swiftusd.LayerRef? {
        swiftusd.LayerRef.FindOrOpen(identifier)
    }

    /// Finds an existing layer with the given identifier
    public static func find(_ identifier: String) -> swiftusd.LayerRef? {
        swiftusd.LayerRef.Find(identifier)
    }

    /// The layer's identifier
    public var identifier: String {
        String(cString: self.GetIdentifier())
    }

    /// The layer's display name
    public var displayName: String {
        String(cString: self.GetDisplayName())
    }

    /// Whether the layer has unsaved changes
    public var isDirty: Bool {
        self.IsDirty()
    }

    /// Whether the layer is anonymous
    public var isAnonymous: Bool {
        self.IsAnonymous()
    }

    /// Whether the layer is empty
    public var isEmpty: Bool {
        self.IsEmpty()
    }

    /// Saves the layer to disk
    @discardableResult
    public func save(force: Bool = false) -> Bool {
        self.Save(force)
    }

    /// Exports the layer to the given path
    @discardableResult
    public func export(to path: String) -> Bool {
        self.Export(path)
    }

    /// Clears all content from the layer
    public func clear() {
        self.Clear()
    }

    /// Reloads the layer from disk
    @discardableResult
    public func reload(force: Bool = false) -> Bool {
        self.Reload(force)
    }

    /// The default prim token
    public var defaultPrim: swiftusd.Token {
        get { self.GetDefaultPrim() }
        set { self.SetDefaultPrim(newValue) }
    }

    /// The start time code
    public var startTimeCode: Double {
        get { self.GetStartTimeCode() }
        set { self.SetStartTimeCode(newValue) }
    }

    /// The end time code
    public var endTimeCode: Double {
        get { self.GetEndTimeCode() }
        set { self.SetEndTimeCode(newValue) }
    }

    /// Time codes per second
    public var timeCodesPerSecond: Double {
        get { self.GetTimeCodesPerSecond() }
        set { self.SetTimeCodesPerSecond(newValue) }
    }

    /// Frames per second
    public var framesPerSecond: Double {
        get { self.GetFramesPerSecond() }
        set { self.SetFramesPerSecond(newValue) }
    }

    /// Number of sublayers
    public var subLayerCount: Int {
        Int(self.GetNumSubLayers())
    }

    /// Inserts a sublayer path at the given index (-1 for append)
    public func insertSubLayer(_ path: String, at index: Int = -1) {
        self.InsertSubLayerPath(path, Int32(index))
    }

    /// Removes the sublayer at the given index
    public func removeSubLayer(at index: Int) {
        self.RemoveSubLayerPath(index)
    }

    /// Number of root prims
    public var rootPrimCount: Int {
        Int(self.GetRootPrimCount())
    }

    /// Whether a spec exists at the given path
    public func hasSpec(at path: swiftusd.Path) -> Bool {
        self.HasSpec(path)
    }
}

// MARK: - Token Extensions

extension swiftusd.Token: CustomStringConvertible {
    public var description: String {
        String(cString: self.GetText())
    }
}

extension swiftusd.Token: CustomDebugStringConvertible {
    public var debugDescription: String {
        "Token(\"\(self.description)\")"
    }
}

extension swiftusd.Token: Hashable {
    public func hash(into hasher: inout Hasher) {
        hasher.combine(self.Hash())
    }
}

extension swiftusd.Token: ExpressibleByStringLiteral {
    public init(stringLiteral value: String) {
        self.init(value)
    }
}

extension swiftusd.Token {
    /// The text of the token
    public var text: String {
        String(cString: self.GetText())
    }

    /// Whether the token is empty
    public var isEmpty: Bool {
        self.IsEmpty()
    }
}
