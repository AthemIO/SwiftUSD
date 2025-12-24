// Shader.swift - UsdShadeShader wrapper
// Mirrors: pxr/usd/usdShade/shader.h

import OpenUSDInterop

// MARK: - Shader

/// A shader node in a shading network.
///
/// Shaders are the building blocks of materials. They have inputs
/// (parameters) and outputs (computed results) that can be connected
/// to form a shader network.
///
/// Mirrors `pxr::UsdShadeShader` from the USD C++ API.
public final class Shader: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdShadeShaderRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdShadeShaderRef) {
        self.handle = handle
    }

    deinit {
        UsdShadeShader_Release(handle)
    }

    // MARK: - Static Factory Methods

    /// Gets a Shader schema from a prim at the given path.
    ///
    /// - Parameters:
    ///   - stage: The stage containing the prim.
    ///   - path: The path to the shader prim.
    /// - Returns: The shader, or `nil` if not found or invalid.
    public static func get(stage: Stage, path: Path) -> Shader? {
        guard let ref = UsdShadeShader_Get(stage.handle, path.handle) else {
            return nil
        }
        return Shader(handle: ref)
    }

    /// Defines a new Shader prim at the given path.
    ///
    /// - Parameters:
    ///   - stage: The stage to create the shader on.
    ///   - path: The path for the new shader.
    /// - Returns: The newly created shader.
    /// - Throws: `ShadeError.creationFailed` if the shader cannot be created.
    public static func define(stage: Stage, path: Path) throws -> Shader {
        guard let ref = UsdShadeShader_Define(stage.handle, path.handle) else {
            throw ShadeError.creationFailed("Failed to define shader at '\(path)'")
        }
        return Shader(handle: ref)
    }

    /// Creates a Shader from an existing prim.
    ///
    /// - Parameter prim: The prim to wrap as a shader.
    /// - Returns: The shader, or `nil` if the prim is not a valid shader.
    public static func fromPrim(_ prim: Prim) -> Shader? {
        guard let ref = UsdShadeShader_FromPrim(prim.handle) else {
            return nil
        }
        return Shader(handle: ref)
    }

    // MARK: - Properties

    /// Whether the shader is valid.
    public var isValid: Bool {
        UsdShadeShader_IsValid(handle)
    }

    /// The underlying prim.
    public var prim: Prim? {
        guard let ref = UsdShadeShader_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    /// The path of the shader.
    public var path: Path? {
        guard let ref = UsdShadeShader_GetPath(handle) else {
            return nil
        }
        return Path(handle: ref)
    }

    // MARK: - Shader ID

    /// The shader ID (e.g., "UsdPreviewSurface").
    public var shaderId: Token? {
        get {
            guard let ref = UsdShadeShader_GetShaderId(handle) else {
                return nil
            }
            return Token(handle: ref)
        }
        set {
            if let token = newValue {
                _ = UsdShadeShader_SetShaderId(handle, token.handle)
            }
        }
    }

    /// Sets the shader ID.
    ///
    /// - Parameter id: The shader ID token.
    /// - Throws: `ShadeError.operationFailed` if the operation fails.
    public func setShaderId(_ id: Token) throws {
        let result = UsdShadeShader_SetShaderId(handle, id.handle)
        guard result == USD_RESULT_SUCCESS else {
            throw ShadeError.operationFailed("Failed to set shader ID")
        }
    }

    /// The ID attribute.
    public var idAttr: Attribute? {
        guard let ref = UsdShadeShader_GetIdAttr(handle) else {
            return nil
        }
        return Attribute(handle: ref)
    }

    /// Creates the ID attribute.
    ///
    /// - Parameter defaultValue: Optional default value for the attribute.
    /// - Returns: The created attribute.
    public func createIdAttr(defaultValue: Token? = nil) -> Attribute? {
        guard let ref = UsdShadeShader_CreateIdAttr(handle, defaultValue?.handle) else {
            return nil
        }
        return Attribute(handle: ref)
    }

    // MARK: - Source Asset

    /// Gets the source asset path for the given source type.
    ///
    /// - Parameter sourceType: The source type (e.g., "glslfx"), or `nil` for any.
    /// - Returns: The asset path, or `nil` if not found.
    public func sourceAsset(for sourceType: Token? = nil) -> AssetPath? {
        var pathRef: SdfAssetPathRef?
        let result = UsdShadeShader_GetSourceAsset(handle, &pathRef, sourceType?.handle)
        guard result == USD_RESULT_SUCCESS, let ref = pathRef else {
            return nil
        }
        return AssetPath(handle: ref)
    }

    /// Sets the source asset path.
    ///
    /// - Parameters:
    ///   - assetPath: The asset path to set.
    ///   - sourceType: The source type (e.g., "glslfx"), or `nil` for default.
    /// - Throws: `ShadeError.operationFailed` if the operation fails.
    public func setSourceAsset(_ assetPath: AssetPath, for sourceType: Token? = nil) throws {
        let result = UsdShadeShader_SetSourceAsset(handle, assetPath.handle, sourceType?.handle)
        guard result == USD_RESULT_SUCCESS else {
            throw ShadeError.operationFailed("Failed to set source asset")
        }
    }

    // MARK: - Source Code

    /// Gets the source code for the given source type.
    ///
    /// - Parameter sourceType: The source type (e.g., "glslfx"), or `nil` for any.
    /// - Returns: The source code string, or `nil` if not found.
    public func sourceCode(for sourceType: Token? = nil) -> String? {
        var codePtr: UnsafeMutablePointer<CChar>?
        let result = UsdShadeShader_GetSourceCode(handle, &codePtr, sourceType?.handle)
        guard result == USD_RESULT_SUCCESS, let code = codePtr else {
            return nil
        }
        defer { UsdShadeShader_FreeString(code) }
        return String(cString: code)
    }

    /// Sets the source code.
    ///
    /// - Parameters:
    ///   - code: The source code string.
    ///   - sourceType: The source type (e.g., "glslfx"), or `nil` for default.
    /// - Throws: `ShadeError.operationFailed` if the operation fails.
    public func setSourceCode(_ code: String, for sourceType: Token? = nil) throws {
        let result = UsdShadeShader_SetSourceCode(handle, code, sourceType?.handle)
        guard result == USD_RESULT_SUCCESS else {
            throw ShadeError.operationFailed("Failed to set source code")
        }
    }

    // MARK: - Inputs

    /// Creates an input on the shader.
    ///
    /// - Parameters:
    ///   - name: The name of the input.
    ///   - typeName: The type name of the input.
    /// - Returns: The created input.
    public func createInput(name: Token, typeName: Token) -> Input? {
        guard let ref = UsdShadeShader_CreateInput(handle, name.handle, typeName.handle) else {
            return nil
        }
        return Input(handle: ref)
    }

    /// Gets an input by name.
    ///
    /// - Parameter name: The name of the input.
    /// - Returns: The input, or `nil` if not found.
    public func input(named name: Token) -> Input? {
        guard let ref = UsdShadeShader_GetInput(handle, name.handle) else {
            return nil
        }
        return Input(handle: ref)
    }

    /// Gets all inputs.
    ///
    /// - Parameter onlyAuthored: If true, only returns authored inputs.
    /// - Returns: Array of inputs.
    public func inputs(onlyAuthored: Bool = true) -> [Input] {
        let count = UsdShadeShader_GetInputCount(handle, onlyAuthored)
        guard count > 0 else { return [] }

        var handles = [UsdShadeInputRef?](repeating: nil, count: count)
        let actualCount = handles.withUnsafeMutableBufferPointer { buffer in
            UsdShadeShader_GetInputs(handle, buffer.baseAddress, count, onlyAuthored)
        }

        var result = [Input]()
        result.reserveCapacity(actualCount)
        for i in 0..<actualCount {
            if let h = handles[i] {
                result.append(Input(handle: h))
            }
        }
        return result
    }

    // MARK: - Outputs

    /// Creates an output on the shader.
    ///
    /// - Parameters:
    ///   - name: The name of the output.
    ///   - typeName: The type name of the output.
    /// - Returns: The created output.
    public func createOutput(name: Token, typeName: Token) -> Output? {
        guard let ref = UsdShadeShader_CreateOutput(handle, name.handle, typeName.handle) else {
            return nil
        }
        return Output(handle: ref)
    }

    /// Gets an output by name.
    ///
    /// - Parameter name: The name of the output.
    /// - Returns: The output, or `nil` if not found.
    public func output(named name: Token) -> Output? {
        guard let ref = UsdShadeShader_GetOutput(handle, name.handle) else {
            return nil
        }
        return Output(handle: ref)
    }

    /// Gets all outputs.
    ///
    /// - Parameter onlyAuthored: If true, only returns authored outputs.
    /// - Returns: Array of outputs.
    public func outputs(onlyAuthored: Bool = true) -> [Output] {
        let count = UsdShadeShader_GetOutputCount(handle, onlyAuthored)
        guard count > 0 else { return [] }

        var handles = [UsdShadeOutputRef?](repeating: nil, count: count)
        let actualCount = handles.withUnsafeMutableBufferPointer { buffer in
            UsdShadeShader_GetOutputs(handle, buffer.baseAddress, count, onlyAuthored)
        }

        var result = [Output]()
        result.reserveCapacity(actualCount)
        for i in 0..<actualCount {
            if let h = handles[i] {
                result.append(Output(handle: h))
            }
        }
        return result
    }
}

// MARK: - CustomStringConvertible

extension Shader: CustomStringConvertible {

    public var description: String {
        path?.description ?? "<invalid shader>"
    }
}

// MARK: - CustomDebugStringConvertible

extension Shader: CustomDebugStringConvertible {

    public var debugDescription: String {
        "Shader(\"\(path?.description ?? "<invalid>")\")"
    }
}
