// Hgi.swift - Hydra Graphics Interface wrapper
// Mirrors: pxr/imaging/hgi/*.h (GPU abstraction for rendering)

import OpenUSDInterop

// MARK: - Hgi Namespace

/// Hydra Graphics Interface - GPU abstraction layer.
/// Hgi provides a platform-independent interface for GPU resource management
/// and command recording. It supports OpenGL, Metal, and Vulkan backends.
///
/// ## Usage
/// ```swift
/// // Create the platform-default Hgi instance
/// let hgi = try Hgi()
///
/// // Create a buffer
/// let buffer = try hgi.createBuffer(
///     byteSize: 1024,
///     usage: .uniform
/// )
///
/// // Use blit commands to upload data
/// let blitCmds = try hgi.createBlitCmds()
/// blitCmds.copyBufferCpuToGpu(
///     destination: buffer,
///     data: myData
/// )
/// try hgi.submit(blitCmds)
/// ```
public final class Hgi: @unchecked Sendable {
    private let handle: HgiRef

    /// Creates the platform-default Hgi instance.
    /// On macOS this returns HgiMetal, on Linux HgiVulkan or HgiGL.
    public init() throws {
        guard let handle = Hgi_CreatePlatformDefaultHgi() else {
            throw HgiError.creationFailed("Failed to create platform default Hgi")
        }
        self.handle = handle
    }

    /// Creates a named Hgi instance.
    /// - Parameter type: The backend type ("OpenGL", "Metal", "Vulkan")
    public init(type: String) throws {
        guard let handle = Hgi_CreateNamedHgi(type) else {
            throw HgiError.creationFailed("Failed to create Hgi with type: \(type)")
        }
        self.handle = handle
    }

    deinit {
        Hgi_Release(handle)
    }

    /// The API name (e.g., "OpenGL", "Metal", "Vulkan", "Standalone").
    public var apiName: String {
        var namePtr: UnsafeMutablePointer<CChar>?
        guard Hgi_GetAPIName(handle, &namePtr) == USD_RESULT_SUCCESS,
              let name = namePtr else {
            return "Unknown"
        }
        defer { Hgi_FreeString(name) }
        return String(cString: name)
    }

    /// Returns true if the backend is supported on current hardware.
    public var isBackendSupported: Bool {
        Hgi_IsBackendSupported(handle)
    }

    /// Checks if a specific backend type is supported.
    public static func isSupported(type: String? = nil) -> Bool {
        Hgi_IsSupported(type)
    }

    /// Gets the device capabilities.
    public var capabilities: Capabilities {
        Capabilities(Hgi_GetCapabilities(handle))
    }

    /// Marks the start of a new rendering frame.
    public func startFrame() {
        Hgi_StartFrame(handle)
    }

    /// Marks the end of a rendering frame.
    public func endFrame() {
        Hgi_EndFrame(handle)
    }

    /// Performs garbage collection of destroyed resources.
    public func garbageCollect() {
        Hgi_GarbageCollect(handle)
    }

    // MARK: - Resource Creation

    /// Creates a GPU buffer.
    public func createBuffer(
        byteSize: Int,
        usage: BufferUsage,
        vertexStride: UInt32 = 0,
        debugName: String? = nil,
        initialData: UnsafeRawPointer? = nil
    ) throws -> Buffer {
        var desc = HgiBufferDesc(
            debugName: nil,
            usage: usage.rawValue,
            byteSize: byteSize,
            vertexStride: vertexStride,
            initialData: initialData
        )

        if let name = debugName {
            return try name.withCString { namePtr in
                desc.debugName = namePtr
                return try createBufferInternal(desc: &desc)
            }
        }
        return try createBufferInternal(desc: &desc)
    }

    private func createBufferInternal(desc: inout HgiBufferDesc) throws -> Buffer {
        guard let bufferHandle = Hgi_CreateBuffer(handle, &desc) else {
            throw HgiError.creationFailed("Failed to create buffer")
        }
        return Buffer(bufferHandle, hgi: self)
    }

