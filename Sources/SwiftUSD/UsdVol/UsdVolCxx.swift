// UsdVolCxx.swift - Swift convenience wrappers for UsdVol C++ interop
// Provides Swift-idiomatic access to UsdVol volume schemas

import USDCxx

// MARK: - Type Aliases for UsdVol types

/// UsdVolVolume - Renderable volume primitive
public typealias Volume = swiftusd.VolVolumeRef

/// UsdVolFieldBase - Base class for field primitives
public typealias FieldBase = swiftusd.VolFieldBaseRef

/// UsdVolFieldAsset - Base class for file-based fields
public typealias FieldAsset = swiftusd.VolFieldAssetRef

/// UsdVolOpenVDBAsset - OpenVDB field primitive
public typealias OpenVDBAsset = swiftusd.VolOpenVDBAssetRef

/// UsdVolField3DAsset - Field3D field primitive
public typealias Field3DAsset = swiftusd.VolField3DAssetRef

/// VectorDataRoleHint - Hint for vector data interpretation
public typealias VectorDataRoleHint = swiftusd.VectorDataRoleHint

/// OpenVDBFieldClass - OpenVDB grid classification
public typealias OpenVDBFieldClass = swiftusd.OpenVDBFieldClass

// MARK: - VectorDataRoleHint Extensions

extension swiftusd.VectorDataRoleHint {
    /// No special role
    public static var none: swiftusd.VectorDataRoleHint { swiftusd.VectorDataRoleHint(swiftusd.VectorDataRoleHint.Value(rawValue: 0)) }
    /// Position data
    public static var point: swiftusd.VectorDataRoleHint { swiftusd.VectorDataRoleHint(swiftusd.VectorDataRoleHint.Value(rawValue: 1)) }
    /// Normal vector
    public static var normal: swiftusd.VectorDataRoleHint { swiftusd.VectorDataRoleHint(swiftusd.VectorDataRoleHint.Value(rawValue: 2)) }
    /// Generic vector
    public static var vector: swiftusd.VectorDataRoleHint { swiftusd.VectorDataRoleHint(swiftusd.VectorDataRoleHint.Value(rawValue: 3)) }
    /// Color data
    public static var color: swiftusd.VectorDataRoleHint { swiftusd.VectorDataRoleHint(swiftusd.VectorDataRoleHint.Value(rawValue: 4)) }
}

// MARK: - OpenVDBFieldClass Extensions

extension swiftusd.OpenVDBFieldClass {
    /// Unknown grid class
    public static var unknown: swiftusd.OpenVDBFieldClass { swiftusd.OpenVDBFieldClass(swiftusd.OpenVDBFieldClass.Value(rawValue: 0)) }
    /// Level set (signed distance field)
    public static var levelSet: swiftusd.OpenVDBFieldClass { swiftusd.OpenVDBFieldClass(swiftusd.OpenVDBFieldClass.Value(rawValue: 1)) }
    /// Fog volume
    public static var fogVolume: swiftusd.OpenVDBFieldClass { swiftusd.OpenVDBFieldClass(swiftusd.OpenVDBFieldClass.Value(rawValue: 2)) }
    /// Staggered MAC grid
    public static var staggered: swiftusd.OpenVDBFieldClass { swiftusd.OpenVDBFieldClass(swiftusd.OpenVDBFieldClass.Value(rawValue: 3)) }
}

// MARK: - Volume Extensions

extension swiftusd.VolVolumeRef {
    /// Create a volume at the given path
    public static func define(on stage: swiftusd.StageRef, at path: swiftusd.Path) -> swiftusd.VolVolumeRef? {
        Define(stage, path)
    }

    /// Create from an existing prim
    public static func fromPrim(_ prim: swiftusd.PrimRef) -> swiftusd.VolVolumeRef? {
        FromPrim(prim)
    }

    /// Create a field relationship binding this volume to a field prim
    @discardableResult
    public func createField(name: String, fieldPath: swiftusd.Path) -> Bool {
        CreateFieldRelationship(swiftusd.Token(name), fieldPath)
    }

    /// Check if a field relationship exists
    public func hasField(name: String) -> Bool {
        HasFieldRelationship(swiftusd.Token(name))
    }

    /// Get the path to a field by relationship name
    public func fieldPath(name: String) -> swiftusd.Path {
        GetFieldPath(swiftusd.Token(name))
    }

    /// Block/remove a field relationship
    @discardableResult
    public func blockField(name: String) -> Bool {
        BlockFieldRelationship(swiftusd.Token(name))
    }

    /// The visibility of this volume
    public var visibility: swiftusd.Token {
        get { GetVisibility() }
        set { SetVisibility(newValue) }
    }

    /// The purpose of this volume
    public var purpose: swiftusd.Token {
        get { GetPurpose() }
        set { SetPurpose(newValue) }
    }
}

// MARK: - FieldBase Extensions

