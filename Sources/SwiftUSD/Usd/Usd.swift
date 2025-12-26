// Usd.swift - Core USD API - Idiomatic Swift API
//
// This module provides Swift wrappers around CxxFacade's Usd types,
// including Stage, Prim, Property, Attribute, Relationship, and
// schema base classes for defining typed USD objects.
//
// The Usd module is the core API for working with USD scenes,
// providing the primary interface for creating, editing, and
// querying scene graphs.

import CxxFacade

// MARK: - Type Aliases

/// UsdTimeCode - Time value for animation queries.
///
/// UsdTimeCode represents a specific time sample for attribute queries.
/// Use `UsdTimeCode.Default()` for non-animated values.
///
/// ## Usage
/// ```swift
/// // Default (non-animated) time
/// let defaultTime = UsdTimeCode.Default()
///
/// // Specific frame number
/// let frame24 = UsdTimeCode(24.0)
///
/// // Earliest available sample
/// let earliest = UsdTimeCode.EarliestTime()
/// ```
public typealias UsdTimeCode = cxxfacade.usd.UsdTimeCode

/// UsdStage - USD scene container.
///
/// UsdStage is the primary container for a composed USD scene. It manages
/// the composition of layers and provides the main interface for accessing
/// and editing prims.
///
/// ## Usage
/// ```swift
/// // Create an in-memory stage
/// var stage = UsdStage.CreateInMemory()
///
/// // Open an existing stage
/// var existing = UsdStage.Open("/path/to/scene.usda")
///
/// // Create a new stage at a path
/// var newStage = UsdStage.CreateNew("/path/to/new.usda")
///
/// // Define prims
/// let worldPrim = stage.DefinePrim(SdfPath("/World"), TfToken("Xform"))
/// ```
public typealias UsdStage = cxxfacade.usd.UsdStage

/// UsdPrim - Scene element (node in the scene graph).
///
/// UsdPrim represents a prim in a USD stage. Prims are the primary
/// elements in a scene graph, containing properties (attributes and
/// relationships) and child prims.
///
/// ## Usage
/// ```swift
/// let prim = stage.GetPrimAtPath(SdfPath("/World/Mesh"))
/// let typeName = prim.GetTypeName()
/// let children = prim.GetChildren()
/// ```
public typealias UsdPrim = cxxfacade.usd.UsdPrim

/// UsdProperty - Base class for UsdAttribute and UsdRelationship.
///
/// UsdProperty is the base class for all properties on a prim. Properties
/// can be either attributes (typed values) or relationships (connections).
public typealias UsdProperty = cxxfacade.usd.UsdProperty

/// UsdAttribute - Scene attribute (typed value on a prim).
///
/// UsdAttribute represents a typed, potentially time-varying value
/// on a prim. Attributes have a specific value type and can have
/// values at different time samples.
///
/// ## Usage
/// ```swift
/// // Get an attribute
/// let attr = prim.GetAttribute(TfToken("visibility"))
///
/// // Get the value at default time
/// var value = VtValue()
/// attr.Get(&value)
///
/// // Set a value
/// attr.Set(VtValue(true))
/// ```
public typealias UsdAttribute = cxxfacade.usd.UsdAttribute

/// UsdRelationship - Scene relationship (connects prims).
///
/// UsdRelationship represents a connection between prims. Unlike attributes,
/// relationships don't have typed values but instead point to other prims
/// or properties in the scene.
///
/// ## Usage
/// ```swift
/// // Get a relationship
/// let rel = prim.GetRelationship(TfToken("material:binding"))
///
/// // Get targets
/// var targets = std.vector<SdfPath>()
/// rel.GetTargets(&targets)
///
/// // Add a target
/// rel.AddTarget(SdfPath("/Materials/Default"))
/// ```
public typealias UsdRelationship = cxxfacade.usd.UsdRelationship

/// UsdEditTarget - Defines where edits should be directed.
///
/// UsdEditTarget specifies which layer in a stage's composition should
/// receive edits. By default, edits go to the root layer.
///
/// ## Usage
/// ```swift
/// // Get current edit target
/// let target = stage.GetEditTarget()
///
/// // Set edit target to session layer
/// let sessionLayer = stage.GetSessionLayer()
/// stage.SetEditTarget(UsdEditTarget(sessionLayer))
/// ```
public typealias UsdEditTarget = cxxfacade.usd.UsdEditTarget

// Note: UsdEditContext is not exposed to Swift as it's a non-copyable RAII type.
// Use UsdStage.SetEditTarget() and manage the edit target manually in Swift.

