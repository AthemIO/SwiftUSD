// UsdShadeCxx.swift - Swift extensions for UsdShade C++ interop types
// This provides Swift convenience APIs on top of the C++ interop layer (USDCxx.UsdShade)

import USDCxx

// MARK: - Type Aliases for C++ Shading Types

/// Typealias for the C++ ShadeInputRef type
public typealias ShadeInputCxx = swiftusd.ShadeInputRef

/// Typealias for the C++ ShadeOutputRef type
public typealias ShadeOutputCxx = swiftusd.ShadeOutputRef

/// Typealias for the C++ ShadeShaderRef type
public typealias ShadeShaderCxx = swiftusd.ShadeShaderRef

/// Typealias for the C++ ShadeNodeGraphRef type
public typealias ShadeNodeGraphCxx = swiftusd.ShadeNodeGraphRef

/// Typealias for the C++ ShadeMaterialRef type
public typealias ShadeMaterialCxx = swiftusd.ShadeMaterialRef

/// Typealias for the C++ ShadeMaterialBindingAPIRef type
public typealias ShadeMaterialBindingAPICxx = swiftusd.ShadeMaterialBindingAPIRef

/// Typealias for the C++ ShadeConnectionSourceInfo type
public typealias ShadeConnectionSourceInfoCxx = swiftusd.ShadeConnectionSourceInfo

// MARK: - ShadeInputRef Extensions

extension swiftusd.ShadeInputRef {
    /// The base name of the input (without "inputs:" prefix)
    public var baseName: swiftusd.Token {
        GetBaseName()
    }

    /// The full name of the input (with "inputs:" prefix)
    public var fullName: swiftusd.Token {
        GetFullName()
    }

    /// The type name of the input
    public var typeName: swiftusd.Token {
        GetTypeName()
    }

    /// Whether the input is valid
    public var isValid: Bool {
        IsValid()
    }

    /// Whether the input has a connected source
    public var hasConnectedSource: Bool {
        HasConnectedSource()
    }

    /// The connected source path
    public var connectedSource: swiftusd.Path {
        GetConnectedSource()
    }

    /// Gets the float value of this input
    public func getFloat(at time: swiftusd.TimeCode = .Default()) -> Float? {
        var value: Float = 0
        return GetFloat(time, &value) ? value : nil
    }

    /// Gets the Vec3f value of this input
    public func getVec3f(at time: swiftusd.TimeCode = .Default()) -> swiftusd.Vec3f? {
        var value = swiftusd.Vec3f()
        return GetVec3f(time, &value) ? value : nil
    }

    /// Gets the token value of this input
    public func getToken(at time: swiftusd.TimeCode = .Default()) -> swiftusd.Token? {
        var value = swiftusd.Token()
        return GetToken(time, &value) ? value : nil
    }

    /// Sets the float value of this input
    @discardableResult
    public func set(_ value: Float, at time: swiftusd.TimeCode = .Default()) -> Bool {
        SetFloat(value, time)
    }

    /// Sets the Vec3f value of this input
    @discardableResult
    public func set(_ value: swiftusd.Vec3f, at time: swiftusd.TimeCode = .Default()) -> Bool {
        SetVec3f(value, time)
    }

    /// Sets the token value of this input
    @discardableResult
    public func set(_ value: swiftusd.Token, at time: swiftusd.TimeCode = .Default()) -> Bool {
        SetToken(value, time)
    }

    /// Sets the string value of this input
    @discardableResult
    public func set(_ value: String, at time: swiftusd.TimeCode = .Default()) -> Bool {
        SetString(value, time)
    }

    /// Connects this input to a source path
    @discardableResult
    public func connect(to source: swiftusd.Path) -> Bool {
        SetConnectedSource(source)
    }

    /// Disconnects this input
    @discardableResult
    public func disconnect() -> Bool {
        ClearSource()
    }
}

// MARK: - ShadeOutputRef Extensions

extension swiftusd.ShadeOutputRef {
    /// The base name of the output (without "outputs:" prefix)
    public var baseName: swiftusd.Token {
        GetBaseName()
    }

    /// The full name of the output (with "outputs:" prefix)
    public var fullName: swiftusd.Token {
        GetFullName()
    }

    /// The type name of the output
    public var typeName: swiftusd.Token {
        GetTypeName()
    }

