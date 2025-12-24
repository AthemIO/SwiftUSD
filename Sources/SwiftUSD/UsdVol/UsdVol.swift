// UsdVol.swift - UsdVol module Swift wrappers
// Mirrors: pxr/usd/usdVol/*.h

import OpenUSDInterop
#if canImport(Darwin)
import Darwin
#elseif canImport(Glibc)
import Glibc
#elseif os(Windows)
import ucrt
#endif

// MARK: - FieldDataType

/// Field data type values for volume fields.
public enum FieldDataType: Int, Sendable {
    case unknown = 0
    case half = 1
    case float = 2
    case double = 3
    case int = 4
    case uint = 5
    case int64 = 6
    case half2 = 7
    case float2 = 8
    case double2 = 9
    case int2 = 10
    case half3 = 11
    case float3 = 12
    case double3 = 13
    case int3 = 14
    case matrix3d = 15
    case matrix4d = 16
    case quatd = 17
    case bool = 18
    case mask = 19
    case string = 20

    internal var cValue: UsdVolFieldDataType {
        UsdVolFieldDataType(rawValue: UInt32(rawValue))
    }

    internal init(cValue: UsdVolFieldDataType) {
        self = FieldDataType(rawValue: Int(cValue.rawValue)) ?? .unknown
    }
}

// MARK: - VectorDataRoleHint

/// Vector data role hint values for volume fields.
public enum VectorDataRoleHint: Int, Sendable {
    /// No specific role
    case none = 0
    /// Point position data
    case point = 1
    /// Normal vector data
    case normal = 2
    /// Generic vector data
    case vector = 3
    /// Color data
    case color = 4

    internal var cValue: UsdVolVectorDataRoleHint {
        UsdVolVectorDataRoleHint(rawValue: UInt32(rawValue))
    }

    internal init(cValue: UsdVolVectorDataRoleHint) {
        self = VectorDataRoleHint(rawValue: Int(cValue.rawValue)) ?? .none
    }
}

// MARK: - FieldClass

/// OpenVDB field class values.
public enum FieldClass: Int, Sendable {
    /// Unknown field class
    case unknown = 0
    /// Level set (signed distance field)
    case levelSet = 1
    /// Fog volume (density field)
    case fogVolume = 2
    /// Staggered velocity field
    case staggered = 3

    internal var cValue: UsdVolFieldClass {
        UsdVolFieldClass(rawValue: UInt32(rawValue))
    }

    internal init(cValue: UsdVolFieldClass) {
        self = FieldClass(rawValue: Int(cValue.rawValue)) ?? .unknown
    }
}

// MARK: - Volume