/// UsdSchemaKind - Kind of schema.
///
/// Enum representing the kind of schema a class is. This determines
/// whether a schema is typed, an API schema, or something else.
public typealias UsdSchemaKind = cxxfacade.usd.UsdSchemaKind

/// UsdSchemaBase - Base class for all schema types.
///
/// UsdSchemaBase is the base class for all USD schema types. Schema
/// objects wrap a prim and provide a typed API for accessing specific
/// properties and behaviors.
public typealias UsdSchemaBase = cxxfacade.usd.UsdSchemaBase

/// UsdTyped - Base class for typed schemas.
///
/// UsdTyped is the base class for all typed schemas (those that can
/// impart a typeName to a prim). Concrete schemas like UsdGeomMesh
/// inherit from UsdTyped.
public typealias UsdTyped = cxxfacade.usd.UsdTyped

/// UsdAPISchemaBase - Base class for API schemas.
///
/// UsdAPISchemaBase is the base class for all API schemas. API schemas
/// provide an interface to a prim's qualities but don't specify a typeName.
public typealias UsdAPISchemaBase = cxxfacade.usd.UsdAPISchemaBase

// MARK: - UsdTimeCode Extensions

extension UsdTimeCode: CustomStringConvertible {
    /// A textual representation of the time code.
    public var description: String {
        if self.IsDefault() {
            return "UsdTimeCode.Default"
        }
        return "UsdTimeCode(\(self.GetValue()))"
    }
}

extension UsdTimeCode: Hashable {
    /// Hashes the essential components of this value.
    public func hash(into hasher: inout Hasher) {
        hasher.combine(self.GetValue())
        hasher.combine(self.IsDefault())
    }
}

extension UsdTimeCode {
    /// Whether this is the default (non-animated) time.
    public var isDefault: Bool {
        return self.IsDefault()
    }

    /// Whether this is a numeric time value.
    public var isNumeric: Bool {
        return self.IsNumeric()
    }

    /// The numeric value of this time code.
    public var value: Double {
        return self.GetValue()
    }

    /// The default (non-animated) time code.
    public static var `default`: UsdTimeCode {
        return UsdTimeCode.Default()
    }

    /// The earliest available time code.
    public static var earliest: UsdTimeCode {
        return UsdTimeCode.EarliestTime()
    }
}

// MARK: - UsdStage Extensions

extension UsdStage: CustomStringConvertible {
    /// A textual representation of the stage.
    public var description: String {
        if !self.IsValid() {
            return "UsdStage(invalid)"
        }
        return "UsdStage(\(rootLayer.identifier))"
    }
}

extension UsdStage {
    /// Whether this is a valid stage reference.
    public var isValid: Bool {
        return self.IsValid()
    }

    /// The root layer of this stage.
    public var rootLayer: SdfLayer {
        return self.GetRootLayer()
    }

    /// The session layer of this stage.
    public var sessionLayer: SdfLayer {
        return self.GetSessionLayer()
    }

    /// The pseudo-root prim.
    public var pseudoRoot: UsdPrim {
        return self.GetPseudoRoot()
    }

    /// The default prim (if set).
    public var defaultPrim: UsdPrim {
        return self.GetDefaultPrim()
    }

    /// The up-axis for this stage ("Y" or "Z").
    public var upAxis: TfToken {
        return self.GetUpAxis()
    }

    /// The meters per unit for this stage.
    public var metersPerUnit: Double {
        get { return self.GetMetersPerUnit() }
        set { _ = self.SetMetersPerUnit(newValue) }
    }

    /// The start time code for animation.
    public var startTimeCode: Double {
        get { return self.GetStartTimeCode() }
        set { self.SetStartTimeCode(newValue) }
    }

    /// The end time code for animation.
    public var endTimeCode: Double {
        get { return self.GetEndTimeCode() }
        set { self.SetEndTimeCode(newValue) }
    }

    /// The current edit target.
    public var editTarget: UsdEditTarget {
        get { return self.GetEditTarget() }
        set { self.SetEditTarget(newValue) }
    }

    /// Create an in-memory stage.
    public static func createInMemory() -> UsdStage {
        return UsdStage.CreateInMemory()
    }

    /// Create a new stage at the given path.
    public static func create(at path: String) -> UsdStage {
        return UsdStage.CreateNew(std.string(path))
    }

    /// Open an existing stage.
    public static func open(_ path: String) -> UsdStage {
        return UsdStage.Open(std.string(path))
    }

