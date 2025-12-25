// UsdCxx.swift - Swift extensions for Usd C++ types
// Provides Swift-idiomatic API for swiftusd::StageRef, swiftusd::PrimRef,
// swiftusd::AttributeRef, swiftusd::RelationshipRef, and swiftusd::TimeCode

import USDCxx

// MARK: - Type Aliases for C++ Interop Types

/// Type alias for the C++ TimeCode type
public typealias CxxTimeCode = swiftusd.TimeCode

/// Type alias for the C++ StageRef type
public typealias CxxStageRef = swiftusd.StageRef

/// Type alias for the C++ PrimRef type
public typealias CxxPrimRef = swiftusd.PrimRef

/// Type alias for the C++ AttributeRef type
public typealias CxxAttributeRef = swiftusd.AttributeRef

/// Type alias for the C++ RelationshipRef type
public typealias CxxRelationshipRef = swiftusd.RelationshipRef

// MARK: - TimeCode Extensions

extension swiftusd.TimeCode: CustomStringConvertible {
    public var description: String {
        if self.IsDefault() {
            return "TimeCode.Default"
        } else if self.IsEarliestTime() {
            return "TimeCode.EarliestTime"
        } else {
            return "TimeCode(\(self.value))"
        }
    }
}

extension swiftusd.TimeCode: CustomDebugStringConvertible {
    public var debugDescription: String {
        description
    }
}

extension swiftusd.TimeCode: Hashable {
    public func hash(into hasher: inout Hasher) {
        if self.IsDefault() {
            hasher.combine(Int.max)
        } else {
            hasher.combine(self.value)
        }
    }
}

extension swiftusd.TimeCode: ExpressibleByFloatLiteral {
    public init(floatLiteral value: Double) {
        self.init(value)
    }
}

extension swiftusd.TimeCode: ExpressibleByIntegerLiteral {
    public init(integerLiteral value: Int) {
        self.init(Double(value))
    }
}

extension swiftusd.TimeCode {
    /// Whether this is the default time code (non-animated)
    public var isDefault: Bool {
        self.IsDefault()
    }

    /// Whether this is the earliest time
    public var isEarliestTime: Bool {
        self.IsEarliestTime()
    }

    /// Whether this is a numeric time code
    public var isNumeric: Bool {
        self.IsNumeric()
    }
}

// MARK: - StageRef Extensions

extension swiftusd.StageRef {
    /// Creates a new stage with the given identifier
    public static func createNew(_ identifier: String) -> swiftusd.StageRef? {
        swiftusd.StageRef.CreateNew(identifier)
    }

    /// Creates a new in-memory stage with optional identifier
    public static func createInMemory(identifier: String? = nil) -> swiftusd.StageRef? {
        if let id = identifier {
            return swiftusd.StageRef.CreateInMemory(id)
        }
        return swiftusd.StageRef.CreateInMemory(nil)
    }

    /// Opens a stage from the given file path
    public static func open(_ path: String) -> swiftusd.StageRef? {
        swiftusd.StageRef.Open(path)
    }

