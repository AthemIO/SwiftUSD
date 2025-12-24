// Output.swift - UsdShadeOutput wrapper
// Mirrors: pxr/usd/usdShade/output.h

import OpenUSDInterop

// MARK: - Output

/// A shader output (computed result).
///
/// Outputs are named, typed attributes on shaders that represent
/// computed results. They can be connected to inputs on other shaders
/// or to material terminal outputs.
///
/// Mirrors `pxr::UsdShadeOutput` from the USD C++ API.
public final class Output: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdShadeOutputRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdShadeOutputRef) {
        self.handle = handle
    }

    deinit {
        UsdShadeOutput_Release(handle)
    }

    // MARK: - Static Factory Methods

    /// Creates an output from an attribute.
    ///
    /// - Parameter attribute: The attribute to wrap as an output.
    /// - Returns: The output, or `nil` if the attribute is not a valid output.
    public static func fromAttribute(_ attribute: Attribute) -> Output? {
        guard let ref = UsdShadeOutput_FromAttribute(attribute.handle) else {
            return nil
        }
        return Output(handle: ref)
    }

    // MARK: - Properties

    /// Whether the output is valid.
    public var isValid: Bool {
        UsdShadeOutput_IsValid(handle)
    }

    /// Whether the output is defined.
    public var isDefined: Bool {
        UsdShadeOutput_IsDefined(handle)
    }

    /// The full name (with "outputs:" prefix).
    public var fullName: Token? {
        guard let ref = UsdShadeOutput_GetFullName(handle) else {
            return nil
        }
        return Token(handle: ref)
    }

    /// The base name (without "outputs:" prefix).
    public var baseName: Token? {
        guard let ref = UsdShadeOutput_GetBaseName(handle) else {
            return nil
        }
        return Token(handle: ref)
    }

    /// The type name of the output.
    public var typeName: Token? {
        guard let ref = UsdShadeOutput_GetTypeName(handle) else {
            return nil
        }
        return Token(handle: ref)
    }

    /// The owning prim.
    public var prim: Prim? {
        guard let ref = UsdShadeOutput_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    /// The underlying attribute.
    public var attribute: Attribute? {
        guard let ref = UsdShadeOutput_GetAttr(handle) else {
            return nil
        }
        return Attribute(handle: ref)
    }

    // MARK: - Render Type

    /// The render type.
    public var renderType: Token? {
        guard let ref = UsdShadeOutput_GetRenderType(handle) else {
            return nil
        }
        return Token(handle: ref)
    }

    /// Whether the output has a render type.
    public var hasRenderType: Bool {
        UsdShadeOutput_HasRenderType(handle)
    }

    /// Sets the render type.
    ///
    /// - Parameter renderType: The render type to set.
    /// - Throws: `ShadeError.operationFailed` if the operation fails.
    public func setRenderType(_ renderType: Token) throws {
        let result = UsdShadeOutput_SetRenderType(handle, renderType.handle)
        guard result == USD_RESULT_SUCCESS else {
            throw ShadeError.operationFailed("Failed to set render type")
        }
    }

    // MARK: - Connections (for NodeGraphs)

    /// Whether this output can connect to the given source output.
    ///
    /// - Parameter source: The output to check.
    /// - Returns: `true` if connection is allowed.
    public func canConnect(to source: Output) -> Bool {
        UsdShadeOutput_CanConnect(handle, source.handle)
    }

    /// Connects this output to a source output (for NodeGraphs).
    ///
    /// - Parameters:
    ///   - source: The output to connect to.
    ///   - modification: How to modify existing connections.
    /// - Throws: `ShadeError.connectionFailed` if the connection fails.
    public func connect(to source: Output, modification: ConnectionModification = .replace) throws {
        let result = UsdShadeOutput_ConnectToOutput(handle, source.handle, modification.cValue)
        guard result == USD_RESULT_SUCCESS else {
            throw ShadeError.connectionFailed("Failed to connect output to output")
        }
    }

    /// Connects this output to a source input (for NodeGraphs).
    ///
    /// - Parameters:
    ///   - source: The input to connect to.
    ///   - modification: How to modify existing connections.
    /// - Throws: `ShadeError.connectionFailed` if the connection fails.
    public func connect(to source: Input, modification: ConnectionModification = .replace) throws {
        let result = UsdShadeOutput_ConnectToInput(handle, source.handle, modification.cValue)
        guard result == USD_RESULT_SUCCESS else {
            throw ShadeError.connectionFailed("Failed to connect output to input")
        }
    }

    /// Connects this output to a source by path (for NodeGraphs).
    ///
    /// - Parameters:
    ///   - sourcePath: The path to connect to.
    ///   - modification: How to modify existing connections.
    /// - Throws: `ShadeError.connectionFailed` if the connection fails.
    public func connect(to sourcePath: Path, modification: ConnectionModification = .replace) throws {
        let result = UsdShadeOutput_ConnectToPath(handle, sourcePath.handle, modification.cValue)
        guard result == USD_RESULT_SUCCESS else {
            throw ShadeError.connectionFailed("Failed to connect output to path")
        }
    }

    /// Whether the output has a connected source.
    public var hasConnectedSource: Bool {
        UsdShadeOutput_HasConnectedSource(handle)
    }

    /// Whether the connection is from a base material.
    public var isSourceConnectionFromBaseMaterial: Bool {
        UsdShadeOutput_IsSourceConnectionFromBaseMaterial(handle)
    }

    /// Gets the connected source paths.
    ///
    /// - Returns: Array of paths to connected sources.
    public var connectedSources: [Path] {
        let count = UsdShadeOutput_GetConnectedSourceCount(handle)
        guard count > 0 else { return [] }

        var handles = [SdfPathRef?](repeating: nil, count: count)
        let actualCount = handles.withUnsafeMutableBufferPointer { buffer in
            UsdShadeOutput_GetConnectedSources(handle, buffer.baseAddress, count)
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
        let result = UsdShadeOutput_DisconnectSource(handle, sourcePath?.handle)
        guard result == USD_RESULT_SUCCESS else {
            throw ShadeError.operationFailed("Failed to disconnect source")
        }
    }

    /// Clears all connections.
    ///
    /// - Throws: `ShadeError.operationFailed` if the operation fails.
    public func clearSources() throws {
        let result = UsdShadeOutput_ClearSources(handle)
        guard result == USD_RESULT_SUCCESS else {
            throw ShadeError.operationFailed("Failed to clear sources")
        }
    }
}

// MARK: - CustomStringConvertible

extension Output: CustomStringConvertible {

    public var description: String {
        fullName?.text ?? "<invalid output>"
    }
}