    /// Get a prim at the given path.
    public func prim(at path: SdfPath) -> UsdPrim {
        return self.GetPrimAtPath(path)
    }

    /// Get a prim at the given path string.
    public func prim(at path: String) -> UsdPrim {
        return self.GetPrimAtPath(SdfPath(path))
    }

    /// Define a prim at the given path with an optional type.
    public mutating func definePrim(at path: SdfPath, type: TfToken = TfToken()) -> UsdPrim {
        return self.DefinePrim(path, type)
    }

    /// Define a prim at the given path string with an optional type name.
    public mutating func definePrim(at path: String, type: String = "") -> UsdPrim {
        return self.DefinePrim(SdfPath(path), TfToken(type))
    }

    /// Override a prim at the given path.
    public mutating func overridePrim(at path: SdfPath) -> UsdPrim {
        return self.OverridePrim(path)
    }

    /// Override a prim at the given path string.
    public mutating func overridePrim(at path: String) -> UsdPrim {
        return self.OverridePrim(SdfPath(path))
    }

    /// Remove a prim at the given path.
    @discardableResult
    public mutating func removePrim(at path: SdfPath) -> Bool {
        return self.RemovePrim(path)
    }

    /// Remove a prim at the given path string.
    @discardableResult
    public mutating func removePrim(at path: String) -> Bool {
        return self.RemovePrim(SdfPath(path))
    }

    /// Export to a file.
    @discardableResult
    public func export(to filename: String) -> Bool {
        return self.Export(std.string(filename))
    }
}

// MARK: - UsdPrim Extensions

extension UsdPrim: CustomStringConvertible {
    /// A textual representation of the prim.
    public var description: String {
        if !self.IsValid() {
            return "UsdPrim(invalid)"
        }
        return "UsdPrim(\(pathString))"
    }
}

extension UsdPrim: CustomDebugStringConvertible {
    /// A debug textual representation of the prim.
    public var debugDescription: String {
        if !self.IsValid() {
            return "UsdPrim(invalid)"
        }
        let typeName = String(cString: self.GetTypeName().GetText())
        let typeStr = typeName.isEmpty ? "" : ", type: \(typeName)"
        return "UsdPrim(\(pathString)\(typeStr))"
    }
}

extension UsdPrim {
    /// Whether this is a valid prim reference.
    public var isValid: Bool {
        return self.IsValid()
    }

    /// Whether this prim is active.
    public var isActive: Bool {
        return self.IsActive()
    }

    /// Whether this prim is defined.
    public var isDefined: Bool {
        return self.IsDefined()
    }

    /// The prim's name.
    public var name: TfToken {
        return self.GetName()
    }

    /// The prim's path as a string.
    public var pathString: String {
        return String(self.GetPath().GetString())
    }

    /// The prim's type name.
    public var typeName: TfToken {
        return self.GetTypeName()
    }

    /// The parent prim.
    public var parent: UsdPrim {
        return self.GetParent()
    }

    /// Get an attribute by name.
    public func attribute(_ name: String) -> UsdAttribute {
        return self.GetAttribute(TfToken(name))
    }

    /// Get a relationship by name.
    public func relationship(_ name: String) -> UsdRelationship {
        return self.GetRelationship(TfToken(name))
    }

    /// Create a new attribute.
    public mutating func createAttribute(name: String, typeName: String, custom: Bool = true) -> UsdAttribute {
        return self.CreateAttribute(TfToken(name), std.string(typeName), custom)
    }

    /// Create a new relationship.
    public mutating func createRelationship(name: String, custom: Bool = true) -> UsdRelationship {
        return self.CreateRelationship(TfToken(name), custom)
    }
}

// MARK: - UsdProperty Extensions

extension UsdProperty: CustomStringConvertible {
    /// A textual representation of the property.
    public var description: String {
        if !self.IsValid() {
            return "UsdProperty(invalid)"
        }
        return "UsdProperty(\(pathString))"
    }
}

extension UsdProperty {
    /// Whether this is a valid property reference.
    public var isValid: Bool {
        return self.IsValid()
    }

    /// The property's name.
    public var name: TfToken {
        return self.GetName()
    }

    /// The property's path as a string.
    public var pathString: String {
        return String(self.GetPath().GetString())
    }

    /// The property's base name (without namespace prefixes).
    public var baseName: TfToken {
        return self.GetBaseName()
    }

    /// Whether this is a custom property.
    public var isCustom: Bool {
        return self.IsCustom()
    }