/// A UsdVolVolume is a renderable volume primitive.
///
/// A volume is made up of any number of FieldBase primitives bound together.
/// Each field is specified as a relationship with a namespace prefix of "field".
///
/// Mirrors `pxr::UsdVolVolume` from the USD C++ API.
public final class Volume: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdVolVolumeRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdVolVolumeRef) {
        self.handle = handle
    }

    deinit {
        UsdVolVolume_Release(handle)
    }

    // MARK: - Factory Methods

    /// Defines a new Volume prim at the given path.
    ///
    /// - Parameters:
    ///   - stage: The stage to define the prim on.
    ///   - path: The path for the new Volume prim.
    /// - Returns: The newly defined Volume.
    /// - Throws: `VolError.definitionFailed` if the prim cannot be defined.
    public static func define(on stage: Stage, at path: Path) throws -> Volume {
        guard let ref = UsdVolVolume_Define(stage.handle, path.handle) else {
            throw VolError.definitionFailed("Failed to define Volume at '\(path)'")
        }
        return Volume(handle: ref)
    }

    /// Creates a Volume schema wrapper from an existing prim.
    ///
    /// - Parameter prim: The prim to wrap.
    /// - Returns: A Volume wrapper, or `nil` if the prim is not a valid Volume.
    public static func from(prim: Prim) -> Volume? {
        guard let ref = UsdVolVolume_FromPrim(prim.handle) else {
            return nil
        }
        let volume = Volume(handle: ref)
        return volume.isValid ? volume : nil
    }

    // MARK: - Validity

    /// Returns `true` if the Volume is valid.
    public var isValid: Bool {
        UsdVolVolume_IsValid(handle)
    }

    /// Gets the underlying prim.
    public var prim: Prim? {
        guard let ref = UsdVolVolume_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    // MARK: - Field Relationships

    /// Gets the field paths for this volume as a dictionary mapping field names to paths.
    public func fieldPaths() -> [Token: Path] {
        let count = Int(UsdVolVolume_GetFieldPathCount(handle))
        guard count > 0 else { return [:] }

        var names = [TfTokenRef?](repeating: nil, count: count)
        var paths = [SdfPathRef?](repeating: nil, count: count)

        let actualCount = names.withUnsafeMutableBufferPointer { namesBuffer in
            paths.withUnsafeMutableBufferPointer { pathsBuffer in
                UsdVolVolume_GetFieldPaths(handle, namesBuffer.baseAddress, pathsBuffer.baseAddress, count)
            }
        }

        var result: [Token: Path] = [:]
        for i in 0..<Int(actualCount) {
            if let nameRef = names[i], let pathRef = paths[i] {
                let token = Token(handle: nameRef)
                let path = Path(handle: pathRef)
                result[token] = path
            }
        }
        return result
    }

    /// Checks if a field relationship exists with the given name.
    ///
    /// - Parameter name: The name of the field relationship.
    /// - Returns: `true` if the field relationship exists.
    public func hasFieldRelationship(_ name: Token) -> Bool {
        UsdVolVolume_HasFieldRelationship(handle, name.handle)
    }

    /// Gets the path for a specific field relationship.
    ///
    /// - Parameter name: The name of the field relationship.
    /// - Returns: The path to the field prim, or `nil` if not found.
    public func fieldPath(for name: Token) -> Path? {
        guard let ref = UsdVolVolume_GetFieldPath(handle, name.handle) else {
            return nil
        }
        return Path(handle: ref)
    }

    /// Creates a field relationship on this volume.
    ///
    /// - Parameters:
    ///   - name: The name for the field relationship.
    ///   - fieldPath: The path to the field prim.
    /// - Throws: `VolError.operationFailed` if the relationship cannot be created.
    public func createFieldRelationship(name: Token, fieldPath: Path) throws {
        let result = UsdVolVolume_CreateFieldRelationship(handle, name.handle, fieldPath.handle)
        guard result == USD_RESULT_SUCCESS else {
            throw VolError.operationFailed("Failed to create field relationship '\(name)'")
        }
    }

    /// Blocks an existing field relationship.
    ///
    /// - Parameter name: The name of the field relationship to block.
    /// - Throws: `VolError.operationFailed` if the relationship cannot be blocked.
    public func blockFieldRelationship(name: Token) throws {
        let result = UsdVolVolume_BlockFieldRelationship(handle, name.handle)
        guard result == USD_RESULT_SUCCESS else {
            throw VolError.operationFailed("Failed to block field relationship '\(name)'")
        }
    }
}

// MARK: - CustomStringConvertible

extension Volume: CustomStringConvertible {

    public var description: String {
        prim?.path.description ?? "<invalid volume>"
    }
}

// MARK: - OpenVDBAsset

