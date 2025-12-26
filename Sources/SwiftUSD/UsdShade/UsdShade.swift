// UsdShade.swift - Shading Schemas - Idiomatic Swift API
//
// This module provides Swift wrappers around CxxFacade's UsdShade types,
// including shaders, materials, node graphs, and material binding APIs.
//
// The UsdShade module is the primary API for working with shading in USD,
// providing schemas for materials, shaders, inputs/outputs, and connections.

import CxxFacade

// MARK: - Type Aliases

// --- Enumerations ---

/// UsdShadeAttributeType - Type of shading attribute.
public typealias UsdShadeAttributeType = cxxfacade.usdShade.UsdShadeAttributeType

/// UsdShadeConnectionModification - Connection modification mode.
public typealias UsdShadeConnectionModification = cxxfacade.usdShade.UsdShadeConnectionModification

// --- Input/Output Types ---

/// UsdShadeInput - Represents an input on a shader or node-graph.
///
/// UsdShadeInput wraps an attribute that represents a connectable
/// parameter for receiving values in a shading network.
public typealias UsdShadeInput = cxxfacade.usdShade.UsdShadeInput

/// UsdShadeOutput - Represents an output on a shader or node-graph.
///
/// UsdShadeOutput wraps an attribute that represents a connectable
/// output value in a shading network.
public typealias UsdShadeOutput = cxxfacade.usdShade.UsdShadeOutput

// --- Schema Types ---

/// UsdShadeConnectableAPI - API for creating outputs and connections.
///
/// UsdShadeConnectableAPI provides a common interface for shaders and
/// node-graphs to create inputs/outputs and establish connections.
public typealias UsdShadeConnectableAPI = cxxfacade.usdShade.UsdShadeConnectableAPI

/// UsdShadeShader - Base class for all USD shaders.
///
/// Shaders are the building blocks of shading networks. They represent
/// a single shading object like a texture, noise function, or mix node.
///
/// ## Usage
/// ```swift
/// var shader = UsdShadeShader.define(&stage, path: "/Materials/MyMaterial/Surface")
/// shader.setShaderId(TfToken("UsdPreviewSurface"))
/// let colorInput = shader.createInput(TfToken("diffuseColor"), typeName: "color3f")
/// ```
public typealias UsdShadeShader = cxxfacade.usdShade.UsdShadeShader

/// UsdShadeNodeGraph - A container for shading nodes.
///
/// A node-graph is a container for shading nodes, as well as other
/// node-graphs. It has a public input interface and provides outputs.
public typealias UsdShadeNodeGraph = cxxfacade.usdShade.UsdShadeNodeGraph

/// UsdShadeMaterial - A material for shading geometry.
///
/// A Material provides a container into which multiple "render contexts"
/// can add data that defines a "shading material" for a renderer.
///
/// ## Usage
/// ```swift
/// var material = UsdShadeMaterial.define(&stage, path: "/Materials/MyMaterial")
/// let surfaceOutput = material.createSurfaceOutput()
/// surfaceOutput.connectToSource(shader.getOutput(TfToken("surface")))
/// ```
public typealias UsdShadeMaterial = cxxfacade.usdShade.UsdShadeMaterial

/// UsdShadeMaterialBindingAPI - API for binding materials to prims.
///
/// UsdShadeMaterialBindingAPI provides an interface for binding materials
/// to prims or collections of prims.
///
/// ## Usage
/// ```swift
/// let bindingAPI = UsdShadeMaterialBindingAPI.apply(mesh.GetPrim())
/// bindingAPI.bind(material)
/// ```
public typealias UsdShadeMaterialBindingAPI = cxxfacade.usdShade.UsdShadeMaterialBindingAPI

// --- Tokens ---

/// UsdShadeTokens - Common tokens used in UsdShade.
public typealias UsdShadeTokens = cxxfacade.usdShade.UsdShadeTokens

// MARK: - UsdShadeInput Extensions

extension UsdShadeInput {
    /// Whether this is a valid input.
    public var isValid: Bool {
        return self.IsValid()
    }

    /// The full name of the attribute (with "inputs:" prefix).
    public var fullName: TfToken {
        return self.GetFullName()
    }

    /// The base name (without "inputs:" prefix).
    public var baseName: TfToken {
        return self.GetBaseName()
    }

    /// The type name of this input.
    public var typeName: String {
        return String(self.GetTypeName())
    }

    /// The prim that owns this input.
    public var prim: UsdPrim {
        return self.GetPrim()
    }

