// Material.swift - UsdShadeMaterial wrapper
// Mirrors: pxr/usdShade/material.h

import OpenUSDInterop

/// A material defines the surface appearance of geometry.
/// Mirrors pxr::UsdShadeMaterial from the USD C++ API.
public struct UsdShadeMaterial: Sendable {
    public var prim: UsdPrim

    public init(_ prim: UsdPrim) {
        self.prim = prim
    }

    /// Returns true if the prim is a valid material.
    public var isValid: Bool {
        return prim.isValid
    }

    // MARK: - Define

    /// Defines a new material at the given path.
    public static func define(on stage: UsdStage, at path: SdfPath) -> UsdShadeMaterial? {
        guard let prim = stage.definePrim(at: path, type: "Material") else {
            return nil
        }
        return UsdShadeMaterial(prim)
    }

    // MARK: - Outputs

    /// Returns the surface output.
    public func getSurfaceOutput() -> UsdShadeOutput? {
        // TODO: Implement via C interop
        return nil
    }

    /// Creates the surface output.
    public func createSurfaceOutput() -> UsdShadeOutput? {
        // TODO: Implement via C interop
        return nil
    }

    /// Returns the displacement output.
    public func getDisplacementOutput() -> UsdShadeOutput? {
        // TODO: Implement via C interop
        return nil
    }

    /// Creates the displacement output.
    public func createDisplacementOutput() -> UsdShadeOutput? {
        // TODO: Implement via C interop
        return nil
    }

    /// Returns the volume output.
    public func getVolumeOutput() -> UsdShadeOutput? {
        // TODO: Implement via C interop
        return nil
    }

    // MARK: - Binding

    /// Binds this material to a prim.
    public func bind(to target: UsdPrim) -> Bool {
        // TODO: Implement via C interop
        return false
    }

    /// Unbinds any material from the given prim.
    public static func unbind(from prim: UsdPrim) -> Bool {
        // TODO: Implement via C interop
        return false
    }

    /// Gets the bound material for a prim.
    public static func getBoundMaterial(for prim: UsdPrim) -> UsdShadeMaterial? {
        // TODO: Implement via C interop
        return nil
    }
}

// MARK: - UsdShadeOutput

/// An output on a shader or material.
/// Mirrors pxr::UsdShadeOutput from the USD C++ API.
public struct UsdShadeOutput: Sendable {
    internal var handle: OpaquePointer?

    internal init(handle: OpaquePointer?) {
        self.handle = handle
    }

    /// Returns true if this output is valid.
    public var isValid: Bool {
        return handle != nil
    }

    /// Connects this output to a source.
    public func connect(to source: UsdShadeOutput) -> Bool {
        // TODO: Implement via C interop
        return false
    }
}