    /// Creates a GPU texture.
    public func createTexture(
        width: Int32,
        height: Int32,
        depth: Int32 = 1,
        format: Format,
        type: TextureType = .texture2D,
        usage: TextureUsage = .shaderRead,
        layerCount: UInt16 = 1,
        mipLevels: UInt16 = 1,
        sampleCount: SampleCount = .count1,
        debugName: String? = nil,
        initialData: UnsafeRawPointer? = nil,
        pixelsByteSize: Int = 0
    ) throws -> Texture {
        var desc = HgiTextureDesc(
            debugName: nil,
            usage: usage.rawValue,
            format: HgiFormat(rawValue: format.rawValue),
            componentMapping: HgiComponentMapping(
                r: HGI_COMPONENT_SWIZZLE_R,
                g: HGI_COMPONENT_SWIZZLE_G,
                b: HGI_COMPONENT_SWIZZLE_B,
                a: HGI_COMPONENT_SWIZZLE_A
            ),
            type: HgiTextureType(rawValue: type.rawValue),
            width: width,
            height: height,
            depth: depth,
            layerCount: layerCount,
            mipLevels: mipLevels,
            sampleCount: HgiSampleCount(rawValue: sampleCount.rawValue),
            pixelsByteSize: pixelsByteSize,
            initialData: initialData
        )

        if let name = debugName {
            return try name.withCString { namePtr in
                desc.debugName = namePtr
                return try createTextureInternal(desc: &desc)
            }
        }
        return try createTextureInternal(desc: &desc)
    }

    private func createTextureInternal(desc: inout HgiTextureDesc) throws -> Texture {
        guard let textureHandle = Hgi_CreateTexture(handle, &desc) else {
            throw HgiError.creationFailed("Failed to create texture")
        }
        return Texture(textureHandle, hgi: self)
    }

    /// Creates a GPU sampler.
    public func createSampler(
        magFilter: SamplerFilter = .linear,
        minFilter: SamplerFilter = .linear,
        mipFilter: MipFilter = .linear,
        addressModeU: SamplerAddressMode = .clampToEdge,
        addressModeV: SamplerAddressMode = .clampToEdge,
        addressModeW: SamplerAddressMode = .clampToEdge,
        borderColor: BorderColor = .transparentBlack,
        enableCompare: Bool = false,
        compareFunction: CompareFunction = .never,
        maxAnisotropy: UInt32 = 16,
        debugName: String? = nil
    ) throws -> Sampler {
        var desc = HgiSamplerDesc(
            debugName: nil,
            magFilter: HgiSamplerFilter(rawValue: magFilter.rawValue),
            minFilter: HgiSamplerFilter(rawValue: minFilter.rawValue),
            mipFilter: HgiMipFilter(rawValue: mipFilter.rawValue),
            addressModeU: HgiSamplerAddressMode(rawValue: addressModeU.rawValue),
            addressModeV: HgiSamplerAddressMode(rawValue: addressModeV.rawValue),
            addressModeW: HgiSamplerAddressMode(rawValue: addressModeW.rawValue),
            borderColor: HgiBorderColor(rawValue: borderColor.rawValue),
            enableCompare: enableCompare,
            compareFunction: HgiCompareFunction(rawValue: compareFunction.rawValue),
            maxAnisotropy: maxAnisotropy
        )

        if let name = debugName {
            return try name.withCString { namePtr in
                desc.debugName = namePtr
                return try createSamplerInternal(desc: &desc)
            }
        }
        return try createSamplerInternal(desc: &desc)
    }

    private func createSamplerInternal(desc: inout HgiSamplerDesc) throws -> Sampler {
        guard let samplerHandle = Hgi_CreateSampler(handle, &desc) else {
            throw HgiError.creationFailed("Failed to create sampler")
        }
        return Sampler(samplerHandle, hgi: self)
    }

    /// Creates a shader function.
    public func createShaderFunction(
        stage: ShaderStage,
        code: String,
        debugName: String? = nil
    ) throws -> ShaderFunction {
        return try code.withCString { codePtr in
            var desc = HgiShaderFunctionDesc(
                debugName: nil,
                shaderStage: stage.rawValue,
                shaderCode: codePtr,
                shaderCodeSize: code.utf8.count
            )

            if let name = debugName {
                return try name.withCString { namePtr in
                    desc.debugName = namePtr
                    return try createShaderFunctionInternal(desc: &desc)
                }
            }
            return try createShaderFunctionInternal(desc: &desc)
        }
    }