    /// The stage's identifier
    public var identifier: String {
        String(cString: self.GetIdentifier())
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

    /// Whether the stage has a default prim
    public var hasDefaultPrim: Bool {
        self.HasDefaultPrim()
    }

    /// The number of root prims
    public var rootPrimCount: Int {
        Int(self.GetRootPrimCount())
    }

    /// Gets the prim at the specified path
    public func prim(at path: swiftusd.Path) -> swiftusd.PrimRef? {
        self.GetPrimAtPath(path)
    }

    /// Gets the prim at the specified path string
    public func prim(atPath pathString: String) -> swiftusd.PrimRef? {
        self.GetPrimAtPath(swiftusd.Path(pathString))
    }

    /// Gets the pseudo root prim
    public var pseudoRoot: swiftusd.PrimRef? {
        self.GetPseudoRoot()
    }

    /// Gets the default prim
    public var defaultPrim: swiftusd.PrimRef? {
        get { self.GetDefaultPrim() }
        set { self.SetDefaultPrim(newValue) }
    }

    /// Defines a new prim at the given path with optional type name
    public func definePrim(at path: swiftusd.Path, typeName: String = "") -> swiftusd.PrimRef? {
        self.DefinePrim(path, typeName.isEmpty ? swiftusd.Token() : swiftusd.Token(typeName))
    }

    /// Defines a new prim at the given path string with optional type name
    public func definePrim(atPath pathString: String, typeName: String = "") -> swiftusd.PrimRef? {
        definePrim(at: swiftusd.Path(pathString), typeName: typeName)
    }

    /// Creates an override prim at the given path
    public func overridePrim(at path: swiftusd.Path) -> swiftusd.PrimRef? {
        self.OverridePrim(path)
    }

    /// Removes the prim at the given path
    @discardableResult
    public func removePrim(at path: swiftusd.Path) -> Bool {
        self.RemovePrim(path)
    }

    /// Gets the root layer
    public var rootLayer: swiftusd.LayerRef? {
        self.GetRootLayer()
    }

    /// Gets the session layer
    public var sessionLayer: swiftusd.LayerRef? {
        self.GetSessionLayer()
    }

    /// Flattens the stage into a single layer
    public func flatten(addSourceFileComment: Bool = true) -> swiftusd.LayerRef? {
        self.Flatten(addSourceFileComment)
    }

    /// Saves the stage
    @discardableResult
    public func save() -> Bool {
        self.Save()
    }

    /// Exports the stage to the given path
    @discardableResult
    public func export(to path: String, addSourceReference: Bool = false) -> Bool {
        self.Export(path, addSourceReference)
    }

    /// Reloads the stage
    public func reload() {
        self.Reload()
    }

    /// Clears the default prim
    public func clearDefaultPrim() {
        self.ClearDefaultPrim()
    }
}

// MARK: - PrimRef Extensions

extension swiftusd.PrimRef {
    /// The prim's path
    public var path: swiftusd.Path {
        self.GetPath()
    }

    /// The prim's name token
    public var name: swiftusd.Token {
        self.GetName()
    }

    /// The prim's name as a string
    public var nameString: String {
        String(cString: self.GetName().GetText())
    }

    /// The prim's type name
    public var typeName: swiftusd.Token {
        self.GetTypeName()
    }

    /// The prim's type name as a string
    public var typeNameString: String {
        String(cString: self.GetTypeName().GetText())
    }

    /// Whether the prim is valid
    public var isValid: Bool {
        self.IsValid()
    }

    /// Whether the prim is active
    public var isActive: Bool {
        get { self.IsActive() }
        set { self.SetActive(newValue) }
    }

    /// Whether the prim is defined
    public var isDefined: Bool {
        self.IsDefined()
    }

    /// Whether the prim is abstract
    public var isAbstract: Bool {
        self.IsAbstract()
    }

    /// Whether the prim is a model
    public var isModel: Bool {
        self.IsModel()
    }

    /// Whether the prim is a group
    public var isGroup: Bool {
        self.IsGroup()
    }

    /// The number of children
    public var childCount: Int {
        Int(self.GetChildCount())
    }

    /// The number of attributes
    public var attributeCount: Int {
        Int(self.GetAttributeCount())
    }

    /// The parent prim
    public var parent: swiftusd.PrimRef? {
        self.GetParent()
    }

    /// The owning stage
    public var stage: swiftusd.StageRef? {
        self.GetStage()
    }

    /// Gets a child by name
    public func child(named name: String) -> swiftusd.PrimRef? {
        self.GetChild(swiftusd.Token(name))
    }

    /// Gets all children
    /// Note: In standalone mode, children may not be populated automatically.
    /// Use stage.definePrim() to create the hierarchy explicitly.
    public var children: [swiftusd.PrimRef] {
        // For now, return empty array - child traversal requires
        // more complex memory management with SWIFT_UNSAFE_REFERENCE types
        return []
    }

    /// Checks if this prim is a type of the given schema
    public func isA(_ schemaType: String) -> Bool {
        self.IsA(swiftusd.Token(schemaType))
    }

    /// Checks if this prim has the given API schema applied
    public func hasAPI(_ apiSchema: String) -> Bool {
        self.HasAPI(swiftusd.Token(apiSchema))
    }

    /// Gets an attribute by name
    public func attribute(named name: String) -> swiftusd.AttributeRef? {
        self.GetAttribute(swiftusd.Token(name))
    }

    /// Creates an attribute with the given name and type
    public func createAttribute(named name: String, typeName: String, custom: Bool = true) -> swiftusd.AttributeRef? {
        self.CreateAttribute(swiftusd.Token(name), swiftusd.Token(typeName), custom)
    }