    /// Whether this property is defined.
    public var isDefined: Bool {
        return self.IsDefined()
    }

    /// Whether this property has authored opinions.
    public var isAuthored: Bool {
        return self.IsAuthored()
    }

    /// Whether this is an attribute.
    public var isAttribute: Bool {
        return self.IsAttribute()
    }

    /// Whether this is a relationship.
    public var isRelationship: Bool {
        return self.IsRelationship()
    }
}

// MARK: - UsdAttribute Extensions

extension UsdAttribute: CustomStringConvertible {
    /// A textual representation of the attribute.
    public var description: String {
        if !self.IsValid() {
            return "UsdAttribute(invalid)"
        }
        return "UsdAttribute(\(pathString))"
    }
}

extension UsdAttribute {
    /// Whether this is a valid attribute reference.
    public var isValid: Bool {
        return self.IsValid()
    }

    /// The attribute's name.
    public var name: TfToken {
        return self.GetName()
    }

    /// The attribute's path as a string.
    public var pathString: String {
        return String(self.GetPath().GetString())
    }

    /// The attribute's type name.
    public var typeName: String {
        return String(self.GetTypeName())
    }

    /// Whether the attribute has any value.
    public var hasValue: Bool {
        return self.HasValue()
    }

    /// Whether the attribute has an authored value.
    public var hasAuthoredValue: Bool {
        return self.HasAuthoredValue()
    }
}

// MARK: - UsdRelationship Extensions

extension UsdRelationship: CustomStringConvertible {
    /// A textual representation of the relationship.
    public var description: String {
        if !self.IsValid() {
            return "UsdRelationship(invalid)"
        }
        return "UsdRelationship(\(pathString))"
    }
}

extension UsdRelationship {
    /// Whether this is a valid relationship reference.
    public var isValid: Bool {
        return self.IsValid()
    }

    /// The relationship's name.
    public var name: TfToken {
        return self.GetName()
    }

    /// The relationship's path as a string.
    public var pathString: String {
        return String(self.GetPath().GetString())
    }

    /// Add a target path.
    @discardableResult
    public mutating func addTarget(_ path: SdfPath) -> Bool {
        return self.AddTarget(path)
    }

    /// Add a target path from a string.
    @discardableResult
    public mutating func addTarget(_ path: String) -> Bool {
        return self.AddTarget(SdfPath(path))
    }
}

// MARK: - UsdEditTarget Extensions

extension UsdEditTarget: CustomStringConvertible {
    /// A textual representation of the edit target.
    public var description: String {
        if self.IsNull() {
            return "UsdEditTarget(null)"
        }
        return "UsdEditTarget(\(layer.identifier))"
    }
}

extension UsdEditTarget {
    /// Whether this edit target is null.
    public var isNull: Bool {
        return self.IsNull()
    }

    /// Whether this edit target is valid.
    public var isValid: Bool {
        return self.IsValid()
    }

    /// The layer this edit target contains.
    public var layer: SdfLayer {
        return self.GetLayer()
    }

    /// Map a scene path to a spec path in the edit target's layer.
    public func mapToSpecPath(_ scenePath: SdfPath) -> SdfPath {
        return self.MapToSpecPath(scenePath)
    }

    /// Map a scene path string to a spec path in the edit target's layer.
    public func mapToSpecPath(_ scenePath: String) -> SdfPath {
        return self.MapToSpecPath(SdfPath(scenePath))
    }
}

// MARK: - UsdSchemaBase Extensions

extension UsdSchemaBase {
    /// The prim this schema wraps.
    public var prim: UsdPrim {
        return self.GetPrim()
    }

    /// The path of the wrapped prim.
    public var path: SdfPath {
        return self.GetPath()
    }

    /// Whether this is a concrete (instantiable) schema.
    public var isConcrete: Bool {
        return self.IsConcrete()
    }

    /// Whether this is a typed schema.
    public var isTyped: Bool {
        return self.IsTyped()
    }

    /// Whether this is an API schema.
    public var isAPISchema: Bool {
        return self.IsAPISchema()
    }

    /// Whether this is an applied API schema.
    public var isAppliedAPISchema: Bool {
        return self.IsAppliedAPISchema()
    }

    /// Whether this is a multiple-apply API schema.
    public var isMultipleApplyAPISchema: Bool {
        return self.IsMultipleApplyAPISchema()
    }

    /// The kind of schema this is.
    public var schemaKind: UsdSchemaKind {
        return self.GetSchemaKind()
    }
}
