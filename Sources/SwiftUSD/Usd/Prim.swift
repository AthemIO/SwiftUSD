// Prim.swift - UsdPrim wrapper
// Mirrors: pxr/usd/usd/prim.h

import OpenUSDInterop

/// A prim is the primary container for scene data in USD.
/// Mirrors pxr::UsdPrim from the USD C++ API.
public struct UsdPrim: Sendable {
    internal var handle: OpaquePointer?

    internal init(handle: OpaquePointer?) {
        self.handle = handle
    }

    /// Returns true if this prim is valid.
    public var isValid: Bool {
        return handle != nil
    }

    /// Returns the path of this prim.
    public var path: SdfPath {
        // TODO: Implement via C interop
        return SdfPath()
    }

    /// Returns the name of this prim.
    public var name: TfToken {
        // TODO: Implement via C interop
        return TfToken()
    }

    /// Returns the type name of this prim.
    public var typeName: TfToken {
        // TODO: Implement via C interop
        return TfToken()
    }

    /// Returns true if this prim is active.
    public var isActive: Bool {
        get {
            // TODO: Implement via C interop
            return true
        }
        nonmutating set {
            // TODO: Implement via C interop
        }
    }

    // MARK: - Hierarchy

    /// Returns the parent prim.
    public var parent: UsdPrim? {
        // TODO: Implement via C interop
        return nil
    }

    /// Returns the children of this prim.
    public var children: [UsdPrim] {
        // TODO: Implement via C interop
        return []
    }

    // MARK: - Attributes

    /// Returns the attribute with the given name.
    public func attribute(_ name: TfToken) -> UsdAttribute? {
        // TODO: Implement via C interop
        return nil
    }

    /// Returns the attribute with the given name string.
    public func attribute(_ name: String) -> UsdAttribute? {
        return attribute(TfToken(name))
    }

    /// Creates a new attribute on this prim.
    public func createAttribute(_ name: TfToken, typeName: SdfValueTypeName) -> UsdAttribute? {
        // TODO: Implement via C interop
        return nil
    }

    /// Returns all attributes on this prim.
    public var attributes: [UsdAttribute] {
        // TODO: Implement via C interop
        return []
    }

    // MARK: - Schema

    /// Returns true if this prim has the given API schema applied.
    public func hasAPI<T: UsdAPISchemaBase>(_ type: T.Type) -> Bool {
        // TODO: Implement via C interop
        return false
    }
}

// MARK: - Supporting Types

/// Placeholder for SdfValueTypeName
public struct SdfValueTypeName: Hashable, Sendable {
    internal var handle: OpaquePointer?

    public init() {
        self.handle = nil
    }

    public static let bool = SdfValueTypeName()
    public static let int32 = SdfValueTypeName()
    public static let float = SdfValueTypeName()
    public static let double = SdfValueTypeName()
    public static let string = SdfValueTypeName()
    public static let token = SdfValueTypeName()
    public static let float3 = SdfValueTypeName()
    public static let double3 = SdfValueTypeName()
    public static let matrix4d = SdfValueTypeName()
}

/// Base protocol for API schemas.
public protocol UsdAPISchemaBase: Sendable {}