    /// Checks if an attribute with the given name exists
    public func hasAttribute(_ name: String) -> Bool {
        self.HasAttribute(swiftusd.Token(name))
    }

    /// Gets a relationship by name
    public func relationship(named name: String) -> swiftusd.RelationshipRef? {
        self.GetRelationship(swiftusd.Token(name))
    }

    /// Creates a relationship with the given name
    public func createRelationship(named name: String, custom: Bool = true) -> swiftusd.RelationshipRef? {
        self.CreateRelationship(swiftusd.Token(name), custom)
    }

    /// Checks if a relationship with the given name exists
    public func hasRelationship(_ name: String) -> Bool {
        self.HasRelationship(swiftusd.Token(name))
    }

    /// Checks if metadata with the given key exists
    public func hasMetadata(_ key: String) -> Bool {
        self.HasMetadata(swiftusd.Token(key))
    }

    /// The prim's kind
    public var kind: swiftusd.Token {
        get { self.GetKind() }
        set { _ = self.SetKind(newValue) }
    }
}

// MARK: - AttributeRef Extensions

extension swiftusd.AttributeRef {
    /// The attribute's name
    public var name: swiftusd.Token {
        self.GetName()
    }

    /// The attribute's name as a string
    public var nameString: String {
        String(cString: self.GetName().GetText())
    }

    /// The attribute's type name
    public var typeName: swiftusd.Token {
        self.GetTypeName()
    }

    /// The attribute's path
    public var path: swiftusd.Path {
        self.GetPath()
    }

    /// Whether the attribute is valid
    public var isValid: Bool {
        self.IsValid()
    }

    /// Whether the attribute is defined
    public var isDefined: Bool {
        self.IsDefined()
    }

    /// Whether the attribute is authored
    public var isAuthored: Bool {
        self.IsAuthored()
    }

    /// Whether the attribute is custom
    public var isCustom: Bool {
        self.IsCustom()
    }

    /// Whether the attribute has a value
    public var hasValue: Bool {
        self.HasValue()
    }

    /// The owning prim
    public var prim: swiftusd.PrimRef? {
        self.GetPrim()
    }

    /// Gets the attribute's boolean value at the given time
    public func getBool(at time: swiftusd.TimeCode = .Default()) -> Bool? {
        var value: Bool = false
        return self.GetBool(time, &value) ? value : nil
    }

    /// Gets the attribute's integer value at the given time
    public func getInt(at time: swiftusd.TimeCode = .Default()) -> Int? {
        var value: Int32 = 0
        return self.GetInt(time, &value) ? Int(value) : nil
    }

    /// Gets the attribute's float value at the given time
    public func getFloat(at time: swiftusd.TimeCode = .Default()) -> Float? {
        var value: Float = 0
        return self.GetFloat(time, &value) ? value : nil
    }

    /// Gets the attribute's double value at the given time
    public func getDouble(at time: swiftusd.TimeCode = .Default()) -> Double? {
        var value: Double = 0
        return self.GetDouble(time, &value) ? value : nil
    }

    /// Gets the attribute's token value at the given time
    public func getToken(at time: swiftusd.TimeCode = .Default()) -> swiftusd.Token? {
        var value = swiftusd.Token()
        return self.GetToken(time, &value) ? value : nil
    }

    /// Gets the attribute's Vec3f value at the given time
    public func getVec3f(at time: swiftusd.TimeCode = .Default()) -> swiftusd.Vec3f? {
        var value = swiftusd.Vec3f()
        return self.GetVec3f(time, &value) ? value : nil
    }

    /// Gets the attribute's Vec3d value at the given time
    public func getVec3d(at time: swiftusd.TimeCode = .Default()) -> swiftusd.Vec3d? {
        var value = swiftusd.Vec3d()
        return self.GetVec3d(time, &value) ? value : nil
    }

    /// Gets the attribute's Matrix4d value at the given time
    public func getMatrix4d(at time: swiftusd.TimeCode = .Default()) -> swiftusd.Matrix4d? {
        var value = swiftusd.Matrix4d()
        return self.GetMatrix4d(time, &value) ? value : nil
    }

    /// Gets the attribute's Vec3fArray value at the given time
    public func getVec3fArray(at time: swiftusd.TimeCode = .Default()) -> swiftusd.Vec3fArray? {
        self.GetVec3fArray(time)
    }