    private func createShaderFunctionInternal(desc: inout HgiShaderFunctionDesc) throws -> ShaderFunction {
        guard let funcHandle = Hgi_CreateShaderFunction(handle, &desc) else {
            throw HgiError.creationFailed("Failed to create shader function")
        }
        return ShaderFunction(funcHandle, hgi: self)
    }

    /// Creates a shader program from shader functions.
    public func createShaderProgram(
        shaderFunctions: [ShaderFunction],
        debugName: String? = nil
    ) throws -> ShaderProgram {
        var funcHandles: [HgiShaderFunctionRef?] = shaderFunctions.map { $0.handle }

        return try funcHandles.withUnsafeMutableBufferPointer { buffer in
            var desc = HgiShaderProgramDesc(
                debugName: nil,
                shaderFunctions: buffer.baseAddress,
                shaderFunctionCount: shaderFunctions.count
            )

            if let name = debugName {
                return try name.withCString { namePtr in
                    desc.debugName = namePtr
                    return try createShaderProgramInternal(desc: &desc)
                }
            }
            return try createShaderProgramInternal(desc: &desc)
        }
    }

    private func createShaderProgramInternal(desc: inout HgiShaderProgramDesc) throws -> ShaderProgram {
        guard let progHandle = Hgi_CreateShaderProgram(handle, &desc) else {
            throw HgiError.creationFailed("Failed to create shader program")
        }
        return ShaderProgram(progHandle, hgi: self)
    }

    // MARK: - Command Recording

    /// Creates a blit command encoder for data transfers.
    public func createBlitCmds() throws -> BlitCmds {
        guard let cmdsHandle = Hgi_CreateBlitCmds(handle) else {
            throw HgiError.creationFailed("Failed to create blit commands")
        }
        return BlitCmds(cmdsHandle)
    }

    /// Submits commands to the GPU.
    public func submit(_ cmds: BlitCmds, wait: SubmitWaitType = .noWait) throws {
        let result = Hgi_SubmitCmds(handle, UnsafeMutableRawPointer(cmds.handle), HgiSubmitWaitType(rawValue: wait.rawValue))
        guard result == USD_RESULT_SUCCESS else {
            throw HgiError.submissionFailed("Failed to submit commands")
        }
    }

    // MARK: - Internal

    /// Internal access to the C handle for resource cleanup.
    internal var cHandle: HgiRef { handle }
}

// MARK: - Capabilities

extension Hgi {
    /// Device capabilities information.
    public struct Capabilities: @unchecked Sendable {
        private let handle: HgiCapabilitiesRef?

        internal init(_ handle: HgiCapabilitiesRef?) {
            self.handle = handle
        }

        /// Checks if a capability flag is set.
        public func isSet(_ capability: DeviceCapability) -> Bool {
            guard let handle = handle else { return false }
            return HgiCapabilities_IsSet(handle, capability.rawValue)
        }

        /// The API version.
        public var apiVersion: Int {
            guard let handle = handle else { return 0 }
            return Int(HgiCapabilities_GetAPIVersion(handle))
        }

        /// The shader version.
        public var shaderVersion: Int {
            guard let handle = handle else { return 0 }
            return Int(HgiCapabilities_GetShaderVersion(handle))
        }

        /// Maximum uniform block size.
        public var maxUniformBlockSize: Int {
            guard let handle = handle else { return 0 }
            return HgiCapabilities_GetMaxUniformBlockSize(handle)
        }

        /// Maximum shader storage block size.
        public var maxShaderStorageBlockSize: Int {
            guard let handle = handle else { return 0 }
            return HgiCapabilities_GetMaxShaderStorageBlockSize(handle)
        }

        /// Uniform buffer offset alignment.
        public var uniformBufferOffsetAlignment: Int {
            guard let handle = handle else { return 0 }
            return HgiCapabilities_GetUniformBufferOffsetAlignment(handle)
        }

        /// Maximum number of clip distances.
        public var maxClipDistances: Int {
            guard let handle = handle else { return 0 }
            return HgiCapabilities_GetMaxClipDistances(handle)
        }
    }
}

