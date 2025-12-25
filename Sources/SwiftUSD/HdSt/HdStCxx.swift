// HdStCxx.swift - Swift wrappers for Storm Renderer
// Provides convenience extensions and typealiases for the HdSt module

import USDCxx

// MARK: - Type Aliases

/// Storm's render delegate implementation
public typealias HdStRenderDelegate = swiftusd.HdStRenderDelegateRef

/// Storm's GPU resource registry
public typealias HdStResourceRegistry = swiftusd.HdStResourceRegistryRef

/// Mesh topology information
public typealias HdStMeshTopologyInfo = swiftusd.HdStMeshTopologyInfo

/// Material parameter description
public typealias HdStMaterialParam = swiftusd.HdStMaterialParam

/// Material binding information
public typealias HdStMaterialBinding = swiftusd.HdStMaterialBinding

/// Draw item metadata
public typealias HdStDrawItemInfo = swiftusd.HdStDrawItemInfo

/// Render pass state configuration
public typealias HdStRenderPassStateConfig = swiftusd.HdStRenderPassStateConfig

/// AOV (render output) descriptor
public typealias HdStAovDescriptor = swiftusd.HdStAovDescriptor

// MARK: - Enumerations

/// Texture types supported by Storm
public typealias HdStTextureType = swiftusd.HdStTextureType

/// GPU compute queue priorities
public typealias HdStComputeQueuePriority = swiftusd.HdStComputeQueuePriority

// MARK: - HdStRenderDelegateRef Extensions

extension swiftusd.HdStRenderDelegateRef {
    /// Create a new Storm render delegate.
    public static func create() -> swiftusd.HdStRenderDelegateRef? {
        return swiftusd.HdStRenderDelegateRef.Create()
    }

    /// Check if Storm is supported on this hardware.
    public static var isSupported: Bool {
        return swiftusd.HdStRenderDelegateRef.IsSupported()
    }

    /// Get supported Rprim types as an array of Tokens.
    public var supportedRprimTypes: [swiftusd.Token] {
        let vec = GetSupportedRprimTypes()
        var result: [swiftusd.Token] = []
        for i in 0..<vec.size() {
            result.append(vec[i])
        }
        return result
    }

    /// Get supported Sprim types as an array of Tokens.
    public var supportedSprimTypes: [swiftusd.Token] {
        let vec = GetSupportedSprimTypes()
        var result: [swiftusd.Token] = []
        for i in 0..<vec.size() {
            result.append(vec[i])
        }
        return result
    }

    /// Get supported Bprim types as an array of Tokens.
    public var supportedBprimTypes: [swiftusd.Token] {
        let vec = GetSupportedBprimTypes()
        var result: [swiftusd.Token] = []
        for i in 0..<vec.size() {
            result.append(vec[i])
        }
        return result
    }

    /// Get material render contexts as an array.
    public var materialRenderContexts: [swiftusd.Token] {
        let vec = GetMaterialRenderContexts()
        var result: [swiftusd.Token] = []
        for i in 0..<vec.size() {
            result.append(vec[i])
        }
        return result
    }

    /// Get shader source types as an array.
    public var shaderSourceTypes: [swiftusd.Token] {
        let vec = GetShaderSourceTypes()
        var result: [swiftusd.Token] = []
        for i in 0..<vec.size() {
            result.append(vec[i])
        }
        return result
    }

    /// Set the driver for this render delegate.
    public func setDrivers(_ driver: swiftusd.HdDriverRef) {
        SetDrivers(driver)
    }

    /// Check if primvar filtering is required.
    public var isPrimvarFilteringNeeded: Bool {
        return IsPrimvarFilteringNeeded()
    }
}

// MARK: - HdStResourceRegistryRef Extensions

extension swiftusd.HdStResourceRegistryRef {
    /// Create a new resource registry.
    public static func create(hgi: swiftusd.HgiRef) -> swiftusd.HdStResourceRegistryRef? {
        return swiftusd.HdStResourceRegistryRef.Create(hgi)
    }

    /// Commit pending resources to the GPU.
    public func commit() {
        Commit()
    }

    /// Garbage collect unreferenced resources.
    public func garbageCollect() {
        GarbageCollect()
    }

    /// Invalidate all shader programs.
    public func invalidateShaders() {
        InvalidateShaderRegistry()
    }

    /// Set memory limit for a texture type.
    public func setMemoryRequest(for textureType: swiftusd.HdStTextureType, bytes: Int) {
        SetMemoryRequestForTextureType(textureType, bytes)
    }

    /// Get resource allocation summary.
    public var allocationSummary: String {
        return String(cString: GetResourceAllocationSummary())
    }
}

// MARK: - HdStMeshTopologyInfo Extensions

extension swiftusd.HdStMeshTopologyInfo {
    /// Create mesh topology info with the given parameters.
    public static func make(
        numFaces: Int32,
        numPoints: Int32,
        numFaceVertices: Int32,
        isLeftHanded: Bool = false,
        scheme: swiftusd.Token = swiftusd.Token("none")
    ) -> swiftusd.HdStMeshTopologyInfo {
        return swiftusd.HdStMeshTopologyInfo(numFaces, numPoints, numFaceVertices, isLeftHanded, scheme)
    }

