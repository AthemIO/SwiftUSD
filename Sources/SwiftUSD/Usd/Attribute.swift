// Attribute.swift - UsdAttribute wrapper
// Mirrors: pxr/usd/usd/attribute.h

import OpenUSDInterop
#if canImport(Darwin)
import Darwin
#elseif canImport(Glibc)
import Glibc
#elseif os(Windows)
import ucrt
#endif

// MARK: - Attribute

/// A typed data value on a prim.
///
/// Attributes hold typed values that can vary over time. They support
/// default values, time-sampled values, and connections to other attributes.
///
/// Mirrors `pxr::UsdAttribute` from the USD C++ API.
public final class Attribute: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdAttributeRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdAttributeRef) {
        self.handle = handle
    }

    deinit {
        UsdAttribute_Release(handle)
    }

    // MARK: - Validity

    /// Returns `true` if the attribute is valid.
    public var isValid: Bool {
        UsdAttribute_IsValid(handle)
    }

    // MARK: - Naming

    /// The attribute's name.
    public var name: Token {
        let ref = UsdAttribute_GetName(handle)!
        return Token(handle: ref)
    }

    /// The attribute's path.
    public var path: Path {
        let ref = UsdAttribute_GetPath(handle)!
        return Path(handle: ref)
    }

    // MARK: - Ownership

    /// The prim that owns this attribute.
    public var prim: Prim? {
        guard let ref = UsdAttribute_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    // MARK: - Type Information

    /// The attribute's type name.
    public var typeName: Token {
        let ref = UsdAttribute_GetTypeName(handle)!
        return Token(handle: ref)
    }

    /// The attribute's variability.
    public var variability: Variability {
        Variability(cValue: UsdAttribute_GetVariability(handle))
    }

    // MARK: - Value Status

    /// Returns `true` if the attribute has a value (default or time-sampled).
    public var hasValue: Bool {
        UsdAttribute_HasValue(handle)
    }

    /// Returns `true` if the attribute has an authored value.
    public var hasAuthoredValue: Bool {
        UsdAttribute_HasAuthoredValue(handle)
    }

    /// Returns `true` if the attribute has a fallback (schema-defined) value.
    public var hasFallbackValue: Bool {
        UsdAttribute_HasFallbackValue(handle)
    }

    /// Returns `true` if the attribute value might be time-varying.
    public var valueMightBeTimeVarying: Bool {
        UsdAttribute_ValueMightBeTimeVarying(handle)
    }

    // MARK: - Time Samples

    /// The number of time samples for this attribute.
    public var timeSampleCount: Int {
        Int(UsdAttribute_GetNumTimeSamples(handle))
    }

    /// Gets the time sample times for this attribute.
    ///
    /// - Returns: Array of time values where samples exist.
    public func timeSamples() -> [Double] {
        let count = timeSampleCount
        guard count > 0 else { return [] }

        var times = [Double](repeating: 0, count: count)
        let actualCount = times.withUnsafeMutableBufferPointer { buffer in
            UsdAttribute_GetTimeSamples(handle, buffer.baseAddress, count)
        }

        return Array(times.prefix(actualCount))
    }

    /// Gets the bracketing time samples around a desired time.
    ///
    /// - Parameter time: The time to query.
    /// - Returns: A tuple of (lowerTime, upperTime, hasTimeSamples), or `nil` on error.
    public func bracketingTimeSamples(
        around time: Double
    ) -> (lower: Double, upper: Double, hasTimeSamples: Bool)? {
        var lower: Double = 0
        var upper: Double = 0
        var hasTimeSamples: Bool = false

        let result = UsdAttribute_GetBracketingTimeSamples(
            handle, time, &lower, &upper, &hasTimeSamples
        )

        guard result == USD_RESULT_SUCCESS else {
            return nil
        }

        return (lower, upper, hasTimeSamples)
    }

    // MARK: - Value Getters (Type-specific)

    /// Gets a Bool value at the specified time.
    public func get(at time: TimeCode = .default) -> Bool? {
        var value: Bool = false
        let result = UsdAttribute_GetBool(handle, time.cTimeCode, &value)
        return result == USD_RESULT_SUCCESS ? value : nil
    }

    /// Gets an Int value at the specified time.
    public func get(at time: TimeCode = .default) -> Int? {
        var value: Int32 = 0
        let result = UsdAttribute_GetInt(handle, time.cTimeCode, &value)
        return result == USD_RESULT_SUCCESS ? Int(value) : nil
    }

    /// Gets a Float value at the specified time.
    public func get(at time: TimeCode = .default) -> Float? {
        var value: Float = 0
        let result = UsdAttribute_GetFloat(handle, time.cTimeCode, &value)
        return result == USD_RESULT_SUCCESS ? value : nil
    }

    /// Gets a Double value at the specified time.
    public func get(at time: TimeCode = .default) -> Double? {
        var value: Double = 0
        let result = UsdAttribute_GetDouble(handle, time.cTimeCode, &value)
        return result == USD_RESULT_SUCCESS ? value : nil
    }

    /// Gets a String value at the specified time.
    public func get(at time: TimeCode = .default) -> String? {
        var cstr: UnsafeMutablePointer<CChar>?
        let result = UsdAttribute_GetString(handle, time.cTimeCode, &cstr)
        guard result == USD_RESULT_SUCCESS, let str = cstr else {
            return nil
        }
        defer { UsdAttribute_FreeString(str) }
        return String(cString: str)
    }

    /// Gets a Token value at the specified time.
    public func getToken(at time: TimeCode = .default) -> Token? {
        var ref: TfTokenRef?
        let result = UsdAttribute_GetToken(handle, time.cTimeCode, &ref)
        guard result == USD_RESULT_SUCCESS, let tokenRef = ref else {
            return nil
        }
        return Token(handle: tokenRef)
    }

    /// Gets a Vec3f value at the specified time.
    public func getVec3f(at time: TimeCode = .default) -> (x: Float, y: Float, z: Float)? {
        var x: Float = 0, y: Float = 0, z: Float = 0
        let result = UsdAttribute_GetVec3f(handle, time.cTimeCode, &x, &y, &z)
        return result == USD_RESULT_SUCCESS ? (x, y, z) : nil
    }

    /// Gets a Vec3d value at the specified time.
    public func getVec3d(at time: TimeCode = .default) -> (x: Double, y: Double, z: Double)? {
        var x: Double = 0, y: Double = 0, z: Double = 0
        let result = UsdAttribute_GetVec3d(handle, time.cTimeCode, &x, &y, &z)
        return result == USD_RESULT_SUCCESS ? (x, y, z) : nil
    }

    /// Gets a Matrix4d value at the specified time.
    ///
    /// - Returns: 16 doubles in row-major order, or `nil` on failure.
    public func getMatrix4d(at time: TimeCode = .default) -> [Double]? {
        var values = [Double](repeating: 0, count: 16)
        let result = values.withUnsafeMutableBufferPointer { buffer in
            UsdAttribute_GetMatrix4d(handle, time.cTimeCode, buffer.baseAddress)
        }
        return result == USD_RESULT_SUCCESS ? values : nil
    }

    // MARK: - Value Setters (Type-specific)

    /// Sets a Bool value at the specified time.
    @discardableResult
    public func set(_ value: Bool, at time: TimeCode = .default) -> Bool {
        UsdAttribute_SetBool(handle, time.cTimeCode, value) == USD_RESULT_SUCCESS
    }

    /// Sets an Int value at the specified time.
    @discardableResult
    public func set(_ value: Int, at time: TimeCode = .default) -> Bool {
        UsdAttribute_SetInt(handle, time.cTimeCode, Int32(value)) == USD_RESULT_SUCCESS
    }

    /// Sets a Float value at the specified time.
    @discardableResult
    public func set(_ value: Float, at time: TimeCode = .default) -> Bool {
        UsdAttribute_SetFloat(handle, time.cTimeCode, value) == USD_RESULT_SUCCESS
    }

    /// Sets a Double value at the specified time.
    @discardableResult
    public func set(_ value: Double, at time: TimeCode = .default) -> Bool {
        UsdAttribute_SetDouble(handle, time.cTimeCode, value) == USD_RESULT_SUCCESS
    }

    /// Sets a String value at the specified time.
    @discardableResult
    public func set(_ value: String, at time: TimeCode = .default) -> Bool {
        UsdAttribute_SetString(handle, time.cTimeCode, value) == USD_RESULT_SUCCESS
    }

    /// Sets a Token value at the specified time.
    @discardableResult
    public func set(_ value: Token, at time: TimeCode = .default) -> Bool {
        UsdAttribute_SetToken(handle, time.cTimeCode, value.handle) == USD_RESULT_SUCCESS
    }

    /// Sets a Vec3f value at the specified time.
    @discardableResult
    public func setVec3f(x: Float, y: Float, z: Float, at time: TimeCode = .default) -> Bool {
        UsdAttribute_SetVec3f(handle, time.cTimeCode, x, y, z) == USD_RESULT_SUCCESS
    }

    /// Sets a Vec3d value at the specified time.
    @discardableResult
    public func setVec3d(x: Double, y: Double, z: Double, at time: TimeCode = .default) -> Bool {
        UsdAttribute_SetVec3d(handle, time.cTimeCode, x, y, z) == USD_RESULT_SUCCESS
    }

    /// Sets a Matrix4d value at the specified time.
    ///
    /// - Parameter values: 16 doubles in row-major order.
    @discardableResult
    public func setMatrix4d(_ values: [Double], at time: TimeCode = .default) -> Bool {
        guard values.count == 16 else { return false }
        return values.withUnsafeBufferPointer { buffer in
            UsdAttribute_SetMatrix4d(handle, time.cTimeCode, buffer.baseAddress) == USD_RESULT_SUCCESS
        }
    }

    // MARK: - Value Clearing

    /// Clears all values (default and time samples).
    @discardableResult
    public func clear() -> Bool {
        UsdAttribute_Clear(handle) == USD_RESULT_SUCCESS
    }

    /// Clears the value at a specific time.
    @discardableResult
    public func clear(at time: TimeCode) -> Bool {
        UsdAttribute_ClearAtTime(handle, time.cTimeCode) == USD_RESULT_SUCCESS
    }

    /// Clears the default value.
    @discardableResult
    public func clearDefault() -> Bool {
        UsdAttribute_ClearDefault(handle) == USD_RESULT_SUCCESS
    }

    /// Blocks the attribute value (prevents value resolution).
    @discardableResult
    public func block() -> Bool {
        UsdAttribute_Block(handle) == USD_RESULT_SUCCESS
    }

    // MARK: - Connections

    /// The number of connections.
    public var connectionCount: Int {
        Int(UsdAttribute_GetConnectionCount(handle))
    }

    /// Returns `true` if the attribute has authored connections.
    public var hasAuthoredConnections: Bool {
        UsdAttribute_HasAuthoredConnections(handle)
    }

    /// Gets the connection paths.
    ///
    /// - Returns: Array of paths to connected attributes.
    public func connections() -> [Path] {
        let count = connectionCount
        guard count > 0 else { return [] }

        var handles = [SdfPathRef?](repeating: nil, count: count)
        let actualCount = handles.withUnsafeMutableBufferPointer { buffer in
            UsdAttribute_GetConnections(handle, buffer.baseAddress, count)
        }

        var result = [Path]()
        result.reserveCapacity(actualCount)
        for i in 0..<actualCount {
            if let h = handles[i] {
                result.append(Path(handle: h))
            }
        }
        return result
    }

    /// Adds a connection to another attribute.
    ///
    /// - Parameters:
    ///   - source: The path to the source attribute.
    ///   - position: Where to add the connection in the list.
    /// - Returns: `true` on success.
    @discardableResult
    public func addConnection(
        to source: Path,
        position: ListPosition = .backOfPrependList
    ) -> Bool {
        UsdAttribute_AddConnection(handle, source.handle, position.cValue) == USD_RESULT_SUCCESS
    }

    /// Removes a connection.
    ///
    /// - Parameter source: The path to the source attribute.
    /// - Returns: `true` on success.
    @discardableResult
    public func removeConnection(to source: Path) -> Bool {
        UsdAttribute_RemoveConnection(handle, source.handle) == USD_RESULT_SUCCESS
    }

    /// Clears all connections.
    ///
    /// - Returns: `true` on success.
    @discardableResult
    public func clearConnections() -> Bool {
        UsdAttribute_ClearConnections(handle) == USD_RESULT_SUCCESS
    }

    /// Sets the connections (replaces all existing).
    ///
    /// - Parameter sources: Array of paths to source attributes.
    /// - Returns: `true` on success.
    @discardableResult
    public func setConnections(_ sources: [Path]) -> Bool {
        var handles = sources.map { $0.handle as SdfPathRef? }
        return handles.withUnsafeMutableBufferPointer { buffer in
            UsdAttribute_SetConnections(handle, buffer.baseAddress, sources.count) == USD_RESULT_SUCCESS
        }
    }
}

// MARK: - CustomStringConvertible

extension Attribute: CustomStringConvertible {

    public var description: String {
        path.description
    }
}

// MARK: - CustomDebugStringConvertible

extension Attribute: CustomDebugStringConvertible {

    public var debugDescription: String {
        "Attribute(\"\(path)\", type: \(typeName))"
    }
}

// MARK: - AttributeError

/// Errors that can occur when working with attributes.
public enum AttributeError: Error, CustomStringConvertible {

    /// Failed to get attribute value.
    case getFailed(String)

    /// Failed to set attribute value.
    case setFailed(String)

    /// Type mismatch when getting or setting value.
    case typeMismatch(String)

    public var description: String {
        switch self {
        case .getFailed(let message):
            return "Attribute get failed: \(message)"
        case .setFailed(let message):
            return "Attribute set failed: \(message)"
        case .typeMismatch(let message):
            return "Attribute type mismatch: \(message)"
        }
    }
}