// MARK: - Buffer

extension Hgi {
    /// A GPU buffer resource.
    public final class Buffer: @unchecked Sendable {
        internal let handle: HgiBufferRef
        private weak var hgi: Hgi?

        internal init(_ handle: HgiBufferRef, hgi: Hgi) {
            self.handle = handle
            self.hgi = hgi
        }

        deinit {
            var mutableHandle: HgiBufferRef? = handle
            if let hgi = hgi {
                Hgi_DestroyBuffer(hgi.cHandle, &mutableHandle)
            }
        }

        /// The buffer byte size.
        public var byteSizeOfResource: Int {
            HgiBuffer_GetByteSizeOfResource(handle)
        }

        /// The buffer byte size from descriptor.
        public var byteSize: Int {
            HgiBuffer_GetByteSize(handle)
        }

        /// The buffer usage flags.
        public var usage: BufferUsage {
            BufferUsage(rawValue: HgiBuffer_GetUsage(handle))
        }

        /// Raw GPU resource handle (platform-specific).
        public var rawResource: UInt64 {
            HgiBuffer_GetRawResource(handle)
        }

        /// CPU staging address for uploads (if available).
        public var cpuStagingAddress: UnsafeMutableRawPointer? {
            HgiBuffer_GetCPUStagingAddress(handle)
        }
    }
}

// MARK: - Texture

extension Hgi {
    /// A GPU texture resource.
    public final class Texture: @unchecked Sendable {
        internal let handle: HgiTextureRef
        private weak var hgi: Hgi?

        internal init(_ handle: HgiTextureRef, hgi: Hgi) {
            self.handle = handle
            self.hgi = hgi
        }

        deinit {
            var mutableHandle: HgiTextureRef? = handle
            if let hgi = hgi {
                Hgi_DestroyTexture(hgi.cHandle, &mutableHandle)
            }
        }

        /// The texture byte size.
        public var byteSizeOfResource: Int {
            HgiTexture_GetByteSizeOfResource(handle)
        }

        /// The texture format.
        public var format: Format {
            Format(rawValue: HgiTexture_GetFormat(handle).rawValue) ?? .invalid
        }

        /// The texture type.
        public var type: TextureType {
            TextureType(rawValue: HgiTexture_GetType(handle).rawValue) ?? .texture2D
        }

        /// The texture dimensions.
        public var dimensions: (width: Int32, height: Int32, depth: Int32) {
            var w: Int32 = 0, h: Int32 = 0, d: Int32 = 0
            HgiTexture_GetDimensions(handle, &w, &h, &d)
            return (w, h, d)
        }

        /// The layer count.
        public var layerCount: UInt16 {
            HgiTexture_GetLayerCount(handle)
        }

        /// The mip level count.
        public var mipLevels: UInt16 {
            HgiTexture_GetMipLevels(handle)
        }

        /// The sample count.
        public var sampleCount: SampleCount {
            SampleCount(rawValue: HgiTexture_GetSampleCount(handle).rawValue) ?? .count1
        }

        /// Raw GPU resource handle (platform-specific).
        public var rawResource: UInt64 {
            HgiTexture_GetRawResource(handle)
        }
    }
}

// MARK: - Sampler

extension Hgi {
    /// A GPU sampler resource.
    public final class Sampler: @unchecked Sendable {
        internal let handle: HgiSamplerRef
        private weak var hgi: Hgi?

        internal init(_ handle: HgiSamplerRef, hgi: Hgi) {
            self.handle = handle
            self.hgi = hgi
        }

        deinit {
            var mutableHandle: HgiSamplerRef? = handle
            if let hgi = hgi {
                Hgi_DestroySampler(hgi.cHandle, &mutableHandle)
            }
        }

        /// Raw GPU resource handle (platform-specific).
        public var rawResource: UInt64 {
            HgiSampler_GetRawResource(handle)
        }
    }
}

// MARK: - ShaderFunction

extension Hgi {
    /// A shader function (single shader stage).
    public final class ShaderFunction: @unchecked Sendable {
        internal let handle: HgiShaderFunctionRef
        private weak var hgi: Hgi?

