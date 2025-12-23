// Shader.swift - UsdShadeShader wrapper
// Mirrors: pxr/usdShade/shader.h

import OpenUSDInterop

/// A shader is a node in a shading network.
/// Mirrors pxr::UsdShadeShader from the USD C++ API.
public struct UsdShadeShader: Sendable {
    public var prim: UsdPrim

    public init(_ prim: UsdPrim) {
        self.prim = prim
    }

    /// Returns true if the prim is a valid shader.
    public var isValid: Bool {
        return prim.isValid
    }

    // MARK: - Define

    /// Defines a new shader at the given path.
    public static func define(on stage: UsdStage, at path: SdfPath) -> UsdShadeShader? {
        guard let prim = stage.definePrim(at: path, type: "Shader") else {
            return nil
        }
        return UsdShadeShader(prim)
    }

    // MARK: - Shader ID

    /// Gets the shader ID (the implementation identifier).
    public func getShaderId() -> TfToken {
        // TODO: Implement via C interop
        return TfToken()
    }

    /// Sets the shader ID.
    public func setShaderId(_ id: TfToken) -> Bool {
        // TODO: Implement via C interop
        return false
    }

    // MARK: - Inputs

    /// Creates an input on this shader.
    public func createInput(_ name: TfToken, typeName: SdfValueTypeName) -> UsdShadeInput? {
        // TODO: Implement via C interop
        return nil
    }

    /// Gets an input by name.
    public func getInput(_ name: TfToken) -> UsdShadeInput? {
        // TODO: Implement via C interop
        return nil
    }

    /// Gets all inputs.
    public func getInputs() -> [UsdShadeInput] {
        // TODO: Implement via C interop
        return []
    }

    // MARK: - Outputs

    /// Creates an output on this shader.
    public func createOutput(_ name: TfToken, typeName: SdfValueTypeName) -> UsdShadeOutput? {
        // TODO: Implement via C interop
        return nil
    }

    /// Gets an output by name.
    public func getOutput(_ name: TfToken) -> UsdShadeOutput? {
        // TODO: Implement via C interop
        return nil
    }

    /// Gets all outputs.
    public func getOutputs() -> [UsdShadeOutput] {
        // TODO: Implement via C interop
        return []
    }
}

// MARK: - UsdShadeInput

/// An input on a shader.
/// Mirrors pxr::UsdShadeInput from the USD C++ API.
public struct UsdShadeInput: Sendable {
    internal var handle: OpaquePointer?

    internal init(handle: OpaquePointer?) {
        self.handle = handle
    }

    /// Returns true if this input is valid.
    public var isValid: Bool {
        return handle != nil
    }

    /// Sets a value on this input.
    public func set<T>(_ value: T, at time: UsdTimeCode = .default) -> Bool {
        // TODO: Implement via C interop
        return false
    }

    /// Gets the value of this input.
    public func get<T>(at time: UsdTimeCode = .default) -> T? {
        // TODO: Implement via C interop
        return nil
    }

    /// Connects this input to a source output.
    public func connect(to source: UsdShadeOutput) -> Bool {
        // TODO: Implement via C interop
        return false
    }
}