/// A UsdVolOpenVDBAsset represents an OpenVDB field primitive.
///
/// The filePath attribute must specify a file in the OpenVDB format on disk.
///
/// Mirrors `pxr::UsdVolOpenVDBAsset` from the USD C++ API.
public final class OpenVDBAsset: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdVolOpenVDBAssetRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdVolOpenVDBAssetRef) {
        self.handle = handle
    }

    deinit {
        UsdVolOpenVDBAsset_Release(handle)
    }

    // MARK: - Factory Methods

    /// Defines a new OpenVDBAsset prim at the given path.
    ///
    /// - Parameters:
    ///   - stage: The stage to define the prim on.
    ///   - path: The path for the new OpenVDBAsset prim.
    /// - Returns: The newly defined OpenVDBAsset.
    /// - Throws: `VolError.definitionFailed` if the prim cannot be defined.
    public static func define(on stage: Stage, at path: Path) throws -> OpenVDBAsset {
        guard let ref = UsdVolOpenVDBAsset_Define(stage.handle, path.handle) else {
            throw VolError.definitionFailed("Failed to define OpenVDBAsset at '\(path)'")
        }
        return OpenVDBAsset(handle: ref)
    }

    /// Creates an OpenVDBAsset schema wrapper from an existing prim.
    ///
    /// - Parameter prim: The prim to wrap.
    /// - Returns: An OpenVDBAsset wrapper, or `nil` if the prim is not a valid OpenVDBAsset.
    public static func from(prim: Prim) -> OpenVDBAsset? {
        guard let ref = UsdVolOpenVDBAsset_FromPrim(prim.handle) else {
            return nil
        }
        let field = OpenVDBAsset(handle: ref)
        return field.isValid ? field : nil
    }

    // MARK: - Validity

    /// Returns `true` if the OpenVDBAsset is valid.
    public var isValid: Bool {
        UsdVolOpenVDBAsset_IsValid(handle)
    }

    /// Gets the underlying prim.
    public var prim: Prim? {
        guard let ref = UsdVolOpenVDBAsset_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    // MARK: - File Path

    /// Gets the file path at the given time.
    public func filePath(at time: TimeCode = .default) -> String? {
        guard let cStr = UsdVolOpenVDBAsset_GetFilePath(handle, time.cTimeCode) else {
            return nil
        }
        let result = String(cString: cStr)
        UsdVol_FreeString(cStr)
        return result
    }

    /// Sets the file path at the given time.
    ///
    /// - Parameters:
    ///   - filePath: The path to the OpenVDB file.
    ///   - time: The time code at which to set the value.
    /// - Throws: `VolError.operationFailed` if the operation fails.
    public func setFilePath(_ filePath: String, at time: TimeCode = .default) throws {
        let result = UsdVolOpenVDBAsset_SetFilePath(handle, time.cTimeCode, filePath)
        guard result == USD_RESULT_SUCCESS else {
            throw VolError.operationFailed("Failed to set file path")
        }
    }

    // MARK: - Field Name

    /// Gets the field name within the file at the given time.
    public func fieldName(at time: TimeCode = .default) -> Token? {
        guard let ref = UsdVolOpenVDBAsset_GetFieldName(handle, time.cTimeCode) else {
            return nil
        }
        return Token(handle: ref)
    }

    /// Sets the field name within the file at the given time.
    ///
    /// - Parameters:
    ///   - fieldName: The name of the field within the file.
    ///   - time: The time code at which to set the value.
    /// - Throws: `VolError.operationFailed` if the operation fails.
    public func setFieldName(_ fieldName: Token, at time: TimeCode = .default) throws {
        let result = UsdVolOpenVDBAsset_SetFieldName(handle, time.cTimeCode, fieldName.handle)
        guard result == USD_RESULT_SUCCESS else {
            throw VolError.operationFailed("Failed to set field name")
        }
    }

    // MARK: - Field Index

    /// Gets the field index for disambiguation at the given time.
    public func fieldIndex(at time: TimeCode = .default) -> Int {
        Int(UsdVolOpenVDBAsset_GetFieldIndex(handle, time.cTimeCode))
    }

    /// Sets the field index for disambiguation at the given time.
    ///
    /// - Parameters:
    ///   - fieldIndex: The index for disambiguation.
    ///   - time: The time code at which to set the value.
    /// - Throws: `VolError.operationFailed` if the operation fails.
    public func setFieldIndex(_ fieldIndex: Int, at time: TimeCode = .default) throws {
        let result = UsdVolOpenVDBAsset_SetFieldIndex(handle, time.cTimeCode, Int32(fieldIndex))
        guard result == USD_RESULT_SUCCESS else {
            throw VolError.operationFailed("Failed to set field index")
        }
    }

    // MARK: - Field Data Type

    /// Gets the field data type.
    public var fieldDataType: FieldDataType {
        FieldDataType(cValue: UsdVolOpenVDBAsset_GetFieldDataType(handle))
    }

    /// Sets the field data type.
    ///
    /// - Parameter dataType: The data type of the field.
    /// - Throws: `VolError.operationFailed` if the operation fails.
    public func setFieldDataType(_ dataType: FieldDataType) throws {
        let result = UsdVolOpenVDBAsset_SetFieldDataType(handle, dataType.cValue)
        guard result == USD_RESULT_SUCCESS else {
            throw VolError.operationFailed("Failed to set field data type")
        }
    }

    // MARK: - Field Class

    /// Gets the OpenVDB field class.
    public var fieldClass: FieldClass {
        FieldClass(cValue: UsdVolOpenVDBAsset_GetFieldClass(handle))
    }

    /// Sets the OpenVDB field class.
    ///
    /// - Parameter fieldClass: The class of the field.
    /// - Throws: `VolError.operationFailed` if the operation fails.
    public func setFieldClass(_ fieldClass: FieldClass) throws {
        let result = UsdVolOpenVDBAsset_SetFieldClass(handle, fieldClass.cValue)
        guard result == USD_RESULT_SUCCESS else {
            throw VolError.operationFailed("Failed to set field class")
        }
    }
}

