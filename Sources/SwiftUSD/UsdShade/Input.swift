// Input.swift - UsdShadeInput wrapper
// Mirrors: pxr/usd/usdShade/input.h

import OpenUSDInterop

// MARK: - ConnectionModification

/// How to modify connections when connecting.
public enum ConnectionModification: Sendable {
    /// Replace all existing connections.
    case replace
    /// Prepend to existing connections.
    case prepend
    /// Append to existing connections.
    case append

    internal var cValue: UsdShadeConnectionModification {
        switch self {
        case .replace: return USDSHADE_CONNECTION_REPLACE
        case .prepend: return USDSHADE_CONNECTION_PREPEND
        case .append: return USDSHADE_CONNECTION_APPEND
        }
    }
}

// MARK: - Input

/// A shader input (parameter).
///
/// Inputs are named, typed attributes on shaders that can receive
/// values or be connected to outputs from other shaders.
///
/// Mirrors `pxr::UsdShadeInput` from the USD C++ API.
public final class Input: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdShadeInputRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdShadeInputRef) {
        self.handle = handle
    }

    deinit {
        UsdShadeInput_Release(handle)
    }

    // MARK: - Static Factory Methods

    /// Creates an input from an attribute.
    ///
    /// - Parameter attribute: The attribute to wrap as an input.
    /// - Returns: The input, or `nil` if the attribute is not a valid input.
    public static func fromAttribute(_ attribute: Attribute) -> Input? {
        guard let ref = UsdShadeInput_FromAttribute(attribute.handle) else {
            return nil
        }
        return Input(handle: ref)
    }

    // MARK: - Properties

    /// Whether the input is valid.
    public var isValid: Bool {
        UsdShadeInput_IsValid(handle)
    }

    /// Whether the input is defined.
    public var isDefined: Bool {
        UsdShadeInput_IsDefined(handle)
    }

    /// The full name (with "inputs:" prefix).
    public var fullName: Token? {
        guard let ref = UsdShadeInput_GetFullName(handle) else {
            return nil
        }
        return Token(handle: ref)
    }

    /// The base name (without "inputs:" prefix).
    public var baseName: Token? {
        guard let ref = UsdShadeInput_GetBaseName(handle) else {
            return nil
        }
        return Token(handle: ref)
    }

    /// The type name of the input.
    public var typeName: Token? {
        guard let ref = UsdShadeInput_GetTypeName(handle) else {
            return nil
        }
        return Token(handle: ref)
    }

    /// The owning prim.
    public var prim: Prim? {
        guard let ref = UsdShadeInput_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    /// The underlying attribute.
    public var attribute: Attribute? {
        guard let ref = UsdShadeInput_GetAttr(handle) else {
            return nil
        }
        return Attribute(handle: ref)
    }

    // MARK: - Value Access

    /// Gets a boolean value.
    ///
    /// - Parameter time: The time code to query at.
    /// - Returns: The boolean value, or `nil` if not available.
    public func getBool(at time: TimeCode = .default) -> Bool? {
        var value: Bool = false
        let result = UsdShadeInput_GetBool(handle, time.cTimeCode, &value)
        guard result == USD_RESULT_SUCCESS else { return nil }
        return value
    }

    /// Sets a boolean value.
    ///
    /// - Parameters:
    ///   - value: The boolean value to set.
    ///   - time: The time code to set at.
    /// - Throws: `ShadeError.operationFailed` if the operation fails.
    public func set(_ value: Bool, at time: TimeCode = .default) throws {
        let result = UsdShadeInput_SetBool(handle, time.cTimeCode, value)
        guard result == USD_RESULT_SUCCESS else {
            throw ShadeError.operationFailed("Failed to set bool value")
        }
    }

    /// Gets an integer value.
    ///
    /// - Parameter time: The time code to query at.
    /// - Returns: The integer value, or `nil` if not available.
    public func getInt(at time: TimeCode = .default) -> Int32? {
        var value: Int32 = 0
        let result = UsdShadeInput_GetInt(handle, time.cTimeCode, &value)
        guard result == USD_RESULT_SUCCESS else { return nil }
        return value
    }

    /// Sets an integer value.
    ///
    /// - Parameters:
    ///   - value: The integer value to set.
    ///   - time: The time code to set at.
    /// - Throws: `ShadeError.operationFailed` if the operation fails.
    public func set(_ value: Int32, at time: TimeCode = .default) throws {
        let result = UsdShadeInput_SetInt(handle, time.cTimeCode, value)
        guard result == USD_RESULT_SUCCESS else {
            throw ShadeError.operationFailed("Failed to set int value")
        }
    }

    /// Gets a float value.
    ///
    /// - Parameter time: The time code to query at.
    /// - Returns: The float value, or `nil` if not available.
    public func getFloat(at time: TimeCode = .default) -> Float? {
        var value: Float = 0
        let result = UsdShadeInput_GetFloat(handle, time.cTimeCode, &value)
        guard result == USD_RESULT_SUCCESS else { return nil }
        return value
    }

    /// Sets a float value.
    ///
    /// - Parameters:
    ///   - value: The float value to set.
    ///   - time: The time code to set at.
    /// - Throws: `ShadeError.operationFailed` if the operation fails.
    public func set(_ value: Float, at time: TimeCode = .default) throws {
        let result = UsdShadeInput_SetFloat(handle, time.cTimeCode, value)
        guard result == USD_RESULT_SUCCESS else {
            throw ShadeError.operationFailed("Failed to set float value")
        }
    }

    /// Gets a Vec3f value (color, normal, etc.).
    ///
    /// - Parameter time: The time code to query at.
    /// - Returns: The vector value as (x, y, z), or `nil` if not available.
    public func getVec3f(at time: TimeCode = .default) -> (x: Float, y: Float, z: Float)? {
        var x: Float = 0, y: Float = 0, z: Float = 0
        let result = UsdShadeInput_GetVec3f(handle, time.cTimeCode, &x, &y, &z)
        guard result == USD_RESULT_SUCCESS else { return nil }
        return (x, y, z)
    }

    /// Sets a Vec3f value (color, normal, etc.).
    ///
    /// - Parameters:
    ///   - x: The x component.
    ///   - y: The y component.
    ///   - z: The z component.
    ///   - time: The time code to set at.
    /// - Throws: `ShadeError.operationFailed` if the operation fails.
    public func setVec3f(x: Float, y: Float, z: Float, at time: TimeCode = .default) throws {
        let result = UsdShadeInput_SetVec3f(handle, time.cTimeCode, x, y, z)
        guard result == USD_RESULT_SUCCESS else {
            throw ShadeError.operationFailed("Failed to set Vec3f value")
        }
    }

    /// Gets a string value.
    ///
    /// - Parameter time: The time code to query at.
    /// - Returns: The string value, or `nil` if not available.
    public func getString(at time: TimeCode = .default) -> String? {
        var cstr: UnsafeMutablePointer<CChar>?
        let result = UsdShadeInput_GetString(handle, time.cTimeCode, &cstr)
        guard result == USD_RESULT_SUCCESS, let str = cstr else { return nil }
        defer { UsdShadeInput_FreeString(str) }
        return String(cString: str)
    }

    /// Sets a string value.
    ///
    /// - Parameters:
    ///   - value: The string value to set.
    ///   - time: The time code to set at.
    /// - Throws: `ShadeError.operationFailed` if the operation fails.
    public func set(_ value: String, at time: TimeCode = .default) throws {
        let result = UsdShadeInput_SetString(handle, time.cTimeCode, value)
        guard result == USD_RESULT_SUCCESS else {
            throw ShadeError.operationFailed("Failed to set string value")
        }
    }

    /// Gets a token value.
    ///
    /// - Parameter time: The time code to query at.
    /// - Returns: The token value, or `nil` if not available.
    public func getToken(at time: TimeCode = .default) -> Token? {
        var tokenRef: TfTokenRef?
        let result = UsdShadeInput_GetToken(handle, time.cTimeCode, &tokenRef)
        guard result == USD_RESULT_SUCCESS, let ref = tokenRef else { return nil }
        return Token(handle: ref)
    }

    /// Sets a token value.
    ///
    /// - Parameters:
    ///   - value: The token value to set.
    ///   - time: The time code to set at.
    /// - Throws: `ShadeError.operationFailed` if the operation fails.
    public func set(_ value: Token, at time: TimeCode = .default) throws {
        let result = UsdShadeInput_SetToken(handle, time.cTimeCode, value.handle)
        guard result == USD_RESULT_SUCCESS else {
            throw ShadeError.operationFailed("Failed to set token value")
        }
    }

    /// Gets an asset path value.
    ///
    /// - Parameter time: The time code to query at.
    /// - Returns: The asset path value, or `nil` if not available.
    public func getAssetPath(at time: TimeCode = .default) -> AssetPath? {
        var pathRef: SdfAssetPathRef?
        let result = UsdShadeInput_GetAssetPath(handle, time.cTimeCode, &pathRef)
        guard result == USD_RESULT_SUCCESS, let ref = pathRef else { return nil }
        return AssetPath(handle: ref)
    }

    /// Sets an asset path value.
    ///
    /// - Parameters:
    ///   - value: The asset path value to set.
    ///   - time: The time code to set at.
    /// - Throws: `ShadeError.operationFailed` if the operation fails.
    public func set(_ value: AssetPath, at time: TimeCode = .default) throws {
        let result = UsdShadeInput_SetAssetPath(handle, time.cTimeCode, value.handle)
        guard result == USD_RESULT_SUCCESS else {
            throw ShadeError.operationFailed("Failed to set asset path value")
        }
    }

    // MARK: - Render Type

    /// The render type.
    public var renderType: Token? {
        guard let ref = UsdShadeInput_GetRenderType(handle) else {
            return nil
        }
        return Token(handle: ref)
    }

    /// Whether the input has a render type.
    public var hasRenderType: Bool {
        UsdShadeInput_HasRenderType(handle)
    }

    /// Sets the render type.
    ///
    /// - Parameter renderType: The render type to set.
    /// - Throws: `ShadeError.operationFailed` if the operation fails.
    public func setRenderType(_ renderType: Token) throws {
        let result = UsdShadeInput_SetRenderType(handle, renderType.handle)
        guard result == USD_RESULT_SUCCESS else {
            throw ShadeError.operationFailed("Failed to set render type")
        }
    }

    // MARK: - Connectability

    /// The connectability ("full" or "interfaceOnly").
    public var connectability: Token? {
        guard let ref = UsdShadeInput_GetConnectability(handle) else {
            return nil
        }
        return Token(handle: ref)
    }

    /// Sets the connectability.
    ///
    /// - Parameter connectability: The connectability to set ("full" or "interfaceOnly").
    /// - Throws: `ShadeError.operationFailed` if the operation fails.
    public func setConnectability(_ connectability: Token) throws {
        let result = UsdShadeInput_SetConnectability(handle, connectability.handle)
        guard result == USD_RESULT_SUCCESS else {
            throw ShadeError.operationFailed("Failed to set connectability")
        }
    }

    /// Clears the connectability.
    ///
    /// - Throws: `ShadeError.operationFailed` if the operation fails.
    public func clearConnectability() throws {
        let result = UsdShadeInput_ClearConnectability(handle)
        guard result == USD_RESULT_SUCCESS else {
            throw ShadeError.operationFailed("Failed to clear connectability")
        }
    }

    // MARK: - Connections

    /// Whether this input can connect to the given source output.
    ///
    /// - Parameter source: The output to check.
    /// - Returns: `true` if connection is allowed.
    public func canConnect(to source: Output) -> Bool {
        UsdShadeInput_CanConnect(handle, source.handle)
    }

    /// Connects this input to a source output.
    ///
    /// - Parameters:
    ///   - source: The output to connect to.
    ///   - modification: How to modify existing connections.
    /// - Throws: `ShadeError.connectionFailed` if the connection fails.
    public func connect(to source: Output, modification: ConnectionModification = .replace) throws {
        let result = UsdShadeInput_ConnectToOutput(handle, source.handle, modification.cValue)
        guard result == USD_RESULT_SUCCESS else {
            throw ShadeError.connectionFailed("Failed to connect input to output")
        }
    }

    /// Connects this input to a source input.
    ///
    /// - Parameters:
    ///   - source: The input to connect to.
    ///   - modification: How to modify existing connections.
    /// - Throws: `ShadeError.connectionFailed` if the connection fails.
    public func connect(to source: Input, modification: ConnectionModification = .replace) throws {
        let result = UsdShadeInput_ConnectToInput(handle, source.handle, modification.cValue)
        guard result == USD_RESULT_SUCCESS else {
            throw ShadeError.connectionFailed("Failed to connect input to input")
        }
    }

    /// Connects this input to a source by path.
    ///
    /// - Parameters:
    ///   - sourcePath: The path to connect to.
    ///   - modification: How to modify existing connections.
    /// - Throws: `ShadeError.connectionFailed` if the connection fails.
    public func connect(to sourcePath: Path, modification: ConnectionModification = .replace) throws {
        let result = UsdShadeInput_ConnectToPath(handle, sourcePath.handle, modification.cValue)
        guard result == USD_RESULT_SUCCESS else {
            throw ShadeError.connectionFailed("Failed to connect input to path")
        }
    }

    /// Whether the input has a connected source.
    public var hasConnectedSource: Bool {
        UsdShadeInput_HasConnectedSource(handle)
    }

    /// Whether the connection is from a base material.
    public var isSourceConnectionFromBaseMaterial: Bool {
        UsdShadeInput_IsSourceConnectionFromBaseMaterial(handle)
    }

    /// Gets the connected source paths.
    ///
    /// - Returns: Array of paths to connected sources.
    public var connectedSources: [Path] {
        let count = UsdShadeInput_GetConnectedSourceCount(handle)
        guard count > 0 else { return [] }

        var handles = [SdfPathRef?](repeating: nil, count: count)
        let actualCount = handles.withUnsafeMutableBufferPointer { buffer in
            UsdShadeInput_GetConnectedSources(handle, buffer.baseAddress, count)
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

    /// Disconnects a specific source.
    ///
    /// - Parameter sourcePath: The path to disconnect, or `nil` to disconnect all.
    /// - Throws: `ShadeError.operationFailed` if the operation fails.
    public func disconnectSource(_ sourcePath: Path? = nil) throws {
        let result = UsdShadeInput_DisconnectSource(handle, sourcePath?.handle)
        guard result == USD_RESULT_SUCCESS else {
            throw ShadeError.operationFailed("Failed to disconnect source")
        }
    }

    /// Clears all connections.
    ///
    /// - Throws: `ShadeError.operationFailed` if the operation fails.
    public func clearSources() throws {
        let result = UsdShadeInput_ClearSources(handle)
        guard result == USD_RESULT_SUCCESS else {
            throw ShadeError.operationFailed("Failed to clear sources")
        }
    }

    // MARK: - Documentation

    /// The documentation string.
    public var documentation: String? {
        guard let cstr = UsdShadeInput_GetDocumentation(handle) else {
            return nil
        }
        defer { UsdShadeInput_FreeString(cstr) }
        return String(cString: cstr)
    }

    /// Sets the documentation string.
    ///
    /// - Parameter docs: The documentation string.
    /// - Throws: `ShadeError.operationFailed` if the operation fails.
    public func setDocumentation(_ docs: String) throws {
        let result = UsdShadeInput_SetDocumentation(handle, docs)
        guard result == USD_RESULT_SUCCESS else {
            throw ShadeError.operationFailed("Failed to set documentation")
        }
    }

    /// The display group.
    public var displayGroup: String? {
        guard let cstr = UsdShadeInput_GetDisplayGroup(handle) else {
            return nil
        }
        defer { UsdShadeInput_FreeString(cstr) }
        return String(cString: cstr)
    }

    /// Sets the display group.
    ///
    /// - Parameter displayGroup: The display group string.
    /// - Throws: `ShadeError.operationFailed` if the operation fails.
    public func setDisplayGroup(_ displayGroup: String) throws {
        let result = UsdShadeInput_SetDisplayGroup(handle, displayGroup)
        guard result == USD_RESULT_SUCCESS else {
            throw ShadeError.operationFailed("Failed to set display group")
        }
    }
}

// MARK: - CustomStringConvertible

extension Input: CustomStringConvertible {

    public var description: String {
        fullName?.text ?? "<invalid input>"
    }
}
