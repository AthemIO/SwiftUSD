// Value.swift - VtValue wrapper
// Mirrors: pxr/base/vt/value.h

import OpenUSDInterop

// MARK: - ValueType

/// Type identifiers for values that can be held by `Value`.
///
/// Corresponds to commonly used types in USD.
public enum ValueType: Int, Sendable {
    case unknown = 0
    case bool
    case int
    case int64
    case uint
    case uint64
    case float
    case double
    case half
    case string
    case token
    case vec2f
    case vec2d
    case vec2i
    case vec3f
    case vec3d
    case vec3i
    case vec4f
    case vec4d
    case vec4i
    case matrix2d
    case matrix3d
    case matrix4d
    case quatd
    case quatf
    case range3d
    case arrayBool
    case arrayInt
    case arrayInt64
    case arrayUInt
    case arrayUInt64
    case arrayFloat
    case arrayDouble
    case arrayHalf
    case arrayString
    case arrayToken
    case arrayVec2f
    case arrayVec2d
    case arrayVec3f
    case arrayVec3d
    case arrayVec4f
    case arrayVec4d
    case arrayMatrix4d
    case dictionary

    /// Converts from the C enum type.
    init(_ cType: VtValueType) {
        self = ValueType(rawValue: Int(cType.rawValue)) ?? .unknown
    }

    /// Converts to the C enum type.
    var cType: VtValueType {
        VtValueType(rawValue: UInt32(rawValue))
    }
}

// MARK: - Value