        internal init(_ handle: HgiShaderFunctionRef, hgi: Hgi) {
            self.handle = handle
            self.hgi = hgi
        }

        deinit {
            var mutableHandle: HgiShaderFunctionRef? = handle
            if let hgi = hgi {
                Hgi_DestroyShaderFunction(hgi.cHandle, &mutableHandle)
            }
        }

        /// Returns true if the shader compiled successfully.
        public var isValid: Bool {
            HgiShaderFunction_IsValid(handle)
        }

        /// Shader compile errors (if any).
        public var compileErrors: String {
            var errorPtr: UnsafeMutablePointer<CChar>?
            guard HgiShaderFunction_GetCompileErrors(handle, &errorPtr) == USD_RESULT_SUCCESS,
                  let errors = errorPtr else {
                return ""
            }
            defer { Hgi_FreeString(errors) }
            return String(cString: errors)
        }

        /// The shader byte size.
        public var byteSizeOfResource: Int {
            HgiShaderFunction_GetByteSizeOfResource(handle)
        }

        /// Raw GPU resource handle (platform-specific).
        public var rawResource: UInt64 {
            HgiShaderFunction_GetRawResource(handle)
        }
    }
}

// MARK: - ShaderProgram

extension Hgi {
    /// A shader program (linked shader stages).
    public final class ShaderProgram: @unchecked Sendable {
        internal let handle: HgiShaderProgramRef
        private weak var hgi: Hgi?

        internal init(_ handle: HgiShaderProgramRef, hgi: Hgi) {
            self.handle = handle
            self.hgi = hgi
        }

        deinit {
            var mutableHandle: HgiShaderProgramRef? = handle
            if let hgi = hgi {
                Hgi_DestroyShaderProgram(hgi.cHandle, &mutableHandle)
            }
        }

        /// Returns true if the program linked successfully.
        public var isValid: Bool {
            HgiShaderProgram_IsValid(handle)
        }

        /// Shader program link errors (if any).
        public var compileErrors: String {
            var errorPtr: UnsafeMutablePointer<CChar>?
            guard HgiShaderProgram_GetCompileErrors(handle, &errorPtr) == USD_RESULT_SUCCESS,
                  let errors = errorPtr else {
                return ""
            }
            defer { Hgi_FreeString(errors) }
            return String(cString: errors)
        }

        /// The program byte size.
        public var byteSizeOfResource: Int {
            HgiShaderProgram_GetByteSizeOfResource(handle)
        }

        /// Raw GPU resource handle (platform-specific).
        public var rawResource: UInt64 {
            HgiShaderProgram_GetRawResource(handle)
        }
    }
}

// MARK: - BlitCmds

extension Hgi {
    /// Blit command encoder for data transfers.
    public final class BlitCmds: @unchecked Sendable {
        internal let handle: HgiBlitCmdsRef

        internal init(_ handle: HgiBlitCmdsRef) {
            self.handle = handle
        }

        deinit {
            HgiBlitCmds_Release(handle)
        }

        /// Returns true if the commands have been submitted.
        public var isSubmitted: Bool {
            HgiBlitCmds_IsSubmitted(handle)
        }

        /// Copies data from CPU to a GPU buffer.
        public func copyBufferCpuToGpu(
            destination: Buffer,
            destinationOffset: Int = 0,
            data: UnsafeRawPointer,
            byteSize: Int
        ) {
            HgiBlitCmds_CopyBufferCpuToGpu(
                handle,
                destination.handle,
                destinationOffset,
                data,
                byteSize
            )
        }

        /// Copies data from CPU to a GPU buffer using Data.
        public func copyBufferCpuToGpu(
            destination: Buffer,
            destinationOffset: Int = 0,
            data: Data
        ) {
            data.withUnsafeBytes { rawBuffer in
                if let ptr = rawBuffer.baseAddress {
                    HgiBlitCmds_CopyBufferCpuToGpu(
                        handle,
                        destination.handle,
                        destinationOffset,
                        ptr,
                        data.count
                    )
                }
            }
        }