extension swiftusd.VolFieldBaseRef {
    /// Create from an existing prim
    public static func fromPrim(_ prim: swiftusd.PrimRef) -> swiftusd.VolFieldBaseRef? {
        FromPrim(prim)
    }
}

// MARK: - FieldAsset Extensions

extension swiftusd.VolFieldAssetRef {
    /// Create from an existing prim
    public static func fromPrim(_ prim: swiftusd.PrimRef) -> swiftusd.VolFieldAssetRef? {
        FromPrim(prim)
    }

    /// The file path to the volume asset
    public func getFilePath() -> String {
        String(cString: self.GetFilePath())
    }

    /// Set the file path to the volume asset
    @discardableResult
    public func setFilePath(_ path: String) -> Bool {
        self.SetFilePath(path)
    }

    /// The name of the field within the file
    public var fieldName: swiftusd.Token {
        get { GetFieldName() }
        set { SetFieldName(newValue) }
    }

    /// The index of the field (for disambiguating multiple fields with same name)
    public var fieldIndex: Int32 {
        get { GetFieldIndex() }
        set { SetFieldIndex(newValue) }
    }

    /// The data type of the field
    public var fieldDataType: swiftusd.Token {
        get { GetFieldDataType() }
        set { SetFieldDataType(newValue) }
    }

    /// The vector data role hint
    public var vectorDataRoleHint: swiftusd.VectorDataRoleHint {
        get { GetVectorDataRoleHint() }
        set { SetVectorDataRoleHint(newValue) }
    }
}

// MARK: - OpenVDBAsset Extensions

extension swiftusd.VolOpenVDBAssetRef {
    /// Create an OpenVDB asset at the given path
    public static func define(on stage: swiftusd.StageRef, at path: swiftusd.Path) -> swiftusd.VolOpenVDBAssetRef? {
        Define(stage, path)
    }

    /// Create from an existing prim
    public static func fromPrim(_ prim: swiftusd.PrimRef) -> swiftusd.VolOpenVDBAssetRef? {
        FromPrim(prim)
    }

    /// The file path to the OpenVDB file
    public func getFilePath() -> String {
        String(cString: self.GetFilePath())
    }

    /// Set the file path to the OpenVDB file
    @discardableResult
    public func setFilePath(_ path: String) -> Bool {
        self.SetFilePath(path)
    }

    /// The name of the field within the VDB file
    public var fieldName: swiftusd.Token {
        get { GetFieldName() }
        set { SetFieldName(newValue) }
    }

    /// The index of the field (for disambiguating multiple fields with same name)
    public var fieldIndex: Int32 {
        get { GetFieldIndex() }
        set { SetFieldIndex(newValue) }
    }

    /// The data type of the field
    public var fieldDataType: swiftusd.Token {
        get { GetFieldDataType() }
        set { SetFieldDataType(newValue) }
    }

    /// The vector data role hint
    public var vectorDataRoleHint: swiftusd.VectorDataRoleHint {
        get { GetVectorDataRoleHint() }
        set { SetVectorDataRoleHint(newValue) }
    }

    /// The OpenVDB field class (levelSet, fogVolume, staggered, unknown)
    public var fieldClass: swiftusd.OpenVDBFieldClass {
        get { GetFieldClass() }
        set { SetFieldClass(newValue) }
    }
}

// MARK: - Field3DAsset Extensions

extension swiftusd.VolField3DAssetRef {
    /// Create a Field3D asset at the given path
    public static func define(on stage: swiftusd.StageRef, at path: swiftusd.Path) -> swiftusd.VolField3DAssetRef? {
        Define(stage, path)
    }

    /// Create from an existing prim
    public static func fromPrim(_ prim: swiftusd.PrimRef) -> swiftusd.VolField3DAssetRef? {
        FromPrim(prim)
    }

    /// The file path to the Field3D file
    public func getFilePath() -> String {
        String(cString: self.GetFilePath())
    }

    /// Set the file path to the Field3D file
    @discardableResult
    public func setFilePath(_ path: String) -> Bool {
        self.SetFilePath(path)
    }

    /// The name of the field within the file
    public var fieldName: swiftusd.Token {
        get { GetFieldName() }
        set { SetFieldName(newValue) }
    }

    /// The index of the field (for disambiguating multiple fields with same name)
    public var fieldIndex: Int32 {
        get { GetFieldIndex() }
        set { SetFieldIndex(newValue) }
    }

    /// The data type of the field
    public var fieldDataType: swiftusd.Token {
        get { GetFieldDataType() }
        set { SetFieldDataType(newValue) }
    }

    /// The vector data role hint
    public var vectorDataRoleHint: swiftusd.VectorDataRoleHint {
        get { GetVectorDataRoleHint() }
        set { SetVectorDataRoleHint(newValue) }
    }

    /// The field purpose (Field3D-specific grouping token)
    public var fieldPurpose: swiftusd.Token {
        get { GetFieldPurpose() }
        set { SetFieldPurpose(newValue) }
    }
}