/// A type-erased container that can hold any supported USD value.
///
/// `Value` provides runtime polymorphism for USD attribute values. It can hold
/// scalar types (bool, int, float, double, string), vector types (Vec3f, Vec3d),
/// and array types.
///
/// Mirrors `pxr::VtValue` from the USD C++ API.
///
/// ## Example Usage
///
/// ```swift
/// // Create values of different types
/// let intValue = try Value(42)
/// let floatValue = try Value(3.14 as Float)
/// let stringValue = try Value("hello")
///
/// // Check type and extract value
/// if intValue.isHolding(.int) {
///     let extracted = intValue.getInt()
///     print("Value is: \(extracted)")
/// }
/// ```
public final class Value: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: VtValueRef

    // MARK: - Initialization

    /// Creates an empty value.
    ///
    /// - Throws: `ValueError.creationFailed` if the value cannot be created.
    public init() throws {
        guard let ref = VtValue_Create() else {
            throw ValueError.creationFailed("Failed to create empty Value")
        }
        self.handle = ref
    }

    /// Creates a value holding a boolean.
    ///
    /// - Parameter value: The boolean value to hold.
    /// - Throws: `ValueError.creationFailed` if the value cannot be created.
    public init(_ value: Bool) throws {
        guard let ref = VtValue_CreateBool(value) else {
            throw ValueError.creationFailed("Failed to create Value from Bool")
        }
        self.handle = ref
    }

    /// Creates a value holding an integer.
    ///
    /// - Parameter value: The integer value to hold.
    /// - Throws: `ValueError.creationFailed` if the value cannot be created.
    public init(_ value: Int32) throws {
        guard let ref = VtValue_CreateInt(value) else {
            throw ValueError.creationFailed("Failed to create Value from Int")
        }
        self.handle = ref
    }

    /// Creates a value holding a 64-bit integer.
    ///
    /// - Parameter value: The 64-bit integer value to hold.
    /// - Throws: `ValueError.creationFailed` if the value cannot be created.
    public init(_ value: Int64) throws {
        guard let ref = VtValue_CreateInt64(value) else {
            throw ValueError.creationFailed("Failed to create Value from Int64")
        }
        self.handle = ref
    }

    /// Creates a value holding an unsigned integer.
    ///
    /// - Parameter value: The unsigned integer value to hold.
    /// - Throws: `ValueError.creationFailed` if the value cannot be created.
    public init(_ value: UInt32) throws {
        guard let ref = VtValue_CreateUInt(value) else {
            throw ValueError.creationFailed("Failed to create Value from UInt")
        }
        self.handle = ref
    }

    /// Creates a value holding a 64-bit unsigned integer.
    ///
    /// - Parameter value: The 64-bit unsigned integer value to hold.
    /// - Throws: `ValueError.creationFailed` if the value cannot be created.
    public init(_ value: UInt64) throws {
        guard let ref = VtValue_CreateUInt64(value) else {
            throw ValueError.creationFailed("Failed to create Value from UInt64")
        }
        self.handle = ref
    }

    /// Creates a value holding a float.
    ///
    /// - Parameter value: The float value to hold.
    /// - Throws: `ValueError.creationFailed` if the value cannot be created.
    public init(_ value: Float) throws {
        guard let ref = VtValue_CreateFloat(value) else {
            throw ValueError.creationFailed("Failed to create Value from Float")
        }
        self.handle = ref
    }

    /// Creates a value holding a double.
    ///
    /// - Parameter value: The double value to hold.
    /// - Throws: `ValueError.creationFailed` if the value cannot be created.
    public init(_ value: Double) throws {
        guard let ref = VtValue_CreateDouble(value) else {
            throw ValueError.creationFailed("Failed to create Value from Double")
        }
        self.handle = ref
    }

    /// Creates a value holding a string.
    ///
    /// - Parameter value: The string value to hold.
    /// - Throws: `ValueError.creationFailed` if the value cannot be created.
    public init(_ value: String) throws {
        guard let ref = VtValue_CreateString(value) else {
            throw ValueError.creationFailed("Failed to create Value from String")
        }
        self.handle = ref
    }

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: VtValueRef) {
        self.handle = handle
    }

    deinit {
        VtValue_Release(handle)
    }

    // MARK: - Type Inspection

    /// Returns `true` if this value is empty (holds no value).
    public var isEmpty: Bool {
        VtValue_IsEmpty(handle)
    }

    /// The type of the held value.
    public var type: ValueType {
        ValueType(VtValue_GetType(handle))
    }

    /// The type name as a human-readable string.
    public var typeName: String {
        String(cString: VtValue_GetTypeName(handle))
    }

    /// Returns `true` if the held value is an array type.
    public var isArrayValued: Bool {
        VtValue_IsArrayValued(handle)
    }

    /// The size of the array if holding an array type, 0 otherwise.
    public var arraySize: Int {
        Int(VtValue_GetArraySize(handle))
    }

    /// Checks if this value holds the specified type.
    ///
    /// - Parameter type: The type to check for.
    /// - Returns: `true` if the value holds the specified type.
    public func isHolding(_ type: ValueType) -> Bool {
        VtValue_IsHolding(handle, type.cType)
    }

    // MARK: - Getters

    /// Gets the boolean value.
    ///
    /// - Returns: The boolean value, or `false` if not holding a bool.
    public func getBool() -> Bool {
        VtValue_GetBool(handle)
    }

    /// Gets the integer value.
    ///
    /// - Returns: The integer value, or 0 if not holding an int.
    public func getInt() -> Int32 {
        VtValue_GetInt(handle)
    }

    /// Gets the 64-bit integer value.
    ///
    /// - Returns: The 64-bit integer value, or 0 if not holding an int64.
    public func getInt64() -> Int64 {
        VtValue_GetInt64(handle)
    }

    /// Gets the unsigned integer value.
    ///
    /// - Returns: The unsigned integer value, or 0 if not holding a uint.
    public func getUInt() -> UInt32 {
        VtValue_GetUInt(handle)
    }

    /// Gets the 64-bit unsigned integer value.
    ///
    /// - Returns: The 64-bit unsigned integer value, or 0 if not holding a uint64.
    public func getUInt64() -> UInt64 {
        VtValue_GetUInt64(handle)
    }

    /// Gets the float value.
    ///
    /// - Returns: The float value, or 0.0 if not holding a float.
    public func getFloat() -> Float {
        VtValue_GetFloat(handle)
    }

    /// Gets the double value.
    ///
    /// - Returns: The double value, or 0.0 if not holding a double.
    public func getDouble() -> Double {
        VtValue_GetDouble(handle)
    }

    /// Gets the string value.
    ///
    /// - Returns: The string value, or an empty string if not holding a string.
    public func getString() -> String {
        guard let cStr = VtValue_GetString(handle) else {
            return ""
        }
        defer { VtValue_FreeString(cStr) }
        return String(cString: cStr)
    }

    // MARK: - Setters

    /// Sets the value to a boolean.
    ///
    /// - Parameter value: The boolean value to set.
    public func set(_ value: Bool) {
        VtValue_SetBool(handle, value)
    }

    /// Sets the value to an integer.
    ///
    /// - Parameter value: The integer value to set.
    public func set(_ value: Int32) {
        VtValue_SetInt(handle, value)
    }

    /// Sets the value to a 64-bit integer.
    ///
    /// - Parameter value: The 64-bit integer value to set.
    public func set(_ value: Int64) {
        VtValue_SetInt64(handle, value)
    }

    /// Sets the value to a float.
    ///
    /// - Parameter value: The float value to set.
    public func set(_ value: Float) {
        VtValue_SetFloat(handle, value)
    }

    /// Sets the value to a double.
    ///
    /// - Parameter value: The double value to set.
    public func set(_ value: Double) {
        VtValue_SetDouble(handle, value)
    }

    /// Sets the value to a string.
    ///
    /// - Parameter value: The string value to set.
    public func set(_ value: String) {
        VtValue_SetString(handle, value)
    }

    /// Clears the value (makes it empty).
    public func clear() {
        VtValue_Clear(handle)
    }

    // MARK: - Casting

    /// Checks if this value can be cast to the specified type.
    ///
    /// - Parameter type: The target type to cast to.
    /// - Returns: `true` if the cast is possible.
    public func canCast(to type: ValueType) -> Bool {
        VtValue_CanCast(handle, type.cType)
    }

    /// Casts the value to the specified type in place.
    ///
    /// - Parameter type: The target type to cast to.
    /// - Throws: `ValueError.castFailed` if the cast is not possible.
    public func cast(to type: ValueType) throws {
        let result = VtValue_Cast(handle, type.cType)
        if result != USD_RESULT_SUCCESS {
            throw ValueError.castFailed("Cannot cast from \(self.type) to \(type)")
        }
    }

    // MARK: - Copying

    /// Creates an independent copy of this value.
    ///
    /// - Returns: A new value with the same contents.
    /// - Throws: `ValueError.creationFailed` if the copy cannot be created.
    public func copy() throws -> Value {
        guard let ref = VtValue_Copy(handle) else {
            throw ValueError.creationFailed("Failed to copy Value")
        }
        return Value(handle: ref)
    }
}