    /// The underlying attribute.
    public var attr: UsdAttribute {
        return self.GetAttr()
    }

    /// Whether this input has a connected source.
    public var hasConnectedSource: Bool {
        return self.HasConnectedSource()
    }

    /// The render type of this input.
    public var renderType: TfToken {
        return self.GetRenderType()
    }

    /// Whether a render type has been specified.
    public var hasRenderType: Bool {
        return self.HasRenderType()
    }

    /// The connectability of this input.
    public var connectability: TfToken {
        return self.GetConnectability()
    }

    /// The documentation string.
    public var documentation: String {
        return String(self.GetDocumentation())
    }

    /// The display group.
    public var displayGroup: String {
        return String(self.GetDisplayGroup())
    }

    /// Connect to a source path.
    @discardableResult
    public func connect(to sourcePath: SdfPath) -> Bool {
        return self.ConnectToSource(sourcePath)
    }

    /// Connect to another input.
    @discardableResult
    public func connect(to source: UsdShadeInput) -> Bool {
        return self.ConnectToSource(source)
    }

    /// Connect to an output.
    @discardableResult
    public func connect(to source: UsdShadeOutput) -> Bool {
        return self.ConnectToSource(source)
    }

    /// Disconnect from source.
    @discardableResult
    public func disconnect() -> Bool {
        return self.DisconnectSource()
    }

    /// Clear all sources.
    @discardableResult
    public func clearSources() -> Bool {
        return self.ClearSources()
    }
}

// MARK: - UsdShadeOutput Extensions

extension UsdShadeOutput {
    /// Whether this is a valid output.
    public var isValid: Bool {
        return self.IsValid()
    }

    /// The full name of the attribute (with "outputs:" prefix).
    public var fullName: TfToken {
        return self.GetFullName()
    }

    /// The base name (without "outputs:" prefix).
    public var baseName: TfToken {
        return self.GetBaseName()
    }

    /// The type name of this output.
    public var typeName: String {
        return String(self.GetTypeName())
    }

    /// The prim that owns this output.
    public var prim: UsdPrim {
        return self.GetPrim()
    }

    /// The underlying attribute.
    public var attr: UsdAttribute {
        return self.GetAttr()
    }

    /// Whether this output has a connected source.
    public var hasConnectedSource: Bool {
        return self.HasConnectedSource()
    }

    /// The render type of this output.
    public var renderType: TfToken {
        return self.GetRenderType()
    }

    /// Whether a render type has been specified.
    public var hasRenderType: Bool {
        return self.HasRenderType()
    }

    /// Connect to a source path.
    @discardableResult
    public func connect(to sourcePath: SdfPath) -> Bool {
        return self.ConnectToSource(sourcePath)
    }

    /// Connect to an input.
    @discardableResult
    public func connect(to source: UsdShadeInput) -> Bool {
        return self.ConnectToSource(source)
    }

    /// Connect to another output.
    @discardableResult
    public func connect(to source: UsdShadeOutput) -> Bool {
        return self.ConnectToSource(source)
    }

    /// Disconnect from source.
    @discardableResult
    public func disconnect() -> Bool {
        return self.DisconnectSource()
    }

    /// Clear all sources.
    @discardableResult
    public func clearSources() -> Bool {
        return self.ClearSources()
    }
}

// MARK: - UsdShadeConnectableAPI Extensions

extension UsdShadeConnectableAPI {
    /// Whether the prim is a container (like a NodeGraph).
    public var isContainer: Bool {
        return self.IsContainer()
    }

    /// Whether container encapsulation rules should be respected.
    public var requiresEncapsulation: Bool {
        return self.RequiresEncapsulation()
    }

    /// Create an input.
    public mutating func createInput(_ name: TfToken, typeName: String) -> UsdShadeInput {
        return self.CreateInput(name, std.string(typeName))
    }

    /// Create an input with a string name.
    public mutating func createInput(_ name: String, typeName: String) -> UsdShadeInput {
        return self.CreateInput(TfToken(name), std.string(typeName))
    }

    /// Get an input by name.
    public func input(_ name: TfToken) -> UsdShadeInput {
        return self.GetInput(name)
    }

    /// Get an input by string name.
    public func input(_ name: String) -> UsdShadeInput {
        return self.GetInput(TfToken(name))
    }

    /// Create an output.
    public mutating func createOutput(_ name: TfToken, typeName: String) -> UsdShadeOutput {
        return self.CreateOutput(name, std.string(typeName))
    }