// MARK: - CustomStringConvertible

extension OpenVDBAsset: CustomStringConvertible {

    public var description: String {
        prim?.path.description ?? "<invalid openvdb asset>"
    }
}

// MARK: - Field3DAsset

/// A UsdVolField3DAsset represents a Field3D field primitive.
///
/// The filePath attribute must specify a file in the Field3D format on disk.
///
/// Mirrors `pxr::UsdVolField3DAsset` from the USD C++ API.
public final class Field3DAsset: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdVolField3DAssetRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdVolField3DAssetRef) {
        self.handle = handle
    }

    deinit {
        UsdVolField3DAsset_Release(handle)
    }

    // MARK: - Factory Methods

    /// Defines a new Field3DAsset prim at the given path.
    ///
    /// - Parameters:
    ///   - stage: The stage to define the prim on.
    ///   - path: The path for the new Field3DAsset prim.
    /// - Returns: The newly defined Field3DAsset.
    /// - Throws: `VolError.definitionFailed` if the prim cannot be defined.
    public static func define(on stage: Stage, at path: Path) throws -> Field3DAsset {
        guard let ref = UsdVolField3DAsset_Define(stage.handle, path.handle) else {
            throw VolError.definitionFailed("Failed to define Field3DAsset at '\(path)'")
        }
        return Field3DAsset(handle: ref)
    }

    /// Creates a Field3DAsset schema wrapper from an existing prim.
    ///
    /// - Parameter prim: The prim to wrap.
    /// - Returns: A Field3DAsset wrapper, or `nil` if the prim is not a valid Field3DAsset.
    public static func from(prim: Prim) -> Field3DAsset? {
        guard let ref = UsdVolField3DAsset_FromPrim(prim.handle) else {
            return nil
        }
        let field = Field3DAsset(handle: ref)
        return field.isValid ? field : nil
    }

    // MARK: - Validity

    /// Returns `true` if the Field3DAsset is valid.
    public var isValid: Bool {
        UsdVolField3DAsset_IsValid(handle)
    }

    /// Gets the underlying prim.
    public var prim: Prim? {
        guard let ref = UsdVolField3DAsset_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    // MARK: - File Path

    /// Gets the file path at the given time.
    public func filePath(at time: TimeCode = .default) -> String? {
        guard let cStr = UsdVolField3DAsset_GetFilePath(handle, time.cTimeCode) else {
            return nil
        }
        let result = String(cString: cStr)
        UsdVol_FreeString(cStr)
        return result
    }

    /// Sets the file path at the given time.
    ///
    /// - Parameters:
    ///   - filePath: The path to the Field3D file.
    ///   - time: The time code at which to set the value.
    /// - Throws: `VolError.operationFailed` if the operation fails.
    public func setFilePath(_ filePath: String, at time: TimeCode = .default) throws {
        let result = UsdVolField3DAsset_SetFilePath(handle, time.cTimeCode, filePath)
        guard result == USD_RESULT_SUCCESS else {
            throw VolError.operationFailed("Failed to set file path")
        }
    }

    // MARK: - Field Name

    /// Gets the field name within the file at the given time.
    public func fieldName(at time: TimeCode = .default) -> Token? {
        guard let ref = UsdVolField3DAsset_GetFieldName(handle, time.cTimeCode) else {
            return nil
        }
        return Token(handle: ref)
    }

    /// Sets the field name within the file at the given time.
    ///
    /// - Parameters:
    ///   - fieldName: The name of the field within the file.
    ///   - time: The time code at which to set the value.
    /// - Throws: `VolError.operationFailed` if the operation fails.
    public func setFieldName(_ fieldName: Token, at time: TimeCode = .default) throws {
        let result = UsdVolField3DAsset_SetFieldName(handle, time.cTimeCode, fieldName.handle)
        guard result == USD_RESULT_SUCCESS else {
            throw VolError.operationFailed("Failed to set field name")
        }
    }

    // MARK: - Field Index

    /// Gets the field index for disambiguation at the given time.
    public func fieldIndex(at time: TimeCode = .default) -> Int {
        Int(UsdVolField3DAsset_GetFieldIndex(handle, time.cTimeCode))
    }

    /// Sets the field index for disambiguation at the given time.
    ///
    /// - Parameters:
    ///   - fieldIndex: The index for disambiguation.
    ///   - time: The time code at which to set the value.
    /// - Throws: `VolError.operationFailed` if the operation fails.
    public func setFieldIndex(_ fieldIndex: Int, at time: TimeCode = .default) throws {
        let result = UsdVolField3DAsset_SetFieldIndex(handle, time.cTimeCode, Int32(fieldIndex))
        guard result == USD_RESULT_SUCCESS else {
            throw VolError.operationFailed("Failed to set field index")
        }
    }

    // MARK: - Field Data Type

    /// Gets the field data type.
    public var fieldDataType: FieldDataType {
        FieldDataType(cValue: UsdVolField3DAsset_GetFieldDataType(handle))
    }

    /// Sets the field data type.
    ///
    /// - Parameter dataType: The data type of the field.
    /// - Throws: `VolError.operationFailed` if the operation fails.
    public func setFieldDataType(_ dataType: FieldDataType) throws {
        let result = UsdVolField3DAsset_SetFieldDataType(handle, dataType.cValue)
        guard result == USD_RESULT_SUCCESS else {
            throw VolError.operationFailed("Failed to set field data type")
        }
    }

    // MARK: - Field Purpose

    /// Gets the field purpose (Field3D-specific).
    public var fieldPurpose: Token? {
        guard let ref = UsdVolField3DAsset_GetFieldPurpose(handle) else {
            return nil
        }
        return Token(handle: ref)
    }

    /// Sets the field purpose (Field3D-specific).
    ///
    /// - Parameter purpose: The purpose token.
    /// - Throws: `VolError.operationFailed` if the operation fails.
    public func setFieldPurpose(_ purpose: Token) throws {
        let result = UsdVolField3DAsset_SetFieldPurpose(handle, purpose.handle)
        guard result == USD_RESULT_SUCCESS else {
            throw VolError.operationFailed("Failed to set field purpose")
        }
    }
}

// MARK: - CustomStringConvertible

extension Field3DAsset: CustomStringConvertible {

    public var description: String {
        prim?.path.description ?? "<invalid field3d asset>"
    }
}

// MARK: - VolError

/// Errors that can occur when working with UsdVol types.
public enum VolError: Error, CustomStringConvertible {

    /// Failed to define a volume prim.
    case definitionFailed(String)

    /// A volume operation failed.
    case operationFailed(String)

    public var description: String {
        switch self {
        case .definitionFailed(let message):
            return "Volume definition failed: \(message)"
        case .operationFailed(let message):
            return "Volume operation failed: \(message)"
        }
    }
}

// MARK: - Type Aliases

/// Type alias for UsdVolVolume
public typealias UsdVolVolume = Volume

/// Type alias for UsdVolOpenVDBAsset
public typealias UsdVolOpenVDBAsset = OpenVDBAsset

/// Type alias for UsdVolField3DAsset
public typealias UsdVolField3DAsset = Field3DAsset