// MARK: - Equatable

extension Value: Equatable {

    public static func == (lhs: Value, rhs: Value) -> Bool {
        VtValue_Equal(lhs.handle, rhs.handle)
    }
}

// MARK: - Hashable

extension Value: Hashable {

    public func hash(into hasher: inout Hasher) {
        if VtValue_CanHash(handle) {
            hasher.combine(VtValue_Hash(handle))
        } else {
            // Fall back to object identity if not hashable
            hasher.combine(ObjectIdentifier(self))
        }
    }
}

// MARK: - CustomStringConvertible

extension Value: CustomStringConvertible {

    public var description: String {
        if isEmpty {
            return "Value(empty)"
        }
        switch type {
        case .bool:
            return "Value(\(getBool()))"
        case .int:
            return "Value(\(getInt()))"
        case .int64:
            return "Value(\(getInt64()))"
        case .float:
            return "Value(\(getFloat()))"
        case .double:
            return "Value(\(getDouble()))"
        case .string:
            return "Value(\"\(getString())\")"
        default:
            return "Value(\(typeName))"
        }
    }
}

// MARK: - CustomDebugStringConvertible

extension Value: CustomDebugStringConvertible {

    public var debugDescription: String {
        "Value(type: \(typeName), isEmpty: \(isEmpty))"
    }
}

// MARK: - ValueError

/// Errors that can occur when working with values.
public enum ValueError: Error, CustomStringConvertible {

    /// Value creation failed.
    case creationFailed(String)

    /// Type cast failed.
    case castFailed(String)

    /// Type mismatch error.
    case typeMismatch(String)

    public var description: String {
        switch self {
        case .creationFailed(let message):
            return "Value creation failed: \(message)"
        case .castFailed(let message):
            return "Value cast failed: \(message)"
        case .typeMismatch(let message):
            return "Type mismatch: \(message)"
        }
    }
}