    /// Whether the output is valid
    public var isValid: Bool {
        IsValid()
    }

    /// Whether the output has a connected source
    public var hasConnectedSource: Bool {
        HasConnectedSource()
    }

    /// The connected source path
    public var connectedSource: swiftusd.Path {
        GetConnectedSource()
    }

    /// Connects this output to a source path
    @discardableResult
    public func connect(to source: swiftusd.Path) -> Bool {
        SetConnectedSource(source)
    }
}

// MARK: - ShadeShaderRef Extensions

extension swiftusd.ShadeShaderRef {
    /// Defines a new Shader prim at the given path.
    public static func define(on stage: swiftusd.StageRef, at path: swiftusd.Path) -> swiftusd.ShadeShaderRef? {
        swiftusd.ShadeShaderRef.Define(stage, path)
    }

    /// Creates a shader from a prim.
    public static func from(prim: swiftusd.PrimRef) -> swiftusd.ShadeShaderRef? {
        swiftusd.ShadeShaderRef.FromPrim(prim)
    }

    /// Whether the shader is valid
    public var isValid: Bool {
        IsValid()
    }

    /// The shader ID (e.g., "UsdPreviewSurface")
    public var shaderId: swiftusd.Token {
        get { GetShaderId() }
        set { _ = SetShaderId(newValue) }
    }

    /// The number of inputs
    public var inputCount: Int {
        Int(GetInputCount())
    }

    /// The number of outputs
    public var outputCount: Int {
        Int(GetOutputCount())
    }

    /// Gets the underlying prim
    public var prim: swiftusd.PrimRef? {
        GetPrim()
    }

    /// Creates an input with the given name and type
    public func createInput(named name: String, typeName: String) -> swiftusd.ShadeInputRef? {
        CreateInput(swiftusd.Token(name), swiftusd.Token(typeName))
    }

    /// Gets an input by name
    public func input(named name: String) -> swiftusd.ShadeInputRef? {
        GetInput(swiftusd.Token(name))
    }

    /// Creates an output with the given name and type
    public func createOutput(named name: String, typeName: String) -> swiftusd.ShadeOutputRef? {
        CreateOutput(swiftusd.Token(name), swiftusd.Token(typeName))
    }

    /// Gets an output by name
    public func output(named name: String) -> swiftusd.ShadeOutputRef? {
        GetOutput(swiftusd.Token(name))
    }
}

// MARK: - ShadeNodeGraphRef Extensions

extension swiftusd.ShadeNodeGraphRef {
    /// Defines a new NodeGraph prim at the given path.
    public static func define(on stage: swiftusd.StageRef, at path: swiftusd.Path) -> swiftusd.ShadeNodeGraphRef? {
        swiftusd.ShadeNodeGraphRef.Define(stage, path)
    }

    /// Creates a node graph from a prim.
    public static func from(prim: swiftusd.PrimRef) -> swiftusd.ShadeNodeGraphRef? {
        swiftusd.ShadeNodeGraphRef.FromPrim(prim)
    }

    /// Whether the node graph is valid
    public var isValid: Bool {
        IsValid()
    }

    /// The number of inputs
    public var inputCount: Int {
        Int(GetInputCount())
    }

    /// The number of outputs
    public var outputCount: Int {
        Int(GetOutputCount())
    }

    /// Gets the underlying prim
    public var prim: swiftusd.PrimRef? {
        GetPrim()
    }

    /// Creates an input with the given name and type
    public func createInput(named name: String, typeName: String) -> swiftusd.ShadeInputRef? {
        CreateInput(swiftusd.Token(name), swiftusd.Token(typeName))
    }

    /// Gets an input by name
    public func input(named name: String) -> swiftusd.ShadeInputRef? {
        GetInput(swiftusd.Token(name))
    }

    /// Creates an output with the given name and type
    public func createOutput(named name: String, typeName: String) -> swiftusd.ShadeOutputRef? {
        CreateOutput(swiftusd.Token(name), swiftusd.Token(typeName))
    }

    /// Gets an output by name
    public func output(named name: String) -> swiftusd.ShadeOutputRef? {
        GetOutput(swiftusd.Token(name))
    }
}

// MARK: - ShadeMaterialRef Extensions

