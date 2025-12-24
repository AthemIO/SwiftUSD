// Usd.swift - Usd module namespace and exports
// Mirrors: pxr/usd/usd/

import OpenUSDInterop

// MARK: - Usd Namespace

/// Core USD API for working with stages, prims, attributes, and relationships.
/// Mirrors pxr::Usd from the USD C++ API.
///
/// The Usd module provides:
/// - `Stage`: The outermost container for scene description
/// - `Prim`: The fundamental addressable unit in a scene graph
/// - `Attribute`: Typed data values on prims
/// - `Relationship`: Dependencies between prims
/// - `Property`: Base class for attributes and relationships
/// - `TimeCode`: Time values for attribute queries
///
/// ## Usage
/// ```swift
/// // Create an in-memory stage
/// let stage = try Stage.createInMemory()
///
/// // Define a prim
/// let prim = try stage.definePrim(at: "/World", typeName: "Xform")
///
/// // Create an attribute
/// let attr = prim.createAttribute(named: "myFloat", typeName: "float")
/// attr?.set(3.14 as Float)
/// ```
public enum Usd {
    // This namespace enum groups Usd-related utilities.
    // The primary types are defined in their own files:
    // - Stage.swift (UsdStage)
    // - Prim.swift (UsdPrim)
    // - Attribute.swift (UsdAttribute)
    // - Relationship.swift (UsdRelationship)
    // - Property.swift (UsdProperty)
    // - TimeCode.swift (UsdTimeCode)
    // - Specifier.swift (Specifier, Variability, ListPosition)
}

// MARK: - Type Aliases

/// Type alias for UsdStage
public typealias UsdStage = Stage

/// Type alias for UsdPrim
public typealias UsdPrim = Prim

/// Type alias for UsdAttribute
public typealias UsdAttribute = Attribute

/// Type alias for UsdRelationship
public typealias UsdRelationship = Relationship

/// Type alias for UsdProperty
public typealias UsdProperty = Property
