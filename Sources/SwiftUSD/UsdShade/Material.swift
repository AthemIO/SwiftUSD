// Material.swift - UsdShadeMaterial wrapper
// Mirrors: pxr/usd/usdShade/material.h

import OpenUSDInterop

// MARK: - Material

/// A material is a container for shading networks.
///
/// Materials provide a binding target for geometry and contain
/// terminal outputs for surface, displacement, and volume shaders.
///
/// Mirrors `pxr::UsdShadeMaterial` from the USD C++ API.
public final class Material: @unchecked Sendable {

    // MARK: - Properties

    /// The underlying opaque pointer to the C wrapper.
    internal let handle: UsdShadeMaterialRef

    // MARK: - Initialization

    /// Internal initializer from an existing handle.
    /// Takes ownership of the handle (does not retain).
    internal init(handle: UsdShadeMaterialRef) {
        self.handle = handle
    }

    deinit {
        UsdShadeMaterial_Release(handle)
    }

    // MARK: - Static Factory Methods

    /// Gets a Material schema from a prim at the given path.
    ///
    /// - Parameters:
    ///   - stage: The stage containing the prim.
    ///   - path: The path to the material prim.
    /// - Returns: The material, or `nil` if not found or invalid.
    public static func get(stage: Stage, path: Path) -> Material? {
        guard let ref = UsdShadeMaterial_Get(stage.handle, path.handle) else {
            return nil
        }
        return Material(handle: ref)
    }

    /// Defines a new Material prim at the given path.
    ///
    /// - Parameters:
    ///   - stage: The stage to create the material on.
    ///   - path: The path for the new material.
    /// - Returns: The newly created material.
    /// - Throws: `ShadeError.creationFailed` if the material cannot be created.
    public static func define(stage: Stage, path: Path) throws -> Material {
        guard let ref = UsdShadeMaterial_Define(stage.handle, path.handle) else {
            throw ShadeError.creationFailed("Failed to define material at '\(path)'")
        }
        return Material(handle: ref)
    }

    /// Creates a Material from an existing prim.
    ///
    /// - Parameter prim: The prim to wrap as a material.
    /// - Returns: The material, or `nil` if the prim is not a valid material.
    public static func fromPrim(_ prim: Prim) -> Material? {
        guard let ref = UsdShadeMaterial_FromPrim(prim.handle) else {
            return nil
        }
        return Material(handle: ref)
    }

    // MARK: - Properties

    /// Whether the material is valid.
    public var isValid: Bool {
        UsdShadeMaterial_IsValid(handle)
    }

    /// The underlying prim.
    public var prim: Prim? {
        guard let ref = UsdShadeMaterial_GetPrim(handle) else {
            return nil
        }
        return Prim(handle: ref)
    }

    /// The path of the material.
    public var path: Path? {
        guard let ref = UsdShadeMaterial_GetPath(handle) else {
            return nil
        }
        return Path(handle: ref)
    }

    // MARK: - Surface Output

    /// Creates the surface output for the given render context.
    ///
    /// - Parameter renderContext: The render context token, or `nil` for universal context.
    /// - Returns: The created output.
    public func createSurfaceOutput(renderContext: Token? = nil) -> Output? {
        guard let ref = UsdShadeMaterial_CreateSurfaceOutput(handle, renderContext?.handle) else {
            return nil
        }
        return Output(handle: ref)
    }

    /// Gets the surface output for the given render context.
    ///
    /// - Parameter renderContext: The render context token, or `nil` for universal context.
    /// - Returns: The surface output, or `nil` if not found.
    public func surfaceOutput(renderContext: Token? = nil) -> Output? {
        guard let ref = UsdShadeMaterial_GetSurfaceOutput(handle, renderContext?.handle) else {
            return nil
        }
        return Output(handle: ref)
    }

    /// Computes the surface source shader.
    ///
    /// - Parameter renderContext: The render context token, or `nil` for universal context.
    /// - Returns: The surface shader, or `nil` if not found.
    public func computeSurfaceSource(renderContext: Token? = nil) -> Shader? {
        guard let ref = UsdShadeMaterial_ComputeSurfaceSource(handle, renderContext?.handle, nil, nil) else {
            return nil
        }
        return Shader(handle: ref)
    }

    // MARK: - Displacement Output

    /// Creates the displacement output for the given render context.
    ///
    /// - Parameter renderContext: The render context token, or `nil` for universal context.
    /// - Returns: The created output.
    public func createDisplacementOutput(renderContext: Token? = nil) -> Output? {
        guard let ref = UsdShadeMaterial_CreateDisplacementOutput(handle, renderContext?.handle) else {
            return nil
        }
        return Output(handle: ref)
    }

    /// Gets the displacement output for the given render context.
    ///
    /// - Parameter renderContext: The render context token, or `nil` for universal context.
    /// - Returns: The displacement output, or `nil` if not found.
    public func displacementOutput(renderContext: Token? = nil) -> Output? {
        guard let ref = UsdShadeMaterial_GetDisplacementOutput(handle, renderContext?.handle) else {
            return nil
        }
        return Output(handle: ref)
    }

    // MARK: - Volume Output

    /// Creates the volume output for the given render context.
    ///
    /// - Parameter renderContext: The render context token, or `nil` for universal context.
    /// - Returns: The created output.
    public func createVolumeOutput(renderContext: Token? = nil) -> Output? {
        guard let ref = UsdShadeMaterial_CreateVolumeOutput(handle, renderContext?.handle) else {
            return nil
        }
        return Output(handle: ref)
    }

