// Property.swift - UsdProperty wrapper
// Mirrors: pxr/usd/usd/property.h

import OpenUSDInterop

// MARK: - Property

/// Base class for attributes and relationships.
///
/// A `Property` is either an `Attribute` or a `Relationship`.
/// It has a name, a path, and belongs to a prim.
///
/// Mirrors `pxr::UsdProperty` from the USD C++ API.
public final class Property: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdPropertyRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdPropertyRef) {
        self.handle = handle
    }

    deinit {
        UsdProperty_Release(handle)
    }

    // MARK: - Validity

    /// Returns `true` if the property is valid.
    public var isValid: Bool {
        UsdProperty_IsValid(handle)
    }

    /// Returns `true` if this property is an attribute.
    public var isAttribute: Bool {
        UsdProperty_IsAttribute(handle)
    }

    /// Returns `true` if this property is a relationship.
    public var isRelationship: Bool {
        UsdProperty_IsRelationship(handle)
    }

    // MARK: - Naming

    /// The property's name.
    public var name: Token {
        let ref = UsdProperty_GetName(handle)!
        return Token(handle: ref)
    }

    /// The property's base name (without namespace prefix).
    public var baseName: Token {
        let ref = UsdProperty_GetBaseName(handle)!
        return Token(handle: ref)
    }

    /// The property's namespace prefix.
    public var namespace: Token {
        let ref = UsdProperty_GetNamespace(handle)!
        return Token(handle: ref)
    }

    /// The property's path.
    public var path: Path {
        let ref = UsdProperty_GetPath(handle)!
        return Path(handle: ref)
    }

    // MARK: - Ownership

    /// The prim that owns this property.
    public var prim: Prim? {
        guard let ref = UsdProperty_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    // MARK: - Status

    /// Returns `true` if this is a custom (user-defined) property.
    public var isCustom: Bool {
        UsdProperty_IsCustom(handle)
    }

    /// Returns `true` if the property is defined (has a defining specifier).
    public var isDefined: Bool {
        UsdProperty_IsDefined(handle)
    }

    /// Returns `true` if the property has authored opinions.
    public var isAuthored: Bool {
        UsdProperty_IsAuthored(handle)
    }

    // MARK: - Conversion

    /// Attempts to convert this property to an attribute.
    ///
    /// - Returns: An `Attribute` if this property is an attribute, `nil` otherwise.
    public func asAttribute() -> Attribute? {
        guard isAttribute, let primRef = UsdProperty_GetPrim(handle) else {
            return nil
        }
        defer { UsdPrim_Release(primRef) }
        let nameRef = UsdProperty_GetName(handle)!
        defer { TfToken_Release(nameRef) }
        guard let attrRef = UsdPrim_GetAttribute(primRef, nameRef) else {
            return nil
        }
        return Attribute(handle: attrRef)
    }

    /// Attempts to convert this property to a relationship.
    ///
    /// - Returns: A `Relationship` if this property is a relationship, `nil` otherwise.
    public func asRelationship() -> Relationship? {
        guard isRelationship, let primRef = UsdProperty_GetPrim(handle) else {
            return nil
        }
        defer { UsdPrim_Release(primRef) }
        let nameRef = UsdProperty_GetName(handle)!
        defer { TfToken_Release(nameRef) }
        guard let relRef = UsdPrim_GetRelationship(primRef, nameRef) else {
            return nil
        }
        return Relationship(handle: relRef)
    }
}

// MARK: - CustomStringConvertible

extension Property: CustomStringConvertible {

    public var description: String {
        path.description
    }
}

// MARK: - CustomDebugStringConvertible

extension Property: CustomDebugStringConvertible {

    public var debugDescription: String {
        let type = isAttribute ? "Attribute" : (isRelationship ? "Relationship" : "Property")
        return "\(type)(\"\(path)\")"
    }
}