extension swiftusd.ShadeMaterialRef {
    /// Defines a new Material prim at the given path.
    public static func define(on stage: swiftusd.StageRef, at path: swiftusd.Path) -> swiftusd.ShadeMaterialRef? {
        swiftusd.ShadeMaterialRef.Define(stage, path)
    }

    /// Creates a material from a prim.
    public static func from(prim: swiftusd.PrimRef) -> swiftusd.ShadeMaterialRef? {
        swiftusd.ShadeMaterialRef.FromPrim(prim)
    }

    /// Whether the material is valid
    public var isValid: Bool {
        IsValid()
    }

    /// Gets the underlying prim
    public var prim: swiftusd.PrimRef? {
        GetPrim()
    }

    /// Creates a surface output for the given render context
    public func createSurfaceOutput(renderContext: String = "") -> swiftusd.ShadeOutputRef? {
        CreateSurfaceOutput(swiftusd.Token(renderContext))
    }

    /// Gets the surface output for the given render context
    public func surfaceOutput(renderContext: String = "") -> swiftusd.ShadeOutputRef? {
        GetSurfaceOutput(swiftusd.Token(renderContext))
    }

    /// Creates a displacement output for the given render context
    public func createDisplacementOutput(renderContext: String = "") -> swiftusd.ShadeOutputRef? {
        CreateDisplacementOutput(swiftusd.Token(renderContext))
    }

    /// Gets the displacement output for the given render context
    public func displacementOutput(renderContext: String = "") -> swiftusd.ShadeOutputRef? {
        GetDisplacementOutput(swiftusd.Token(renderContext))
    }

    /// Creates a volume output for the given render context
    public func createVolumeOutput(renderContext: String = "") -> swiftusd.ShadeOutputRef? {
        CreateVolumeOutput(swiftusd.Token(renderContext))
    }

    /// Creates an input with the given name and type
    public func createInput(named name: String, typeName: String) -> swiftusd.ShadeInputRef? {
        CreateInput(swiftusd.Token(name), swiftusd.Token(typeName))
    }

    /// Gets an input by name
    public func input(named name: String) -> swiftusd.ShadeInputRef? {
        GetInput(swiftusd.Token(name))
    }
}

// MARK: - ShadeMaterialBindingAPIRef Extensions

extension swiftusd.ShadeMaterialBindingAPIRef {
    /// Applies the MaterialBindingAPI to a prim.
    public static func apply(to prim: swiftusd.PrimRef) -> swiftusd.ShadeMaterialBindingAPIRef? {
        swiftusd.ShadeMaterialBindingAPIRef.Apply(prim)
    }

    /// Gets the MaterialBindingAPI from a prim.
    public static func from(prim: swiftusd.PrimRef) -> swiftusd.ShadeMaterialBindingAPIRef? {
        swiftusd.ShadeMaterialBindingAPIRef.FromPrim(prim)
    }

    /// Whether the API is valid
    public var isValid: Bool {
        IsValid()
    }

    /// Gets the underlying prim
    public var prim: swiftusd.PrimRef? {
        GetPrim()
    }

    /// Binds a material to this prim
    @discardableResult
    public func bind(material: swiftusd.ShadeMaterialRef, purpose: String = "") -> Bool {
        Bind(material, swiftusd.Token(), swiftusd.Token(purpose))
    }

    /// Unbinds the material from this prim
    @discardableResult
    public func unbind(purpose: String = "") -> Bool {
        Unbind(swiftusd.Token(purpose))
    }

    /// Gets the bound material
    public func boundMaterial(purpose: String = "") -> swiftusd.ShadeMaterialRef? {
        GetBoundMaterial(swiftusd.Token(purpose))
    }

    /// Whether a material is bound
    public func hasBoundMaterial(purpose: String = "") -> Bool {
        HasBoundMaterial(swiftusd.Token(purpose))
    }

    /// Gets the directly bound material path
    public func directlyBoundMaterialPath(purpose: String = "") -> swiftusd.Path {
        GetDirectlyBoundMaterialPath(swiftusd.Token(purpose))
    }
}

// MARK: - ShadeConnectionSourceInfo Extensions

extension swiftusd.ShadeConnectionSourceInfo {
    /// Whether this connection info is valid
    public var isValid: Bool {
        IsValid()
    }
}