    /// Gets the volume output for the given render context.
    ///
    /// - Parameter renderContext: The render context token, or `nil` for universal context.
    /// - Returns: The volume output, or `nil` if not found.
    public func volumeOutput(renderContext: Token? = nil) -> Output? {
        guard let ref = UsdShadeMaterial_GetVolumeOutput(handle, renderContext?.handle) else {
            return nil
        }
        return Output(handle: ref)
    }

    // MARK: - Base Material

    /// The base material (parent in specialization hierarchy).
    public var baseMaterial: Material? {
        guard let ref = UsdShadeMaterial_GetBaseMaterial(handle) else {
            return nil
        }
        return Material(handle: ref)
    }

    /// The path of the base material.
    public var baseMaterialPath: Path? {
        guard let ref = UsdShadeMaterial_GetBaseMaterialPath(handle) else {
            return nil
        }
        return Path(handle: ref)
    }

    /// Whether the material has a base material.
    public var hasBaseMaterial: Bool {
        UsdShadeMaterial_HasBaseMaterial(handle)
    }

    /// Sets the base material.
    ///
    /// - Parameter baseMaterial: The base material to set.
    /// - Throws: `ShadeError.operationFailed` if the operation fails.
    public func setBaseMaterial(_ baseMaterial: Material) throws {
        let result = UsdShadeMaterial_SetBaseMaterial(handle, baseMaterial.handle)
        guard result == USD_RESULT_SUCCESS else {
            throw ShadeError.operationFailed("Failed to set base material")
        }
    }

    /// Sets the base material by path.
    ///
    /// - Parameter path: The path to the base material.
    /// - Throws: `ShadeError.operationFailed` if the operation fails.
    public func setBaseMaterialPath(_ path: Path) throws {
        let result = UsdShadeMaterial_SetBaseMaterialPath(handle, path.handle)
        guard result == USD_RESULT_SUCCESS else {
            throw ShadeError.operationFailed("Failed to set base material path")
        }
    }

    /// Clears the base material.
    ///
    /// - Throws: `ShadeError.operationFailed` if the operation fails.
    public func clearBaseMaterial() throws {
        let result = UsdShadeMaterial_ClearBaseMaterial(handle)
        guard result == USD_RESULT_SUCCESS else {
            throw ShadeError.operationFailed("Failed to clear base material")
        }
    }

    // MARK: - Inputs

    /// Creates an input on the material.
    ///
    /// - Parameters:
    ///   - name: The name of the input.
    ///   - typeName: The type name of the input.
    /// - Returns: The created input.
    public func createInput(name: Token, typeName: Token) -> Input? {
        guard let ref = UsdShadeMaterial_CreateInput(handle, name.handle, typeName.handle) else {
            return nil
        }
        return Input(handle: ref)
    }

    /// Gets an input by name.
    ///
    /// - Parameter name: The name of the input.
    /// - Returns: The input, or `nil` if not found.
    public func input(named name: Token) -> Input? {
        guard let ref = UsdShadeMaterial_GetInput(handle, name.handle) else {
            return nil
        }
        return Input(handle: ref)
    }

    /// Gets all inputs.
    ///
    /// - Parameter onlyAuthored: If true, only returns authored inputs.
    /// - Returns: Array of inputs.
    public func inputs(onlyAuthored: Bool = true) -> [Input] {
        let count = UsdShadeMaterial_GetInputCount(handle, onlyAuthored)
        guard count > 0 else { return [] }

        var handles = [UsdShadeInputRef?](repeating: nil, count: count)
        let actualCount = handles.withUnsafeMutableBufferPointer { buffer in
            UsdShadeMaterial_GetInputs(handle, buffer.baseAddress, count, onlyAuthored)
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

    /// Creates an output on the material.
    ///
    /// - Parameters:
    ///   - name: The name of the output.
    ///   - typeName: The type name of the output.
    /// - Returns: The created output.
    public func createOutput(name: Token, typeName: Token) -> Output? {
        guard let ref = UsdShadeMaterial_CreateOutput(handle, name.handle, typeName.handle) else {
            return nil
        }
        return Output(handle: ref)
    }

    /// Gets an output by name.
    ///
    /// - Parameter name: The name of the output.
    /// - Returns: The output, or `nil` if not found.
    public func output(named name: Token) -> Output? {
        guard let ref = UsdShadeMaterial_GetOutput(handle, name.handle) else {
            return nil
        }
        return Output(handle: ref)
    }

    /// Gets all outputs.
    ///
    /// - Parameter onlyAuthored: If true, only returns authored outputs.
    /// - Returns: Array of outputs.
    public func outputs(onlyAuthored: Bool = true) -> [Output] {
        let count = UsdShadeMaterial_GetOutputCount(handle, onlyAuthored)
        guard count > 0 else { return [] }

        var handles = [UsdShadeOutputRef?](repeating: nil, count: count)
        let actualCount = handles.withUnsafeMutableBufferPointer { buffer in
            UsdShadeMaterial_GetOutputs(handle, buffer.baseAddress, count, onlyAuthored)
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

extension Material: CustomStringConvertible {

    public var description: String {
        path?.description ?? "<invalid material>"
    }
}

// MARK: - CustomDebugStringConvertible

extension Material: CustomDebugStringConvertible {

    public var debugDescription: String {
        "Material(\"\(path?.description ?? "<invalid>")\")"
    }
}