        /// Copies data from GPU buffer to CPU.
        public func copyBufferGpuToCpu(
            source: Buffer,
            sourceOffset: Int = 0,
            destination: UnsafeMutableRawPointer,
            byteSize: Int
        ) {
            HgiBlitCmds_CopyBufferGpuToCpu(
                handle,
                source.handle,
                sourceOffset,
                destination,
                byteSize
            )
        }

        /// Copies data from one GPU buffer to another.
        public func copyBufferGpuToGpu(
            source: Buffer,
            sourceOffset: Int = 0,
            destination: Buffer,
            destinationOffset: Int = 0,
            byteSize: Int
        ) {
            HgiBlitCmds_CopyBufferGpuToGpu(
                handle,
                source.handle,
                sourceOffset,
                destination.handle,
                destinationOffset,
                byteSize
            )
        }

        /// Copies data from CPU to a GPU texture.
        public func copyTextureCpuToGpu(
            destination: Texture,
            mipLevel: UInt16 = 0,
            layer: UInt16 = 0,
            data: UnsafeRawPointer,
            byteSize: Int
        ) {
            HgiBlitCmds_CopyTextureCpuToGpu(
                handle,
                destination.handle,
                mipLevel,
                layer,
                data,
                byteSize
            )
        }

        /// Generates mipmaps for a texture.
        public func generateMipMaps(texture: Texture) {
            HgiBlitCmds_GenerateMipMaps(handle, texture.handle)
        }

        /// Fills a buffer with a constant value.
        public func fillBuffer(_ buffer: Buffer, value: UInt8) {
            HgiBlitCmds_FillBuffer(handle, buffer.handle, value)
        }
    }
}

// MARK: - Enums

extension Hgi {
    /// Pixel/vertex format.
    public enum Format: Int32, Sendable {
        case invalid = -1
        case unorm8 = 0
        case unorm8Vec2
        case unorm8Vec4
        case snorm8
        case snorm8Vec2
        case snorm8Vec4
        case float16
        case float16Vec2
        case float16Vec3
        case float16Vec4
        case float32
        case float32Vec2
        case float32Vec3
        case float32Vec4
        case int16
        case int16Vec2
        case int16Vec3
        case int16Vec4
        case uint16
        case uint16Vec2
        case uint16Vec3
        case uint16Vec4
        case int32
        case int32Vec2
        case int32Vec3
        case int32Vec4
        case unorm8Vec4Srgb
        case bc6FloatVec3
        case bc6UfloatVec3
        case bc7Unorm8Vec4
        case bc7Unorm8Vec4Srgb
        case bc1Unorm8Vec4
        case bc3Unorm8Vec4
        case float32Uint8
        case packedInt1010102

        /// Number of components in the format.
        public var componentCount: Int {
            HgiFormat_GetComponentCount(HgiFormat(rawValue: rawValue))
        }

        /// Byte size of a single element.
        public var dataSize: Int {
            HgiFormat_GetDataSize(HgiFormat(rawValue: rawValue))
        }

        /// Whether the format is compressed.
        public var isCompressed: Bool {
            HgiFormat_IsCompressed(HgiFormat(rawValue: rawValue))
        }

        /// Whether the format is floating-point.
        public var isFloat: Bool {
            HgiFormat_IsFloat(HgiFormat(rawValue: rawValue))
        }
    }

    /// Texture type.
    public enum TextureType: UInt32, Sendable {
        case texture1D = 0
        case texture2D
        case texture3D
        case cubemap
        case texture1DArray
        case texture2DArray
    }

    /// Texture usage flags.
    public struct TextureUsage: OptionSet, Sendable {
        public let rawValue: UInt32

        public init(rawValue: UInt32) {
            self.rawValue = rawValue
        }

        public static let colorTarget = TextureUsage(rawValue: 1 << 0)
        public static let depthTarget = TextureUsage(rawValue: 1 << 1)
        public static let stencilTarget = TextureUsage(rawValue: 1 << 2)
        public static let shaderRead = TextureUsage(rawValue: 1 << 3)
        public static let shaderWrite = TextureUsage(rawValue: 1 << 4)
    }

    /// Buffer usage flags.
    public struct BufferUsage: OptionSet, Sendable {
        public let rawValue: UInt32

        public init(rawValue: UInt32) {
            self.rawValue = rawValue
        }

