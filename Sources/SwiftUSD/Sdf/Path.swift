// Path.swift - SdfPath wrapper
// Mirrors: pxr/usd/sdf/path.h

import OpenUSDInterop

/// A path to a prim, property, or other element in a USD scene.
/// Mirrors pxr::SdfPath from the USD C++ API.
public struct SdfPath: Hashable, Sendable {
    internal var handle: OpaquePointer?

    public init() {
        self.handle = nil
    }

    public init(_ path: String) {
        // TODO: Implement via C interop
        self.handle = nil
    }

    /// The absolute root path "/".
    public static let absoluteRootPath = SdfPath("/")

    /// The empty path.
    public static let emptyPath = SdfPath()

    /// Returns the path as a string.
    public var pathString: String {
        // TODO: Implement via C interop
        return ""
    }

    /// Returns true if this is an absolute path.
    public var isAbsolutePath: Bool {
        // TODO: Implement via C interop
        return false
    }

    /// Returns true if this path is empty.
    public var isEmpty: Bool {
        return handle == nil
    }

    /// Returns the parent path.
    public var parentPath: SdfPath {
        // TODO: Implement via C interop
        return SdfPath()
    }

    /// Appends a child path element.
    public func appendingChild(_ name: String) -> SdfPath {
        // TODO: Implement via C interop
        return SdfPath()
    }

    /// Appends a property path element.
    public func appendingProperty(_ name: TfToken) -> SdfPath {
        // TODO: Implement via C interop
        return SdfPath()
    }
}

extension SdfPath: ExpressibleByStringLiteral {
    public init(stringLiteral value: String) {
        self.init(value)
    }
}

extension SdfPath: CustomStringConvertible {
    public var description: String {
        return pathString
    }
}