    /// Common subdivision schemes
    public static var catmullClark: swiftusd.Token { swiftusd.Token("catmullClark") }
    public static var loop: swiftusd.Token { swiftusd.Token("loop") }
    public static var bilinear: swiftusd.Token { swiftusd.Token("bilinear") }
    public static var noSubdivision: swiftusd.Token { swiftusd.Token("none") }
}

// MARK: - HdStMaterialBinding Extensions

extension swiftusd.HdStMaterialBinding {
    /// Create a material binding with the given path.
    public static func make(path: swiftusd.Path, tag: swiftusd.Token = swiftusd.Token()) -> swiftusd.HdStMaterialBinding {
        return swiftusd.HdStMaterialBinding(path, tag)
    }

    /// Check if the binding is valid.
    public var isValid: Bool {
        return IsValid()
    }
}

// MARK: - HdStRenderPassStateConfig Extensions

extension swiftusd.HdStRenderPassStateConfig {
    /// Create a default render pass state config.
    public static func makeDefault() -> swiftusd.HdStRenderPassStateConfig {
        return swiftusd.HdStRenderPassStateConfig()
    }
}

// MARK: - HdStAovDescriptor Extensions

extension swiftusd.HdStAovDescriptor {
    /// Create an AOV descriptor.
    public static func make(
        name: swiftusd.Token,
        format: swiftusd.Token,
        clearValue: swiftusd.Vec4f,
        multiSampled: Bool = false
    ) -> swiftusd.HdStAovDescriptor {
        return swiftusd.HdStAovDescriptor(name, format, clearValue, multiSampled)
    }

    /// Create a color AOV descriptor with default settings.
    public static func colorAov(clearColor: swiftusd.Vec4f = swiftusd.Vec4f(0, 0, 0, 1)) -> swiftusd.HdStAovDescriptor {
        return swiftusd.HdStAovDescriptor(
            swiftusd.HdStAovTokens.Color(),
            swiftusd.Token("float16Vec4"),
            clearColor,
            false
        )
    }

    /// Create a depth AOV descriptor with default settings.
    public static func depthAov() -> swiftusd.HdStAovDescriptor {
        return swiftusd.HdStAovDescriptor(
            swiftusd.HdStAovTokens.Depth(),
            swiftusd.Token("float32"),
            swiftusd.Vec4f(1, 1, 1, 1),
            false
        )
    }
}

// MARK: - Token Extensions for Storm

extension swiftusd.Token {
    // HdSt texture type tokens
    public static var uvTexture: swiftusd.Token { swiftusd.HdStTokens.UV() }
    public static var fieldTexture: swiftusd.Token { swiftusd.HdStTokens.Field() }
    public static var ptexTexture: swiftusd.Token { swiftusd.HdStTokens.Ptex() }
    public static var udimTexture: swiftusd.Token { swiftusd.HdStTokens.Udim() }

    // HdSt shader tokens
    public static var displacementShader: swiftusd.Token { swiftusd.HdStTokens.DisplacementShader() }
    public static var surfaceShader: swiftusd.Token { swiftusd.HdStTokens.SurfaceShader() }
    public static var volumeShader: swiftusd.Token { swiftusd.HdStTokens.VolumeShader() }

    // Material tag tokens
    public static var maskedMaterial: swiftusd.Token { swiftusd.HdStMaterialTagTokens.Masked() }
    public static var additiveMaterial: swiftusd.Token { swiftusd.HdStMaterialTagTokens.Additive() }
    public static var translucentMaterial: swiftusd.Token { swiftusd.HdStMaterialTagTokens.Translucent() }
    public static var volumeMaterial: swiftusd.Token { swiftusd.HdStMaterialTagTokens.Volume() }

    // AOV tokens
    public static var colorAov: swiftusd.Token { swiftusd.HdStAovTokens.Color() }
    public static var depthAov: swiftusd.Token { swiftusd.HdStAovTokens.Depth() }
    public static var cameraDepthAov: swiftusd.Token { swiftusd.HdStAovTokens.CameraDepth() }
    public static var primIdAov: swiftusd.Token { swiftusd.HdStAovTokens.PrimId() }
    public static var instanceIdAov: swiftusd.Token { swiftusd.HdStAovTokens.InstanceId() }
    public static var elementIdAov: swiftusd.Token { swiftusd.HdStAovTokens.ElementId() }
    public static var normalAov: swiftusd.Token { swiftusd.HdStAovTokens.Normal() }
}

// MARK: - HdStTextureType Helpers

extension swiftusd.HdStTextureType {
    /// Get the string name for this texture type.
    public var name: String {
        return String(cString: swiftusd.HdStGetTextureTypeName(self))
    }
}

// MARK: - Global Functions

/// Check if Storm is available on this system.
public func hdStIsSupported() -> Bool {
    return swiftusd.HdStIsSupported()
}