    /// Gets the attribute's IntArray value at the given time
    public func getIntArray(at time: swiftusd.TimeCode = .Default()) -> swiftusd.IntArray? {
        self.GetIntArray(time)
    }

    /// Gets the attribute's FloatArray value at the given time
    public func getFloatArray(at time: swiftusd.TimeCode = .Default()) -> swiftusd.FloatArray? {
        self.GetFloatArray(time)
    }

    /// Sets the attribute's boolean value at the given time
    @discardableResult
    public func set(_ value: Bool, at time: swiftusd.TimeCode = .Default()) -> Bool {
        self.SetBool(value, time)
    }

    /// Sets the attribute's integer value at the given time
    @discardableResult
    public func set(_ value: Int, at time: swiftusd.TimeCode = .Default()) -> Bool {
        self.SetInt(Int32(value), time)
    }

    /// Sets the attribute's float value at the given time
    @discardableResult
    public func set(_ value: Float, at time: swiftusd.TimeCode = .Default()) -> Bool {
        self.SetFloat(value, time)
    }

    /// Sets the attribute's double value at the given time
    @discardableResult
    public func set(_ value: Double, at time: swiftusd.TimeCode = .Default()) -> Bool {
        self.SetDouble(value, time)
    }

    /// Sets the attribute's string value at the given time
    @discardableResult
    public func set(_ value: String, at time: swiftusd.TimeCode = .Default()) -> Bool {
        self.SetString(value, time)
    }

    /// Sets the attribute's token value at the given time
    @discardableResult
    public func set(_ value: swiftusd.Token, at time: swiftusd.TimeCode = .Default()) -> Bool {
        self.SetToken(value, time)
    }

    /// Sets the attribute's Vec3f value at the given time
    @discardableResult
    public func set(_ value: swiftusd.Vec3f, at time: swiftusd.TimeCode = .Default()) -> Bool {
        self.SetVec3f(value, time)
    }

    /// Sets the attribute's Vec3d value at the given time
    @discardableResult
    public func set(_ value: swiftusd.Vec3d, at time: swiftusd.TimeCode = .Default()) -> Bool {
        self.SetVec3d(value, time)
    }

    /// Sets the attribute's Matrix4d value at the given time
    @discardableResult
    public func set(_ value: swiftusd.Matrix4d, at time: swiftusd.TimeCode = .Default()) -> Bool {
        self.SetMatrix4d(value, time)
    }

    /// Clears the attribute's value
    @discardableResult
    public func clear() -> Bool {
        self.Clear()
    }
}

// MARK: - RelationshipRef Extensions

extension swiftusd.RelationshipRef {
    /// The relationship's name
    public var name: swiftusd.Token {
        self.GetName()
    }

    /// The relationship's name as a string
    public var nameString: String {
        String(cString: self.GetName().GetText())
    }

    /// The relationship's path
    public var path: swiftusd.Path {
        self.GetPath()
    }

    /// Whether the relationship is valid
    public var isValid: Bool {
        self.IsValid()
    }

    /// Whether the relationship is custom
    public var isCustom: Bool {
        self.IsCustom()
    }

    /// The number of targets
    public var targetCount: Int {
        Int(self.GetTargetCount())
    }

    /// The owning prim
    public var prim: swiftusd.PrimRef? {
        self.GetPrim()
    }

    /// Gets all targets
    public var targets: [swiftusd.Path] {
        let count = self.targetCount
        guard count > 0 else { return [] }

        var paths = [swiftusd.Path](repeating: swiftusd.Path(), count: count)
        self.GetTargets(&paths, count)
        return paths
    }

    /// Adds a target path
    @discardableResult
    public func addTarget(_ path: swiftusd.Path, position: Bool = false) -> Bool {
        self.AddTarget(path, position)
    }

    /// Adds a target path from a string
    @discardableResult
    public func addTarget(_ pathString: String, position: Bool = false) -> Bool {
        self.AddTarget(swiftusd.Path(pathString), position)
    }

    /// Removes a target path
    @discardableResult
    public func removeTarget(_ path: swiftusd.Path) -> Bool {
        self.RemoveTarget(path)
    }

    /// Clears all targets
    @discardableResult
    public func clearTargets(removeSpec: Bool = true) -> Bool {
        self.ClearTargets(removeSpec)
    }
}