    /// Create an output with a string name.
    public mutating func createOutput(_ name: String, typeName: String) -> UsdShadeOutput {
        return self.CreateOutput(TfToken(name), std.string(typeName))
    }

    /// Get an output by name.
    public func output(_ name: TfToken) -> UsdShadeOutput {
        return self.GetOutput(name)
    }

    /// Get an output by string name.
    public func output(_ name: String) -> UsdShadeOutput {
        return self.GetOutput(TfToken(name))
    }
}

// MARK: - UsdShadeShader Extensions

extension UsdShadeShader {
    /// Define a UsdShadeShader at the given path.
    public static func define(_ stage: inout UsdStage, path: SdfPath) -> UsdShadeShader {
        return UsdShadeShader.Define(&stage, path)
    }

    /// Define a UsdShadeShader at the given path string.
    public static func define(_ stage: inout UsdStage, path: String) -> UsdShadeShader {
        return UsdShadeShader.Define(&stage, SdfPath(path))
    }

    /// Get the ConnectableAPI for this shader.
    public func connectableAPI() -> UsdShadeConnectableAPI {
        return self.ConnectableAPI()
    }

    /// The implementation source.
    public var implementationSource: TfToken {
        return self.GetImplementationSource()
    }

    /// Set the shader ID.
    @discardableResult
    public mutating func setShaderId(_ id: TfToken) -> Bool {
        return self.SetShaderId(id)
    }

    /// Set the shader ID from a string.
    @discardableResult
    public mutating func setShaderId(_ id: String) -> Bool {
        return self.SetShaderId(TfToken(id))
    }

    /// Get the shader ID.
    public func shaderId() -> TfToken? {
        var id = TfToken()
        if self.GetShaderId(&id) {
            return id
        }
        return nil
    }

    /// Create an input.
    public mutating func createInput(_ name: TfToken, typeName: String) -> UsdShadeInput {
        return self.CreateInput(name, std.string(typeName))
    }

    /// Create an input with a string name.
    public mutating func createInput(_ name: String, typeName: String) -> UsdShadeInput {
        return self.CreateInput(TfToken(name), std.string(typeName))
    }

    /// Get an input by name.
    public func input(_ name: TfToken) -> UsdShadeInput {
        return self.GetInput(name)
    }

    /// Get an input by string name.
    public func input(_ name: String) -> UsdShadeInput {
        return self.GetInput(TfToken(name))
    }

    /// Create an output.
    public mutating func createOutput(_ name: TfToken, typeName: String) -> UsdShadeOutput {
        return self.CreateOutput(name, std.string(typeName))
    }

    /// Create an output with a string name.
    public mutating func createOutput(_ name: String, typeName: String) -> UsdShadeOutput {
        return self.CreateOutput(TfToken(name), std.string(typeName))
    }

    /// Get an output by name.
    public func output(_ name: TfToken) -> UsdShadeOutput {
        return self.GetOutput(name)
    }

    /// Get an output by string name.
    public func output(_ name: String) -> UsdShadeOutput {
        return self.GetOutput(TfToken(name))
    }
}

// MARK: - UsdShadeNodeGraph Extensions

extension UsdShadeNodeGraph {
    /// Define a UsdShadeNodeGraph at the given path.
    public static func define(_ stage: inout UsdStage, path: SdfPath) -> UsdShadeNodeGraph {
        return UsdShadeNodeGraph.Define(&stage, path)
    }

    /// Define a UsdShadeNodeGraph at the given path string.
    public static func define(_ stage: inout UsdStage, path: String) -> UsdShadeNodeGraph {
        return UsdShadeNodeGraph.Define(&stage, SdfPath(path))
    }

    /// Get the ConnectableAPI for this node graph.
    public func connectableAPI() -> UsdShadeConnectableAPI {
        return self.ConnectableAPI()
    }

    /// Create an input (interface attribute).
    public mutating func createInput(_ name: TfToken, typeName: String) -> UsdShadeInput {
        return self.CreateInput(name, std.string(typeName))
    }

    /// Create an input with a string name.
    public mutating func createInput(_ name: String, typeName: String) -> UsdShadeInput {
        return self.CreateInput(TfToken(name), std.string(typeName))
    }

    /// Get an input by name.
    public func input(_ name: TfToken) -> UsdShadeInput {
        return self.GetInput(name)
    }

    /// Get an input by string name.
    public func input(_ name: String) -> UsdShadeInput {
        return self.GetInput(TfToken(name))
    }