        public static let uniform = BufferUsage(rawValue: 1 << 0)
        public static let index32 = BufferUsage(rawValue: 1 << 1)
        public static let vertex = BufferUsage(rawValue: 1 << 2)
        public static let storage = BufferUsage(rawValue: 1 << 3)
        public static let indirect = BufferUsage(rawValue: 1 << 4)
    }

    /// Sample count for multi-sampling.
    public enum SampleCount: UInt32, Sendable {
        case count1 = 1
        case count2 = 2
        case count4 = 4
        case count8 = 8
        case count16 = 16
    }

    /// Sampler filter mode.
    public enum SamplerFilter: UInt32, Sendable {
        case nearest = 0
        case linear = 1
    }

    /// Mip filter mode.
    public enum MipFilter: UInt32, Sendable {
        case notMipmapped = 0
        case nearest = 1
        case linear = 2
    }

    /// Sampler address mode.
    public enum SamplerAddressMode: UInt32, Sendable {
        case clampToEdge = 0
        case mirrorClampToEdge
        case `repeat`
        case mirrorRepeat
        case clampToBorder
    }

    /// Border color for clamped texture values.
    public enum BorderColor: UInt32, Sendable {
        case transparentBlack = 0
        case opaqueBlack = 1
        case opaqueWhite = 2
    }

    /// Compare function for depth/stencil testing.
    public enum CompareFunction: UInt32, Sendable {
        case never = 0
        case less
        case equal
        case lessEqual
        case greater
        case notEqual
        case greaterEqual
        case always
    }

    /// Shader stage.
    public struct ShaderStage: OptionSet, Sendable {
        public let rawValue: UInt32

        public init(rawValue: UInt32) {
            self.rawValue = rawValue
        }

        public static let vertex = ShaderStage(rawValue: 1 << 0)
        public static let fragment = ShaderStage(rawValue: 1 << 1)
        public static let compute = ShaderStage(rawValue: 1 << 2)
        public static let tessellationControl = ShaderStage(rawValue: 1 << 3)
        public static let tessellationEval = ShaderStage(rawValue: 1 << 4)
        public static let geometry = ShaderStage(rawValue: 1 << 5)
    }

    /// Submit wait type.
    public enum SubmitWaitType: UInt32, Sendable {
        case noWait = 0
        case waitUntilCompleted
    }

    /// Device capabilities.
    public struct DeviceCapability: OptionSet, Sendable {
        public let rawValue: UInt32

        public init(rawValue: UInt32) {
            self.rawValue = rawValue
        }

        public static let presentation = DeviceCapability(rawValue: 1 << 0)
        public static let bindlessBuffers = DeviceCapability(rawValue: 1 << 1)
        public static let concurrentDispatch = DeviceCapability(rawValue: 1 << 2)
        public static let unifiedMemory = DeviceCapability(rawValue: 1 << 3)
        public static let builtinBarycentrics = DeviceCapability(rawValue: 1 << 4)
        public static let shaderDrawParameters = DeviceCapability(rawValue: 1 << 5)
        public static let multiDrawIndirect = DeviceCapability(rawValue: 1 << 6)
        public static let bindlessTextures = DeviceCapability(rawValue: 1 << 7)
        public static let shaderDoublePrecision = DeviceCapability(rawValue: 1 << 8)
        public static let conservativeRaster = DeviceCapability(rawValue: 1 << 11)
        public static let indirectCommandBuffers = DeviceCapability(rawValue: 1 << 17)
    }
}

// MARK: - Error Type

/// Errors that can occur in Hgi operations.
public enum HgiError: Error, CustomStringConvertible {
    case creationFailed(String)
    case submissionFailed(String)
    case compilationFailed(String)
    case invalidOperation(String)

    public var description: String {
        switch self {
        case .creationFailed(let message):
            return "Hgi creation failed: \(message)"
        case .submissionFailed(let message):
            return "Hgi submission failed: \(message)"
        case .compilationFailed(let message):
            return "Hgi compilation failed: \(message)"
        case .invalidOperation(let message):
            return "Hgi invalid operation: \(message)"
        }
    }
}

// Import Foundation for Data support
import Foundation