    /// Create an output.
    public mutating func createOutput(_ name: TfToken, typeName: String) -> UsdShadeOutput {
        return self.CreateOutput(name, std.string(typeName))
    }

    /// Create an output with a string name.
    public mutating func createOutput(_ name: String, typeName: String) -> UsdShadeOutput {
        return self.CreateOutput(TfToken(name), std.string(typeName))
    }

    /// Get an output by name.
    public func output(_ name: TfToken) -> UsdShadeOutput {
        return self.GetOutput(name)
    }

    /// Get an output by string name.
    public func output(_ name: String) -> UsdShadeOutput {
        return self.GetOutput(TfToken(name))
    }
}

// MARK: - UsdShadeMaterial Extensions

extension UsdShadeMaterial {
    /// Define a UsdShadeMaterial at the given path.
    public static func define(_ stage: inout UsdStage, path: SdfPath) -> UsdShadeMaterial {
        return UsdShadeMaterial.Define(&stage, path)
    }

    /// Define a UsdShadeMaterial at the given path string.
    public static func define(_ stage: inout UsdStage, path: String) -> UsdShadeMaterial {
        return UsdShadeMaterial.Define(&stage, SdfPath(path))
    }

    /// Create a surface output.
    public mutating func createSurfaceOutput(renderContext: TfToken = TfToken()) -> UsdShadeOutput {
        return self.CreateSurfaceOutput(renderContext)
    }

    /// Get the surface output.
    public func surfaceOutput(renderContext: TfToken = TfToken()) -> UsdShadeOutput {
        return self.GetSurfaceOutput(renderContext)
    }

    /// Create a displacement output.
    public mutating func createDisplacementOutput(renderContext: TfToken = TfToken()) -> UsdShadeOutput {
        return self.CreateDisplacementOutput(renderContext)
    }

    /// Get the displacement output.
    public func displacementOutput(renderContext: TfToken = TfToken()) -> UsdShadeOutput {
        return self.GetDisplacementOutput(renderContext)
    }

    /// Create a volume output.
    public mutating func createVolumeOutput(renderContext: TfToken = TfToken()) -> UsdShadeOutput {
        return self.CreateVolumeOutput(renderContext)
    }

    /// Get the volume output.
    public func volumeOutput(renderContext: TfToken = TfToken()) -> UsdShadeOutput {
        return self.GetVolumeOutput(renderContext)
    }

    /// Get the base material.
    public var baseMaterial: UsdShadeMaterial {
        return self.GetBaseMaterial()
    }

    /// Get the base material path.
    public var baseMaterialPath: SdfPath {
        return self.GetBaseMaterialPath()
    }

    /// Whether this material has a base material.
    public var hasBaseMaterial: Bool {
        return self.HasBaseMaterial()
    }

    /// Set the base material.
    public mutating func setBaseMaterial(_ base: UsdShadeMaterial) {
        self.SetBaseMaterial(base)
    }

    /// Set the base material path.
    public mutating func setBaseMaterialPath(_ path: SdfPath) {
        self.SetBaseMaterialPath(path)
    }

    /// Clear the base material.
    public mutating func clearBaseMaterial() {
        self.ClearBaseMaterial()
    }
}

// MARK: - UsdShadeMaterialBindingAPI Extensions

extension UsdShadeMaterialBindingAPI {
    /// Apply this API schema to the given prim.
    public static func apply(_ prim: UsdPrim) -> UsdShadeMaterialBindingAPI {
        return UsdShadeMaterialBindingAPI.Apply(prim)
    }

    /// Check if this API can be applied to the given prim.
    public static func canApply(_ prim: UsdPrim) -> Bool {
        return UsdShadeMaterialBindingAPI.CanApply(prim, nil)
    }

    /// Bind a material directly.
    @discardableResult
    public mutating func bind(_ material: UsdShadeMaterial,
                              strength: TfToken = TfToken(),
                              purpose: TfToken = TfToken()) -> Bool {
        return self.Bind(material, strength, purpose)
    }

    /// Unbind the direct binding.
    @discardableResult
    public mutating func unbindDirectBinding(purpose: TfToken = TfToken()) -> Bool {
        return self.UnbindDirectBinding(purpose)
    }

    /// Unbind all bindings.
    @discardableResult
    public mutating func unbindAllBindings() -> Bool {
        return self.UnbindAllBindings()
    }

    /// Compute the bound material for the given purpose.
    public func computeBoundMaterial(purpose: TfToken = TfToken()) -> UsdShadeMaterial {
        return self.ComputeBoundMaterial(purpose, nil)
    }
}
